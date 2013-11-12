////////////////////////////////////////////////////////////////////////////////////
///
///  \file DiscoveryPanel.h
///  \brief Panel for running a subsystem discovery component.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 02/01/2012
///  <br>Copyright (c) 2012
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
#ifndef __WX_DISCOVERY_PANEL__H
#define __WX_DISCOVERY_PANEL__H

#include "NodeManagerGUI.h"
#include "jaus/core/transport/nodemanager.h"
#include "jaus/core/component.h"

////////////////////////////////////////////////////////////////////////////////
///
///   \class NodeManagerFrame
///   \brief Min frame for the NodeManager GUI.
///
////////////////////////////////////////////////////////////////////////////////
class DiscoveryPanel : public DiscoveryPanelBase
{
public:
    /** Constructor */
    DiscoveryPanel( wxWindow* parent );
    void OnUpdateMainPanel( wxCommandEvent& event);
    virtual void OnShutdownButton( wxCommandEvent& event );
    void SetNodeManager(JAUS::NodeManager* ptr) { mpNodeManager = ptr; }
protected:
    // Virtual event handlers, overide them in your derived class
    virtual void OnInitializeButton( wxCommandEvent& event );
    virtual void OnSelectSubsystem( wxCommandEvent& event );
    virtual void OnUpdateUI( wxUpdateUIEvent& event );
    virtual void OnAuthorityLevel( wxCommandEvent& event );
    virtual void OnTakeControl( wxCommandEvent& event );
    virtual void OnSelectComponentChoice( wxCommandEvent& event );
    virtual void OnSendResume( wxCommandEvent& event );
    virtual void OnSendStandby( wxCommandEvent& event );
    virtual void OnSendShutdown( wxCommandEvent& event );

    void UpdateSubsystemTree(const bool force = false);
    JAUS::Component mComponent;         ///<  JAUS Component.
#ifdef _DEBUG
    JAUS::Component mDebugComponent;    ///<  Debug component for testing.
#endif
    JAUS::UShort mCurrentSelection;     ///<  Currently selected vehicle.
    JAUS::Subsystem::Map mSubsystems;   ///<  JAUS Subsystems.
    JAUS::Time mTreeUpdateTime;         ///<  Tree update time.
    JAUS::NodeManager* mpNodeManager;   ///<  Node Manager.
    JAUS::Address::List mControlList;   ///<  List of control options.
};

#endif // __WX_NODE_MANAGER_FRAME__H

