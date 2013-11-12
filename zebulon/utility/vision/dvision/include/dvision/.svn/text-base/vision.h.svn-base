/////////////////////////////////////////////////////////////////////////////
///
/// \file vision.h
/// \brief Class with functions for doing computer vision operations
///
/// Author(s): Michael Scherer, David Adams<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
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

#ifndef __ZEBULON_AI_UTILITY_VISION_
#define __ZEBULON_AI_UTILITY_VISION_

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cxutils/cxutils.h>
#include <gvision/visionutil.h>

#include "dvision/visualtarget.h"
#include "dvision/visionutils.h"

namespace Zebulon
{
    namespace Vision
    {
        //structure that is used for all vision algorithms
        struct VisionReturn
        {
            //I is like X 0 to width = left to right
            double CenterI;
            //J is like Y 0 to height  top to bottom
            double CenterJ;
            //Area in pixels, either pixel count or width*height of bounding box
            int Area;
            //Angle is in degrees, 0 being up
            //0 to 90 to the right "/"
            //0 to -90 to the left "\"
            double Angle;
            //Valid is an on or off (or boolean) if something is detected
            int Valid;
            //confidence in identification of target, higher is better
            double Confidence;
            enum BombTargets
            {
                bNONE=0,    //should just check valid flag
                bMACHETE,
                bAXE,
                bSHEARS,
                bHAMMER
            } BombTarget;
        };
        
        class DVision
        {
        public:
            static const std::string NotFound;
            
            static std::list<CvBox2D> FindSquares(const IplImage* img, IplImage *dest, const double minsize = .05, const double maxsize = .75, const int cannyThreshold = 50, const bool angle3d = false);
            
            static void DrawSquare(IplImage* img, const CvSeq* squares, CvScalar color= cvScalar(255,255,255), int thickness =3);
            static void DrawSquare(IplImage* img, const CvBox2D square, const CvScalar color = cvScalar(255,255,255), const int thickness = 3);
            
            static bool Calc3DSquareAngle(CvSeq*& polySquareApprox, float& angle, IplImage* dest = NULL);
            static double GetAngle(const CvPoint* pt1, const CvPoint* pt2, const CvPoint* pt0 );
            
            virtual std::list<VisionReturn> BombingObject(std::list<VisualTarget*> target,IplImage *input,IplImage *&output);
            
            static int ConvertID(std::string name)
            {
                if (name == "NULL")
                    return 0;
                std::map<std::string, int>::iterator ret;
                ret = mIDMap.find(name);
                if (ret != mIDMap.end())
                {
                    return ret->second;
                }
                else
                {
                    int size = mIDMap.size()+1;
                    mIDMap[name] = size;
                    return size;
                }
            }
            
        private:
            void InitializeVisionReturn(VisionReturn& result, const CvBox2D* box = NULL, const double confidence = 0, const int value = 0) const;
            IplImage * mFiltered;
            
            static std::map<std::string, int> mIDMap;
        };
    }
}

#endif // __ZEBULON_AI_UTILITY_VISION_
