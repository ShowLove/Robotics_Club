#include "cvision/threshgaussian.h"

using namespace Zebulon;
using namespace Vision;

ThreshGaussian::ThreshGaussian()
{
    
}
ThreshGaussian::~ThreshGaussian()
{
    
}

void ThreshGaussian::Setup(Target target,int Edge)
{
    Setup(target.mRedMean,target.mGreenMean,target.mBlueMean,target.mRedVariance,target.mGreenVariance,target.mBlueVariance,edge);
}



void ThreshGaussian::Setup(double redMean,double greenMean,double blueMean,double redVariance,double greenVariance,double blueVariance,int edge)
{
    mRedMean = redMean;
    mGreenMean = greenMean;
    mBlueMean = blueMean;
    mRedVariance = redVariance;
    mGreenVariance = greenVariance;
    mBlueVariance = blueVariance;
    
    mEdge = edge;
}

void ThreshGaussian::Process(const IplImage *Input,IplImage *&Output)
{
    if(Input==NULL)
    {
        printf("ThreshGaussian image corrupt\n");
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
 
    unsigned char *imageData=(unsigned char *)Input->imageData;
    
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
            
            if(gaussianFinal > 0.9)
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
    }
    
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

double ThreshGaussian::Gaussian(double mean, double variance, double x)
{
    double gaussian = 0;
    //gaussian = ((1.0/sqrt(2*M_PI*pow(mVariance[position],2.0)))*exp((-1.0)*(pow(x - mMeans[position], 2.0)/(2.0*pow(mVariance[position],2)))));
    gaussian = (exp((-1.0)*(((x - mean)*(x - mean))/(2.0*(variance*variance)))));
    
    return gaussian;
}
