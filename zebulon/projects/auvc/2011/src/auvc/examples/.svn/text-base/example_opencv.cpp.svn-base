/////////////////////////////////////////////////////////////////////////////////////
///
/// \file example_opencv.h
/// \brief A program to test some opencv algorithms.
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
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

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cxutils/cxutils.h>
#include <vector>
#include "dvision/visionutils.h"
#include "dvision/visionobject.h"
#include "ransac/RANSAC.h"
#include "ransac/LineParamEstimator.h"
#include "auvc/underwater/vision/pathobject.h"
#include "auvc/underwater/vision/bouyobject.h"
#include "auvc/underwater/vision/bouybaseobject.h"
#include "auvc/underwater/vision/lovelaneobject2.h"
#include "auvc/underwater/vision/lovelaneobject.h"
using namespace Zebulon;
using namespace Vision;

IplImage * gImageIn = NULL;
IplImage * gImageOut = NULL;

std::string gHomoXML = "settings/vision/homo.xml";
std::string gIntrinsicXML = "settings/vision/intrinsics.xml";
std::string gDistortXML = "settings/vision/distortion.xml";
std::string gColorFile = "settings/vision/barrelcolor.png";

bool RansacFit(const IplImage* binaryImage, CxUtils::Segment3D& line)
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

    //if(lineParameters.size() > 0 && usedData > mMinRansacUsedData && usedData < mMaxRansacUsedData)
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

BouyObject red;
BouyObject green;
BouyObject yellow;
BouyBaseObject base;
PathObject path;
LoveLaneObject2 lane;
bool Step0(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
    //red.Initialize("RedBouy");
    //green.Initialize("GreenBouy");
    //yellow.Initialize("YellowBouy");
    //path.Initialize("Path");
    //base.Initialize("BouyBase");
    lane.Initialize("LoveLane2");
//    base.Initialize("BouyBase");

    imgOut = cvCloneImage(imgIn);
    IplImage * temp;
//    cvZero(imgOut);

    //temp = yellow.GetMask(imgIn, imgOut);
    //cvReleaseImage(&temp);
    //temp = green.GetMask(imgIn, imgOut);
    //cvReleaseImage(&temp);
    //temp = red.GetMask(imgIn, imgOut);
    //cvReleaseImage(&temp);
    //temp = path.GetMask(imgIn, imgOut);
    //cvReleaseImage(&temp);
    temp = lane.GetMask(imgIn, imgOut);
    cvReleaseImage(&temp);
//    cvShowImage("g",g);
//    cvShowImage("r",r);
//    cvShowImage("y",y);
//    cvShowImage("path",p);
//    cvShowImage("base",b);
   // cvShowImage("lane",l);
    //VisionUtils::CombineMasks(r,g,imgOut);
    //VisionUtils::CombineMasks(imgOut,y,imgOut,2,1);
    //VisionUtils::CombineMasks(imgOut,p,imgOut,3,1);
    //VisionUtils::CombineMasks(imgOut,b,imgOut,4,1);

//    cvReleaseImage(&r);
//    cvReleaseImage(&g);
//    cvReleaseImage(&y);
//    cvReleaseImage(&p);
//      cvReleaseImage(&b);
   //   cvReleaseImage(&l);
    return true;
}

bool Step1(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
    imgOut = cvCloneImage(original);
    Vision::Return a, b;

    //red.GetVisionReturn(original,a,imgOut);
   //green.GetVisionReturn(original,a,imgOut);
    //yellow.GetVisionReturn(original,a,imgOut);
    //path.GetVisionReturn(original,imgOut);
    //lane.GetBounding(original,imgOut);
    lane.GetVisionReturn(original,a,imgOut);
//    IplImage * debug = cvCloneImage(original);

//    imgOut = cvCloneImage(original);
//    std::list<CvBox2D> blobList;
//    blobList = red.GetBounding(original);
//    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
//    {
//        Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it,CV_RGB(255,0,0));
//    }
//    blobList.clear();
//    blobList = green.GetBounding(original);
//    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
//    {
//        Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it,CV_RGB(0,255,0));
//    }
//    blobList.clear();
//    blobList = yellow.GetBounding(original);
//    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
//    {
//        Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it,CV_RGB(255,255,100));
//    }
//    blobList.clear();
//    blobList = path.GetBounding(original,imgOut);
//    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
//    {
//        Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it,CV_RGB(255,165,0));
//    }
//    blobList.clear();


//    CvSize imageSize = cvSize(imgIn->width & -2, imgIn->height & -2 );
//    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 1 );
//    cvPyrDown( imgOut, imgSmallCopy);
//    cvPyrUp( imgSmallCopy, imgOut);
//    cvSmooth(imgIn, imgOut, CV_GAUSSIAN, 5);
    return true;
    
}

bool Step2(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
    imgOut = cvCloneImage(imgIn);
    //imgOut = b.HistogramMask(original);
//    CvMemStorage* storage = cvCreateMemStorage(0);
//    CvSeq* results = cvHoughCircles(
//                imgOut,
//                storage,
//                CV_HOUGH_GRADIENT,
//                2,
//                imgOut->width/20,
//                40,
//                100);
//    for( int i = 0; i < results->total; i++ ) {
//        float* p = (float*) cvGetSeqElem( results, i );
//        CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
//        cvCircle(
//                    imgOut,
//                    pt,
//                    cvRound( p[2] ),
//                    CV_RGB(0xff,0xff,0xff));
//    }
//    //cvThreshold(imgIn,imgOut,150, 255,CV_THRESH_TOZERO);
    return false;
    
}

bool Step3(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
//    std::list<CvBox2D> blobList;
    //imgOut = b.EdgeMask(original);
//    //cvZero(imgOut);
//    blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(imgOut,0,.005);
//    //    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
//    //    {
//    //        Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
//    //    }
//    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
//    {
//        CvPoint2D32f boxCorners32[4];
//        CvPoint boxCorners[4];
//        cvBoxPoints(*it,boxCorners32);
//        for(int i = 0; i < 4; i ++)
//        {
//            boxCorners[i] = cvPointFrom32f(boxCorners32[i]);
//        }
//        cvFillConvexPoly(imgOut,boxCorners,4,cvScalar(0,0,0),4);
//        //Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
//    }
    return false;
}

//hough standard
bool Step4(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{

    imgOut = cvCloneImage(imgIn);
//    std::list<CvBox2D> blobList;
//    blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(imgOut,.001,.95);
//    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
//    {
//        Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
//    }
    //cvAnd(imgOut,mask,imgOut);
    //cvDilate(imgOut,imgOut,0,1);
    return false;

    //    imgOut = cvCloneImage(imgIn);
    //    return true;

    //    CvMemStorage* memstorage = cvCreateMemStorage(0);
    //    CvSeq * lines = NULL;
    //    int best;
    //    CvScalar diff;
    //    CvScalar white;
    //    CvPoint bestPt1, bestPt2;
    //    CvScalar ave;
    //    for(int i = 0; i < MIN(lines->total,100); i++ )
    //    {
    //        float* line = (float*)cvGetSeqElem(lines,i);
    //        float rho = line[0];
    //        float theta = line[1];
    //        CvPoint pt1, pt2;
    //        double a = cos(theta), b = sin(theta);
    //        double x0 = a*rho, y0 = b*rho;
    //        pt1.x = cvRound(x0 + 10000*(-b));
    //        pt1.y = cvRound(y0 + 10000*(a));
    //        pt2.x = cvRound(x0 - 10000*(-b));
    //        pt2.y = cvRound(y0 - 10000*(a));

    //        cvClipLine(cvSize(imgIn->width, imgIn->height), &pt1,&pt2);
    //        ave = Zebulon::Vision::VisionUtils::AveColorUnderLine(imgIn,pt1,pt2);
    //        diff.val[0] = 255 - ave.val[0];
    //        diff.val[1] = 255 - ave.val[1];
    //        diff.val[2] = 255 - ave.val[2];
    //        int temp = diff.val[0] + diff.val[1] + diff.val[2];
    //        if(temp < best)
    //        {
    //            best = temp;
    //            bestPt1 = pt1;
    //            bestPt2 = pt2;
    //        }
    //        std::cout << pt1.x << " " << pt1.y << " " << pt2.x << " " << pt2.y << std::endl;

    //        cvLine( imgOut, pt1, pt2, CV_RGB(255,255,255), 3, 8 );
    //    }
    //cvLine( imgOut, bestPt1, bestPt2, CV_RGB(255,255,255), 3, 8 );
    return false;



}
//hough probabilistic
bool Step5(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
    imgOut = cvCloneImage(imgIn);
    //    //cvCanny( imgIn, imgOut, 50, 100, 3);
    //    return true;

    //    CvMemStorage* memstorage = cvCreateMemStorage(0);
    //    CvSeq * lines = NULL;
    //    imgOut = cvCloneImage(imgIn);
    //    //IplImage* intermediate = cvCloneImage(imgIn);
    //    //lines = cvHoughLines2(imgOut, memstorage, CV_HOUGH_STANDARD,1,CV_PI/180,50,0,0);
    //    lines = cvHoughLines2( imgOut,
    //                           memstorage,
    //                           CV_HOUGH_PROBABILISTIC,
    //                           1,
    //                           CV_PI/180,
    //                           1,
    //                           50,
    //                           1);
    //    int best;
    //    CvScalar diff;
    //    CvScalar white;
    //    CvPoint bestPt1, bestPt2;
    //    CvScalar ave;
    //    for(int i = 0; i < lines->total; i++ )
    //    {
    //        CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
    //        //double dist = Zebulon::Vision::VisionUtils::Dist(line[0],line[1]);
    //        //if(dist < 100) continue;
    //        ave = Zebulon::Vision::VisionUtils::AveColorUnderLine(imgIn,line[0],line[1]);
    //        diff.val[0] = 255 - ave.val[0];
    //        diff.val[1] = 255 - ave.val[1];
    //        diff.val[2] = 255 - ave.val[2];
    //        int temp = diff.val[0] + diff.val[1] + diff.val[2];
    //        if(temp < best)
    //        {
    //            best = temp;
    //            bestPt1 = line[0];
    //            bestPt2 = line[1];
    //        }
    //        //std::cout << pt1.x << " " << pt1.y << " " << pt2.x << " " << pt2.y << std::endl;
    //        //cvLine( imgOut, line[0], line[1], CV_RGB(255,255,255), 3, 8 );
    //    }
    //    if(lines->total > 0)
    //    {
    //        cvLine( imgOut, bestPt1, bestPt2, CV_RGB(255,255,255), 3, 8 );
    //    }
    return false;

}

bool Step6(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
    //     std::list<CvBox2D> blobList;
    imgOut = cvCloneImage(imgIn);
    //    //cvZero(imgOut);
    //    blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(imgOut,0,.005);
    ////    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    ////    {
    ////        Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
    ////    }
    //    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    //    {
    //        CvPoint2D32f boxCorners32[4];
    //        CvPoint boxCorners[4];
    //        cvBoxPoints(*it,boxCorners32);
    //        for(int i = 0; i < 4; i ++)
    //        {
    //            boxCorners[i] = cvPointFrom32f(boxCorners32[i]);
    //        }
    //        cvFillConvexPoly(imgOut,boxCorners,4,cvScalar(0,0,0),4);
    //        //Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
    //    }
    ////    blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(imgOut,0,.001);
    ////    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    ////    {
    ////        CvPoint2D32f boxCorners32[4];
    ////        CvPoint boxCorners[4];
    ////        cvBoxPoints(*it,boxCorners32);
    ////        for(int i = 0; i < 4; i ++)
    ////        {
    ////            boxCorners[i] = cvPointFrom32f(boxCorners32[i]);
    ////        }
    ////        cvFillConvexPoly(imgOut,boxCorners,4,cvScalar(0,0,0),4);
    ////        Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
    ////    }
    return true;
}
//ransac
bool Step7(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{

    imgOut = cvCloneImage(imgIn);
    //    return true;
    //    CvPoint p1, p2, p3, p4;

    //    imgOut = cvCloneImage(imgIn);
    //    std::vector<double> lineParameters;
    //    std::vector<Point2D *> pointDataPtr;
    //    std::vector<Point2D> pointData;
    //    LineParamEstimator lpEstimator(0.5);
    //    double desiredProbabilityForNoOutliers = 0.999;
    //    double usedData = 0;
    //    //populate point2d vector with image
    //    for(int x = 0; x < imgOut->width; x++)
    //    {
    //        for(int y = 0; y < imgOut->height; y++)
    //        {
    //            unsigned char val = Zebulon::Vision::VisionUtils::GetPixel(imgOut,x,y,0);
    //            if(val > 10)
    //            {
    //                pointDataPtr.push_back(new Point2D(x,y));
    //                pointData.push_back(*(pointDataPtr[pointDataPtr.size()-1]));
    //            }
    //        }
    //    }


    //    lpEstimator.leastSquaresEstimate(pointDataPtr,lineParameters);
    //    //A RANSAC estimate of the line parameters
    //    usedData = RANSAC<Point2D,double>::compute(lineParameters,
    //                                                      &lpEstimator ,
    //                                                      pointData,
    //                                                      desiredProbabilityForNoOutliers);
    //    std::cout << "Percent used: " << usedData << std::endl;
    //    if(lineParameters.size() > 0 && usedData > .04 && usedData < .15)
    //    {
    //        //std::cout<<"RANSAC line parameters [n_x,n_y,a_x,a_y]\n\t [ "<<lineParameters[0]<<", "<<lineParameters[1]<<", ";
    //        //std::cout<<lineParameters[2]<<", "<<lineParameters[3]<<" ]"<<std::endl;
    //        double rise = -lineParameters[1];
    //        double run = lineParameters[0];
    //        double x1 = lineParameters[3] - 1000*run;
    //        double y1 = lineParameters[2] - 1000*rise;
    //        double x2 = lineParameters[3] + 1000*run;
    //        double y2 = lineParameters[2] + 1000*rise;
    //        p1 = cvPoint(x1,y1);
    //        p2 = cvPoint(x2,y2);
    //        cvLine( imgOut, p1, p2, CV_RGB(0,0,0), 80, 8 );
    //        //cvCircle(imgOut,cvPoint(lineParameters[3],lineParameters[2]),10,CV_RGB(255,255,255));
    //    }
    //    for(int i = 0; i < pointDataPtr.size(); i++)
    //    {
    //        delete pointDataPtr[i];
    //        pointDataPtr[i] = NULL;
    //    }
    //    pointData.clear();
    //    pointDataPtr.clear();
    //    for(int x = 0; x < imgOut->width; x++)
    //    {
    //        for(int y = 0; y < imgOut->height; y++)
    //        {
    //            unsigned char val = Zebulon::Vision::VisionUtils::GetPixel(imgOut,x,y,0);
    //            if(val > 10)
    //            {
    //                pointDataPtr.push_back(new Point2D(x,y));
    //                pointData.push_back(*(pointDataPtr[pointDataPtr.size()-1]));
    //            }
    //        }
    //    }

    //    lpEstimator.leastSquaresEstimate(pointDataPtr,lineParameters);
    //    //A RANSAC estimate of the line parameters
    //    usedData = RANSAC<Point2D,double>::compute(lineParameters,
    //                                                      &lpEstimator ,
    //                                                      pointData,
    //                                                      desiredProbabilityForNoOutliers);

    //    std::cout << "Percent used: " << usedData << std::endl;
    //    if(lineParameters.size() > 0 && usedData > .05 && usedData < .15)
    //    {
    //        //std::cout<<"RANSAC line parameters [n_x,n_y,a_x,a_y]\n\t [ "<<lineParameters[0]<<", "<<lineParameters[1]<<", ";
    //        //std::cout<<lineParameters[2]<<", "<<lineParameters[3]<<" ]"<<std::endl;
    //        double rise = -lineParameters[1];
    //        double run = lineParameters[0];
    //        double x1 = lineParameters[3] - 1000*run;
    //        double y1 = lineParameters[2] - 1000*rise;
    //        double x2 = lineParameters[3] + 1000*run;
    //        double y2 = lineParameters[2] + 1000*rise;
    //        p3 = cvPoint(x1,y1);
    //        p4 = cvPoint(x2,y2);
    //        cvLine( imgOut, p3, p4, CV_RGB(0,0,0), 80, 8 );
    //        //cvCircle(imgOut,cvPoint(lineParameters[3],lineParameters[2]),10,CV_RGB(255,255,255));
    //    }

    //    cvLine( imgOut, p1, p2, CV_RGB(255,255,255), 1, 8 );
    //    cvLine( imgOut, p3, p4, CV_RGB(255,255,255), 1, 8 );

    return true;
}

int ProcessImage(const IplImage* imgIn, IplImage*& imgOut)
{
    int key = 0;
    IplImage * intermediate1 = cvCloneImage(imgIn);
    IplImage * intermediate2 = NULL;
    IplImage * mask = NULL;
    int numsteps = 7;
    bool success = true;

    //CreateMask(intermediate1,mask,NULL);
    //FixPerspective(mask,mask, NULL);
    //FixPerspective(intermediate1,intermediate1,NULL);
    //cvShowImage("Perspective", intermediate1);
    //cvShowImage("Output", mask);
    //key = cvWaitKey(0);

    for(int i = 0; i <= numsteps; i++)
    {
        switch(i)
        {
        case 0:
            success = Step0(intermediate1,intermediate2, imgIn, mask);
            break;
        case 1:
            success = Step1(intermediate1,intermediate2, imgIn,mask);
            break;
        case 2:
            success = Step2(intermediate1,intermediate2, imgIn,mask);
            break;
        case 3:
            success = Step3(intermediate1,intermediate2, imgIn,mask);
            break;
        case 4:
            success = Step4(intermediate1,intermediate2, imgIn,mask);
            break;
        case 5:
            success = Step5(intermediate1,intermediate2, imgIn,mask);
            break;
        case 6:
            success = Step6(intermediate1,intermediate2, imgIn,mask);
            break;
        case 7:
            success = Step7(intermediate1,intermediate2, imgIn,mask);
            break;
        default:
            break;
        }
        if(success)
        {
            cvShowImage("Input", intermediate1);
            cvShowImage("Output", intermediate2);
            cvShowImage("Original", imgIn);
            key = cvWaitKey(0);
            cvReleaseImage(&intermediate1);
            intermediate1 = cvCloneImage(intermediate2);
            cvReleaseImage(&intermediate2);
            if(key == 1048603)
                break;
        }
        else
        {
            break;
        }
    }
    cvReleaseImage(&imgOut);
    imgOut = cvCloneImage(intermediate1);
    cvReleaseImage(&intermediate1);
    cvReleaseImage(&intermediate2);
    return key;

}

int main(int argc, char** argv)
{
    int key = 0;
    if(argc < 2)
    {
        printf("must specify file\nExiting..");
        return -1;
    }
    //cvNamedWindow("Perspective", CV_WINDOW_AUTOSIZE );
    //cvNamedWindow("Input", CV_WINDOW_AUTOSIZE );
    //cvNamedWindow("Output", CV_WINDOW_AUTOSIZE);
    //cvNamedWindow("Stitched", CV_WINDOW_AUTOSIZE);
    std::vector<IplImage*> processedCells;
    std::vector<std::string> files;
    CxUtils::FileIO::GetFiles(files,"*.png",argv[1],false,true);
    CvMat *intrinsic = (CvMat*)cvLoad(gIntrinsicXML.c_str());
    CvMat *distort = (CvMat*)cvLoad(gDistortXML.c_str());
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .25,1);
    for(int i = 0; i < files.size(); i++)
    {
        std::stringstream s;
        printf("%s:\n",files[i].c_str());
        if( (gImageIn = cvLoadImage(files[i].c_str(), CV_LOAD_IMAGE_COLOR)) == 0 )
        {
            printf("unable to load file \n");
            continue;
        }
//        IplImage *t;
//        IplImage* mapx = cvCreateImage( cvGetSize(gImageIn), IPL_DEPTH_32F, 1 );
//        IplImage* mapy = cvCreateImage( cvGetSize(gImageIn), IPL_DEPTH_32F, 1 );
//        if(intrinsic && distort)
//        {
//            cvInitUndistortMap(
//                        intrinsic,
//                        distort,
//                        mapx,
//                        mapy
//                        );
//            t = cvCloneImage(gImageIn);
//            cvRemap( t, gImageIn, mapx, mapy );
//        }
        cvShowImage("Input", gImageIn);
//        cvShowImage("Distorted", t);
        //key = cvWaitKey(0);
        key = ProcessImage(gImageIn,gImageOut);

        s << files[i];
        cvPutText(gImageOut,s.str().c_str(),cvPoint(0,20),&font,CV_RGB(255,255,255));
        s.clear();
        s.flush();
        cvShowImage("Output", gImageOut);
        key = cvWaitKey(0);
        cvReleaseImage(&gImageIn);
        cvReleaseImage(&gImageOut);
//        cvReleaseImage(&t);
//        cvReleaseImage(&mapx);
//        cvReleaseImage(&mapy);
        if(key == 1048603)
            break;
        
    }
    return 0;
}
