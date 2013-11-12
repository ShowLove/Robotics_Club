#include "gvision/threshhold.h"
using namespace Zebulon;
using namespace Vision;

Threshhold::Threshhold()
{
    output=NULL;
    red=green=blue=127;
    rrange=grange=brange=255;
    numpoints=0;
    edge=0;
}

Threshhold::~Threshhold()
{
    if(output!=NULL)
    {
        cvReleaseImage(&output);
    }
}

void Threshhold::Setup(Target target,int Edge)
{
    Setup(target.mRed,target.mGreen,target.mBlue,target.mRedRange,target.mGreenRange,target.mBlueRange,Edge);
}

void Threshhold::Setup(int Red,int Green,int Blue,int Rrange,int Grange,int Brange,int Edge)
{
    red=Red;
    green=Green;
    blue=Blue;
    
    rrange=Rrange;
    grange=Grange;
    brange=Brange;
    
    edge=Edge;
}

void Threshhold::Process(const IplImage *Input,IplImage *&Output)
{
    if(Input==NULL)
    {
        printf("threshhold image corrupt\n");
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
    //right now binary, personally I like "distance"
    for(int j=0;j<output->height;j++)
    {
        for(int i=0;i<output->width;i++)
        {
            pos=j*output->widthStep+3*i;
            if(abs(imageData[pos+2]-red)<=rrange 
               && abs(imageData[pos+1]-green)<=grange 
               && abs(imageData[pos+0]-blue)<=brange)
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

void Threshhold::TrapOut(int TrapBottom,int TrapTop,int TrapWidth)
{
    numpoints=4;
    trapbottom=TrapBottom;
    traptop=TrapTop;
    trapwidth=TrapWidth;

    //FIXME: hardcoded
    points[0]=cvPoint(0,trapbottom);
    points[1]=cvPoint(320-TrapWidth/2,traptop);
    points[2]=cvPoint(320+TrapWidth/2,traptop);
    points[3]=cvPoint(640,trapbottom);
    
    stripe[0]=cvPoint(0,480);
    stripe[1]=cvPoint(0,trapbottom);
    stripe[2]=cvPoint(640,trapbottom);
    stripe[3]=cvPoint(640,480);
}

void Threshhold::TrapClear(IplImage *Input)
{
    if(numpoints>0)
    {
        //printf("Clear Trap\n");
        cvFillConvexPoly(Input , points, numpoints, CV_RGB(0,0,0));
        cvFillConvexPoly(Input , stripe, numpoints, CV_RGB(0,0,0));
    }    
}

void Threshhold::Process(const cv::Mat &Input, cv::Mat &Output)
{

    if(Input.empty())
    {
        printf("threshhold image corrupt\n");
        return;
    }

    if(Output.empty())
    {
        //create "grey" image of threshhold
        printf("Input %dx%d\n", Input.rows, Input.cols);
        Input.copyTo(Output);
        //output = cvCreateImage(cvSize(Input->width,Input->height),IPL_DEPTH_8U,3);
    }

    //clear the threshhold
    ////memset(output->imageData,0,output->imageSize);

    ////unsigned char *imageData=(unsigned char *)Input->imageData;


    //int pos;
    cv::Vec3b tempWhiteVec;
    tempWhiteVec[0] = 255;
    tempWhiteVec[1] = 255;
    tempWhiteVec[2] = 255;
    cv::Vec3b tempBlackVec;
    tempBlackVec[0] = 0;
    tempBlackVec[1] = 0;
    tempBlackVec[2] = 0;

    //right now binary, personally I like "distance"
    for(int j=0;j<Output.rows;j++)
    {
        for(int i=0;i<Output.cols;i++)
        {
            /*pos=j*output->widthStep+3*i;

            if(abs(imageData[pos+2]-red)<=rrange
               && abs(imageData[pos+1]-green)<=grange
               && abs(imageData[pos+0]-blue)<=brange)
            {
                output->imageData[pos+0]=255;
                output->imageData[pos+1]=255;
                output->imageData[pos+2]=255;
            }*/

            cv::Vec3b tempVec;

            tempVec = Input.at< cv::Vec3b >(j, i);

            if(abs(tempVec[0]-blue) <= brange
               && abs(tempVec[1]-green) <= grange
               && abs(tempVec[2]-red) <= rrange)
            {
                Output.at< cv::Vec3b >(j, i) = tempWhiteVec;
            }
            else
            {
                Output.at< cv::Vec3b >(j, i) = tempBlackVec;
            }
        }
    }
    if(edge>0)
    {
        /*if(edge>output->height/2)
        {
            printf("Border too big: %d\n",edge);
            edge=output->height/2;
        }
        cvRectangle(output,cvPoint(0,0),cvPoint(output->width,output->height),CV_RGB(0,0,0),edge);*/
    }

    //TrapClear(output);
    //Output=output;
}
