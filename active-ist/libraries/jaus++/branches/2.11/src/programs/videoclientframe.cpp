////////////////////////////////////////////////////////////////////////////////////
///
///  \file videoclientframe.cpp
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
#include "programs/videoclientframe.h"
#include "jaus/extras/video/visualsensor.h"
#include "jaus/mobility/drivers/primitivedriver.h"
#include <cxutils/fileio.h>
#include <wx/image.h>
#include <wx/aboutdlg.h>
#include <wx/numdlg.h>
#include <wx/choicdlg.h>
#include <wx/arrstr.h>
#include <wx/sizer.h>
#include <cxutils/fileio.h>
#include <cctype>
#include <algorithm>

int gTimerDelayMs = 20;

// ID numbers for GUI related events.
#define TIMER_ID      1000
#define DISCONNECT_ID 1001
#define RECORD_ID     1002
#define DRIVE_ID      1003
#define AUTHORITY_ID  1004

// specialized definese used in wxwidgets programming
BEGIN_EVENT_TABLE(VideoClientFrame, wxFrame)
    EVT_MENU(wxID_ABOUT, VideoClientFrame::OnAbout)
    EVT_MENU(wxID_NEW, VideoClientFrame::OnSelectVisualSensor)
    EVT_MENU(wxID_EXIT, VideoClientFrame::OnQuit)
    EVT_MENU(DISCONNECT_ID, VideoClientFrame::OnDisconnect)
    EVT_MENU(RECORD_ID, VideoClientFrame::OnRecord)
    EVT_MENU(DRIVE_ID, VideoClientFrame::OnSelectPrimitiveDriver)
    EVT_MENU(AUTHORITY_ID, VideoClientFrame::OnSelectAuthorityLevel)
    EVT_TIMER(TIMER_ID, VideoClientFrame::OnTimer)
    EVT_SIZE(VideoClientFrame::OnSize)
END_EVENT_TABLE()


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the window.
///
///////////////////////// ///////////////////////////////////////////////////////////
VideoClientFrame::VideoClientFrame(wxWindow* parent,
                                   wxWindowID id,
                                   const wxString& title,
                                   const wxPoint& pos,
                                   const wxSize& size,
                                   long style) : wxFrame(parent, 
                                                         id,
                                                         title,
                                                         pos,
                                                         size,
                                                         style)
{
    // Widgets.
    mpFileMenu = NULL;
    mpHelpMenu = NULL;
    mpMenuBar = NULL;
    mpImagePanel = NULL;

    //Timer
    mpTimer = NULL;

    // Video related data members.
    mFrameNumber = 0;
    mRecordFrameNumber = 0;
    mFrameUpdateTimeMs = 0;
    mRecordFlag = false;
    mCameraNumber = 0;
    mpVideo = NULL;
    mpJoystick = NULL;

    // Create the video panel, and attach to the main panel.
    mpImagePanel = new wxImagePanel(this);

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(mpImagePanel, 1, wxEXPAND);
    SetSizer(sizer);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
VideoClientFrame::~VideoClientFrame()
{
    // Shutdown JAUS services
    mComponent.Shutdown();
    // Kill the timer.
    if(mpTimer)
    {
        delete mpTimer;
        mpTimer = NULL;
    }
    // Close the GUI.
    Close(true);    
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the component interface for getting video data.
///
////////////////////////////////////////////////////////////////////////////////////
bool VideoClientFrame::Initialize()
{
    // Initialize the file menu.
    mpFileMenu = new wxMenu();
    mpFileMenu->Append(wxID_NEW, wxT("Select &Sensor\tAlt-S"), wxT("Select Visual Sensor"));
    mpFileMenu->Append(DISCONNECT_ID, wxT("&Disconnect\tAtl-D"), wxT("Disconnect from Sensor"));
    mpFileMenu->Append(RECORD_ID, wxT("&Record\tAtl-R"), wxT("Record from Sensor"));
    mpFileMenu->Append(DRIVE_ID, wxT("Enable/Disable &Joystick\tAtl-J"), wxT("Enable/Disable Joystick Control"));
    mpFileMenu->Append(AUTHORITY_ID, wxT("Authority &Level\tAtl-L"), wxT("Change Authority Level"));   
    mpFileMenu->Append(wxID_ABOUT, wxT("&About Client\tAtl-A"), wxT("About Video Client"));    
    mpFileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"), wxT("Exit Program"));
    
    // Create a menu bar and attach the file menu.
    mpMenuBar = new wxMenuBar();
    mpMenuBar->Append(mpFileMenu, wxT("&File"));
    SetMenuBar(mpMenuBar);

    // Load the default image, and have the image panel show it.
    ResetVideoPanel();

    // Create the status bar.
    CreateStatusBar(2);

    // Add an icon/logo to the GUI.
    wxImage logo;
    if(logo.LoadFile(wxString("icons/camera-photo.png", wxConvUTF8)))
    {
        wxIcon icon;
        icon.CopyFromBitmap(logo);
        SetIcon(icon);
    }

    // Create the timer to draw to the GUI new image data.
    mpTimer = new wxTimer(this, TIMER_ID);

    // Initialize joystick service.
    mpJoystick = new JAUS::Joystick();
    mpJoystick->EnableJoystick(false);
    mComponent.AddService(mpJoystick);

    // Create a Video Subscriber Service, and add it to the
    // Component for use initialization.
    mpVideo = new JAUS::VideoSubscriber();
    mComponent.AddService(mpVideo);

    // Register a callback with the video service, to get video
    // data as it arrives.
    mpVideo->RegisterCallback(this, true);  // RAW image data.
    mpVideo->RegisterCallback(this, false); // Compressed data.

    // Load settings for the components services.
    mComponent.LoadSettings("settings/services.xml");
    // Set our identification as an OCU type component.
    mComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::OCU,
                                                              "Video Subscriber");
    mComponent.DiscoveryService()->SetDiscoveryFrequency(0.25);
    
    mpTimer->Start(gTimerDelayMs);

    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Method called when a Close guiEvent is generated.
///
////////////////////////////////////////////////////////////////////////////////////
void VideoClientFrame::OnQuit(wxCommandEvent& guiEvent)
{
    // Shutdown JAUS services
    mComponent.Shutdown();
    // Kill the timer.
    if(mpTimer)
    {
        delete mpTimer;
        mpTimer = NULL;
    }
    // Close the GUI.
    Close(true);   
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Function called when about is clicked.  This will display a dialog
///         window describing the application.
///
///  \param[in] guiEvent Required wxCommandEvent parameter
///
////////////////////////////////////////////////////////////////////////////////////
void VideoClientFrame::OnAbout(wxCommandEvent& guiEvent)
{
    wxAboutDialogInfo info;
    wxString text;
    info.SetName(TEXT_TYPE("JAUS++ Video Client"));
    info.SetVersion(TEXT_TYPE("2.0"));

    text.Append(TEXT_TYPE("This program connects to a\nJAUS++ Visual Sensor and\n"));
    text.Append(TEXT_TYPE("acquires image data using Events."));
    info.SetDescription(text);
    text.Clear();

    info.AddDeveloper(TEXT_TYPE("Daniel Barber"));

    text.Append(TEXT_TYPE("Copyright (c) 2010\n"));
    text.Append(TEXT_TYPE("Univeristy of Central Florida\n"));
    text.Append(TEXT_TYPE("Institute for Simulation and Training (IST)\n"));
    text.Append(TEXT_TYPE("Applied Cognition and Training in Immersive Virtual Environments Laboratory\n"));

    info.SetCopyright(text);
    text.Clear();

    info.SetWebSite(TEXT_TYPE("http://active.ist.ucf.edu"));
    
    wxImage logo;
    if(logo.LoadFile(wxString("logo/jaus++_logo_100x100.gif", wxConvUTF8)))
    {
        wxIcon icon;
        icon.CopyFromBitmap(logo);
        info.SetIcon(icon);
    }
    wxAboutBox(info);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief When the select visual sensor menu option is selected this method
///         asks the user to select from available Visual Sensor services
///         discovered.
///
///  \param[in] guiEvent Required wxCommandEvent parameter
///
////////////////////////////////////////////////////////////////////////////////////
void VideoClientFrame::OnSelectVisualSensor(wxCommandEvent& guiEvent)
{
    wxArrayString choices;
    // Lookup all the components supporting the Visual Sensor Service
    // and display the choices to the user.

    JAUS::Address::List components = mComponent.DiscoveryService()->GetComponentsWithService(JAUS::VisualSensor::Name);
    JAUS::Address::List::iterator c;
    for(c = components.begin();
        c != components.end();
        c++)
    {
        choices.Add(wxString(mComponent.DiscoveryService()->GetSubsystem(c->mSubsystem)->mIdentification.c_str(), wxConvUTF8) + 
                    wxString(" - ", wxConvUTF8) + 
                    wxString(c->ToString().c_str(), wxConvUTF8));
    }

    if(choices.size() == 0)
    {
        JAUS::QueryConfiguration queryConfiguration(JAUS::Address(JAUS::Address::GlobalBroadcast, 255, 255), mComponent.GetComponentID());
        queryConfiguration.SetQueryType(JAUS::QueryConfiguration::SubsystemConfiguration);
        mComponent.Send(&queryConfiguration);
        CxUtils::SleepMs(1000);
        components = mComponent.DiscoveryService()->GetComponentsWithService(JAUS::VisualSensor::Name);
        for(c = components.begin();
            c != components.end();
            c++)
        {
            choices.Add(wxString(mComponent.DiscoveryService()->GetSubsystem(c->mSubsystem)->mIdentification.c_str(), wxConvUTF8) + 
                        wxString(" - ", wxConvUTF8) + 
                        wxString(c->ToString().c_str(), wxConvUTF8));
        }
        if(choices.size() == 0)
        {
            wxMessageBox(wxT("No Components with Visual Sensor Service Discovered"));
            return;
        }
    }

    wxSingleChoiceDialog componentChoice(this,
                                         wxT("Please Select a Component"),
                                         wxT("Visual Sensor Component Selection"),
                                         choices);
    // If the user makes a selection.
    if(componentChoice.ShowModal() == wxID_OK)
    {
        JAUS::Address id;
        int n = componentChoice.GetSelection();
        unsigned int count = 1;
        id = components[n];
        JAUS::Byte cameraID = 0;

        // Find out how many cameras the service is
        // supporting, and if more than one, have the
        // user make a choice between them.
        std::vector<JAUS::Byte> cameraList;
        if(mpVideo->GetCameraList(id, cameraList, 3000))
        {
            choices.Clear();
            // Add choices to list.
            for(unsigned int i = 0; i < (unsigned int)cameraList.size(); i++)
            {
                char str[256];
                sprintf(str, "Camera %3d", cameraList[i]);
                choices.Add(wxString(str, wxConvUTF8));
            }
            wxSingleChoiceDialog cameraChoice(this,
                                              wxT("Please Select a Camera"),
                                              wxT("Camera Selection"),
                                              choices);
            // Have the user choose, or select default
            // camera number/ID.
            if(choices.size() == 1 || cameraChoice.ShowModal() == wxID_OK)
            {
                
                if(choices.size() > 1)
                {
                    cameraID = (JAUS::Byte)cameraList[cameraChoice.GetSelection()];
                }
                
            }
        }
        /*
        else
        {
            wxMessageBox(wxT("Could not Query Number of Cameras"));
        }
        */

        // Cancel all previous connections.
        mpTimer->Stop();
        
        mpVideo->CancelVideoSubscription(mVisualSensorID, mCameraNumber);
        mVisualSensorID.Clear();

        // Reset the GUI to defaults.
        if(mpTimer->IsRunning() == false)
        {
            ResetVideoPanel();
        }
        
        // Release drive control.
        mpJoystick->TakeDriveControl(false);
        CxUtils::SleepMs(100);
        mpJoystick->EnableJoystick(false);
        CxUtils::SleepMs(100);
        mpJoystick->SetSubsystemToControl(0);  

        // Try to create a video subscription.
        if(mpVideo->CreateVideoSubscription(id,
                                            cameraID,
                                            1000))
        {
            mVisualSensorID = id;
            mCameraNumber = cameraID;
            mFrameNumber = 0;
            mFrameUpdateTimeMs = 0;
            mRecordFrameNumber = 0;
            mpTimer->Start(gTimerDelayMs);
        }
        else
        {
            wxMessageBox(wxT("Failed to Create Video Subscription"));
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief When the select Primitive Driver/Joystick menu option is selected 
///         this method asks the user to select from available Primitive Driver
///         service for the robot we are viewing video of.
///
///  \param[in] guiEvent Required wxCommandEvent parameter
///
////////////////////////////////////////////////////////////////////////////////////
void VideoClientFrame::OnSelectPrimitiveDriver(wxCommandEvent& guiEvent)
{
    if(mpJoystick->IsJoystickEnabled())
    {
        mpJoystick->TakeDriveControl(false);
        CxUtils::SleepMs(100);
        mpJoystick->EnableJoystick(false);
        CxUtils::SleepMs(100);
        mpJoystick->SetSubsystemToControl(0); 
        return;
    }

    JAUS::UShort subsystemID = 0;

    if(mVisualSensorID.IsValid() == false)
    {
        mMutex.Unlock();
        wxMessageBox(wxT("Please Select a Visual Sensor First!"));
        return;
    }

    subsystemID = mVisualSensorID.mSubsystem;
    
    // Load an appropriate settings file for the joystick.
    if(mpJoystick->LoadSettingsForVehicle(mComponent.DiscoveryService()->GetSubsystem(subsystemID)->mIdentification,
                                          "settings/joysticks/") == false)
    {
        mpJoystick->LoadSettings("settings/joysticks/default.xml");
    }
    mpJoystick->SetSubsystemToControl(subsystemID);
    mpJoystick->EnableJoystick();
    mpJoystick->TakeDriveControl(true);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Called when the user selects the option to change authority level
///         from the file menu.
///
///  \param[in] guiEvent Required wxCommandEvent parameter
///
////////////////////////////////////////////////////////////////////////////////////
void VideoClientFrame::OnSelectAuthorityLevel(wxCommandEvent& guiEvent)
{
    wxNumberEntryDialog authorityDialog(this, 
                                        TEXT_TYPE("Change Authority"), 
                                        TEXT_TYPE("Authority:"), 
                                        TEXT_TYPE("Authority Level"), 
                                        mComponent.AccessControlService()->GetAuthorityCode(), 0, 255);
    if(authorityDialog.ShowModal() == wxID_OK)
    {
        JAUS::Byte authority = (JAUS::Byte)authorityDialog.GetValue();
        mComponent.AccessControlService()->SetAuthorityCode(authority);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Function called when mpTimer activates
///
///  This method checks to see if the video has been updating from a visual
///  sensor and if so, re-draws the window.
///
///  \param guiEvent Required wxTimerEvent parameter
///
////////////////////////////////////////////////////////////////////////////////////
void VideoClientFrame::OnTimer(wxTimerEvent& guiEvent)
{
    // If the component is not initialized, do so.
    if(mComponent.IsInitialized() == false)
    {
        if(mComponent.InitializeWithUniqueID())
        {
            std::string id = "ID " + mComponent.GetComponentID().ToString();
            SetStatusText(wxString(id.c_str(), wxConvUTF8), 1);
        }
    }
    else
    {
        std::string id = "ID " + mComponent.GetComponentID().ToString();
        if(mpJoystick->IsJoystickEnabled())
        {
            if(mpJoystick->GetDriverID().IsValid() == false && mpJoystick->GetMicrocontrollerID().IsValid() == false)
            {
                id += " - Joystick On - No Driver Found";
            }
            else
            {
                id += " - Joystick On";
                if(mComponent.AccessControlService()->HaveControl(mpJoystick->GetDriverID()) ||
                   mComponent.AccessControlService()->HaveControl(mpJoystick->GetMicrocontrollerID()))
                {
                    id += " - In Control";
                }
                else
                {
                    id += " - Control Lost";
                }
            }
        }
        SetStatusText(wxString(id.c_str(), wxConvUTF8), 1);
    }
    static JAUS::Time::Stamp prevUpdateTimeMs = 0;

    if(mCurrentImage.mDataSize > 0 &&
       mCurrentImage.mpImage != NULL &&
       mFrameUpdateTimeMs != prevUpdateTimeMs)
    {       
        mMutex.Lock();
        
        wxImage image(mCurrentImage.mWidth,
                      mCurrentImage.mHeight,
                      mCurrentImage.mpImage,
                      true);
        mMutex.Unlock();
        if(image.Ok())
        {
            if(prevUpdateTimeMs == 0)
            {
                this->SetClientSize(wxSize(image.GetWidth(), image.GetHeight() + 15));
            }
            mpImagePanel->SetImage(image);

            // Show the user the frame number and source.
            char buffer[256];
            sprintf(buffer, 
                    "Video Client - [%s - %d] - %d", 
                    mVisualSensorID.ToString().c_str(), 
                    (int)mCameraNumber, 
                    mFrameNumber);
            SetTitle(wxString(buffer, wxConvUTF8));
        }
    }
    
    prevUpdateTimeMs = mFrameUpdateTimeMs;
} 


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Disconnects from any visual sensors we are subscribing to on a 
///         Disconnect event.
///
///  \param[in] guiEvent Required wxCommandEvent parameter
///
////////////////////////////////////////////////////////////////////////////////////
void VideoClientFrame::OnDisconnect(wxCommandEvent& guiEvent)
{
    mpTimer->Stop();
    // Cancel the video subscription.
    mpVideo->CancelVideoSubscription(mVisualSensorID, mCameraNumber);
    mpVideo->CancelVideoSubscription(mVisualSensorID, mCameraNumber);
    mVisualSensorID.Clear();
    mFrameUpdateTimeMs = 0;
    
    mpJoystick->TakeDriveControl(false);
    CxUtils::SleepMs(100);
    mpJoystick->EnableJoystick(false);
    CxUtils::SleepMs(100);
    mpJoystick->SetSubsystemToControl(0);    
    
    ResetVideoPanel();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Method called when an On Record Video event triggered from menu.
///
///  \param[in] guiEvent Required wxCommandEvent parameter
///
////////////////////////////////////////////////////////////////////////////////////
void VideoClientFrame::OnRecord(wxCommandEvent& guiEvent)
{
    // Create dialog.
    wxMessageDialog quitDialog(NULL, TEXT_TYPE("Record Image Data"), TEXT_TYPE("Record Video?"), wxNO_DEFAULT|wxYES_NO|wxICON_QUESTION);
    // Show dialog and get response from user.
    switch (quitDialog.ShowModal())
    {
    case wxID_YES: // Record image data.
        mRecordFlag = true;
        mRecordFrameNumber = 0;
        CxUtils::FileIO::CreateDir("video");
        CxUtils::FileIO::CreateDir("video/log");
        break;
    case wxID_NO: //  Don't record.
        mRecordFlag = false;
        mRecordFrameNumber = 0;
        break;
    default:
        break;
    }
}

/*
////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Method called when an On Record Video event triggered from menu.
///
///  \param[in] guiEvent Required wxCommandEvent parameter
///
////////////////////////////////////////////////////////////////////////////////////
void VideoClientFrame::OnSize(wxSizeEvent& sizeEvent)
{
    sizeEvent.Skip();
    //mpImagePanel->OnSize(sizeEvent);
}
*/

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Processes compressed image data  via a callback.
///
///  \param[in] source Source of the video data.
///  \param[in] camera The camera ID from the source.
///  \param[in] format Image compression format.
///  \param[in] compressedImage Compressed image data.
///  \param[in] compressedImageSize Size of data in bytes.
///  \param[in] frameNumber The frame ID number the source provided.
///
////////////////////////////////////////////////////////////////////////////////////
void VideoClientFrame::ProcessCompressedVideo(const JAUS::Address& source,
                                              const JAUS::Byte camera,
                                              const JAUS::Image::Format format,
                                              const unsigned char* compressedImage,
                                              const unsigned int compressedImageSize,
                                              const unsigned int frameNumber)
{
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Processes image data  via a callback.
///
///  \param[in] source Source of the video data.
///  \param[in] camera The camera ID from the source.
///  \param[in] image Decompressed image data.
///  \param[in] frameNumber The frame ID number the source provided.
///
////////////////////////////////////////////////////////////////////////////////////
void VideoClientFrame::ProcessRawImage(const JAUS::Address& source,
                                       const JAUS::Byte camera,
                                       const JAUS::Image& image,
                                       const unsigned int frameNumber)
{
    JAUS::Image flipped(image);
    flipped.FlipChannels();
        
    mMutex.Lock();
    if(source == mVisualSensorID && camera == mCameraNumber)
    {
        mCurrentImage = flipped;
        mFrameNumber = frameNumber;
        mFrameUpdateTimeMs = JAUS::Time::GetUtcTimeMs();
    }
    mMutex.Unlock();
    
    if(mRecordFlag)
    {
        char buffer[512];
        char dirName[512];
        sprintf(dirName, "video/log/%s-%d", source.ToString().c_str(), (int)camera);
        CxUtils::FileIO::CreateDir(dirName);
        sprintf(buffer, "%s/%05d.jpg", dirName, mRecordFrameNumber++);
        flipped.Save(buffer);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the default image to use for the Video Panel, and clears
///         video image frame numbers, and disables recording.
///
////////////////////////////////////////////////////////////////////////////////////
void VideoClientFrame::ResetVideoPanel()
{
    JAUS::Mutex::ScopedLock lock(&mMutex);
    // Load default image for video panel.
    wxImage image;
    wxInitAllImageHandlers();
    image.LoadFile(wxT("images/please_stand_by.png"));
    mpImagePanel->SetImage(image);

    // Reset image information.
    mFrameNumber = 0;
    mFrameUpdateTimeMs = 0;
    mRecordFlag = false;

    // Update Status Bar.
    SetTitle(wxString("Video Client", wxConvUTF8));
}

/*  End of File */
