////////////////////////////////////////////////////////////////////////////////////
///
/// \file groundvision.cpp
/// \brief Functions for doing vision processing for the ground vehicle.
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams.ucf.edu<br>
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
#include "igvc/ground/vision/groundvision.h"
#include "cxutils/math/coordinates.h"
using namespace Zebulon;
using namespace IGVC;
using namespace Vision;


GroundVision::GroundVision()
{
    mHomo.LoadSettings();

    IGVCBarrelObject * barrelobject = new IGVCBarrelObject();
    mObjects.push_back(barrelobject);
    //mRansac.setNumberOfThreads(4);
    mMinWhiteThreshold = 240;
    mMaxWhiteThreshold = 255;
    mMinCanny = 50;
    mMaxCanny = 100;
    mMinBlobNoiseFilter = 0;
    mMaxBlobNoiseFilter = .005;
    mMinRansacUsedData = .02;
    mMaxRansacUsedData = .15;
    mLineMaskWidth = 125;
    mMaxAngleDiffDegrees = 45;
    mOriginOffsetPercentX = .5;
    mOriginOffsetPercentY = 1;
    LoadSettings("settings/vision/groundvision.xml");
}

GroundVision::~GroundVision()
{
    for(int i = 0; i < mObjects.size(); i++)
    {
        delete mObjects[i];
    }
}

bool GroundVision::LoadSettings(const std::string& xmlFilename)
{
    if(!mXML.Load(xmlFilename, "GroundVision")) return false;
    LoadSettings();

    return true;
}
bool GroundVision::LoadSettings()
{
    mXML.GetVar("WhiteThreshold@min", mMinWhiteThreshold);
    mXML.GetVar("WhiteThreshold@max", mMaxWhiteThreshold);
    mXML.GetVar("Canny@min", mMinCanny);
    mXML.GetVar("Canny@max", mMaxCanny);
    mXML.GetVar("BlobNoiseFilter@min", mMinBlobNoiseFilter);
    mXML.GetVar("BlobNoiseFilter@max", mMaxBlobNoiseFilter);
    mXML.GetVar("RansacUsedData@min", mMinRansacUsedData);
    mXML.GetVar("RansacUsedData@max", mMaxRansacUsedData);
    mXML.GetVar("LineMaskWidth@value", mLineMaskWidth);
    mXML.GetVar("AngleDiffDegrees@max", mMaxAngleDiffDegrees);
    mXML.GetVar("OriginOffset@x", mOriginOffsetPercentX);
    mXML.GetVar("OriginOffset@y", mOriginOffsetPercentY);

    std::string visionXML;
    mXML.GetVar("VisionObject@xml", visionXML);
    for(int i = 0; i < mObjects.size(); i++)
    {
        mObjects[i]->Initialize(visionXML);
    }



}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns lane lines found in image. Origin offset from top left
///          to bottom middle(local coordinates).
///
///   \param[in] ImgIn Image to process.
///   \param[in] ImgOut Optional Output Image.
///   \param[in] rows Number of rows to split image up into before processing.
///   \param[in] cols Number of cols to split image up into before processing.
///
///   \return A list of Segment 3D objects representing each line found.  Will not
///           be limited to 2, but can vary from 0 up to number of rows x cols.
///
////////////////////////////////////////////////////////////////////////////////////
std::vector<CxUtils::Segment3D> GroundVision::FindLaneLines(const IplImage* imgIn, IplImage*& imgOut, int rows, int cols)
{

    std::vector<CxUtils::Segment3D> lines;
    if(imgIn == NULL) return lines;
    IplImage * edges = NULL;
    CxUtils::Segment3D line;
    edges = FindLaneEdges(imgIn);
    std::vector<IplImage*> cells = Zebulon::Vision::VisionUtils::GetImageCells(edges,rows,cols);
    cvShowImage("edges",edges);
    if(imgOut == NULL)
    {
        imgOut = mHomo.Warp(imgIn);
    }
    for(int i = 0; i < cells.size(); i++)
    {
        if(RansacFit(cells[i],line))
        {
            CxUtils::Point3D offset;
            offset = Zebulon::Vision::VisionUtils::GetCellOffset(i,rows,cols,cells[i]);
            //cvShowImage("edges",cells[i]);
            //cvWaitKey(100);
            line.mPoint1 += offset;
            line.mPoint2 += offset;
            //temp = line;
            CxUtils::Segment3D temp = mHomo.Warp(line,Homography::Inverse);
            DrawSegment(temp,imgOut,CV_RGB(0,0,255));
            //DrawSegment(line,imgOut);
            line = FromImageToLocal(line,imgIn);
            lines.push_back(line);
        }
        cvReleaseImage(&(cells[i]));
    }
    cvReleaseImage(&edges);
    return lines;
}

//std::vector<CxUtils::Segment3D> GroundVision::FindPerspectiveLaneLines(const IplImage* imgIn, IplImage*& imgOut)
//{
//    IplImage * edges = NULL;
//    std::vector<CxUtils::Segment3D> lines;
//    CxUtils::Segment3D line;
//    edges = FindLaneEdges(imgIn);
//    CvPoint p1, p2;
//    if(edges)
//    {
//        if(imgOut == NULL)
//        {
//            imgOut = mHomo.Warp(imgIn);
//        }
//        cvShowImage("edges",edges);
//        if(RansacFit(edges,line))
//        {
//            lines.push_back(FromImageToLocal(line,imgIn));
//            p1.x = line.mPoint1.mX;
//            p1.y = line.mPoint1.mY;
//            p2.x = line.mPoint2.mX;
//            p2.y = line.mPoint2.mY;
//            cvLine( edges, p1, p2, CV_RGB(0,0,0), mLineMaskWidth, 8 );
//            if(imgOut)
//            {
//                 cvLine( imgOut, mHomo.Warp(p1,Homography::Inverse),  mHomo.Warp(p2,Homography::Inverse), CV_RGB(0,0,255), 20, 8 );
//            }
//        }
//        if(RansacFit(edges,line))
//        {
//            lines.push_back(FromImageToLocal(line,imgIn));
//            p1.x = line.mPoint1.mX;
//            p1.y = line.mPoint1.mY;
//            p2.x = line.mPoint2.mX;
//            p2.y = line.mPoint2.mY;
//            if(imgOut)
//            {
//                 cvLine( imgOut, mHomo.Warp(p1,Homography::Inverse),  mHomo.Warp(p2,Homography::Inverse), CV_RGB(255,0,0), 20, 8 );
//            }
//        }
//    }
//    cvReleaseImage(&edges);
//    return lines;
//}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Uses RANSAC to fit a line to white pixels in a binary image.
///
///   \param[in] binaryImage Image to process.
///   \param[in] line line found in pixel coordinates. Usually spans to edge of image.
///
///   \return True if a line is found, false if not.
///
////////////////////////////////////////////////////////////////////////////////////
bool GroundVision::RansacFit(const IplImage* binaryImage, CxUtils::Segment3D& line) const
{
    CvPoint p1, p2, p3, p4;
    bool result = false;
    std::vector<double> lineParameters;
    std::vector<Point2D *> pointDataPtr;
    std::vector<Point2D> pointData;
    LineParamEstimator lpEstimator(0.5);
    double desiredProbabilityForNoOutliers = 0.999;
    double usedData = 0;
    //populate point2d vector with image
    for(int x = 0; x < binaryImage->width; x++)
    {
        for(int y = 0; y < binaryImage->height; y++)
        {
            unsigned char val = Zebulon::Vision::VisionUtils::GetPixel(binaryImage,y,x,0);
            if(val > 10)
            {
                pointDataPtr.push_back(new Point2D(y,x));
                pointData.push_back(*(pointDataPtr[pointDataPtr.size()-1]));
            }
        }
    }
    lpEstimator.leastSquaresEstimate(pointDataPtr,lineParameters);
    //A RANSAC estimate of the line parameters
    //usedData = mRansac.compute(lineParameters,
    usedData = RANSAC<Point2D,double>::compute(lineParameters,
                                                      &lpEstimator ,
                                                      pointData,
                                                      desiredProbabilityForNoOutliers);

    if(lineParameters.size() > 0 && usedData > mMinRansacUsedData && usedData < mMaxRansacUsedData)
    {
        //std::cout<<"RANSAC line parameters [n_x,n_y,a_x,a_y]\n\t [ "<<lineParameters[0]<<", "<<lineParameters[1]<<", ";
        //std::cout<<lineParameters[2]<<", "<<lineParameters[3]<<" ]"<<std::endl;
        double rise = -lineParameters[1];
        double run = lineParameters[0];
        double x1 = lineParameters[3] - 1000*run;
        double y1 = lineParameters[2] - 1000*rise;
        double x2 = lineParameters[3] + 1000*run;
        double y2 = lineParameters[2] + 1000*rise;
        p1 = cvPoint(x1,y1);
        p2 = cvPoint(x2,y2);
        cvClipLine(cvSize(binaryImage->width,binaryImage->height),&p1,&p2);
        //cvLine( binaryImage, p1, p2, CV_RGB(0,0,0), mLineMaskWidth, 8 );
        //cvShowImage("blah",binaryImage);
        line = CxUtils::Segment3D(CxUtils::Point3D(p1.x,p1.y), CxUtils::Point3D(p2.x,p2.y));
        result = true;
    }
    for(int i = 0; i < pointDataPtr.size(); i++)
    {
        delete pointDataPtr[i];
        pointDataPtr[i] = NULL;
    }
    pointData.clear();
    pointDataPtr.clear();
    return result;

}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Basically does edge detection on an image with some minimal noise filter.
///
///   \param[in] Image Image to process.
///
///   \return The resulting image which ransac or other line detection can be run.
///
////////////////////////////////////////////////////////////////////////////////////
IplImage * GroundVision::FindLaneEdges(const IplImage* imgIn)
{
    //create object mask
    if(imgIn == NULL) return NULL;
    LoadSettings();
    IplImage * mask = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    cvZero(mask);
    IplImage * result = NULL;
    for(int i = 0; i < mObjects.size(); i++)
    {
        IplImage * temp = NULL;
        temp = mObjects[i]->GetMask(imgIn);

//        cvShowImage("mask",temp);
//        cvWaitKey(1);
        cvOr(mask,temp,mask);
        cvReleaseImage(&temp);
    }
    //need to invert image.  we want background to be white, objects to be black.
    cvAbsDiffS(mask,mask,cvScalar(255,255,255));

    ///////////////////////////////threshold lanelines///////////////////
    IplImage * hsv = cvCloneImage(imgIn);
    IplImage * chan0 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan1 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan2 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan3 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    cvCvtColor(imgIn, hsv, CV_BGR2HSV);

    cvSplit(hsv,chan0,chan1,chan2, NULL);

    CvScalar white = cvRealScalar(255);
    result = cvCloneImage(chan1);
    cvEqualizeHist( result, result);
    //invert black and white
    cvAbsDiffS(result, result, white);
    cvReleaseImage(&hsv);
    cvReleaseImage(&chan0);
    cvReleaseImage(&chan1);
    cvReleaseImage(&chan2);
    cvReleaseImage(&chan3);
    //////////////////////////////////////////////////////////////////////////////////////////////////

    CvSize imageSize = cvSize(imgIn->width & -2, imgIn->height & -2 );
    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 1 );
    cvPyrDown( result, imgSmallCopy);
    cvPyrUp( imgSmallCopy, result);
    //////////////////////////////////////////////////////////////////////////////////////////////////
    cvThreshold(result,result,mMinWhiteThreshold, mMaxWhiteThreshold,CV_THRESH_TOZERO);
    IplImage * temp = cvCloneImage(result);
    cvAnd(result,mask,result);
    //cvShowImage("mask",mask);
    //cvShowImage("result",result);
    //cvWaitKey(0);
    cvCanny( result, temp, mMinCanny, mMaxCanny, 3);;
    cvCopy(temp,result);
    //mask around edge of image to get rid of other wierd noise
    cvDrawRect(result,cvPoint(0,0),cvPoint(result->width,result->height),cvScalar(0,0,0),20);
    cvReleaseImage(&temp);
    cvReleaseImage(&imgSmallCopy);
    //////////////////////////////////////////////////////////////////////////////////////////////////
    std::list<CvBox2D> blobList;
    blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(result,mMinBlobNoiseFilter,mMaxBlobNoiseFilter);
    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    {
       CvPoint2D32f boxCorners32[4];
       CvPoint boxCorners[4];
       cvBoxPoints(*it,boxCorners32);
       for(int i = 0; i < 4; i ++)
       {
           boxCorners[i] = cvPointFrom32f(boxCorners32[i]);
       }
       cvFillConvexPoly(result,boxCorners,4,cvScalar(0,0,0),4);
       //Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
    }
    cvReleaseImage(&mask);
    return result;
}

CxUtils::Point3D GroundVision::FromImageToLocal(const CxUtils::Point3D& point, const IplImage* image)
{
    CxUtils::Point3D result = mHomo.FromImageToGround(point,image,mOriginOffsetPercentX,mOriginOffsetPercentY);
    //swapping x and y for cartographer mapping
    std::swap(result.mX,result.mY);
    //converting centimeters to meters
    result/=100;
    return result;
}

CxUtils::Segment3D GroundVision::FromImageToLocal(const CxUtils::Segment3D& line, const IplImage* image)
{
    return CxUtils::Segment3D(FromImageToLocal(line.mPoint1,image), FromImageToLocal(line.mPoint2,image));
}

//untested
CxUtils::Point3D GroundVision::FromGroundToImage(const CxUtils::Point3D point, const IplImage* image)
{
    CxUtils::Point3D result;
    //swapping x and y for cartographer mapping
    result.mX = point.mY;
    result.mY = point.mX;
    //converting centimeters to meters
    result *= 100.;
    result = mHomo.FromGroundToImage(result,image);
    return result;
}

CxUtils::Segment3D GroundVision::FromGroundToImage(const CxUtils::Segment3D& line, const IplImage* image)
{
    return CxUtils::Segment3D(FromGroundToImage(line.mPoint1,image), FromImageToLocal(line.mPoint2,image));
}

bool GroundVision::FixPerspective(const IplImage* imgIn, IplImage*& imgOut)
{
    if(imgOut!=NULL) return false;
    imgOut = mHomo.Warp(imgIn,Homography::Original);
}

void GroundVision::DrawSegment(const CxUtils::Segment3D& line, IplImage* imgIn, CvScalar color) const
{
    if(imgIn)
    {
        CvPoint p1, p2;
        p1 = cvPoint(line.mPoint1.mX,line.mPoint1.mY);
        p2 = cvPoint(line.mPoint2.mX,line.mPoint2.mY);
        cvLine( imgIn, p1, p2, color, 5, 8 );
    }
}
