/////////////////////////////////////////////////////////////////////////////
///
/// \file message.h
/// \brief Message is the interface class for creating SICK LIDAR
/// message structures.
///
/// Author(s): Jon Carbone<br>
/// Created: 2/16/2008<br>
/// Last Modified: 2/16/2008<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: joncarbone@gmail.com<br>
/// Web: http://robotics.ucf.edu <br>
///
/////////////////////////////////////////////////////////////////////////////
#ifndef SICK_MESSAGE_H
#define SICK_MESSAGE_H

#include <cxutils/packet.h>

#ifndef ZEB_SICK_DLL
#define ZEB_SICK_DLL
#endif

#include <list>

/////////////////////////////////////////////////////////////////////////////
///
///  \namespace Zebulon
///  \brief Zebulon is the main namespace used for all software developed
///         for the Robotics Laboratory at UCF.
///
/////////////////////////////////////////////////////////////////////////////
namespace Zebulon
{
    namespace Laser
    {
        /////////////////////////////////////////////////////////////////////////////
        ///
        ///  \namespace SICK
        ///  \brief Main namespace for SICK LIDAR software.
        ///
        /////////////////////////////////////////////////////////////////////////////
        namespace SICK
        {
            /////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class Message
            ///  \brief Message is the primary base class for deriving all SICK LIDAR
            ///         messages.
            ///
            /////////////////////////////////////////////////////////////////////////////
            class ZEB_SICK_DLL Message
            {
            public:
                typedef std::list<Message*> Queue;   ///<  STL queue of message pointers.
                /////////////////////////////////////////////////////////////////////////////
                ///
                /// \var Type
                /// \brief The Type enumeration lists the different kinds of messages
                ///        available for communication with the SICK LIDAR.
                ///
                /////////////////////////////////////////////////////////////////////////////
                enum Type
                {
                    StatusModeCommand        = 0x31,
                    ConfirmStatusMode        = 0x00,
                    OperatingModeCommand     = 0x20,
                    SwitchOperatingModeResponse  = 0xA0,
                    ContinuousOutput         = 0xB0,
                    SetPermanentBaud         = 0x66,
                    Configure                = 0x77,
                    SwitchScanning           = 0x3B,
                    ConfirmVariantSwitching  = 0xBB,
                    ConfirmationConfiguration = 0xF7
                };
                const static unsigned char STX = 0x02;          ///<  Signals start of a message.
                const static unsigned int MIN_MESSAGE_SIZE = 8; ///<  Minimum message size.
                Message(const Message::Type type);
                virtual ~Message();
                // Serializes the message data to a packet for sending.
                virtual unsigned int Write(CxUtils::Packet& packet) const = 0;
                // Reads serialized data and saves to internal data members.
                virtual unsigned int Read(const CxUtils::Packet& packet) = 0;
                // Test Case Method
                virtual int RunTestCase() const = 0;
                // If a message is in the buffer, it pulls it out.
                static int ExtractMessage(const unsigned char *buffer,
                                          const unsigned int len,
                                          CxUtils::Packet& packet);
                static int WriteCRC(CxUtils::Packet& packet);
                void SetAddress(const unsigned char addr);
                void SetMessageTimeStamp(const unsigned long long int ms);
                unsigned char GetAddress() const { return mAddress; }
                unsigned int GetMessageTimeStamp() const { return mMessageTimeStampMs; }
                // Gets the type of the message.
                Message::Type GetMessageType() const { return mMessageType; };
                // Gets a clone of the message.
                virtual Message* Clone() const = 0;
                void CopyMessageData(const Message* msg)
                {
                    if(msg)
                    {
                        mAddress = msg->mAddress;
                        mMessageTimeStampMs = msg->mMessageTimeStampMs;
                    }
                }
                virtual void Print() const {}
            protected:
                unsigned char GetMessageTypeFromPacket(const CxUtils::Packet& msg) const;
                unsigned short GetDataSize(const CxUtils::Packet& msg) const;
                unsigned int WriteHeader(CxUtils::Packet& packet,
                    unsigned char type,
                    unsigned short len) const;
                static unsigned short GenerateCRC(const unsigned char *data,
                    const unsigned int len);
                unsigned char mAddress;           ///<  Destination address of SICK LIDAR.
                const Message::Type mMessageType; ///<  Message type.
                unsigned int mMessageTimeStampMs; ///<  Time of message receipt in milliseconds.
            };
        }
    }
}

#endif
/*  End of File */
