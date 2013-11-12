////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportcameracount.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  Created: 26 January 2010
///  Copyright (c) 2010
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
#ifndef __JAUS_EXTRAS_VIDEO_REPORT_CAMERA_COUNT__H
#define __JAUS_EXTRAS_VIDEO_REPORT_CAMERA_COUNT__H

#include "jaus/extras/extrascodes.h"
#include "jaus/core/message.h"
#include <cxutils/images/image.h>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ReportCameraCount
    ///   \brief This message allows a component to report how many cameras
    ///          a Visual Sensor service supports.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL ReportCameraCount : public Message
    {
    public:
        ReportCameraCount(const Address& dest = Address(), const Address& src = Address()) : Message(REPORT_CAMERA_COUNT, dest, src)
        {
        }
        ReportCameraCount(const ReportCameraCount& message) : Message(REPORT_CAMERA_COUNT)
        {
            *this = message;
        }
        ~ReportCameraCount() {}  
        std::vector<Byte>* GetCameraList() { return &mList; }
        const std::vector<Byte>* GetCameraList() const { return &mList; }
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const 
        { 
            int total = 0;
            int expected = (int)(BYTE_SIZE + BYTE_SIZE*mList.size());
            total += packet.WriteByte((Byte)mList.size());
            for(unsigned int i = 0; i < (unsigned int)mList.size(); i++)
            {
                total += packet.WriteByte(mList[i]);
            }
            return total == expected ? total : -1;
        }
        virtual int ReadMessageBody(const Packet& packet)
        { 
            int total = 0;
            int expected = BYTE_SIZE;
            Byte count = 0, temp = 0;
            total += packet.Read(count);
            expected += count*BYTE_SIZE;
            for(Byte i = 0; i < count; i++)
            {
                total += packet.Read(temp);
                mList.push_back(temp);
            }
            return total == expected ? total : -1;
        }
        virtual Message* Clone() const { return new ReportCameraCount(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Report Camera Count"; }
        virtual void ClearMessageBody() { mList.clear(); }
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const { return false; }
        ReportCameraCount& operator=(const ReportCameraCount& message)
        {
            CopyHeaderData(&message);
            mList = message.mList;
            return *this;
        }
    protected:
        std::vector<Byte> mList;    ///< List of camera IDs.
    };
}

#endif
/*  End of File */
