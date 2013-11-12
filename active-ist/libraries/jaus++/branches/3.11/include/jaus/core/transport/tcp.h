////////////////////////////////////////////////////////////////////////////////////
///
///  \file tcp.h
///  \brief TCP interface for transport services.
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
#ifndef __JAUS_CORE_TRANSPORT_TCP_CONNECTION__H
#define __JAUS_CORE_TRANSPORT_TCP_CONNECTION__H

#include "jaus/core/transport/connection.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class TCP
    ///   \brief Defines a JTCP Connection.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL TCP : public Connection
    {
    public:
        /** TCP Parameters. */
        class JAUS_CORE_DLL Parameters : public Connection::Parameters
        {
        public:
            Parameters();
            virtual ~Parameters();
            virtual bool LoadSettings(const std::string& xmlSettingsFile);
            TCP::Parameters& operator=(const Info& info)
            {
                *( (Info*)this) = *( (Info *)&info);
                return *this;
            }
            TCP::Parameters& operator=(const TCP::Parameters& params);
            bool mClientFlag;           ///<  Client connection.
        };

        const static unsigned short Port = 3794;         ///< JAUS UDP/TCP Port Number == "jaus".
        const static unsigned int OverheadSizeBytes = 73;///< JTCP Overhead in bytes including JAUS General Header
        const static Byte Version = 0x02;                ///< JTCP Header Version.

        TCP(const bool singleThread = true);
        virtual ~TCP();
        virtual bool Initialize(const Connection::Parameters* parameters);
        virtual void Shutdown();
        virtual bool IsConnected() const;
        virtual Connection::Statistics GetStatistics();
        virtual bool CanBroadcast(const int transportType = Transport::All) const;
        virtual bool SendPacket(const Packet& packet, 
                                const Header& packetHeader) const;
        virtual void UpdateConnection();
        virtual Connection* CreateConnection(const Address& id,
                                             const Connection::Info* destination);
        virtual const Packet& GetTransportHeader() const;
        /** Gets the recommend maximum packet size for the transport. */
        virtual unsigned int GetMaximumPacketSizeInBytes() const { return mParameters.mMaxPacketSizeBytes; }
        /** Gets the overhead for each packet added by medium in bytes. */
        inline virtual unsigned int GetTransportOverheadInBytes() const { return OverheadSizeBytes; }
        virtual Parameters* CreateParameters() const { return new TCP::Parameters(); }
        bool IsClient() const;
    protected:
        void CloseSocket();
        void DeleteSocket();
        void RemoveConnection(TCP* connection);
        void ListenForConnections();
        void ReceiveIncommingData();
        void* mpSocket;             ///<  The actual network connection.
        bool mListenerFlag;         ///<  Is this a TCP listener.
        Packet mTransportHeader;    ///<  Transport header data.
        Parameters mParameters;     ///<  Connection options/parameters.
        TCP* mpParent;              ///<  Parent server/connection.
        std::set<TCP*> mNewConnections; ///<  New connections.
        Packet mLargeBuffer;            ///<  Main buffer for incomming serialized data.
        Packet mSmallBuffer;            ///<  Secondary buffer for receiving incomming serialized data.
    };
}

#endif
/*  End of File */
