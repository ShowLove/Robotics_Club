#include "ieee/communication/datareceiver.h"

using namespace CxUtils;
using namespace std;
using namespace cv;

DataReceiver::DataReceiver()
{
    mProcData.mScore=0;
    mProcData.mCorrect=0;
    mProcData.mIncorrect=0;
    mProcData.mTimeMs=0;
    mProcData.mTeamID=-1;
    mProcData.mLaps = 0;
    mValid = false;
}

DataReceiver::~DataReceiver()
{
    mThread.StopThread(1000);

}

void DataReceiver::Initialize(int Port,int TrailLength)
{
    mPort=Port;
    mReceiveCount=0;
    mTrailLength=TrailLength;
    if(!mServer.InitializeSocket(mPort))
    {
        cout << "Unable to initialize on port " << mPort << endl;
        return;
    }
    cout << "Initialized UDP Server Socket on port " << mPort << endl;

    if(!mThread.CreateThread(&DataReceiver::Interface,this))
    {
            cout << "Could not Create Thread" << endl;
    }
}

void DataReceiver::Interface(void *args)
{
    DataReceiver *imgr=(DataReceiver *)args;
    imgr->WorkerThread();
}

void DataReceiver::WorkerThread()
{
    cout << "Thread Started..." << endl;
    int timeOutCount=0;
    while(mThread.QuitThreadFlag() == false)
    {
        //wait can determine frame rate
        if(mServer.Recv(mData, sizeof(struct GamePacket), 100, &mSource) > 0)
        {
            //cout << mSource.mString << " " << mPort << " Received (" << mData.Size() << " bytes): " << endl;
            GamePacket data;
            memcpy(&data,mData.Ptr(),sizeof(struct GamePacket));
            //printf("%d vs %d\n",mData.Size(),sizeof(struct GamePacket));
            mMutex.Lock();
            Process(data);
            mMutex.Unlock();
            timeOutCount=0;
            mValid=true;
        }
        else
        {
            if(timeOutCount>100)
            {
                //std::cout << "TIMED OUT: DATA " << timeOutCount << std::endl;
                mValid=false;
            }
            else
            {
                timeOutCount++;
            }

        }
    }
}

void DataReceiver::Process(GamePacket &Data)
{
    //extract things out
    //Local Data


    //Most replace recent
    mProcData.mScore=Data.mScore;
    mProcData.mCorrect=Data.mCorrect;
    mProcData.mIncorrect=Data.mIncorrect;
    mProcData.mTimeMs=Data.mTimeMs;
    mProcData.mLaps=Data.mLaps;

    mProcData.mCourseMoves=Data.mCourseMoves;

    //new team, reset
    if(mProcData.mTeamID!=Data.mTeamID)
    {
        mProcData.mTrail.clear();
    }
    mProcData.mTeamID=Data.mTeamID;
    printf("%d vs %d\n",mProcData.mTeamID,Data.mTeamID);

    //for now throw in randomly
    for(int i=0;i<Data.mNumPoints;i++)
    {
        cv::Point2f tmp;
        tmp.x=Data.mXList[i];
        tmp.y=Data.mYList[i];
        mProcData.mTrail.push_back(tmp);
    }
    //Ensure it is a certain length
    while(mProcData.mTrail.size()>mTrailLength)
    {
        mProcData.mTrail.erase(mProcData.mTrail.begin());
    }
}

ProcessedData DataReceiver::GetData()
{
    mMutex.Lock();
    ProcessedData tmp=mProcData;
    mMutex.Unlock();
    return tmp;
}

bool DataReceiver::GetValid()
{
    return mValid;
}
