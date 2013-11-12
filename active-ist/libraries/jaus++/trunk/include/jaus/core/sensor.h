////////////////////////////////////////////////////////////////////////////////////
///
///  \file sensor.h
///  \brief Contains base class for deriving sensor services.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 24 April 2010
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
#ifndef __JAUS_CORE_SENSOR__H
#define __JAUS_CORE_SENSOR__H

#include "jaus/core/control/accesscontrol.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Sensor
    ///   \brief A Sensor is a type of service that primarily provides information
    ///          to other services, or synchronizes data with other services
    ///          of the same time.
    ///
    ///   For example, one component contains a 
    ///   Time service on it, that is the master Time service for that
    ///   subsystem.  Another component has a slave service which synchronizes
    ///   it's data with the master, so that other services on the same
    ///   component that need time data can access it directly from the
    ///   slave (useing a method call) service without having to create
    ///   subscriptions to that data themselves.
    ///
    ///   For examples of how to use this class see TimeService or GlobalPoseSensor
    ///   in the Mobility Library.
    ///
    ///   <b>A sensor can only synchronize to one other service, and you cannot
    ///      have more than one of the same sensor service on a single component.
    ///      also, once a service is synchronized to another component
    ///      it can not be discovered by other external components using
    ///      the Discovery services.</b>
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Sensor
    {
    public:
        Sensor() {}
        virtual ~Sensor() {}
        // Used to set the ID of the component to synchronize with.
        void SynchronizeToComponent(const Address& componentID) { Mutex::ScopedLock lock(&mSensorSyncMutex); mSyncComponentID = componentID; }
        bool IsSynchronizing() const { Mutex::ScopedLock lock(&mSensorSyncMutex); return mSyncComponentID.IsValid(); }
        Address GetSynchronizeID() const { Mutex::ScopedLock lock(&mSensorSyncMutex); return mSyncComponentID; }
        // Method called periodically by Component or other Services, can be used for periodic udpdates.
        virtual void CheckServiceSynchronization(const unsigned int timeSinceLastCheckMs) = 0;
    private:
        Mutex mSensorSyncMutex;         ///<  Mutex for synchronization settings.
        Address mSyncComponentID;       ///<  ID of the component to synchronize with.
    };
}

#endif
/*  End of File */
