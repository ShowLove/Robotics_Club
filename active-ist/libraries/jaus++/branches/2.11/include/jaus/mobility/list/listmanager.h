////////////////////////////////////////////////////////////////////////////////////
///
///  \file listmanager.h
///  \brief This file contains the implementation of the List Manager service.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 27 February 2010
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
#ifndef __JAUS_MOBILITY_LIST_MANAGER__H
#define __JAUS_MOBILITY_LIST_MANAGER__H

#include "jaus/core/management/management.h"
#include "jaus/mobility/list/element.h"
#include "jaus/mobility/list/setelement.h"
#include "jaus/mobility/list/confirmelementrequest.h"
#include "jaus/mobility/list/deleteelement.h"
#include "jaus/mobility/list/executelist.h"
#include "jaus/mobility/list/queryactiveelement.h"
#include "jaus/mobility/list/queryelement.h"
#include "jaus/mobility/list/queryelementcount.h"
#include "jaus/mobility/list/queryelementlist.h"
#include "jaus/mobility/list/rejectelementrequest.h"
#include "jaus/mobility/list/reportactiveelement.h"
#include "jaus/mobility/list/reportelement.h"
#include "jaus/mobility/list/reportelementcount.h"
#include "jaus/mobility/list/reportelementlist.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ListManager
    ///   \brief The List Manager Service permits operations on a single ordered
    ///          sequence of connected elements. 
    ///
    ///   This service supports add, replace, or delete elements from a list, as 
    ///   well as query and report optoins.  Lists are stored similarly to a
    ///   double linked list.
    ///
    ///   1) A list must contain exactly one head element which is defined as having
    ///      a previous (parent) identifier of zero (0).
    ///   2) For non-circular lists, the list must contain exactly one tail element
    ///      which is defined as having a next (child) identifier of (0).
    ///   3) Each element must reference existing (previous) parent and next (child)
    ///      elements, or zero.
    ///   4) Elements cannot be orphaned.  An orphan is defined as an element that is
    ///      not connected in any way to the other elements in the list.
    ///   5) The prevous (parent) and next(child) reference for each element cannot 
    ///      point to itself.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL ListManager : public Management::Child
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Child
        ///   \brief This class is used to create services that inherit from the
        ///          List Manager service.  
        ///
        ///   This service class maintains the list to be executed, and has methods
        ///   to advance to the next element and get the current list element so
        ///   that your service does not need to maintain it itself.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_MOBILITY_DLL Child : public Management::Child
        {
            friend class ListManager;
        public:            
            Child(const ID& serviceIdentifier,
                  const ID& parentServiceIdentifier);
            virtual ~Child();
            // Method called when transitioning to a ready state.
            virtual bool Resume() = 0;
            // Method called to transition due to reset.
            virtual bool Reset() = 0;
            // Method called when transitioning to a standby state.
            virtual bool Standby() = 0;            
            // Method called to check if an element type (message payload) is supported.
            virtual bool IsElementSupported(const Message* message) const = 0;
            // Sets the current active list element.
            void SetActiveListElement(const UShort uid);
            // Deletes an element from the list.
            bool DeleteListElement(const UShort id, Byte& rejectReason);
            // Inserts an element into the list.
            bool SetElements(const Element::List& elements);
            // Get the current active element in the list.
            Element GetActiveListElement() const;
            // Method to get a specific element.
            Element GetElement(const UShort id) const;
            // Gets the current active element (0 if not initialized yet or list is complete).
            UShort GetActiveListElementID() const { return mActiveElement; }
            // Advances the active element in the list (called when active element is completed).
            void AdvanceListElement();
            // Method to get a copy of the element list.
            Element::Map GetElementList() const;
            // Gets a copy of the list IDs.
            void GetElementList(std::vector<UShort>& list) const;
            // Get the size of the element list.
            unsigned int GetElementCount() const;
        private:
            Mutex mListMutex;          ///<  Mutex for thread protection of list.
            Element::Map mElementList; ///<  List of elements to execute.
            UShort mActiveElement;     ///<  The active element in the list.
        };
        const static std::string Name; ///<  String name of the Service.
        ListManager();
        ~ListManager();
        virtual bool Resume() { return true; }
        virtual bool Reset();
        virtual bool Standby() { return true; }
        virtual bool IsDiscoverable() const { return true; }
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        virtual void Receive(const Message* message);
        virtual Message* CreateMessage(const UShort messageCode) const;
    };
}

#endif
/*  End of File */
