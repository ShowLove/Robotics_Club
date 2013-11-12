////////////////////////////////////////////////////////////////////////////////////
///
/// \file calibration_program.h
/// \brief Finds camera calibration characteristics and saves them to re-loadable
///        XML files.
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu<br>
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
////////////////////////////////////////////////////////////////////////////////////
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cxutils/cxutils.h>
#include <cvcapture/cvimagecapture.h>
//#include <ground/vision/homography.h>
#include <homography/homography.h>
using namespace Zebulon;
using namespace Vision;

Homography gHomo;
Zebulon::Video::CvImageCapture gCamera;
int gBoardWidth = 5;
int gBoardHeight = 8;
std::string gSaveName = "settings/vision/homo.xml";
std::string gCameraDir = "/dev/video0";

bool ConnectToCamera(const std::string& camera)
{
    if(gCamera.Start(camera, 0, 0, true, true))
        return true;
    return false;
}

IplImage * CaptureFrame()
{
    //Zebulon::Video::CvConvertToIplImage();
    Zebulon::Video::CvImageCapture::Image img;
    gCamera.GetFrame(&img);
    IplImage * image = NULL;
    Zebulon::Video::CvConvertToIplImage(img,&image);
    return image;
}

int main(int argc, char* argv[])
{
    int key = 0;
    bool process = false;
    IplImage * image = NULL;
    IplImage * processed = NULL;
    if(argc > 1) gCameraDir = argv[1];
    if(!ConnectToCamera(gCameraDir))
    {
        return 0;
    }
    gHomo.LoadSettings();
    cvNamedWindow("Camera");
    cvNamedWindow("Processed");
    while(process == false && key != 1048603)
    {
        image = CaptureFrame();
        cvShowImage("Camera",image);
        if(gHomo.CheckerBoardCalibrate(image))
        {
            processed = gHomo.Warp(image);
        }
        cvShowImage("Processed", processed);
        key = cvWaitKey(10);

        //press 'c' to capture and save homography
        if(key == 1048675)
        {
            gHomo.SaveSettings("settings/vision/homo2.xml");
        }
        cvReleaseImage(&processed);
        cvReleaseImage(&image);
        processed = NULL;
        image = NULL;
    }

}
