////////////////////////////////////////////////////////////////////////////////////
///
/// \file main.cpp
///
/// Author(s): Michael Scherer<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Email: michael.scherer@ieee.org<br>
/// Web: http://zebulon.svn.sourceforge.net/viewvc/zebulon/v4/software/utility/threading/pipetree/<br>
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

#include <iostream>
#include <typeinfo>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <pipetree/pipetree.h>

using namespace PipeTree;

class Resize : public Branch
{
public:
    static const std::string ArgImage;

    Resize(double factor) : mFactor(factor)
    {
        AddArgument(ArgImage);
    }

    virtual cv::Mat Process() const
    {
        cv::Mat resized;
        const cv::Mat input = Get(ArgImage);

        cv::resize(input, resized, cv::Size(input.cols*mFactor, input.rows*mFactor));

        return resized;
    }

protected:
    double mFactor;
};

const std::string Resize::ArgImage = "Image";

class Gaussian : public Branch
{
public:
    static const std::string ArgImage;

    Gaussian(double sigma) : mSigma(sigma)
    {
        AddArgument(ArgImage);
    }

    virtual cv::Mat Process() const
    {
        cv::Mat result;
        const cv::Mat input = Get(ArgImage);

        cv::GaussianBlur(input, result, cv::Size(2*mSigma+1, 2*mSigma+1), mSigma);

        return result;
    }

protected:
    double mSigma;
};

const std::string Gaussian::ArgImage = "Image";

class Difference : public Branch
{
public:
    static const std::string Image1;
    static const std::string Image2;

    Difference()
    {
        AddArgument(Image1);
        AddArgument(Image2);
    }

    virtual cv::Mat Process() const
    {
        cv::Mat result;
        const cv::Mat input1 = Get(Image1);
        const cv::Mat input2 = Get(Image2);

        result = input1 - input2;

        return result;
    }

protected:
};

const std::string Difference::Image1 = "Image1";
const std::string Difference::Image2 = "Image2";

class Hough : public Branch
{
public:
    static const std::string ArgImage;

    Hough()
    {
        AddArgument(ArgImage);
    }

    virtual cv::Mat Process() const
    {
        const cv::Mat input = Get(ArgImage);
        cv::Mat result = cv::Mat(input.rows, input.cols, CV_8UC3);

        cv::Mat gray;
        cv::cvtColor(input, gray, CV_BGR2GRAY);

        cv::Mat thresh;
        cv::threshold(gray, thresh, 25, 255, CV_THRESH_BINARY);

        std::vector<cv::Vec2f> lines;
        cv::HoughLines(thresh, lines, 1, CV_PI/180, 100, 0, 0 );
        for( size_t i = 0; i < lines.size(); i++ )
        {
            float rho = lines[i][0], theta = lines[i][1];
            cv::Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            cv::line( result, pt1, pt2, cv::Scalar(0,0,255), 1, CV_AA);
        }


        return result;
    }

protected:

};

const std::string Hough::ArgImage = "Image1";//*/

/*class Display : public Branch
{
public:
    Display(int number) : mNumDisplays(number)
    {
        char buffer[10];
        for (int i=0; i<number; i++)
        {
            sprintf(buffer, "%d", i);
            AddArgument(buffer);
        }
    }

    virtual cv::Mat Process() const
    {
        char buffer[10];
        for (int i=0; i<mNumDisplays; i++)
        {
            sprintf(buffer, "%d", i);
            const cv::Mat image = Get(buffer);
            cv::imshow(buffer, image);
        }

        cv::waitKey(0);
        return cv::Mat();
    }

protected:
    //Lockfree::Queue<cv::Mat> mArguments;
    int mNumDisplays;
};//*/



void wait(int millis)
{
    pthread_mutex_t fakeMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t fakeCond = PTHREAD_COND_INITIALIZER;

    struct timespec timeToWait;
    struct timeval now;
    int rt;

    gettimeofday(&now,NULL);

    int secs = (now.tv_usec/1000 + millis)/1000;
    timeToWait.tv_sec = now.tv_sec + secs;
    timeToWait.tv_nsec = now.tv_usec*1000 + ((now.tv_usec/1000 + millis) % 1000) *1000;

    pthread_mutex_lock(&fakeMutex);
    rt = pthread_cond_timedwait(&fakeCond, &fakeMutex, &timeToWait);
    pthread_mutex_unlock(&fakeMutex);
}

cv::Mat HoughTrans(const cv::Mat& input)
{
    cv::Mat result = cv::Mat(input.rows, input.cols, CV_8UC3);

    cv::Mat gray;
    cv::cvtColor(input, gray, CV_BGR2GRAY);

    cv::Mat thresh;
    cv::threshold(gray, thresh, 25, 255, CV_THRESH_BINARY);

    std::vector<cv::Vec2f> lines;
    cv::HoughLines(thresh, lines, 1, CV_PI/180, 100, 0, 0 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        cv::line( result, pt1, pt2, cv::Scalar(0,0,255), 1, CV_AA);
    }


    return result;
}

class MyCallback : public Terminal::TreeCallback
{
public:
    Lockfree::Queue<long long int> mRuntimes;

    boost::posix_time::ptime mStartTime;
    volatile bool complete;
    cv::Mat mResult;

    PipeTree::Tree mTree;

    MyCallback()
    {
        complete = false;
    }

    void ProcessComplete(const cv::Mat& result, void* user)
    {
        result.copyTo(mResult);

        __sync_synchronize();
        boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration msdiff = mst1 - mStartTime;
        //std::cout << "Graph execution time ( BMP-RAP ): " << msdiff.total_milliseconds() << " ms" << std::endl;
        mRuntimes.push_back(msdiff.total_milliseconds());

        //complete = true;
        //__sync_synchronize();

        //while(!__sync_bool_compare_and_swap(&complete, false, true));

        if ((*static_cast<int*>(user))++ < 100)
        {
            mStartTime = boost::posix_time::microsec_clock::local_time();
            __sync_synchronize();
            mTree.SetJob(cv::imread("test.jpg"));
        }
        else
        {
            complete = true;
            __sync_synchronize();
        }
    }
};

int main()
{
    std::cout << "BMP-RAP Example" << std::endl;

    cv::Mat testImage = cv::imread("test.jpg");

    //int test=0;
    //for (; test<5; test++)
    {
        //std::cout << "Test: " << test << std::endl;
        {
            MyCallback myCallback;


            myCallback.mTree.Initialize(2);
            Terminal::GetInstance().AddSubscriber(&myCallback, new int(0));

            //Display* disp = new Display(4);

            Resize *r0, *r1, *r2, *r3;
            Gaussian *g0, *g1, *g2, *g3, *g4, *g5, *g6;
            Difference *d0, *d1, *d2, *d3, *d4, *d5;
            Hough *h0, *h1, *h2, *h3, *h4, *h5;

            //Tree::Terminal t

            myCallback.mTree.AddChild(r0 = new Resize(.5), Resize::ArgImage);
                r0->AddChild(g0 = new Gaussian(2), Gaussian::ArgImage);
                    g0->AddChild(d0 = new Difference(), Difference::Image1);
                r0->AddChild(g1 = new Gaussian(4), Gaussian::ArgImage);
                    g1->AddChild(d0, Difference::Image2);
                    g1->AddChild(d1 = new Difference(), Difference::Image1);

                r0->AddChild(g2 = new Gaussian(8), Gaussian::ArgImage);
                    g2->AddChild(d1, Difference::Image2);
                    g2->AddChild(d2 = new Difference(), Difference::Image1);
                r0->AddChild(g3 = new Gaussian(16), Gaussian::ArgImage);
                    g3->AddChild(d2, Difference::Image2);
                    g3->AddChild(d3 = new Difference(), Difference::Image1);
                r0->AddChild(g4 = new Gaussian(32), Gaussian::ArgImage);
                    g4->AddChild(d3, Difference::Image2);
                    g4->AddChild(d4 = new Difference(), Difference::Image1);
                r0->AddChild(g5 = new Gaussian(64), Gaussian::ArgImage);
                    g5->AddChild(d4, Difference::Image2);
                    g5->AddChild(d5 = new Difference(), Difference::Image1);
                r0->AddChild(g6 = new Gaussian(128), Gaussian::ArgImage);
                    g6->AddChild(d5, Difference::Image2);

                d0->AddChild(h0 = new Hough(), Hough::ArgImage);
                    h0->AddChild(&Terminal::GetInstance(), "");
                d1->AddChild(h1 = new Hough(), Hough::ArgImage);
                    h1->AddChild(&Terminal::GetInstance(), "");
                d2->AddChild(h2 = new Hough(), Hough::ArgImage);
                    h2->AddChild(&Terminal::GetInstance(), "");
                d3->AddChild(h3 = new Hough(), Hough::ArgImage);
                    h3->AddChild(&Terminal::GetInstance(), "");
                d4->AddChild(h4 = new Hough(), Hough::ArgImage);
                    h4->AddChild(&Terminal::GetInstance(), "");
                d5->AddChild(h5 = new Hough(), Hough::ArgImage);
                    h5->AddChild(&Terminal::GetInstance(), "");

            myCallback.mStartTime = boost::posix_time::microsec_clock::local_time();
            __sync_synchronize();
            myCallback.mTree.SetJob(cv::imread("test.jpg"));
            /*for (int test=0; test<50; test++)
            {
                //std::cout << "Test " << test << std::endl;
                myCallback.mStartTime = boost::posix_time::microsec_clock::local_time();
                tree.SetJob(cv::imread("test.jpg"));

                do
                {
                    wait(40);
                    __sync_synchronize();
                } while(!myCallback.complete);


                while(!__sync_bool_compare_and_swap(&myCallback.complete, true, false));
                //myCallback.complete = false;
                //__sync_synchronize();
            }*/

            while (true)
            {
                wait(40);
                __sync_synchronize();
                if (myCallback.complete)
                    break;
            }


            myCallback.mTree.Stop();

            int testnum=0;
            while (myCallback.mRuntimes.size())
            {
                testnum++;

                long long int millis;
                if (myCallback.mRuntimes.pop_front(millis))
                    std::cout << "BMP-RAP Test " << testnum << " " << millis << " ms" << std::endl;
            }

            if (!myCallback.mResult.empty())
                cv::imshow("Final Result ( BMP-RAP )", myCallback.mResult);
            cv::waitKey(50);
        }

        {
            boost::posix_time::ptime mst0 = boost::posix_time::microsec_clock::local_time();

            cv::Mat resized;
            const cv::Mat input = cv::imread("test.jpg");
            cv::resize(input, resized, cv::Size(input.cols*.5, input.rows*.5));

            double mSigma;

            cv::Mat g0; mSigma = 2;
            cv::GaussianBlur(resized, g0, cv::Size(2*mSigma+1, 2*mSigma+1), mSigma);

            cv::Mat g1; mSigma = 4;
            cv::GaussianBlur(resized, g1, cv::Size(2*mSigma+1, 2*mSigma+1), mSigma);

            cv::Mat g2; mSigma = 8;
            cv::GaussianBlur(resized, g2, cv::Size(2*mSigma+1, 2*mSigma+1), mSigma);

            cv::Mat g3; mSigma = 16;
            cv::GaussianBlur(resized, g3, cv::Size(2*mSigma+1, 2*mSigma+1), mSigma);

            cv::Mat g4; mSigma = 32;
            cv::GaussianBlur(resized, g4, cv::Size(2*mSigma+1, 2*mSigma+1), mSigma);

            cv::Mat g5; mSigma = 64;
            cv::GaussianBlur(resized, g5, cv::Size(2*mSigma+1, 2*mSigma+1), mSigma);

            cv::Mat g6; mSigma = 128;
            cv::GaussianBlur(resized, g6, cv::Size(2*mSigma+1, 2*mSigma+1), mSigma);

            cv::Mat d0 = g0-g1;
            cv::Mat d1 = g1-g2;
            cv::Mat d2 = g2-g3;
            cv::Mat d3 = g3-g4;
            cv::Mat d4 = g4-g5;
            cv::Mat d5 = g5-g6;

            cv::Mat h0 = HoughTrans(d0);
            cv::Mat h1 = HoughTrans(d1);
            cv::Mat h2 = HoughTrans(d2);
            cv::Mat h3 = HoughTrans(d3);
            cv::Mat h4 = HoughTrans(d4);
            cv::Mat h5 = HoughTrans(d5);

            cv::Mat avg = h0*(1./6.) + h1*(1./6.) + h2*(1./6.)
                        + h3*(1./6.) + h4*(1./6.) + h5*(1./6.);

            boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
            boost::posix_time::time_duration msdiff = mst1 - mst0;
            std::cout << "Graph execution time ( Sequential ): " << msdiff.total_milliseconds() << " ms" << std::endl;

            cv::imshow("Final Result ( Sequential )", avg);
            cv::waitKey(25);
        }
    }

    return 0;
}
