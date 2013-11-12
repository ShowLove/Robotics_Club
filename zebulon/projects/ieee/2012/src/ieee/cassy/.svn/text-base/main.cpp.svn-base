/////////////////////////////////////////////////////////////////////////////////////
///
/// \file main.cpp
/// \brief A program to test some opencv algorithms.
///
/// Author(s): Michael Scherer<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@knights.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
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

#if 0

#include "ieee/cassy/ieeevision.h"

#include "ieee/cassy/ieeetasks.h"

#include "opencv/cv.h"
#include "opencv/highgui.h"
//#include "opencv/"

#include <stdio.h>
#include <iostream>

using namespace Zebulon;
using namespace IEEE;

void buttonPressed_StartRun(int state, void* user)
{
    Tasks* gameLogic = static_cast<Tasks*>(user);

    gameLogic->Start();
}

void buttonPressed_StopRun(int state, void* user)
{
    Tasks* gameLogic = static_cast<Tasks*>(user);

    gameLogic->Stop();
}

void buttonPressed_NewRun(int state, void* user)
{
    Tasks* gameLogic = static_cast<Tasks*>(user);

    gameLogic->NewRun();
}

void UpdateServer(void* args);

CxUtils::Thread gServerUpdateThread;
int main(int argc, char** argv)
{
    std::cout << "BEGINNING" << std::endl;

    Zebulon::Configuration::XmlConfigurator _config;
    bool success = _config.Load("settings/ieeevision.xml", "IEEEVision");
    if (!success)
        std::cout << "Error loading xml file" << std::cout;

    char* buffer;
    bool _useCamera = true;
    cv::VideoCapture capture(0);
    if (argc > 1)
    {
        _useCamera = false;
        std::cout << "Loading from file \'" << argv[1] << "\'" << std::endl;
        buffer = (char*)malloc(sizeof(char) * (strlen(argv[1]) + 17));
    }
    else
    {
        if (capture.isOpened())
            std::cout << "No file specified, loading from camera" << std::endl;
        else
        {
            std::cout << "Camera failed to load, shutting down..." << std::endl;
            return -1;
        }
    }

    Tasks _gameLogic;
    _gameLogic.Initialize(&_config);
    _gameLogic.NewRun();
    int _teamID;

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>....
    // Gui

    cv::namedWindow("Out");

    cv::createButton("STOP Run", buttonPressed_StopRun, &_gameLogic);
    cv::createButton("New Run", buttonPressed_NewRun, &_gameLogic);
    cv::createButton("Start Run", buttonPressed_StartRun, &_gameLogic);

    cv::createTrackbar("Team ID", "", &_teamID, 52);

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>....

    //std::cout << "Loading \"" << argv[1] << '\"' << std::endl << " with calibration from \"" << argv[2] << '\"' << std::endl;
    cv::Mat image;
    cv::Mat out = cv::Mat(cv::Size(640,480), CV_8UC1);
    cv::Mat blur1;
    cv::Mat blur2;
    cv::Mat blurred;
    std::vector< cv::Point2f > trail;
    std::vector< Vision::Return > returns;
    IEEEVisionCassy vision;

    cv::Mat finalImg(600, 800, CV_8UC3);

    //vision.Load("settings/IEEEVision.xml");


    gServerUpdateThread.CreateThread(UpdateServer, &_gameLogic);

    // "/image00000.png" 16 chars (including \0)
    int imageIndex = 0;
    int frameCount = 0;
    while (true)
    {
        if (_useCamera)
        {
            capture >> image;
        }
        else
        {
            sprintf(buffer, "%s/image%05d.png", argv[1], imageIndex);
            image = cv::imread(buffer);
        }

        //cv::imshow("Raw", image);

        vision.backgroundSubtraction(image, out);
        vision.backgroundSubtractionCleanUp(out, out);
        vision.segmentation(out, out);
        //vision.AdditionalInformation(true);

        //std::cout << "Rows: " << finalImg.rows << ", cols: " << finalImg.cols << std::endl;
        vision.ConditionFinalImage(finalImg);
        vision.AddRawImage(image, finalImg);

        cv::imshow("Final Output", finalImg);

        returns = vision.GetSegments();

        //std::cout << "returns: " << returns.size() << std::endl;
        _gameLogic.Update(returns);
        //std::cout << "Score: " << _gameLogic.GetScore() << " Laps: " << _gameLogic.GetLaps() << std::endl;

        trail = vision.TrailCalc(trail, returns.size(), 100);

        _gameLogic.DrawDebug(out);
        vision.DisplayTrail(out, out, trail);

        cv::imshow("Out", out);

        //delete image;
        imageIndex = (imageIndex + 1) % 1582; // number picked for now, need to change
        frameCount++;
        // Escape key pressed
        if ( (cvWaitKey(0) & 255) == 27 ) break;
    }

    gServerUpdateThread.StopThread();

    return 0;
}

void UpdateServer(void* args)
{
    Tasks* gameLogic = static_cast<Tasks*>(args);

    while(!gServerUpdateThread.QuitThreadFlag())
    {

        CxUtils::SleepMs(20);
    }
}

#endif

/* End of file */
