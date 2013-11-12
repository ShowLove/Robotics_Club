/*
    Basic interpolation class
    
    Others expand from here
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<vector>
#include<queue>
#include <cxutils/mutex.h>
#include <cxutils/math/cxmath.h>
#include <cxutils/math/matrix.h>

#ifndef _ZEBULON_INTERPOLATE__H
#define _ZEBULON_INTERPOLATE__H

namespace Zebulon
{
    namespace Utility
    {
        class Interpolate
        {
            protected:
            //simple data structure for I/O matching
            struct mapper
            {
                //Input values
                std::vector<double> input;
                //Output values
                std::vector<double> output;
                //dist for sorting
                //not all types use, pre-calculated
                double dist;
                
                //For sorting (backward on purpose)
                bool operator < (const mapper &that) const
                {
                    return dist>that.dist;
                };
                
            };
            
            public:
                //Size must be known to all methods
                Interpolate(int NumInputs,int NumOutputs);
                virtual ~Interpolate();
                
                //all methods also require a file of original points
                int Setup(std::string Filename);
                
                //Wrapper to be called from outside
                std::vector<double> GetOutput(std::vector<double> Input);

                //Uses internal method for checking accurracy with own database
                void CheckRMSE();

            protected:

                //must be overloaded by actual interpolator
                virtual mapper MapIt(std::vector<double> Input) = 0;
                virtual void Init(){};
                
                //Input and Output Size
                int mNumInputs;
                int mNumOutputs;
                
                //stores dataset, assumed list of doubles
                std::vector<mapper> mInterMap;
        };
        
        
        //Case Based Interpolator
        //Looks for N-closest in dataset and bases it's output on that
        class CaseBasedInter : public Interpolate
        {
            public:
                //N-size of values to keep
                CaseBasedInter(int NumInputs,int NumOutputs,int KeepSize=5);
                virtual ~CaseBasedInter();
            protected:
                virtual mapper MapIt(std::vector<double> Input);
                
                int mKeepSize;
        };
        
        //Linear fit of dataset (assume linearized outside)
        class LinearRegInter : public Interpolate
        {
            public:
                LinearRegInter(int NumInputs,int NumOutputs);
                virtual ~LinearRegInter();
            protected:
                virtual mapper MapIt(std::vector<double> Input);
                virtual void Init();
                
                std::vector< std::vector<double> > mConstants;
        };
        
        //Use a jacobian method for prediction
        //Not finished
        class CaseJacobInter : public Interpolate
        {
            public:
                //N-size of values to keep
                CaseJacobInter(int NumInputs,int NumOutputs,int KeepSize=5);
                virtual ~CaseJacobInter();
            protected:
                virtual mapper MapIt(std::vector<double> Input);
                
                int mKeepSize;
        };
        
    }
}
#endif
