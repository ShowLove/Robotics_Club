////////////////////////////////////////////////////////////////////////////////////
///
///  \file primitivedriver.h
///  \brief This file contains the definition of the PrimitiveDriver class, used
///         as an interface for basic platform mobility.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 12 January 2010
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
#ifndef __JAUS_MOBILITY_PRIMITIVE_DRIVER__H
#define __JAUS_MOBILITY_PRIMITIVE_DRIVER__H

#include "jaus/core/management/management.h"
#include "jaus/mobility/drivers/querywrencheffort.h"
#include "jaus/mobility/drivers/reportwrencheffort.h"
#include "jaus/mobility/drivers/setwrencheffort.h"

namespace JAUS
{
    typedef ReportWrenchEffort WrenchEffort;

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class PrimitiveDriver
    ///   \brief The Primitive Driver allows for basic platform mobility.
    ///
    //    This service does not imply any particular platform type such as tracked or
    ///   wheeled, but describes mobility in six degrees of freedom using a percent of
    ///   availible effort in each of the directions.  Additionally, no power plant
    ///   (gasoline, diesel, or battery) is implied and the service functions strictly
    ///   in an open loop manner, i.e., a velocity is not commanded since that requires
    ///   a speed sensor.  Note that the specific actuator commands are not defined by
    ///   JAUS.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL PrimitiveDriver : public Management::Child
    {
    public:
        const static std::string Name; ///< String name of the Service.
        PrimitiveDriver();
        virtual ~PrimitiveDriver();
        // Method called when Resume command received, overload for additional behavior.
        virtual bool Resume() { return true; }
        // Method called when Reset command received, overload for additional behavior.
        virtual bool Reset()
        { 
            Mutex::ScopedLock lock(&mPrimitiveDriverMutex); 
            mCurrentWrenchEffort.ClearMessage(); 
            mWrenchEffortTime.Clear();
            return true; 
        }
        // Method called when Standby command received, overload for additional behavior.
        virtual bool Standby() { return Reset(); }
        // Method called when Set Emergency command received, overload for additional behavior.
        virtual bool SetEmergency() { return Reset(); }
        // Method called when Clear Emergency command received, overload for additional behavior.
        virtual bool ClearEmergency() { return true; }
        // Method called when Release Control command received, overload for additional behavior.
        virtual bool ReleaseControl() { return Reset(); }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Method called whenever a SetWrenchEffort command is received.  
        ///
        ///   Overload this method to be notified when a new command is received. Wrench
        ///   commands are saved internally regardless and can be accessed using the
        ///   GetCurrentWrenchEffort method.
        ///
        ///   \param[in] command Drive command message.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual bool SetWrenchEffort(const JAUS::SetWrenchEffort* command) {  return true; };
        // Gets the current wrench effort command.
        virtual JAUS::SetWrenchEffort GetCurrentWrenchEffort() const;
        // Gets the time when the last wrench effort command was received.
        virtual Time GetWrenchEffortTime() const;
        // Generates Primitive Driver related events.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Adds support for Report Wrench Effort events.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        // By default, the Primitive Driver is discoverable to other components (overload to hide).
        virtual bool IsDiscoverable() const { return true; }
        // Method called whenver a message is received.
        virtual void Receive(const Message* message);
        // Creates messages associated with this service.
        virtual Message* CreateMessage(const UShort messageCode) const;  
        // Prints information about the service.
        virtual void PrintStatus() const;
    private:
        // Creates a ReportWrenchEffort from QueryWrenchEffort.
        void CreateReportFromQuery(const QueryWrenchEffort* query, 
                                   ReportWrenchEffort& report) const;
        Mutex mPrimitiveDriverMutex;                ///<  Mutex for thread protection of data.
        Time mWrenchEffortTime;                     ///<  Time when the last wrench effort was received.
        JAUS::SetWrenchEffort mCurrentWrenchEffort; ///<  The last wrench effort received.
    };
}

#endif
/*  End of File */
