/////////////////////////////////////////////////////////////////////////////
///
/// \file gpgga.cpp
/// \brief GPGGA NMEA message structure.
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
#include "nmea/messages/gpgga.h"

using namespace Zebulon;
using namespace GPS;
using namespace NMEA;

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GPGGA::GPGGA() : Message(Message::GPGGA, "GPGGA")
{
    mQuality = mNumSatellites = mBaseId = 0;
    mAltUnits = mGeoidalUnits = mIndicatorNS = mIndicatorEW = '0';
    mLatitude = mLongitude = mUTC = mAltitude = mAgeDGPS = mGeoidalSeparation = mHDOP = 0.0;

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
GPGGA::GPGGA(const GPGGA &arg) : Message(Message::GPGGA, "GPGGA")
{
    *this = arg;   
}


////////////////////////////////////////////////////////////////////////////////////
///
///  Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
GPGGA::~GPGGA()
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
int GPGGA::GetPosition(CxUtils::Wgs& pos) const
{
    //Do the math to actually be lat and long
    
    pos.mLatitude = floor(mLatitude/100.0)+fmod(mLatitude,100.0)/60.0;
    if(mIndicatorNS == 'S' || mIndicatorNS == 's')
        pos.mLatitude *= -1;

    pos.mLongitude = floor(mLongitude/100.0)+fmod(mLongitude,100.0)/60.0;
    if(mIndicatorEW == 'w' || mIndicatorEW == 'W')
        pos.mLongitude *= -1;
    pos.mElevation = mAltitude;
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
int GPGGA::GetUTC(double& utc) const
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
unsigned int GPGGA::Write(CxUtils::Packet& msg) const
{
    char *ptr;
    int len = 0;
    msg.Clear();
    msg.Reserve(128);

    ptr = (char *)(msg.Ptr());
    
    if((len = sprintf(ptr, 
                      "$GPGGA,%.2lf,%.2lf,%c,%.2lf,%c,%d,%d,%.1lf,%.1lf,%c,%.1lf,%c,%.1lf,%04d*",
                      mUTC, 
                      mLatitude, 
                      mIndicatorNS, 
                      mLongitude, 
                      mIndicatorEW, 
                      mQuality,
                      mNumSatellites, 
                      mHDOP, 
                      mAltitude, 
                      mAltUnits, 
                      mGeoidalSeparation, 
                      mGeoidalUnits,
                      mAgeDGPS, 
                      mBaseId)) <= 0)
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
unsigned int GPGGA::Read(const CxUtils::Packet& msg)
{
    if(Message::IsValidMessage(msg))
    {
        int result = 0;
        
        mQuality = mNumSatellites = mBaseId = 0;
        mAltUnits = mGeoidalUnits = mIndicatorNS = mIndicatorEW = '0';
        mLatitude = mLongitude = mUTC = mAltitude = mAgeDGPS = mGeoidalSeparation = mHDOP = 0.0;

        result = sscanf((char *)msg.Ptr(), 
                        "$GPGGA,%lf,%lf,%c,%lf,%c,%d,%d,%lf,%lf,%c,%lf,%c,%lf,%d*",
                        &mUTC, &mLatitude, &mIndicatorNS, &mLongitude, &mIndicatorEW, &mQuality,
                        &mNumSatellites, &mHDOP, &mAltitude, &mAltUnits, &mGeoidalSeparation, &mGeoidalUnits,
                        &mAgeDGPS, &mBaseId);
        if(result > 0)
        {
            return msg.Length();
        }
        // If <= 0, then we have a valid message, however we
        // don't have data from sattelites yet.
        else if(strstr((const char *)msg.Ptr(), "$GPGGA") > 0)
        {
            mQuality = mNumSatellites = mBaseId = 0;
            mAltUnits = mGeoidalUnits = mIndicatorNS = mIndicatorEW = '0';
            mLatitude = mLongitude = mUTC = mAltitude = mAgeDGPS = mGeoidalSeparation = mHDOP = 0.0;
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
int GPGGA::RunTestCase()
{
    int result = 0;
    CxUtils::Packet packet;
    std::string str = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
    GPGGA msg1, msg2;

    Message::ExtractMessage((unsigned char *)str.c_str(), (unsigned int)str.length(), packet);

    if(msg1.Read(packet))
    {
        if(msg1.Write(packet))
        {
            if(msg2.Read(packet))
            {
                result = 1;
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
GPGGA &GPGGA::operator=(const GPGGA &arg)
{
    this->CopyMessageData(&arg);    // Copies data from Message class.
    this->mIndicatorNS = arg.mIndicatorNS;
    this->mIndicatorEW = arg.mIndicatorEW;
    this->mLatitude = arg.mLatitude;
    this->mLongitude = arg.mLongitude;
    this->mUTC = arg.mUTC;
    this->mQuality = arg.mQuality;
    this->mNumSatellites = arg.mNumSatellites;
    this->mBaseId = arg.mBaseId;
    this->mAltUnits = arg.mAltUnits;
    this->mGeoidalUnits = arg.mGeoidalUnits;
    this->mAltitude = arg.mAltitude;
    this->mAgeDGPS = arg.mAgeDGPS;
    this->mGeoidalSeparation = arg.mGeoidalSeparation;
    this->mHDOP = arg.mHDOP;

    return *this;
}

/*  End of File */
