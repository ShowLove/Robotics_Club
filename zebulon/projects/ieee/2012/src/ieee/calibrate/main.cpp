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

#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <stdio.h>
#include <iostream>

#include <cxutils/mutex.h>
#include <tinyxml/tinyxml.h>

const std::string WindowName = "Calibrate";

enum Task
{
    Task1Left = 0,
    Task1Right,

    Task2Left,
    Task2Right,

    Task3Left,
    Task3Right,

    Task4Left,
    Task4Right
};

enum Mode
{
    TaskMode,
    MaskMode,
    StartMode
};
Mode gMode = TaskMode;

Task gCurrentTask = Task1Left;
cv::Rect gTasks[8];

cv::Rect gStartArea;
cv::Rect gMask[4];
int gCurrentMask;

CxUtils::Mutex gMutex;

bool gDrawing=false;
void mouseEvent(int event, int x, int y, int flags, void* param)
{
    CxUtils::Mutex::ScopedLock lock(&gMutex);

    switch (event)
    {
    case CV_EVENT_MOUSEMOVE:
        if (gDrawing == true)
        {
            switch (gMode)
            {
            case TaskMode:
                gTasks[gCurrentTask].width = abs(x - gTasks[gCurrentTask].x);
                gTasks[gCurrentTask].height = abs(y - gTasks[gCurrentTask].y);
                break;

            case StartMode:
                gStartArea.width = abs(x - gStartArea.x);
                gStartArea.height = abs(y - gStartArea.y);
                break;

            case MaskMode:
                gMask[gCurrentMask].width = abs(x - gMask[gCurrentMask].x);
                gMask[gCurrentMask].height = abs(y - gMask[gCurrentMask].y);
                break;
            }


        }
        break;

    case CV_EVENT_LBUTTONDOWN:
        gDrawing = true;

        switch (gMode)
        {
        case TaskMode:
            gTasks[gCurrentTask].x = x;
            gTasks[gCurrentTask].y = y;
            break;

        case StartMode:
            gStartArea.x = x;
            gStartArea.y = y;
            break;

        case MaskMode:
            gMask[gCurrentMask].x = x;
            gMask[gCurrentMask].y = y;
            break;
        }


        break;

    case CV_EVENT_LBUTTONUP:
        gDrawing = false;
        break;
    }
}

void saveWork(const std::string& file);

int main(int argc, char** argv)
{
    bool _useCamera = true;
    cv::VideoCapture capture(0);
    cv::VideoCapture capture1(1);
    capture.set(CV_CAP_PROP_CONVERT_RGB, false);
    capture.set(CV_CAP_PROP_FPS, 9);
    capture1.set(CV_CAP_PROP_CONVERT_RGB, false);
    capture1.set(CV_CAP_PROP_FPS, 9);
    char* buffer = NULL;

    std::string _saveFileName = "settings/course.xml";

    cv::Mat frame;
    cv::Mat frame1;

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

    cv::namedWindow(WindowName);
    cv::setMouseCallback(WindowName, mouseEvent);

    cv::namedWindow("Frame0");
    cv::namedWindow("Frame1");

    int key=0;
    int _frameIndex = 0;
    while (true)
    {
        if (_useCamera)
        {
            capture >> frame;
            cv::imshow("Frame0", frame);

            /*capture1 >> frame1;
            cv::imshow("Frame1", frame1);//*/
        }
        else
        {
            sprintf(buffer, "%s/image%05d.png", argv[1], _frameIndex);
            frame = cv::imread(buffer);
        }

        gMutex.Lock();
        switch (key)
        {
        // Save work
        case 115: //s
            std::cout << "Saving to \'" << _saveFileName << "\'...";
            saveWork(_saveFileName);
            std::cout << "Done!" << std::endl;
            break;

        case 49: //0
            if (gMode==TaskMode)
            {
                gCurrentTask = Task1Left;
                std::cout << "Task 1 Left" << std::endl;
            }

            break;
        case 50: //1
            if (gMode==TaskMode)
            {
                gCurrentTask = Task2Left;
                std::cout << "Task 2 Left" << std::endl;
            }
            break;
        case 51: //2
            if (gMode==TaskMode)
            {
                gCurrentTask = Task3Left;
                std::cout << "Task 3 Left" << std::endl;
            }
            break;
        case 52: //3
            if (gMode==TaskMode)
            {
                gCurrentTask = Task4Left;
                std::cout << "Task 4 Left" << std::endl;
            }
            break;

        case 53: //4
            if (gMode==TaskMode)
            {
                gCurrentTask = Task1Right;
                std::cout << "Task 1 Right" << std::endl;
            }
            break;
        case 54: //5
            if (gMode==TaskMode)
            {
                gCurrentTask = Task2Right;
                std::cout << "Task 2 Right" << std::endl;
            }
            break;
        case 55: //6
            if (gMode==TaskMode)
            {
                gCurrentTask = Task3Right;
                std::cout << "Task 3 Right" << std::endl;
            }
            break;
        case 56: //7
            if (gMode==TaskMode)
            {
                gCurrentTask = Task4Right;
                std::cout << "Task 4 Right" << std::endl;
            }
            break;

        case 97: // a
            gMode = StartMode;
            gDrawing = false;
            break;

        case 109: // m
            gMode = MaskMode;
            gDrawing = false;
            break;

        case 116: // t
            gMode = TaskMode;
            gDrawing = false;
            break;

        default:
            if (key != 255)
                std::cout << "Uknown key " << key << std::endl;
        }

        for (int i=0; i<8; i++)
        {
            if (i == gCurrentTask)
                cv::rectangle(frame, gTasks[i], cv::Scalar(0,127,255));
            else
                cv::rectangle(frame, gTasks[i], cv::Scalar(255,127,0));
        }

        for (int i=0; i<4; i++)
        {
            if (i == gCurrentMask)
                cv::rectangle(frame, gMask[i], cv::Scalar(0,0,0), 2);
            else
                cv::rectangle(frame, gMask[i], cv::Scalar(127,127,127), 2);
        }

        cv::rectangle(frame, gStartArea, cv::Scalar(0,255,255));
        gMutex.Unlock();

        cv::imshow(WindowName, frame);

        _frameIndex = (_frameIndex + 1) % 1582; // number picked for now, need to change
        if (_useCamera)
        {
            if ( (key = (cvWaitKey(1) & 255)) == 27 )
            {
                break;
            }
        }
        else
        {
            if ( (key = (cvWaitKey(10) & 255)) == 27 )
            {
                break;
            }
        }
    }


    return 0;
}


void saveWork(const std::string& file)
{
    CxUtils::Mutex::ScopedLock lock(&gMutex);

    TiXmlDocument doc;
    //TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    //doc.LinkEndChild(decl);

    TiXmlElement * element;

    TiXmlElement* decl = new TiXmlElement( "xml" );

    element = new TiXmlElement( "Start" );
    element->SetAttribute("x", gStartArea.x);
    element->SetAttribute("y", gStartArea.y);
    element->SetAttribute("width", gStartArea.width);
    element->SetAttribute("height", gStartArea.height);
    decl->LinkEndChild(element);

    element = new TiXmlElement( "Task1Left" );
    element->SetAttribute("x", gTasks[Task1Left].x);
    element->SetAttribute("y", gTasks[Task1Left].y);
    element->SetAttribute("width", gTasks[Task1Left].width);
    element->SetAttribute("height", gTasks[Task1Left].height);
    decl->LinkEndChild(element);

    element = new TiXmlElement( "Task2Left" );
    element->SetAttribute("x", gTasks[Task2Left].x);
    element->SetAttribute("y", gTasks[Task2Left].y);
    element->SetAttribute("width", gTasks[Task2Left].width);
    element->SetAttribute("height", gTasks[Task2Left].height);
    decl->LinkEndChild(element);

    element = new TiXmlElement( "Task3Left" );
    element->SetAttribute("x", gTasks[Task3Left].x);
    element->SetAttribute("y", gTasks[Task3Left].y);
    element->SetAttribute("width", gTasks[Task3Left].width);
    element->SetAttribute("height", gTasks[Task3Left].height);
    decl->LinkEndChild(element);

    element = new TiXmlElement( "Task4Left" );
    element->SetAttribute("x", gTasks[Task4Left].x);
    element->SetAttribute("y", gTasks[Task4Left].y);
    element->SetAttribute("width", gTasks[Task4Left].width);
    element->SetAttribute("height", gTasks[Task4Left].height);
    decl->LinkEndChild(element);


    element = new TiXmlElement( "Task1Right" );
    element->SetAttribute("x", gTasks[Task1Right].x);
    element->SetAttribute("y", gTasks[Task1Right].y);
    element->SetAttribute("width", gTasks[Task1Right].width);
    element->SetAttribute("height", gTasks[Task1Right].height);
    decl->LinkEndChild(element);

    element = new TiXmlElement( "Task2Right" );
    element->SetAttribute("x", gTasks[Task2Right].x);
    element->SetAttribute("y", gTasks[Task2Right].y);
    element->SetAttribute("width", gTasks[Task2Right].width);
    element->SetAttribute("height", gTasks[Task2Right].height);
    decl->LinkEndChild(element);

    element = new TiXmlElement( "Task3Right" );
    element->SetAttribute("x", gTasks[Task3Right].x);
    element->SetAttribute("y", gTasks[Task3Right].y);
    element->SetAttribute("width", gTasks[Task3Right].width);
    element->SetAttribute("height", gTasks[Task3Right].height);
    decl->LinkEndChild(element);

    element = new TiXmlElement( "Task4Right" );
    element->SetAttribute("x", gTasks[Task4Right].x);
    element->SetAttribute("y", gTasks[Task4Right].y);
    element->SetAttribute("width", gTasks[Task4Right].width);
    element->SetAttribute("height", gTasks[Task4Right].height);
    decl->LinkEndChild(element);

    doc.LinkEndChild(decl);

    doc.SaveFile(file.c_str());
}
