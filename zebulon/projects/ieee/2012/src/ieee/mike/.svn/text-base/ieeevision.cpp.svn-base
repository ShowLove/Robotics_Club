/////////////////////////////////////////////////////////////////////////////////////
///
/// \file ieeevision.cpp
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

#include "ieee/mike/ieeevision.h"

using namespace Zebulon;
using namespace IEEE;

bool IEEEVision::getObjects(const cv::Mat &input, cv::Mat &output)
{
    if (mFirstTime)
    {
        mLastFrame = input.clone();

        for (int r=0; r<input.rows; r+=10)
        {
            for (int c=0; c<input.cols; c+=10)
            {
                mPreviousPoints.push_back(cv::Point2f(c,r));
            }
        }

        mFirstTime = false;
    }

    std::vector<cv::Point2f> nextPoints;
    std::vector<uchar> status;
    std::vector<float> error;
    cv::calcOpticalFlowPyrLK(mLastFrame, input, mPreviousPoints, nextPoints, status, error);

    //mPreviousPoints = nextPoints;

    mLastFrame = input.clone();

    //output;
    cv::bitwise_and(output, cv::Scalar(0), output);
    //*output = input->clone();
    std::vector<cv::Point2f>::const_iterator iter_s = mPreviousPoints.begin();
    std::vector<cv::Point2f>::const_iterator iter;
    for (iter=nextPoints.begin(); iter!=nextPoints.end(); iter++, iter_s++)
    {
        cv::Point2f line = *iter - *iter_s;
        float mag = line.x * line.x + line.y * line.y;

        if ((iter->y > 0 && iter->y < output.rows &&
             iter->x > 0 && iter->x < output.cols) &&
                (mag < 200) && (mag > 4))
        {
            cv::line(output, *iter_s, *iter, cv::Scalar(255));
            //((uchar*)(output->data + output->step*((int)iter->y)))[(int)iter->x] = 255;
        }
    }

    return true;
}

void IEEEVision::LowPass(const cv::Mat &input, cv::Mat &output, double dt, double rc)
{
    //if (mLPFirstTime)
    if (output.cols == 0)
    {
        //output = input.clone();
        input.copyTo(output);
        //mLPFirstTime = false;
    }

    /* for i from 1 to n
     *    y[i] = y[i-1] + α * (x[i] - y[i-1])
     */
    double alpha = dt / (rc + dt);
    output += alpha*(input - output);
    //mAccumulator = output.clone();
}

/* End of file */
