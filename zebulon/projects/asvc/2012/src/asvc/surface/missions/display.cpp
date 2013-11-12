//////////////////////////////////////////////////////////////////////////////////////
///
/// \file display.cpp
/// \brief Mission to only show sensor data.
///
/// Author(s): Daniel Barber<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:dbarber@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
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
//////////////////////////////////////////////////////////////////////////////////////
#include "asvc/surface/missions/display.h"
#include "asvc/surface/depthmap.h"
#include <display/display.h>
#include <opencv/highgui.h>

using namespace Zebulon;
using namespace ASVC;

const std::string Display::Name = "Display";

int gX = -1;
int gY = -1;
int gRegisterCallback = -1;
CxUtils::Time::Stamp gClickTimeMs = 0;

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Opencv function that gets called on mouse click event.
///
///  \param event Refer to opencv.
///  \param x X Refer to opencv.
///  \param y Y Refer to opencv.
///  \param flags Refer to opencv.
///  \param param Refer to opencv.
///
////////////////////////////////////////////////////////////////////////////////////
void DisplayMouseEvent(int event, int x, int y, int flags, void* param)
{
    if(event == CV_EVENT_LBUTTONDOWN)
    {
        gX = x;
        gY = y;
        gClickTimeMs = CxUtils::Time::GetUtcTimeMs();
        //Kinect::Point3D point = Kinect::DepthToWorld(x, y, depthValue);

    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
Display::Display()
{
    ID = Name;
    mpRawImage = mpDepthData = mpDepthImage = mpBackupCamera = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Display::~Display()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the mission.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int Display::Init()
{
    Mission::Init();
    ID = Name;
    //mXmlConf.GetVar("Surface.Display.NextMission@name", mNextMission);
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Error Check method, does any required error checking.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int Display::ErrorCheck()
{
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Fetches data required for execution.
///
///  Gets start button state, and checks to see if GPS data is available.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int Display::FetchProcess()
{
    if(mpGlobalInfo->GetPylonCamera())
    {
        mpGlobalInfo->GetPylonCamera()->LoadSettings("settings/pylon_settings.txt");
    }
    if(mpGlobalInfo->IsValueSet(GlobalInfo::MainCamera))
    {
        mpGlobalInfo->GetInfo(GlobalInfo::MainCamera, mpRawImage);
    }
    if(mpGlobalInfo->IsValueSet(GlobalInfo::DepthDataMainCamera))
    {
        mpGlobalInfo->GetInfo(GlobalInfo::DepthDataMainCamera, mpDepthData);
        mpGlobalInfo->GetInfo(GlobalInfo::DepthImageMainCamera, mpDepthImage);
        if(gRegisterCallback < 0)
        {
            gRegisterCallback = 0;
        }
        DepthMap* depthMap = mpGlobalInfo->GetDepthMap();
        if(gX > 0 && gY > 0 && depthMap)
        {
            int x = gX;
            int y = gY;
            // Get depth at location
            float distance =  CV_IMAGE_ELEM(mpDepthData, float, y, x);

            CxUtils::Point3D point;

            point = depthMap->ConvertPixelToCoordinates(y, x, distance);
            // Convert
            std::cout << "Distance at PX [" << gX << ", " << gY << "] - ";
            point.Print();
        }
    }

    if(mpGlobalInfo->IsValueSet(GlobalInfo::BackupCamera))
    {
        mpGlobalInfo->GetInfo(GlobalInfo::BackupCamera, mpBackupCamera);
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief If there is an early exit condition, this method will handle it.
///
///  \return Mission::KeepRunning.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Display::ExitEarly()
{
    return Mission::KeepRunning;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Executes the mission data.
///
///  \return Mission::KeepRunning.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Display::Execute()
{
    if(mpRawImage)
    {
        Utility::Display::DisplayImage(GlobalInfo::MainCamera, mpRawImage, 10);
    }
    if(mpDepthImage)
    {
        Utility::Display::DisplayImage(GlobalInfo::DepthImageMainCamera, mpDepthImage, 10);
        if(gRegisterCallback == 0)
        {
            cvSetMouseCallback(GlobalInfo::DepthImageMainCamera.c_str(), DisplayMouseEvent, NULL);
            gRegisterCallback = 1;
        }
    }
    if(mpBackupCamera)
    {
        Utility::Display::DisplayImage(GlobalInfo::BackupCamera, mpBackupCamera, 10);
    }
    return Mission::KeepRunning;
}
/* End of File */
