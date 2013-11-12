////////////////////////////////////////////////////////////////////////////////////
///
///  \file reporttravelspeed.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 25 November 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: bsun@ist.ucf.edu
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
#include "jaus/mobility/drivers/reporttravelspeed.h"
#include "jaus/core/scaledinteger.h"
#include <cxutils/math/cxmath.h>

const double JAUS::ReportTravelSpeed::Limits::MinSpeed = 0.0;
const double JAUS::ReportTravelSpeed::Limits::MaxSpeed = 327.67;

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
ReportTravelSpeed::ReportTravelSpeed(const Address& dest, const Address& src) : Message(REPORT_TRAVEL_SPEED, dest, src)
{
    mSpeed = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportTravelSpeed::ReportTravelSpeed(const ReportTravelSpeed& message) : Message(REPORT_TRAVEL_SPEED)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportTravelSpeed::~ReportTravelSpeed()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the speed and updates the presence vector for the message.
///
///   \param[in] value Desired speed in meters per second [0, 327.67].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportTravelSpeed::SetSpeed(const double value)
{
    if(value >= Limits::MinSpeed && value <= Limits::MaxSpeed)
    {
        mSpeed = value;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes message payload to the packet.
///
///   Message contents are written to the packet following the JAUS standard.
///
///   \param[out] packet Packet to write payload to.
///
///   \return -1 on error, otherwise number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int ReportTravelSpeed::WriteMessageBody(Packet& packet) const
{
    int expected = USHORT_SIZE;
    int written = 0;

    written += ScaledInteger::Write(packet, mSpeed, Limits::MaxSpeed, Limits::MinSpeed, ScaledInteger::UShort);

    return expected == written ? written : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads message payload from the packet.
///
///   Message contents are read from the packet following the JAUS standard.
///
///   \param[in] packet Packet containing message payload data to read.
///
///   \return -1 on error, otherwise number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int ReportTravelSpeed::ReadMessageBody(const Packet& packet)
{
    int expected = USHORT_SIZE;
    int read = 0;

    read += ScaledInteger::Read(packet, mSpeed, Limits::MaxSpeed, Limits::MinSpeed, ScaledInteger::UShort);

    return expected == read ? read : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void ReportTravelSpeed::ClearMessageBody()
{
    mSpeed = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints data to console.
///
////////////////////////////////////////////////////////////////////////////////////
void ReportTravelSpeed::PrintMessageBody() const
{
    std::cout << "Meter/Sec: [" << mSpeed << "]" << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a test case to validate the message class.
///
///   \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int ReportTravelSpeed::RunTestCase() const
{
    int result = 0;

    Packet packet;
    ReportTravelSpeed msg1, msg2;
    msg1.SetSpeed(30);

    if(msg1.WriteMessageBody(packet))
    {
        msg2.ClearMessage();
        if(msg2.ReadMessageBody(packet))
        {
            if(msg1.GetSpeed()- msg2.GetSpeed() <= 0.001)
            {
                result = 1;
            }
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
ReportTravelSpeed& ReportTravelSpeed::operator=(const ReportTravelSpeed& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mSpeed = message.mSpeed;
    }
    return *this;
}


/*  End of File */
