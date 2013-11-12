////////////////////////////////////////////////////////////////////////////////////
///
///  \file tutorial_07.cpp
///  \brief This file is part of a set of tutorials for learning how to use
///         JAUS++.  This program demonstrates how to create your own
///         custom service that is used to subscribe to data from another
///         component.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 3 July 2010
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
#include <jaus/core/component.h>
#include <jaus/mobility/sensors/queryglobalpose.h>
#include <jaus/mobility/sensors/reportglobalpose.h>
#include <cxutils/keyboard.h>
#include <iostream>

// NOTE - Run another JAUS program like tutorial_06
// so that this program can talk to it.

////////////////////////////////////////////////////////////////////////////////////
///
///   \class SubscriberService
///   \brief This is an example service showing how to make subscriptions
///          to data you want.
///
///   Since we are using events to create subscriptions, we inherit from 
///   the Events service using the class JAUS::Events::Child, 
///   overloading any methods it requires.
///
////////////////////////////////////////////////////////////////////////////////////
class SubscriberService : public JAUS::Events::Child
{
public:
    SubscriberService() : JAUS::Events::Child(JAUS::Service::ID("SubscriberService"), // ID of my service.
                                              JAUS::Service::ID(JAUS::Events::Name))  // ID of the service 
                                                                                      // I inherit from 
                                                                                      // (i.e. Events).
    {
        // Sets the ID of the component with the
        // Global Pose Sensor we want to subscribe to.  For
        // simplicity, this is set to the ID of the component
        // in tutorial 6.
        mSensorComponent(1000, 1, 6);
    }
    ~SubscriberService()
    {
    } 
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief This method indicates if other services/components can discover
    ///   this service.  Since it doesn't do anything but get data, it is returning
    ///   false in this tutorial.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual bool IsDiscoverable() const { return false; }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief This method is used to create Message structures to read data
    ///          received by the Transport service (e.g. UDP, TCP, Serial).
    ///
    ///   This method must be overloaded by every Service, otherwise no Callbacks
    ///   or Receive method will be called to consume messages received.  This
    ///   method acts like a Message Factory for the Transport Service.
    ///
    ///   \param[in] messageCode The ID/Message Code of the message to create.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual JAUS::Message* CreateMessage(const JAUS::UShort messageCode) const
    {
        JAUS::Message* message = NULL;
        switch(messageCode)
        {
        case JAUS::REPORT_GLOBAL_POSE:
            message = new JAUS::ReportGlobalPose();
            break;
        case JAUS::QUERY_GLOBAL_POSE:
            message = new JAUS::QueryGlobalPose();
            break;
        default:
            message = NULL;
            break;
        }
        return message;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief This method is called whenever a message is received by the
    ///          Transport service.  It is up to you to add functionality to
    ///          process/consume/respond to the messages.
    ///
    ///   The main reason(s) this method would fail is  1) CreateMessage method
    ///   has not been overloaded, 2) Your service is not added to a component
    ///   that has been initialized, 3) It is a command message and the sender
    ///   is not in control of the component.
    ///
    ///   \param[in] message Pointer to a message to consume/respond to.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual void Receive(const JAUS::Message* message)
    {
        switch(message->GetMessageCode())
        {
        case JAUS::REPORT_GLOBAL_POSE:
            // Type cast the message and use the data
            {
                const JAUS::ReportGlobalPose* report = 
                            dynamic_cast<const JAUS::ReportGlobalPose*>(message);
                if(report)
                {
                    std::cout << "\nRecieved Report Global Pose Message:\n";
                    report->PrintMessageBody();
                }
            }
            break;
        case JAUS::QUERY_GLOBAL_POSE:
            {
                // Do nothing, this service doesn't generate
                // global pose, only subscribes to it.
            }
            break;
        default:
            // Do nothing, service is not meant to process
            break;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief This method tries to create subscriptions to a global pose data
    ///          on a component.  This is just an example of helper methods you
    ///          may want to add to your service.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    bool CreateSubscriptions()
    {
        // See if we have a subscription, and if not, create one.
        if(EventsService()->HaveSubscription(JAUS::REPORT_GLOBAL_POSE,
                                             mSensorComponent) == false)
        {
            // Setup the query for the type of report message we want to
            // get for our subscription.  In this case, we want to
            // get ReportGlobalPose, so we use a QueryGlobalPose
            JAUS::QueryGlobalPose query;
            // Set the PresenceVector mask to request any available
            // pose data.  Not all Query messages have a PresenceVector.
            // By using the GetPresenceVectorMask method of the query
            // message, we are requesting all/any available bits of data
            // for Pose that the component we are requesting from supports.
            query.SetPresenceVector(query.GetPresenceVectorMask());
            if(EventsService()->RequestEveryChangeEvent(mSensorComponent,
                                                        &query))
            {
                // Success!  We should not be getting
                // data through the Receive method or any other callbacks we
                // have setup.
                return true;
            }
        }

        return false;
    }
    // Method called when an Event has been signaled, generates an Event message.
    virtual bool GenerateEvent(const JAUS::Events::Subscription& info) const
    {
        // We are not supporting generation of events, so we can simply
        // return false.  For examples of how to create a sensor
        // service that supports generation of events, see the Liveness
        // service.
        return false;
    }
    // Method called to determine if an Event is supported by the service.
    virtual bool IsEventSupported(const JAUS::Events::Type type,
                                  const double requestedPeriodicRate,
                                  const JAUS::Message* queryMessage,
                                  double& confirmedPeriodicRate,
                                  std::string& errorMessage) const
    {
        // We are not supporting generation of events, so we can simply
        // return false.  For examples of how to create a sensor
        // service that supports generation of events, see the Liveness
        // service.
        return false;
    }
    JAUS::Address mSensorComponent; ///<  ID of the component to subscribe to.
};


int main(int argc, char* argv[])
{
    JAUS::Component component;

    // Add our own services to the component.
    // By default the Component will already have
    // the Core Service Set (e.g. Events, Discovery, AccessControl).

    // Services must be added before initialization of the component.
    SubscriberService* service = new SubscriberService();
    component.AddService(service);

    // Setup identification (types are Subsystem::Vehicle or Subsystem::OCU).
    // string name is whatever your platform/ocu identification is (e.g. XUV, AUV, OCU).
    component.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::OCU,
                                                             "OCU");

    // Initialize component.
    if(component.Initialize(JAUS::Address(1000, 1, 7)) == false)
    {
        return 0;
    }

    std::cout << "Component Initialized!\n";
    std::cout << "Creating subscription(s)...\n";
    while(service->CreateSubscriptions() == false)
    {
        if(CxUtils::GetChar() == 27)
        {
            return 0;
        }
        CxUtils::SleepMs(1);
    }

    while(CxUtils::GetChar() != 27)
    {
        // Do stuff...
        CxUtils::SleepMs(1);
    }

    // Shutdown component.  Services will be deleted automatically.
    component.Shutdown();

    return 0;
}


/* End of File */
