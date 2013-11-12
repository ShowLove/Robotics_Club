////////////////////////////////////////////////////////////////////////////////////
///
/// \file grapher.cpp
/// \brief A Grapher class used for debugging display
///
/// Author(s): Gary Stein<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF)<br>
/// Email: gstein@mail.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
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
#include "grapher/grapher.h"

using namespace Zebulon;
using namespace Utility;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Grapher::Grapher()
{
    mPlotImage=NULL;
    cvInitFont(&mFont,CV_FONT_HERSHEY_SIMPLEX,1.0,1.0,0,2);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Grapher::~Grapher()
{
    if(mPlotImage!=NULL)
    {
        cvReleaseImage(&mPlotImage);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Setup the graphs to draw.
///
///   \param[in] windowName Name of the window.
///   \param[in] numAxis Number of items to plot (max).
///   \param[in] resolutoin Resolution in pixels.
///   \param[in] width Width in pixels per graph.
///   \param[in] height Height in pixels per graph.
///
////////////////////////////////////////////////////////////////////////////////////
void Grapher::Setup(const std::string& windowName,int numAxis,int resolution,int width,int height)
{
    //AI::Display::CreateWindow(WindowName);
    
    mNumAxis=numAxis;
    mResolution=resolution;
    mWidth=width;
    mHeight=height;
    mWindowName=windowName;
    
    // clear out list in case (instead of reserve)
    // precalculate the X-axis
    double value=0.0;
    std::vector<double> axis;
    for(int i=0; i < mResolution;i++)
    {
        axis.push_back(0.0);
        value=((double)i/(double)mResolution)*mWidth;
        mXAxis.push_back(value);
    }
    //bit tricky for colors
    int color=1;
    for(int i=0;i<mNumAxis;i++)
    {
        mAxii.push_back(axis);
        
        rgb tmp;
        tmp.r=(color&0x01)?255:0;
        tmp.g=(color&0x02)?255:0;
        tmp.b=(color&0x04)?255:0;
        mColors.push_back(tmp);
        color++;
        //skip white
        if(color%8==7)
        {
            color++;
        }
        
        mMins.push_back(-10.0);
        mMaxs.push_back(10.0);
    }
    //make the image
    mPlotImage=cvCreateImage(cvSize(mWidth,mHeight*mNumAxis),IPL_DEPTH_8U,3);
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Setup range of values for an item to graph (scales data).
///
///   \param[in] axis Data axis (item to graph).
///   \param[in] min Minimum range value.
///   \param[in] max Maximum value.
///
////////////////////////////////////////////////////////////////////////////////////
void Grapher::Range(int axis, double min, double max)
{
    if(axis < 0 || axis >= mNumAxis)
    {
        return;
    }
    mMins[axis]= min;
    mMaxs[axis]= max;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Update value to graph.
///
///   \param[in] axis Data axis (item to graph).
///   \param[in] value Data value to add to graph.
///
////////////////////////////////////////////////////////////////////////////////////
void Grapher::AddValue(int axis,double value)
{
    if(axis < 0 || axis >= mNumAxis)
    {
        return;
    }
    //erase the first element
    mAxii[axis].erase(mAxii[axis].begin());
    mAxii[axis].push_back(value);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Updates the graph image.
///
////////////////////////////////////////////////////////////////////////////////////
void Grapher::Graph()
{
    char Number[1024];
    if(mPlotImage == NULL)
    {
        return;
    }
    //memset(mPlotImage->imageData,255,mPlotImage->imageSize);
    memset(mPlotImage->imageData,128,mPlotImage->imageSize);
    for(int j=0;j<mNumAxis;j++)
    {
        int dispMin=mHeight*j;
        int dispMax=mHeight*j+mHeight;
        double CenterY=mHeight*(j+1)-mHeight*(0.0-mMins[j])/(mMaxs[j]-mMins[j]);
        if(CenterY >= dispMin && CenterY <= dispMax)
        {
            cvLine(mPlotImage, cvPoint(0,(int)CenterY), cvPoint(mWidth, (int)CenterY), cvScalar(0,0,0), 1);
        }
        sprintf(Number,"%.02lf",mAxii[j][mResolution-1]);
        cvPutText(mPlotImage,Number, cvPoint(0,mHeight*(j+1)),&mFont,cvScalar(216,216,216));
        double y = 0,oy = -1;
        double x = 0,ox = -1;
        for(int i=0;i<mResolution-1;i++)
        {
            oy=y;
            y=mHeight*(j+1)-mHeight*(mAxii[j][i]-mMins[j])/(mMaxs[j]-mMins[j]);
            ox=x;
            x=mXAxis[i];
            if(i==0)
            {
                continue;
            }
            //make sure it is in range
            if(y >= dispMin && y <= dispMax && oy >= dispMin && oy <= dispMax)
            {
                cvLine(mPlotImage, cvPoint((int)ox, (int)oy), cvPoint((int)x, (int)y), cvScalar(mColors[j].r,mColors[j].g,mColors[j].b), 2);
            }
        }
    }
    Utility::Display::DisplayImage(mWindowName,mPlotImage);
}

/*  End of File */

