////////////////////////////////////////////////////////////////////////////////////
///
///  \file executelist.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 1 March 2010
///  <br>Copyright (c) 2010
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
#include "jaus/mobility/list/executelist.h"
#include "jaus/core/scaledinteger.h"

const double JAUS::ExecuteList::Limits::MinSpeed = 0.0;
const double JAUS::ExecuteList::Limits::MaxSpeed = 327.67;

using namespace JAUS;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
ExecuteList::ExecuteList(const Address& dest, const Address& src) : Message(EXECUTE_LIST, dest, src)
{
    mPresenceVector = 0;
    mSpeed = 0;
    mElementUID = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ExecuteList::ExecuteList(const ExecuteList& message) : Message(EXECUTE_LIST)
{
    mPresenceVector = 0;
    mSpeed = 0;
    mElementUID = 0;
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ExecuteList::~ExecuteList()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the desired speed.
///
///   \param[in] value Desired speed in meters per second [0, 327.67].
///
///   \return True on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ExecuteList::SetSpeed(const double value)
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
///   \brief Sets the desired UID of the element in the list to start with.
///
///   \param[in] uid Element ID to start execution with.  A value of 0 indicates
///                  the first element in the list.
///
///   \return True on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
UShort ExecuteList::SetElementUID(const UShort uid)
{
    mPresenceVector |= PresenceVector::ElementUID;
    return mElementUID = uid;
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
int ExecuteList::WriteMessageBody(Packet& packet) const
{
    int expected = BYTE_SIZE;
    int written = 0;

    written += packet.Write(mPresenceVector);

    expected += USHORT_SIZE;
    written += ScaledInteger::Write(packet, mSpeed, Limits::MaxSpeed, Limits::MinSpeed, ScaledInteger::UShort);
    if((mPresenceVector & PresenceVector::ElementUID) > 0)
    {
        expected += USHORT_SIZE;
        written += packet.Write(mElementUID);
    }

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
int ExecuteList::ReadMessageBody(const Packet& packet)
{
    int expected = BYTE_SIZE;
    int read = 0;

    read += packet.Read(mPresenceVector);
    
    expected+= USHORT_SIZE;
    read += ScaledInteger::Read(packet, mSpeed, Limits::MaxSpeed, Limits::MinSpeed, ScaledInteger::UShort);
    if((mPresenceVector & PresenceVector::ElementUID) > 0)
    {
        expected += USHORT_SIZE;
        read += packet.Read(mElementUID);
    }

    return expected == read ? read : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message contents.
///
////////////////////////////////////////////////////////////////////////////////////
void ExecuteList::ClearMessageBody()
{
    mPresenceVector = 0;
    mSpeed = 0;
    mElementUID = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a test case to validate the message class.
///
///   \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int ExecuteList::RunTestCase() const
{
    int result = 0;

    Packet packet;

    ExecuteList msg1, msg2;

    msg1.SetElementUID(54);
    msg1.SetSpeed(5.5);

    if((msg1.WriteMessageBody(packet) != -1) &&
        (msg2.ReadMessageBody(packet) != -1))
    {
        double diff = msg1.GetSpeed() - msg2.GetSpeed();
        if(msg2.AreFieldsPresent(PresenceVector::ElementUID) &&
            (msg1.GetElementUID() == msg2.GetElementUID()) &&
            (diff < 0.001))
         {
                result = 1;
         }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
ExecuteList& ExecuteList::operator=(const ExecuteList& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mPresenceVector = message.mPresenceVector;
        mSpeed = message.mSpeed;
        mElementUID = message.mElementUID;
    }
    return *this;
}


/*  End of File */
