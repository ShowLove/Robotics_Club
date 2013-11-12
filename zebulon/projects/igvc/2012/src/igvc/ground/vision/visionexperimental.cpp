////////////////////////////////////////////////////////////////////////////////////
///
/// \file groundvision.cpp
/// \brief Functions for doing vision processing for the ground vehicle.
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
#include "igvc/ground/vision/visionexperimental.h"
#include "cxutils/math/coordinates.h"
using namespace Zebulon;
using namespace IGVC;

#define GVE_DEBUG


GroundVisionExperimental::GroundVisionExperimental()
{
}

GroundVisionExperimental::~GroundVisionExperimental()
{
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads settings
///
////////////////////////////////////////////////////////////////////////////////////
void GroundVisionExperimental::LoadSettings()
{
    const int NUMLANEIMAGES=10;

    cv::Mat laneColors[NUMLANEIMAGES];

    laneColors[0] = cvLoadImage("settings/vision/lane0.png");
    laneColors[1] = cvLoadImage("settings/vision/lane1.png");
    laneColors[2] = cvLoadImage("settings/vision/lane2.png");
    laneColors[3] = cvLoadImage("settings/vision/lane3.png");
    laneColors[4] = cvLoadImage("settings/vision/lane4.png");
    laneColors[5] = cvLoadImage("settings/vision/lane5.png");
    laneColors[6] = cvLoadImage("settings/vision/lane6.png");
    laneColors[7] = cvLoadImage("settings/vision/lane7.png");
    laneColors[8] = cvLoadImage("settings/vision/lane8.png");
    laneColors[9] = cvLoadImage("settings/vision/lane9.png");

    for (int i=0; i<NUMLANEIMAGES; i++)
    {
        cv::cvtColor(laneColors[i], laneColors[i], CV_BGR2HSV);
    }

    // quantize hue to 30 levels, sat to 32
    int vbins = 32, sbins = 32, hbins = 30;
    int histSize[] = {sbins, vbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    // value varies from 0-255
    float vranges[] = { 0, 256 };

    const float* ranges[] = { sranges, vranges };

    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {1, 2};

    cv::calcHist( laneColors, NUMLANEIMAGES, channels, cv::Mat(), // do not use mask
             mHist, 2, histSize, ranges,
             true, // the histogram is uniform
             false );
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns lane lines found in image. Origin offset from top left
///          to bottom middle(local coordinates).
///
///   \param[in] ImgIn Image to process.
///   \param[in] ImgOut Optional Output Image.
///   \param[in] rows Number of rows to split image up into before processing.
///   \param[in] cols Number of cols to split image up into before processing.
///
///   \return A list of Segment 3D objects representing each line found.  Will not
///           be limited to 2, but can vary from 0 up to number of rows x cols.
///
////////////////////////////////////////////////////////////////////////////////////
std::vector<CxUtils::Segment3D> GroundVisionExperimental::FindLaneLinesMeanShift(const cv::Mat imgIn, cv::Mat& imgOut, int rows, int cols)
{
    const int NUMWINDOWS=64;
    const int WINDOWSIZE=64;

#ifdef GVE_DEBUG
    cv::Mat debugImg(imgIn);
#endif

    // quantize hue to 30 levels, sat to 32
    int vbins = 32, sbins = 32, hbins = 30;
    int histSize[] = {sbins, vbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    // value varies from 0-255
    float vranges[] = { 0, 256 };

    const float* ranges[] = { sranges, vranges };
    // we compute the histogram from the 1-st and 2-nd channels (ignore 0th Hue channel)
    int channels[] = {1, 2};

    cv::Mat hsvIn;
    cv::cvtColor(imgIn, hsvIn, CV_BGR2HSV);

    cv::Mat projectedImg;
    cv::calcBackProject(&hsvIn,1,channels,mHist, projectedImg, ranges);

#ifdef GVE_DEBUG
    //cvShowImage("BackProject", projectedImg);
    cv::imshow("BackProject", projectedImg);
#endif


    cv::TermCriteria tc(CV_TERMCRIT_EPS, 0, 4);
    for (int i=0; i<NUMWINDOWS; i++)
    {
        int x=abs(rand()) % (projectedImg.cols-WINDOWSIZE-1);
        int y=abs(rand()) % (projectedImg.rows-WINDOWSIZE-1);

        cv::Rect window(x, y, WINDOWSIZE, WINDOWSIZE);
        cv::RotatedRect rr = cv::CamShift(projectedImg, window, tc);

#ifdef GVE_DEBUG
        if (rr.angle == rr.angle && rr.size.height == rr.size.height && rr.size.width == rr.size.width
                && rr.center.x == rr.center.x && rr.center.y == rr.center.y)
            cv::ellipse(debugImg, rr, cv::Scalar(0,0,255));
#endif
    }

#ifdef GVE_DEBUG
    cv::imshow("Object in view", debugImg);

    //cv::threshold(debugImg, debugImg,
    //cv::Mat thresh(debugImg.rows, debugImg.cols);
    cv::Mat thresh(projectedImg);
    //cv::adaptiveThreshold(projectedImg, thresh, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 3, 0);
    cv::threshold(projectedImg, thresh, 64, 255, thresh.type());
    cv::imshow("Thresh", thresh);
#endif

    std::vector<CxUtils::Segment3D> lines;
//    lines.push_back(CxUtils::Segment3D());
    return lines;
}

/* End of file */
