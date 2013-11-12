/////////////////////////////////////////////////////////////////////////////////////
///
/// \file main.cpp
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

#include "opencv/cv.h"
#include "opencv/highgui.h"


#include <cxutils/cxutils.h>

#include <stdio.h>
#include <iostream>

using namespace Zebulon;
using namespace IEEE;

int main(int argc, char** argv)
{
    std::cout << "Mike's version" << std::endl;

    cv::Mat image;

    cv::Mat out = cv::Mat(cv::Size(640,480), CV_8UC1);
    cv::Mat blur1;
    cv::Mat blur2;
    cv::Mat blurred;
    cv::Mat blurreder;

    cv::Mat in16;
    cv::Mat out16;
    cv::Mat out16_1;
    cv::Mat outP16;
    cv::Mat outP16_2;

    cv::Mat outFinal;

    IEEEVision vision;

    if (argc < 2)
    {
        std::cout << "No folder specified, loading from camera" << std::endl;

        cv::VideoCapture cap(0);
        if (!cap.isOpened())
        {
            std::cout << "Unable to open device!" << std::endl;
            return -1;
        }

        while (true)
        {
            cap >> image;
            cv::imshow("Raw", image);

            image.convertTo(in16, CV_16SC3);

            vision.LowPass(in16, out16, .1, .1);
            vision.LowPass(in16, out16_1, .1, 1);
            //cv::absdiff(out16, cv::Scalar(0,0,0), outP16);
            outP16_2 = (out16 - out16_1);
            outP16_2.convertTo(outP16, CV_8UC3);

            cv::threshold(outP16, outFinal, 15, 255, CV_THRESH_BINARY);

            cv::imshow("Blurred", outFinal);

            if ( (cvWaitKey(10) & 255) == 27 ) break;
        }
    }
    //
    else if (argc > 2)
    {
        std::cout << "Loading \"" << argv[1] << '\"' << std::endl << " with calibration from \"" << argv[2] << '\"' << std::endl;

        // "/image00000.png" 16 chars (including \0)
        char* buffer = (char*)malloc(sizeof(char) * (strlen(argv[1]) + 17));
        int imageIndex = 0;
        int framecount = 0;

        while (true)
        {
            CxUtils::Time t(true);
            sprintf(buffer, "%s/image%05d.png", argv[1], imageIndex);
            image = cv::imread(buffer);

            cv::imshow("Raw", image);
            //image.convertTo(outP16_2, CV_16SC3);
            //cv::imshow("16 bit signed", outP16_2);

            //if ((framecount++ % 1) == 0)
            {
                /*
                //cv::blur(image, blurred, cv::Size(5,5));
                cv::GaussianBlur(image, blur1, cv::Size(13,13), 2);
                cv::GaussianBlur(image, blur2, cv::Size(7,7), 1);
                blurred = blur1-blur2;

                //cv::threshold(blurred, blurreder, 30, 255, CV_THRESH_BINARY);
                //cv::threshold(blurred, blurreder, 5, 255, CV_THRESH_TOZERO);
                //cv::GaussianBlur(image, blurreder, cv::Size(13,13), 2);
                cv::imshow("Blurred", blurreder);   //*/

                /*
                vision.LowPass(image, blurred, 1, 100);

                cv::imshow("Blurred", blurred);
                //*/
                //*
                image.convertTo(in16, CV_16SC3);

                vision.LowPass(in16, out16, .1, 1);
                vision.LowPass(in16, out16_1, .1, 2);
                //cv::absdiff(out16, cv::Scalar(0,0,0), outP16);
                outP16_2 = (out16 - out16_1);
                outP16_2.convertTo(outP16, CV_8UC3);

                cv::threshold(outP16, outFinal, 15, 255, CV_THRESH_BINARY);

                cv::imshow("Blurred", outFinal);     //*/

                //vision.getObjects(blurred, out);
            }

            //cv::imshow("Out", out);

            std::cout << 1/(CxUtils::Time(true) - t) << std::endl;

            //delete image;
            imageIndex = (imageIndex + 1) % 1582; // number picked for now, need to change
            // Escape key pressed
            if ( (cvWaitKey(10) & 255) == 27 ) break;
        }

        //cv::imread()
    }
}

/* End of file */
