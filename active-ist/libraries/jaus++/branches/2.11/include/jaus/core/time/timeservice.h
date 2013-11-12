////////////////////////////////////////////////////////////////////////////////////
///
///  \file timeservice.h
///  \brief Contains the Time Service implementation.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 29 December 2009
///  <br>Copyright (c) 2009
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
#ifndef __JAUS_CORE_TIME_SERVICE__H
#define __JAUS_CORE_TIME_SERVICE__H

#include "jaus/core/sensor.h"
#include "jaus/core/date.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class TimeService
    ///   \brief This Time Service allows clients to query and set the system
    ///          time for the component.  Note that exclusive control is
    ///          required to set the time, but is not required to query it.
    ///
    ///   Depending your system or OS, the way the system time is changed may vary.
    ///   Therefore, it is recommend you set the function you want your Time Service
    ///   to use with the SetChangeTimeMethod access function.  Although there
    ///   is a method supplied to do this, it may not work depending on your 
    ///   permissions, etc. of your program.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL TimeService : public AccessControl::Child, public Sensor
    {
    public:
        const static std::string Name; ///< String name of the Service.
        // Constructor.
        TimeService();
        // Destructor.
        ~TimeService();
        // Sensors can always be discovered (overload if you don't want this).
        virtual bool IsDiscoverable() const { return true; }
        // Method called when an Event has been signaled, generates an Event message.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Method called to determine if an Event is supported by the service.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        // Processes messages associated with the TimeService Services.
        virtual void Receive(const Message* message);
        // Creates messages associated with the TimeService Service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Set System Time Method
        void SetChangeTimeMethod(bool (*changeTime)(const Time& time, const Date& date)) { mpChangeTimeMethod = changeTime; }
    protected:
        virtual void CheckServiceSynchronization(const unsigned int timeSinceLastCheckMs);
        bool (*mpChangeTimeMethod)(const Time& time, const Date& date);
    };
}

#endif
/*  End of File */
