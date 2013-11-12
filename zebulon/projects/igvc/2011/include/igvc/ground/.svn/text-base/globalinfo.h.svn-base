////////////////////////////////////////////////////////////////////////////////////
///
/// \file globalinfo.h
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
#ifndef __ZEBULON_GROUND_GLOBAL_INFO__H
#define __ZEBULON_GROUND_GLOBAL_INFO__H

#include <statemachine/globalinfo.h>
#include <cxutils/math/coordinates.h>

namespace Zebulon
{
    namespace IGVC
    {
        typedef CxUtils::Utm Utm;               ///<  Type definition for UTM data.
        typedef CxUtils::Wgs Wgs;               ///<  Type defintion for WGS data.
        typedef CxUtils::Point3D Orientation;   ///<  Robot orientation data.

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class GlobalInfo
        ///  \brief Interface class for collecting all information about the platform
        ///         that the Ground Vehicle AI will use for navigation and decision
        ///            making.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class GlobalInfo : public AI::GlobalInfo
        {
        public:
            static const std::string VehicleWidth;  ///<  Label name for vehicle width.
            static const std::string VehicleLength; ///<  Label name for vehicle length.
            static const std::string Camera; ///<  Label name for vehicle length.
            static const std::string Laser;  ///<  Label name for vehicle length.
            GlobalInfo();
            virtual ~GlobalInfo();
            // Optional method to load settings from an XML file.
            virtual bool LoadSettings(const std::string& xmlFilename="");
            // Method called to verify that a minimum number of capabilities have been
            // initialized.
            virtual bool VerifyInitialization();
            // This method must be overloaded to add support for new message types you are subscribing to!
            virtual JAUS::Message* CreateMessage(const JAUS::UShort jausMessageCode);
            // Processes messages created by the sychronize service.
            virtual void Receive(const JAUS::Message* jausMessage);
            // Creates a subscription to the video subscriber service.
            virtual bool CreateSubscriptions();
            // This method is called periodically, overload to perform periodic checks, events, etc.
            virtual void CheckServiceStatus(const unsigned int timeSinceLastUpdateMs);
            // Gets UTM position of robot.
            Utm GetPosition(const bool setElevationToZero = true) const;
            // Gets world position (lat/long) values.
            Wgs GetPositionWgs(const bool setElevationToZero = true) const;
            // Gets the orientation of robot.
            Orientation GetOrientation(const bool degrees = false) const;
            // Gets the velocity of the robot
            double GetVelocityX() const;
            // Gets the rotation rate of the robot
            double GetRotationRateZ(const bool degrees = false) const;
            // Gets the operator state of the robot
            bool IsAutonomous() const;
        };

    }
}

 #endif ///__ZEBULON_GROUND_GLOBAL_INFO__H
