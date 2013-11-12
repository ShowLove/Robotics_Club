////////////////////////////////////////////////////////////////////////////////////
///
///  \file rangeclientframe.h
///  \brief This file containts a wxWidget frame for displaying range data
///  received by a client using JAUS.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 10 April 2010
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
#ifndef __WX_JAUS_RANGE_CLIENT_FRAME__H
#define __WX_JAUS_RANGE_CLIENT_FRAME__H

#include <wx/wx.h>
#include <wx/timer.h>
#include "programs/imagepanel.h"
#include "jaus/core/component.h"
#include "jaus/extras/rangesensor/rangesubscriber.h"
#include "jaus/extras/rangesensor/rangesensor.h"

#define TEXT_TYPE wxT

////////////////////////////////////////////////////////////////////////////////////
///
///  \class RangeClientFrame
///  \brief wxFrame for showing a video data received by a JAUS Component.
///
////////////////////////////////////////////////////////////////////////////////////
class RangeClientFrame : public wxFrame,
                         public JAUS::RangeSubscriber::Callback
{
public:
    RangeClientFrame(wxWindow* parent, 
                     wxWindowID id, 
                     const wxString& title, 
                     const wxPoint& pos, 
                     const wxSize& size, 
                     long style);
    ~RangeClientFrame();
    bool Initialize();
    void OnQuit(wxCommandEvent& guiEvent);
    void OnAbout(wxCommandEvent& guiEvent);
    void OnSelectRangeSensor(wxCommandEvent& guiEvent);
    void OnTimer(wxTimerEvent& guiEvent);
    void OnDisconnect(wxCommandEvent& guiEvent);
    void OnSize(wxSizeEvent &e);    
    void OnChangeScale(wxCommandEvent& guiEvent);
protected:
    virtual void ProcessLocalRangeScan(const JAUS::Point3D::List& scan,
                                       const JAUS::Address& sourceID, 
                                       const JAUS::Byte deviceID,
                                       const JAUS::Time& timestamp);
    void ResetPanel();
private:
    DECLARE_EVENT_TABLE()
    wxMenu* mpFileMenu;         ///<  "File" category of the top menu
    wxMenu* mpHelpMenu;         ///<  "Help category of the top menu
    wxMenuBar* mpMenuBar;       ///<  Top menu bar.    
    wxImagePanel* mpImagePanel; ///<  Panel for viewing image.
    wxTimer* mpTimer;           ///<  Periodic timer used re-draw image data.
    unsigned int mFrameNumber;  ///<  Frame number.
    JAUS::Time::Stamp mFrameUpdateTimeMs;   ///<  Timestamp of current frame number.
    CxUtils::Mutex mMutex;              ///<  Mutex for thread protection.
    JAUS::Address mRangeSensorID;       ///<  ID of the visual sensor we are subscribing to.
    JAUS::Byte    mRangeDeviceID;       ///<  Range sensor device ID.
    JAUS::Component mComponent;         ///<  JAUS Component.
    JAUS::RangeSubscriber* mpClient;    ///<  JAUS Service for range data.
    JAUS::Point3D::List mScan;          ///<  Current scan data.
    double mPixelsPerMeter;             ///<  Draw resolution.
};

#endif
/*  End of File */
