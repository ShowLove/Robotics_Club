/////////////////////////////////////////////////////////////////////////////
///
/// \file colorspace.h
/// \brief Allow converstions between colorspaces
///
/// Author(s): Cassondra Puklavage<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
/// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
/// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
/// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
/// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
/// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
/// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
/// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
/// OF SUCH DAMAGE.
///
/////////////////////////////////////////////////////////////////////////////

#ifndef __ZEBULON_AI_UTILITY__VISION_COLORSPACE__H
#define __ZEBULON_AI_UTILITY__VISION_COLORSPACE__H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <math.h>
#include <vector>

namespace Zebulon
{
    namespace Vision
    {
        class ColorSpace
        {
            public:
                ColorSpace();
                ~ColorSpace();
                
                // Initialize memory
                void Setup(int sizeI, int sizeJ, int channelNum);
                
                // Decide which channels to manipulate, three channel image is always done
                void SetIndividualChannels(bool getChannelOne = false, bool getChannelTwo = false, bool getChannelThree = false, bool invertOne = false, bool invertTwo = false, bool invertThree = false, bool invertFull = false);
                // OpenCV colorspaces
                void Gray(IplImage *src);
                void HSV(IplImage *src);
                void HLS(IplImage *src);
                void YCrCb(IplImage *src);
                void CIEXYZ(IplImage *src);
                void CIELuv(IplImage *src);
                void CIELab(IplImage *src);
                // Our colorspaces
                void Ratio(IplImage *src);
                
                enum ColorSpaces
                {
                    GRAY_CS = 0,
                    HSV_CS,
                    HLS_CS,
                    YCrCb_CS,
                    CIEXYZ_CS,
                    CIELuv_CS,
                    CIELab_CS,
                    Ratio_CS
                };
                
                //void SetMix(IplImage *src, int gray, int hsv, int hls, int ycrcb, int xyz, int luv, int lab, int ratio);
                void SetMix(int channels);
                void Mix(IplImage *src, std::vector< std::vector<int> > mixType);
                // zero means non, one mean channel one, 
                //void Mix(IplImage *src);
                // Other
                void Invert(IplImage *src, int channelNum);
                //void Combine(IplImage *dst, IplImage *src1, IplImage *src2, IplImage *src3 = NULL, IplImage *src4 = NULL, IplImage *src5 = NULL, IplImage *src6 = NULL, IplImage *src7 = NULL);
                void Combine(std::vector<IplImage*> src, IplImage *dst);
                
                // Decide which channels to manipulate
                void SetIndividualChannelHisto(bool getChannelFull = true, bool getChannelOne = false, bool getChannelTwo = false, bool getChannelThree = false);
                // Just do the OpenCV histogram equalization
                void HistogramEqualization(IplImage *src);
                // Do OpenCV histogram equalization on 
                void HistogramEqualization();
                
                enum ImageInfo
                {
                    CHANNEL_FULL = 0,   // Three channel image
                    CHANNEL_ONE,        // Only specified channel
                    CHANNEL_TWO,        // Only specified channel
                    CHANNEL_THREE,       // Only specified channel
                    MIX_CHANNEL_ONE,        // Only specified channel
                    MIX_CHANNEL_TWO,        // Only specified channel
                    MIX_CHANNEL_THREE       // Only specified channel
                };
                
                enum Mix
                {
                    NONE = 0,
                    ONE,
                    TWO,
                    THREE,
                    ONE_TWO,
                    ONE_THREE,
                    TWO_THREE
                };
                
                void GetImage(IplImage *dest, int imageInfo);
                
                
        protected:
            
                // Where images are stored
                std::vector<IplImage*> mImageVector;
                std::vector<IplImage*> mMixedImageVector;
                std::vector< std::vector<int> > mMixType;
                
                // Grabs individual channels
                void IndividualChannels();
                
                int mChannels;
                
                bool mChannelOne;
                bool mChannelTwo;
                bool mChannelThree;
                
                bool mInvertChannelOne;
                bool mInvertChannelTwo;
                bool mInvertChannelThree;
                bool mInvertFull;
                
                bool mChannelHistoFull;
                bool mChannelHistoOne;
                bool mChannelHistoTwo;
                bool mChannelHistoThree;
            
        };
    }
}


#endif
