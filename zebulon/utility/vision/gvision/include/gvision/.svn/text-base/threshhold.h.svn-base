#include<opencv/cv.h>
#include<opencv2/core/core.hpp>
#include<stdio.h>
#include<string.h>

#include "gvision/target.h"
#include "gvision/return.h"

#ifndef _THRESHHOLD
#define _THRESHHOLD

namespace Zebulon
{
    namespace Vision
    {
        class Threshhold
        {
            public:
                Threshhold();
                ~Threshhold();
                
                virtual void Setup(int Red,int Green,int Blue,int Rrange=32,int Grange=32,int Brange=32,int Edge=0);
                virtual void Setup(Target target,int Edge=0);

                void TrapOut(int TrapBottom,int TrapTop,int TrapWidth);
            
                virtual void Process(const IplImage *Input,IplImage *&Output);
                
                virtual void Process(const cv::Mat &Input, cv::Mat &Output);
                
            protected:
                void TrapClear(IplImage *Input);
            
                //block out certain area
                int trapbottom,traptop,trapwidth;
                int numpoints;
                CvPoint points[4];
                CvPoint stripe[4];
            
                int red,green,blue;
                int rrange,grange,brange;
                int edge;
                
                IplImage *output;
        };
    }
}
#endif
