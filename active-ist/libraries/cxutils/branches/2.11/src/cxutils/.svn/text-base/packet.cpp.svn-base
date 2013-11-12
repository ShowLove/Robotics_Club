////////////////////////////////////////////////////////////////////////////////////
///
///  \file packet.cpp
///  \brief Contains software for automatic reading and writing of data to
///  a byte array (serialization de-serialization of data).
///
///  The Packet structure is extremely useful for creating/reading messages
///  that need to be sent/received from a serial port, network connection, etc.
///  It automatically resizes the memory as needed, and can automatically handle
///  byte order conversions.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 24 April 2007
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
#include "cxutils/packet.h"
#include <stdlib.h>
#include <string>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "cxutils/fileio.h"

using namespace CxUtils;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///////////////////////////////////////////////////////////////////////////////////
Packet::Wrapper::Wrapper() : mPacket(0)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
///   \param[in] wrapper The wrapper to duplicate.
///
///////////////////////////////////////////////////////////////////////////////////
Packet::Wrapper::Wrapper(const Packet::Wrapper& wrapper) : mPacket(0)
{
    *this = wrapper;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor for Wrapper packet.  A Wrapper packet wraps an existing
///          buffer with a Packet so users can easily Read/Write to/from the
///          buffer.  On destruction, the buffer is <b>not</b> deleted.
///
///   \param[in] buffer Pointer to buffer to wrap.
///   \param[in] len Length of the buffer in bytes.
///
///////////////////////////////////////////////////////////////////////////////////
Packet::Wrapper::Wrapper(unsigned char* buffer, const unsigned int len) : mPacket(0)
{
    if(buffer && len > 0)
    {
        mPacket = new Packet();
        mPacket->mpPacket = buffer;
        mPacket->mReserved = mPacket->mLength = len;
        mPacket->mWrappedPacketFlag = true;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
///////////////////////////////////////////////////////////////////////////////////
Packet::Wrapper::~Wrapper()
{
    Clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Create a Wrapper packet.  A Wrapper packet wraps an existing
///          buffer with a Packet so users can easily Read/Write to/from the
///          buffer.  On destruction, the buffer is <b>not</b> deleted.
///
///   \param[in] buffer Pointer to buffer to wrap.
///   \param[in] len Length of the buffer in bytes.
///
///   \return True on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////////
bool Packet::Wrapper::Create(unsigned char* buffer, const unsigned int len)
{
    if(mPacket)
    {
        mPacket->mpPacket = 0;
        mPacket->mReserved = mPacket->mLength = 0;
        mPacket->mWrappedPacketFlag = true;
        delete mPacket;
    }
    if(buffer && len > 0)
    {
        mPacket = new Packet();
        mPacket->mpPacket = buffer;
        mPacket->mReserved = mPacket->mLength = len;
        mPacket->mWrappedPacketFlag = true;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears wrapped data.
///
///////////////////////////////////////////////////////////////////////////////////
void Packet::Wrapper::Clear()
{
    if(mPacket)
    {
        mPacket->mpPacket = 0;
        mPacket->mReserved = mPacket->mLength = 0;
        mPacket->mWrappedPacketFlag = true;
        delete mPacket;
    }
    mPacket = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to get a pointer to the wrapper packet for read/write
///          operations.
///
///////////////////////////////////////////////////////////////////////////////////
const Packet* Packet::Wrapper::operator ->() const
{
    return ((const Packet*)(mPacket));
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to get a pointer to the wrapper packet for read/write
///          operations.
///
///////////////////////////////////////////////////////////////////////////////////
Packet* Packet::Wrapper::operator ->()
{
    return ((Packet*)(mPacket));
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to get a pointer to the wrapper packet for read/write
///          operations.
///
///////////////////////////////////////////////////////////////////////////////////
const Packet* Packet::Wrapper::GetData() const
{
    return ((const Packet*)(mPacket));
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to get a pointer to the wrapper packet for read/write
///          operations.
///
///////////////////////////////////////////////////////////////////////////////////
Packet* Packet::Wrapper::GetData()
{
    return ((Packet*)(mPacket));
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Makes a wrapper packet copy.
///
///////////////////////////////////////////////////////////////////////////////////
Packet::Wrapper& Packet::Wrapper::operator=(const Packet::Wrapper& wrapper)
{
    if(this != &wrapper)
    {
        if(mPacket)
        {
            mPacket->mpPacket = 0;
            mPacket->mReserved = mPacket->mLength = 0;
            mPacket->mWrappedPacketFlag = true;
            delete mPacket;
        }
        if(wrapper.mPacket)
        {
            mPacket = new Packet();
            mPacket->mpPacket = wrapper.mPacket->mpPacket;
            mPacket->mReserved = mPacket->mLength = wrapper.mPacket->mLength;
            mPacket->mWrappedPacketFlag = true;
        }
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param border Byte order to store data as internally.
///
////////////////////////////////////////////////////////////////////////////////////
Packet::Packet(const unsigned int border)
{
    mpPacket = NULL;
    mByteOrder = CX_PACKET_LITTLE_ENDIAN;
    if(border == CX_PACKET_BIG_ENDIAN)
        mByteOrder = border;

    mLength = mReserved = mWritePos = mReadPos = 0;
    mWrappedPacketFlag = false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Packet::Packet(const Packet& another)
{
    mpPacket = NULL;
    mLength = mReserved = mWritePos = mReadPos = 0;
    mWrappedPacketFlag = false;
    *this = another;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Packet::~Packet() 
{
    Destroy();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets what byte order the data should be stored as in the buffer.
///
///   If the byte order is set to little endian (CX_PACKET_LITTLE_ENDIAN), and the
///   system is big endian, the data will be converted when writing to and
///   reading from the packet.  The same is true if the byte order is set
///   to big endian (CX_PACKET_BIG_ENDIAN) and the system is little endian.  If the
///   system byte order matches the byte order value set, then not byte
///   order conversion occurs.  
///
///   The default value for storage of data is CX_PACKET_LITTLE_ENDIAN.
///
///   \param order Byte order type to store data in packet as.
///
///   \return CX_PACKET_FAILURE on error, CX_PACKET_OK if set properly.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::SetByteOrder(const unsigned int order)
{
    if(order == CX_PACKET_LITTLE_ENDIAN || order == CX_PACKET_BIG_ENDIAN) 
    {
        mByteOrder = order;
        return CX_PACKET_OK;
    }
    return CX_PACKET_FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the position for writing data in the packet.
///
///   \param pos Byte number to start writing to.
///
///   \return CX_PACKET_FAILURE on error, CX_PACKET_OK if set properly.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::SetWritePos(const unsigned int pos)
{
    unsigned int *p = (unsigned int *)(&mWritePos);
    if(pos <= mLength) 
    {
        *p = pos;
        return CX_PACKET_OK;
    }
    return CX_PACKET_FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the position for reading data from the packet.  This is the
///   byte position.
///
///   \param pos Byte number to start reading from.
///
///   \return CX_PACKET_FAILURE on error, CX_PACKET_OK if set properly.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::SetReadPos(const unsigned int pos) const
{
    unsigned int *p = (unsigned int *)(&mReadPos);
    if(pos <= mLength) 
    {
        *p = pos;
        return CX_PACKET_OK;
    }
    return CX_PACKET_FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the length of the data written in the packet to a new value.
///
///   The length set must be less than the amount of memory reserved.
///   This is only an extra function for some special use cases, and should
///   typically not be used.
///
///   The advantage to having this method is if you know you need to write
///   a header at the beginning of the packet, but it requires values
///   that can't be known until after the message body has been written.
///   Using this method, you can set the length to after the header, 
///   write the data, then go back and write the header at the beginning.
///
///   The write and read position values within the packet will be reset
///   to zero on this function call.
///
///   \param len New length of written data in packet.
///
///   \return CX_PACKET_FAILURE on error, CX_PACKET_OK if set properly.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::SetLength(const unsigned int len)
{
    if(mpPacket && len <= mReserved) 
    {
        mLength = len;
        mWritePos = mReadPos = 0;
        return CX_PACKET_OK;
    }
    return CX_PACKET_FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Inserts buffered data into the packet at a position.
///
///   This function can be used to pre-pend data or insert data into the middle
///   of a packet.  This also resets the read position back to the beginning
///   and moves the write pos to the end of the packet.
///
///   \param len The length of the data to insert.
///   \param buff The buffered data to insert.
///   \param pos The starting position in the packet [0,Length()).
///
///   \return CX_PACKET_FAILURE on error, CX_PACKET_OK if inserted properly.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Insert(const unsigned char *buff, const unsigned int len, const unsigned int pos)
{
    if(mLength == 0)
        return Write(buff, len, pos);

    if(!buff || pos > mLength)
        return CX_PACKET_FAILURE;

    //  Make room to fit data.
    if(mLength + len >= mReserved) 
        GrowPacket(mLength + len);

    //  Now move/split data inside packet
    memmove(&mpPacket[pos + len], &mpPacket[pos], sizeof(unsigned char)*(mLength - pos));
    //  Now copy new data
    memcpy(&mpPacket[pos], buff, sizeof(unsigned char)*len);
    mLength += len;
    mpPacket[mLength] = 0;
    mReadPos = 0;
    mWritePos = mLength;
    return CX_PACKET_OK;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Inserts a value into the packet and repeats it the
///   number of times specified.  This can be useful when you are trying to
///   reserve a block of memory within a packet for future data.
///
///   This function can be used to pre-pend data or insert data into the middle
///   of a packet.  This function will reset the read pos back to 0 (the
///   beginning).  The write pos will be reset to the end of the packet.
///
///   \param len The length of the data to insert. ( number of times to repeat
///                the value ).
///   \param value The byte value to insert, typically 0.
///   \param pos The starting position in the packet [0,Length()).
///
///   \return CX_PACKET_FAILURE on error, CX_PACKET_OK if inserted properly.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::InsertCharacter(const unsigned char value, 
                            const unsigned int len, 
                            const unsigned int pos)
{
    if(mLength == 0) 
    {
        if( mReserved < len ) 
        {
            GrowPacket( len + 1 );
        }
        memset( mpPacket, value, len );
        mLength = len;
        mWritePos = mLength;
        mReadPos = 0;
        return CX_PACKET_OK;
    }

    if(pos > mLength)
        return CX_PACKET_FAILURE;

    //  Make room to fit data.
    if(mLength + len >= mReserved) 
        GrowPacket(mLength + len);

    //  Now move/split data inside packet
    memmove(&mpPacket[pos + len], &mpPacket[pos], sizeof(unsigned char)*(mLength - pos));
    //  Set the fill value
    memset( &mpPacket[pos], value, sizeof(unsigned char)*len );
    mLength += len;
    mpPacket[mLength] = 0;
    mReadPos = 0;
    mWritePos = mLength;

    return CX_PACKET_OK;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes data in the packet.
///
///   Deletes the range of data from the packet from a starting point to a
///   length in bytes.  Encoding and Decoding positions are reset also.
///
///   \param len The number of bytes to delete.
///   \param start The starting position in the packet [0,Length()).
///
///   \return CX_PACKET_FAILURE on error, CX_PACKET_OK if cleared properly.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Delete(const unsigned int len, unsigned int start)
{
    int result = CX_PACKET_FAILURE;
    if(start < mLength && start + len <= mLength) 
    {
        if(mpPacket) 
        {
            if(len == mLength && start == 0)
            {
                mLength = 0;
            }
            else
            {
                memmove(&mpPacket[start], &mpPacket[start + len], sizeof(unsigned char)*(mLength - (start + len)));
                mLength -= len;
            }
            mpPacket[mLength] = 0;
            mWritePos = mReadPos = 0;
            result = CX_PACKET_OK;
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The position being used for writing data in the packet.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Packet::GetWritePos() const
{
    return mWritePos;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \return The position being used for reading data.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Packet::GetReadPos() const
{
    return mReadPos;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets what byte order the data should be stored as in the buffer.
///
///   If the byte order is set to little endian (CX_PACKET_LITTLE_ENDIAN), and the
///   system is big endian, the data will be converted when writing to and
///   reading from the packet.  The same is true if the byte order is set
///   to big endian (CX_PACKET_BIG_ENDIAN) and the system is little endian.  If the
///   system byte order matches the byte order value set, then not byte
///   order conversion occurs.  
///
///   \return Gets the byte order that the data is being stored as.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Packet::GetByteOrder() const
{
    return mByteOrder;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \return Length of the entire packet in bytes (includes header and data).
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Packet::Size() const { return mLength; }

////////////////////////////////////////////////////////////////////////////////////
///
///   \return Length of the entire packet in bytes (includes header and data).
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Packet::Length() const { return mLength; }

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reserves memory for storing data or receiving data over
///   a connection.
///
///   \param size Number of bytes to reserve (no matter what will be 
///   equal to CX_HEADER_SIZE to hold minimum packet size).
///
////////////////////////////////////////////////////////////////////////////////////
void Packet::Reserve(const unsigned int size)
{
    if(mWrappedPacketFlag)
    {
        return;
    }
    assert(size > 0);
    if(mReserved >= size)
        return;

    unsigned char *newPtr = new unsigned char[size];
    assert(newPtr);
    //  Clear the memory.
    memset(newPtr, 0, sizeof(unsigned char)*size);
    if(mLength > 0)
    {
        //  Try save data if possible.
        if(mReserved < size && mpPacket)
            memcpy(newPtr, mpPacket, sizeof(unsigned char)*mReserved);
        else {
            mLength = mReadPos = mWritePos = 0;
        }
    }

    mReserved = size;
    if(mpPacket) 
    {
        delete[] mpPacket;
        mpPacket = NULL;
    }
    mpPacket = newPtr;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Write the data to the end of the packet.  No byte order conversion
///   is perfomed using this function.
///
///   \param buff The data to add to end of packet.
///   \param len The size of the data in bytes.
///   \param pos The byte position within the packet to write at.  If pos is
///              equal to UINT_MAX, then the internal write position is used, 
///              which is the default behavior.
///
///   \return Number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Write(const unsigned char *buff, const unsigned int len, const unsigned int pos)
{
    assert(len > 0);
    bool overwrite = false;
    unsigned int epos = 0;
    /*  If the write position passed is equal to
        UINT_MAX, then use the internal write
        position parameter of the packet. */
    if(pos == UINT_MAX) 
    {
        epos = mWritePos;
    }
    else 
    {
        epos = pos;
    }

    // Don't write if passed the length of the packet.
    if( epos > mLength || epos == UINT_MAX) 
    {
        return 0;
    }
    if(epos + len >= mReserved && mWrappedPacketFlag)
    {
        return 0;
    }
    if(epos + len >= mReserved)
        GrowPacket(epos + len + 1);
    memcpy(&mpPacket[epos], buff, sizeof(unsigned char)*len);
    /*  If we use the position value passed
        then we are writing over existing data, and
        not adjusting the current write position. */
    if(!overwrite)
        mWritePos += len;
    //  Adjust the length of the packet if needed.
    if (epos + len > mLength) 
    {
        mLength = epos + len;
        // Add a null value at the end if space is available.
        if(mLength < mReserved)
        {
            mpPacket[mLength] = '\0';
        }
    }
    return len;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the data into the packet at the byte position specified
///          or using the internal write byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the write position the number of
///   bytes written if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data to write.
///   \param pos The byte position within the packet to write at.  If pos is
///              equal to UINT_MAX, then the internal write position is used, 
///              which is the default behavior.
///
///   \return The number of bytes written, 0 if nothing written.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Write(const unsigned char val, const unsigned int pos)
{
    return WritePacket(this, (unsigned char)(val), pos);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the data into the packet at the byte position specified
///          or using the internal write byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the write position the number of
///   bytes written if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data to write.
///   \param pos The byte position within the packet to write at.  If pos is
///              equal to UINT_MAX, then the internal write position is used, 
///              which is the default behavior.
///
///   \return The number of bytes written, 0 if nothing written.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Write(const char val, const unsigned int pos)
{
    return WritePacket(this, (char)(val), pos);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the data into the packet at the byte position specified
///          or using the internal write byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the write position the number of
///   bytes written if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data to write.
///   \param pos The byte position within the packet to write at.  If pos is
///              equal to UINT_MAX, then the internal write position is used, 
///              which is the default behavior.
///
///   \return The number of bytes written, 0 if nothing written.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Write(const unsigned int val, const unsigned int pos)
{
    return WritePacket(this, val, pos);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the data into the packet at the byte position specified
///          or using the internal write byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the write position the number of
///   bytes written if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data to write.
///   \param pos The byte position within the packet to write at.  If pos is
///              equal to UINT_MAX, then the internal write position is used, 
///              which is the default behavior.
///
///   \return The number of bytes written, 0 if nothing written.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Write(const unsigned short val, const unsigned int pos)
{
    return WritePacket(this, val, pos);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the data into the packet at the byte position specified
///          or using the internal write byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the write position the number of
///   bytes written if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data to write.
///   \param pos The byte position within the packet to write at.  If pos is
///              equal to UINT_MAX, then the internal write position is used, 
///              which is the default behavior.
///
///   \return The number of bytes written, 0 if nothing written.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Write(const int val, const unsigned int pos)
{
    return WritePacket(this, val, pos);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the data into the packet at the byte position specified
///          or using the internal write byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the write position the number of
///   bytes written if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data to write.
///   \param pos The byte position within the packet to write at.  If pos is
///              equal to UINT_MAX, then the internal write position is used, 
///              which is the default behavior.
///
///   \return The number of bytes written, 0 if nothing written.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Write(const short val, const unsigned int pos)
{
    return WritePacket(this, val, pos);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the data into the packet at the byte position specified
///          or using the internal write byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the write position the number of
///   bytes written if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data to write.
///   \param pos The byte position within the packet to write at.  If pos is
///              equal to UINT_MAX, then the internal write position is used, 
///              which is the default behavior.
///
///   \return The number of bytes written, 0 if nothing written.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Write(const long long int val, const unsigned int pos)
{
    return WritePacket(this, val, pos);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the data into the packet at the byte position specified
///          or using the internal write byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the write position the number of
///   bytes written if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data to write.
///   \param pos The byte position within the packet to write at.  If pos is
///              equal to UINT_MAX, then the internal write position is used, 
///              which is the default behavior.
///
///   \return The number of bytes written, 0 if nothing written.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Write(const unsigned long long int val, const unsigned int pos)
{
    return WritePacket(this, val, pos);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the data into the packet at the byte position specified
///          or using the internal write byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the write position the number of
///   bytes written if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data to write.
///   \param pos The byte position within the packet to write at.  If pos is
///              equal to UINT_MAX, then the internal write position is used, 
///              which is the default behavior.
///
///   \return The number of bytes written, 0 if nothing written.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Write(const float val, const unsigned int pos)
{
    int result;
    int temp;
    memcpy(&temp, &val, sizeof(val));
    result = WritePacket(this, temp, pos);
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the data into the packet at the byte position specified
///          or using the internal write byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the write position the number of
///   bytes written if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data to write.
///   \param pos The byte position within the packet to write at.  If pos is
///              equal to UINT_MAX, then the internal write position is used, 
///              which is the default behavior.
///
///   \return The number of bytes written, 0 if nothing written.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Write(const double val, const unsigned int pos)
{
    int result;
    long long int temp;
    memcpy(&temp, &val, sizeof(val));
    result = WritePacket(this, temp, pos);
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the data into the packet at the byte position specified
///          or using the internal write byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the write position the number of
///   bytes written if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param packet Packet data to write.
///   \param pos The byte position within the packet to write at.  If pos is
///              equal to UINT_MAX, then the internal write position is used, 
///              which is the default behavior.
///
///   \return The number of bytes written, 0 if nothing written.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Write(const Packet& packet, const unsigned int pos)
{
    return Write(packet.mpPacket, packet.mLength, pos);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the string data to the packet.
///
///   \param str String data to write.
///   \param pos The byte position within the packet to write at.  If pos is
///              equal to UINT_MAX, then the internal write position is used, 
///              which is the default behavior.
///
///   \return The number of bytes written, 0 if nothing written.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Write(const std::string& str, const unsigned int pos)
{
    return Write((unsigned char *)(str.c_str()), (unsigned int)(str.size()), pos);
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Resets length, write, and read positions and sets all
///   memory to values of zero.  This function does not delete any memory.  To
///   delete memory use Destroy.
///
///   \param[in] setZeroFlag If true, all values in the byte array are set to 0.
///                          If false, only the first byte value is set to 0.
///
////////////////////////////////////////////////////////////////////////////////////
void Packet::Clear(const bool setZeroFlag)
{
    if(mpPacket && mReserved)
    {
        *mpPacket = 0;
        if(setZeroFlag)
        {
            memset(mpPacket, 0, sizeof(unsigned char)*mReserved);
        }
    }
    mWritePos = mReadPos = 0;
    mLength = 0;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes all data in the packet passed the header.  The header data
///   size values are also set to zero to match the cleared state.
///
////////////////////////////////////////////////////////////////////////////////////
void Packet::Destroy()
{
    if(mWrappedPacketFlag == false)
    {
        if(mpPacket)
            delete[] mpPacket;
    }
    mpPacket = 0;
    mWritePos = mReadPos = 0;
    mLength = mReserved = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints byte data to the console.
///
///   \param bytesPerLine Number of bytes to print per line.
///
////////////////////////////////////////////////////////////////////////////////////
void Packet::Print(const unsigned int bytesPerLine) const
{
    unsigned int b = 0;
    for(unsigned int i = 0; i < mLength; i++)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)mpPacket[i] << " "; 
        if(b == bytesPerLine)
        {
            std::cout << std::endl;
            b = 0;
        }
        else
        {
            b++;
        }
    }
    std::cout << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from the packet at the byte position specified
///          or using the internal read byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the read position the number of
///   bytes read if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data read.
///   \param pos The byte position within the packet to read at.  If pos is
///              equal to UINT_MAX, then the internal read position is used, 
///              which is the default behavior.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(unsigned char& val, const unsigned int pos) const
{
    return ReadPacket(this, val, pos); 
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from the packet at the byte position specified
///          or using the internal read byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the read position the number of
///   bytes read if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data read.
///   \param pos The byte position within the packet to read at.  If pos is
///              equal to UINT_MAX, then the internal read position is used, 
///              which is the default behavior.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(char& val, const unsigned int pos) const
{
    return ReadPacket(this, val, pos); 
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from the packet at the byte position specified
///          or using the internal read byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the read position the number of
///   bytes read if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data read.
///   \param pos The byte position within the packet to read at.  If pos is
///              equal to UINT_MAX, then the internal read position is used, 
///              which is the default behavior.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(int& val, const unsigned int pos)  const
{
    return ReadPacket(this, val, pos); 
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from the packet at the byte position specified
///          or using the internal read byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the read position the number of
///   bytes read if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data read.
///   \param pos The byte position within the packet to read at.  If pos is
///              equal to UINT_MAX, then the internal read position is used, 
///              which is the default behavior.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(short& val, const unsigned int pos) const 
{
    return ReadPacket(this, val, pos); 
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from the packet at the byte position specified
///          or using the internal read byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the read position the number of
///   bytes read if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data read.
///   \param pos The byte position within the packet to read at.  If pos is
///              equal to UINT_MAX, then the internal read position is used, 
///              which is the default behavior.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(long long int& val, const unsigned int pos) const 
{
    return ReadPacket(this, val, pos); 
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from the packet at the byte position specified
///          or using the internal read byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the read position the number of
///   bytes read if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data read.
///   \param pos The byte position within the packet to read at.  If pos is
///              equal to UINT_MAX, then the internal read position is used, 
///              which is the default behavior.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(unsigned int& val, const unsigned int pos) const 
{
    return ReadPacket(this, val, pos); 
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from the packet at the byte position specified
///          or using the internal read byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the read position the number of
///   bytes read if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data read.
///   \param pos The byte position within the packet to read at.  If pos is
///              equal to UINT_MAX, then the internal read position is used, 
///              which is the default behavior.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(unsigned short& val, const unsigned int pos) const 
{
    return ReadPacket(this, val, pos); 
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from the packet at the byte position specified
///          or using the internal read byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the read position the number of
///   bytes read if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data read.
///   \param pos The byte position within the packet to read at.  If pos is
///              equal to UINT_MAX, then the internal read position is used, 
///              which is the default behavior.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(unsigned long long int& val, const unsigned int pos) const 
{
    return ReadPacket(this, val, pos); 
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from the packet at the byte position specified
///          or using the internal read byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the read position the number of
///   bytes read if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data read.
///   \param pos The byte position within the packet to read at.  If pos is
///              equal to UINT_MAX, then the internal read position is used, 
///              which is the default behavior.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(float& val, const unsigned int pos) const 
{
    int temp;
    int result = ReadPacket(this, temp, pos); 
    if(result != 0) 
    {
        memcpy(&val, &temp, sizeof(val));
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from the packet at the byte position specified
///          or using the internal read byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the read position the number of
///   bytes read if pos is not used because it is set to UINT_MAX.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set using the SetByteOrder function.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param val The data read.
///   \param pos The byte position within the packet to read at.  If pos is
///              equal to UINT_MAX, then the internal read position is used, 
///              which is the default behavior.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(double& val, const unsigned int pos) const 
{
    long long int temp;
    int result = ReadPacket(this, temp, pos); 
    if(result != 0) 
    {
        memcpy(&val, &temp, sizeof(val));
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from the packet at the byte position specified
///          or using the internal read byte position value.  The latter
///          is the default behavior.
///
///   A call to this function will advance the read position the number of
///   bytes read if pos is not used because it is set to UINT_MAX.
///
///
///   \param buff Pointer to array to save results to.
///   \param len Number of bytes to read/size of buff.
///   \param pos The byte position within the packet to read at.  If pos is
///              equal to UINT_MAX, then the internal read position is used, 
///              which is the default behavior.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(unsigned char *buff, const unsigned int len, const unsigned int pos) const
{
    bool useReadPos = false;
    unsigned int dpos = 0;

    if(pos == UINT_MAX) 
    {
        useReadPos = true;
        dpos = mReadPos;
    }
    else
        dpos = pos;

    if(mpPacket && len + dpos <= mLength) 
    {
        memcpy(buff, &mpPacket[dpos], len);
        if(useReadPos) 
        {
            SetReadPos(mReadPos + len);
        }
        return len;
    }

    return CX_PACKET_FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads in the number of bytes listed and puts the result into a 
///          packet.
///
///   A call to this function will advance the read position the number of
///   bytes read if pos is not used because it is set to UINT_MAX.
///
///
///   \param packet The packet to read into.
///   \param len Number of bytes to read/size and put in the string.
///   \param pos The byte position within the packet to read at.  If pos is
///              equal to UINT_MAX, then the internal read position is used, 
///              which is the default behavior.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(Packet& packet, const unsigned int len, const unsigned int pos) const
{
    bool useReadPos = false;
    unsigned int dpos = 0;

    packet.Clear();

    if(pos == UINT_MAX) 
    {
        useReadPos = true;
        dpos = mReadPos;
    }
    else
        dpos = pos;

    if(len + dpos <= mLength) 
    {
        packet.Reserve(len + 2);
        if(Read(packet.mpPacket, len, pos) == (int)len)
        {
            packet.mLength = len;
            packet.mReadPos = packet.mWritePos = 0;
            return len;
        }
    }

    return CX_PACKET_FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads in the number of bytes listed and puts the result into a string.
///
///   A call to this function will advance the read position the number of
///   bytes read if pos is not used because it is set to UINT_MAX.
///
///
///   \param str String to store result int.
///   \param len Number of bytes to read/size and put in the string.
///   \param pos The byte position within the packet to read at.  If pos is
///              equal to UINT_MAX, then the internal read position is used, 
///              which is the default behavior.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(std::string& str, const unsigned int len, const unsigned int pos) const
{
    bool useReadPos = false;
    unsigned int dpos = 0;

    str.clear();
    if(pos == UINT_MAX) 
    {
        useReadPos = true;
        dpos = mReadPos;
    }
    else
        dpos = pos;

    if(mpPacket && len + dpos <= mLength) 
    {
        for(unsigned int i = dpos; i < dpos + len; i++)
        {
            str.push_back(mpPacket[i]);
        }
        if(useReadPos) 
        {
            SetReadPos(mReadPos + len);
        }
        return len;
    }

    return CX_PACKET_FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from a buffer at the byte position specified.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set by the border parameter.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param buff The buffered data to try read from.
///   \param len The total length of the buffer.
///   \param pos The byte position in the buffer to read at.
///   \param val The read data.
///   \param border The byte order of the data in the buffer.  Set this so that
///   the data is converted to the system byte order automatically. Possible
///   values are CX_PACKET_LITTLE_ENDIAN, CX_PACKET_BIG_ENDIAN.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, unsigned char& val, const unsigned int border)
{
    int result = 0;
    Packet p(border);
    assert(buff && len > 0 && pos < len);
    p.mpPacket = (unsigned char *)buff;
    p.mLength = len;
    p.mReserved = len;
    result = p.Read(val, pos);
    p.mpPacket = NULL;
    p.mLength = 0;
    p.mReserved = 0;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from a buffer at the byte position specified.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set by the border parameter.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param buff The buffered data to try read from.
///   \param len The total length of the buffer.
///   \param pos The byte position in the buffer to read at.
///   \param val The read data.
///   \param border The byte order of the data in the buffer.  Set this so that
///   the data is converted to the system byte order automatically. Possible
///   values are CX_PACKET_LITTLE_ENDIAN, CX_PACKET_BIG_ENDIAN.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, char& val, const unsigned int border)
{
    int result = 0;
    Packet p(border);
    assert(buff && len > 0 && pos < len);
    p.mpPacket = (unsigned char *)buff;
    p.mLength = len;
    p.mReserved = len;
    result = p.Read(val, pos);
    p.mpPacket = NULL;
    p.mLength = 0;
    p.mReserved = 0;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from a buffer at the byte position specified.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set by the border parameter.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param buff The buffered data to try read from.
///   \param len The total length of the buffer.
///   \param pos The byte position in the buffer to read at.
///   \param val The read data.
///   \param border The byte order of the data in the buffer.  Set this so that
///   the data is converted to the system byte order automatically. Possible
///   values are CX_PACKET_LITTLE_ENDIAN, CX_PACKET_BIG_ENDIAN.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, int& val, const unsigned int border)
{
    int result = 0;
    Packet p(border);
    assert(buff && len > 0 && pos < len);
    p.mpPacket = (unsigned char *)buff;
    p.mLength = len;
    p.mReserved = len;
    result = p.Read(val, pos);
    p.mpPacket = NULL;
    p.mLength = 0;
    p.mReserved = 0;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from a buffer at the byte position specified.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set by the border parameter.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param buff The buffered data to try read from.
///   \param len The total length of the buffer.
///   \param pos The byte position in the buffer to read at.
///   \param val The read data.
///   \param border The byte order of the data in the buffer.  Set this so that
///   the data is converted to the system byte order automatically. Possible
///   values are CX_PACKET_LITTLE_ENDIAN, CX_PACKET_BIG_ENDIAN.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, unsigned int& val, const unsigned int border)
{
    int result = 0;
    Packet p(border);
    assert(buff && len > 0 && pos < len);
    p.mpPacket = (unsigned char *)buff;
    p.mLength = len;
    p.mReserved = len;
    result = p.Read(val, pos);
    p.mpPacket = NULL;
    p.mLength = 0;
    p.mReserved = 0;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from a buffer at the byte position specified.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set by the border parameter.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param buff The buffered data to try read from.
///   \param len The total length of the buffer.
///   \param pos The byte position in the buffer to read at.
///   \param val The read data.
///   \param border The byte order of the data in the buffer.  Set this so that
///   the data is converted to the system byte order automatically. Possible
///   values are CX_PACKET_LITTLE_ENDIAN, CX_PACKET_BIG_ENDIAN.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, short& val, const unsigned int border)
{
    int result = 0;
    Packet p(border);
    assert(buff && len > 0 && pos < len);
    p.mpPacket = (unsigned char *)buff;
    p.mLength = len;
    p.mReserved = len;
    result = p.Read(val, pos);
    p.mpPacket = NULL;
    p.mLength = 0;
    p.mReserved = 0;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from a buffer at the byte position specified.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set by the border parameter.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param buff The buffered data to try read from.
///   \param len The total length of the buffer.
///   \param pos The byte position in the buffer to read at.
///   \param val The read data.
///   \param border The byte order of the data in the buffer.  Set this so that
///   the data is converted to the system byte order automatically. Possible
///   values are CX_PACKET_LITTLE_ENDIAN, CX_PACKET_BIG_ENDIAN.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, unsigned short& val, const unsigned int border)
{
    int result = 0;
    Packet p(border);
    assert(buff && len > 0 && pos < len);
    p.mpPacket = (unsigned char *)buff;
    p.mLength = len;
    p.mReserved = len;
    result = p.Read(val, pos);
    p.mpPacket = NULL;
    p.mLength = 0;
    p.mReserved = 0;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from a buffer at the byte position specified.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set by the border parameter.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param buff The buffered data to try read from.
///   \param len The total length of the buffer.
///   \param pos The byte position in the buffer to read at.
///   \param val The read data.
///   \param border The byte order of the data in the buffer.  Set this so that
///   the data is converted to the system byte order automatically. Possible
///   values are CX_PACKET_LITTLE_ENDIAN, CX_PACKET_BIG_ENDIAN.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, long long int& val, const unsigned int border)
{
    int result = 0;
    Packet p(border);
    assert(buff && len > 0 && pos < len);
    p.mpPacket = (unsigned char *)buff;
    p.mLength = len;
    p.mReserved = len;
    result = p.Read(val, pos);
    p.mpPacket = NULL;
    p.mLength = 0;
    p.mReserved = 0;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from a buffer at the byte position specified.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set by the border parameter.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param buff The buffered data to try read from.
///   \param len The total length of the buffer.
///   \param pos The byte position in the buffer to read at.
///   \param val The read data.
///   \param border The byte order of the data in the buffer.  Set this so that
///   the data is converted to the system byte order automatically. Possible
///   values are CX_PACKET_LITTLE_ENDIAN, CX_PACKET_BIG_ENDIAN.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, unsigned long long int& val, const unsigned int border)
{
    int result = 0;
    Packet p(border);
    assert(buff && len > 0 && pos < len);
    p.mpPacket = (unsigned char *)buff;
    p.mLength = len;
    p.mReserved = len;
    result = p.Read(val, pos);
    p.mpPacket = NULL;
    p.mLength = 0;
    p.mReserved = 0;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from a buffer at the byte position specified.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set by the border parameter.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param buff The buffered data to try read from.
///   \param len The total length of the buffer.
///   \param pos The byte position in the buffer to read at.
///   \param val The read data.
///   \param border The byte order of the data in the buffer.  Set this so that
///   the data is converted to the system byte order automatically. Possible
///   values are CX_PACKET_LITTLE_ENDIAN, CX_PACKET_BIG_ENDIAN.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, float& val, const unsigned int border)
{
    int result = 0;
    Packet p(border);
    assert(buff && len > 0 && pos < len);
    p.mpPacket = (unsigned char *)buff;
    p.mLength = len;
    p.mReserved = len;
    result = p.Read(val, pos);
    p.mpPacket = NULL;
    p.mLength = 0;
    p.mReserved = 0;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the data from a buffer at the byte position specified.
///
///   Byte order conversion is performed automatically based on the 
///   byte order set by the border parameter.  The default
///   byte order is used is CX_PACKET_LITTLE_ENDIAN.
///
///   \param buff The buffered data to try read from.
///   \param len The total length of the buffer.
///   \param pos The byte position in the buffer to read at.
///   \param val The read data.
///   \param border The byte order of the data in the buffer.  Set this so that
///   the data is converted to the system byte order automatically. Possible
///   values are CX_PACKET_LITTLE_ENDIAN, CX_PACKET_BIG_ENDIAN.
///
///   \return The number of bytes read, 0 if nothing read.
///
////////////////////////////////////////////////////////////////////////////////////
int Packet::Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, double& val, const unsigned int border)
{
    int result = 0;
    Packet p(border);
    assert(buff && len > 0 && pos < len);
    p.mpPacket = (unsigned char *)buff;
    p.mLength = len;
    p.mReserved = len;
    result = p.Read(val, pos);
    p.mpPacket = NULL;
    p.mLength = 0;
    p.mReserved = 0;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to written data buffer.
///
////////////////////////////////////////////////////////////////////////////////////
const unsigned char *Packet::Ptr() const
{
    return mpPacket;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to written data buffer.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned char *Packet::Ptr()
{
    return mpPacket;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copies contents of packet.
///
////////////////////////////////////////////////////////////////////////////////////
Packet& Packet::operator=(const Packet& another)
{
    if(this != &another && !mWrappedPacketFlag)
    {
        if(mLength < another.mLength && another.mLength > 0)
        {
            Reserve(another.mLength + 1);
        }
        if(another.mLength > 0 && another.mpPacket)
        {
            //  Just in case.
            if(!mpPacket)
            {
                Reserve(another.mLength + 1);
            }
            memcpy(mpPacket, another.mpPacket, sizeof(unsigned char)*another.mLength);
        }
        mLength = another.mLength;
        mReadPos = another.mReadPos;
        mWritePos = another.mWritePos;
        mByteOrder = another.mByteOrder;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds packet data to the end of the current packet.
///
////////////////////////////////////////////////////////////////////////////////////
Packet& Packet::operator +=(const Packet& another)
{
    Write(another, mLength);
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to index into internal array.
///
///   \param[in] index Index into packet byte array [0, Length()).
///
///   \return Value in packet buffer at index (0 on out of range).
///
////////////////////////////////////////////////////////////////////////////////////
unsigned char Packet::operator [](const unsigned int index) const
{
    if(index < mLength && mpPacket)
    {
        return mpPacket[index];
    }
    assert("CxUtils::Packet - Out of Range <operator[]>");
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Grows the packet size to fit the size desired.
///
///   \param size The size to grow to fit.
///
////////////////////////////////////////////////////////////////////////////////////
void Packet::GrowPacket(const unsigned int size)
{
    if(mWrappedPacketFlag == false)
    {
        unsigned int newSize = mReserved;
        if(size >= UINT_MAX)
            return;

        while(newSize < size + 1 && newSize + CX_PACKET_BLOCK_SIZE < UINT_MAX)
            newSize += CX_PACKET_BLOCK_SIZE;
        if(newSize > mReserved)
            Reserve(newSize);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to load a exported capture file constructed by the program
///          Wireshark.  
///
///   This method will only load an export of the packet data, with the file
///   containing contents in a specifc format exported from WireShark.
///
///
///   \param[in] filename Name of the file containing data.
///   \param[out] packets Packet data extracted.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Packet::LoadWiresharkCapturePacketExport(const std::string &filename, Packet::List &packets)
{
    bool result = false;

    packets.clear();

    std::fstream infile;
    infile.open(filename.c_str());
    if(infile.is_open() == false)
    {
        return result;
    }
    Packet p;
    std::vector<std::string> lines, tokens;
    std::string l;
    while(infile.eof() == false)
    {
        CxUtils::FileIO::ReadLine(infile, l);
        if(l.size() > 0)
        {
            lines.clear();
            lines.push_back(l);
            while(CxUtils::FileIO::ReadLine(infile, l))
            {
                lines.push_back(l);
            }
            // Extract the data.
            p.Clear();
            unsigned long ul = 0;
            std::vector<std::string>::iterator d;
            for(d = lines.begin();
                d != lines.end();
                d++)
            {
                // Read the byte data out.
                ul = 0;
                tokens = CxUtils::FileIO::Tokenize(*d, " ");
                for(unsigned int i = 2; i < 18 && i < tokens.size(); i++)
                {
                    if(tokens[i].size() < 1)
                    {
                        break;
                    }
                    ul = strtoul(tokens[i].c_str(), NULL, 16);
                    p.WriteByte((unsigned char)ul);
                    result = true;
                }
            }
            packets.push_back(p);
        }
    }

    return result;
}


/* End of File */
