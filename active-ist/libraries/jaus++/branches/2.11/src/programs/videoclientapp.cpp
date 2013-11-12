////////////////////////////////////////////////////////////////////////////////////
///
///  \file videoclientapp.cpp
///  \brief WxWidgets program for JAUS++ Library Visual Sensor client
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 15 June 2008
///  <br>Last Modified: 15 June 2008
///  <br>Copyright (c) 2008
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)node
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
#include <wx/app.h>
#include <wx/cmdline.h>
#include <cxutils/mouse.h>

#ifdef WIN32
#ifndef WIN64
//#include <vld.h>
#endif
#endif

#include "programs/videoclientframe.h"

////////////////////////////////////////////////////////////////////////////////////
///
///  \class VideoClientApp
///  \brief Creates a JAUS++ Node Manager and its GUI.
///
////////////////////////////////////////////////////////////////////////////////////
class VideoClientApp : public wxApp
{
public:
    virtual bool OnInit();
};

DECLARE_APP(VideoClientApp)

IMPLEMENT_APP(VideoClientApp)


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Called when VideoClientApp is initialized.
///
///  Creates and Initializes a VideoClientFrame object.
///
////////////////////////////////////////////////////////////////////////////////////
bool VideoClientApp::OnInit()
{

    unsigned int width, height;
    CxUtils::Mouse::GetScreenResolution(width, height);
    VideoClientFrame* client = new VideoClientFrame(NULL,
                                                    wxID_ANY,
                                                    wxT("JAUS++ Video Client"),
                                                    wxPoint(width/2 - 320, height/2 - 240),
                                                    wxSize(460, 450),
                                                    wxDEFAULT_FRAME_STYLE);
    
    client->Show();  
    if(client->Initialize())
    {         
        return true;
    }
    return false;
}

/*  End of File */
