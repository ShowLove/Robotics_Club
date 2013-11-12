////////////////////////////////////////////////////////////////////////////////////
///
/// \file objectdetection.h
/// \brief Class for detecting colors and objects with bounding boxes
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams.ucf.edu<br>
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
#ifndef __ZEBULON_VISION_OBJECT__H
#define __ZEBULON_VISION_OBJECT__H

#include <list>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "xmlconfig/xmlconfigurator.h"
namespace Zebulon
{
    namespace Vision
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class VisionObject
        ///  \brief Interface for a Vision objects that finds itself
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class VisionObject
        {
        public:

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class MaskOption
            ///  \brief Helper object for common mask settings
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class MaskOption
            {
            public:
                typedef std::vector<MaskOption> Vector;
                std::string mCvColorConversionName;
                unsigned char mChannelIndex;
                double mWeight;
                bool mEnabledFlag;
                bool mInvertFlag;
            };

            virtual bool Initialize(const std::string& parameter) = 0;
            virtual IplImage* GetMask(const IplImage * imgIn, IplImage * debugOut = NULL) const = 0;
            virtual std::list<CvBox2D> GetBounding(const IplImage * imgIn, IplImage * imgOut=NULL) const = 0;
        };
    }
}
#endif //__ZEBULON_VISION_OBJECT__H
