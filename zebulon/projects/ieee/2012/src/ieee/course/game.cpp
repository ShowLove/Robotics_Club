#include "ieee/course/game.h"

using namespace Zebulon;
using namespace IEEE;

Game::Game()
{

}

Game::~Game()
{
    Shutdown();
}

void Game::Shutdown()
{
    mGameThread.StopThread();
    mServerThread.StopThread();
    mLogThread.StopThread(10000);
}

bool Game::Initialize(int index, std::string usbport, std::string server, int dataport, int imageport, bool judgeOverride)
{
    mCameraIndex = index;

    mServerName = server;
    mDataPort = dataport;
    mImagePort = imageport;

    mJudgeOverride = judgeOverride;

    Configuration::XmlConfigurator config;
    config.Load("settings/ieeevision.xml", "IEEEVision");
    mGameLogic.Initialize(&config);

    mSender.Initialize(mServerName, mDataPort);
    mImageSender.Initialize(mServerName, mImagePort, 75);

    char buffer[50];
    sprintf(buffer, "Camera %d", mCameraIndex);
    mWindowName = std::string(buffer);

    mGameThread.CreateThread(GameThreadCall, this);
    mServerThread.CreateThread(ServerThreadCall, this);
    mLogThread.CreateThread(LogThreadCall, this);

    std::cout << mGameThread.GetThreadID() << " " << mGameThread.GetThreadName() << std::endl;
    std::cout << mServerThread.GetThreadID() << " " << mServerThread.GetThreadName() << std::endl;

    cv::namedWindow(mWindowName);

    sprintf(buffer, "(%d) Start Run", mCameraIndex);
    cv::createButton(std::string(buffer), Game::buttonPressed_StartRun, this);

    sprintf(buffer, "(%d) STOP Run", mCameraIndex);
    cv::createButton(std::string(buffer), Game::buttonPressed_StopRun, this);

    if (mJudgeOverride)
    {
        sprintf(buffer, "(%d) Randomize", mCameraIndex);
        cv::createButton(std::string(buffer), Game::buttonPressed_RandomizeCourse, this);
    }

    /*sprintf(buffer, "(%d) New Run", mCameraIndex);
    cv::createButton(std::string(buffer), Game::buttonPressed_NewRun, this);//*/

    sprintf(buffer, "(%d) Team ID", mCameraIndex);
    mSelectedID = 0;
    cv::createTrackbar(std::string(buffer), "", &mSelectedID, 54);

    sprintf(buffer, "(%d) Run Number", mCameraIndex);
    mSelectedRunNumber = 0;
    cv::createTrackbar(std::string(buffer), "", &mSelectedRunNumber, 3);

    //sprintf(buffer,"/dev/ttyUSB%d",mCameraIndex);
    mMCU.Initialize(usbport,9600);

    return true;
}

void Game::GameThreadCall(void* args)
{
    Game* game = static_cast<Game*>(args);

    if (game != NULL)
        game->GameThread();
}

void Game::GameThread()
{
    cv::VideoCapture capture(mCameraIndex);
    capture.set(CV_CAP_PROP_CONVERT_RGB, false);
    //Laptop can't keep frame rate, lowered to 5 fps
    capture.set(CV_CAP_PROP_FPS, 7);

    cv::Mat frame;
    //cv::Mat out = cv::Mat(cv::Size(640,480), CV_8UC1);
    cv::Mat out = cv::Mat(480, 640, CV_8UC1);

    std::vector< Vision::Return > returns;
    IEEEVisionCassy vision;

    int i=0;
    while (!mGameThread.QuitThreadFlag())
    {
        capture >> frame;

        mFrameMutex.Lock();
        frame.copyTo(mCurrentFrame);
        mFrameMutex.Unlock();

        vision.backgroundSubtraction(frame, out);
        vision.backgroundSubtractionCleanUp(out, out);
        vision.segmentation(out, out);

        returns = vision.GetSegments();

        i=0;
        mMutex.Lock();
        std::vector< Vision::Return >::iterator iter;
        for (iter=returns.begin();
             iter != returns.end() && i < 16;
             iter++, i++)
        {
            mXList[i] = iter->mCenterI;
            mYList[i] = iter->mCenterJ;
        }

        mNumPoints = i;
        //std::cout << "Num Points " << mNumPoints << std::endl;
        mMutex.Unlock();

        mGameLogic.Update(returns, mJudgeOverride);

        //mGameLogic.DrawDebug(out);

        CxUtils::SleepMs(20);
    }
}

void Game::ServerThreadCall(void* args)
{
    Game* game = static_cast<Game*>(args);

    if (game != NULL)
        game->ServerThread();
}

void Game::ServerThread()
{
    //cv::Mat frame;
    while (!mServerThread.QuitThreadFlag())
    {
        if (mGameLogic.IsRunning())
        {
            mFrameMutex.Lock();
            if (mCurrentFrame.rows)
                mImageSender.SendImage(mCurrentFrame);
            mFrameMutex.Unlock();

            GamePacket packet;

            mMutex.Lock();
            if (mNumPoints <= 16 && mNumPoints > 0)
            {
                packet.mNumPoints = mNumPoints;
                memcpy(packet.mXList, mXList, sizeof(float)*mNumPoints);
                memcpy(packet.mYList, mYList, sizeof(float)*mNumPoints);
            }
            else
            {
                packet.mNumPoints = 0;
            }
            packet.mTeamID = mTeamID;
            //std::cout << mTeamID << " . " << packet.mNumPoints << std::endl;
            mMutex.Unlock();

            Tasks::Stats stats = mGameLogic.GetStats();

            //std::cout << mTeamID << stats.toString() << std::endl;

            if (!mJudgeOverride)
            {
                packet.mCorrect = stats.mCorrect;
                packet.mIncorrect = stats.mIncorrect;
                packet.mLaps = stats.mLaps;
                packet.mTimeMs = stats.mTimeMs;
                packet.mScore = stats.mScore;
            }


            //mTrue[4];
            //mMeasured[4]
            CourseConfig cc;
            if (mJudgeOverride)
            {
                mJudgeCCMutex.Lock();
                cc = mJudgeCourseConfig;
                mJudgeCCMutex.Unlock();
            }
            else
                mGameLogic.GetCurrentLap(cc);


            memcpy(packet.mCourseMoves.mTrue, cc.CorrectTask, 4*sizeof(int));
            if (mJudgeOverride)
            {
                packet.mCourseMoves.mMeasured[0] = -1;
                packet.mCourseMoves.mMeasured[1] = -1;
                packet.mCourseMoves.mMeasured[2] = -1;
                packet.mCourseMoves.mMeasured[3] = -1;
            }
            else
                memcpy(packet.mCourseMoves.mMeasured, cc.TraversedTask, 4*sizeof(int));

            mSender.SendData(packet);

            //Update the MCU
            MCU::SetDigitalOutputs DO;
            DO.mUseStaticFormatFlag=false;
            DO.mPort=0;
            DO.mValue=0;
            //printf("Tasks:");
            for(int i=0;i<4;i++)
            {
                DO.mValue|=(cc.CorrectTask[i]%2)<<(7-i);
                //printf(" %d",(cc.CorrectTask[i]%2));
            }
            //printf(" : %0X",(unsigned int)DO.mValue);
            //printf("\n");
            mMCU.Send(&DO);


        }

        CxUtils::SleepMs(150);
    }
}

void Game::LogThreadCall(void* args)
{
    Game* game = static_cast<Game*>(args);

    if (game != NULL)
        game->LogThread();
}

void Game::LogThread()
{
    bool recorded=true;

    int lastID;

    char buffer[256];

    sprintf(buffer, "whoami");
    //printf("CMD: %s\n",buffer);
    FILE* whof = popen(buffer, "r");
    fgets(buffer, 18, whof);
    std::string username = std::string(buffer);
    username = username.substr(0, username.size()-1);
    pclose(whof);

    cv::Mat frame;
    while (!mLogThread.QuitThreadFlag())
    {
        if (mGameLogic.IsRunning())
        {
            recorded = false;

            mFrameMutex.Lock();
            if (mCurrentFrame.rows)
                mCurrentFrame.copyTo(frame);
            mFrameMutex.Unlock();

            mMutex.Lock();
            Tasks::Stats stats = mGameLogic.GetStats();
            lastID = mTeamID;
            mMutex.Unlock();

            sprintf(buffer, "mkdir -p /home/%s/software/ieeedata/%d/%d/", username.c_str(), lastID, mRunNumber);
            system(buffer);

            sprintf(buffer, "/home/%s/software/ieeedata/%d/%d/%d.jpg", username.c_str(), lastID, mRunNumber, stats.mTimeMs);
            cv::imwrite(buffer, frame);
        }
        else
        {
            if (!recorded)
            {
                mMutex.Lock();
                Record(mTeamID);
                recorded = true;
                mMutex.Unlock();
            }
        }

        CxUtils::SleepMs(150);
    }

    if (!recorded)
    {
        mMutex.Lock();
        Record(mTeamID);
        mMutex.Unlock();
    }
}

bool Game::Record(int id)
{
    static char buffer[128];

    sprintf(buffer, "whoami");
    //printf("CMD: %s\n",buffer);
    FILE* whof = popen(buffer, "r");
    fgets(buffer, 18, whof);
    std::string username = std::string(buffer);
    username = username.substr(0, username.size()-1);
    pclose(whof);

    // Data File
    if (!mJudgeOverride)
    {
        std::string lastDataName;

        sprintf(buffer, "/home/%s/software/ieeedata/%d/data%d.csv", username.c_str(), id, mRunNumber);
        lastDataName = buffer;
        mDataFile.open(buffer, std::ios::out);

        Tasks::Stats stats = mGameLogic.GetStats();

        // score,correct,incorrect,laps,time
        mDataFile << stats.mScore << ',' << stats.mCorrect << ','
                  << stats.mIncorrect << ',' << stats.mLaps << ','
                  << stats.mTimeMs << '\n';

        mDataFile.close();

        /*sprintf(buffer, "svn add %s", lastDataName.c_str());
        system(buffer);

        sprintf(buffer, "svn commit -m \"Team: %d Run: %d\" %s", id, mRunNumber, lastDataName.c_str());
        system(buffer);//*/

        sprintf(buffer, "svn add %s 2>&1", lastDataName.c_str());
        //printf("CMD: %s\n",buffer);
        FILE* datafa = popen(buffer, "r");
        fgets(buffer, 18, datafa);
        //printf("RES: %s\n",buffer);
        pclose(datafa);

        sprintf(buffer, "svn commit -m \"Team: %d Run: %d\" %s 2>&1", id, mRunNumber, lastDataName.c_str());
        //printf("CMD: %s\n",buffer);
        FILE* datafc = popen(buffer, "r");
        fgets(buffer, 18, datafc);
        pclose(datafc);

        //printf("RES: %s\n",buffer);
        if (strcmp(buffer, "svn: Commit failed")==0)
        {
            std::cout << "Error writing data file, check connection to score server. Failure on commit." << std::endl;
        }
    }

    // Raw File
    std::string lastRawName;

    sprintf(buffer, "/home/%s/software/ieeedata/%d/raw%d.csv", username.c_str(), id, mRunNumber);
    lastRawName = buffer;
    mRawFile.open(buffer, std::ios::out);

    std::vector<CourseConfig> configs = mGameLogic.GetCourseHistory();
    std::vector<CourseConfig>::iterator cc;
    for (cc=configs.begin(); cc!=configs.end(); cc++)
    {
        //true,measured,time
        for (int k=0; k<4; k++)
            mRawFile << cc->CorrectTask[k] << ','
                     << cc->TraversedTask[k] << ','
                     << id << '\n';
    }

    CourseConfig _cc;
    mGameLogic.GetCurrentLap(_cc);
    for (int k=0; k<4; k++)
        mRawFile << _cc.CorrectTask[k] << ','
                 << _cc.TraversedTask[k] << ','
                 << id << '\n';

    mRawFile.close();

    /*sprintf(buffer, "svn add %s", lastRawName.c_str());
    system(buffer);

    sprintf(buffer, "svn commit -m \"Team: %d Run: %d\" %s", id, mRunNumber, lastRawName.c_str());
    system(buffer);//*/

    sprintf(buffer, "svn add %s  2>&1", lastRawName.c_str());
    //printf("CMD: %s\n",buffer);
    FILE* rawfa = popen(buffer, "r");
    fgets(buffer, 18, rawfa);
    //printf("RES: %s\n",buffer);
    pclose(rawfa);

    sprintf(buffer, "svn commit -m \"Team: %d Run: %d\" %s 2>&1", id, mRunNumber, lastRawName.c_str());
    //printf("CMD: %s\n",buffer);
    FILE* rawfc = popen(buffer, "r");
    fgets(buffer, 18, rawfc);
    //printf("RES: %s\n",buffer);
    pclose(rawfc);

    if (strcmp(buffer, "svn: Commit failed")==0)
    {
        std::cout << "Error writing raw file, check connection to score server. Failure on commit." << std::endl;
    }

    return true;
}

void Game::buttonPressed_StartRun(int state, void* user)
{
    Game* game = static_cast<Game*>(user);
    game->mTeamID = game->mSelectedID;
    game->mRunNumber = game->mSelectedRunNumber;

    game->mGameLogic.Start();
}

void Game::buttonPressed_StopRun(int state, void* user)
{
    Game* game = static_cast<Game*>(user);

    game->mGameLogic.Stop();
}

void Game::buttonPressed_NewRun(int state, void* user)
{
    Game* game = static_cast<Game*>(user);

    game->mTeamID = game->mSelectedID;
    game->mRunNumber = game->mSelectedRunNumber;
    game->mGameLogic.NewRun();
}

void Game::buttonPressed_RandomizeCourse(int state, void* user)
{
    Game* game = static_cast<Game*>(user);

    Task correctTask[4];
    CourseConfig cc;
    memcpy(correctTask, cc.CorrectTask, sizeof(Task)*4);

    game->mJudgeCCMutex.Lock();
    game->mJudgeCourseConfig = cc;
    game->mJudgeCCMutex.Unlock();

    std::cout << "Voltage     (Yellow) " << ((correctTask[0]%2)? "Right" : "Left") << std::endl
              << "Capacitance (Blue)   " << ((correctTask[1]%2)? "Right" : "Left") << std::endl
              << "Temperature (Green)  " << ((correctTask[2]%2)? "Right" : "Left") << std::endl
              << "Waveform    (Red)    " << ((correctTask[3]%2)? "Right" : "Left") << std::endl;
}
