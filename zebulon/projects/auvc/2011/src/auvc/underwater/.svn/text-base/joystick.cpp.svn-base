#include "auvc/underwater/joystick.h"

using namespace Zebulon;
using namespace Underwater;

Joystick::Joystick()
{
    ID="Joystick";
    mJoystick.Initialize();
    mCurrState=DISABLED;
    
    mDownwardFrame=NULL;
    mForwardFrame=NULL;
}
Joystick::~Joystick()
{
    
}

// Not implemented yet
int Joystick::ErrorCheck()
{
    return 0;
}

// Not implemented yet
int Joystick::FetchProcess()
{
    
    //double GetAxisPercentage(const short int axis, const short int deadzone = 0) const;
    //bool IsButtonPressed(const unsigned int num = 0) const;
    //Look up these values
    mForwardSpeed=-mJoystick.GetAxisPercentage(1);
    mLateralSpeed=mJoystick.GetAxisPercentage(0);
    
    mYawCommand=mJoystick.GetAxisPercentage(2);
    mDesiredDepth=12*4*(100.0-(mJoystick.GetAxisPercentage(3)+100)/2.0)/100.0;
    //mDepthCommand=-mJoystick.GetAxisPercentage(3);

    mGlobalInfo->GetInfo(AI::Names::YawDegrees,mCurrentYaw);
    mGlobalInfo->GetInfo(GlobalInfo::DepthSensor,mCurrentDepth);
    
    mEnable=mJoystick.IsButtonPressed(7);
    mDisable=mJoystick.IsButtonPressed(11);
    //std::cout << mEnable << " " << mDisable << std::endl;
    
    mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera,mDownwardFrame);
    mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera,mForwardFrame);
    
    return 0;
}

// Not implemented yet
std::string Joystick::ExitEarly()
{
    return "KeepRunning";
}

// Not implemented yet
std::string Joystick::Execute()
{
    /*std::cout << "Test " 
        << "Heading " << mCurrentHeading
        << std::endl;*/
    switch(mCurrState)
    {
        case DISABLED:
            std::cout << "Joystick: Disabled\n";
            //same, only because I don't have a direction
            mGlobalCommand->SetDesiredYaw(mCurrentYaw);
            mGlobalCommand->SetDesiredPitch(0);
            mGlobalCommand->SetDesiredRoll(0);
            mGlobalCommand->SetDesiredDepth(mCurrentDepth);
            mGlobalCommand->SetDesiredLateralVel(0.0);
            mGlobalCommand->SetDesiredAxialVel(0.0);
            if(mEnable)
            {
                mCurrState=ENABLED;
            }

            break;
        case ENABLED:
            std::cout << "Joystick: Enabled\n";
            if(mDisable)
            {
                mCurrState=DISABLED;
            }
            //mDesiredYaw=CxUtils::Orientation::AddToAngle(mCurrentYaw,mYawCommand/100.0*60.0,false);
            mDesiredYaw=CxUtils::Orientation::AddToAngle(mDesiredYaw,mYawCommand/100.0*5.0,false);
            
            //mDesiredDepth=mCurrentDepth+mDepthCommand;
            
            /*if(fabs(mDepthCommand)>25.0)
            {
                mDesiredDepth=mDesiredDepth+mDepthCommand*0.5/100.0;
            }*/
             
            std::cout << "Yaw: " << mCurrentYaw << " Desired: " << mDesiredYaw << " Command: " << mYawCommand << std::endl;
            std::cout << "Depth: " << mCurrentDepth << " Desired: " << mDesiredDepth << " Command: " << mDepthCommand << std::endl;
            
            mGlobalCommand->SetDesiredYaw(mDesiredYaw);
            mGlobalCommand->SetDesiredPitch(0);
            mGlobalCommand->SetDesiredRoll(0);
            mGlobalCommand->SetDesiredDepth(mDesiredDepth);
            mGlobalCommand->SetDesiredLateralVel(mLateralSpeed);
            mGlobalCommand->SetDesiredAxialVel(mForwardSpeed);
            break;
    }
    Utility::Display::DisplayImage("ForwardCamera",mForwardFrame);
    Utility::Display::DisplayImage("DownwardCamera",mDownwardFrame);

    return "KeepRunning";
}

int Joystick::Init()
{
    AI::Mission::Init();
    mGlobalInfo->GetInfo(AI::Names::YawDegrees,mCurrentYaw);
    mDesiredYaw=mCurrentYaw;
    mGlobalInfo->GetInfo(GlobalInfo::DepthSensor,mCurrentDepth);
    mDesiredDepth=mCurrentDepth;
    
}