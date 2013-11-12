////////////////////////////////////////////////////////////////////////////////////
///
///  \file rangesensor.h
///  \brief Contains the Range Sensor Service implementation.
///
///  <br>Author(s): Daniel Barber
///  Created: 10 March 2010
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
#ifndef __JAUS_EXTRAS_RANGE_SENSOR__H
#define __JAUS_EXTRAS_RANGE_SENSOR__H

#include "jaus/core/events/events.h"
#include "jaus/extras/rangesensor/reportlocalrangescan.h"
#include "jaus/extras/rangesensor/reportrangesensorconfiguration.h"
#include "jaus/extras/rangesensor/querylocalrangescan.h"
#include "jaus/extras/rangesensor/queryrangesensorconfiguration.h"
#include <string>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class RangeSensor
    ///   \brief The RangeSensor service allows you to share range scan information
    ///          from multiple devices with different capabilities.  Example
    ///          devices include LIDAR, or sonar, etc.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL RangeSensor : public Events::Child
    {
    public:
        static const std::string Name;   ///<  String name of the Service.
        // Constructor.
        RangeSensor();
        // Destructor.
        ~RangeSensor();
        // Add information about a range sensor devide.
        void AddRangeDevice(const RangeSensorConfig& configuration);
        // Set the current scan data from a device.
        void SetLocalRangeScan(const Byte deviceID,
                               const Point3D& deviceLocation,
                               const Point3D& deviceOrientation,
                               const std::vector<UShort>& scan,
                               const Time& timestamp = Time());
        // Set the current scan data from a device.
        void SetLocalRangeScan(const Byte deviceID,
                               const Point3D& deviceLocation,
                               const Point3D& deviceOrientation,
                               const std::vector<double>& scan,
                               const Time& timestamp = Time());
        // Set the current scan data from a device.
        void SetLocalRangeScan(const Byte deviceID,
                               const Point3D& deviceLocation,
                               const Point3D& deviceOrientation,
                               const Point3D::List& scan,
                               const Time& timestamp = Time());
        // Method called when an Event has been signaled, generates an Event message.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Method called to determine if an Event is supported by the service.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        // VisualSensor is always discoverable.
        virtual bool IsDiscoverable() const { return true; }
        // Processes messages associated with the VisualSensor Services.
        virtual void Receive(const Message* message);
        // Creates messages associated with the VisualSensor Service.
        virtual Message* CreateMessage(const UShort messageCode) const;
    protected:
        Mutex mRangeSensorMutex;                            ///<  Mutex for thread protection of data.
        RangeSensorConfig::Map mRangeSensors;               ///<  Range sensor information.
        std::map<Byte, ReportLocalRangeScan> mRangeScans;   ///<  Current range scan data for sensors.
    };
}

#endif

/*  End of File */
