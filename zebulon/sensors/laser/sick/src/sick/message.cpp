/////////////////////////////////////////////////////////////////////////////
///
/// \file message.cpp
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
#include "sick/message.h"

using namespace Zebulon;
using namespace Laser;
using namespace SICK;
using namespace CxUtils;

///////////////////////////////////////////////////////////////////////////////
///
///  \def CRC16_GEN_POL
///  \brief Value use din CRC calculation
///
///////////////////////////////////////////////////////////////////////////////
#define CRC16_GEN_POL 0x8005

///////////////////////////////////////////////////////////////////////////////
///
///  \def MKSHORT
///  \brief Splits a word into two shorts.
///
///////////////////////////////////////////////////////////////////////////////
#define MKSHORT(a,b) ((unsigned short) (a) | ((unsigned short)(b) << 8))


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///////////////////////////////////////////////////////////////////////////////
Message::Message(const Message::Type type) : mAddress(0x00),
                                             mMessageType(type),
                                             mMessageTimeStampMs(0)
{
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
///////////////////////////////////////////////////////////////////////////////
Message::~Message()
{
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the destination address/ID of the sensor the message
///         is being sent to.  By default this value is 0.
///
///////////////////////////////////////////////////////////////////////////////
void Message::SetAddress(const unsigned char addr)
{
    mAddress = addr;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the time that the message was recevied/generated.
///
///  \param ms Time message was received in milliseconds.
///
///////////////////////////////////////////////////////////////////////////////
void Message::SetMessageTimeStamp(const unsigned long long int ms)
{
    mMessageTimeStampMs = (unsigned int)ms;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Checks the data in buffer, and if there is a properly
///         formatted and complete message starting at index
///         0 of buffer (value of 0x02), then the data is
///         removed and copied to the packet.
///
///  \param buffer Buffered data possibly containing a message.
///  \param len Length of the buffer in bytes (total length).
///  \param packet Message extracted from buffer.
///
///  \return 1 on success, 0 on failure.
///
///////////////////////////////////////////////////////////////////////////////
int Message::ExtractMessage(const unsigned char* buffer,
                            const unsigned int len,
                            CxUtils::Packet& packet)
{
    int result = 0;

    packet.Clear();

    // 7 is the minimum size of a SICK message
    // which includes start byte, address, length,
    // and CRC
    if(len >= 7 && buffer[0] == STX && buffer[1] == 0x80)
    {
        // Get the length data
        unsigned short dataSize = 0;
        dataSize = buffer[3];
        dataSize = (dataSize << 8) | buffer[2];
        
        if( (unsigned int)(dataSize + 6) <= len )
        {
            unsigned short crc1, crc2;
            crc1 = GenerateCRC(buffer, dataSize + 4);
            CxUtils::Packet::Read(buffer, dataSize + 6, dataSize + 4, crc2, CX_PACKET_LITTLE_ENDIAN);
            if( crc1 == crc2 )
            {
                packet.Write(buffer, dataSize + 6);
                result = 1;
            }
            else
            {
                //packet.Write(buffer, dataSize + 6);
                result = 0;
            }
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Calculates a CRC value from the beginning of the packet, and
///  writes the 16 bit value at the end.
///
///  \param packet Message data to write a CRC for.
///
///  \return 1 on success, 0 on failure.
///
///////////////////////////////////////////////////////////////////////////////
int Message::WriteCRC(CxUtils::Packet& packet)
{
    unsigned short crc = 0;

    crc = GenerateCRC(packet.Ptr(), packet.Length());
    if(packet.Write(crc))
    {
        return 1;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Retrieves the type of message
///
///  \param packet Message data to write a CRC for.
///
///  \return The command/response value of the message
///
///////////////////////////////////////////////////////////////////////////////
unsigned char Message::GetMessageTypeFromPacket(const CxUtils::Packet& msg) const
{
    if(msg.Length() >= 7 && msg.Ptr()[0] == STX)
    {
        return msg.Ptr()[4];
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the size of the data block in the message.
///
///  \param Message to the data block size for.
///
///  \return Size of the data block.
///
///////////////////////////////////////////////////////////////////////////////
unsigned short Message::GetDataSize(const CxUtils::Packet& msg) const
{
    if(msg.Length() > MIN_MESSAGE_SIZE && msg.Ptr()[msg.GetReadPos()] == STX)
    {
        unsigned short size = 0;
        if( msg.Read(size, msg.GetReadPos() + 2) )
        {
            return size;
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Erases the packet, and starts writes the header for a message
///         which includes STX byte, sensor ID, length, and message code
///         provided.
///
///  \param packet Packet to erase and start writing message to.
///  \param command The message command code.
///  \param length The length of the data block for the message.  This
///                value includes only the data and message command code, not
///                the start byte, sensor ID, length or CRC fields.
///
///  \return Number of bytes written, 0 on failure.
///
///////////////////////////////////////////////////////////////////////////////
unsigned int Message::WriteHeader(CxUtils::Packet& packet,
                                  unsigned char command,
                                  const unsigned short length) const
{
    packet.Clear();
    packet.SetByteOrder(CX_PACKET_LITTLE_ENDIAN);
    packet.WriteByte(STX);
    packet.WriteByte(mAddress);
    packet.Write(length);
    packet.Write(command);
    return packet.Length();
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates a CRC value.
///
///  \param data Data to create CRC from (first byte in packet).
///  \param length Number of bytes in data.
///
///  \return The CRC value.
///
///////////////////////////////////////////////////////////////////////////////
unsigned short Message::GenerateCRC(const unsigned char *data, unsigned int length)
{
    unsigned short uCrc16;
    unsigned char  abData[2];

    uCrc16 = 0;
    abData[0] = 0;
    while(length--)
    {
        abData[1] = abData[0];
        abData[0] = *data++;
        if(uCrc16 & 0x8000)
        {
            uCrc16 = (uCrc16 & 0x7FFF) << 1;
            uCrc16 ^= CRC16_GEN_POL;
        }
        else
        {
            uCrc16 <<= 1;
        }
        uCrc16 ^= MKSHORT(abData[0], abData[1]);
    }

    return (uCrc16);
}

/*  End of File */
