////////////////////////////////////////////////////////////////////////////////////
///
///  \file rangesensorconfig.h
///  \brief This file contains the implementation of a JAUS structure.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 8 March 2010
///  <br>Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
#ifndef __JAUS_EXTRAS_RANGE_SENSOR_CONFIG__H
#define __JAUS_EXTRAS_RANGE_SENSOR_CONFIG__H

#include "jaus/extras/jausextrasdll.h"
#include "jaus/core/types.h"
#include <vector>
#include <map>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class RangeSensorConfig
    ///   \brief Stores configuration data related to a range sensor.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL RangeSensorConfig
    {
    public:
        enum UnitType
        {
            MM = 0,
            CM
        };
        typedef std::vector<RangeSensorConfig> List;
        typedef std::map<Byte, RangeSensorConfig> Map;
        RangeSensorConfig();
        RangeSensorConfig(const RangeSensorConfig& config);
        ~RangeSensorConfig();
        RangeSensorConfig& operator=(const RangeSensorConfig& config);
        Byte mID;               ///<  Range sensor ID number [0-255].
        std::string mName;      ///<  Name of the sensor (default is "Range Sensor").
        double mMaxRange;       ///<  Maximum range of sensor in meters.
        double mMinRange;       ///<  Minimum range of sensor in meters.
        double mAngleIncrement; ///<  Angle between range scans in radians.
        double mScanAngle;      ///<  Scan angle in radians [0, 2PI].  A value of 10 degrees is from -5 to 5 degrees in front of sensor.            
        UnitType mUnitType;     ///<  Unit type (see UnitType enum).
    };
}


#endif
/* End of File */
