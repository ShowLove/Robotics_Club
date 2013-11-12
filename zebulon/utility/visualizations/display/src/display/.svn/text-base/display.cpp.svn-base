/////////////////////////////////////////////////////////////////////////////////////
///
/// \file display.cpp
/// \brief Thread-safe way to display IplImages.  Also includes other
///        methods to help with window management.
///
/// Author(s): Gary Stein<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: gstein@ucf.edu <br>
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
#include "display/display.h"

using namespace Zebulon;
using namespace Utility;

CxUtils::Mutex Display::mDispMutex;
// Assume display is turned on by default.
bool Display::mDisplayFlag = true;
double Display::mScale = 1;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a window for display.
///
///   \param[in] windowName Window name.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Display::CreateNamedWindow(const std::string& windowName)
{
    if(mDisplayFlag)
    {
        mDispMutex.Lock();
        cvNamedWindow( windowName.c_str() , CV_WINDOW_AUTOSIZE);
        mDispMutex.Unlock();
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Displays an image to a window.
///
///   \param[in] windowName Window name.
///   \param[in] image Image to display.
///   \param[in] doWaitKey How long to do a cvWaitKey to call, set to -1 to not
///                        do cvWaitKey call.
///
///   \return 1 on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Display::DisplayImage(const std::string& windowName, IplImage *image, const int doWaitKey)
{
    if(image==NULL)
    {
        return 0;
    }
    
    if(mDisplayFlag)
    {
        mDispMutex.Lock();

        cvNamedWindow( windowName.c_str() , CV_WINDOW_AUTOSIZE);
        if(fabs(mScale - 1) > 0.01)
        {
            IplImage *smallImage = cvCreateImage(cvSize((int)(image->width*mScale),(int)(image->height*mScale)),
                                                 image->depth, image->nChannels);
            cvResize(image,smallImage);
            cvShowImage(windowName.c_str(), smallImage);
            if(doWaitKey >= 0)
                cvWaitKey(doWaitKey);
            cvReleaseImage(&smallImage);
        }
        else
        {
            cvShowImage(windowName.c_str(), image);
            if(doWaitKey >= 0)
                cvWaitKey(doWaitKey);
        }

        mDispMutex.Unlock();
    }
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to globally turn off all windows across all threads.
///
///   \param[in] displayFlag Flag used to turn on/off display of imaeges within
///              a process (using this interface).
///
////////////////////////////////////////////////////////////////////////////////////
void Display::SetDisplay(const bool displayFlag)
{
    mDisplayFlag = displayFlag;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets image scaling.
///
///   \param[in] scale Scale values for images displayed (0, X].
///
////////////////////////////////////////////////////////////////////////////////////
void Display::SetScale(const double scale)
{
    if(scale <= 0)
    {
        return;
    }
    
    mScale = scale;
}

/*  End of File */
