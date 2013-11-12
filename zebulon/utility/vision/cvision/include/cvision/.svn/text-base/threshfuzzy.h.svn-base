#include <opencv/cv.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "gvision/threshhold.h"

#ifndef _THRESH_FUZZY_H
#define _THRESH_FUZZY_H

namespace Zebulon
{
    namespace Vision
    {

        class ThreshFuzzy : public Threshhold
        {
            public:
                ThreshFuzzy();
                ~ThreshFuzzy();
                        
                virtual void Setup(double redMean,double greenMean,double blueMean,double redTrue=1,double greenTrue=1,double blueTrue=1, double rlf=1, double rrf=1, double glf=1, double grf=1, double blf=1, double brf=1, int edge=0);
                virtual void Setup(Target target,int Edge=0);
                virtual void Process(const IplImage *Input,IplImage *&Output);
                
            protected:
            
                double mRedMean, mGreenMean, mBlueMean;
                double mRedTrue, mGreenTrue, mBlueTrue;
                double mRedLeftFalse, mRedRightFalse;
                double mGreenLeftFalse, mGreenRightFalse;
                double mBlueLeftFalse, mBlueRightFalse;
                
                double mColorMeans[3];
                double mTrue[3];
                double mLeftFalse[3];
                double mRightFalse[3];
                
                int mEdge;
                
                enum Colors
                {
                    RED = 0,
                    GREEN,
                    BLUE
                };
        };
    }
}
#endif
