//////////////////////////////////////////////////////////////////////////////////////////
///
/// \file depthmap.cpp
/// \brief Class for creating a depth map image from lidar data.
///
/// Author(s): Jonathan Mohlenhoff, Tuyen Do<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: jmohlenh@ist.ucf.edu<br>
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
//////////////////////////////////////////////////////////////////////////////////////////
#include "asvc/surface/depthmap.h"
#include <iostream>

using namespace Zebulon;
using namespace ASVC;

DepthMap::DepthMap()
{
    mpDepthData = mpDepthImage = NULL;

    mRmat = cvCreateMat(3, 3, CV_64F);
    mTvec = cvCreateMat(1, 3, CV_64F);
    mRvec = cvCreateMat(1, 3, CV_64F);
    cvZero(mRmat);
    cvZero(mTvec);
    cvZero(mRvec);
    mLaserDataFlag = false;
    //mMaxDynamixelDegrees = 141; // Tilt down
    //mMinDynamixelDegrees = 110; // Tilt up

    mMaxDynamixelDegrees = 141; // Tilt down
    mMinDynamixelDegrees = 110; // Tilt up

    mDynamixelActualDegrees = -5000;
    mCommandedAngle = 126; //Start angle for Hokuyo
    mTick = 1.0;

    mpGlobalCommand = NULL;
    mpGlobalInfo = NULL;

    mMax = 15.0;
    mMin = 0.2;
} // default constructor

DepthMap::~DepthMap()
{
    cvReleaseImage(&mpDepthData);
    cvReleaseImage(&mpDepthImage);
    cvReleaseMat(&mIntrinsicMatrix);
    cvReleaseMat(&mDistortionCoeffs);
    cvReleaseMat(&mRmat);
    cvReleaseMat(&mTvec);
    cvReleaseMat(&mRvec);
} // destructor

void DepthMap::Initialize(const std::string intrFilename,
                          const std::string distFilename,
                          int width, int height,
                          GlobalCommand* pGlobalCommand,
                          GlobalInfo* pGlobalInfo)
{
    mHeight = height;
    mWidth = width;
    mpDepthData = cvCreateImage(cvSize(mWidth, mHeight), IPL_DEPTH_32F, 1); //Create initial depth image
    cvSet(mpDepthData, cvScalar(0));
    
    mpDepthImage = cvCreateImage(cvSize(mpDepthData->width, mpDepthData->height), IPL_DEPTH_8U, 1);
    cvZero(mpDepthImage);

    mpGlobalCommand = pGlobalCommand;
    mpGlobalInfo = pGlobalInfo;

    mIntrinsicMatrix = (CvMat*)cvLoad(intrFilename.c_str());
    mDistortionCoeffs = (CvMat*)cvLoad(distFilename.c_str());

    if (mIntrinsicMatrix == NULL || mDistortionCoeffs == NULL)
    {
        std::cout << "Could not find file." << std::endl;
        return;
    }
    /*
    for(int i=0; i<mIntrinsicMatrix->cols; i++)
    {
        for(int j=0; j<mIntrinsicMatrix->rows; j++)
        {
            printf("%lf, ", CV_MAT_ELEM(*mIntrinsicMatrix, double, i, j));
        }
        printf("\n");
    }
    */
    // Sets the translation vector in meters
    CV_MAT_ELEM(*mTvec, double, 0, 0) =  0.0095f;
    CV_MAT_ELEM(*mTvec, double, 0, 1) = -0.1016f;
    CV_MAT_ELEM(*mTvec, double, 0, 2) = -0.0254f;

    // Initializes the rotation vector
    CV_MAT_ELEM(*mRmat, double, 0, 0) =  1;
    CV_MAT_ELEM(*mRmat, double, 0, 1) =  0;
    CV_MAT_ELEM(*mRmat, double, 0, 2) =  0;
    CV_MAT_ELEM(*mRmat, double, 1, 0) =  0;
    CV_MAT_ELEM(*mRmat, double, 1, 1) =  0;
    CV_MAT_ELEM(*mRmat, double, 1, 2) =  0;
    CV_MAT_ELEM(*mRmat, double, 2, 0) =  0;
    CV_MAT_ELEM(*mRmat, double, 2, 1) =  0;
    CV_MAT_ELEM(*mRmat, double, 2, 2) =  0;

    mThread_ProcessLaserData.CreateThread(DepthMap::ProcessLaserDataThread, this);
} // Initialize method

void DepthMap::ProcessData(const std::string& name, const CxUtils::Point3D::List& value)
{
    CxUtils::Mutex::ScopedLock lock(&mMutex1);
    mLaserDataBuffer.points = value;
    mLaserDataBuffer.angle = mCurrentAngle;
    mLaserDataFlag = true;
} // ProcessData method

void DepthMap::ProcessData(const std::string& name, const double value)
{
    if(name == Microcontroller::DYNAMIXELACTUAL)
    {
        CxUtils::Mutex::ScopedLock lock(&mMutex1);
        mCurrentAngle = -(value - CxUtils::CxToRadians(126)); // Offset of the Dynamixel
        mDynamixelActualDegrees = CxUtils::CxToDegrees(value);
    }
} // ProcessData method

CxUtils::Point3D DepthMap::ConvertPixelToCoordinates(double row, double col, double depth)
{
    double fx, fy, cx, cy;
    fx = 1.0/CV_MAT_ELEM(*mIntrinsicMatrix, double, 0, 0);
    fy = 1.0/CV_MAT_ELEM(*mIntrinsicMatrix, double, 1, 1);
    cx = CV_MAT_ELEM(*mIntrinsicMatrix, double, 0, 2);
    cy = CV_MAT_ELEM(*mIntrinsicMatrix, double, 1, 2);

    double x, y, z;
    x = depth;
    y = (col - cx)*depth*fx;
    z = ((row - cy)*depth*fy);

    return CxUtils::Point3D(x, y, z);
}
void DepthMap::ToGrayImage(IplImage * inimg, IplImage*& outimg)
{
    if(inimg == NULL) return;
    if(outimg == NULL)  outimg = cvCreateImage(cvSize(mpDepthData->width, mpDepthData->height), IPL_DEPTH_8U, 1);

    unsigned char* out = (unsigned char*)outimg->imageData;

    float* in = (float *)(inimg->imageData);
    for(unsigned int i = 0; i < outimg->width*outimg->height; i++)
    {
        double val = *in;
        if(val > mMax)
            val = mMax;
        if(val < mMin)
            val = 0;

        *out = (unsigned char)(255 - val*255/mMax);
        if(val == 0)
        {
            *out = 0;
        }
        in++;
        out++;
    }

}

typedef std::pair<int, double> Depth; // Row and Depth

void DepthMap::ProcessLaserDataThread(void* ptr)
{
    DepthMap* dm = (DepthMap*)ptr;

    int i = 0;  // indexing variable
    double listAngle = 0; // angle of the incoming data
    bool flag = false; // new data flag
    int listSize = 0; // local copy of the size of the incoming list
    double rotateAngle = 0; // the angle to rotate the data points.

    CxUtils::SleepMs(3000);
    CxUtils::Time::Stamp drawTimeMs = 0;
    CxUtils::Time::Stamp drawIntervalMs = 250;

    const double cameraOffsetAngle  = -(CxUtils::CxToRadians(10)); // camera offset angle

    CxUtils::Point3D::List pointList;
    CvPoint center;

    double totalTimeSeconds = 0;

    int minRow = 999999999;
    int maxRow = -1;

    double threshold = 3;
    bool first = true;
    double prevCommandedAngle = 0;

    dm->mCommandedAngle = dm->mMaxDynamixelDegrees;
    std::vector<Depth> prevDepthValues;
    std::vector<Depth> curDepthValues;

    while (!dm->mThread_ProcessLaserData.QuitThreadFlag())
    {
        double currentAngleDegrees;

        { // Lock
            CxUtils::Mutex::ScopedLock lock(&dm->mMutex1);
            flag = dm->mLaserDataFlag;
            currentAngleDegrees = dm->mDynamixelActualDegrees;

        } // Unlock

        // Command Dynamixel to next angle

        if(fabs(dm->mCommandedAngle - currentAngleDegrees) <= threshold) // Tilted down
        {
            // Tilt up now
            dm->mCommandedAngle = dm->mMinDynamixelDegrees;

            // Clear bottom of image
            if(maxRow <= dm->mHeight - 1)
            {
                cvDrawRect(dm->mpDepthData,
                           cvPoint(0, maxRow), cvPoint(dm->mWidth - 1, dm->mHeight - 1),
                           cvScalar(dm->mMax, dm->mMax, dm->mMax), CV_FILLED);
            }
            //cvSmooth(dm->mpDepthData, dm->mpDepthData, CV_GAUSSIAN, 3);
            // Reset max
            maxRow = -1;
        }
        if(fabs(dm->mCommandedAngle - currentAngleDegrees) <= threshold) // Tillted up
        {
            dm->mCommandedAngle = dm->mMaxDynamixelDegrees;
            // Clear top of image
            if(minRow < dm->mHeight - 1 && minRow >= 0)
            {

                cvDrawRect(dm->mpDepthData,
                           cvPoint(0, 0), cvPoint(dm->mWidth - 1, minRow),
                           cvScalar(dm->mMax, dm->mMax, dm->mMax), CV_FILLED);

            }
            //cvSmooth(dm->mpDepthData, dm->mpDepthData, CV_GAUSSIAN, 3);
            // Reset min
            minRow = 999999999;
        }

        dm->mpGlobalCommand->SetDynamixelDegrees(dm->mCommandedAngle);

        if (flag)
        {
            double startTimeSeconds = CxUtils::GetTimeSeconds();
            // Get data

            {
                // Lock
                CxUtils::Mutex::ScopedLock lock(&dm->mMutex1);
                pointList = dm->mLaserDataBuffer.points;
                listAngle = dm->mLaserDataBuffer.angle;
                if(first == true)
                {
                    dm->mCommandedAngle = dm->mMaxDynamixelDegrees;
                    if(dm->mDynamixelActualDegrees < -1000)
                    {
                        continue;
                    }
                    first = false;
                }

            }   // Unlock

            // Angle to rotate data points clockwise. Camera offset + current angle of the incoming data
            rotateAngle = -(cameraOffsetAngle - listAngle);

            /* Ry(theta) = [ cos(theta)    0   sin(theta) ]
             *             [ 0             1   0          ]
             *             [ -sin(theta)   0   cos(theta) ]
             */

            // Sets the rotation vector
            CV_MAT_ELEM(*dm->mRmat, double, 1, 1) =  cos(rotateAngle);
            CV_MAT_ELEM(*dm->mRmat, double, 1, 2) =  -sin(rotateAngle);
            CV_MAT_ELEM(*dm->mRmat, double, 2, 1) =  sin(rotateAngle);
            CV_MAT_ELEM(*dm->mRmat, double, 2, 2) =  cos(rotateAngle);

            cvRodrigues2(dm->mRmat, dm->mRvec); // gets the Rodrigues

            // Processing Data
            listSize = pointList.size();
            dm->mObjectPoints = cvCreateMat(listSize, 3, CV_64F);
            dm->mImagePoints = cvCreateMat(listSize, 2, CV_64F);

            CxUtils::Point3D::List::iterator iter;
            i = 0;

            for (iter = pointList.begin(); iter != pointList.end(); iter++, i++)
            {
                CV_MAT_ELEM(*dm->mObjectPoints, double, i, 0) = iter->mY; // becomes Y
                CV_MAT_ELEM(*dm->mObjectPoints, double, i, 1) = iter->mZ; // becomes Z
                CV_MAT_ELEM(*dm->mObjectPoints, double, i, 2) = iter->mX; // becomes X
            } // for

            /*
            printf("mObjectPoints:\n");
            for (i = 0; i < listSize; i++)
            {
                printf("%f, %f, %f\n",
                       CV_MAT_ELEM(*dm->mObjectPoints, double, i, 0),
                       CV_MAT_ELEM(*dm->mObjectPoints, double, i, 1),
                       CV_MAT_ELEM(*dm->mObjectPoints, double, i, 2));
            }
            */

            // Projects points using a rotation (mRvec) as well as a translation )mTvec).
            // Output is stored with 6th parameter (mImagePoints).
            cvProjectPoints2(dm->mObjectPoints, dm->mRvec, dm->mTvec, dm->mIntrinsicMatrix,
                             dm->mDistortionCoeffs, dm->mImagePoints);

            /*
            printf("\nmImagePoints:\n");
            for (i = 0; i < listSize; i++)
            {
                printf("%f, %f\n",
                       CV_MAT_ELEM(*dm->mImagePoints, double, i, 0),
                       CV_MAT_ELEM(*dm->mImagePoints, double, i, 1));
            }
            */

            i = 0;
            float distance;
            int tall;
            CvPoint point1, point2;
            unsigned char color;

            curDepthValues.clear();
            int prevX = 0;
            double prevDistance = dm->mMax;
            CvPoint prevPoint = cvPoint(0, 0);
            bool first = true;
            for (iter = pointList.begin(); iter != pointList.end(); iter++, i++)
            {
                center.x = cvRound(CV_MAT_ELEM(*dm->mImagePoints, double, i, 0));
                center.y = cvRound(CV_MAT_ELEM(*dm->mImagePoints, double, i, 1));

                if(first)
                {
                    prevPoint = center;
                }
                if((center.x >= 0 && center.x < dm->mWidth) && (center.y >= 0 && center.y < dm->mHeight))
                {
                    if(center.y < minRow)
                    {
                        minRow = center.y;
                    }
                    if(center.y > maxRow)
                    {
                        maxRow = center.y;
                    }
                    distance = sqrt(iter->mX * iter->mX + iter->mY * iter->mY + iter->mZ * iter->mZ);
                    if(distance > dm->mMax)
                        distance = dm->mMax;
                    //std::cout << distance << std::endl;
                    //CV_IMAGE_ELEM(dm->mDepthImage, float, (int)center.y, (int)center.x) = distance;
                    //cvCircle(dm->mDepthImage, center, 1, (distance), 3);
                    //tall = cvRound(distance/15.0*15.0);


                    tall = 7;
                    tall = 7*(distance/(dm->mMax - dm->mMin));
                    if(tall < 4)
                    {
                        tall = 4;
                    }
                    point1 = center;
                    point1.y -= tall;
                    point2 = center;
                    point2.y += tall;
                    cvLine(dm->mpDepthData, point1, point2, cvScalar(distance), 2);

                    //cvLine(dm->mpDepthData, prevPoint, center, cvScalar((prevDistance + distance)/2.0), 1);
                    //cvCircle(dm->mpDepthData, prevPoint, 2, cvScalar(prevDistance), -1);
                    cvCircle(dm->mpDepthData, center, 1, cvScalar(distance), -1);
                    prevPoint = center;
                    prevDistance = distance;
                    /*
                    if(prevX == 0)
                    {
                        prevDistance = distance;
                    }
                    double slope = (distance - prevDistance)/(center.x - prevX);
                    for(int c = 0; c <= center.x - prevX; c++)
                    {
                        double val = prevDistance + c*slope;
                        if(c >= 0 && c < dm->mWidth)
                        {
                            cvCircle(dm->mpDepthData, cvPoint(c, center.y), 1, cvScalar(val), -1);
                        }

                    }
                    prevX = center.x;
                    prevDistance = distance;
                    */
                    curDepthValues.push_back(Depth(center.y, distance));

                }
            }
            cvSmooth(dm->mpDepthData, dm->mpDepthData, CV_MEDIAN, 5);
            //cvSmooth(dm->mpDepthData, dm->mpDepthData, CV_GAUSSIAN, 3);
            dm->ToGrayImage(dm->mpDepthData,dm->mpDepthImage);
//            unsigned char* out = (unsigned char*)dm->mpDepthImage->imageData;
//            float* in = (float *)dm->mpDepthData->imageData;
//            for(unsigned int i = 0; i < dm->mpDepthImage->width*dm->mpDepthImage->height; i++)
//            {
//                double val = *in;
//                if(val > dm->mMax)
//                    val = dm->mMax;
//                if(val < dm->mMin)
//                    val = 0;

//                *out = (unsigned char)(255 - val*255/dm->mMax);
//                if(val == 0)
//                {
//                    *out = 0;
//                }
//                in++;
//                out++;
//            }

            dm->mpGlobalInfo->SetInfo(GlobalInfo::DepthDataMainCamera, dm->mpDepthData);
            dm->mpGlobalInfo->SetInfo(GlobalInfo::DepthImageMainCamera, dm->mpDepthImage);

            /*
            if(CxUtils::GetTimeMs() - drawTimeMs >= drawIntervalMs)
            {
                //Show image
                Utility::Display::DisplayImage("Depth Image", dm->mDepthImage);

                drawTimeMs = CxUtils::GetTimeMs();

                std::cout << "Time to Draw: " << CxUtils::GetTimeSeconds() - startTimeSeconds << std::endl;
            }
            */
            cvReleaseMat(&dm->mObjectPoints);
            cvReleaseMat(&dm->mImagePoints);

            { // Lock
                CxUtils::Mutex::ScopedLock lock(&dm->mMutex1);
                dm->mLaserDataFlag = false;
            } // Unlock

            double endTimeSeconds = CxUtils::GetTimeSeconds();
            totalTimeSeconds = endTimeSeconds - startTimeSeconds;
            //std::cout << "DYNAMIXEL UPDATE TIME: " << std::fixed << std::setprecision(8) << totalTimeSeconds << std::endl;
        } // if

        CxUtils::SleepMs(1);
    } // while

} // CopyLaserData method

/* END OF FILE */

