////////////////////////////////////////////////////////////////////////////////////
///
///  \file packet.h
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
#ifndef __CXUTILS_PACKET_H
#define __CXUTILS_PACKET_H

#include <limits.h>
#include <string>
#include <vector>
#include <list>
#include "cxbase.h"

#define CX_PACKET_BYTE_SWAP_16(a)   \
    (((a & 0xFF00) >> 8)|           \
     ((a & 0x00FF) << 8))           \

#define CX_PACKET_BYTE_SWAP_32(a)   \
    (((a & 0xFF000000) >> 24)|      \
     ((a & 0x00FF0000) >> 8) |      \
     ((a & 0x0000FF00) << 8) |      \
     ((a & 0x000000FF) << 24))      \

#define CX_PACKET_BYTE_SWAP_64(a)               \
    (((a & 0xFF00000000000000ULL) >> 56)|       \
     ((a & 0x00FF000000000000ULL) >> 40)|       \
     ((a & 0x0000FF0000000000ULL) >> 24)|       \
     ((a & 0x000000FF00000000ULL) >> 8 )|       \
     ((a & 0x00000000FF000000ULL) << 8 )|       \
     ((a & 0x0000000000FF0000ULL) << 24)|       \
     ((a & 0x000000000000FF00ULL) << 40)|       \
     ((a & 0x00000000000000FFULL) << 56))       \


namespace CxUtils
{
    const unsigned int CX_PACKET_OK             = 1;    ///<  Success value on return.
    const unsigned int CX_PACKET_FAILURE        = 0;    ///<  Failure value on return.
    const unsigned int CX_PACKET_BLOCK_SIZE     = 32;   ///<  Allocate memory in this size byte blocks.
    const unsigned int CX_PACKET_LITTLE_ENDIAN  = 0;    ///<  Little endian byte order.
    const unsigned int CX_PACKET_BIG_ENDIAN     = 1;    ///<  Big endian byte order.

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Packet
    ///   \brief Data structure for storing message data into a buffer.
    ///
    ///   All written messages are stored within a Packet.  This data structure 
    ///   handles all memory allocation and deletion for buffered data.  It also
    ///   has utility functions for encoding different size integers and buffers
    ///   into the packet.
    ///
    ///   One bonus feature of this data structure is that it can automatically
    ///   handle byte order conversions.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Packet
    {        
        friend class Wrapper;
    public:
        typedef std::vector<Packet> List; ///<  List of packets.
        typedef std::list<Packet> Queue;  ///<  Queue of messages.
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Wrapper
        ///   \brief Method to wrap an existing byte array within a Packet for easy
        ///          Read/Write operations.  The wrapped buffer is not resized or
        ///          deleted by the Packet.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CX_UTILS_DLL Wrapper
        {
        public:
            Wrapper();
            Wrapper(const Wrapper& wrapper);
            Wrapper(unsigned char* buffer, const unsigned int len);
            ~Wrapper();
            bool Create(unsigned char* buffer, const unsigned int len);
            void Clear();
            Packet* operator->();
            const Packet* operator->() const;
            Packet* GetData();
            const Packet* GetData() const;
            Wrapper& operator=(const Wrapper& wrapper);
        private:
            CxUtils::Packet* mPacket;    ///<  Pointer to packet wrapping byte array.
        };
        Packet(const unsigned int border = CX_PACKET_LITTLE_ENDIAN);
        Packet(const Packet& another);
        virtual ~Packet();
        int SetByteOrder(const unsigned int order = CX_PACKET_LITTLE_ENDIAN);
        int SetWritePos(const unsigned int pos = 0);
        int SetReadPos(const unsigned int pos = 0) const;
        int SetLength(const unsigned int len = 0);
        int Delete(const unsigned int len, const unsigned int start = 0);
        int Insert(const unsigned char *buff, const unsigned int len, const unsigned int pos = 0);
        int InsertCharacter(const unsigned char value, const unsigned int count, const unsigned int pos = 0);
        unsigned int GetByteOrder() const;
        unsigned int GetWritePos() const;
        unsigned int GetReadPos() const;
        unsigned int Size() const;
        unsigned int Length() const;
        unsigned int Reserved() const { return mReserved; }
        void Reserve(const unsigned int size);
     
        virtual int Write(const unsigned char val, const unsigned int pos = UINT_MAX);
        virtual int Write(const char val, const unsigned int pos = UINT_MAX);
        virtual int Write(const int val, const unsigned int pos = UINT_MAX);
        virtual int Write(const short val, const unsigned int pos = UINT_MAX);
        virtual int Write(const unsigned short val, const unsigned int pos = UINT_MAX);
        virtual int Write(const unsigned int val, const unsigned int pos = UINT_MAX);
        virtual int Write(const long long int val, const unsigned int pos = UINT_MAX);
        virtual int Write(const unsigned long long int val, const unsigned int pos = UINT_MAX);
        virtual int Write(const float val, const unsigned int pos = UINT_MAX);
        virtual int Write(const double val, const unsigned int pos = UINT_MAX);
        virtual int Write(const unsigned char* buff, const unsigned int len, const unsigned int pos = UINT_MAX);
        virtual int Write(const Packet& packet, const unsigned int pos = UINT_MAX);
        virtual int Write(const std::string& str, const unsigned int pos = UINT_MAX);
        virtual int WriteByte(const unsigned char byte) { return Write(byte); }
        
        virtual int Read(unsigned char& val, const unsigned int pos = UINT_MAX) const;
        virtual int Read(char& val, const unsigned int pos = UINT_MAX) const;
        virtual int Read(int& val, const unsigned int pos = UINT_MAX) const;
        virtual int Read(short& val, const unsigned int pos = UINT_MAX) const;
        virtual int Read(unsigned short& val, const unsigned int pos = UINT_MAX) const;
        virtual int Read(unsigned int& val, const unsigned int pos = UINT_MAX) const;
        virtual int Read(long long int& val, const unsigned int pos = UINT_MAX) const;
        virtual int Read(unsigned long long int& val, const unsigned int pos = UINT_MAX) const;
        virtual int Read(float& val, const unsigned int pos = UINT_MAX) const;
        virtual int Read(double& val, const unsigned int pos = UINT_MAX) const;
        virtual int Read(unsigned char *buff, const unsigned int len, const unsigned int pos = UINT_MAX) const;
        virtual int Read(Packet& packet, const unsigned int len, const unsigned int pos = UINT_MAX) const;
        virtual int Read(std::string& str, const unsigned int len, const unsigned int pos = UINT_MAX) const;

        static int Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, char& val, const unsigned int border = CX_PACKET_LITTLE_ENDIAN);
        static int Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, unsigned char& val, const unsigned int border = CX_PACKET_LITTLE_ENDIAN);
        static int Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, int& val, const unsigned int border = CX_PACKET_LITTLE_ENDIAN);
        static int Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, short& val, const unsigned int border = CX_PACKET_LITTLE_ENDIAN);
        static int Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, unsigned short& val, const unsigned int border = CX_PACKET_LITTLE_ENDIAN);
        static int Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, unsigned int& val, const unsigned int border = CX_PACKET_LITTLE_ENDIAN);
        static int Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, long long int& val, const unsigned int border = CX_PACKET_LITTLE_ENDIAN);
        static int Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, unsigned long long int& val, const unsigned int border = CX_PACKET_LITTLE_ENDIAN);
        static int Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, float& val, const unsigned int border = CX_PACKET_LITTLE_ENDIAN);
        static int Read(const unsigned char *buff, const unsigned int len, const unsigned int pos, double& val, const unsigned int border = CX_PACKET_LITTLE_ENDIAN);
        
        void Clear(const bool setZeroFlag = true);
        void Destroy();
        void Print(const unsigned int bytesPerLine = 8) const;
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \return CX_PACKET_LITTLE_ENDIAN if the current machine's byte order is
        ///    little endian, otherwise CX_PACKET_BIG_ENDIAN.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static unsigned int GetMachineEndianness()
        {
            static int endianness = -1;

            if( endianness == -1 ) 
            {
                int i = 1;
                char *p = (char *) &i;
                if (p[0] == 1) 
                { // Lowest address contains the least significant byte
                    endianness = CX_PACKET_LITTLE_ENDIAN;
                }
                else {
                    endianness = CX_PACKET_BIG_ENDIAN;
                }
            }
            return endianness;
        }
        const unsigned char* Ptr() const;
        unsigned char* Ptr();
        Packet& operator=(const Packet& another);        
        Packet& operator+=(const Packet& another);
        unsigned char operator[](const unsigned int index) const;
        static bool LoadWiresharkCapturePacketExport(const std::string& filename,
                                                     Packet::List& packets);
    protected:
        void GrowPacket(unsigned int size);
        unsigned char* mpPacket;    ///<  Writed data.
        unsigned int   mLength;     ///<  Length of written data.
        unsigned int   mReserved;   ///<  Amount of data reserved.
        unsigned int   mWritePos;   ///<  Encoding position in packet.
        unsigned int   mReadPos;    ///<  Read position in packet.
        unsigned int   mByteOrder;  ///<  Byte order to write data as within packet.
        bool mWrappedPacketFlag;    ///<  Flag used to force a fixed size packet (mainly used by Wrapper).
        /// Writes a value to buffer in packet.
        template <class T>
        static inline int WritePacket(Packet* p, const T val, const unsigned int pos = UINT_MAX)
        {
            if (!p)
                return 0;

            bool overwrite = false;
            unsigned int epos = 0;
            /*  If the write position passed is equal to
                UINT_MAX, then use the internal write
                position parameter of the packet. */
            if(pos == UINT_MAX) 
            {
                epos = p->mWritePos;
            }
            else 
            {
                overwrite = true;
                epos = pos;
            }
            // Don't write if passed the length of the packet.
            if(epos > p->mLength || epos == UINT_MAX) 
            {
                return 0;
            }

            T data;
            unsigned int bytes = sizeof(data);
            //  Check for byte order conversion.
            if(p->GetMachineEndianness() != p->mByteOrder && bytes > 1) 
            {
                switch(bytes)
                {
                case 2:
                    data = (T)CX_PACKET_BYTE_SWAP_16(val);
                    break;
                case 4:
                    data = (T)CX_PACKET_BYTE_SWAP_32(val);
                    break;
                case 8:
                    data = (T)CX_PACKET_BYTE_SWAP_64(val);
                    break;
                default:
                    data = val;  //  Don't do anything for a single byte.
                    break;
                };
            }
            else
            {
                data = val;
            }

            // If resize is disabled, and we need to resize to fit the
            // new data, return failure (0 bytes written).
            if((epos + bytes) > p->mReserved && p->mWrappedPacketFlag)
            {
                return 0;
            }

            /*  Enlarge the packet if it is not
                big enough to fit the data. */
            while((epos + bytes) > p->mReserved) 
            {
                p->GrowPacket( epos + bytes + 2 );
            }

            memcpy(&p->mpPacket[epos], &data, bytes);

            /*  If we use the position value passed
                then we are writing over existing data, and
                not adjusting the current write position. */
            if(!overwrite)
                p->mWritePos += bytes;
            //  Adjust the length of the packet if needed.
            if (epos + bytes > p->mLength) 
            {
                p->mLength = epos + bytes;
                if(p->mLength < p->mReserved)
                {
                    p->mpPacket[p->mLength] = 0;
                }
            }

            return bytes;
        }

        ///  Read and read value from packet.
        template <class T>
        static inline int ReadPacket(const Packet* packet, T& val, unsigned int pos = UINT_MAX)
        {
            Packet* p = (Packet* )(packet);
            if(!p->mpPacket)
                return 0;

            bool useReadPos = false;
            unsigned int dpos = 0;
            /*  If the read position passed is equal to 
                UINT_MAX, then we will use the internal
                read position of the packet instead. */
            if(pos == UINT_MAX) 
            {
                useReadPos = true;
                dpos = p->mReadPos;
            }
            else {
                dpos = pos;
            }
            unsigned int bytes = sizeof(val);
            if(dpos + bytes <= p->mLength) 
            {
                memcpy(&val, &p->mpPacket[dpos], bytes); 
                //  Check for byte order conversion.
                if(p->GetMachineEndianness() != p->mByteOrder && bytes > 1) 
                {
                    switch(bytes)
                    {
                    case 2:
                        val = (T)CX_PACKET_BYTE_SWAP_16(val);
                        break;
                    case 4:
                        val = (T)CX_PACKET_BYTE_SWAP_32(val);
                        break;
                    case 8:
                        val = (T)CX_PACKET_BYTE_SWAP_64(val);
                        break;
                    default:
                        val = val;  //  Don't do anything for a single byte.
                        break;
                    };
                }
                /*  If we are using the packets internal
                    read position value, then we must update
                    it to reflect the number of bytes read. */
                if(useReadPos) 
                {
                    p->mReadPos += bytes;
                }
                return bytes;
            }
            else
                return 0;
        }

        ///  Read and read value from packet.
        template <class T>
        static inline int ReadAtPos(const unsigned char *buff, const unsigned int len, const unsigned int pos, T& val, unsigned int border = CX_PACKET_LITTLE_ENDIAN)
        {
            if(!buff || len == 0 || pos >= len || border > 1)
                return 0;

            
            unsigned int bytes = sizeof(val);
            if(pos + bytes <= len) 
            {
                memcpy(&val, &buff[pos], bytes); 
                //  Check for byte order conversion.
                if(Packet::GetMachineEndianness() != border && bytes > 1) 
                {
                    switch(bytes)
                    {
                    case 2:
                        val = (T)CX_PACKET_BYTE_SWAP_16(val);
                        break;
                    case 4:
                        val = (T)CX_PACKET_BYTE_SWAP_32(val);
                        break;
                    case 8:
                        val = (T)CX_PACKET_BYTE_SWAP_64(val);
                        break;
                    default:
                        val = val;  //  Don't do anything for a single byte.
                        break;
                    };
                }
                return bytes;
            }
            else
                return 0;
        }
    };
}

#endif
/*  End of File */

