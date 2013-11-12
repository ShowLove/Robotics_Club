#include <opencv/cv.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "gvision/threshhold.h"

#ifndef _THRESH_GAUSSIAN_H
#define _THRESH_GAUSSIAN_H

namespace Zebulon
{
    namespace Vision
    {

        class ThreshGaussian : public Threshhold
        {
            public:
                ThreshGaussian();
                ~ThreshGaussian();
                        
                virtual void Setup(double redMean,double greenMean,double blueMean,double redVariance=1,double greenVariance=1,double blueVariance=1,int edge=0);
                virtual void Setup(Target target,int Edge=0);
                virtual void Process(const IplImage *Input,IplImage *&Output);
                
            protected:
                double Gaussian(double mean, double variance, double x);
                
                double mRedMean, mGreenMean, mBlueMean;
                double mRedVariance, mGreenVariance, mBlueVariance;
                
                int mEdge;
        };
    }
}
#endif
