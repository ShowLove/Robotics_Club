////////////////////////////////////////////////////////////////////////////////////
///
///  \file header.h
///  \brief This file contains a data structure for storing JAUS transport header
///  information.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 26 September 2009
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
#ifndef __JAUS_CORE_HEADER__H
#define __JAUS_CORE_HEADER__H

#include "jaus/core/address.h"
#include <string>
#include <vector>
#include <cxutils/packet.h>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Header
    ///   \brief JAUS Message Header Structure used by the Transport Services.
    ///
    ///   The JAUS Transport header is defined by the AS5669A Standard.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Header
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Priority
        ///   \brief Class containing constants for different types of priority values
        ///          in the JAUS transport header.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Priority
        {
        public:
            const static Byte Low             = 0;       ///<  Low priority message.
            const static Byte Standard        = 1;       ///<  Standard priority message.
            const static Byte High            = 2;       ///<  High priority message.
            const static Byte SafetyCritical  = 3;       ///<  High Safety Critical message.
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class DataControl
        ///   \brief Class containing constants for different types of data control 
        ///          values.
        ///
        ///   Data control values are used to identify that a message is stand alone
        ///   (single) or part of a multi-packet stream.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class DataControl
        {
        public:
            const static Byte Single      = 0;    ///<  Single-packet message stream (standard).
            const static Byte First       = 1;    ///<  First message in multi-packet stream.
            const static Byte Normal      = 2;    ///<  Normal message in multi-packet stream.
            const static Byte Last        = 3;    ///<  Last message in multi-packet stream.
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class MessageType
        ///   \brief Class containing constants for message type flags used in the
        ///          header.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class MessageType
        {
        public:
            const static Byte Normal       = 0;   ///<  Normal message (other values are reserved by standard).
        };  
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class AckNack
        ///   \brief Class containing constants for acknowledgement or negative
        ///          acknowledgment of message receipt.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class AckNack
        {
        public:
            const static Byte None         = 0;   ///<  No Acknowledge/Negative acknowledge.
            const static Byte Request      = 1;   ///<  Request acknowledge/negative acknowledge.
            const static Byte Nack         = 2;   ///<  Negtive acknowledge.
            const static Byte Ack          = 3;   ///<  Acknowledge.
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Broadcast
        ///   \brief Class containing constants for broadcast flags.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Broadcast
        {
        public:
            const static UShort None         = 0;   ///<  No broadcast.
            const static UShort Local        = 1;   ///<  Local broadcast.
            const static UShort Global       = 2;   ///<  Global broadcast.
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Compression
        ///   \brief Class containing constants for header compression flags.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Compression
        {
        public:
            const static Byte None          = 0;    ///<  No header compression.
            const static Byte Request       = 1;    ///<  Request header compression.
        };
        static const UShort MinSize    = 14;                       ///<  Minimum header size.
        static const UShort PayloadOffset = MinSize - USHORT_SIZE; ///<  Offset from start of general header to payload.
        static const UShort MaxPacketSize = JAUS_USHORT_MAX;       ///<  Maximum packet size including header.
        typedef std::vector<Header> List;                          ///<  Vector of Header data.
        Header();
        Header(const Header& header);
        ~Header();
        int Write(Packet& packet) const;
        int Read(const Packet& packet);
        bool IsValid(std::string* errorMessage) const;
        Header& operator=(const Header& header);
        Byte mMessageType;      ///<  Type of message being sent through transport layer (does not indicate payload type).
        Address mDestinationID; ///<  Destination ID/Address.
        Address mSourceID;      ///<  Source ID/Address.        
        UShort mSequenceNumber; ///<  Sequence number of packet (used for multi-packet streams).
        UShort mSize;           ///<  Size of the transport packet including general header in bytes.
        Byte mPriorityFlag;     ///<  Message priority flag.
        Byte mBroadcastFlag;    ///<  Broadcast flag.
        Byte mAckNackFlag;      ///<  ACK/NACK flag.
        Byte mControlFlag;      ///<  Data control flags (single or mulit-packet streams).
        Byte mCompressionFlag;  ///<  Header compression bits (header compression not supported by JAUS++).

    };
}

#endif

/*  End of File */
