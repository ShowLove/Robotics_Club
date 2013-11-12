#include <auvc/underwater/testtracking.h>
#include <dvision/visionutils.h>
#define GAREBEAR
//#define SCHERERERER

using namespace Zebulon;
using namespace Underwater;

TestTracking::TestTracking()
{
    ID = "TestTracking";
    
    mForwardFrame = NULL;
    mSegFrame = NULL;
    mThreshFrame = NULL;
    mProcessedImage = NULL;
    mHistImage = NULL;
    
    mSegment = new Vision::Segment();
    mThreshold = new Vision::ThreshRatio();
    
    mDebounce.Initialize(0, true);
}

TestTracking::~TestTracking()
{
    
}

int TestTracking::Init()
{
    mHistImage = cvLoadImage("/home/michael/sublogs/14.20.53.50.271/red_hist_01.png");
    return 0;
}

int TestTracking::ErrorCheck()
{
    return 0;
}

int TestTracking::FetchProcess()
{
    mXmlConf.GetVar("Underwater.TestTracking.Default@Depth", mDefaultDepth);
    
    int flip;
    mXmlConf.GetVar("Underwater.TestTracking.Configuration@Turn", flip);
    if (flip)
        mState=Turn;
    mXmlConf.GetVar("Underwater.TestTracking.Configuration@Lateral", flip);
    if (flip)
        mState=Lateral;
    mXmlConf.GetVar("Underwater.TestTracking.Configuration@Search", flip);
    if (flip)
        mState=Search;
    
    mXmlConf.GetVar("Underwater.TestTracking.Buoy@Red", mTarget.mRed);
    mXmlConf.GetVar("Underwater.TestTracking.Buoy@Green", mTarget.mGreen);
    mXmlConf.GetVar("Underwater.TestTracking.Buoy@Blue", mTarget.mBlue);
    mXmlConf.GetVar("Underwater.TestTracking.BuoyThresh@Red", mTarget.mRedRange);
    mXmlConf.GetVar("Underwater.TestTracking.BuoyThresh@Green", mTarget.mGreenRange);
    mXmlConf.GetVar("Underwater.TestTracking.BuoyThresh@Blue", mTarget.mBlueRange);
    
    double num, denom;
    mXmlConf.GetVar("Underwater.TestTracking.LatPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.TestTracking.LatPixelScale@Denom", denom);
    mLatPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.TestTracking.DepthPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.TestTracking.DepthPixelScale@Denom", denom);
    mDepthPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.TestTracking.YawPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.TestTracking.YawPixelScale@Denom", denom);
    mYawPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.TestTracking.AxialPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.TestTracking.AxialPixelScale@Denom", denom);
    mAxialPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.TestTracking.Buoys@MinArea", mMinArea);
    mXmlConf.GetVar("Underwater.TestTracking.Buoys@MaxArea", mMaxArea);
    mXmlConf.GetVar("Underwater.TestTracking.Buoys@TargetArea", mTargetArea);
    
    mXmlConf.GetVar("Underwater.TestTracking.Laser@CloseDist", mLaserCloseDist);
    
    double mSearchStrafeTimeTmp = mSearchStrafeTime;
    double mSearchStrafeSpeedTmp = mSearchStrafeSpeed;
    double mSearchForwardTimeTmp = mSearchForwardTime;
    double mSearchForwardSpeedTmp = mSearchForwardSpeed;
    double mSearchSmoothSpeedTmp = mSearchSmoothSpeed;
    
    mXmlConf.GetVar("Underwater.TestTracking.Search@StrafeTime", mSearchStrafeTime);
    mXmlConf.GetVar("Underwater.TestTracking.Search@StrafeSpeed", mSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.TestTracking.Search@ForwardTime", mSearchForwardTime);
    mXmlConf.GetVar("Underwater.TestTracking.Search@ForwardSpeed", mSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.TestTracking.Search@SmoothSpeed", mSearchSmoothSpeed);
    
    if (mSearchStrafeTime != mSearchStrafeTimeTmp
        || mSearchStrafeSpeed != mSearchStrafeSpeedTmp
        || mSearchForwardTime != mSearchForwardTimeTmp
        || mSearchForwardSpeed != mSearchForwardSpeedTmp
        || mSearchSmoothSpeed != mSearchSmoothSpeedTmp)
    {
        mSearchPattern.InitSearch(mSearchStrafeTime, mSearchStrafeSpeed, mSearchForwardTime, mSearchForwardSpeed,mSearchSmoothSpeed, -1);
    }
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrent.mYaw))
    {
        printf("ERROR::TestTracking: Couldn't get Compass data");
        return 1;
    }
    
    // Depth
    if(!mGlobalInfo->GetInfo("DepthSensor", mCurrent.mDepth))
    {
        printf("ERROR::TestTracking: Couldn't get depth data");
        return 1;
    }
    
    // Laser
    /*if(!mGlobalInfo->GetInfo("Laser", mLaserDist))
    {
        printf("ERROR::TestTracking: Couldn't get laser data");
        return 1;
    }*/
    
    // Forward
    if(!mGlobalInfo->GetInfo("ForwardCamera", mForwardFrame))
    {
        printf("ERROR::TestTracking: Couldn't get forward camera data");
        return 1;
    }
    
    if(mProcessedImage == NULL)
    {
        mProcessedImage = cvCreateImage(cvSize(mForwardFrame->width,mForwardFrame->height), mForwardFrame->depth, mForwardFrame->nChannels);
    }
    Vision::VisionUtil::ResetImage(mProcessedImage);
    
#ifdef GAREBEAR
    mThreshold->Setup(mTarget);
    mSegment->Setup(mTarget, 50);
    
    mThreshold->Process(mForwardFrame, mThreshFrame);
#endif
#ifdef SCHERERERER
    //mThreshFrame = TestTracking::BackProject(mForwardFrame, cvLoadImage("/home/michael/sublogs/14.20.53.50.271/fw_hist_01.png"), 100, 1);
    mThreshFrame = TestTracking::BackProject(mForwardFrame, mHistImage, 100, 1);
#endif
    mSegment->Process(mThreshFrame, mSegFrame);
    
    mSegment->GetMaxPosition(mReturn);
    
    Vision::VisionUtil::CombineImages(mSegFrame, mProcessedImage);

    
    
    mDebounce.Bounce(mReturn.mValid && mReturn.mArea >= mMinArea && mReturn.mArea <= mMaxArea);
    
    
    return 0;
}

IplImage * TestTracking::BackProject(const IplImage * img, const IplImage * sampleHistImg, const int threshold, const int dilate)
{
    CvArr* back_project = (CvArr*)cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
    
    IplImage* h_plane = cvCreateImage( cvSize(img->width, img->height), 8, 1 );
    IplImage* s_plane = cvCreateImage( cvSize(img->width, img->height), 8, 1 );
    IplImage* v_plane = cvCreateImage( cvSize(img->width, img->height), 8, 1 );
    IplImage* planes[] = { h_plane, s_plane };
    IplImage* hsv = cvCreateImage( cvSize(img->width, img->height), 8, 3 );
    
    //int h_bins = 30, s_bins = 32;
    //int hist_size[] = {h_bins, s_bins};
    ///* hue varies from 0 (~0 deg red) to 180 (~360 deg red again) *
    //float h_ranges[] = { 0, 180 };
    ///* saturation varies from 0 (black-gray-white) to
    //   255 (pure spectrum color) *
    //float s_ranges[] = { 0, 255 };
    //float* ranges[] = { h_ranges, s_ranges };
    
    CvHistogram* hist;
    
    cvCvtColor( img, hsv, CV_BGR2HSV );
    cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
    //hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
    
    IplImage* sa_h_plane = cvCreateImage( cvSize(sampleHistImg->width, sampleHistImg->height), 8, 1 );
    IplImage* sa_s_plane = cvCreateImage( cvSize(sampleHistImg->width, sampleHistImg->height), 8, 1 );
    IplImage* sa_v_plane = cvCreateImage( cvSize(sampleHistImg->width, sampleHistImg->height), 8, 1 );
    IplImage* sa_planes[] = { sa_h_plane, sa_s_plane };
    IplImage* sa_hsv = cvCreateImage( cvSize(sampleHistImg->width, sampleHistImg->height), 8, 3 );
    
    int sa_h_bins = 30, sa_s_bins = 32;
    int sa_hist_size[] = {sa_h_bins, sa_s_bins};
    /* hue varies from 0 (~0 deg red) to 180 (~360 deg red again) */
    float sa_h_ranges[] = { 0, 180 };
    /* saturation varies from 0 (black-gray-white) to
       255 (pure spectrum color) */
    float sa_s_ranges[] = { 0, 255 };
    float* sa_ranges[] = { sa_h_ranges, sa_s_ranges };
    
    CvHistogram* sa_hist;
    
    cvCvtColor( sampleHistImg, sa_hsv, CV_BGR2HSV );
    cvCvtPixToPlane( sa_hsv, sa_h_plane, sa_s_plane, sa_v_plane, 0 );
    sa_hist = cvCreateHist( 2, sa_hist_size, CV_HIST_ARRAY, sa_ranges, 1 );
    
    cvCalcHist( sa_planes, sa_hist, 0, 0 );
    
    cvCalcBackProject(planes, back_project, sa_hist);
    
//    I(x,y)blue ~ ((uchar*)(img->imageData + img->widthStep*y))[x*3]
  //  I(x,y)green ~ ((uchar*)(img->imageData + img->widthStep*y))[x*3+1]
    //I(x,y)red ~ ((uchar*)(img->imageData + img->widthStep*y))[x*3+2]
    
    cvReleaseImage(&h_plane);
    cvReleaseImage(&s_plane);
    cvReleaseImage(&v_plane);
    //cvReleaseImage(&hsv);
    
    cvReleaseImage(&sa_h_plane);
    cvReleaseImage(&sa_s_plane);
    cvReleaseImage(&sa_v_plane);
    //cvReleaseImage(&sa_hsv);
    
    IplImage stub, *dst_img;
    dst_img = cvGetImage(back_project, &stub);
    return dst_img;
}

std::string TestTracking::ExitEarly()
{
    return "KeepRunning";
}

std::string TestTracking::Execute()
{
    double centerI = 0;
    double centerJ = 0;
    double area = 0;
    int validcount = 0;
    
    switch (mState)
    {
        case Lateral:
            if (mDebounce.GetDebounced())
            {
                std::cout << "IN DEBOUNCE" << std::endl;
                centerI = mReturn.mCenterI;
                centerJ = mReturn.mCenterJ;
                
                mDesired.mYaw = 140;
                mDesired.mLateralThrust = (centerI - mProcessedImage->width/2)*mLatPixelScale;
                mDesired.mDepth = mCurrent.mDepth + (centerJ - mProcessedImage->height/2)*mDepthPixelScale;
                //mDesired.mAxialThrust = (mTargetArea - area)*mAxialPixelScale;
                mDesired.mAxialThrust = 0;
                
                /*if (mLaserDist <= mLaserCloseDist)
                {
                }*/
            }
            else
            {
                mDesired.mYaw = 140;
                mDesired.mLateralThrust = 0;
                mDesired.mDepth = mDefaultDepth;
                mDesired.mAxialThrust = 0;
            }
            
            std::cout << "State: " << "TestTracking" << "::Lateral" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " DepthDiff: " << mCurrent.mDepth - mDesired.mDepth
                    << " Lat: " << mDesired.mLateralThrust
                    //<< " I: " << centerI
                    //<< " W: " << mProcessedImage->width/2
                    << " Area: " << mReturn.mArea
                    << " Axial: " << mDesired.mAxialThrust
                    << std::endl;
            break;
            
        case Turn:
            if (mDebounce.GetDebounced())
            {
                std::cout << "IN DEBOUNCE" << std::endl;
                centerI = mReturn.mCenterI;
                centerJ = mReturn.mCenterJ;
                
                mDesired.mYaw = mCurrent.mYaw + (centerI - mProcessedImage->width/2)*mYawPixelScale;
                //mDesired.mYaw = 180;
                mDesired.mLateralThrust = 0;
                mDesired.mDepth = mCurrent.mDepth + (centerJ - mProcessedImage->height/2)*mDepthPixelScale;
                //mDesired.mDepth = 36;
                //mDesired.mAxialThrust = (mTargetArea - area)*mAxialPixelScale;
                mDesired.mAxialThrust = 0;
                
                if (mLaserDist <= mLaserCloseDist)
                {
                }
            }
            else
            {
                mDesired.mYaw = mCurrent.mYaw;
                mDesired.mLateralThrust = 0;
                mDesired.mDepth = mDefaultDepth;
                mDesired.mAxialThrust = 0;
            }
            
            std::cout << "State: " << "TestTracking" << "::Turn" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " centerI: " << centerI
                    << " centerI - mProcessedImage->width/2: " << centerI - mProcessedImage->width/2
                    << " centerJ: " << centerJ
                    << " mDesired.mDepth: " << mDesired.mDepth
                    << " Area: " << mReturn.mArea
                    << " Axial: " << mDesired.mAxialThrust
                    << std::endl;
            break;
        case Search:
            mDesired.mYaw = 0;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mDefaultDepth;
            mDesired.mAxialThrust = 0;
            
            mSearchPattern.SearchStep(mDesired.mLateralThrust, mDesired.mAxialThrust);
            
            if (mDesired.mLateralThrust > 1.0)
            {
                mDesired.mLateralThrust = +mSearchStrafeSpeed;
            }
            else if (mDesired.mLateralThrust < -1.0)
            {
                mDesired.mLateralThrust = -mSearchStrafeSpeed;
            }
            else
            {
                mDesired.mLateralThrust = 0;
            }
            
            std::cout << "State: " << "TestTracking" << "::Search" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " Lateral: " << mDesired.mLateralThrust
                    << " Axial: " << mDesired.mAxialThrust
                    << " DefaultDepth: " << mDefaultDepth
                    << std::endl;
            break;
    }
    
    AI::Utility::HeadingDisplay(mProcessedImage, mCurrent.mYaw, mDesired.mYaw, 0, 255, 255);
    AI::Utility::DepthDisplay(mProcessedImage, mCurrent.mDepth, mDesired.mDepth, 0, 24);
    AI::Utility::ThrustDisplay(mProcessedImage, mDesired.mAxialThrust, mDesired.mLateralThrust);
    Utility::Display::DisplayImage("Post Processing",mProcessedImage);
    Utility::Display::DisplayImage("Pre Processing",mForwardFrame);
    
    mGlobalCommand->SetDesiredPitch(mDesired.mPitch);
    mGlobalCommand->SetDesiredRoll(mDesired.mRoll);
    mGlobalCommand->SetDesiredYaw(mDesired.mYaw);
    
    mGlobalCommand->SetDesiredAxialVel(mDesired.mAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesired.mLateralThrust);
    mGlobalCommand->SetDesiredDepth(mDesired.mDepth);
    
    return "KeepRunning";
}
