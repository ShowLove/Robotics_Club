/////////////////////////////////////////////////////////////////////////////////////
///
/// \file display.h
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
#ifndef _ZEBULON_VISUALIZATIONS_DISPLAY__H
#define _ZEBULON_VISUALIZATIONS_DISPLAY__H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

#include <string>
#include <cxutils/mutex.h>

////////////////////////////////////////////////////////////////////////////////////
///
///  \def ZEB_VISUALIZE_DISPLAY_DLL
///  \brief Pre-processor definition used for exporting/importing DLL for library.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef ZEB_VISUALIZE_DISPLAY_DLL
    #ifdef WIN32
        #define ZEB_VISUALIZE_DISPLAY_DLL __declspec(dllimport)
    #else
        #define ZEB_VISUALIZE_DISPLAY_DLL
    #endif
#endif

namespace Zebulon
{
    namespace Utility
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Display
        ///   \brief Contains methods for displaying IplImage data from OpenCV in a
        ///          multi-threading environment.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_VISUALIZE_DISPLAY_DLL Display
        {
            public:
#ifndef WIN32
                // Method kept for combatibility with old code used in Linux systems.
                static int CreateWindow(const std::string& windowName) { return CreateNamedWindow(windowName); }
#endif
                static int CreateNamedWindow(const std::string& windowName);
                static int DisplayImage(const std::string& windowName,
                                        IplImage *image,
                                        const int doWaitKey = 10);
                static void SetDisplay(const bool displayFlag);
                static void SetScale(const double scale);
                static bool GetDisplayFlag() { return mDisplayFlag; }
                static bool CloseAllWindows()
                {
                    if(mDisplayFlag)
                    {
                        mDispMutex.Lock();
                        cvDestroyAllWindows();
                        mDispMutex.Unlock();
                    }
                    return true;
                }

            private:
                static CxUtils::Mutex mDispMutex; ///<  Mutex for thread protection.
                static bool mDisplayFlag;         ///<  Turns on/off display globally.
                static double mScale;             ///<  Percentage to scale all displayed data.
        };
    }
}
#endif
