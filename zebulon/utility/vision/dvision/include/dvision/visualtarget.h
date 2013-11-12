/////////////////////////////////////////////////////////////////////////////
///
/// \file visualtarget.h
/// \brief Target for dropper bins
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

#ifndef __ZEBULON_AI_UTILITY_VISIUALTARGET_
#define __ZEBULON_AI_UTILITY_VISIUALTARGET_

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cxutils/cxutils.h>
#include <dvision/visionutils.h>
#include <xmlconfig/xmlconfigurator.h>

namespace Zebulon
{
    namespace Vision
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class VisualTarget
        ///   \brief Class with useful computer vision functions
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class VisualTarget
        {
            public:
                struct Color
                {
                    unsigned int red;
                    unsigned int blue;
                    unsigned int green;
                };
                VisualTarget();
                VisualTarget(const std::string id);
                ~VisualTarget();
                virtual bool Load(const std::string& filename);
                
                void SetID(const std::string id);
                void SetColor(const unsigned int r, const unsigned int g, const unsigned int b);
                void SetThreshold(const unsigned int r, const unsigned int g, const unsigned int b);
                unsigned int Red() const { return mColor.red;}
                unsigned int Green() const { return mColor.green;}
                unsigned int Blue() const { return mColor.blue;}
                unsigned int ThresholdRed() const { return mThreshold.red;}
                unsigned int ThresholdGreen() const { return mThreshold.green;}
                unsigned int ThresholdBlue() const { return mThreshold.blue;}
                
                std::string ToString();
                std::string mID;
                std::string mTemplateFilename;
                double mSize;
                int mDilate;
                int mThresholdAll;
                double mMinSize;
                double mMinConfidence;
                IplImage * mTemplate;
                
                unsigned int mAlgorithm;
            protected:
                Color mColor;
                Color mThreshold;
        
        };
    }
}

#endif // __ZEBULON_AI_UTILITY_VISIUALTARGET_
/* End of file */
