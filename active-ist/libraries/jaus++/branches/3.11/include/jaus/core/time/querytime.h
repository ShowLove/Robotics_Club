////////////////////////////////////////////////////////////////////////////////////
///
///  \file querytime.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 29 December
///  <br>Copyright (c) 2009
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
#ifndef __JAUS_CORE_TIME_QUERY_TIME__H
#define __JAUS_CORE_TIME_QUERY_TIME__H

#include "jaus/core/corecodes.h"
#include "jaus/core/message.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class QueryTime
    ///   \brief This method is used to query the system time of a component.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class QueryTime : public Message
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class PresenceVector
        ///   \brief This class contains bit masks for bitwise operations on the
        ///          presence vector for this message.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL PresenceVector : public JAUS::PresenceVector
        {
        public:
            const static Byte Time = 0x01; ///< Bitmask for Time field.
            const static Byte Date = 0x02; ///< Bitmask for Date field.
        };
        QueryTime(const Address& dest = Address(), 
                  const Address& src = Address()) : Message(QUERY_TIME, dest, src)
        {
            mPresenceVector = 0;
        }
        QueryTime(const QueryTime& message) : Message(QUERY_TIME)
        {
            *this = message;
        }
        ~QueryTime() 
        {
        }
        void SetPresenceVector(const Byte presenceVector) { mPresenceVector = presenceVector; }
        // Return true if a command message, otherwise false.
        virtual bool IsCommand() const { return false; }
        // Writes message payload data to the packet at the current write position.
        virtual int WriteMessageBody(Packet& packet) const
        {
            int expected = BYTE_SIZE;
            int written = 0;
            written += packet.Write(mPresenceVector);
            return expected == written ? expected : -1;
        }
        // Reads message payload data from the packets current read position.
        virtual int ReadMessageBody(const Packet& packet)
         {
            int expected = BYTE_SIZE;
            int read = 0;
            read += packet.Read(mPresenceVector);
            return expected == read ? expected : -1;
        }
        // Make a copy of the message and returns pointer to it.
        virtual Message* Clone() const { return new QueryTime(*this); }
        // Gets the presence vector data for the message.
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        // Get the size of the message presence vector. 0 value indicates no presence vector.
        virtual UInt GetPresenceVectorSize() const { return BYTE_SIZE; }
        // Get the mask associated with presence vector.  Indicates what bits are used.
        virtual UInt GetPresenceVectorMask() const { return 0x03; }
        // Gets the response type to the associated message, 0 if message is a response type.
        virtual UShort GetMessageCodeOfResponse() const { return REPORT_TIME; }
        // Gets the name of the message in human readable format (for logging, etc.)
        virtual std::string GetMessageName() const { return "Query Time"; }
        // Clears only message body information.
        virtual void ClearMessageBody() { mPresenceVector = 0;}
        // Return true if payload is greater than maxPaylodSize (payload == message data only).
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const { return false; }
        QueryTime& operator=(const QueryTime& message)
        {
            CopyHeaderData(&message);
            mPresenceVector = message.mPresenceVector;
            return *this;
        }
    protected:
        Byte mPresenceVector;   ///<  Presence vector.
    };
}

#endif
/*  End of File */
