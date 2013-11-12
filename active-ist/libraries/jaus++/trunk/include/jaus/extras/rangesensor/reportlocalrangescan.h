////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportlocalrangescan.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  Created: 8 March 2010
///  Copyright (c) 2010
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
#ifndef __JAUS_EXTRAS_RANGE_SENSOR_REPORT_RANGE_SENSOR_LOCAL_SCAN__H
#define __JAUS_EXTRAS_RANGE_SENSOR_REPORT_RANGE_SENSOR_LOCAL_SCAN__H

#include "jaus/extras/extrascodes.h"
#include "jaus/core/message.h"
#include "jaus/mobility/mobilitycodes.h"
#include <cxutils/circulararray.h>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ReportLocalRangeScan
    ///   \brief This message is used to report the a scan of relative object
    ///          position data from a range sensor. The order of the data
    ///          is based on the configuration of the sensor itself.
    ///
    ///   Scan data is in a 2D plane and relative to the <b>sensor</b>.  Sensor position
    ///   and orientation data is local to the vehicle coordinate frame and origin.
    ///
    ///   If a sensor has a configuration with a scan angle of 90 degrees, and an
    ///   angle increment of 1, then the first value of the scan vector will be
    ///   the range at angle -45, the next value at angle -44, etc.  Based on the
    ///   unit type (MM, CM), the range value will either be the distance at a
    ///   specified angle in mm or cm units relative to the sensor.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL ReportLocalRangeScan : public Message
    {
    public:
        typedef CxUtils::CircularArray<UShort> Scan;
        ReportLocalRangeScan(const Address& dest = Address(), 
                             const Address& src = Address()) : Message(REPORT_LOCAL_RANGE_SCAN, dest, src)
        {
            mSensorID = 0;
        }
        ReportLocalRangeScan(const ReportLocalRangeScan& message) : Message(REPORT_LOCAL_RANGE_SCAN)
        {
            *this = message;
        }
        ~ReportLocalRangeScan() {}  
        void SetSensorID(const Byte id) { mSensorID = id; }
        void SetSensorLocation(const Point3D& position) { mLocation = position; }
        void SetSensorOrientation(const Point3D& orientation) { mOrientation = orientation; }
        void SetTimeStamp(const Time& timestamp) { mTimeStamp = timestamp; }
        Byte GetSensorID() const { return mSensorID; } 
        Point3D GetSensorLocation() const { return mLocation; }
        Point3D GetSensorOrientation() const { return mOrientation; }
        Scan* GetScan() { return &mScan; }
        const Scan* GetScan() const { return &mScan; }
        Time GetTimeStamp() const { return mTimeStamp; }
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new ReportLocalRangeScan(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Report Local Range Scan"; }
        virtual void ClearMessageBody()
        {
            mSensorID = 0;
            mLocation = mOrientation = Point3D();
            mScan.clear(false);
        }
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const;
        ReportLocalRangeScan& operator=(const ReportLocalRangeScan& message)
        {
            CopyHeaderData(&message);
            mScan = message.mScan;
            mSensorID = message.mSensorID;
            mLocation = message.mLocation;
            mOrientation = message.mOrientation;
            return *this;
        }
    protected:
        Byte mSensorID;             ///<  Sensor ID.
        Point3D mLocation;          ///<  Location of the sensor in meters relative to platform origin.
        Point3D mOrientation;       ///<  Orientation of the sensor relative to platform orientation in radians.
        Scan mScan;                 ///<  Scan data (format depends on configuration of sensor).
        Time mTimeStamp;            ///<  Time when the data was captured.
    };
}

#endif
/*  End of File */
