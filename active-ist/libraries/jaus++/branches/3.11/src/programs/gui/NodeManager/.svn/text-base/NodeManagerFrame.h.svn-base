////////////////////////////////////////////////////////////////////////////////////
///
///  \file NodeManagerFrame.h
///  \brief Main file for the NodeManagerFrame code.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 12/2/2011
///  <br>Copyright (c) 2011
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
#ifndef __WX_NODE_MANAGER_FRAME__H
#define __WX_NODE_MANAGER_FRAME__H

#include <cxutils/timer.h>
#include "NodeManagerGUI.h"
#include "jaus/core/transport/nodemanager.h"

////////////////////////////////////////////////////////////////////////////////
///
///   \class NodeManagerFrame
///   \brief Min frame for the NodeManager GUI.
///
////////////////////////////////////////////////////////////////////////////////
class NodeManagerFrame : public NodeManagerFrameBase,
                         public CxUtils::Timer::Callback
{
public:
    /** Constructor */
    NodeManagerFrame( wxWindow* parent );
protected:
    // Handlers for NodeManagerFrameBase events.
    void OnUpdateUI( wxUpdateUIEvent& event );
    void OnExit( wxCommandEvent& event );
    virtual void OnUpdateInterval( wxCommandEvent& event );
    void OnInitialize( wxCommandEvent& event );
    void OnShutdown( wxCommandEvent& event );
    void OnLoadSettings( wxCommandEvent& event );
    void OnUpdateMainPanel( wxCommandEvent& event);
    virtual void ProcessTimerEvent();
    JAUS::NodeManager mNodeManager; ///<  Node Manager
    CxUtils::Timer mTimer;          ///<  Used to trigger GUI update.
    wxPanel* mpDiscoveryPanel;      ///<  Discovery panel.
};

#endif // __WX_NODE_MANAGER_FRAME__H

