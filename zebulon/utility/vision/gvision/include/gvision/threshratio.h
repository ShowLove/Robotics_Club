#include<opencv/cv.h>
#include<stdio.h>
#include<string.h>

#include "gvision/threshhold.h"

#ifndef _THRESHRATIO
#define _THRESHRATIO
namespace Zebulon
{
    namespace Vision
    {

        class ThreshRatio : public Threshhold
        {
            public:
                ThreshRatio();
                ~ThreshRatio();
                        
                virtual void Setup(int Red,int Green,int Blue,int Rrange=32,int Grange=32,int Brange=32,int Edge=0);
                virtual void Setup(Target target,int Edge=0);
                virtual void Process(const IplImage *Input,IplImage *&Output);
                
        };
    }
}
#endif
