////////////////////////////////////////////////////////////////////////////////////
///
///  \file listdriver.h
///  \brief This file contains the implementation of the List Driver class for
///         creating services that execute lists.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 4 August 2010
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
#ifndef __JAUS_MOBILITY_LIST_DRIVER__H
#define __JAUS_MOBILITY_LIST_DRIVER__H

#include "jaus/mobility/list/listmanager.h"
#include "jaus/mobility/drivers/settravelspeed.h"
#include "jaus/mobility/drivers/querytravelspeed.h"
#include "jaus/mobility/drivers/reporttravelspeed.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ListDriver
    ///   \brief This class is used to create driving services that execute lists
    ///          of elements (e.g. LocalWaypointListDriver).
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL ListDriver : public ListManager::Child
    {
    public:
        ListDriver(const ID& serviceIdentifier,
                   const ID& parentServiceIdentifier);
        virtual ~ListDriver();
        // Method called when transitioning to a ready state.
        virtual bool Resume() { return true; }
        // Method called to transition due to reset.
        virtual bool Reset() { return true; }
        // Method called when transitioning to a standby state.
        virtual bool Standby() { return true; }
        // Method called to modify travel speed.
        virtual void SetExecutionSpeed(const double speedMetersPerSecond);
        // Method called to begin/continue execution of a list.
        virtual void ExecuteList(const double speedMetersPerSecond) = 0;
        // Method to check if we should be executing the list because we received a command.
        virtual bool IsExecuting() const;
        // Service is always discoverable.
        virtual bool IsDiscoverable() const { return true; }
        // Method called whenever a message is received.
        virtual void Receive(const Message* message);
        // Creates messages associated with this service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Gets the speed to execute at.
        inline double GetExecutionSpeed() const { Mutex::ScopedLock lock(&mListDriverMutex); return mSpeedMetersPerSecond; }
    protected:
        Mutex mListDriverMutex;                 ///<  For thread protection of data.
        double mSpeedMetersPerSecond;           ///<  Travel speed in m/s.
    };
}

#endif
/*  End of File */
