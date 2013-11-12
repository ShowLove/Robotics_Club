/////////////////////////////////////////////////////////////////////////////////////
///
/// \file example_visionexperimental.h
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

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cxutils/cxutils.h>
#include <vector>

#include "ground/vision/visionexperimental.h"

//#include "ground/vision/homography.h"
#include <homography/homography.h>

#include <pipetree/tree.h>
#include <pipetree/branch.hpp>

cv::Mat gImageIn;// = NULL;
cv::Mat gImageOut;// = NULL;

//#define SURF_OPTICAL_FLOW
//#define GLOBAL_MOTION_EST
//#define GLOBAL_MOTION_EST_AFFINE

using namespace Zebulon;

int main(int argc, char** argv)
{
    bool first=true;

    if (argc < 2)
    {
        std::cerr << "usage: example_visionexperimental <filepath>" << std::endl;
        return -1;
    }

    // Files
    std::vector<std::string> files;
    CxUtils::FileIO::GetFiles(files,"*.png",argv[1],false);
    std::sort(files.begin(), files.end());

    cv::Mat frame;
    IplImage iplImage;
    IplImage* homoOut = NULL;

    cv::Mat homoFrame;
    cv::Mat prevFrame;
    cv::Mat outFrame;

    // Homography
    Vision::Homography homo;
    homo.LoadSettings();

    // SURF
    std::vector<cv::KeyPoint> keypoints;
    cv::SurfFeatureDetector surf;
    std::vector<cv::Point2f> prePoints;
    std::vector<cv::Point2f> newPoints;
    std::vector<uchar> status;
    std::vector<float> error;

    cv::Rect rect;
    rect.x = 310;
    rect.y = 310;
    rect.width = 420-310;
    rect.height = 450-310;

    for (int i=80; i<files.size(); i++)
    {
        std::cout << files[i] << std::endl;
        frame = cv::imread(files[i]);

        //cv::imshow("Raw", frame);

#ifdef GLOBAL_MOTION_EST
        if (prevFrame.empty())
        {
            frame.copyTo(prevFrame);
            continue;
        }


#endif

#ifdef GLOBAL_MOTION_EST_AFFINE
        iplImage = frame;
        homoFrame = homo.Warp(&iplImage);

        if (prevFrame.empty())
        {
            //prevFrame = homoOut;
            homoFrame.copyTo(prevFrame);
            continue;
        }

        cv::Mat est = cv::estimateRigidTransform(prevFrame, homoFrame, true);

        /*
        float a[] = {0.0f, -10.0f, 0.0f};
        cv::Mat vec(3, 1, CV_32FC1, a);

        cv::Mat out;
        cv::gemm(est, vec, 1, cv::Mat(), 0, out);

        cv::Point2d center;
        center.x = homoFrame.cols/2;
        center.y = homoFrame.rows/2;

        cv::Point2d offset;
        offset.x = center.x + out.at<float>(0,0);
        offset.y = center.y + out.at<float>(0,1);

        homoFrame.copyTo(outFrame);
        cv::line(outFrame, center, offset, cv::Scalar(0,0,255), 2);//*/

        //cv::transform(prevFrame, outFrame, est);
        cv::warpAffine(prevFrame, outFrame, est, prevFrame.size());

        cv::imshow("t1", homoFrame);
        cv::imshow("t0", prevFrame);
        cv::imshow("t1*", outFrame);

        cv::imshow("Diff", outFrame - homoFrame);

        prevFrame = homoFrame;
#endif


#ifdef SURF_OPTICAL_FLOW
        iplImage = frame;
        homoOut = homo.Warp(&iplImage);

        homoFrame = homoOut;

        surf.detect(homoFrame, keypoints);

        prePoints.clear();
        std::vector<cv::KeyPoint>::iterator iter;
        for (iter=keypoints.begin(); iter!=keypoints.end(); iter++)
        {
            //cv::circle(homoFrame, iter->pt, iter->size/10.0, cv::Scalar(255,0,0));
            /*if (((uchar *)(homoFrame.data + iter->pt.x*homoFrame.step))[iter->pt.y*homoFrame.channels() + 0] > 0 ||
                ((uchar *)(homoFrame.data + iter->pt.x*homoFrame.step))[iter->pt.y*homoFrame.channels() + 1] > 0 ||
                ((uchar *)(homoFrame.data + iter->pt.x*homoFrame.step))[iter->pt.y*homoFrame.channels() + 2] > 0)//*/
            /*cv::Point2d pt;
            pt.x = (int)iter->pt.x;
            pt.y = (int)iter->pt.y;
            cv::Vec3b v = homoFrame.at<cv::Vec3b>(pt);
            if (v[0] > 0 || v[1] > 0 || v[2] > 0)//*/

            if (rect.contains(iter->pt))
            {
                prePoints.push_back(iter->pt);
            }
        }
        /*newPoints.reserve(prePoints.capacity());
        status.reserve(prePoints.capacity());
        error.reserve(prePoints.capacity());//*/

        if (first)
        {
            prevFrame = homoFrame.clone();
            first=false;
        }

        cv::calcOpticalFlowPyrLK(prevFrame, homoFrame, prePoints, newPoints, status, error);

        cv::Point2d pt;
        cv::Vec3b v;
        int i=0;
        std::vector<cv::Point2f>::iterator ip=prePoints.begin();
        std::vector<cv::Point2f>::iterator in=newPoints.begin();
        std::vector<uchar>::iterator is = status.begin();

        std::vector<cv::Vec2f> vectors;

        for (; ip!=prePoints.end() && in!=newPoints.end() && is!=status.end(); ip++, in++, is++, i++)
        {
            if (*is)
            {
                pt.x = (int)in->x;
                pt.y = (int)in->y;
                v = homoFrame.at<cv::Vec3b>(pt);

                //if (v[0] > 0 || v[1] > 0 || v[2] > 0)
                if(rect.contains(*in))
                {
                    cv::line(homoFrame, *ip, *in, cv::Scalar(0,0,255));

                    cv::Vec2f vec;
                    vec[0] = in->x - ip->x;
                    vec[1] = in->y - ip->y;
                    vectors.push_back(vec);
                }
            }
        }

        i=0;
        cv::Vec2f globalVector;
        std::vector<cv::Vec2f>::const_iterator iv;
        for (iv=vectors.begin(); iv!=vectors.end(); iv++, i++)
        {
            globalVector += *iv;
        }
        globalVector[0] = globalVector[0] / (float)i;
        globalVector[1] = globalVector[1] / (float)i;

        cv::Point2d center;
        center.x = homoFrame.cols/2;
        center.y = homoFrame.rows/2;

        cv::Point2d offset;
        offset.x = center.x - (globalVector[0]*4);
        offset.y = center.y - (globalVector[1]*4);

        cv::line(homoFrame, center, offset, cv::Scalar(0,255,255), 2);

        cv::rectangle(homoFrame, rect, cv::Scalar(255,255,0));

        cv::imshow("Homo", homoFrame);

        prevFrame = homoFrame.clone();
#endif

        if ((cv::waitKey() & 255) == 27)
           break;
    }

    /*CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
    if ( !capture ) {
        fprintf( stderr, "ERROR: capture is NULL \n" );
        getchar();
        return -1;
    }
    // Create a window in which the captured images will be presented
    cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );
    // Show the image captured from the camera in the window and repeat
    char strbuffer[25];
    int num=0;
    while ( 1 )
    {
        // Get one frame
        IplImage* frame = cvQueryFrame( capture );
        if ( !frame )
        {
            fprintf( stderr, "ERROR: frame is null...\n" );
            getchar();
            break;
        }
        cvShowImage( "mywindow", frame );
        sprintf(strbuffer, "/media/Cassie/IEEE/free/image%05d.png", num++);
        cvSaveImage(strbuffer, frame);


        // Do not release the frame!
        //If ESC key pressed, Key=0x10001B under OpenCV 0.9.7(linux version),
        //remove higher bits using AND operator
        if ( (cvWaitKey(10) & 255) == 27 ) break;
    }
    // Release the capture device housekeeping
    cvReleaseCapture( &capture );
    cvDestroyWindow( "mywindow" );
    return 0;//*/

    /*


    int key = 0;
    if(argc < 2)
    {
        printf("must specify file\nExiting..");
        return -1;
    }

    IGVC::GroundVisionExperimental gve;
    gve.LoadSettings();

    cvNamedWindow("Perspective", CV_WINDOW_AUTOSIZE );
    cvNamedWindow("Input", CV_WINDOW_AUTOSIZE );
    //cvNamedWindow("Output", CV_WINDOW_AUTOSIZE);
    std::vector<std::string> files;
    CxUtils::FileIO::GetFiles(files,"*.png",argv[1],false);

    for(int i = 1; i < files.size(); i++)
    {
        printf("%s:\n",files[i].c_str());

        if( (gImageIn = cv::imread(files[i])).empty() )
        {
            printf("unable to load file \n");
            continue;
        }

        cv::imshow("Input", gImageIn);
        key = cvWaitKey(0);

        gve.FindLaneLinesMeanShift(gImageIn, gImageOut);

        //cv::imshow("Output", gImageOut);
        key = cvWaitKey(0);


        //cvReleaseImage(&gImageIn);
        //cvReleaseImage(&gImageOut);

        if(key == 1048603)
            break;
        
    }
    return 0;*/
}
