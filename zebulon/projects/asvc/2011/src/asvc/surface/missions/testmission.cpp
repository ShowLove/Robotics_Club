#include "asvc/surface/missions/testmission.h"

using namespace Zebulon;
using namespace ASVC;

const std::string TestMission::Name = "TestMission";

TestMission::TestMission()
{
    ID = Name;
    mLightValue = false;
    mpMainImage = NULL;
}
TestMission::~TestMission()
{
    if(mpMainImage)
    {
        cvReleaseImage(&mpMainImage);
    }
}

// Not implemented yet
int TestMission::ErrorCheck()
{
    return 0;
}

// Not implemented yet
int TestMission::FetchProcess()
{
    mpGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentHeading);
    mpGlobalInfo->GetInfo(AI::Names::Northing,mCurrentNorthing);
    mpGlobalInfo->GetInfo(AI::Names::Easting,mCurrentEasting);

    mpGlobalInfo->GetInfo(GlobalInfo::MainCamera, mpMainImage);
    if(mpMainImage)
    {
        //Utility::Display::DisplayImage(GlobalInfo::MainCamera, mpMainImage);
    }
    CxUtils::Point3D::List scan;
    if(mpGlobalInfo->GetInfo(GlobalInfo::Laser, scan))
    {
        mLaserDisplay.Clear();
        mLaserDisplay.AddScanCartesian(scan);
        //mLaserDisplay.Display();
    }

    return 0;
}

// Not implemented yet
std::string TestMission::ExitEarly()
{
    return "KeepRunning";
}

// Not implemented yet
std::string TestMission::Execute()
{
    double dockEasting = 393768.12907;
    double dockNorthing = 4073551.76493;
    
    double g2Easting = 393734.83279;
    double g2Northing = 4073552.91897;
    
    double bilboEasting = 393719.40821;
    double bilboNorthing = 4073556.25917;
    
    //cout << "Distance: " << AI::Utility::CalcDistance(dockEasting, dockNorthing, bilboEasting, bilboNorthing) << std::endl;
    
    //return "KeepRunning";
//#define TESTTHRUST
#ifdef TESTTHRUST
    
    std::cout << "ForwardSpeed TurnSpeed LateralSpeed: ";
    std::cout.flush();
    
    //std::cin >> mDesiredHeading;
    scanf("%lf %lf %lf %lf",&mForwardSpeed,&mTurnSpeed,&mLateralSpeed, mCannon);
    
    printf("forward:%lf turn:%lf lat:%lf cannon:%lf\n",mForwardSpeed,mTurnSpeed,mLateralSpeed, mCannon);
    //std::cout << "Current: " << mCurrentHeading << " Desired: " << mDesiredHeading << " Diff: " << AI::Utility::AngleDiff(mDesiredHeading,mCurrentHeading) << std::endl;
    mpGlobalCommand->SetTurningThrust(mTurnSpeed);
    //mGlobalCommand->SetDesiredYaw(mTurnSpeed);
    mpGlobalCommand->SetLateralThrust(mLateralSpeed);
    mpGlobalCommand->SetForwardThrust(mForwardSpeed);
    mGlobalCommand->SetWaterCannon(mCannon);
#endif    
    
    
    
#define TESTDIRECTION
#ifdef TESTDIRECTION
    
    std::cout << "ForwardSpeed LateralSpeed DesiredHeading: ";
    std::cout.flush();
    
    //std::cin >> mDesiredHeading;
    scanf("%lf %lf %lf %lf",&mForwardSpeed,&mLateralSpeed, &mDesiredHeading, &mCannon);
    mDesiredHeading=CxUtils::CxToRadians(mDesiredHeading);
    
    //mForwardSpeed = 10;
    //mLateralSpeed = 9;
    //mDesiredHeading = 8;
    
    std::cout << "Current: " << mCurrentHeading << " Desired: " << mDesiredHeading << " Diff: " << AI::Utility::AngleDiff(mDesiredHeading,mCurrentHeading) << std::endl
                 << " lateral: " << mLateralSpeed << " cannon: " << mCannon;
    mpGlobalCommand->SetYaw(mDesiredHeading);
    mpGlobalCommand->SetForwardVelocityPercent(mForwardSpeed);
    mpGlobalCommand->SetLateralVelocityPercent(mLateralSpeed);
    mpGlobalCommand->SetWaterCannon(mCannon);
#endif

//#define TESTPOSITION
#ifdef TESTPOSITION
    //std::cout << mCurrentEasting-mDesiredEasting << " " << mCurrentNorthing-mDesiredNorthing << " " << AI::Utility::CalcDistance(mCurrentEasting,mCurrentNorthing,mDesiredEasting,mDesiredNorthing) << std::endl;
    //std::cout << mDesiredEasting << " " << mDesiredNorthing << std::endl;
    
    mpGlobalCommand->SetYawWgs(28.58562,-81.19956);
    mpGlobalCommand->SetForwardVelocity(2.0);
#endif

//#define TESTPT
#ifdef TESTPT

    scanf("%lf %lf",&mTilt,&mPan);
    mpGlobalCommand->SetPanPosition(mPan);
    mpGlobalCommand->SetTiltPosition(mTilt);

#endif

    return "KeepRunning";

    mpGlobalCommand->SetComputerLightOne(mLightValue);
    mLightValue = !mLightValue;

    return "KeepRunning";
}

int TestMission::Init()
{
    AI::Mission::Init();
    //CxUtils::SleepMs(1000);
    mpGlobalInfo->GetInfo(AI::Names::YawDegrees, mDesiredHeading);
    mpGlobalInfo->GetInfo(AI::Names::Northing,mDesiredNorthing);
    mpGlobalInfo->GetInfo(AI::Names::Easting, mDesiredEasting);
}
