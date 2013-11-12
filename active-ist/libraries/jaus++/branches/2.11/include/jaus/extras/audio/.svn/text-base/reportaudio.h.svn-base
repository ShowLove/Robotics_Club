////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportaudio.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  Created: 5 July 2010
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
#ifndef __JAUS_EXTRAS_AUDIO_REPORT_AUDIO__H
#define __JAUS_EXTRAS_AUDIO_REPORT_AUDIO__H

#include "jaus/extras/extrascodes.h"
#include "jaus/core/message.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ReportAudio
    ///   \brief This message allows a component to publish audio data.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL ReportAudio : public Message
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \enum Format
        ///   \brief Audio data format.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        enum Format
        {
            Mono8 = 0,
            Mono16,
            Stereo8,
            Stereo16
        };
        ReportAudio(const Address& dest = Address(), const Address& src = Address());
        ReportAudio(const ReportAudio& message);
        ~ReportAudio();
        void SetDeviceID(const Byte id) { mDeviceID = id; }
        void SetFormat(const Format format) { mFormat = format; }
        void SetFrequency(const int freq) { mFrequency = freq; }
        void SetTimeStamp(const Time& tstamp) { mTimeStamp = tstamp; }
        void SetAudio(const Packet& buffer) { mAudio = buffer; }
        inline Byte GetDeviceID() const { return mDeviceID; }
        inline Format GetFormat() const { return mFormat; }
        inline int GetFrequency() const { return mFrequency; }
        inline Time GetTimeStamp() const { return mTimeStamp; }
        inline Packet* GetAudio() { return &mAudio; }
        inline const Packet* GetAudio() const { return &mAudio; }
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new ReportAudio(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Report Audio"; }
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const;
        ReportAudio& operator=(const ReportAudio& message);
    protected:
        Byte mDeviceID;   ///<  Audio ID (number).
        Format mFormat;   ///<  Audio format.
        int mFrequency;   ///<  Audio frequency.
        Packet mAudio;    ///<  Audio data.
        Time mTimeStamp;  ///<  Time stamp.
    };
}

#endif
/*  End of File */
