#include "asvc/surface/missions/testmission.h"
#include "cxutils/math/coordinates.h"
using namespace Zebulon;
using namespace ASVC;

const std::string TestMission::Name = "TestMission";

TestMission::TestMission()
{
    ID = Name;
    mLightValue = false;
    mpMainImage = NULL;
    Utility::Display::SetDisplay(true);
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
    if(mpGlobalInfo->GetPylonCamera())
    {
        mpGlobalInfo->GetPylonCamera()->LoadSettings("settings/pylon_settings.txt");
    }
    mpGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentHeading);
    mpGlobalInfo->GetInfo(AI::Names::Northing,mCurrentNorthing);
    mpGlobalInfo->GetInfo(AI::Names::Easting,mCurrentEasting);
    double temp, dyna, motorvolt, logicvolt = 0;
    mpGlobalInfo->GetMicroInfo(Microcontroller::Temperature,temp);
    mpGlobalInfo->GetMicroInfo(Microcontroller::DynamixelActual, dyna);
    mpGlobalInfo->GetMicroInfo(Microcontroller::VoltageMotors,motorvolt);
    mpGlobalInfo->GetMicroInfo(Microcontroller::VoltageLogic,logicvolt);
    std::cout << "Temperature: " << temp << std::endl;
    std::cout << "Dynamix Angle: " << dyna << std::endl;
    std::cout << "VoltageMotors: " << motorvolt << std::endl;
    std::cout << "VoltageElectronics: " << logicvolt << std::endl;

//    cv::Mat img;
//    if(mpGlobalInfo->GetFrame(img))
//    {
//        cv::imshow("test out", img);
//        cv::waitKey(1);

//    }
//    mpGlobalInfo->GetFrame(mpMainImage);

//    if(mpMainImage)
//    {

//        Utility::Display::DisplayImage(GlobalInfo::MainCamera, mpMainImage);
//        //cvShowImage("blah", mpMainImage);
//        //cvWaitKey(1);
//    }
    if(mpGlobalInfo->GetInfo(GlobalInfo::MainCamera,mpMainImage))
    {
        Utility::Display::DisplayImage(GlobalInfo::MainCamera, mpMainImage);
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
    scanf("%lf %lf %lf",&mForwardSpeed,&mTurnSpeed,&mLateralSpeed);
    
    printf("forward:%lf turn:%lf lat:%lf\n",mForwardSpeed,mTurnSpeed,mLateralSpeed);
    //std::cout << "Current: " << mCurrentHeading << " Desired: " << mDesiredHeading << " Diff: " << AI::Utility::AngleDiff(mDesiredHeading,mCurrentHeading) << std::endl;
    mpGlobalCommand->SetTurningThrust(mTurnSpeed);
    //mGlobalCommand->SetDesiredYaw(mTurnSpeed);
    mpGlobalCommand->SetLateralThrust(mLateralSpeed);
    mpGlobalCommand->SetForwardThrust(mForwardSpeed);
#endif    
    
    
    
//#define TESTDIRECTION
#ifdef TESTDIRECTION
    
    std::cout << "SetForwardVelocityPercent SetLateralVelocityPercent SetYaw: ";
    std::cout.flush();
    
    //std::cin >> mDesiredHeading;
    scanf("%lf %lf %lf",&mForwardSpeed,&mLateralSpeed, &mDesiredHeading);
    //mDesiredHeading=CxUtils::CxToRadians(mDesiredHeading);
    mDesiredHeading=CxUtils::CxToRadians(-106);
    
    //mForwardSpeed = 10;
    //mLateralSpeed = 9;
    //mDesiredHeading = 8;
    
    std::cout << "Current: " << mCurrentHeading << " Desired: " << mDesiredHeading << " Diff: " << AI::Utility::AngleDiff(mDesiredHeading,mCurrentHeading) << std::endl
                 << " lateral: " << mLateralSpeed << " cannon: " << mCannon;
    mpGlobalCommand->SetYaw(mDesiredHeading);
    mpGlobalCommand->SetForwardVelocityPercent(mForwardSpeed);
    mpGlobalCommand->SetLateralVelocityPercent(mLateralSpeed);
    //mpGlobalCommand->SetWaterCannon(mCannon);
#endif

//#define TESTPOSITION
#ifdef TESTPOSITION
    //std::cout << mCurrentEasting-mDesiredEasting << " " << mCurrentNorthing-mDesiredNorthing << " " << AI::Utility::CalcDistance(mCurrentEasting,mCurrentNorthing,mDesiredEasting,mDesiredNorthing) << std::endl;
    //std::cout << mDesiredEasting << " " << mDesiredNorthing << std::endl;
    CxUtils::Wgs istlakecenter(28.585655,-81.199675,0);
    if(CxUtils::Wgs::GreatCircleDistance(mpGlobalInfo->GetPositionWgs(),istlakecenter) < 1)
    {
        mpGlobalCommand->SetYaw(mDesiredHeading);
        mpGlobalCommand->SetForwardVelocity(0);
    }
    else
    {
        mpGlobalCommand->SetYawWgs(istlakecenter);
        mpGlobalCommand->SetForwardVelocity(2.0);
    }
#endif

//#define TESTPT
#ifdef TESTPT

    scanf("%lf %lf",&mTilt,&mPan);
    mpGlobalCommand->SetPanPosition(mPan);
    mpGlobalCommand->SetTiltPosition(mTilt);

#endif

#define ACTUATORS
#ifdef ACTUATORS
    double angle, actuator, pump;
    std::cout << "Dynamixel(degree)    Actuator(percent)    Pump(1/0): ";
    std::cout.flush();

    //std::cin >> mDesiredHeading;
    scanf("%lf %lf %lf",&angle,&actuator, &pump);
     mpGlobalCommand->SetActuator(actuator);
     mpGlobalCommand->SetPump(pump==1?true:false);
     mpGlobalCommand->SetDynamixel(CxUtils::CxToRadians(angle));
#endif

    return "KeepRunning";

    //mpGlobalCommand->SetComputerLightOne(mLightValue);
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
    if(mpGlobalInfo->GetPylonCamera())
    {
        mpGlobalInfo->GetPylonCamera()->LoadSettings("settings/pylon_settings.txt");
        mpGlobalInfo->GetPylonCamera()->SetWhiteBalanceMode(1);
        CxUtils::SleepMs(2000);
        mpGlobalInfo->GetPylonCamera()->SetWhiteBalanceMode(0);
    }
}
