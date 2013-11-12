#include "auvc/underwater/globalinfo.h"
#include <xmlconfig/xmlconfigurator.h>
#include <jaus/extras/mcu/reportmicrocontrollerstate.h>

using namespace Zebulon;
using namespace Underwater;
using namespace Utility;

const std::string GlobalInfo::ForwardCamera = "ForwardCamera";
const std::string GlobalInfo::DownwardCamera = "DownwardCamera";
const std::string GlobalInfo::Laser = "Laser";
const std::string GlobalInfo::DepthSensor = "DepthSensor";

const std::string GlobalInfo::StartButton = "StartButton";
const std::string GlobalInfo::Started = "Started";
const std::string GlobalInfo::StartYaw = "StartYaw";

const std::string GlobalInfo::ExitEarlyToFinalMission = "ExitEarlyToFinalMission";

const std::string GlobalInfo::PingerYaw = "PingerYaw";
const std::string GlobalInfo::PingerUpdateTime = "PingerUpdateTime";
const std::string GlobalInfo::PingerDistance = "PingerDistance";
const std::string GlobalInfo::PingerSelected = "PingerSelected";
const std::string GlobalInfo::PingerGain = "PingerGain";

const std::string GlobalInfo::EStop = "E-Stop";

GlobalInfo::GlobalInfo()
{
    //Filters the "High Speed" signal
    //mDepthFilter=new AverageFilter(50);
    //mDepthFilter=new KalmanLinearFilter(1,0.01,10.0);
    ////mDepthFilter=new LowPassFilter(.01,.17);
    //mDepthFilter=new FIRFilter(100,.01);
    
    //Filters the "Raw" sensor
    ////mRawDepthFilter=new MedianFilter(50);
    mDepthFlag=0;
}

GlobalInfo::~GlobalInfo()
{
    //delete mDepthFilter;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Load settings from XML file.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::LoadSettings(const std::string& xmlFilename)
{
    /*Configuration::XmlConfigurator xmlConfig;
    if(xmlConfig.Load(xmlFilename, "Underwater") == false)
    {
        std::cout << "Failed to load XML configuration file.\n";
        return false;
    }

    double value = 0;
    if(xmlConfig.GetVar("VehicleDimensions@width", value, true))
    {
        SetInfo(VehicleWidth, value);
    }
    if(xmlConfig.GetVar("VehicleDimensions@length", value, true))
    {
        SetInfo(VehicleLength, value);
    }*/

    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method verifies a minimum number of capabilities 
///          were initialized correctly.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::VerifyInitialization()
{
    // Verify that global info has everything.
    JAUS::Component* component = GetComponent();
    if(component)
    {
        if(component->GetService(JAUS::GlobalPoseSensor::Name) &&
           component->GetService(JAUS::Microcontroller::Name) &&
           component->GetService(JAUS::VideoSubscriber::Name) &&
           component->GetService(JAUS::RangeSubscriber::Name))
           {
               // Add check here to verify that we have GPS, Compass, Video
               // and any other required data from Baseline.
               bool result = true;
               result &= IsValueSet(AI::Names::YawDegrees);
               
               return result;
               
           }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief JAUS method to read JAUS messages received by the JAUS component.
///
///   \param[in] jausMessageCode Message ID code for JAUS message to create
///                               data structure for.
///
///   \return Pointer to message type requested.
///
////////////////////////////////////////////////////////////////////////////////////
JAUS::Message* GlobalInfo::CreateMessage(const JAUS::UShort jausMessageCode)
{
    // Don't need to add support for any messages at this time.
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief JAUS method to read JAUS messages received by the JAUS component.
///
///   \param[in] jausMessage JAUS message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::Receive(const JAUS::Message* jausMessage)
{
    // Call parent class first to be nice.
    JAUS::SubsystemCommand::Receive(jausMessage);
    
    
    // check receive
    //std::cout << "*********************************Received stuff" << std::endl;
    
    switch(jausMessage->GetMessageCode())
    {
    case JAUS::REPORT_GLOBAL_POSE:
        // Do stuff with data
        {
            const JAUS::ReportGlobalPose* report = dynamic_cast<const JAUS::ReportGlobalPose*>(jausMessage);
            this->ProcessGlobalPose(report);
            
            //SetInfo("Yaw", report->GetYaw());
            //SetInfo("Pitch", report->GetPitch());
            //SetInfo("Roll", report->GetRoll());
            
            SetInfo("DepthSensor", report->GetAltitude()/0.0254);
            
            
            
            //std::cout << "HELLOWWWWWWWWWW" << std::endl;
            //Hack to run depth sensor at higher rate
            //driven by the compass
            /*double depth;
            static int count = 0;
            static double depthOffset = 0;
            //wait till ready
            if(mDepthFlag== 1 && GetInfo("DepthSensorRaw",depth))
            {
                mDepthFilter->Add(depth);
                //wait till filter ready
                if(mDepthFilter->Output(depth))
                {
                    count++;
                    if(count > 100)
                    {
                        SetInfo("DepthSensor",depth - depthOffset);
                    
                    }
                    else
                    {
                        depthOffset = depth;
                        SetInfo("DepthOffset",depthOffset);
                    }
                }
            }*/
        }
        break;
    case JAUS::REPORT_VELOCITY_STATE:
        // Do stuff with data
        {
            const JAUS::ReportVelocityState* report = dynamic_cast<const JAUS::ReportVelocityState*>(jausMessage);
            this->ProcessVelocityState(report);
            
            if (report != NULL)
            {
                SetInfo("VelocityX", report->GetVelocityX());
                SetInfo("VelocityY", report->GetVelocityY());
                SetInfo("VelocityZ", report->GetVelocityZ());
                
                SetInfo("RollRate", report->GetRollRate());
                SetInfo("PitchRate", report->GetPitchRate());
                SetInfo("YawRate", report->GetYawRate());
            }
        }
        break;
    case JAUS::REPORT_MICROCONTROLLER_STATE:
        {
            const JAUS::ReportMicrocontrollerState* report = dynamic_cast<const JAUS::ReportMicrocontrollerState*>(jausMessage);   

            if(report)
            {
                //Get the states map from mcu
                const JAUS::Microcontroller::DigitalStates* ds = report->GetDigitalStates();
                //iterate through adding to globalinfo
                JAUS::Microcontroller::DigitalStates::const_iterator di;
                for(di = ds->begin(); di != ds->end();di++)
                {
                    // di->first = Name of Digital Pin
                    // di->second = Value (boolean)
                    //std::cout << di->first << " , " << di->second << std::endl;
                    SetInfo(di->first, di->second);
                }
                
                //Get the states map from mcu
                const JAUS::Microcontroller::AnalogStates* as = report->GetAnalogStates();
                //iterate through adding to globalinfo
                JAUS::Microcontroller::AnalogStates::const_iterator ai;
                for(ai = as->begin(); ai != as->end();ai++)
                {
                    // ai->first = Name of Analog Pin
                    // ai->second = Value (double)
                    
                    //Hack to Filter Depth Sensor a little better
                    //at a higher rate
                    /*if(ai->first=="DepthSensor")
                    {
                        //Filter raw with median
                        mRawDepthFilter->Add(ai->second);
                        double rawDepth;
                        if(mRawDepthFilter->Output(rawDepth))
                        {
                            SetInfo("DepthSensorRaw",rawDepth);
                            mDepthFlag=1;
                        }
                    }
                    else*/
                    {
                        SetInfo(ai->first,ai->second);
                    }
                }
            }
        }   
        break;
    /*case JAUS::REPORT_IMAGE:
    {
        // No work needed.
        int x;
        x = 3;
        break;
    }
        default:
            //  Is called when we recieve a message that we do not support.
            break;*/
    }
}

///////////////////////////////////////////////////////////////////////////////////
///
///   \brief  Method to create any required data subscriptions.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::CreateSubscriptions()
{
    bool result = true;
    //result &= CreateVideoSubscriptions(); - last year
    
    result &= CreateVideoSubscriptions(JAUS::Address());
    result &= CreateRangeSensorSubscriptions(JAUS::Address());
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief  Does any simple updates periodically.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::CheckServiceStatus(const unsigned int timeSinceLastUpdateMs)
{ 
    static JAUS::Time::Stamp lastCheckMs = 0;
    if(JAUS::Time::GetUtcTimeMs() - lastCheckMs > 500)
    {
        CreateSubscriptions();
        lastCheckMs = JAUS::Time::GetUtcTimeMs();
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief  Gets the orientation of the robot.
///
///   \param[in] degrees If true, gets values in degrees, otherwise
///                      radians.
///
///   \return Roll (x), Pitch (y), and Yaw (z) of robot.
///
////////////////////////////////////////////////////////////////////////////////////
Orientation GlobalInfo::GetOrientation(const bool degrees) const
{
    Orientation rpy;

    if(degrees)
    {
        GetInfo(AI::Names::RollDegrees, rpy.mX);
        GetInfo(AI::Names::PitchDegrees, rpy.mY);
        GetInfo(AI::Names::YawDegrees, rpy.mZ);
    }
    else
    {
        GetInfo(AI::Names::Roll, rpy.mX);
        GetInfo(AI::Names::Pitch, rpy.mY);
        GetInfo(AI::Names::Yaw, rpy.mZ);
    }

    return rpy;
}

/* End of File */
