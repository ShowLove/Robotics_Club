////////////////////////////////////////////////////////////////////////////////////
///
///  \file videoclientframe.h
///  \brief This file containts a wxWidget frame for displaying video data
///  received by a client using JAUS.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 3 February 2010
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
#ifndef __WX_JAUS_VIDEO_CLIENT_FRAME__H
#define __WX_JAUS_VIDEO_CLIENT_FRAME__H

#include <wx/wx.h>
#include <wx/timer.h>
#include "programs/imagepanel.h"
#include "jaus/core/component.h"
#include "jaus/extras/video/videosubscriber.h"
#include "jaus/extras/controllers/joystick.h"

#define TEXT_TYPE wxT

////////////////////////////////////////////////////////////////////////////////////
///
///  \class VideoClientFrame
///  \brief wxFrame for showing a video data received by a JAUS Component.
///
////////////////////////////////////////////////////////////////////////////////////
class VideoClientFrame : public wxFrame,
                         public JAUS::VideoSubscriber::Callback
{
public:
    VideoClientFrame(wxWindow* parent, 
                     wxWindowID id, 
                     const wxString& title, 
                     const wxPoint& pos, 
                     const wxSize& size, 
                     long style);
    ~VideoClientFrame();
    bool Initialize();
    void OnQuit(wxCommandEvent& guiEvent);
    void OnAbout(wxCommandEvent& guiEvent);
    void OnSelectVisualSensor(wxCommandEvent& guiEvent);
    void OnSelectPrimitiveDriver(wxCommandEvent& guiEvent);
    void OnSelectAuthorityLevel(wxCommandEvent& guiEvent);
    void OnTimer(wxTimerEvent& guiEvent);
    void OnDisconnect(wxCommandEvent& guiEvent);
    void OnRecord(wxCommandEvent& guiEvent);  
    virtual void ProcessCompressedVideo(const JAUS::Address& source,
                                        const JAUS::Byte camera,
                                        const JAUS::Image::Format format,
                                        const unsigned char* compressedImage,
                                        const unsigned int compressedImageSize,
                                        const unsigned int frameNumber);
    virtual void ProcessRawImage(const JAUS::Address& source,
                                 const JAUS::Byte camera,
                                 const JAUS::Image& image,
                                 const unsigned int frameNumber);
protected:
    void ResetVideoPanel();
private:
    DECLARE_EVENT_TABLE()
    wxMenu* mpFileMenu;         ///<  "File" category of the top menu
    wxMenu* mpHelpMenu;         ///<  "Help category of the top menu
    wxMenuBar* mpMenuBar;       ///<  Top menu bar.    
    wxImagePanel* mpImagePanel; ///<  Panel for viewing image.
    wxTimer* mpTimer;           ///<  Periodic timer used re-draw image data.
    unsigned int mFrameNumber;  ///<  Frame number.
    unsigned int mRecordFrameNumber;        ///<  Frame number of image saved to disk.
    JAUS::Time::Stamp mFrameUpdateTimeMs;   ///<  Timestamp of current frame number.
    volatile bool mRecordFlag;          ///<  If true, images are saved to disk.
    CxUtils::Mutex mMutex;              ///<  Mutex for thread protection.
    JAUS::Image mCurrentImage;          ///<  Curent image received from the sensor.
    JAUS::Address mVisualSensorID;      ///<  ID of the visual sensor we are subscribing to.
    JAUS::Byte mCameraNumber;           ///<  Camera number we are subscribing to.
    JAUS::Component mComponent;         ///<  JAUS Component.
    JAUS::VideoSubscriber* mpVideo;     ///<  JAUS Video Subscriber Service.
    JAUS::Joystick* mpJoystick;         ///<  JAUS Joystick Service.
};

#endif
/*  End of File */
