////////////////////////////////////////////////////////////////////////////////////
///
///  \file NodeManagerApp.cpp
///  \brief Main entry point for program.
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
#ifndef __WX_NODE_MANAGER_APP_H__
#define __WX_NODE_MANAGER_APP_H__

#ifdef VLD_ENABLED
#include <vld.h>
#endif

#include <wx/wx.h>

#include "NodeManagerFrame.h"

class NodeManagerApp : public wxApp
{
public:
    NodeManagerApp();
    virtual ~NodeManagerApp();
    virtual bool OnInit();
};

DECLARE_APP(NodeManagerApp)

IMPLEMENT_APP(NodeManagerApp)

NodeManagerApp::NodeManagerApp()
{
}

NodeManagerApp::~NodeManagerApp()
{
}

bool NodeManagerApp::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler());
    wxImage::AddHandler(new wxJPEGHandler());
    wxImage::AddHandler(new wxICOHandler());
    
    NodeManagerFrame* frame = new NodeManagerFrame( (wxWindow*)NULL );
    
    if(this->argc > 1)
    {
        //frame->SetSettingsFile(wxString(argv[1]).ToAscii().data());
    }

    frame->Show();
    SetTopWindow( frame );

    return true;
}

#endif 
/* End of File */

