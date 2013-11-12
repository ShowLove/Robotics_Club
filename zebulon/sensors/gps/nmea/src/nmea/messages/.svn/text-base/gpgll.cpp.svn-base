/////////////////////////////////////////////////////////////////////////////
///
/// \file gpgll.cpp
/// \brief GPGLL NMEA message structure.
///
/// Author(s): Daniel Barber<br>
/// Created: 2/23/2008<br>
/// Last Modified: 2/23/2008<br>
/// Copyright (c) 2007<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu <br>
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
/////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "nmea/messages/gpgll.h"

using namespace Zebulon;
using namespace GPS;
using namespace NMEA;

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GPGLL::GPGLL() : Message(Message::GPGLL, "GPGLL")
{
    mIndicatorNS = 'N';
    mIndicatorEW = 'W';
    mDataActive = 'A';
    mLatitude = mLongitude = mUTC = 0.0;

    // Testing Values
    //mLatitude = 4807.038;
    //mLongitude = 8182.4356;
    //mUTC = 123519;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GPGLL::GPGLL(const GPGLL &arg) : Message(Message::GPGLL, "GPGLL")
{
    *this = arg;   
}


////////////////////////////////////////////////////////////////////////////////////
///
///  Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
GPGLL::~GPGLL()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Since this is a GP message, it saves WGS data to structure.
///
///  \param pos Wgs structure to populate with position data.
///
///  \return 1.
///
////////////////////////////////////////////////////////////////////////////////////
int GPGLL::GetPosition(CxUtils::Wgs& pos) const
{
    //Do the math to actually be lat and long
    
    pos.mLatitude = floor(mLatitude/100)+fmod(mLatitude,100)/60.0;
    if(mIndicatorNS == 'S' || mIndicatorNS == 's')
        pos.mLatitude *= -1;

    pos.mLongitude = floor(mLongitude/100)+fmod(mLongitude,100)/60.0;
    if(mIndicatorEW == 'w' || mIndicatorEW == 'W')
        pos.mLongitude *= -1;

    pos.mElevation = 0;
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Message has UTC data, so save it to parameter.
///
///  \param utc UTC info hhmmss.ss.
///
///  \return 1.
///
////////////////////////////////////////////////////////////////////////////////////
int GPGLL::GetUTC(double& utc) const
{
    utc = mUTC;
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Converts the data members to it's appropriate NMEA formatted string.
///
///  \param msg The packet structure to write the message to.
///
///  \return Number of bytes written, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int GPGLL::Write(CxUtils::Packet& msg) const
{
    char *ptr;
    int len = 0;
    msg.Clear();
    msg.Reserve(64);

    ptr = (char *)(msg.Ptr());
    
    if((len = sprintf(ptr, 
                      "$GPGLL,%.4lf,%c,%.4lf,%c,%.3lf,%c*",
                      mLatitude, 
                      mIndicatorNS, 
                      mLongitude, 
                      mIndicatorEW, 
                      mUTC, 
                      mDataActive)) <= 0)
    {
        //  Clear the string
        msg.Clear();
        return 0;
    }
    msg.SetLength(len);
    if(Message::WriteChecksum(msg))
    {
        return msg.Length();
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Reads the NMEA message if supported, overwriting internal data 
///  members.
///
///  \return Number of bytes read.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int GPGLL::Read(const CxUtils::Packet& msg)
{
    if(Message::IsValidMessage(msg))
    {
        if(sscanf((const char *)msg.Ptr(), 
                  "$GPGLL,%lf,%c,%lf,%c,%lf,%c*\n",
                  &mLatitude, 
                  &mIndicatorNS, 
                  &mLongitude, 
                  &mIndicatorEW, 
                  &mUTC, 
                  &mDataActive) > 0 )
        {
            return msg.Length();
        }
        else if(strstr((const char *)msg.Ptr(), "$GPGLL") > 0)
        {
            mIndicatorNS = 'N';
            mIndicatorEW = 'W';
            mDataActive = 'A';
            mLatitude = mLongitude = mUTC = 0.0;
            return msg.Length();
        }
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Runs a test case to validate the methods of the message class.
///
///  \return 1 if test case passed, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int GPGLL::RunTestCase()
{
    int result = 0;
    CxUtils::Packet packet;
    std::string header;
    std::string str = "$GPGLL,3723.2475,N,12158.3416,W,161229.487,A*2C";
    GPGLL msg1, msg2;

    Message::ExtractMessage((unsigned char *)str.c_str(), (unsigned int)str.length(), packet, &header);

    if(header == "GPGLL" && msg1.Read(packet))
    {
        if(msg1.Write(packet))
        {
            if(msg2.Read(packet))
            {
                if( fabs(msg1.mLatitude - msg2.mLatitude) < .0001 &&
                    fabs(msg1.mLongitude - msg2.mLongitude) < .0001 &&
                    fabs(msg1.mUTC - msg2.mUTC) < .0001 &&
                    msg1.mDataActive == msg2.mDataActive &&
                    msg1.mIndicatorEW == msg2.mIndicatorEW &&
                    msg1.mIndicatorNS == msg2.mIndicatorNS )
                {      
                    result = 1;
                }
            }
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Operator overloading for = operator.
///
////////////////////////////////////////////////////////////////////////////////////
GPGLL &GPGLL::operator=(const GPGLL &arg)
{
    this->CopyMessageData(&arg);    // Copies data from Message class.
    this->mDataActive = arg.mDataActive;
    this->mIndicatorNS = arg.mIndicatorNS;
    this->mIndicatorEW = arg.mIndicatorEW;
    this->mLatitude = arg.mLatitude;
    this->mLongitude = arg.mLongitude;
    this->mUTC = arg.mUTC;
    return *this;
}

/*  End of File */
