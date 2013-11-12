////////////////////////////////////////////////////////////////////////////////////
///
///  \file localposesensor.h
///  \brief This file contains the definition of the LocalPoseSensor class,
///         used as an interface for reporting Local Pose information.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 4 February 2010
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
#ifndef __JAUS_MOBILITY_LOCAL_POSE_SENSOR__H
#define __JAUS_MOBILITY_LOCAL_POSE_SENSOR__H

#include "jaus/mobility/sensors/globalposesensor.h"
#include "jaus/mobility/sensors/querylocalpose.h"
#include "jaus/mobility/sensors/reportlocalpose.h"
#include "jaus/mobility/sensors/setlocalpose.h"


namespace JAUS
{
    typedef ReportLocalPose LocalPose;

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class LocalPoseSensor 
    ///   \brief The Local Pose Sensor service reports global position and
    ///          orientation of a platform.
    ///
    ///   The function of the Local Pose Sensor is to report the local position
    ///   and orientation of the platform.  The Report Local Pose message provides
    ///   the position and orientation of the platform, relative to a local reference
    ///   frame.  The origin of the local reference frame may be altered using the Set
    ///   local Pose message, which sets the current position and orientation of the
    ///   platform to the specified values.  Platform orientation is defined in the
    ///   JAUS AS6009 document section 3.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL LocalPoseSensor : public AccessControl::Child,
                                              public Sensor
    {
    public:
        const static std::string Name;  ///< String name of the Service.
        // Constructor.
        LocalPoseSensor(const double updateRate = 10);
        // Destructor.
        virtual ~LocalPoseSensor();
        // Called when a command is received, or when sensor needs to update values.
        void SetLocalPose(const JAUS::SetLocalPose& localPose);
        // Called when when sensor needs to update values.
        void SetLocalPose(const JAUS::LocalPose& localPose);
        // Called when when sensor needs to update position, orientation, and time.
        bool SetLocalPose(const Point3D& position,
                          const Point3D& orientation,
                          const Time& time = Time(true));
        // If you have a global pose sensor, this is an easy way to set local pose data.
        bool SetLocalPose(const JAUS::GlobalPose& globalPose);
        // Use this method if you need to set a reference frame for calculating local pose (optional).
        void SetLocalPoseReference(const GlobalPose& globalPose);
        // Called when when sensor needs to update position and time.
        bool SetLocalPosition(const Point3D& position, const Time& time = Time(true));
        // Called when when sensor needs to update orientation and time.
        bool SetLocalOrientation(const Point3D& orientation, const Time& time = Time(true));
        // Called when when sensor needs to update position, orientation, and time.
        bool AddToLocalPose(const Point3D& position,
                            const Point3D& orientation,
                            const Time& time = Time(true));
        // Called when when sensor needs to update position and time.
        bool AddToLocalPosition(const Point3D& position, const Time& time = Time(true));
        // Called when when sensor needs to update orientation and time.
        bool AddtoLocalOrientation(const Point3D& orientation, const Time& time = Time(true));
        // Called when a command is received or software needs to set the value.
        bool SetSensorUpdateRate(const double rate);        
        // Gets the current global pose.
        LocalPose GetLocalPose() const;
        // Gets the reference point in the world coordinate frame (if set or looked up from Global Pose Sensor service).
        GlobalPose GetLocalPoseReference() const;
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
        // By default, the Local Pose Sensor is discoverable to other components (overload to hide).
        virtual bool IsDiscoverable() const { return true; }
        // Method called whenver a message is received.
        virtual void Receive(const Message* message);
        // Creates messages associated with this service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Prints status to console.
        virtual void PrintStatus() const;
    private:
        void CreateReportFromQuery(const QueryLocalPose* query, 
                                   ReportLocalPose& report) const;
        virtual void CheckServiceSynchronization(const unsigned int timeSinceLastCheckMs);
        double mMaxUpdateRate;                  ///<  Update rate of the sensor.
        ReportLocalPose mLocalPose;             ///<  Local pose of the sensor.
        GlobalPose mGlobalPoseReference;        ///<  Reference point for local pose calculations.
        CxUtils::Mutex mLocalPoseMutex;         ///<  Mutex for thread protection.
    };
}

#endif
/*  End of File */
