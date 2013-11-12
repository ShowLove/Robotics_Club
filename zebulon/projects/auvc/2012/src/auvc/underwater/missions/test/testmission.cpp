#include "auvc/underwater/missions/test/testmission.h"

//#define TESTTHRUST
//#define TESTTHRUSTANGLE
#define TESTMCU
//#define TESTDIRECTION
//#define SETPOSITION
//#define SQUARE
//#define DISPLAYTEST
//#define BARREL_ROLL
//#define SINEFORWARD


using namespace Zebulon;
using namespace Underwater;

TestMission::TestMission()
{
    ID="TestMission";
    mLightValue = false;
}
TestMission::~TestMission()
{
    
}

// Not implemented yet
int TestMission::ErrorCheck()
{
    return 0;
}

// Not implemented yet
int TestMission::FetchProcess()
{
    int haveData=1;
    haveData&=mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentHeading);
    haveData&=mGlobalInfo->GetInfo(AI::Names::PitchDegrees, mCurrentPitch);
    haveData&=mGlobalInfo->GetInfo(AI::Names::RollDegrees, mCurrentRoll);
    haveData&=mGlobalInfo->GetInfo("DepthSensor",mCurrentDepth);
    return !haveData;
}

// Not implemented yet
std::string TestMission::ExitEarly()
{
    return "KeepRunning";
}

// Not implemented yet
std::string TestMission::Execute()
{

#ifdef SINEFORWARD
    mGlobalCommand->SetDesiredPitch(0);
    mGlobalCommand->SetDesiredRoll(0);
    mGlobalCommand->SetDesiredDepth(30);
    static double t=0.0;
    //lock heading from when mission started
    static double heading=mCurrentHeading;
    mGlobalCommand->SetDesiredYaw(heading);
    //smaller is slower
    t+=0.5/20.0;
    double out=100.0*sin(t);
    mGlobalCommand->SetDesiredAxialVel(out);
    mGlobalCommand->SetDesiredLateralVel(0.0);
    printf("Sine: %lf\n",out);
#endif


#ifdef BARREL_ROLL
    mGlobalCommand->SetDesiredPitch(0);
    mGlobalCommand->SetDesiredRoll(0);
    mGlobalCommand->SetDesiredYaw(90);
    mGlobalCommand->SetDesiredDepth(30);
    mGlobalCommand->SetDesiredAxialVel(0);
    
    static int count = 1;
    static int flip = 160;
    
    std::cout << "Yaw: " << mCurrentHeading << " ";
    std::cout << "Pitch: " << mCurrentPitch << " ";

    if (count % flip > flip/2)
    {
        mGlobalCommand->SetDesiredLateralVel(100);
        std::cout << "+100  " << count << std::endl;
    }
    else
    {
        mGlobalCommand->SetDesiredLateralVel(-100);
        std::cout << "-100  " << count << std::endl;
    }
    count++;
    
    
    
#endif // BARREL_ROLL
    
    
    /*std::cout << "Test " 
        << "Heading " << mCurrentHeading
        << std::endl;*/

#ifdef TESTTHRUST
    mGlobalCommand->SetDesiredPitch(0);
    mGlobalCommand->SetDesiredRoll(0);
    mGlobalCommand->SetDesiredYaw(-140);
    //mGlobalCommand->SetDesiredDepth(0);
    
    std::cout << "ForwardSpeed LateralSpeed Set Depth: ";
    std::cout.flush();
    //std::cin >> mDesiredHeading;
    ////scanf("%lf %lf %lf", &mForwardSpeed, &mLateralSpeed, &mVerticalSpeed);
    ////printf("%lf %lf %lf\n", mForwardSpeed, mLateralSpeed, mVerticalSpeed);
    scanf("%lf %lf %lf", &mForwardSpeed, &mLateralSpeed, &mDesiredDepth);
    printf("%lf %lf %lf\n", mForwardSpeed, mLateralSpeed, mDesiredDepth);
    //std::cout << "Current: " << mCurrentHeading << " Desired: " << mDesiredHeading << " Diff: " << AI::Utility::AngleDiff(mDesiredHeading,mCurrentHeading) << std::endl;
    mGlobalCommand->SetDesiredLateralVel(mLateralSpeed);
    mGlobalCommand->SetDesiredAxialVel(mForwardSpeed);
    //mGlobalCommand->SetDesiredDepth(mVerticalSpeed);
    mGlobalCommand->SetDesiredDepth(mDesiredDepth);
    
    
#endif

#ifdef TESTTHRUSTANGLE
    std::cout << "YawSpeed PitchSpeed RollSpeed: ";
    std::cout.flush();
    //std::cin >> mDesiredHeading;
    scanf("%lf %lf %lf", &mYawSpeed, &mPitchSpeed, &mRollSpeed);
    printf("%lf %lf %lf\n", mYawSpeed, mPitchSpeed, mRollSpeed);
    //std::cout << "Current: " << mCurrentHeading << " Desired: " << mDesiredHeading << " Diff: " << AI::Utility::AngleDiff(mDesiredHeading,mCurrentHeading) << std::endl;
    mGlobalCommand->SetYawThrust(mYawSpeed);
    mGlobalCommand->SetPitchThrust(mPitchSpeed);
    mGlobalCommand->SetRollThrust(mRollSpeed);
#endif


#ifdef TESTMCU
    std::cout << "Torpedo1 Torpedo2 Dropper1 Dropper2 GrabberArm1 GrabberArm2\n";
    std::cout.flush();
    
    int tmpTor1=0, tmpTor2=0, tmpDrop1=0, tmpDrop2=0, tmpArm1=0, tmpArm2=0;

    //std::cin >> mDesiredHeading;
    scanf("%d %d %d %d %d %d", &tmpTor1, &tmpTor2, &tmpDrop1, &tmpDrop2, &tmpArm1, &tmpArm2);
    printf("%d %d %d %d %d %d\n", tmpTor1, tmpTor2, tmpDrop1, tmpDrop2, tmpArm1, tmpArm2);
    //std::cout << "Current: " << mCurrentHeading << " Desired: " << mDesiredHeading << " Diff: " << AI::Utility::AngleDiff(mDesiredHeading,mCurrentHeading) << std::endl;


    mGlobalCommand->SetTorpedo1(tmpTor1);
    mGlobalCommand->SetTorpedo2(tmpTor2);
    mGlobalCommand->SetDropper1(tmpDrop1);
    mGlobalCommand->SetDropper2(tmpDrop2);
    mGlobalCommand->SetGrabber1(tmpArm1);
    mGlobalCommand->SetGrabber2(tmpArm2);
    
#endif
    
#ifdef TESTDIRECTION
    std::cout << "ForwardSpeed DesiredHeading DesiredDepth: ";
    std::cout.flush();
    mGlobalCommand->SetDesiredPitch(0);
    mGlobalCommand->SetDesiredRoll(0);
    
    //std::cin >> mDesiredHeading;
    scanf("%lf %lf %lf", &mForwardSpeed, &mDesiredHeading, &mDesiredDepth);
    
    std::cout << "Current: " << mCurrentHeading << " Desired: " << mDesiredHeading << " Diff: " << AI::Utility::AngleDiff(mDesiredHeading,mCurrentHeading) << std::endl;
    std::cout << "Current: " << mCurrentDepth << " Desired: " << mDesiredDepth << " Diff: " << AI::Utility::DepthDiff(mCurrentDepth,mDesiredDepth) << std::endl;

    /*mForwardSpeed=0.0;
    mDesiredHeading=0.0;
    mDesiredDepth=.25;*/


    
    mGlobalCommand->SetDesiredYaw(mDesiredHeading);
    mGlobalCommand->SetDesiredAxialVel(mForwardSpeed);
    mGlobalCommand->SetDesiredDepth(mDesiredDepth);
    
#endif

#ifdef SETPOSITION
    std::cout << "DesiredYaw DesiredPitch DesiredRoll DesiredDepth" << std::endl;
    std::cout.flush();
    
    scanf("%lf %lf %lf %lf", &mDesiredHeading, &mDesiredPitch, &mDesiredRoll, &mDesiredDepth);
    
    /*mDesiredHeading = CxUtils::CxToRadians(mDesiredHeading);
    mDesiredPitch = CxUtils::CxToRadians(mDesiredPitch);
    mDesiredRoll = CxUtils::CxToRadians(mDesiredRoll);*/
    
    std::cout << "CurrentYaw: " << mCurrentHeading << " Desired: " << mDesiredHeading << " Diff: " << AI::Utility::AngleDiff(mDesiredHeading, mCurrentHeading) << std::endl;
    std::cout << "CurrentPitch: " << mCurrentPitch << " Desired: " << mDesiredPitch << " Diff: " << AI::Utility::AngleDiff(mDesiredPitch, mCurrentPitch) << std::endl;
    std::cout << "CurrentRoll: " << mCurrentRoll << " Desired: " << mDesiredRoll << " Diff: " << AI::Utility::AngleDiff(mDesiredRoll, mCurrentRoll) << std::endl;
    std::cout << "CurrentDepth: " << mCurrentDepth << " Desired: " << mDesiredDepth << " Diff: " << AI::Utility::AngleDiff(mDesiredDepth, mCurrentDepth) << std::endl;
    mGlobalCommand->SetDesiredYaw(mDesiredHeading);
    mGlobalCommand->SetDesiredPitch(mDesiredPitch);
    mGlobalCommand->SetDesiredRoll(mDesiredRoll);
    mGlobalCommand->SetDesiredDepth(mDesiredDepth);
#endif

#ifdef SQUARE
    if(mSquareWatch.IsFinished())
    {
        mSquareWatch.Reset();
        mDesiredHeading -= 90.0;
        //mDesiredHeading = CxUtils::Orientation::AddToAngle(mDesiredHeading, CxUtils::CxToRadians(90));
        
    }
    
    //mGlobalCommand->SetDesiredAxialVel(40);
    mGlobalCommand->SetDesiredYaw(mDesiredHeading);
    mGlobalCommand->SetDesiredPitch(0);
    mGlobalCommand->SetDesiredRoll(0);
    mGlobalCommand->SetDesiredDepth(25);
    
    std:: cout << " mGlobalCommand->SetDesiredYaw(mDesiredHeading): " << mGlobalCommand->SetDesiredYaw(mDesiredHeading) << std::endl;
    std:: cout << " mDesiredHeading: " << mDesiredHeading << std::endl;
    
    std::cout << "Yaw: " << mCurrentHeading
        << "    Pitch: " << mCurrentPitch
        << "    Roll: " << mCurrentRoll
        << "    Depth: " << mCurrentDepth
        << "    Time: " << mSquareWatch.TimeElapsed() << std::endl;

#endif

#ifdef DISPLAYTEST

    std::cout << "Group Red Green Blue" << std::endl;
    std::cout.flush();

    scanf("%d %lf %lf %lf", &mGroup, &mRed, &mGreen, &mBlue);
    
    switch(mGroup)
    {
        case 0:
            mGlobalCommand->SetDispUpperLeft(mRed, mGreen, mBlue);
            break;
            
        case 1:
            mGlobalCommand->SetDispForwardArrow(mRed, mGreen, mBlue);
            break;
            
        case 2:
            mGlobalCommand->SetDispUpperRight(mRed, mGreen, mBlue);
            break;
            
        case 3:
            mGlobalCommand->SetDispLeftArrow(mRed, mGreen, mBlue);
            break;
            
        case 4:
            mGlobalCommand->SetDispCenter(mRed, mGreen, mBlue);
            break;
            
        case 5:
            mGlobalCommand->SetDispRightArrow(mRed, mGreen, mBlue);
            break;
        
        case 6:
            mGlobalCommand->SetDispLowerLeft(mRed, mGreen, mBlue);
            break;
        
        case 7:
            mGlobalCommand->SetDispBackArrow(mRed, mGreen, mBlue);
            break;
        
        case 8:
            mGlobalCommand->SetDispLowerRight(mRed, mGreen, mBlue);
            break;
    }

#endif

    std::cout << "YAW: " << mCurrentHeading << ", PITCH: " << mCurrentPitch << ", ROLL: " << mCurrentRoll << ", DEPTH: " << mCurrentDepth << std::endl;


    return "KeepRunning";
}

int TestMission::Init()
{
    AI::Mission::Init();
    mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentHeading);
    mGlobalInfo->GetInfo(AI::Names::PitchDegrees, mCurrentPitch);
    mGlobalInfo->GetInfo(AI::Names::RollDegrees, mCurrentRoll);
    mGlobalInfo->GetInfo("DepthSensor",mCurrentDepth);
    
    mSquareWatch.Initialize(10.0);
    mDesiredHeading = 0.0;

}
