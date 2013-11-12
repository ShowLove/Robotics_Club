////////////////////////////////////////////////////////////////////////////////////
///
///  \file udp.h
///  \brief UDP interface for transport services.
///
///  <br>Author(s): Daniel Barber
///  <br>Copyright (c) 2011
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
#ifndef __JAUS_CORE_TRANSPORT_UDP_CONNECTION__H
#define __JAUS_CORE_TRANSPORT_UDP_CONNECTION__H

#include "jaus/core/transport/connection.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class UDP
    ///   \brief Defines a JUDP Connection.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL UDP : public Connection
    {
    public:
        /** UDP Parameters. */
        class JAUS_CORE_DLL Parameters : public Connection::Parameters
        {
        public:
            Parameters();
            virtual ~Parameters();
            virtual bool LoadSettings(const std::string& xmlSettingsFile);
            UDP::Parameters& operator=(const Info& info)
            {
                *( (Info*)this) = *( (Info *)&info);
                return *this;
            }
            UDP::Parameters& operator=(const UDP::Parameters& params);
            bool mClientFlag;           ///<  Client connection.
            bool mUseBroadcastingFlag;  ///<  Broadcast, or multicast?
            IP4Address mMulticastIP;    ///<  Multicast group.
            unsigned char mTimeToLive;  ///<  Time to Live TTL for UDP.
        };

        const static unsigned short Port = 3794;            ///< JAUS UDP/UDP Port Number == "jaus".
        const static unsigned int OverheadSizeBytes = 61;   ///< Total overhead in bytes include JAUS General header and JUDP 
        const static Byte Version = 0x02;                   ///< JUDP Header Version.

        UDP(const bool singleThread = true);
        virtual ~UDP();
        virtual bool Initialize(const Connection::Parameters* parameters);
        virtual void Shutdown();
        virtual bool IsConnected() const;
        virtual Connection::Statistics GetStatistics();
        virtual bool CanBroadcast(const int transportType = Transport::All) const;
        virtual bool SendPacket(const Packet& packet, 
                                const Header& packetHeader) const;
        virtual void UpdateConnection();
        virtual bool ConnectionChanged(const Connection::Info* destination)  const;
        virtual Connection* CreateConnection(const Address& id,
                                             const Connection::Info* destination);
        virtual const Packet& GetTransportHeader() const;
        /** Gets the recommend maximum packet size for the transport. */
        virtual unsigned int GetMaximumPacketSizeInBytes() const { return mParameters.mMaxPacketSizeBytes; }
        /** Gets the overhead for each packet added by medium in bytes. */
        inline virtual unsigned int GetTransportOverheadInBytes() const { return OverheadSizeBytes; }
        virtual Parameters* CreateParameters() const { return new UDP::Parameters(); }
        /** Gets the port number for the connection. */
        inline unsigned short GetDestPortNumber() const { return mParameters.mDestPortNumber; }
        /** Gets the port number for the connection. */
        inline unsigned short GetSourcePortNumber() const { return mParameters.mSourcePortNumber; }
    protected:
        void CloseSocket();
        void DeleteSocket();
        void ReceiveIncommingData();
        void* mpSocket;             ///<  The actual network connection.
        Packet mTransportHeader;    ///<  Transport header data.
        Parameters mParameters;     ///<  Connection options/parameters.
        Packet mSendCache;          ///<  Reusable packet for sending.
        Packet mRecvBuffer;         ///<  Main buffer for incomming serialized data.
    };
}

#endif
/*  End of File */
