/////////////////////////////////////////////////////////////////////////////////////
///
/// \file ieeevision.cpp
/// \brief A program to test some opencv algorithms.
///
/// Author(s): Michael Scherer<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@knights.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////

#include "ieee/cassy/ieeevision.h"

using namespace Zebulon;
using namespace IEEE;

IEEEVisionCassy::IEEEVisionCassy()
{
    mFirstTime = true;
    mSegment = new Vision::Segment();
    mThreshold = new Vision::Threshhold();

    mStartTime = CxUtils::GetTimeSeconds();

    mInitTrailFlag = true;

    // Team Colors
    mTeamColorText[0] = 04;
    mTeamColorText[1] = 201;
    mTeamColorText[2] = 255;

    mTeamColorBG[0] = 0;
    mTeamColorBG[1] = 0;
    mTeamColorBG[2] = 0;
    //mTeamColorText = cv::Scalar(0, 0, 0);

    mBOLeft = 16;
    mBORight = 617;
    mBOTop = 5;
    mBOBottom = 475;

    // Text Placement
    mUniversityNameTextPos.x = 20;
    mUniversityNameTextPos.y = 25;

    mTeamNameTextPos.x = (int)640/2 + 10;
    mTeamNameTextPos.y = 25;

    /////

    mRankTextPos.x = 20;
    mRankTextPos.y = 55;

    mBestScoreTextPos.x = (int)640/2 + 10;
    mBestScoreTextPos.y = 55;

    /////

    mTimeTextPos.x = 20;
    mTimeTextPos.y = 85;


    mCurrentScoreTextPos.x = (int)640/2 + 10;
    mCurrentScoreTextPos.y = 85;



}

bool IEEEVisionCassy::Load(const std::string& filename)
{

    // TODO:: FILL IN XML STUFF BASED ON NEW STRUCTURE
    Configuration::XmlConfigurator* config = new Configuration::XmlConfigurator();
    config->Load(filename,"TeamInfo");

    int tmp;
    std::string tmpString;
    config->GetVar("TeamInfo@University", tmpString);   mUniversity = tmpString;
    config->GetVar("TeamInfo@Name", tmpString); mTeamName = tmpString;
    config->GetVar("TeamInfo@Rank", tmp);  mTeamRank = tmp;
    config->GetVar("TeamInfo@HighScore", tmp);  mTeamHighScore = tmp;
    config->GetVar("TeamInfo@TextColorRed", tmp);  mTeamColorText[2] = tmp;
    config->GetVar("TeamInfo@TextColorGreen", tmp);  mTeamColorText[1] = tmp;
    config->GetVar("TeamInfo@TextColorBlue", tmp);  mTeamColorText[0] = tmp;
    config->GetVar("TeamInfo@BGColorRed", tmp);  mTeamColorBG[2] = tmp;
    config->GetVar("TeamInfo@BGColorGreen", tmp);  mTeamColorBG[1] = tmp;
    config->GetVar("TeamInfo@BGColorBlue", tmp);  mTeamColorBG[0] = tmp;

    config->Load(filename,"CourseInfo");
    config->GetVar("TeamInfo@University", tmpString);   mUniversity = tmpString;
    config->GetVar("TeamInfo@Name", tmpString); mTeamName = tmpString;
    config->GetVar("TeamInfo@Rank", tmp);  mTeamRank = tmp;
    config->GetVar("TeamInfo@HighScore", tmp);  mTeamHighScore = tmp;


    config->Load(filename,"AdditionalInfo");
    config->GetVar("TextPosition@University", tmpString);   mUniversity = tmpString;
    config->GetVar("TextPosition@Name", tmpString); mTeamName = tmpString;
    config->GetVar("TextPosition@Rank", tmp);  mTeamRank = tmp;
    config->GetVar("TextPosition@HighScore", tmp);  mTeamHighScore = tmp;

    /*std::cout << "mUniversity: " << mUniversity << std::endl;
    std::cout << "mTeamName: " << mTeamName << std::endl;
    std::cout << "mTeamRank: " << mTeamRank << std::endl;
    std::cout << "mTeamHighScore: " << mTeamHighScore << std::endl;
    std::cout << "mTeamColorText[2]: " << (int)mTeamColorText[2] << std::endl;
    std::cout << "mTeamColorText[1]: " << (int)mTeamColorText[1] << std::endl;
    std::cout << "mTeamColorText[0]: " << (int)mTeamColorText[0] << std::endl;
    std::cout << "mTeamColorBG[2]: " << (int)mTeamColorBG[2] << std::endl;
    std::cout << "mTeamColorBG[1]: " << (int)mTeamColorBG[1] << std::endl;
    std::cout << "mTeamColorBG[0]: " << (int)mTeamColorBG[0] << std::endl;*/

    /*config->GetVar("CourseInfo@Red", tmp);   mThreshold.red = tmp;
    config->GetVar("CourseInfo@Green", tmp); mThreshold.green = tmp;
    config->GetVar("CourseInfo@Blue", tmp);  mThreshold.blue = tmp;
    config->GetVar("CourseInfo@All", mThresholdAll);
    config->GetVar("CourseInfo@Size", mSize);
    config->GetVar("CourseInfo@Dilate", mDilate);
    config->GetVar("CourseInfo@MinSize", mMinSize);*/

    //config->GetVar("IEEEVision.Template@Filename", mTemplateFilename);
    //config->GetVar("IEEEVision.Template@MinConfidence", mMinConfidence);

    //config->GetVar("IEEEVision.Algorithm@Algorithm", tmp); mAlgorithm = tmp;

    //std::cout << "IMAGE IMAGE IMAGE" << std::endl;

    //std::cout << "mUniversity: " << mUniversity << std::endl;


    delete config;

    return true;

}

IEEEVisionCassy::~IEEEVisionCassy()
{
    delete mSegment;
}

bool IEEEVisionCassy::backgroundSubtraction(const cv::Mat &input, cv::Mat &output)
{


    if (mFirstTime)
    {
        input.copyTo(mLastFrame);
        input.copyTo(mForegroundFrame);

        mFinalOutput.create(input.rows + HEIGHT_OFFSET, input.cols, CV_8UC3);

        mBackgroundSubtractorMOG.initialize(mLastFrame.size(), CV_8UC3);
        mBackgroundSubtractorMOG.history = 500;
        mBackgroundSubtractorMOG.backgroundRatio = 0.7; // higher number - less output
        mBackgroundSubtractorMOG.nmixtures = 5;
        mBackgroundSubtractorMOG.noiseSigma = 15; // higher number - lower noise
        mBackgroundSubtractorMOG.varThreshold = 5; // 0 - everything, 1 - looks cool,

        mFirstTime = false;
        mFrameCount = 0;
    }

    mCurrentFrame = input.clone();

    //cv::GaussianBlur(mCurrentFrame, mCurrentFrame, cv::Size(15,15), 2);

    //cv::imshow("BLUR", mCurrentFrame);

    mBackgroundSubtractorMOG(mCurrentFrame, output, .015); //.015

    //mBackgroundSubtractorMOG.getBackgroundImage(mBGImage);
    //std::cout << "mBGImage: " << mBGImage << std::endl;
    //cv::imshow("BG", mBGImage);

    //std::cout << "History: " << mBackgroundSubtractorMOG.history << std::endl;
    // Default: 200
    //std::cout << "BG Ratio: " << mBackgroundSubtractorMOG.backgroundRatio << std::endl;
    // Default: 0.7
    //std::cout << "BG Model: " << mBackgroundSubtractorMOG.bgmodel << std::endl;
    // Possibly all values for bg model
    //std::cout << "Frame Size: h- " << mBackgroundSubtractorMOG.frameSize.height << ", w-" << mBackgroundSubtractorMOG.frameSize.width << std::endl;
    // Image size information
    //std::cout << "Frame Type: " << mBackgroundSubtractorMOG.frameType << std::endl;
    // Image type information
    //std::cout << "N Frames: " << mBackgroundSubtractorMOG.nframes << std::endl;
    // number of frames fed
    //std::cout << "N Mixtures: " << mBackgroundSubtractorMOG.nmixtures << std::endl;
    // Gaussians per pixel? Default: 5
    //std::cout << "Noise Sigma: " << mBackgroundSubtractorMOG.noiseSigma << std::endl;
    // Default: 15
    //std::cout << "Var Thresh: " << mBackgroundSubtractorMOG.varThreshold << std::endl;
    // Default 6.25

    mCurrentFrame.copyTo(mLastFrame);

    mFrameCount++;

    return true;
}

bool IEEEVisionCassy::backgroundSubtractionCleanUp(const cv::Mat &input, cv::Mat &output)
{
    cv::rectangle(output, cv::Point(0, 0), cv::Point(mBOLeft, input.rows), cv::Scalar(0, 0, 0), -1);
    cv::rectangle(output, cv::Point(mBORight, 0), cv::Point(input.cols, input.rows), cv::Scalar(0, 0, 0), -1);
    cv::rectangle(output, cv::Point(0, 0), cv::Point(input.cols, mBOTop), cv::Scalar(0, 0, 0), -1);
    cv::rectangle(output, cv::Point(0, mBOBottom), cv::Point(input.cols, input.rows), cv::Scalar(0, 0, 0), -1);

    cv::dilate(input, output, cv::Mat(), cv::Point(-1, -1), 3, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
    cv::erode(input, output, cv::Mat(), cv::Point(-1, -1), 4, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());

    return true;
}

bool IEEEVisionCassy::segmentation(const cv::Mat &input, cv::Mat &output)
{
    //Setup(int Red,int Green,int Blue,int MinArea,int ComboDist, int DrawNum)
    mSegment->Setup(255, 0, 255, 100, 10);

    cv::Mat tempInput(input.rows, input.cols, CV_8UC3);

    cv::Vec3b tempVec;

    //std::cout << "CONVERT TO COLOR" << std::endl;
    for(int i = 0; i < input.cols; i++)
    {
        //std::cout << i << std::endl;
        for(int j = 0; j < input.rows; j++)
        {
            tempVec[0] = input.at<uchar>(j, i);
            tempVec[1] = input.at<uchar>(j, i);
            tempVec[2] = input.at<uchar>(j, i);

            //std::cout << "tempVec: " << tempVec[0] << std::endl;

            tempInput.at< cv::Vec3b >(j, i) = tempVec;
        }
    }

    //imshow("tempInput", tempInput);
    //cv::Mat tempOutput;

    mSegment->Process(tempInput, output);

}

std::vector< Vision::Return > IEEEVisionCassy::GetSegments()
{
    bool continueFlag = true;
    int validCount = 0;

    std::vector< Vision::Return > visionOutput;
    Vision::Return tempReturn;

    for(int i = 0; i < 10; i++)
    {
        mSegment->GetMaxPosition(tempReturn, validCount);

        if(tempReturn.mValid == true && tempReturn.mArea < (320.0*240.0)) // Add a max size
        {
            visionOutput.push_back(tempReturn);
            validCount++;
        }
    }

    mCurrentReturns.clear();
    for(int i = 0; i < visionOutput.size(); i++)
    {
        mCurrentReturns.push_back(visionOutput[i]);
    }
    //mCurrentReturns = visionOutput;

    return visionOutput;
}

std::vector< cv::Point2f > IEEEVisionCassy::TrailCalc(std::vector< cv::Point2f > prevTrail, int objectNum, int trackingTime)
{

    //for(int i = 0; i < objectNum.size(), )
    // add mutlitple trails

    //input.copyTo(output);
    std::vector< cv::Point2f > outputTrail;

    double currJ, currI, currAngle;
    int currArea, currValid;

    mSegment->GetMaxPosition(currJ, currI, currArea, currValid, currAngle);

    /*std::cout << "currJ: " << currJ << std::endl;
    std::cout << "currI: " << currI << std::endl;
    std::cout << "currArea: " << currArea << std::endl;
    std::cout << "currValid: " << currValid << std::endl;
    std::cout << "currAngle: " << currAngle << std::endl;*/

    if(currValid == true)
    {
        if(mInitTrailFlag == true)
        {
            //std::cout << "INITIAL" << std::endl;
            mInitTrailFlag = false;

            cv::Point currPos;
            currPos.x = currI;
            currPos.y = currJ;

            /*for(int i = 0; i < trackingTime; i++)
            {
                outputTrail.push_back(currPos);
            }//*/

            outputTrail.assign(trackingTime, currPos);
        }
        else
        {
            cv::Point currPos;
            currPos.x = currI;
            currPos.y = currJ;

            outputTrail.push_back(currPos);
            std::vector<cv::Point2f>::const_iterator iter;
            int i=0;
            for (iter=prevTrail.begin(); iter!=prevTrail.end() && i<prevTrail.size()-1; iter++, i++)
                 outputTrail.push_back(*iter);


            //std::cout << "NOT INITIAL" << std::endl;
            /*for(int i = 0; i < prevTrail.size() - 1; i++)
            {
                outputTrail.push_back(prevTrail[i]);
            }//*/

//<<<<<<< .mine
//=======
            //outputTrail.assign(prevTrail.begin(), prevTrail.begin()+(prevTrail.size()-1));


//>>>>>>> .r3956
        }
    }

    /*for(int i = 0; i < outputTrail.size(); i++)
    {
        std::cout << "TRAIL " << i << ": " << outputTrail[i].x << ", " << outputTrail[i].y << std::endl;
    }*/



    return outputTrail;
}

void IEEEVisionCassy::DisplayTrail(const cv::Mat &input, cv::Mat &output, std::vector< cv::Point2f > trail)
{
    input.copyTo(output);

    for(int i = 0; i < trail.size(); i++)
    {
        cv::circle(output, trail[i], 2, cv::Scalar(255, 0, 0), 1, 8, 0);
    }
}

void IEEEVisionCassy::AdditionalInformation(bool displayFlag)
{
    //mCurrentFrame.copyTo(mFinalOutput);

    mCurrentTime = CxUtils::GetTimeSeconds() - mStartTime;

    //std::cout << "mCurrentTime: " << mCurrentTime << std::endl;

    //CvFont font = fontQt(''Times'');
    //addText( img1, ``Hello World !'', Point(50,50), font);

    if(displayFlag)
    {
        // Refresh text bar
        for(int i = 0; i < mFinalOutput.cols; i++)
        {
            for(int j = 0; j < HEIGHT_OFFSET; j++)
            {
                mFinalOutput.at< cv::Vec3b >(j, i) = mTeamColorBG;
            }
        }

        // Update Video
        for(int j = 0; j < mCurrentFrame.rows; j++)
        {
            mFinalOutput.row(j + HEIGHT_OFFSET) = 1.0 * mCurrentFrame.row(j);
        }


        //CvFont font = cv::fontQt(''Times'');
        //cv::addText( mFinalOutput, ``Hello World !'', Point(10,10), font);

        mFont = cv::fontQt("Times", 15, cv::Scalar(mTeamColorText[2], mTeamColorText[1], mTeamColorText[0]));

        char universityName[50] = "University of Central Florida : ";
        char teamName[50] = "Test!!!!!!!";
        char rankString[50] = "Rank: 25";
        char timeString[50] = "Run Time: 01.94.1932";
        char bestScoreString[50] = "Best Score: 300";
        char currScoreString[50] = "Current Score: 150";

        cv::addText(mFinalOutput, universityName, cv::Point(mUniversityNameTextPos.x, mUniversityNameTextPos.y), mFont);
        cv::addText(mFinalOutput, teamName, cv::Point(mTeamNameTextPos.x, mTeamNameTextPos.y), mFont);
        cv::addText(mFinalOutput, currScoreString, cv::Point(mCurrentScoreTextPos.x, mCurrentScoreTextPos.y), mFont);
        cv::addText(mFinalOutput, timeString, cv::Point(mTimeTextPos.x, mTimeTextPos.y), mFont);
        cv::addText(mFinalOutput, rankString, cv::Point(mRankTextPos.x, mRankTextPos.y), mFont);
        cv::addText(mFinalOutput, bestScoreString, cv::Point(mBestScoreTextPos.x, mBestScoreTextPos.y), mFont);

        cv::imshow("Final Output", mFinalOutput);
    }
}

void IEEEVisionCassy::ThresholdLines(const cv::Mat &input, cv::Mat &output)
{
    input.copyTo(output);



    int mBOLeftLines = 53; // 16
    int mBORightLines = 580; // 617
    int mBOTopLines = 35; // 5
    int mBOBottomLines = 442; // 475

    cv::rectangle(output, cv::Point(0, 0), cv::Point(mBOLeftLines, input.rows), cv::Scalar(0, 0, 0), -1);
    cv::rectangle(output, cv::Point(mBORightLines, 0), cv::Point(input.cols, input.rows), cv::Scalar(0, 0, 0), -1);
    cv::rectangle(output, cv::Point(0, 0), cv::Point(input.cols, mBOTopLines), cv::Scalar(0, 0, 0), -1);
    cv::rectangle(output, cv::Point(0, mBOBottomLines), cv::Point(input.cols, input.rows), cv::Scalar(0, 0, 0), -1);

    //std::cout << "Current Ret in Lines: " << mCurrentReturns.size() << std::endl;
    for(int i = 0; i < mCurrentReturns.size(); i++)
    {
        //std::cout << "MinI: " << mCurrentReturns[i].mMinI << std::endl;

        double radius = sqrt((mCurrentReturns[i].mMinI - mCurrentReturns[i].mMaxI)*(mCurrentReturns[i].mMinI - mCurrentReturns[i].mMaxI)
                             + (mCurrentReturns[i].mMinJ - mCurrentReturns[i].mMaxJ)*(mCurrentReturns[i].mMinJ - mCurrentReturns[i].mMaxJ));

        radius /= 2;

        cv::Point tempPos;
        tempPos.x = mCurrentReturns[i].mCenterI;
        tempPos.y = mCurrentReturns[i].mCenterJ;

        cv::circle(output, tempPos, radius, cv::Scalar(255,0,255),-1);

    }
    //cv::imshow("Lines", output);
    //cv::threshold(output, output, 240, 255, cv::THRESH_BINARY);
    //mThreshold->Setup();
    mThreshold->Setup(255, 255, 255, 20, 15, 15, 0);


    mThreshold->Process(output, output);

    cv::dilate(output, output, cv::Mat(), cv::Point(-1, -1), 1, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());


}

void IEEEVisionCassy::ColorLines(const cv::Mat &input, cv::Mat &output)
{
    // Have larger bounding box, if white, color red/green
    // fade from white to green - gaussian blur?
}

void IEEEVisionCassy::ConditionFinalImage(cv::Mat &output)
{
    for(int j = 0; j < output.rows; j++)
    {
        for(int i = 0; i < output.cols; i++)
        {
            cv::Vec3b tempVec;
            tempVec[0] = 0;
            tempVec[1] = 0;
            tempVec[2] = 0;
            output.at< cv::Vec3b >(j, i) = tempVec;
        }
    }
}

void IEEEVisionCassy::AddRawImage(const cv::Mat &input, cv::Size resolution, cv::Point initPos, cv::Mat &output)
{
    cv::Mat tempImg;
    cv::Mat mask(output.rows, output.cols, CV_8UC1);

    cv::resize(input, tempImg, resolution);

    cv::Rect border;
    border.x = initPos.x - 1;
    border.y = initPos.y - 1;
    border.width = resolution.width + 2;
    border.height = resolution.height + 2;
    cv::rectangle(output, border, cv::Scalar(04, 201, 255),1);

    for(int j = 0; j < tempImg.rows; j++)
    {
        for(int i = 0; i < tempImg.cols; i++)
        {
            cv::Vec3b tempVec = tempImg.at< cv::Vec3b >(j, i);
            output.at< cv::Vec3b >(j + initPos.y, i + initPos.x) = tempVec;
        }
    }

}

void IEEEVisionCassy::AddRawText(TeamTextInfo textInfo, ProcessedData processedData, TextPos textPos, cv::Mat &output)
{
    //std::string University, std::string team, std::string compType, cv::Vec3b textColor, cv::Vec3b bgColor,
    //CvFont font = cv::fontQt(''Times'');
    //cv::addText( mFinalOutput, ``Hello World !'', Point(10,10), font);

    CvFont largeFont = cv::fontQt("Times", textPos.LargeText, cv::Scalar(textInfo.TextColor[0], textInfo.TextColor[1], textInfo.TextColor[2]));
    CvFont mediumFont = cv::fontQt("Times", textPos.MediumText, cv::Scalar(textInfo.TextColor[0], textInfo.TextColor[1], textInfo.TextColor[2]));
    CvFont smallFont = cv::fontQt("Times", textPos.SmallText, cv::Scalar(textInfo.TextColor[0], textInfo.TextColor[1], textInfo.TextColor[2]));

    if(textInfo.University != textInfo.Team)
    {
        cv::addText(output, textInfo.University, textPos.University, mediumFont);
    }

    cv::addText(output, textInfo.Team, textPos.Team, largeFont);

    cv::addText(output, textInfo.CompType, textPos.CompType, smallFont);

    char tmpString[64];

    int minutes = std::floor((processedData.mTimeMs/1000.0)/60);
    int seconds = std::floor((processedData.mTimeMs/1000.0) - minutes*60);
    int mseconds = std::floor(processedData.mTimeMs - seconds*1000 - minutes*60*1000);
    cv::Vec3b tempRawColor;
    if(minutes < 1 && seconds < 30)
    {
        tempRawColor[0] = 255;
        tempRawColor[1] = 0;
        tempRawColor[2] = 0;
    }
    else
    {
        tempRawColor[0] = 255;//0;
        tempRawColor[1] = 201;//0;
        tempRawColor[2] = 04;//255;
    }
    CvFont rawTimeFont = cv::fontQt("Times", textPos.LargeText, cv::Scalar(0,0,0));
    sprintf(tmpString, "%01d : %02d : %03d", minutes, seconds, mseconds);
    // BG for Time: so it can be read
    /*cv::Rect timeRec;
    timeRec.x = 463, timeRec.y = 423; // was 459
    timeRec.width = (595-463), timeRec.height = (447-423);
    cv::rectangle(output, timeRec, cv::Scalar(225,225,225),-1);*/
    cv::addText(output, tmpString, textPos.RawTime, rawTimeFont);
    rawTimeFont = cv::fontQt("Times", textPos.LargeText, cv::Scalar(tempRawColor[0],tempRawColor[1],tempRawColor[2]));
    textPos.RawTime.x++;
    textPos.RawTime.y++;
    cv::addText(output, tmpString, textPos.RawTime, rawTimeFont);

    //////////////// SCORES ////////////////////////
    sprintf(tmpString, "Current Score");
    //std::cout << "curr score: " << textPos.CurrScore.x << std::endl;
    //std::cout << "curr score: " << textPos.CurrScore.y << std::endl;
    textPos.CurrScore.x -= 80;
    cv::addText(output, tmpString, textPos.CurrScore, smallFont);
    cv::Vec3b currScoreColor;
    if(processedData.mScore == 0)
    {
        currScoreColor[0]=225,currScoreColor[1]=225,currScoreColor[2]=225;
    }
    else if(processedData.mScore >= 100)
    {
        currScoreColor[0]=0,currScoreColor[1]=255,currScoreColor[2]=0;
    }
    else
    {
        currScoreColor[0]=255,currScoreColor[1]=0,currScoreColor[2]=0;
    }
    CvFont currScoreText = cv::fontQt("Times", 50, cv::Scalar(currScoreColor[0],currScoreColor[1],currScoreColor[2]));
    sprintf(tmpString, "%04d", processedData.mScore);
    textPos.CurrScore.x -= 50;
    textPos.CurrScore.y -= 15;
    cv::addText(output, tmpString, textPos.CurrScore, currScoreText);


    sprintf(tmpString, "Percent Correct");
    //std::cout << "perc correct: " << textPos.Percentage.x << std::endl;
    //std::cout << "perc correct: " << textPos.Percentage.y << std::endl;
    textPos.Percentage.x -= 50;
    cv::addText(output, tmpString, textPos.Percentage, smallFont);
    cv::Vec3b percentageColor;
    int percentage = 0;
    if(processedData.mCorrect != 0 || processedData.mIncorrect != 0)
    {
        percentage = floor( ((100.0*processedData.mCorrect)/(processedData.mCorrect + processedData.mIncorrect)) );
    }
    if(percentage == 50)
    {
        percentageColor[0]=225, percentageColor[1]=225, percentageColor[2]=225;
    }
    else if(percentage > 50)
    {
        percentageColor[0]=0, percentageColor[1]=255, percentageColor[2]=0;
    }
    else
    {
        percentageColor[0]=255, percentageColor[1]=0, percentageColor[2]=0;
    }
    CvFont percentageText = cv::fontQt("Times", 50, cv::Scalar(percentageColor[0],percentageColor[1],percentageColor[2]));
    sprintf(tmpString, "%03d%%", percentage);
    textPos.Percentage.x -= 40;
    textPos.Percentage.y -= 15;
    cv::addText(output, tmpString, textPos.Percentage, percentageText);

    sprintf(tmpString, "Completed Laps");
    //std::cout << "comp laps: " << textPos.Laps.x << std::endl;
   // std::cout << "comp laps: " << textPos.Laps.y << std::endl;
    textPos.Laps.x -= 20;
    cv::addText(output, tmpString, textPos.Laps, smallFont);
    cv::Vec3b lapsColor;
    if(processedData.mLaps >= 0)
    {
        lapsColor[0]=225, lapsColor[1]=225, lapsColor[2]=225;
    }
    CvFont lapsText = cv::fontQt("Times", 50, cv::Scalar(lapsColor[0],lapsColor[1],lapsColor[2]));
    sprintf(tmpString, "%03d", processedData.mLaps);
    textPos.Laps.x -= 20;
    textPos.Laps.y -= 15;
    cv::addText(output, tmpString, textPos.Laps, lapsText);

    /*sprintf()
    cv::addText(output, processedData.mTimeMs, cv::Point(mTimeTextPos.x, mTimeTextPos.y), mFont);
    cv::addText(output, processedData.mScore, cv::Point(mRankTextPos.x, mRankTextPos.y), mFont);
    cv::addText(output, processedData.m, cv::Point(mBestScoreTextPos.x, mBestScoreTextPos.y), mFont);*/


    /*char universityName[50] = "University of Central Florida : ";
    char teamName[50] = "Test!!!!!!!";
    char rankString[50] = "Rank: 25";
    char timeString[50] = "Run Time: 01.94.1932";
    char bestScoreString[50] = "Best Score: 300";
    char currScoreString[50] = "Current Score: 150";*/
}

void IEEEVisionCassy::ConditionTrailSpace(cv::Point initPos, int offset, cv::Size resolution, cv::Vec3b color, cv::Mat &output)
{

    /*std::cout << "CONDITION TRAIL SPACE" << std::endl;
    std::cout << "initPos: x- " << initPos.x << ", y- " << initPos.y << std::endl;
    std::cout << "offset: " << offset << std::endl;
    std::cout << "resolution: width- " << resolution.width << ", height- " << resolution.height << std::endl;*/

    for(int k = 0; k < 4; k++)
    {
        for(int j = 0; j < resolution.height; j++)
        {
            for(int i = 0; i < resolution.width; i++)
            {
                cv::Vec3b tempVec = color;
                //tempVec[0] = color.;
                //tempVec[1] = 56;
                //tempVec[2] = 56;

                output.at< cv::Vec3b >((j + initPos.y) + k*offset, i + initPos.x) = tempVec;
            }
        }
    }
}

void IEEEVisionCassy::HighlightCurrentTrail(int currTrail, cv::Point initPos, int offset, cv::Size resolution, cv::Scalar color, cv::Mat &output)
{
    cv::Rect highlightRec;

    highlightRec.x = initPos.x - 1;
    highlightRec.y = initPos.y + currTrail*offset - 1;

    highlightRec.height = resolution.height + 2;
    highlightRec.width = resolution.width + 2;

    cv::rectangle(output, highlightRec, color, 1);
}

void IEEEVisionCassy::AddTrail(int offAmount, int offset, ProcessedData processedData, cv::Point initPos, cv::Size origRes, cv::Size trailRes, cv::Mat &output)
{
    //std::cout << "PRINT TRAIL SIZE: " << processedData.mTrail.size() << std::endl;
    for(int i = 0; i < processedData.mTrail.size(); i++)
    {
        cv::Point2f tempPoint;

        tempPoint.x = initPos.x + processedData.mTrail[i].x * 0.30625;//(trailRes.width / origRes.width);
        tempPoint.y = initPos.y + offAmount*offset + processedData.mTrail[i].y * 0.30625;//(trailRes.height / origRes.height);

        //std::cout << "x: " << i << " - " << processedData.mTrail[i].x << std::endl;
        //std::cout << "y: " << i << " - " << processedData.mTrail[i].y << std::endl;

        cv::Vec3b trailColor;
        if(offAmount == 0)
        {
            trailColor[0] = 255, trailColor[1] = 0, trailColor[2] = 255;
        }
        else if(offAmount == 1)
        {
            trailColor[0] = 0, trailColor[1] = 255, trailColor[2] = 255;
        }
        else if(offAmount == 2)
        {
            trailColor[0] = 0, trailColor[1] = 255, trailColor[2] = 0;
        }
        else
        {
            trailColor[0] = 255, trailColor[1] = 255, trailColor[2] = 0;
        }

        if(i == processedData.mTrail.size()-1)
        {
            cv::circle(output, tempPoint, 5, cv::Scalar(trailColor[0],trailColor[1],trailColor[2]), -1);
        }
        else
        {
            cv::circle(output, tempPoint, 1, cv::Scalar(trailColor[0],trailColor[1],trailColor[2]), -1);
        }
    }
}

void IEEEVisionCassy::AddTrailText(int offset, ProcessedData processedData, TextPos textPos, cv::Mat &output)
{
    //CvFont font = cv::fontQt("Times", 8, cv::Scalar(textInfo.TextColor[0], textInfo.TextColor[1], textInfo.TextColor[2]));

    textPos.InitTrailTime.y += offset*textPos.TrailOffset;
    textPos.InitTrailScore.y += offset*textPos.TrailOffset;
    textPos.InitTrailPercent.y += offset*textPos.TrailOffset;
    textPos.InitTrailLaps.y += offset*textPos.TrailOffset;

    textPos.SmallText = 11;
    char tmpString[64];

    int minutes = std::floor((processedData.mTimeMs/1000.0)/60);
    int seconds = std::floor((processedData.mTimeMs/1000.0) - minutes*60);
    int mseconds = std::floor(processedData.mTimeMs - seconds*1000 - minutes*60*1000);
    cv::Vec3b tempRawColor;
    if(minutes < 1 && seconds < 30)
    {
        tempRawColor[0] = 255;
        tempRawColor[1] = 0;
        tempRawColor[2] = 0;
    }
    else
    {
        tempRawColor[0] = 255;
        tempRawColor[1] = 201;
        tempRawColor[2] = 04;
    }
    CvFont rawTimeFont = cv::fontQt("Times", textPos.SmallText, cv::Scalar(tempRawColor[0],tempRawColor[1],tempRawColor[2]));
    sprintf(tmpString, "%01d : %02d : %03d", minutes, seconds, mseconds);
    // BG for Time: so it can be read
    ////cv::Rect timeRec;
    ////timeRec.x = 463, timeRec.y = 423; // was 459
    ////timeRec.width = (595-463), timeRec.height = (447-423);
    ////cv::rectangle(output, timeRec, cv::Scalar(225,225,225),-1);
    cv::addText(output, tmpString, textPos.InitTrailTime, rawTimeFont);



    //////////////// SCORES ////////////////////////
    //sprintf(tmpString, "Current Score");
    //std::cout << "curr score: " << textPos.CurrScore.x << std::endl;
    //std::cout << "curr score: " << textPos.CurrScore.y << std::endl;
    //textPos.CurrScore.x -= 80;
    //cv::addText(output, tmpString, textPos.CurrScore, smallFont);
    cv::Vec3b currScoreColor;
    if(processedData.mScore == 0)
    {
        currScoreColor[0]=225,currScoreColor[1]=225,currScoreColor[2]=225;
    }
    else if(processedData.mScore >= 100)
    {
        currScoreColor[0]=0,currScoreColor[1]=255,currScoreColor[2]=0;
    }
    else
    {
        currScoreColor[0]=255,currScoreColor[1]=0,currScoreColor[2]=0;
    }
    CvFont currScoreText = cv::fontQt("Times", textPos.SmallText, cv::Scalar(currScoreColor[0],currScoreColor[1],currScoreColor[2]));
    sprintf(tmpString, "%04d", processedData.mScore);
    //textPos.CurrScore.x -= 50;
    //textPos.CurrScore.y -= 15;
    cv::addText(output, tmpString, textPos.InitTrailScore, currScoreText);


    cv::Vec3b percentageColor;
    int percentage = 0;
    if(processedData.mCorrect != 0 || processedData.mIncorrect != 0)
    {
        percentage = floor( ((100.0*processedData.mCorrect)/(processedData.mCorrect + processedData.mIncorrect)) );
    }
    if(percentage == 50)
    {
        percentageColor[0]=225, percentageColor[1]=225, percentageColor[2]=225;
    }
    else if(percentage > 50)
    {
        percentageColor[0]=0, percentageColor[1]=255, percentageColor[2]=0;
    }
    else
    {
        percentageColor[0]=255, percentageColor[1]=0, percentageColor[2]=0;
    }
    CvFont percentageText = cv::fontQt("Times", textPos.SmallText, cv::Scalar(percentageColor[0],percentageColor[1],percentageColor[2]));
    sprintf(tmpString, "%03d%%", percentage);
    //textPos.Percentage.x -= 40;
    //textPos.Percentage.y -= 15;
    cv::addText(output, tmpString, textPos.InitTrailPercent, percentageText);

    //sprintf(tmpString, "Completed Laps");
    //std::cout << "comp laps: " << textPos.Laps.x << std::endl;
    //std::cout << "comp laps: " << textPos.Laps.y << std::endl;
    //textPos.Laps.x -= 20;
    //cv::addText(output, tmpString, textPos.Laps, smallFont);
    cv::Vec3b lapsColor;
    if(processedData.mLaps >= 0)
    {
        lapsColor[0]=225, lapsColor[1]=225, lapsColor[2]=225;
    }
    CvFont lapsText = cv::fontQt("Times", textPos.SmallText, cv::Scalar(lapsColor[0],lapsColor[1],lapsColor[2]));
    sprintf(tmpString, "%03d", processedData.mLaps);
    //textPos.Laps.x -= 20;
    //textPos.Laps.y -= 15;
    cv::addText(output, tmpString, textPos.InitTrailLaps, lapsText);


}

void IEEEVisionCassy::DrawX(cv::Point pos, int length, int thickness, cv::Scalar color, cv::Mat &output)
{
    double newVal = length*0.707106;

    cv::line(output, cv::Point(pos.x-newVal, pos.y-newVal), cv::Point(pos.x+newVal, pos.y+newVal), color, thickness);
    cv::line(output, cv::Point(pos.x-newVal, pos.y+newVal), cv::Point(pos.x+newVal, pos.y-newVal), color, thickness);
}

void IEEEVisionCassy::DrawCheck(cv::Point pos, int length, int thickness, cv::Scalar color, cv::Mat &output)
{
    double longLength = length*0.707106;
    double halfLength = length*0.707106*0.25;

    cv::line(output, cv::Point(pos.x-halfLength, pos.y-halfLength), cv::Point(pos.x+halfLength, pos.y+longLength), color, thickness);
    cv::line(output, cv::Point(pos.x+halfLength, pos.y+longLength), cv::Point(pos.x+longLength, pos.y-longLength), color, thickness);

    //cv::circle(output, pos, radius, color, thickness);
}

void IEEEVisionCassy::DrawRawAnswers(cv::Mat &output, CourseMoves currMoveInfo, std::vector< cv::Point > taskPos)
{
    cv::Vec3b tempVec;
    for(int i = 0; i < 4; i++)
    {
        if(currMoveInfo.mMeasured[i] != -1)
        {
            if(i == 0)
            {
                tempVec[0] = 0;
                tempVec[1] = 255;
                tempVec[2] = 255;
            }
            else if(i == 1)
            {
                tempVec[0] = 255;
                tempVec[1] = 0;
                tempVec[2] = 0;
            }
            else if(i == 2)
            {
                tempVec[0] = 0;
                tempVec[1] = 255;
                tempVec[2] = 0;
            }
            else
            {
                tempVec[0] = 0;
                tempVec[1] = 0;
                tempVec[2] = 255;
            }

            if(currMoveInfo.mTrue[i] == currMoveInfo.mMeasured[i])
            {
                DrawCheck(taskPos[i], 20, 5, cv::Scalar(0,0,0),output);
                DrawCheck(cv::Point(taskPos[i].x + 1,taskPos[i].y + 1), 20, 5, cv::Scalar(tempVec[0],tempVec[1],tempVec[2]),output);
            }
            else
            {
                DrawX(taskPos[i], 20, 5, cv::Scalar(0,0,0),output);
                DrawX(cv::Point(taskPos[i].x + 1,taskPos[i].y + 1), 20, 5, cv::Scalar(tempVec[0],tempVec[1],tempVec[2]),output);
            }
        }
    }
}

void IEEEVisionCassy::DrawTrialAnswers(cv::Mat &output, int offsetAmount, int offset, CourseMoves currMoveInfo, std::vector< cv::Point > taskPos)
{
    cv::Vec3b tempVec;
    for(int i = 0; i < 4; i++)
    {
        if(currMoveInfo.mMeasured[i] != -1)
        {
            if(i == 0)
            {
                tempVec[0] = 0;
                tempVec[1] = 255;
                tempVec[2] = 255;
            }
            else if(i == 1)
            {
                tempVec[0] = 255;
                tempVec[1] = 0;
                tempVec[2] = 0;
            }
            else if(i == 2)
            {
                tempVec[0] = 0;
                tempVec[1] = 255;
                tempVec[2] = 0;
            }
            else
            {
                tempVec[0] = 0;
                tempVec[1] = 0;
                tempVec[2] = 255;
            }

            if(currMoveInfo.mTrue[i] == currMoveInfo.mMeasured[i])
            {
                //DrawCheck(taskPos[i], 10, 5, cv::Scalar(0,0,0),output);
                DrawCheck(cv::Point(taskPos[i].x + 0, taskPos[i].y + 0 + offsetAmount*offset), 10, 5, cv::Scalar(tempVec[0],tempVec[1],tempVec[2]),output);
            }
            else
            {
                //DrawX(taskPos[i], 10, 5, cv::Scalar(0,0,0),output);
                DrawX(cv::Point(taskPos[i].x + 0, taskPos[i].y + 0 + offsetAmount*offset), 10, 5, cv::Scalar(tempVec[0],tempVec[1],tempVec[2]),output);
            }
        }
    }
}

void IEEEVisionCassy::DrawWhiteLines(cv::Mat &output)
{
    //cv::Mat temp = cv::imread("dump_training/image00000.png");
    //cv::imshow("temp", temp);

    int height = 480;
    int width = 640;

    int newHeight = 147;
    int newWidth = 196;

    double m1, m2, m3, m4, m5, m6, m7, m8;
    m1 = 0;
    m2 = 1/8;
    m3 = 1/4;
    m4 = 3/8;
    m5 = 1/2;
    m6 = 5/8;
    m7 = 3/4;
    m8 = 7/8;

    std::vector< cv::Point > whitePts;

    //whitePts.push_back(cv::Point(m7,344));
    /*whitePts.push_back(cv::Point(,m3));
    whitePts.push_back(cv::Point(m3,m3));
    whitePts.push_back(cv::Point(m4,m4));
    whitePts.push_back(cv::Point(m2,m2));
    whitePts.push_back(cv::Point(m4,m4));
    whitePts.push_back(cv::Point(m2,m2));
    whitePts.push_back(cv::Point(m3,m3));
    whitePts.push_back(cv::Point(m3,m3));
    whitePts.push_back(cv::Point(m4,m4));
    whitePts.push_back(cv::Point(m2,m2));
    whitePts.push_back(cv::Point(m4,m4));
    whitePts.push_back(cv::Point(m2,m2));
    whitePts.push_back(cv::Point(m3,m3));
    whitePts.push_back(cv::Point(m3,m3));*/

    /*std::vector< cv::Point > whitePts;
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(64,234));
    whitePts.push_back(cv::Point(71,126));
    whitePts.push_back(cv::Point(152,114));
    whitePts.push_back(cv::Point(69,181));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));
    whitePts.push_back(cv::Point(69,344));

    cv::line(output, )*/
}



/* End of file */
