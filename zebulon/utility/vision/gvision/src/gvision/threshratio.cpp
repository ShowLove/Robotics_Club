#include "gvision/threshratio.h"

using namespace Zebulon;
using namespace Vision;

ThreshRatio::ThreshRatio()
{

}
ThreshRatio::~ThreshRatio()
{

}

void ThreshRatio::Setup(Target target,int Edge)
{
    Setup(target.mRed,target.mGreen,target.mBlue,target.mRedRange,target.mGreenRange,target.mBlueRange,Edge);
}



void ThreshRatio::Setup(int Red,int Green,int Blue,int Rrange,int Grange,int Brange,int Edge)
{
    //calculate the gray value
    int gray=(Red+Green+Blue)/3;

    //offset of color from gray, to cancel lighting conditions
    //value between 0 and 254 still
    red=(Red-gray)/2+127;
    green=(Green-gray)/2+127;
    blue=(Blue-gray)/2+127;

    rrange=Rrange;
    grange=Grange;
    brange=Brange;

    edge=Edge;
}

void ThreshRatio::Process(const IplImage *Input,IplImage *&Output)
{
    if(Input==NULL)
    {
        printf("threshratio image corrupt\n");
        return;
    }

    if(output==NULL)
    {
        //create "grey" image of threshhold
        //printf("Input %dx%d\n",Input->width,Input->height);
        output=cvCreateImage(cvSize(Input->width,Input->height),IPL_DEPTH_8U,3);
    }

    //clear the threshhold
    memset(output->imageData,0,output->imageSize);

    unsigned char *imageData=(unsigned char *)Input->imageData;


    int pos;
    int gray;

    int ratred;
    int ratblue;
    int ratgreen;
    for(int j=0;j<output->height;j++)
    {
        for(int i=0;i<output->width;i++)
        {
            pos=j*output->widthStep+3*i;

            //calculate the gray value
            gray=((int)imageData[pos+2]+(int)imageData[pos+1]+(int)imageData[pos+0])/3;

            //offset of color from gray, to cancel lighting conditions
            //value between 0 and 254 still
            ratred=((int)imageData[pos+2]-gray)/2+127;
            ratgreen=((int)imageData[pos+1]-gray)/2+127;
            ratblue=((int)imageData[pos+0]-gray)/2+127;

            if(abs(ratred-red)<rrange && abs(ratgreen-green)<grange && abs(ratblue-blue)<brange)
            {
                output->imageData[pos+0]=255;
                output->imageData[pos+1]=255;
                output->imageData[pos+2]=255;
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
