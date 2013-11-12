//////////////////////////////////////////////////////////////////////////////////////
///
/// \file dataeuler.cpp
/// \brief This file contains the message CORAL_DATA_EULER.
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
#include "coralahrs/messages/dataeuler.h"
#include <cxutils/math/cxmath.h>
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
DataEuler::DataEuler() : Message( (unsigned char)Message::DataEuler)
{
    mSystemTimeMs = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
DataEuler::DataEuler(const DataEuler& message) : Message( (unsigned char)Message::DataEuler)
{
    mSystemTimeMs = 0;
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
DataEuler::~DataEuler()
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
int DataEuler::WriteMessageBody(CxUtils::Packet& packet) const
{
    short x, y, z;
    int written = 0;
    x = y = z = 0;
    written += packet.Write(mSystemTimeMs);
    // Convert from double to fixed point for storage in packet.
    x = DoubleToCoral(mEulerAngles.mX);
    y = DoubleToCoral(mEulerAngles.mY);
    if(mEulerAngles.mZ < 0)
    {
        z = DoubleToCoral((mEulerAngles.mZ + CxUtils::CX_TWO_PI));
    }
    else
    {
        z = DoubleToCoral(mEulerAngles.mZ);
    }
    written += packet.Write(x);
    written += packet.Write(y);
    written += packet.Write(z);

    return written;
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
int DataEuler::ReadMessageBody(const CxUtils::Packet& packet)
{
    short x, y, z;
    int read = 0;
    x = y = z = 0;
    read += packet.Read(mSystemTimeMs);

    read += packet.Read(x);
    read += packet.Read(y);
    read += packet.Read(z);
    // Convert from fixed point to floating point
    mEulerAngles(CoralToDouble(x), CoralToDouble(y), CoralToDouble(z));
    // Convert heading to [-pi,pi].
    if(mEulerAngles.mZ > CxUtils::CX_PI)
    {
        mEulerAngles.mZ -= CxUtils::CX_TWO_PI;
    }

    return read;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints data to console window.
///
////////////////////////////////////////////////////////////////////////////////////
void DataEuler::Print() const 
{
    cout << "Time: " << mSystemTimeMs << endl;
    cout << "Euler: <" << CxUtils::CxToDegrees(mEulerAngles.mX)
         << ", " << CxUtils::CxToDegrees(mEulerAngles.mY)
         << ", " << CxUtils::CxToDegrees(mEulerAngles.mZ) << ">\n";
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
DataEuler& DataEuler::operator=(const DataEuler& message)
{
    mSystemTimeMs = message.mSystemTimeMs;
    mEulerAngles = message.mEulerAngles;
    return *this;
}


/*  End of File */
