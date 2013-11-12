/////////////////////////////////////////////////////////////////////////////
///
/// \file opencvsurf.h
/// \brief Allow converstions between colorspaces
///
/// Author(s): Cassondra Puklavage, Michael Scherer<br>
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

#ifndef __ZEBULON_AI_UTILITY__VISION_OPENCV_SURF__H
#define __ZEBULON_AI_UTILITY__VISION_OPENCV_SURF__H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <math.h>
#include <vector>

// shows window with object and the descriptors highlighted
//#define DRAW_OBJECT
// shows window with object of interest and the descriptor matches
// to the current image
//#define DRAW
// prints descriptor information to the screen
//#define DEBUG

namespace Zebulon
{
    namespace Vision
    {
        struct SurfReturn
        {
            /// I is like X 0 to width = left to right
            double CenterI;

            /// J is like Y 0 to height  top to bottom
            double CenterJ;

            /// Area in pixels, either pixel count or width*height of bounding box
            double Area;
            
            ///Valid is an on or off (or boolean) if something is detected
            int Valid;

            /** Angle is in degrees, 0 being up
                0 to 90 to the right "/"
                0 to -90 to the left "\" */
            double Angle;

            //confidence in identification of target, higher is better
            /*double Confidence;
            enum BombTargets
            {
                bNONE=0,    //should just check valid flag
                bMACHETE,
                bAXE,
                bSHEARS,
                bHAMMER
            } BombTarget;*/
        };
        
        class OpenCvSurf
        {
            public:
                OpenCvSurf();
                ~OpenCvSurf();
                
                // param1 and param2
                // cvSURFParams(param1, param2)
                // If param1 is increase, descriptors must be stronger (you'll get less)
                // Input grayscale images
                SurfReturn FindObject(IplImage* src, IplImage* object, double param1, double param2);

                /** Sets the minimum number of pairs required for a valid image */
                void SetMinPointPairs(int value)
                {
                    mMinPointPairs = value;
                }

                /** Gets the minimum number of pairs required for a valid image */
                int GetMinPointPairs()
                {
                    return mMinPointPairs;
                }

                static void CvtBGRtoHSGray(IplImage* src, IplImage* dest);
            protected:
            
                //SurfReturn mOutput;
                
                /*static const CvScalar mColors[] = 
                {
                    {{0,0,255}},
                    {{0,128,255}},
                    {{0,255,255}},
                    {{0,255,0}},
                    {{255,128,0}},
                    {{255,255,0}},
                    {{255,0,0}},
                    {{255,0,255}},
                    {{255,255,255}},
                    {{255, 0, 127}}
                };*/
                
                double CompareSURFDescriptors( const float* d1, const float* d2, double best, int length );

                int NaiveNearestNeighbor( const float* vec, int laplacian,
                    const CvSeq* model_keypoints,
                    const CvSeq* model_descriptors );
                      
                void FindPairs( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
                    const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, std::vector<int>& ptpairs );
                    
                int LocatePlanarObject( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
                    const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
                    const CvPoint src_corners[4], CvPoint dst_corners[4] );

                int mMinPointPairs;                     ///< Minimum number of point pairs necessary for something to not get thrown out

                /*// Initialize memory
                void Setup(int sizeX, int sizeY, int channelNum);
                
                // Decide which channels to manipulate, three channel image is always done
                void SetIndividualChannels(bool getChannelOne = false, bool getChannelTwo = false, bool getChannelThree = false);
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
                    CHANNEL_THREE       // Only specified channel
                };
                
                void GetImage(IplImage *dest, int imageInfo);
                
                
        protected:
            
                // Where images are stored
                std::vector<IplImage*> mImageVector;
                
                // Grabs individual channels
                void IndividualChannels();
                
                bool mChannels;
                
                bool mChannelOne;
                bool mChannelTwo;
                bool mChannelThree;
                
                bool mChannelHistoFull;
                bool mChannelHistoOne;
                bool mChannelHistoTwo;
                bool mChannelHistoThree;*/
            
        };
    }
}


#endif
