#include "cvision/threshfuzzy.h"

using namespace Zebulon;
using namespace Vision;

ThreshFuzzy::ThreshFuzzy()
{
    
}
ThreshFuzzy::~ThreshFuzzy()
{
    
}

void ThreshFuzzy::Setup(Target target,int Edge)
{
    Setup(target.mRedMean,target.mGreenMean,target.mBlueMean,target.mRedTrue,target.mGreenTrue,target.mBlueTrue,target.mRedLeftFalse,target.mRedRightFalse,target.mGreenLeftFalse,target.mGreenRightFalse,target.mBlueLeftFalse,target.mBlueRightFalse,edge);
}

void ThreshFuzzy::Setup(double redMean,double greenMean,double blueMean,double redTrue,double greenTrue,double blueTrue, double rlf, double rrf, double glf, double grf, double blf, double brf, int edge)
{
    mRedMean = redMean;
    mColorMeans[RED] = redMean;
    mGreenMean = greenMean;
    mColorMeans[GREEN] = greenMean;
    mBlueMean = blueMean;
    mColorMeans[BLUE] = blueMean;
    
    mRedTrue = redTrue;
    mTrue[RED] = redTrue;
    mRedLeftFalse = rlf;
    mLeftFalse[RED] = rlf;
    mRedRightFalse = rrf;
    mRightFalse[RED] = rrf;
    
    mGreenTrue = greenTrue;
    mTrue[GREEN] = greenTrue;
    mGreenLeftFalse = glf;
    mLeftFalse[GREEN] = glf;
    mGreenRightFalse = grf;
    mRightFalse[GREEN] = grf;
    
    mBlueTrue = blueTrue;
    mTrue[BLUE] = blueTrue;
    mBlueLeftFalse = blf;
    mLeftFalse[BLUE] = blf;
    mBlueRightFalse = brf;
    mRightFalse[BLUE] = brf;
    
    mEdge = edge;
}

void ThreshFuzzy::Process(const IplImage *Input,IplImage *&Output)
{
    if(Input==NULL)
    {
        printf("ThreshFuzzy image corrupt\n");
        return;
    }
    
    if(output==NULL)
    {
        //create "grey" image of threshhold
        printf("Input %dx%d\n",Input->width,Input->height);
        output=cvCreateImage(cvSize(Input->width,Input->height),IPL_DEPTH_8U,3);
    }
    
    //clear the threshhold
    memset(output->imageData,0,output->imageSize);
    
    
    int pos = 0;
    unsigned char *imageData=(unsigned char *)Input->imageData;
    
    for(int j=0;j<output->height;j++)
    {
        for(int i=0;i<output->width;i++)
        {
            pos=j*output->widthStep+3*i;
            
            double trueLeft[3];
            double trueRight[3];
            for(int k = 0; k < 3; k++)
            {
                trueLeft[k] = mColorMeans[k] - mTrue[k];
                trueRight[k] = mColorMeans[k] + mTrue[k];
            }
            
            // Find slope
            double slopePos[3];
            double slopeNeg[3];
            for(int k = 0; k < 3; k++)
            {
                slopePos[k] = 255.0/(trueLeft[k] - mLeftFalse[k] + 0.0000000000000001);
                slopeNeg[k] = 255.0/(trueRight[k] - mRightFalse[k] + 0.0000000000000001);
            }
                
            double bLow[3];
            double bHigh[3];
            for(int k = 0; k < 3; k++)
            {
                bLow[k] = 255.0 - trueLeft[k]*slopePos[k];
                bHigh[k] = 255.0 - trueRight[k]*slopeNeg[k];
            }
                
            double yLow[3];
            double yHigh[3];
            double finalColors[3];
            
            for(int k = 0; k < 3; k++)
            {
                //double leftTrue = mColorMeans[k] - trueLeft[k];
                //double rightTrue = mColorMeans[k] + trueRight[k];
                if(trueLeft[k] < imageData[pos+k] && imageData[pos+k] < trueRight[k])
                {
                    finalColors[k] = 255;
                }
                else if(mLeftFalse[k] <= imageData[pos+k] && imageData[pos+k] <= trueLeft[k])
                {
                    finalColors[k] = slopePos[k]*imageData[pos+k] + bLow[k];
                }
                else if(trueRight[k] <= imageData[pos+k] && imageData[pos+k] <= mRightFalse[k])
                {
                    finalColors[k] = slopeNeg[k]*imageData[pos+k] + bHigh[k];
                }
                else
                {
                    finalColors[k] = 0;
                }
            }
            
            for(int k = 0; k < 3; k++)
            {
                if(finalColors[k] < 0)
                {
                    finalColors[k] = 0;
                }
                else if(finalColors[k] > 255)
                {
                    finalColors[k] = 255;
                }
            }
                    
            double finalColor = 9999999999999.9;
            
            // Fuzzy Min
            for(int k = 0; k < 3; k++)
            {
                if(finalColors[k] < finalColor)
                {
                    finalColor = finalColors[k];
                }
            }
            
            finalColor = round(finalColor);
            
            // Create Binary Image
            if(finalColor > 127)
            {
                output->imageData[pos+0] = 255;
                output->imageData[pos+1] = 255;
                output->imageData[pos+2] = 255;
            }
            else
            {
                output->imageData[pos+0] = 0;
                output->imageData[pos+1] = 0;
                output->imageData[pos+2] = 0;
            }
            
        }
    }
    
    
    
    
    
    
    
    
 
    /*unsigned char *imageData=(unsigned char *)Input->imageData;
    
    int pos;
    double gaussianRed = 0;
    double gaussianGreen = 0;
    double gaussianBlue = 0;
    double gaussianFinal = 0;
    
    for(int j=0;j<output->height;j++)
    {
        for(int i=0;i<output->width;i++)
        {
            pos=j*output->widthStep+3*i;
            
            gaussianRed = Gaussian(mRedMean, mRedVariance, imageData[pos+2]);
            gaussianGreen = Gaussian(mGreenMean, mGreenVariance, imageData[pos+1]);
            gaussianBlue = Gaussian(mBlueMean, mBlueVariance, imageData[pos+0]);
            gaussianFinal = gaussianRed*gaussianGreen*gaussianBlue;
            
            if(gaussianFinal > 0.75)
            {
                output->imageData[pos+0]=255;
                output->imageData[pos+1]=255;
                output->imageData[pos+2]=255;
            }
            else
            {
                output->imageData[pos+0]=0;
                output->imageData[pos+1]=0;
                output->imageData[pos+2]=0;
            }
        }
    }*/
    
    if(edge>0)
    {
        if(edge>output->height/2)
        {
            printf("Border too big: %d\n",edge);
            edge=output->height/2;
        }
        cvRectangle(output,cvPoint(0,0),cvPoint(output->width,output->height),CV_RGB(0,0,0),edge);
    }
        
    TrapClear(output);
    Output=output;
    
}
