////////////////////////////////////////////////////////////////////////////////////
///
///  \file globalposesensor.h
///  \brief This file contains the definition of the GlobalPoseSensor class,
///         used as an interface for reporting Global Pose information.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 4 February 2010
///  <br>Copyright (c) 2010
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
#ifndef __JAUS_MOBILITY_GLOBAL_POSE_SENSOR__H
#define __JAUS_MOBILITY_GLOBAL_POSE_SENSOR__H

#include "jaus/core/sensor.h"
#include "jaus/mobility/sensors/querygeomagneticproperty.h"
#include "jaus/mobility/sensors/queryglobalpose.h"
#include "jaus/mobility/sensors/reportglobalpose.h"
#include "jaus/mobility/sensors/reportgeomagneticproperty.h"
#include "jaus/mobility/sensors/setglobalpose.h"
#include "jaus/mobility/sensors/setgeomagneticproperty.h"

namespace JAUS
{
    typedef ReportGlobalPose GlobalPose;

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class GlobalPoseSensor 
    ///   \brief The Global Pose Sensor service reports global position and
    ///          orientation of a platform.
    ///
    ///   The function of the Global Pose Sensor is to report the global position
    ///   and orientation of the platform.  The Report Global Pose message provides
    ///   the position and orientation of the platform.  The position of the platform
    ///   is given in latitude, longitude, and elevation, in accordance with the WGS
    ///   84 standard.  Platform orientation is defined in the JAUS AS6009 document
    ///   section 3.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL GlobalPoseSensor : public Sensor, public AccessControl::Child
    {
    public:
        const static std::string Name;  ///< String name of the Service.
        // Constructor.
        GlobalPoseSensor(const double updateRate = 10);
        // Destructor.
        virtual ~GlobalPoseSensor();
        // Sensors can always be discovered (overload if you don't want this).
        virtual bool IsDiscoverable() const { return true; }
        // Called  when a command is received, or when sensor needs to update values.
        void SetGlobalPose(const JAUS::SetGlobalPose& globalPose);
        // Called  when when sensor needs to update values.
        void SetGlobalPose(const JAUS::GlobalPose& globalPose);
        // Called when when sensor needs to update position, orientation, and time.
        bool SetGlobalPose(const Wgs& position,
                           const Point3D& orientation,
                           const Time& time = Time(true));
        // Called when when sensor needs to update position and time.
        bool SetGlobalPosition(const Wgs& position, const Time& time = Time(true));
        // Called when when sensor needs to update orientation and time.
        bool SetGlobalOrientation(const Point3D& orientation, const Time& time = Time(true));
        // Called when a command is received or software needs to set the value.
        void SetGeomagneticVariation(const double magneticVariation);
        // Sets maximum sensor update rate.
        bool SetSensorUpdateRate(const double rate);
        // Gets the current global pose.
        GlobalPose GetGlobalPose() const;
        // Gets the current geomagnetic property.
        double GetGeomagneticVariation() const;
        // Gets the maximum sensor update rate.
        double GetSensorUpdateRate() const { return mMaxUpdateRate; }
        // Method called when an Event has been signaled, generates an Event message.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Method called to determine if an Event is supported by the service.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        // Method called whenver a message is received.
        virtual void Receive(const Message* message);
        // Creates messages associated with this service.
        virtual Message* CreateMessage(const UShort messageCode) const; 
        // Prints status to console.
        virtual void PrintStatus() const;
    private:
        void CreateReportFromQuery(const QueryGlobalPose* query, 
                                   GlobalPose& report) const;
        virtual void CheckServiceSynchronization(const unsigned int timeSinceLastCheckMs);
        double mMaxUpdateRate;                  ///<  Update rate of the sensor.
        double mGeomagneticProperty;            ///<  The magnetic variation for adjusting GPS heading.
        GlobalPose mGlobalPose;           ///<  Global pose of the sensor.
        CxUtils::Mutex mGlobalPoseMutex;        ///<  Mutex for thread protection.
    };
}

#endif
/*  End of File */
