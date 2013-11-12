//////////////////////////////////////////////////////////////////////////////////////
///
/// \file felixcom.h
/// \brief Communication interface so that missions can send messages
///        to the networked server at boat competition setup by Felix.
///
/// Author(s): Daniel Barber<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:dbarber@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
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
//////////////////////////////////////////////////////////////////////////////////////
#ifndef ZEBULON_ASVC_FELIXCOM_H
#define ZEBULON_ASVC_FELIXCOM_H


#include <cxutils/networking/tcpclient.h>
#include <cxutils/math/coordinates.h>


namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class FelixCom
        ///  \brief TCP interface for sending messages to a proprietary system setup by
        ///   Felix for the RoboBoats competition.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class FelixCom
        {
        public:
            static const std::string SPADE;
            static const std::string HEART;
            static const std::string DIAMOND;
            static const std::string CLUB;
            FelixCom();
            ~FelixCom();
            int Initialize(const unsigned short port = 5555,
                           const CxUtils::IP4Address& serverIP = "10.11.12.208");
            void Shutdown();
            bool IsConnected() const { return mTcp.IsValid(); }
            bool SendPing() const;
            bool SendLocation(const std::string& typeName,
                              CxUtils::Wgs& position) const;
        protected:
            CxUtils::TcpClient mTcp; ///<  TCP connection.
        };
    }
}

#endif // ZEBULON_ASVC_FELIXCOM_H
