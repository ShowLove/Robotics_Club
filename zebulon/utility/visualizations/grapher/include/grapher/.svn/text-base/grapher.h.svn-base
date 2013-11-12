////////////////////////////////////////////////////////////////////////////////////
///
/// \file grapher.h
/// \brief A Grapher class used for debugging display
///
/// Author(s): Gary Stein<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF)<br>
/// Email: gstein@mail.ucf.edu<br>
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
#ifndef __ZEBULON_UTILITY_GRAPHER
#define __ZEBULON_UTILITY_GRAPHER

#include <cmath>
#include <cxutils/time.h>
#include <cxutils/mutex.h>
#include <display/display.h>
#include <vector>
#include <cstdio>


//////////////////////////////////////////////////////////////////////////////////// 
///  
///  \def ZEB_GRAPHER_DLL
///  \brief Pre-processor definition used for exporting/importing DLL for library.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef ZEB_GRAPHER_DLL
    #ifdef WIN32
        #define ZEB_GRAPHER_DLL __declspec(dllimport)
    #else
        #define ZEB_GRAPHER_DLL
    #endif
#endif

namespace Zebulon
{
    namespace Utility
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Grapher 
        ///   \brief Visualization utility for drawing graphs using OpenCV.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_GRAPHER_DLL Grapher
        {
            public:
                Grapher();
                virtual ~Grapher();
                void Setup(const std::string& windowName,   // Window name.
                           int numAxis,                     // Number of items to graph
                           int resolution=1000,             // Pixel resolution.
                           int width=1000,                  // Width in pixels per graph
                           int height=100);                 // Height in pixels per graph
                void AddValue(int axis, double value);
                void Graph();
                void Range(int axis,double min, double max);
            protected:
                std::vector< std::vector<double> > mAxii;   ///<  All Axii.
                CxUtils::Mutex mMutex;                      ///<  For thread proteciton.
                std::string mWindowName;                    ///<  Window name.
                int mNumAxis;                               ///<  Number of things to plot.
                int mResolution;                            ///<  Resolution.
                int mHeight;                                ///<  Pixel height.
                int mWidth;                                 ///<  Pixel width.
                std::vector<double> mXAxis;                 ///<  Number of X axis.
                std::vector<double> mMins;                  ///<  Minimum values.
                std::vector<double> mMaxs;                  ///<  Maximum values.
                IplImage *mPlotImage;                       ///<  Plot image.
                struct rgb
                {
                    int r,g,b;
                };
                std::vector<rgb> mColors;                   ///< RGB color triplet per data.
                CvFont mFont;
        };
    }
}

#endif  // __ZEBULON_UTILITY_GRAPHER

/* End of File */
