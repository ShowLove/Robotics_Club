/////////////////////////////////////////////////////////////////////////////////////
///
/// \file names.h
/// \brief This file contains a list of constants and pre-defined names/labels
///        for common data types.
///
///  Author(s): Daniel Barber<br>
///  Created: May 9 2010<br>
///  Copyright (c) 2010<br>
///  Robotics Laboratory and Club<br>
///  University of Central Florida (UCF)<br>
///  Email: dbarber@ist.ucf.edu <br>
///  Web: http://robotics.ucf.edu <br>
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
#ifndef __ZEBULON_AI_NAMES__H
#define __ZEBULON_AI_NAMES__H

#include <string>
#include "statemachine/libstatemachinedll.h"

namespace Zebulon
{
    namespace AI
    {
        namespace Names
        {
            // Names for position related information.
            static const std::string Latitude       = "WGS Latitude";   // Latitude in degrees.
            static const std::string Longitude      = "WGS Longitude";  // Longitude in degrees.
            static const std::string Elevation      = "Elevation";      // Elevation in meters.
            static const std::string Northing       = "UTM Northing";   // UTM Northing (meters).
            static const std::string Easting        = "UTM Easting";    // UTM Easting (meters).
            static const std::string ZoneNumber     = "UTM Zone Number";// UTM Zone Number.
            static const std::string ZoneLetter     = "UTM Zone Letter";// UTM Zone Letter.
            // Names for orientation related information.
            static const std::string Roll           = "Orientation Roll";           // By default value is in radians.
            static const std::string Pitch          = "Orientation Pitch";          // By default value is in radians.
            static const std::string Yaw            = "Orientation Yaw";            // By default value is in radians.
            static const std::string RollDegrees    = "Orientation Roll Degrees";   // Roll information in degrees.
            static const std::string PitchDegrees   = "Orientation Pitch Degrees";  // Pitch information in degrees.
            static const std::string YawDegrees     = "Orientation Yaw Degrees";    // Yaw information in degrees.
            static const std::string PoseTimeSeconds    = "Pose Time Seconds";      // Update of pose data in ms.
            // Names for velocity related information.
            static const std::string VelocityX      = "Velocity X";                 // By default value is in meter/s.
            static const std::string VelocityY      = "Velocity Y";                 // By default value is in meter/s.
            static const std::string VelocityZ      = "Velocity Z";                 // By default value is in meter/s.
            static const std::string VelocityRMS    = "Velocity RMS";               // Velocity RMS.
            static const std::string RollRate       = "Roll Rate";                  // Roll rate in rad/s.
            static const std::string PitchRate      = "Pitch Rate";                 // Pitch rate in rad/s.
            static const std::string YawRate        = "Yaw Rate";                   // Yaw rate in rad/s.
            static const std::string RollRateDegrees       = "Roll Rate Degrees";   // Roll rate in rad/s.
            static const std::string PitchRateDegrees      = "Pitch Rate Degrees";  // Pitch rate in rad/s.
            static const std::string YawRateDegrees        = "Yaw Rate Degrees";    // Yaw rate in rad/s.
            static const std::string RateRMS               = "Rate RMS";            // Rate RMS.
            static const std::string VelocityTimeSeconds   = "Velocity Time Seconds";  // Update of velocity data in ms.

        }
    }
}

#endif
/*  End of File */
