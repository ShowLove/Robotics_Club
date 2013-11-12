/////////////////////////////////////////////////////////////////////////////
///
/// \file calibrate.h
/// \brief DESCRIPTION OF PROGRAM
///
/// Author(s): Cassie Puklavage<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: AUTHOREMAIL<br>
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
#ifndef __ZEBULON_UNDERWATER_Calibrate_Cassie_H
#define __ZEBULON_UNDERWATER_Calibrate_Cassie_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cxutils/mutex.h>

#include "auvc/underwater/mission.h"
#include "gvision/segment.h"
#include "gvision/threshhold.h"
#include "gvision/visionutil.h"
//#include "gvision/threshratio.h"
#include "cvision/threshgaussian.h"
#include "cvision/threshfuzzy.h"
#include "cvision/colorspace.h"
#include <vector>

#define GAUSSIAN
//#define FUZZY
//#define THRESH

#ifdef GAUSSIAN
    #define PRECISION_AMOUNT 1000
#endif
#ifdef FUZZY
    #define PRECISION_AMOUNT 1000
#endif
#ifdef THRESH
    #define PRECISION_AMOUNT 1
#endif

#define AVGWIN 2
namespace Zebulon
{
    namespace Underwater
    { 
        class CalibrateCassie : public Underwater::Mission
        {
        public:
            enum State
            {
                PutStatesHere
            };
            
            CalibrateCassie();
            ~CalibrateCassie();
            
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();
            
        private:
            State mState;
            
            void SetupCV();
            static void Foo(int event, int x, int y, int flags, void* param);
            
            IplImage *displayImage;
            IplImage *threshImage;
            IplImage *segmentImage;
            IplImage *displayCSImage;

            Vision::Threshhold *currThresh;

            Vision::Threshhold *hold;
            Vision::ThreshGaussian *gaussian;
            Vision::ThreshFuzzy *fuzzy;
            //Vision::ThreshRatio *ratio;

            Vision::Segment *seg;

            CxUtils::Mutex displayMutex;

            Vision::Target target;
            Vision::Return ret;
            
            int edge;
            int MinArea;
            int ComboDist;
            
            Vision::ColorSpace colorSpace2;
            std::vector< std::vector<int> > mixChannels;
            
            int Camera;
            
            int LoadFlag;
            int pauseFlag;
            int autoFlag;
            int firstFlag;
            int tempCS[8][3];
            
            #ifdef GAUSSIAN
            int tempRedMean, tempGreenMean, tempBlueMean;
            int tempRedVar, tempGreenVar, tempBlueVar;
            #endif
            #ifdef FUZZY
            int tempRedMean, tempGreenMean, tempBlueMean;
            int tempRedTrue, tempGreenTrue, tempBlueTrue;
            int tempRedLF, tempGreenLF, tempBlueLF;
            int tempRedRF, tempGreenRF, tempBlueRF;
            #endif
            #ifdef THRESH
            int tempRed, tempGreen, tempBlue;
            int tempRedRange, tempGreenRange, tempBlueRange;
            #endif
        };
    }
}

#endif // __ZEBULON_UNDERWATER_Calibrate_H
/* End of file */
