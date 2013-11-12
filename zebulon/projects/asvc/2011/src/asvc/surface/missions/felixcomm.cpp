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
#include "asvc/surface/missions/felixcom.h"
#include <sstream>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>

using namespace Zebulon;
using namespace ASVC;

const std::string FelixCom::Air = "AIR";
const std::string FelixCom::Water = "WATER";
const std::string FelixCom::Earth = "EARTH";
const std::string FelixCom::Fire = "FIRE";


/** Constructor. */
FelixCom::FelixCom()
{

}

/** Destructor */
FelixCom::~FelixCom()
{
    Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes communication.
///
///  \param[in] port Port to connect on.
///  \param[in] serverIP Server IP address.
///
////////////////////////////////////////////////////////////////////////////////////
int FelixCom::Initialize(const unsigned short port,
                         const CxUtils::IP4Address& serverIP)
{
    Shutdown();
    return mTcp.InitializeSocket(serverIP, port);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Shutdown communication.
///
////////////////////////////////////////////////////////////////////////////////////
void FelixCom::Shutdown()
{
    this->mTcp.Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends PING message.
///
///  \return True of PONG received = success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool FelixCom::SendPing() const
{
    std::string message = "PING\n";
    CxUtils::Packet response;
    if(mTcp.Send(message.c_str(), (unsigned int)message.size()))
    {
        response.Reserve(512);
        if(mTcp.Recv(response, 4, 100))
        {
            if(strstr((const char *)response.Ptr(), "PONG") != NULL)
            {
                return true;
            }
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends the location of what we believe the "hot" target is.
///
///  \param[in] typeName Type of targe (see name labels in class header).
///  \param[in] position GPS position of target.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool FelixCom::SendLocation(const std::string& typeName,
                            CxUtils::Wgs& position) const
{
    std::stringstream str;

    str << "REPORT;" << typeName
        << "@" << position.mLatitude << "," << position.mLongitude << "\n";
    CxUtils::Packet response;
    if(mTcp.Send(str.str().c_str(), (unsigned int)str.str().size()))
    {
        response.Reserve(512);
        if(mTcp.Recv(response, 256, 100))
        {
            if(strstr((const char *)response.Ptr(), "SUCCESS") != NULL)
            {
                return true;
            }
            else
            {
                std::cout << "FELIX COM Received: " << response.Ptr();
            }
        }
    }
    return false;
}
/*  End of File */
