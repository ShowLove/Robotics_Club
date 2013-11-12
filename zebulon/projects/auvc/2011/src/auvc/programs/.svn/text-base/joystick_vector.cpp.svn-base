
#include <cxutils/joystick.h>
#include <iostream>
#include <jaus/core/management/management.h>
#include <jaus/mobility/drivers/velocitystatedriver.h>
#include <jaus/extras/mobility/setvelocityvector.h>
#include "auvc/underwater/globalcommand.h"
#include <cxutils/cxutils.h>
#include <jaus/core/component.h>
#include <cxutils/fileio.h>
#include <jaus/extras/command/subsystemcommand.h>

enum States
{
    DISABLED,
    ENABLED
};


int main()
{
    States mCurrState;
    //non-callback/events joystick class
    CxUtils::Joystick mJoystick;
            
    double mForwardSpeed;
    double mLateralSpeed;
    
    double mDesiredYaw;
    double mYawCommand;
    
    double mDesiredDepth;
    double mDepthCommand;
    
    int mEnable;
    int mDisable;
    
    //setup joystick
    mJoystick.Initialize();
    mCurrState=DISABLED;

    //JAUS stuff
    JAUS::Component component;
    // Add the services we want our component to have
    // beyond the core set.

    // In this example, we are testing a custom service.
    JAUS::SubsystemCommand* command = new JAUS::SubsystemCommand(JAUS::Service::ID(JAUS::SubsystemCommand::Name));
    // Set up to get global pose data from vehicles every
    // time the pose changes (must be done before initialization).
    command->SetGlobalPoseSubscriptionType(true);

    // Add to component.
    component.AddService(command);

    // Try load settings files.
    // These files determine your UDP network 
    // settings, what Services to turn on/off 
    // or any Service specific settings. See the
    // example file for settings file format.
    component.LoadSettings("settings/services.xml");

    //((JAUS::JUDP *)component.TransportService())->SetInterfaceIP("10.36.14.106");

    // In this example, lets make an OCU subsystem subscribing to video.  You
    // could just as easily make the SubsystemCommand part of a Vehicle, acting as
    // the A.I.
    component.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::OCU,
                                                             "SubsystemCommand");

    // Initialize component component with component given ID.
    if(component.Initialize(JAUS::Address(1000, 1, 1)) == false)
    {
        std::cout << "Failed to Initialize Component.\n";
        return 0;
    }

    component.AccessControlService()->SetAuthorityCode(255);


    //CxUtils::SleepMs(2000);
    while(1)
    {
        JAUS::Address baselineComponentID;
        
        //fetch joystick info

        //double GetAxisPercentage(const short int axis, const short int deadzone = 0) const;
        //bool IsButtonPressed(const unsigned int num = 0) const;
        //Look up these values
        mForwardSpeed=-mJoystick.GetAxisPercentage(1);
        mLateralSpeed=mJoystick.GetAxisPercentage(0);
        
        mYawCommand=mJoystick.GetAxisPercentage(2);
        mDesiredDepth=12*8*(100.0-(mJoystick.GetAxisPercentage(3)+100)/2.0)/100.0+30;
        /*if(mDesiredDepth<20)
        {
            mDesiredDepth=20;
        }*/
        //mDepthCommand=-mJoystick.GetAxisPercentage(3);
        
        mEnable=mJoystick.IsButtonPressed(7);
        mDisable=mJoystick.IsButtonPressed(11);
        //std::cout << mEnable << " " << mDisable << std::endl;
        int subsystemToControl=20;
        
        if(baselineComponentID.IsValid() == false)
        {
            //JAUS::Address::List idList = GetComponent()->DiscoveryService()->GetSubsystem(GetComponentID().mSubsystem)->GetComponentsWithService(JAUS::PrimitiveDriver::Name);
            JAUS::Address::List idList = component.DiscoveryService()->GetSubsystem(subsystemToControl)->GetComponentsWithService(JAUS::VelocityStateDriver::Name);
            if(idList.size() > 0)
            {
                baselineComponentID = idList.front();
            }
        }
        if(baselineComponentID.IsValid() == false)
        {
            CxUtils::SleepMs(100);
            std::cout << "Baseline Invalid\n";
            continue;
        }
        
        //Command what to do
        switch(mCurrState)
        {
            case DISABLED:
                std::cout << "Joystick: Disabled\n";
                //same, only because I don't have a direction
                /*gc.SetDesiredYaw(mCurrentYaw);
                gc.SetDesiredPitch(0);
                gc.SetDesiredRoll(0);
                gc.SetDesiredDepth(mCurrentDepth);
                gc.SetDesiredLateralVel(0.0);
                gc.SetDesiredAxialVel(0.0);*/
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
                mDesiredYaw=CxUtils::Orientation::AddToAngle(mDesiredYaw,mYawCommand/100.0*2.0,false);
                
                //mDesiredDepth=mCurrentDepth+mDepthCommand;
                
                /*if(fabs(mDepthCommand)>25.0)
                {
                    mDesiredDepth=mDesiredDepth+mDepthCommand*0.5/100.0;
                }*/
                 
                std::cout << "Desired: " << mDesiredYaw << " Command: " << mYawCommand << std::endl;
                std::cout << "Desired: " << mDesiredDepth << std::endl;
                
                /*gc.SetDesiredYaw(mDesiredYaw);
                gc.SetDesiredPitch(0);
                gc.SetDesiredRoll(0);
                gc.SetDesiredDepth(mDesiredDepth);
                gc.SetDesiredLateralVel(mLateralSpeed);
                gc.SetDesiredAxialVel(mForwardSpeed);*/
                break;
        }
        std::cout << "Authority Control: " << (int) component.AccessControlService()->GetAuthorityCode() << std::endl;
        
        
        JAUS::SetVelocityVector setVelocityVector;
        setVelocityVector.SetRoll(0);
        setVelocityVector.SetPitch(0);
        setVelocityVector.SetYaw(mDesiredYaw/180.0*M_PI);
        
        //Set the depth in meters
        setVelocityVector.SetPositionZ(mDesiredDepth/39.37);
        
        //Set the velocities
        setVelocityVector.SetVelocityX(mForwardSpeed);
        setVelocityVector.SetVelocityY(mLateralSpeed);
        setVelocityVector.SetSourceID(component.GetComponentID());
        setVelocityVector.SetDestinationID(baselineComponentID);
        
        
        command->SendCommand(&setVelocityVector, true);

        
        
        CxUtils::SleepMs(100);
    }

    // Shutdown the components.
    component.Shutdown();
    
    return 0;
}