////////////////////////////////////////////////////////////////////////////////////
///
///  \file message.h
///  \brief This file contains the definition of the Message class which
///         all JAUS++ messages are based from.
///
///  All messages are written into Packet structures, however the Message
///  base class is used for creating data structures that store read
///  message data and contain methods for  reading/riting from/to
///  a Packet structure.
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
#ifndef __JAUS_CORE_MESSAGE_H
#define __JAUS_CORE_MESSAGE_H

#include "header.h"
#include "bitvector.h"
#include "transport/largedataset.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Message
    ///   \brief Main class for creating implementations of specific JAUS messages.
    ///   Each Message based class is capable of reading/writing a specific
    ///   message.
    ///
    ///   All message classes inherit from this interface.  It requies the child class
    ///   to implement methods for serializing/writing and de-serializing/reading
    ///   message data from a Packet structure.  It also has methods for
    ///   making message duplicates.  Each message can read/write a whole message or
    ///   just the message body.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Message
    {
    public:
        typedef Header::Priority Priority;
        typedef std::vector<Message*> List;
        typedef std::map<UShort, Message*> Map;
        // Sets message type, and allows for initialization of src/dest.
        Message(const UShort messageCode, const Address& dest = Address(), const Address& src = Address());
        // Destructor, does any cleanup.
        virtual ~Message();
        // Return true if a command message, otherwise false.
        virtual bool IsCommand() const = 0;
        // Writes message payload data to the packet at the current write position.
        virtual int WriteMessageBody(Packet& packet) const = 0;
        // Reads message payload data from the packets current read position.
        virtual int ReadMessageBody(const Packet& packet) = 0;
        // Make a copy of the message and returns pointer to it.
        virtual Message* Clone() const = 0;
        // Gets the presence vector for the message.
        virtual UInt GetPresenceVector() const = 0;
        // Get the size of the message presence vector. 0 value indicates no presence vector.
        virtual UInt GetPresenceVectorSize() const = 0;
        // Get the mask associated with presence vector.  Indicates what bits are used.
        virtual UInt GetPresenceVectorMask() const = 0;
        // Gets the response type to the associated message, 0 if message is a response type.
        virtual UShort GetMessageCodeOfResponse() const = 0;
        // Gets the name of the message in human readable format (for logging, etc.)
        virtual std::string GetMessageName() const = 0;        
        // Clears only message body information.
        virtual void ClearMessageBody() = 0;
        // Return true if payload is greater than maxPaylodSize (payload == message data only).
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const = 0;
        // Sets the message priority.
        int SetPriority(const Byte p = Header::Priority::Standard);
        // Set the source ID of the message.
        int SetSourceID(const Address &src);
        // Set the destination ID of the message.
        int SetDestinationID(const Address &dest);
        // Swap source and destination ID.
        int SwapSourceAndDestination();
        // Copies header data common to all Messages.
        void CopyHeaderData(const Message* msg);
        // Sets header values to defaults.
        void ClearMessageHeader();
        // Clears/Resets all message values to defaults.
        void ClearMessage();
        // Get the message type.
        inline UShort GetMessageCode() const { return mMessageCode; }
        // Get the priority of the message.
        inline Byte GetPriority() const { return mPriority; }
        // Get the source ID of the message.
        inline Address GetSourceID() const { return mSourceID; }
        // Get the destination ID of the message.
        inline Address GetDestinationID() const { return mDestinationID; }
        // Checks if a specific field is set in the presence vector.
        inline bool IsFieldPresent(const UInt presenceVector) const { return (GetPresenceVector() & presenceVector) > 0 ;}
        // Checks if multiple fields are present in the message using the presence vector.
        inline bool AreFieldsPresent(const UInt presenceVector) const { return (GetPresenceVector() & presenceVector) == presenceVector; }
        // Prints message data to console window.
        virtual void Print() const;
        // Prints only message body information.
        virtual void PrintMessageBody() const {}
        // Runs a test case for the message.
        virtual int RunTestCase() const { return FAILURE; }
        // Method to check if this message is a matching response to a query or command.
        virtual bool IsResponseToMessage(const Message* requestingMessage) const;
        // Writes/Serializes message to a Packet.
        virtual int Write(Packet& packet, 
                          Header& header,
                          const Packet* transportHeader = NULL,
                          const bool clearPacket = true,
                          const UShort startingSequenceNumber = 0,
                          const Byte broadcastFlag = 0) const;
        // Writes/Serializes messages to a multi-packet stream (only used if IsLargeDataSet() returns true).
        virtual int WriteLargeDataSet(Packet::List& stream, 
                                      Header::List& streamHeaders,
                                      const UShort maxPayloadSize = 1437, 
                                      const Packet* transportHeader = NULL,
                                      const UShort startingSequenceNumber = 0,
                                      const Byte broadcastFlag = 0) const;
        // Reads/De-serialize data from Packet, and overwrites internal members.
        virtual int Read(const Packet &packet, const Packet* transportHeader = NULL);
        // Reads messages from a multi-packet stream.  Only overload for optimization purposes.
        virtual int ReadLargeDataSet(const Packet::List& stream, 
                                     const Packet* transportHeader = NULL);
        // Reads messages from a multi-packet stream.  Only overload for optimization purposes.
        virtual int ReadLargeDataSet(const std::map<UShort, Packet>& stream, 
                                     const Packet* transportHeader = NULL);
    protected:
        Byte mPriority;                    ///<  Message priority.
        UShort mMessageCode;               ///<  Message payload type (message code).
        Address mSourceID;                 ///<  Source ID of the message.
        Address mDestinationID;            ///<  Destination ID of the message.
    private:
        Packet mStreamPayload;             ///<  Temp structure used for large data sets.
    };

} //  End of JAUS namespace

#endif
/* End of file */
