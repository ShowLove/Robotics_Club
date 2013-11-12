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
#ifndef __ZEBULON_IGVC_BARREL_OBJECT__H
#define __ZEBULON_IGVC_BARREL_OBJECT__H
#include "igvc/ground/vision/visionobject.h"
#include <list>
#include "dvision/visionutils.h"
#include <map>
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
        class IGVCBarrelObject : public VisionObject
        {
        public:
            IGVCBarrelObject()
            {
                mThreshold = 100;
                mMinSizePercent = .01;
                mMaxSizePercent = 1;
            }
            class ColorSetting
            {
            public:
                ColorSetting()
                {
                    mThreshold = 100;
                    mMinSizePercent = .01;
                    mMaxSizePercent = 1;
                }
                bool Initialize(TiXmlElement * xmlElement)
                {
                    if(xmlElement == NULL) return false;
                    TiXmlHandle handle(xmlElement);
                    TiXmlElement * element = handle.FirstChild("File").ToElement();
                    if(element)
                    {
                        if(!SetColor(element->GetText()))
                        {
                            std::cout << "ERROR IGVC Barrel - Unable to load file: " << element->GetText() << std::endl;
                            return false;
                        }

                    }
                    element = handle.FirstChild("Threshold").ToElement();
                    if(element)
                    {
                        mThreshold = atoi(element->GetText());
                    }
                    element = handle.FirstChild("SizePercent").ToElement();
                    if(element)
                    {
                        element->QueryDoubleAttribute("min",&mMinSizePercent);
                        element->QueryDoubleAttribute("max",&mMaxSizePercent);
                    }
                    return true;
                }

                bool SetColor(const std::string& fileName)
                {
                    IplImage * img = NULL;
                    img = cvLoadImage(fileName.c_str());
                    if(img)
                    {
                        mColor = (cvAvg(img));
                        mColorSampleFile = fileName;
                        cvReleaseImage(&img);
                        return true;
                    }
                    return false;
                }

                int mThreshold;
                double mMinSizePercent;
                double mMaxSizePercent;
                std::string mColorSampleFile;
                CvScalar mColor;
            };
            virtual std::list<CvBox2D> GetBounding(const IplImage * imgIn) const;

//            std::list<CvBox2D> GetBounding(const IplImage * imgIn, CvScalar color) const;
            std::list<CvBox2D> GetBounding(const IplImage * imgIn, ColorSetting color) const;
            std::list<CvBox2D> GetBounding(const IplImage * imgIn, const int r,  const int g,  const int b, const int threshold, const double minsizepercent, const double maxsizepercent) const;

            virtual IplImage * GetMask(const IplImage * imgIn) const;

            IplImage * GetMask(const IplImage * imgIn, const int r,  const int g,  const int b, const int threshold, const double minsizepercent, const double maxsizepercent) const;
//            IplImage * GetMask(const IplImage * imgIn, CvScalar color) const;
            IplImage * GetMask(const IplImage * imgIn, ColorSetting color) const;

            bool Initialize(const std::string& xmlFilename);
            bool AddColorSetting(const std::string& fileName);

            int mThreshold;
            double mMinSizePercent;
            double mMaxSizePercent;
            std::vector<ColorSetting> mColors;
        };

    }
}
#endif //__ZEBULON_IGVC_BARREL_OBJECT__H
