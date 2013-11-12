#include <auvc/underwater/testlasertracking.h>
//#include <dvision/visionutils.h>

using namespace Zebulon;
using namespace Underwater;
using namespace std;

TestLaserTracking::TestLaserTracking()
{
    ID = "TestLaserTracking";
    
    mDisplayLaser = NULL;
    
    mDebounce.Initialize(3, true);
    
    mDisplayLaser = NULL;
    mDisplayLaser = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);
}

TestLaserTracking::~TestLaserTracking()
{
    
}

int TestLaserTracking::Init()
{
    mObjectScanData.Setup(0.33, 1, 5, 0.2); //0.075
    
    return 0;
}

int TestLaserTracking::ErrorCheck()
{
    return 0;
}

int TestLaserTracking::FetchProcess()
{
    
#if 0
    mXmlConf.GetVar("Underwater.TestLaserTracking.Default@Depth", mDefaultDepth);
    
    int flip;
    mXmlConf.GetVar("Underwater.TestLaserTracking.Configuration@Turn", flip);
    if (flip)
        mState=Turn;
    mXmlConf.GetVar("Underwater.TestLaserTracking.Configuration@Lateral", flip);
    if (flip)
        mState=Lateral;
    mXmlConf.GetVar("Underwater.TestLaserTracking.Configuration@Search", flip);
    if (flip)
        mState=Search;
    
    double num, denom;
    mXmlConf.GetVar("Underwater.TestLaserTracking.LatDistScale@Num", num);
    mXmlConf.GetVar("Underwater.TestLaserTracking.LatDistScale@Denom", denom);
    mLatDistScale = num/denom;
    
    mXmlConf.GetVar("Underwater.TestLaserTracking.DepthDistScale@Num", num);
    mXmlConf.GetVar("Underwater.TestLaserTracking.DepthDistScale@Denom", denom);
    mDepthDistScale = num/denom;
    
    mXmlConf.GetVar("Underwater.TestLaserTracking.YawDistScale@Num", num);
    mXmlConf.GetVar("Underwater.TestLaserTracking.YawDistScale@Denom", denom);
    mYawDistScale = num/denom;
    
    mXmlConf.GetVar("Underwater.TestLaserTracking.AxialDistScale@Num", num);
    mXmlConf.GetVar("Underwater.TestLaserTracking.AxialDistScale@Denom", denom);
    mAxialDistScale = num/denom;
    
    mXmlConf.GetVar("Underwater.TestLaserTracking.Buoys@MinArea", mMinArea);
    mXmlConf.GetVar("Underwater.TestLaserTracking.Buoys@MaxArea", mMaxArea);
    mXmlConf.GetVar("Underwater.TestLaserTracking.Buoys@TargetArea", mTargetArea);
    
    //mXmlConf.GetVar("Underwater.TestLaserTracking.Laser@CloseDist", mLaserCloseDist);
    
    double mSearchStrafeTimeTmp = mSearchStrafeTime;
    double mSearchStrafeSpeedTmp = mSearchStrafeSpeed;
    double mSearchForwardTimeTmp = mSearchForwardTime;
    double mSearchForwardSpeedTmp = mSearchForwardSpeed;
    double mSearchSmoothSpeedTmp = mSearchSmoothSpeed;
    
    mXmlConf.GetVar("Underwater.TestLaserTracking.Search@StrafeTime", mSearchStrafeTime);
    mXmlConf.GetVar("Underwater.TestLaserTracking.Search@StrafeSpeed", mSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.TestLaserTracking.Search@ForwardTime", mSearchForwardTime);
    mXmlConf.GetVar("Underwater.TestLaserTracking.Search@ForwardSpeed", mSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.TestLaserTracking.Search@SmoothSpeed", mSearchSmoothSpeed);
    
    if (mSearchStrafeTime != mSearchStrafeTimeTmp
        || mSearchStrafeSpeed != mSearchStrafeSpeedTmp
        || mSearchForwardTime != mSearchForwardTimeTmp
        || mSearchForwardSpeed != mSearchForwardSpeedTmp
        || mSearchSmoothSpeed != mSearchSmoothSpeedTmp)
    {
        mSearchPattern.InitSearch(mSearchStrafeTime, mSearchStrafeSpeed, mSearchForwardTime, mSearchForwardSpeed,mSearchSmoothSpeed);
    }
#endif
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrent.mYaw))
    {
        printf("ERROR::TestLaserTracking: Couldn't get Compass data");
        return 1;
    }
    
    // Depth
    if(!mGlobalInfo->GetInfo(GlobalInfo::Laser, mRawLaserData))
    {
        printf("ERROR::TestLaserTracking: Couldn't get laser data");
        return 1;
    }
    
    
    
    //mScanData.CalculateCartesian(mRawLaserData, true, );
    
    // Laser
    /*if(!mGlobalInfo->GetInfo("Laser", mLaserDist))
    {
        printf("ERROR::TestTracking: Couldn't get laser data");
        return 1;
    }*/
    
    /*void CalculateCartesian(const CxUtils::Point3D::List& rangeScan,
                                    bool giveRadius = false,    // Give predicated radius of object based on number of points connected.
                                    double angleRange=270,      // Total scan angle (degrees)
                                    double startAngle = -135,   // Starting angle of scan on platform (where to start) (degrees)
                                    double endAngle = 135);     // Ending angle of scan on plat*/

    //mDebounce.Bounce(mReturn.mValid && mReturn.mArea >= mMinArea && mReturn.mArea <= mMaxArea);
    
    mObjectScanData.CalculateCartesian(mRawLaserData, true, 100, -50, 50);
    
    return 0;
}

std::string TestLaserTracking::ExitEarly()
{
    return "KeepRunning";
}

std::string TestLaserTracking::Execute()
{
    
    cout << "LASER DATA: " << endl;
    cout << "mRawLaserData.size(): " << mRawLaserData.size() << endl;
    //cout << "data: " << endl;
    cvZero(mDisplayLaser);
    double x, y;
    for(int i = 0; i < mRawLaserData.size(); i++)
    {
        //cout << mRawLaserData[i].mZ << ", ";
        //cvPoint(mRawLaserData[i].mX, mRawLaserData[i].mY);
        /*x = (mRawLaserData[i].mX*sin(mRawLaserData[i].mZ))*30;
        y = (mRawLaserData[i].mX*cos(mRawLaserData[i].mZ))*30;
        cvCircle(mDisplayLaser, cvPoint(x+320, y+240), 1, cvScalar(0,255,0), 1);*/
        x = (mRawLaserData[i].mX*(0) - mRawLaserData[i].mY*(-1))*100;
        y = (mRawLaserData[i].mX*(-1) + mRawLaserData[i].mY*(0))*100;
        
        //if(sqrt(mRawLaserData[i].mX*mRawLaserData[i].mX - mRawLaserData[i].mY*mRawLaserData[i].mY) > 0.1)
        {
            cvCircle(mDisplayLaser, cvPoint(x+320, y+470), 1, cvScalar(0,255,0), 1);
        }
        
        //cout << mRawLaserData[i].mZ << ", ";
    }
    //cout << endl;
    
    bool valid;
    mLaserObjects = mObjectScanData.GetObjects(valid, mObjectScanData.BIGGEST);
    cout << "Number of Objects: " << mLaserObjects.size() << endl;
    
    for(int i = 0; i < mLaserObjects.size(); i++)
    {
        x = (mLaserObjects[i].mX*(0) - mLaserObjects[i].mY*(-1))*100;
        y = (mLaserObjects[i].mX*(-1) + mLaserObjects[i].mY*(0))*100;
        cvCircle(mDisplayLaser, cvPoint(x+320, y+470), mLaserObjects[i].mZ*100, cvScalar(0,0,255), 2);
        CvFont font;
        char radius[10];
        sprintf(radius, "Rad: %lf", mLaserObjects[i].mZ*39.37007874015748);
        cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX, 0.5,0.5,0,1);
        cvPutText (mDisplayLaser, radius, cvPoint(x+320, y+470), &font, cvScalar(255,255,0));
    }
    
    cvShowImage("LaserData", mDisplayLaser);
    cvWaitKey(10);
        
    //cout << mRawLaserData. << ", " << endl;
    
    /*cout << "mRawLaserData.size(): " << mRawLaserData.size() << endl;
    static FILE *outstream = fopen("laser.txt", "w");
    for(int i = 0; i < mRawLaserData.size(); i++)
    {
        fprintf(outstream, "%lf,%lf,%lf ", mRawLaserData[i].mX, mRawLaserData[i].mY, mRawLaserData[i].mZ);
    }
    fprintf(outstream, "\n");
    fflush(outstream);*/
    
#if 0
    double centerI = 0;
    double centerJ = 0;
    double area = 0;
    int validcount = 0;
    
    switch (mState)
    {
        case Lateral:
            if (mDebounce.GetDebounced())
            {
                centerI = mReturn.mCenterI;
                centerJ = mReturn.mCenterJ;
                
                mDesired.mYaw = 0;
                mDesired.mLateralThrust = (centerI - mProcessedImage->width/2)*mLatPixelScale;
                mDesired.mDepth = mCurrent.mDepth + (centerJ - mProcessedImage->height/2)*mDepthPixelScale;
                mDesired.mAxialThrust = (mTargetArea - area)*mAxialPixelScale;
                
                if (mLaserDist <= mLaserCloseDist)
                {
                }
            }
            else
            {
                mDesired.mYaw = 0;
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
                mDesired.mLateralThrust = 0;
                mDesired.mDepth = mCurrent.mDepth + (centerJ - mProcessedImage->height/2)*mDepthPixelScale;
                mDesired.mAxialThrust = (mTargetArea - area)*mAxialPixelScale;
                
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
            
            std::cout << "State: " << "TestTracking" << "::Search" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " Lateral: " << mDesired.mLateralThrust
                    << " Axial: " << mDesired.mAxialThrust
                    << " DefaultDepth: " << mDefaultDepth
                    << std::endl;
            break;
    }
    
#endif
    
    //AI::Utility::HeadingDisplay(mProcessedImage, mCurrent.mYaw, mDesired.mYaw, 0, 255, 255);
    //AI::Utility::DepthDisplay(mProcessedImage, mCurrent.mDepth, mDesired.mDepth, 0, 24);
    //AI::Utility::ThrustDisplay(mProcessedImage, mDesired.mAxialThrust, mDesired.mLateralThrust);

    
    mGlobalCommand->SetDesiredPitch(mDesired.mPitch);
    mGlobalCommand->SetDesiredRoll(mDesired.mRoll);
    mGlobalCommand->SetDesiredYaw(mDesired.mYaw);
    
    mGlobalCommand->SetDesiredAxialVel(mDesired.mAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesired.mLateralThrust);
    mGlobalCommand->SetDesiredDepth(mDesired.mDepth);
    
    return "KeepRunning";
}
