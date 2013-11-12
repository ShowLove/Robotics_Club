/////////////////////////////////////////////////////////////////////////////////////
///
/// \file continuousoutputresponse.cpp
/// \brief Message class for storing continous output from the SICK LIDAR.
///
/// Author(s): Jon Carbone<br>
/// Created: 3/13/2008<br>
/// Last Modified: 3/13/2008<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: joncarbone@gmail.com<br>
/// Web: http://robotics.ucf.edu <br>
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
////////////////////////////////////////////////////////////////////////////////////
#include "sick/messages/continuousoutputresponse.h"

using namespace Zebulon;
using namespace Laser;
using namespace SICK;

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///////////////////////////////////////////////////////////////////////////////
ContinuousOutputResponse::ContinuousOutputResponse() : Message(Message::ContinuousOutput)
{
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy Constructor.
///
///////////////////////////////////////////////////////////////////////////////
ContinuousOutputResponse::ContinuousOutputResponse(const ContinuousOutputResponse& msg) : Message(Message::ContinuousOutput)
{
    *this = msg;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Writes a ContinuousOutputResponse message to a packet.
///
///  \param packet Packet to write the ContinuousOutputResponse message to
///
///  \return The number of bytes written to the packet
///
///////////////////////////////////////////////////////////////////////////////
unsigned int ContinuousOutputResponse::Write(CxUtils::Packet& packet) const
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Reads a ContinuousOutputResponse message and populates the internal members.
///         The range values are stored into the mPoints array.
///         TBD: Only read set values based, which can be determined either by
///         looking at the scanning angle and angular resolution or by identifying
///         repetitive unset values in the message.
///
///  \param packet The packet containing the ContinuousOutputResponse message
///
///  \return 1 if the message was successfully parsed, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
unsigned int ContinuousOutputResponse::Read(const CxUtils::Packet& packet)
{
    unsigned char response = 0;
    unsigned short dataSize = GetDataSize(packet);

    if(dataSize > 0 &&
        packet.Size() >= (unsigned int)(dataSize + 6))
    {
        packet.Read(response, packet.GetReadPos() + 4);

        if( Message::ContinuousOutput == response )
        {
            mTimeStamp.SetCurrentTime();
            unsigned short point;
            packet.SetReadPos(packet.GetReadPos() + 5);

            //number of 'point' bytes will be the size of the data minus 2
            for ( unsigned short currentPoint = 0; currentPoint < dataSize - 4; currentPoint+=2 )
            {
                packet.Read(point);
                mPoints.push_back((point & 0x1FFF)); // Only first 12 bits used.
            }

            unsigned char laserStatus;
            packet.Read(laserStatus);

            return packet.Length();
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Runs the test case of the ContinuousOutputResponse message
///
///  \return 1 if the test case passes, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
int ContinuousOutputResponse::RunTestCase() const
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Assignment operator overload to copy to internal members
///
///  \param msg A ContinuousOutputResponse message to copy values from
///
///  \return A pointer to the instance of the ContinuousOutputResponse message
///
///////////////////////////////////////////////////////////////////////////////
ContinuousOutputResponse& ContinuousOutputResponse::operator=(const ContinuousOutputResponse& msg)
{
    if( this != &msg )
    {
        CopyMessageData(&msg);
        mPoints = msg.mPoints;
        mTimeStamp = msg.mTimeStamp;
    }

    return *this;
}


/*  End of File */
