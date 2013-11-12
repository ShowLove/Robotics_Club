//////////////////////////////////////////////////////////////////////////////////////
///
/// \file reportmotorstatus.h
/// \brief This message contains the information about the current motor status.
///
/// Author(s): Cassondra Puklavage<br>
/// Created: 2/14/2009<br>
/// Copyright (c) 2009<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu <br>
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
#ifndef _ZEBULON_MCU_REPORTMOTORSTATUS__H
#define _ZEBULON_MCU_REPORTMOTORSTATUS__H

#include "mcu/message.h"
#include "mcu/messages/custom/customcodes.h"

namespace Zebulon
{
    namespace MCU
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class ReportMotorStatus
        ///   \brief Report message of a motor's status.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_MICROCONTROLLER_DLL ReportMotorStatus : public Message
        {
        public:
            ReportMotorStatus();
            virtual ~ReportMotorStatus();
            virtual int WriteMessageBody(CxUtils::Packet& packet) const;
            virtual int ReadMessageBody(const CxUtils::Packet& packet);
            virtual void Print() const;
            virtual Message* Clone() const;
            unsigned char mPort; ///< Port number [0x00, 0xFF]
            
            struct BitFault
            {
                 unsigned nOverTemp  : 1;
                 unsigned nStalled  : 1;
                 unsigned nHallError  : 1;
                 unsigned nGroundFault  : 1;
                 unsigned nWaterDetect  : 1;
            };
            
            union FaultType
            {
                unsigned char FByte;
                BitFault FBits;
            } mFault;
            
            unsigned char mCurrent;
            unsigned char mTempurature;
        }; 
    }
}
#endif //_ZEBULON_MCU_REPORTMOTORSTATUS__H
/*  End of File */
