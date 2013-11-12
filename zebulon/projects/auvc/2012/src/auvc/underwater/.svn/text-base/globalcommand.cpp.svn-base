#include "auvc/underwater/globalcommand.h"
#include "auvc/underwater/globalinfo.h"
#include <jaus/mobility/drivers/primitivedriver.h>
#include <statemachine/utility/utility.h>
#include <statemachine/names.h>
#include <jaus/core/management/management.h>
#include "jaus/extras/mcu/setmicrocontrollerstate.h"

#include "auvc/underwater/globalcommand.h"
#include <iostream>

using namespace Zebulon;
using namespace Underwater;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalCommand::GlobalCommand()
{
    mDelayTimeMs = 50;
    mCommandMode = AI::GlobalCommand::Power;
    
    /*GetVelocityX();
    GetVelocityY();
    GetPositionZ();
    GetYaw();
    GetPitch();
    GetRoll();*/
    
    
    //Set all control values to flat
    mDesiredYaw=0;
    mDesiredPitch=0;
    mDesiredRoll=0;
    mDesiredDepth=0;
    mDesiredAxialVel=0;
    mDesiredLateralVel=0;
    mControlReady=false;
    
    //Assume all solenoids off
    mTorpedo1=false;
    mTorpedo2=false;
    mDropper1=false;
    mDropper2=false;
    mGrabber1=false;
    mGrabber2=false;
    
    //Assume all ticks start at zero (not fired)
    mTorpedo1Ticks=0;
    mTorpedo2Ticks=0;
    mDropper1Ticks=0;
    mDropper2Ticks=0;
    mGrabber1Ticks=0;
    mGrabber2Ticks=0;
    
    mSurfaceFlag=false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalCommand::~GlobalCommand()
{
    mCommandThread.StopThread(500);
    ReleaseComponentControl(JAUS::Address(),true);
}

/*void GlobalCommand::ProcessAcquisitionOfControl(const JAUS::Address& controlledComponent)
{
    for(unsigned int i = 0; i < 3; i++)
    {
        if(GetComponent()->ManagementService()->Resume(controlledComponent, 750))
        {
            break;
        }
    }
}

void GlobalCommand::ProcessLossOfControl(const JAUS::Address& controlledComponent)
{
}*/

double GlobalCommand::ConvertInchesToMeters(double inches)
{
    return inches*(0.0254);
}

double GlobalCommand::ConvertFeetToMeters(double feet)
{
    return feet*(0.3048);
}

int GlobalCommand::SetDesiredDepth(double depth)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    //Cap depth to not allow for surfacing
    if (depth < mCeilingDepth)
    {
        depth = mCeilingDepth;
    }
    else if (depth > mFloorDepth)
    {
        depth = mFloorDepth;
    }
//    static FILE* tmpstream = fopen("depthcmd.txt","w");
//    fprintf(tmpstream,"%lf\n",depth);
//    fflush(tmpstream);
    //SetDispCenter(0, (depth/mFloorDepth) * 254.0, (1 - (depth/mFloorDepth)) * 254.0);
    mDesiredDepth = ConvertInchesToMeters(depth);
    //mDesiredDepth = depth;
    //std::cout << "mDesiredDepth: " << mDesiredDepth << std::endl;
    mControlReady = true;
}

int GlobalCommand::Surface()
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mDesiredDepth = 0;
    mSurfaceFlag=true;
    mControlReady = true;
}

int GlobalCommand::SetDesiredAxialVel(double velocity)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mCommandMode = AI::GlobalCommand::Power;
    if(velocity > 100)
    {
        velocity = 100;
    }
    else if(velocity < -100)
    {
        velocity = -100;
    }
    
    /*if (velocity > 0)
    {
        SetDispForwardArrow(0, 254.0 * velocity/100.0, 254.0 * velocity/100.0);
        SetDispBackArrow(0,0,0);
    }
    else
    {
        SetDispBackArrow(0, 254.0 * velocity/100.0, 254.0 * velocity/100.0);
        SetDispForwardArrow(0,0,0);
    }*/
    
    mDesiredAxialVel = velocity;
    //std::cout << "mDesiredAxialVel: " << mDesiredDepth << std::endl;
    mControlReady = true;
}

int GlobalCommand::SetDesiredLateralVel(double velocity)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mCommandMode = AI::GlobalCommand::Power;
    if(velocity > 100)
    {
        velocity = 100;
    }
    else if(velocity < -100)
    {
        velocity = -100;
    }
    
    /*if (velocity > 0)
    {
        SetDispRightArrow(0, 254.0 * velocity/100.0, 254.0 * velocity/100.0);
        SetDispLeftArrow(0,0,0);
    }
    else
    {
        SetDispLeftArrow(0, 254.0 * velocity/100.0, 254.0 * velocity/100.0);
        SetDispRightArrow(0,0,0);
    }*/
    
    mDesiredLateralVel = velocity;
    mControlReady = true;
}

int GlobalCommand::SetDesiredPitch(double pitch)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mCommandMode = AI::GlobalCommand::Power;
    mDesiredPitch = pitch;
    mControlReady = true;
}

int GlobalCommand::SetDesiredRoll(double roll)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mCommandMode = AI::GlobalCommand::Power;
    mDesiredRoll = roll;
    //std::cout << "mDesiredRoll: " << mDesiredRoll << std::endl;
    mControlReady = true;
}

int GlobalCommand::SetDesiredYaw(double yaw)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mCommandMode = AI::GlobalCommand::Power;
    mDesiredYaw = CxUtils::Orientation::AddToAngle(CxUtils::CxToRadians(yaw),0);
    //std::cout << "mDesiredYaw: " << mDesiredYaw << std::endl;
    mControlReady = true;
}

int GlobalCommand::SetTorpedo1(bool value)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mTorpedo1 = value;
    
    /*if (value)
    {
        SetDispUpperLeft(255, 0, 255);
        SetDispUpperRight(255, 0, 255);
        SetDispLowerLeft(255, 0, 255);
        SetDispLowerRight(255, 0, 255);
    }
    else
    {
        SetDispUpperLeft(0, 0, 0);
        SetDispUpperRight(0, 0, 0);
        SetDispLowerLeft(0, 0, 0);
        SetDispLowerRight(0, 0, 0);
    }*/
}

int GlobalCommand::SetTorpedo2(bool value)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mTorpedo2 = value;
    
    /*if (value)
    {
        SetDispUpperLeft(255, 255, 255);
        SetDispUpperRight(255, 255, 255);
        SetDispLowerLeft(255, 255, 255);
        SetDispLowerRight(255, 255, 255);
    }
    else
    {
        SetDispUpperLeft(0, 0, 0);
        SetDispUpperRight(0, 0, 0);
        SetDispLowerLeft(0, 0, 0);
        SetDispLowerRight(0, 0, 0);
    }*/
}

int GlobalCommand::SetDropper1(bool value)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mDropper1 = value;
    
    /*if (value)
    {
        SetDispUpperLeft(255, 0, 0);
        SetDispUpperRight(255, 0, 0);
        SetDispLowerLeft(255, 0, 0);
        SetDispLowerRight(255, 0, 0);
    }
    else
    {
        SetDispUpperLeft(0, 0, 0);
        SetDispUpperRight(0, 0, 0);
        SetDispLowerLeft(0, 0, 0);
        SetDispLowerRight(0, 0, 0);
    }*/
}

int GlobalCommand::SetDropper2(bool value)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mDropper2 = value;
    
    /*if (value)
    {
        SetDispUpperLeft(255, 255, 0);
        SetDispUpperRight(255, 255, 0);
        SetDispLowerLeft(255, 255, 0);
        SetDispLowerRight(255, 255, 0);
    }
    else
    {
        SetDispUpperLeft(0, 0, 0);
        SetDispUpperRight(0, 0, 0);
        SetDispLowerLeft(0, 0, 0);
        SetDispLowerRight(0, 0, 0);
    }*/
}

int GlobalCommand::SetGrabber1(bool value)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mGrabber1 = value;
    
    /*if (value)
    {
        SetDispUpperLeft(0, 255, 0);
        SetDispUpperRight(0, 255, 0);
        SetDispLowerLeft(0, 255, 0);
        SetDispLowerRight(0, 255, 0);
    }
    else
    {
        SetDispUpperLeft(0, 0, 0);
        SetDispUpperRight(0, 0, 0);
        SetDispLowerLeft(0, 0, 0);
        SetDispLowerRight(0, 0, 0);
    }*/
}

int GlobalCommand::SetGrabber2(bool value)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mGrabber2 = value;
    
    /*if (value)
    {
        SetDispUpperLeft(0, 255, 255);
        SetDispUpperRight(0, 255, 255);
        SetDispLowerLeft(0, 255, 255);
        SetDispLowerRight(0, 255, 255);
    }
    else
    {
        SetDispUpperLeft(0, 0, 0);
        SetDispUpperRight(0, 0, 0);
        SetDispLowerLeft(0, 0, 0);
        SetDispLowerRight(0, 0, 0);
    }*/
}

bool GlobalCommand::FireUpdate(bool &mDevice,int &mTicks)
{
    //if not wanting to fire, constantly reset ticks
    if(mDevice==false)
    {
        mTicks=0;
    }
    //otherwise to fire, keep going
    else
    {
        //increament tick count
        mTicks++;
    }
    //check if still should be firing, and the desire is to fire
    if(mTicks<=mFireTicks && mDevice==true)
    {
        return true;
    }
    else
    {
        return false;
    }
        
}

/*
int GlobalCommand::SetDispUpperLeft(double red, double green, double blue)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mUpperLeftRed = red;
    mUpperLeftGreen = green;
    mUpperLeftBlue = blue;
    
    return true;
}sb


int GlobalCommand::SetDispForwardArrow(double red, double green, double blue)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mForwardArrowRed = red;
    mForwardArrowGreen = green;
    mForwardArrowBlue = blue;
    
    return true;
}


int GlobalCommand::SetDispUpperRight(double red, double green, double blue)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mUpperRightRed = red;
    mUpperRightGreen = green;
    mUpperRightBlue = blue;
    
    return true;
}


int GlobalCommand::SetDispLeftArrow(double red, double green, double blue)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mLeftArrowRed = red;
    mLeftArrowGreen = green;
    mLeftArrowBlue = blue;
    
    return true;
}


int GlobalCommand::SetDispCenter(double red, double green, double blue)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mCenterRed = red;
    mCenterGreen = green;
    mCenterBlue = blue;
    
    return true;
}


int GlobalCommand::SetDispRightArrow(double red, double green, double blue)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mRightArrowRed = red;
    mRightArrowGreen = green;
    mRightArrowBlue = blue;
    
    return true;
}


int GlobalCommand::SetDispLowerLeft(double red, double green, double blue)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mLowerLeftRed = red;
    mLowerLeftGreen = green;
    mLowerLeftBlue = blue;
    
    return true;
}


int GlobalCommand::SetDispBackArrow(double red, double green, double blue)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mBackArrowRed = red;
    mBackArrowGreen = green;
    mBackArrowBlue = blue;
    
    return true;
}


int GlobalCommand::SetDispLowerRight(double red, double green, double blue)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mLowerRightRed = red;
    mLowerRightGreen = green;
    mLowerRightBlue = blue;
    
    return true;
}
*/

/** Loop for generating drive commands. */
void GlobalCommand::CommandThread()
{
    JAUS::AccessControl* control = NULL;
    JAUS::Management* management = NULL;
    control = GetComponent()->AccessControlService();
    management = GetComponent()->ManagementService();
    while(mCommandThread.QuitThreadFlag() == false && control)
    {
        
        JAUS::Address baselineComponentID;
        JAUS::Address driverToControl;
        JAUS::Message* driveCommandToSend = NULL;
        
        if(baselineComponentID.IsValid() == false)
        {
            //JAUS::Address::List idList = GetComponent()->DiscoveryService()->GetSubsystem(GetComponentID().mSubsystem)->GetComponentsWithService(JAUS::PrimitiveDriver::Name);
            JAUS::Address::List idList = GetComponent()->DiscoveryService()->GetSubsystem(GetComponentID().mSubsystem)->GetComponentsWithService(JAUS::Microcontroller::Name);
            if(idList.size() > 0)
            {
                baselineComponentID = idList.front();
            }
        }
        if(baselineComponentID.IsValid() == false)
        {
            CxUtils::SleepMs(1);
            //std::cout << "Baseline Invalid\n";
            continue;
        }
        
        // Take control of the correct component based on the
        // type of control mode we are.  If Power, control Primitive Driver
        // component, and release control of Velocity State Driver.  Do the
        // reverse for Velocity State Driver
        
        JAUS::SetVelocityVector setVelocityVector;
        JAUS::SetMicrocontrollerState setMicrocontrollerState;
        mCommandMutex.Lock();
        if(mControlReady==true)
        {
            //Control the three orientation angles
            setVelocityVector.SetRoll(mDesiredRoll);
            setVelocityVector.SetPitch(mDesiredPitch);
            setVelocityVector.SetYaw(mDesiredYaw);
            
            //Set the depth in meters, if surface called, force surface
            if(mSurfaceFlag)
            {
                setVelocityVector.SetPositionZ(0);
            }
            else
            {
                setVelocityVector.SetPositionZ(mDesiredDepth);
            }
            //Set the velocities
            setVelocityVector.SetVelocityX(mDesiredAxialVel);
            setVelocityVector.SetVelocityY(mDesiredLateralVel);
        }
        mCommandMutex.Unlock();
        setVelocityVector.SetSourceID(mGlobalInfo->GetComponentID());
        setVelocityVector.SetDestinationID(baselineComponentID);
        //setVelocityVector.Print();
        
        mCommandMutex.Lock();
        (*setMicrocontrollerState.GetDigitalStates())["Torpedo1"] = FireUpdate(mTorpedo1,mTorpedo1Ticks);
        (*setMicrocontrollerState.GetDigitalStates())["Torpedo2"] = FireUpdate(mTorpedo2,mTorpedo2Ticks);
        (*setMicrocontrollerState.GetDigitalStates())["Dropper1"] = FireUpdate(mDropper1,mDropper1Ticks);
        (*setMicrocontrollerState.GetDigitalStates())["Dropper2"] = FireUpdate(mDropper2,mDropper2Ticks);
        (*setMicrocontrollerState.GetDigitalStates())["GrabberArm1"] = FireUpdate(mGrabber1,mGrabber1Ticks);
        (*setMicrocontrollerState.GetDigitalStates())["GrabberArm2"] = FireUpdate(mGrabber2,mGrabber2Ticks);
        
        //setMicrocontrollerState
        mCommandMutex.Unlock();
        
/*        mCommandMutex.Lock();
        bool estop;
        mGlobalInfo->GetInfo(GlobalInfo::EStop, estop);
        if (!estop)
        {
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Upper_Left_Red"] = 255;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Upper_Left_Green"] = 0;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Upper_Left_Blue"] = 0;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Forward_Arrow_Red"] = 255;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Forward_Arrow_Green"] = 0;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Forward_Arrow_Blue"] = 0;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Upper_Right_Red"] = 255;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Upper_Right_Green"] = 0;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Upper_Right_Blue"] = 0;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Left_Arrow_Red"] = 255;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Left_Arrow_Green"] = 0;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Left_Arrow_Blue"] = 0;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Center_Red"] = 255;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Center_Green"] = 0;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Center_Blue"] = 0;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Right_Arrow_Red"] = 255;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Right_Arrow_Green"] = 0;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Right_Arrow_Blue"] = 0;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Lower_Left_Red"] = 255;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Lower_Left_Green"] = 0;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Lower_Left_Blue"] = 0;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Back_Arrow_Red"] = 255;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Back_Arrow_Green"] = 0;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Back_Arrow_Blue"] = 0;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Lower_Right_Red"] = 255;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Lower_Right_Green"] = 0;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Lower_Right_Blue"] = 0;
        }
        else
        {
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Upper_Left_Red"] = mUpperLeftRed;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Upper_Left_Green"] = mUpperLeftGreen;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Upper_Left_Blue"] = mUpperLeftBlue;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Forward_Arrow_Red"] = mForwardArrowRed;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Forward_Arrow_Green"] = mForwardArrowGreen;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Forward_Arrow_Blue"] = mForwardArrowBlue;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Upper_Right_Red"] = mUpperRightRed;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Upper_Right_Green"] = mUpperRightGreen;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Upper_Right_Blue"] = mUpperRightBlue;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Left_Arrow_Red"] = mLeftArrowRed;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Left_Arrow_Green"] = mLeftArrowGreen;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Left_Arrow_Blue"] = mLeftArrowBlue;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Center_Red"] = mCenterRed;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Center_Green"] = mCenterGreen;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Center_Blue"] = mCenterBlue;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Right_Arrow_Red"] = mRightArrowRed;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Right_Arrow_Green"] = mRightArrowGreen;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Right_Arrow_Blue"] = mRightArrowBlue;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Lower_Left_Red"] = mLowerLeftRed;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Lower_Left_Green"] = mLowerLeftGreen;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Lower_Left_Blue"] = mLowerLeftBlue;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Back_Arrow_Red"] = mBackArrowRed;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Back_Arrow_Green"] = mBackArrowGreen;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Back_Arrow_Blue"] = mBackArrowBlue;
            
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Lower_Right_Red"] = mLowerRightRed;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Lower_Right_Green"] = mLowerRightGreen;
            (*setMicrocontrollerState.GetAnalogStates())["Disp_Lower_Right_Blue"] = mLowerRightBlue;
        }
        mCommandMutex.Unlock();
*/
        setMicrocontrollerState.SetSourceID(mGlobalInfo->GetComponentID());
        setMicrocontrollerState.SetDestinationID(baselineComponentID);
        
        bool mHaveControl;
        
        //release in surface
        double currDepth;
        mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, currDepth);
        if(mSurfaceFlag && currDepth<mSurfaceDepth)
        {
            AccessControlService()->ReleaseComponentControl(baselineComponentID);
        }
        else if(!AccessControlService()->HaveControl(baselineComponentID))
        {
            AccessControlService()->RequestComponentControl(baselineComponentID);
        }
        
        //Don't send Microcontroller for now
        mHaveControl &= SendCommand(&setMicrocontrollerState, true);

        //mHaveControl &= SendCommand(&setWrenchEffort, true);
        if(mControlReady==true)
        {
            mHaveControl &= SendCommand(&setVelocityVector, true);
        }
        
        //std::cout << "Here: " << std::endl;
        //commandLoop++;
        
        CxUtils::SleepMs(mDelayTimeMs);
        
        
        /*if(mCommandMode == AI::GlobalCommand::Power)
        {
            driverToControl = mPrimitiveDriverComponentID;
            if(control->HaveControl(mVelocityStateDriverComponentID))
            {
                control->ReleaseComponentControl(mVelocityStateDriverComponentID);
            }
            if(!control->HaveControl(driverToControl))
            {
                this->RequestComponentControl(driverToControl, true, true);
            }
            driveCommandToSend = &mWrenchEffort;
        }
        else if(mCommandMode == AI::GlobalCommand::Velocity)
        {
            driverToControl = mVelocityStateDriverComponentID;
            if(control->HaveControl(mPrimitiveDriverComponentID))
            {
                control->ReleaseComponentControl(mPrimitiveDriverComponentID);
            }
            if(!control->HaveControl(driverToControl))
            {
                this->RequestComponentControl(driverToControl, true, true);
            }
            if(mStandardsModeFlag)
            {
                driveCommandToSend = &mVelocityCommand;
            }
            else
            {
                driveCommandToSend = &mVelocityVector;
            }
        }*/
        
        /*******driverToControl = mVelocityStateDriverComponentID;
        if(control->HaveControl(mPrimitiveDriverComponentID))
        {
            control->ReleaseComponentControl(mPrimitiveDriverComponentID);
        }
        if(!control->HaveControl(driverToControl))
        {
            this->RequestComponentControl(driverToControl, true, true);
        }
        
        driveCommandToSend = &mVelocityVector;******/
        
        
        /*if(mStandardsModeFlag)
        {
            driveCommandToSend = &mVelocityCommand;
        }
        
        if(control->HaveControl(mVelocityStateDriverComponentID))
        {
            driveCommandToSend = &mVelocityVector;
        }*/
        
        // Send drive command.
        /******if(driveCommandToSend)
        {
            CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
            driveCommandToSend->SetSourceID(GetComponentID());
            driveCommandToSend->SetDestinationID(driverToControl);
            Send(driveCommandToSend);
        }
        
        CxUtils::SleepMs(mDelayTimeMs);*****/
    }
}

/** Prints status to console window. */
void GlobalCommand::PrintStatus() const
{
    AI::GlobalCommand::PrintStatus();
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    /*switch(mCommandMode)
    {
    case AI::GlobalCommand::Power:
        mWrenchEffort.PrintMessageBody();
        break;
    default:
        if(mStandardsModeFlag)
        {
            mVelocityCommand.PrintMessageBody();
        }
        else
        {*/
            ////mVelocityVector.PrintMessageBody();
        /*}
    break;
    }*/
}

bool GlobalCommand::Initialize(AI::GlobalInfo* globalInfo, unsigned int delayTimeMs)
{    
    mXmlConfig.GetVar("Ceiling@Depth", mCeilingDepth);
    mXmlConfig.GetVar("Floor@Depth", mFloorDepth);
    mXmlConfig.GetVar("Surface@Depth", mSurfaceDepth);    
    
    return AI::GlobalCommand::Initialize(globalInfo, delayTimeMs);
}

/*////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts to power control drive mode and sets vehicle thrust.
///
///   \param[in] thrust Vehicle thrust [-100,100]% with positive forward.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetThrust(const double thrust)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mCommandMode = AI::GlobalCommand::Power;
    
    // I think this is where i add the velocity vector stuff
    
    //mWrenchEffort.SetPropulsiveLinearEffortX(thrust);
    mVelocityVector.ClearMessageBody();
    //mVelocityCommand.ClearMessageBody();
}*/

/*void GlobalCommand::CommandThread()
{
    JAUS::Address baselineComponentID;
    mGlobalInfo->GetComponentID().Print();
    int commandLoop=0;
    while (mCommandThread.QuitThreadFlag() == false)
    {
        if(baselineComponentID.IsValid() == false)
        {
            JAUS::Address::List idList = GetComponent()->DiscoveryService()->GetSubsystem(GetComponentID().mSubsystem)->GetComponentsWithService(JAUS::PrimitiveDriver::Name);
            if(idList.size() > 0)
            {
                baselineComponentID = idList.front();
            }
        }
        if(baselineComponentID.IsValid() == false)
        {
            CxUtils::SleepMs(1);
            continue;
        }

        //doesn't exist anymore?
        //JAUS::SetWrenchEffort setWrenchEffort;
        //setWrenchEffort.SetSourceID(mGlobalInfo->GetComponentID());
        //setWrenchEffort.SetDestinationID(baselineComponentID);
        //setWrenchEffort.Print();

        JAUS::SetVelocityVector setVelocityVector;
        JAUS::SetMicrocontrollerState setMicrocontrollerState;
        mCommandMutex.Lock();
        if(mControlReady==true)
        {
            //Control the three orientation angles
            setVelocityVector.SetRoll(mDesiredRoll);
            setVelocityVector.SetPitch(mDesiredPitch);
            setVelocityVector.SetYaw(mDesiredYaw);
            
            //Set the depth in meters
            setVelocityVector.SetPositionZ(mDesiredDepth);
            
            //Set the velocities
            setVelocityVector.SetVelocityX(mDesiredAxialVel);
            setVelocityVector.SetVelocityY(mDesiredLateralVel);
        }
        mCommandMutex.Unlock();
        setVelocityVector.SetSourceID(mGlobalInfo->GetComponentID());
        setVelocityVector.SetDestinationID(baselineComponentID);
        setVelocityVector.Print();
        
        mCommandMutex.Lock();
        (*setMicrocontrollerState.GetDigitalStates())["Torpedo1"] = FireUpdate(mTorpedo1,mTorpedo1Ticks);
        (*setMicrocontrollerState.GetDigitalStates())["Torpedo2"] = FireUpdate(mTorpedo2,mTorpedo2Ticks);
        (*setMicrocontrollerState.GetDigitalStates())["Dropper1"] = FireUpdate(mDropper1,mDropper1Ticks);
        (*setMicrocontrollerState.GetDigitalStates())["Dropper2"] = FireUpdate(mDropper2,mDropper2Ticks);
        (*setMicrocontrollerState.GetDigitalStates())["GrabberArm"] = FireUpdate(mGrabber,mGrabberTicks);
        (*setMicrocontrollerState.GetDigitalStates())["Extra"] = FireUpdate(mExtraDigital,mExtraDigitalTicks);
        
        mCommandMutex.Unlock();
                
        setMicrocontrollerState.SetSourceID(mGlobalInfo->GetComponentID());
        setMicrocontrollerState.SetDestinationID(baselineComponentID);
        
        bool mHaveControl;
        
        if(!AccessControlService()->HaveControl(baselineComponentID))
        {
            AccessControlService()->RequestComponentControl(baselineComponentID);
        }
        GetComponent()->ManagementService()->Resume(baselineComponentID);
        
        mHaveControl &= SendCommand(&setMicrocontrollerState, true);
        //mHaveControl &= SendCommand(&setWrenchEffort, true);
        mHaveControl &= SendCommand(&setVelocityVector, true);
        
        //std::cout << "Here: " << commandLoop << std::endl;
        commandLoop++;
        
        CxUtils::SleepMs(mDelayTimeMs);
    }
    Shutdown();
}*/

/*GlobalCommand::CommandMode GlobalCommand::GetDriveMode() const
{
}*/



