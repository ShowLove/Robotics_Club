//////////////////////////////////////////////////////////////////////////////////////
///
/// \file dataquat.h
/// \brief This file contains the message CORAL_DATA_QUAT.
///
/// Author(s): Jonathan Mohlenhoff<br>
/// Created: 1/24/2009<br>
/// Copyright (c) 2009<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: jmohlenh@ist.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the Robotics Laboratory, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
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
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _ZEBULON_CORAL_AHRS_DATA_QUAT__H
#define _ZEBULON_CORAL_AHRS_DATA_QUAT__H

#include <cxutils/math/quaternion.h>
#include "coralahrs/message.h"

namespace Zebulon
{
    namespace Compass
    {
        namespace CoralAHRS
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class DataQuat
            ///   \brief This message is sent when the system output mode has been set to
            ///          CORAL_QUAT. It contains system time and quanternion orientation data.
            ///          These packets are sent at a constant rate equal to the standard
            ///          system output rate divided by the output rate divisor.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_CORALAHRS_DLL DataQuat : public Message
            {
            public:
                DataQuat();
                DataQuat(const DataQuat& message);
                ~DataQuat();
                virtual int WriteMessageBody(CxUtils::Packet& packet) const;
                virtual int ReadMessageBody(const CxUtils::Packet& packet);
                virtual Message* Clone() const { return (Message*)new DataQuat(*this); }
                virtual void Print() const;
                DataQuat& operator=(const DataQuat& message);
                
                unsigned short mSystemTimeMs;           ///<  System time from Coral AHRS internal clock (milliseconds).
                CxUtils::Quaternion mOrientation;       ///<  Quaternion values of w, x, y and z components
            };
        }
    }
}


#endif
/*  End of File */
