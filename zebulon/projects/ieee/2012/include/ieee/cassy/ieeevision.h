/////////////////////////////////////////////////////////////////////////////////////
///
/// \file ieeevision.h
/// \brief A program to test some opencv algorithms.
///
/// Author(s): Michael Scherer<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@knights.ucf.edu <br>
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

#ifndef __ZEBULON_IEEE_2012_IEEEVISIONCASSY_
#define __ZEBULON_IEEE_2012_IEEEVISIONCASSY_

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv2/video/background_segm.hpp"

#include <string>

#include "cxutils/cxutils.h"
#include "xmlconfig/xmlconfigurator.h"

#include "gvision/segment.h"
#include "gvision/threshhold.h"

#include <ieee/communication/gamepacket.h>

#define HEIGHT_OFFSET 100

struct TeamTextInfo
{
    std::string University;
    std::string Team;
    std::string CompType;

    cv::Vec3b TextColor;
    cv::Vec3b BGColor;
};

struct TextPos
{
    int LargeText;
    int MediumText;
    int SmallText;

    cv::Point University;
    cv::Point Team;
    cv::Point CompType;

    cv::Point CurrScore;
    cv::Point Correct;
    cv::Point Incorrect;
    cv::Point Percentage;
    cv::Point Laps;

    cv::Point RawTime;

    cv::Point InitTrailScore;
    cv::Point InitTrailPercent;
    cv::Point InitTrailLaps;
    cv::Point InitTrailTime;
    int TrailOffset;
};

namespace Zebulon
{
    namespace IEEE
    {
        class IEEEVisionCassy
        {
        public:
            /*IEEEVisionCassy()
            {
                mFirstTime = true;
            }*/

            IEEEVisionCassy();
            ~IEEEVisionCassy();

            bool backgroundSubtraction(const cv::Mat &input, cv::Mat &output);
            bool backgroundSubtractionCleanUp(const cv::Mat &input, cv::Mat &output);
            bool segmentation(const cv::Mat &input, cv::Mat &output);

            void AdditionalInformation(bool displayFlag);

            std::vector< cv::Point2f > TrailCalc(std::vector< cv::Point2f > prevTrail, int objectNum, int trackingTime);

            void DisplayTrail(const cv::Mat &input, cv::Mat &output, std::vector< cv::Point2f > trail);

            bool Load(const std::string& filename); // was virtual

            std::vector< Vision::Return > GetSegments();

            void ThresholdLines(const cv::Mat &input, cv::Mat &output);
            void ColorLines(const cv::Mat &input, cv::Mat &output);

            // Calls other functions
            void GenerateFinalWindow(int rawImageIndex);

            // Sets background colors and borders
            void ConditionFinalImage(cv::Mat &output);

            void ConditionTrailSpace(cv::Point initPos, int offset, cv::Size resolution, cv::Vec3b color, cv::Mat &output);
            void HighlightCurrentTrail(int currTrail, cv::Point initPos, int offset, cv::Size resolution, cv::Scalar color, cv::Mat &output);

            void AddRawImage(const cv::Mat &input, cv::Size resolution, cv::Point initPos, cv::Mat &output);
            void AddRawText(TeamTextInfo textInfo, ProcessedData processedData, TextPos textPos, cv::Mat &output);

            void AddTrail(int offAmount, int offset, ProcessedData processedData, cv::Point initPos, cv::Size origRes, cv::Size trailRes, cv::Mat &output);
            void AddTrailText(int offset, ProcessedData processedData, TextPos textPos, cv::Mat &output);
            void AddText();

            void DrawX(cv::Point pos, int length, int thickness, cv::Scalar color, cv::Mat &output);
            void DrawCheck(cv::Point pos, int length, int thickness, cv::Scalar color, cv::Mat &output);

            void DrawRawAnswers(cv::Mat &output, CourseMoves currMoveInfo, std::vector< cv::Point > taskPos);
            void DrawTrialAnswers(cv::Mat &output, int offsetAmount, int offset, CourseMoves currMoveInfo, std::vector< cv::Point > taskPos);

            void DrawWhiteLines(cv::Mat &output);

        private:
            cv::Mat mCurrentFrame;
            cv::Mat mLastFrame;
            cv::Mat mForegroundFrame;
            cv::Mat mBGImage;
            cv::Mat mFinalOutput;

            cv::Mat mSegmentOutput;

            std::vector< cv::Rect > mSegmentPoints;
            // camShift

            cv::BackgroundSubtractor mBackgroundSubtractor;
            cv::BackgroundSubtractorMOG mBackgroundSubtractorMOG;

            std::vector<cv::Point2f> mPreviousPoints;
            bool mFirstTime;
            int mFrameCount;

            // Tracking / trail
            bool mInitTrailFlag;

            // Segmentation
            Vision::Segment *mSegment;
            Vision::Threshhold *mThreshold;
            //Zebulon::Vision::Segment mSegment;

            std::vector< Vision::Return > mCurrentReturns;

            // Useful changing parameters - load from XML
            // school name
            //string mTeamName;
            // time
            std::string mUniversity;
            std::string mTeamName;
            int mTeamRank;
            int mTeamHighScore;

            double mStartTime;
            double mCurrentTime;
            double mTopScore;
            double mCurrentScore;
            int mNumVehicles;
            cv::Vec3b mTeamColorText;
            cv::Vec3b mTeamColorBG;
            // X, Y location?

            //string mFontType;
            //Zebulon::Configuration::XmlConfigurator mXml;


            CvFont mFont;
            //cv::fontQt()

            // Black out around course
            int mBOLeft;
            int mBORight;
            int mBOTop;
            int mBOBottom;

            // Always the same: Text placement
            cv::Point2i mUniversityNameTextPos;
            cv::Point2i mTeamNameTextPos;
            cv::Point2i mTimeTextPos;
            cv::Point2i mRankTextPos;
            cv::Point2i mBestScoreTextPos;
            cv::Point2i mCurrentScoreTextPos;
        };
    }
}


#endif // __ZEBULON_IEEE_2012_IEEEVISION_
/* End of file */
