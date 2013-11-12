////////////////////////////////////////////////////////////////////////////////////
///
/// \file globalinfo.cpp
/// \brief Class for getting all sensor data required for use
///        by GlobalCommand interface.
///
/// Author(s): Daniel Barber, David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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
#include "asvc/surface/globalinfo.h"
#include <jaus/extras/mcu/querymicrocontrollerstate.h>
#include <jaus/extras/mcu/reportmicrocontrollerstate.h>
#include <xmlconfig/xmlconfigurator.h>
#include <statemachine/utility/objectscan.h>

using namespace Zebulon;
using namespace ASVC;

const std::string GlobalInfo::VehicleWidth = "Vehicle Width";
const std::string GlobalInfo::VehicleLength = "Vehicle Length";
const std::string GlobalInfo::MainCamera = "Forward";
const std::string GlobalInfo::TiltCamera = "Tilt Camera";
const std::string GlobalInfo::Laser = "Laser";

const std::string GlobalInfo::StartPosition = "Start";
const std::string GlobalInfo::ChannelStartPosition = "Channel Start";
const std::string GlobalInfo::ChannelStartHeading = "Channel Start Heading";
const std::string GlobalInfo::ChannelEndPosition = "Channel End";
const std::string GlobalInfo::ChannelEndHeading = "Channel End Heading";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalInfo::GlobalInfo()
{
    mDebugLaserFlag = false;
    mLaserDisplay.Setup("LIDAR Test", 15, 640);
    mObjectDisplay.Setup("LIDAR Object Test", 15, 640);
    mObjectScan.Setup(0.25, 1, 10, .1);
    SetInfo(GlobalInfo::PinNames::StartButton, (bool)false);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalInfo::~GlobalInfo()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Load settings from XML file.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::LoadSettings(const std::string& xmlFilename)
{
    Configuration::XmlConfigurator xmlConfig;
    if(xmlConfig.Load(xmlFilename, "Surface") == false)
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
    }

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
           component->GetService(JAUS::VideoSubscriber::Name) &&
           component->GetService(JAUS::RangeSubscriber::Name) &&
           component->GetService(JAUS::Microcontroller::Name))
           {
               // Add check here to verify that we have GPS, Compass, Video
               // and any other required data from Baseline.
               bool result = true;
               result &= IsValueSet("Start Button");
               result &= IsValueSet(AI::Names::Easting);
               result &= IsValueSet(AI::Names::Northing);
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
    // essentially processe message fucntion from baseline
    JAUS::SubsystemCommand::Receive(jausMessage);
    switch(jausMessage->GetMessageCode())
    {
        case JAUS::REPORT_GLOBAL_POSE:
            {
                const JAUS::ReportGlobalPose* report = dynamic_cast<const JAUS::ReportGlobalPose*>(jausMessage);
                this->ProcessGlobalPose(report);
            }
            break;
        case JAUS::REPORT_VELOCITY_STATE:
            {
                const JAUS::ReportVelocityState* report = dynamic_cast<const JAUS::ReportVelocityState*>(jausMessage);
                this->ProcessVelocityState(report);
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
                        SetInfo(ai->first,ai->second);
                    }
                }
            }
            break;
        default:
            //Zebulon::AI::GlobalInfo::Receive(jausMessage);
            break;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief  Method to create any required data subscriptions.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::CreateSubscriptions()
{
    bool result = true;

    // Connects to ALL.
    result &= CreateVideoSubscriptions(JAUS::Address());
    // Connects to ALL.
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
        JAUS::QueryMicrocontrollerState query(this->GetComponentID(),
                                              ((JAUS::Microcontroller*)this->GetComponent()->GetService(JAUS::Microcontroller::Name))->GetSynchronizeID());
        this->Send(&query);
        lastCheckMs = JAUS::Time::GetUtcTimeMs();
    }
    if(mDebugLaserFlag)
    {
        CxUtils::Point3D::List scan;
        if(this->GetInfo(GlobalInfo::Laser, scan))
        {
            mLaserDisplay.Clear();
            mLaserDisplay.AddScanCartesian(scan);
            mLaserDisplay.Display();

            mObjectScan.CalculateCartesian(scan, true, 270, -135, 135);
            bool valid = false;
            scan = mObjectScan.GetObjects(valid);
            mObjectDisplay.Clear();
            mObjectDisplay.AddScanCartesian(scan, true);
            mObjectDisplay.Display();
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief  Gets the UTM position of the robot.
///
///   \param[in] setElevationToZero If true, elevation is set to zero, otherwise
///                                 the actual value is used.
///
///   \return UTM position in world.
///
////////////////////////////////////////////////////////////////////////////////////
Utm GlobalInfo::GetPosition(const bool setElevationToZero) const
{
    Utm utm;
    GetInfo(AI::Names::Northing, utm.mNorthing);
    GetInfo(AI::Names::Easting, utm.mEasting);
    if(setElevationToZero)
    {
        utm.mElevation = 0;
    }
    else
    {
        GetInfo(AI::Names::Elevation, utm.mElevation);
    }
    GetInfo(AI::Names::ZoneNumber, utm.mZoneNumber);
    GetInfo(AI::Names::ZoneLetter, utm.mZoneLetter);
    return utm;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief  Gets the WGS position of the robot.
///
///   \param[in] setElevationToZero If true, elevation is set to zero, otherwise
///                                 the actual value is used.
///
///   \return WGS position in world.
///
////////////////////////////////////////////////////////////////////////////////////
Wgs GlobalInfo::GetPositionWgs(const bool setElevationToZero) const
{
    Wgs wgs;
    GetInfo(AI::Names::Latitude, wgs.mLatitude);
    GetInfo(AI::Names::Longitude, wgs.mLongitude);
    if(setElevationToZero)
    {
        wgs.mElevation = 0;
    }
    else
    {
        GetInfo(AI::Names::Elevation, wgs.mElevation);
    }

    return wgs;
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


/** Gets velcoity in X axis. */
double GlobalInfo::GetVelocityX() const
{
    double vel;
    GetInfo(AI::Names::VelocityX, vel);
    return vel;
}

/** Gets velcoity in Y axis. */
double GlobalInfo::GetVelocityY() const
{
    double vel;
    GetInfo(AI::Names::VelocityY, vel);
    return vel;
}


/** Gets rotation in Z axis. */
double GlobalInfo::GetRotationRateZ(const bool degrees) const
{
    double rate;
    if (degrees)
    {
        GetInfo(AI::Names::YawRateDegrees, rate);
    }
    else
    {
        GetInfo(AI::Names::YawRate, rate);
    }
    return rate;
}

/** Gets cartographer pose data. */
Cartographer::Pose GlobalInfo::GetPose(const Cartographer::Map* map) const
{
    Cartographer::Pose pose;
    Utm currentPosition = this->GetPosition(true);
    Orientation currentOrientation = this->GetOrientation();
    if(map)
    {
        pose.mPosition = map->ConvertFromGlobalToMapCoordinates(currentPosition);
        pose.mRotation.mX = currentOrientation.mX;
        pose.mRotation.mY = currentOrientation.mY;
        pose.mRotation.mZ = currentOrientation.mZ;
        pose.mLinearVelocity.mX = this->GetVelocityX();
        pose.mLinearVelocity.mY = this->GetVelocityY();
        pose.mRotationalVelocity.mZ = GetRotationRateZ();
        pose.mTimeStamp.SetCurrentTime();
    }
    return pose;
}

/** Pushes current utm position onto trail of waypoints visited. */
void GlobalInfo::PushCurrentPositionToTrail()
{
    Utm currentPosition = this->GetPosition(true);
    mTrailMutex.Lock();
    this->mPositionTrail.push_back(currentPosition);
    mTrailMutex.Unlock();
}


/** Gets the poistion trail data pushed onto stack.  If reverse parameter
    is true, data is returned in reverse order. */
Utm::List GlobalInfo::GetPositionTrail(const bool reverse) const
{
    CxUtils::Mutex::ScopedLock lock(&mTrailMutex);
    if(reverse)
    {
        Utm::List reverseList;
        Utm::List::const_reverse_iterator r;
        for(r = mPositionTrail.rbegin();
            r != mPositionTrail.rend();
            r++)
        {
            reverseList.push_back(*r);
        }
        return reverseList;
    }
    return mPositionTrail;
}
/*  End of File */
