//////////////////////////////////////////////////////////////////////////////////////
///
/// \file dataquat.cpp
/// \brief This file contains the message CORAL_DATA_QUAT.
///
/// Author(s): Jonathan Mohlenhoff<br>
/// Created: 1/24/2009<br>
/// Copyright (c) 2009<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: jmohlenh@ist.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the Robotics Laboratory, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
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
#include "coralahrs/messages/dataquat.h"
#include <iostream>

using namespace std;
using namespace Zebulon;
using namespace Compass;
using namespace CoralAHRS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
DataQuat::DataQuat() : Message( (unsigned char)Message::DataQuat)
{
    mSystemTimeMs = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
DataQuat::DataQuat(const DataQuat& message) : Message( (unsigned char)Message::DataQuat)
{
    mSystemTimeMs = 0;
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
DataQuat::~DataQuat()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes contents of message body to packet.
///
///   \param[in] packet The packet to write data to.
///
///   \return Number of bytes written to packet.
///
////////////////////////////////////////////////////////////////////////////////////
int DataQuat::WriteMessageBody(CxUtils::Packet& packet) const
{
    int write = 0;

    write += packet.Write(mSystemTimeMs);
    write += packet.Write((short)(mOrientation.mW * 4096));
    write += packet.Write((short)(mOrientation.mX * 4096));
    write += packet.Write((short)(mOrientation.mY * 4096));
    write += packet.Write((short)(mOrientation.mZ * 4096));

    return write;    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads contents of message body from the packet, saving to internal
///   data members.
///
///   \param[in] packet The packet to read data from.
///
///   \return Number of bytes read from packet.
///
////////////////////////////////////////////////////////////////////////////////////
int DataQuat::ReadMessageBody(const CxUtils::Packet& packet)
{
    short w, x, y, z;
    int read = 0;

    read += packet.Read(mSystemTimeMs);
    read += packet.Read(w);
    read += packet.Read(x);
    read += packet.Read(y);
    read += packet.Read(z);

    mOrientation = CxUtils::Quaternion(w/4096.0, x/4096.0, y/4096.0, z/4096.0);

    return read;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints data to console window.
///
////////////////////////////////////////////////////////////////////////////////////
void DataQuat::Print() const 
{
    cout << mSystemTimeMs << endl;
    mOrientation.Print();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
DataQuat& DataQuat::operator=(const DataQuat& message)
{
    mSystemTimeMs = message.mSystemTimeMs;
    mOrientation = message.mOrientation;
    return *this;
}


/*  End of File */
