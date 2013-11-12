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
#include "RANSAC.h"
#include "LineParamEstimator.h"

#include "gvision/target.h"
#include "gvision/return.h"

#include "gvision/threshratio.h"
#include "gvision/segment.h"

IplImage * gImageIn = NULL;
IplImage * gImageOut = NULL;

std::string gHomoXML = "settings/vision/homo.xml";
std::string gColorFile = "settings/vision/barrelcolor.png";
using namespace Zebulon;
bool FixPerspective(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original)
{
    CvMat * homoMat = NULL;
    homoMat = (CvMat*)cvLoad(gHomoXML.c_str());
    if(homoMat == NULL) return false;

    if(imgOut != NULL)
    {
        //cvReleaseImage(&imgOut);
    }

    double cornerPts[2];
    CvMat *H_inv = cvCreateMat(3, 3, CV_32F);
    cvInvert(homoMat,H_inv);
    CvMat src = cvMat(1, 2, CV_64FC2,cornerPts);
    CvMat* dst = cvCreateMat(1, 2, CV_64FC2);
    double newWidth = 0;
    double newHeight = 0;

    cornerPts[0] = imgIn->width;
    cornerPts[1] = 0;
    cvPerspectiveTransform(&src,dst,H_inv);
    newWidth += CV_MAT_ELEM(*dst, double, 0, 0);
    //newHeight = CV_MAT_ELEM(*dst, double, 0, 1);

    cornerPts[0] = 0;
    cornerPts[1] = imgIn->height;
    cvPerspectiveTransform(&src,dst,H_inv);
    newHeight += CV_MAT_ELEM(*dst, double, 0, 1);

    imgOut = cvCreateImage(cvSize(std::ceil(newWidth),std::ceil(newHeight)),imgIn->depth,imgIn->nChannels);
    cvWarpPerspective(imgIn,imgOut,homoMat,CV_INTER_LINEAR | CV_WARP_INVERSE_MAP | CV_WARP_FILL_OUTLIERS);

    return true;

}

bool CreateMask(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original)
{
    imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
//    IplImage * clone = cvCloneImage(imgIn);
//    IplImage * chan0 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
//    IplImage * chan1 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
//    IplImage * chan2 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
//    IplImage * chan3 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
//    cvSplit(imgIn, chan0,chan1,chan2,NULL);

    Vision::VisionUtils::ExtractColor(imgIn,imgOut,cvAvg(cvLoadImage(gColorFile.c_str())),100);
    std::list<CvBox2D> blobs = Vision::VisionUtils::GetBlobBoxes(imgOut,0.01,1);
    for(std::list<CvBox2D>::iterator i = blobs.begin(); i != blobs.end(); i++)
    {
        CvBox2D rotatedBox = *i;
        rotatedBox.angle += 90;
        Vision::VisionUtils::DrawSquare(imgOut,rotatedBox);
    }
    blobs = Vision::VisionUtils::GetBlobBoxes(imgOut,0.01,1);
    for(std::list<CvBox2D>::iterator i = blobs.begin(); i != blobs.end(); i++)
    {
        CvPoint2D32f points[4];
        cvBoxPoints(*i,points);
        CvPoint point2d[4];
        for(int i = 0; i < 4; i++)
        {
            point2d[i] = cvPointFrom32f(points[i]);
        }
        cvFillConvexPoly(imgOut,point2d,4,cvScalar(255,255,255));
        //Vision::VisionUtils::DrawSquare(imgOut,*i);
    }
    cvDrawRect(imgOut,cvPoint(0,0),cvPoint(imgOut->width,imgOut->height),cvScalar(255,255,255),20);
    cvAbsDiffS(imgOut,imgOut,cvScalar(255,255,255));
}

bool Step0(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
    IplImage * hsv = cvCloneImage(imgIn);
    IplImage * chan0 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan1 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan2 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan3 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    cvCvtColor(imgIn, hsv, CV_BGR2HSV);

    cvSplit(hsv,chan0,chan1,chan2, NULL);
    
    CvScalar white = cvRealScalar(255);
    imgOut = cvCloneImage(chan1);
    cvEqualizeHist( imgOut, imgOut);
    //invert black and white
    cvAbsDiffS(imgOut, imgOut, white);
    cvShowImage("hue",chan0);
    cvShowImage("sat",chan1);
    cvShowImage("val",chan2);
    cvShowImage("inv",imgOut);
    cvWaitKey(0);

    cvReleaseImage(&hsv);
    cvReleaseImage(&chan0);
    cvReleaseImage(&chan1);
    cvReleaseImage(&chan2);
    cvReleaseImage(&chan3);
    return true;    
}

bool Step1(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
    CvSize imageSize = cvSize(imgIn->width & -2, imgIn->height & -2 );
    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 1 );
    imgOut = cvCloneImage(imgIn); 
    cvPyrDown( imgOut, imgSmallCopy);
    cvPyrUp( imgSmallCopy, imgOut);
    //cvSmooth(imgIn, imgOut, CV_GAUSSIAN, 5);
    return true;
    
}

bool Step2(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
    imgOut = cvCloneImage(imgIn);
    cvThreshold(imgIn,imgOut,230, 255,CV_THRESH_TOZERO);
    return true;
    
}

bool Step3(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
    imgOut = cvCloneImage(imgIn);
    cvCanny( imgIn, imgOut, 50, 100, 3);
    cvAnd(imgOut,mask,imgOut);
    cvDilate(imgOut,imgOut,0,1);
    return true;
}

//hough standard
bool Step4(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
    imgOut = cvCloneImage(imgIn);
    return true;

    CvMemStorage* memstorage = cvCreateMemStorage(0);
    CvSeq * lines = NULL;
    int best;
    CvScalar diff;
    CvScalar white;
    CvPoint bestPt1, bestPt2;
    CvScalar ave;
    for(int i = 0; i < MIN(lines->total,100); i++ )
    {
        float* line = (float*)cvGetSeqElem(lines,i);
        float rho = line[0];
        float theta = line[1];
        CvPoint pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 10000*(-b));
        pt1.y = cvRound(y0 + 10000*(a));
        pt2.x = cvRound(x0 - 10000*(-b));
        pt2.y = cvRound(y0 - 10000*(a));
        
        cvClipLine(cvSize(imgIn->width, imgIn->height), &pt1,&pt2);
        ave = Zebulon::Vision::VisionUtils::AveColorUnderLine(imgIn,pt1,pt2);
        diff.val[0] = 255 - ave.val[0];
        diff.val[1] = 255 - ave.val[1];
        diff.val[2] = 255 - ave.val[2];
        int temp = diff.val[0] + diff.val[1] + diff.val[2];
        if(temp < best)
        {
            best = temp;
            bestPt1 = pt1;
            bestPt2 = pt2;
        }
        std::cout << pt1.x << " " << pt1.y << " " << pt2.x << " " << pt2.y << std::endl;
        
        cvLine( imgOut, pt1, pt2, CV_RGB(255,255,255), 3, 8 );
    }
    //cvLine( imgOut, bestPt1, bestPt2, CV_RGB(255,255,255), 3, 8 );
    return true;



}
//hough probabilistic
bool Step5(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
    imgOut = cvCloneImage(imgIn);
    //cvCanny( imgIn, imgOut, 50, 100, 3);
    return true;

    CvMemStorage* memstorage = cvCreateMemStorage(0);
    CvSeq * lines = NULL;
    imgOut = cvCloneImage(imgIn);
    //IplImage* intermediate = cvCloneImage(imgIn);
    //lines = cvHoughLines2(imgOut, memstorage, CV_HOUGH_STANDARD,1,CV_PI/180,50,0,0);
    lines = cvHoughLines2( imgOut,
                           memstorage,
                           CV_HOUGH_PROBABILISTIC,
                           1,
                           CV_PI/180,
                           1,
                           50,
                           1);
    int best;
    CvScalar diff;
    CvScalar white;
    CvPoint bestPt1, bestPt2;
    CvScalar ave;
    for(int i = 0; i < lines->total; i++ )
    {
        CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
        //double dist = Zebulon::Vision::VisionUtils::Dist(line[0],line[1]);
        //if(dist < 100) continue;
        ave = Zebulon::Vision::VisionUtils::AveColorUnderLine(imgIn,line[0],line[1]);
        diff.val[0] = 255 - ave.val[0];
        diff.val[1] = 255 - ave.val[1];
        diff.val[2] = 255 - ave.val[2];
        int temp = diff.val[0] + diff.val[1] + diff.val[2];
        if(temp < best)
        {
            best = temp;
            bestPt1 = line[0];
            bestPt2 = line[1];
        }
        //std::cout << pt1.x << " " << pt1.y << " " << pt2.x << " " << pt2.y << std::endl;
        //cvLine( imgOut, line[0], line[1], CV_RGB(255,255,255), 3, 8 );
    }
    if(lines->total > 0)
    {
        cvLine( imgOut, bestPt1, bestPt2, CV_RGB(255,255,255), 3, 8 );
    }
    return true;

}

bool Step6(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
     std::list<CvBox2D> blobList;
    imgOut = cvCloneImage(imgIn);
    //cvZero(imgOut);
    blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(imgOut,0,.005);
//    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
//    {
//        Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
//    }
    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    {
        CvPoint2D32f boxCorners32[4];
        CvPoint boxCorners[4];
        cvBoxPoints(*it,boxCorners32);
        for(int i = 0; i < 4; i ++)
        {
            boxCorners[i] = cvPointFrom32f(boxCorners32[i]);
        }
        cvFillConvexPoly(imgOut,boxCorners,4,cvScalar(0,0,0),4);
        //Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
    }
//    blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(imgOut,0,.001);
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
//        Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
//    }
    return true;
}
//ransac
bool Step7(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{

    imgOut = cvCloneImage(imgIn);
    return true;
    CvPoint p1, p2, p3, p4;

    imgOut = cvCloneImage(imgIn);
    std::vector<double> lineParameters;
    std::vector<Point2D *> pointDataPtr;
    std::vector<Point2D> pointData;
    LineParamEstimator lpEstimator(0.5);
    double desiredProbabilityForNoOutliers = 0.999;
    double usedData = 0;
    //populate point2d vector with image
    for(int x = 0; x < imgOut->width; x++)
    {
        for(int y = 0; y < imgOut->height; y++)
        {
            unsigned char val = Zebulon::Vision::VisionUtils::GetPixel(imgOut,x,y,0);
            if(val > 10)
            {
                pointDataPtr.push_back(new Point2D(x,y));
                pointData.push_back(*(pointDataPtr[pointDataPtr.size()-1]));
            }
        }
    }


    lpEstimator.leastSquaresEstimate(pointDataPtr,lineParameters);
    //A RANSAC estimate of the line parameters
    usedData = RANSAC<Point2D,double>::compute(lineParameters,
                                                      &lpEstimator ,
                                                      pointData,
                                                      desiredProbabilityForNoOutliers);
    std::cout << "Percent used: " << usedData << std::endl;
    if(lineParameters.size() > 0 && usedData > .04 && usedData < .15)
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
        cvLine( imgOut, p1, p2, CV_RGB(0,0,0), 80, 8 );
        //cvCircle(imgOut,cvPoint(lineParameters[3],lineParameters[2]),10,CV_RGB(255,255,255));
    }
    for(int i = 0; i < pointDataPtr.size(); i++)
    {
        delete pointDataPtr[i];
        pointDataPtr[i] = NULL;
    }
    pointData.clear();
    pointDataPtr.clear();
    for(int x = 0; x < imgOut->width; x++)
    {
        for(int y = 0; y < imgOut->height; y++)
        {
            unsigned char val = Zebulon::Vision::VisionUtils::GetPixel(imgOut,x,y,0);
            if(val > 10)
            {
                pointDataPtr.push_back(new Point2D(x,y));
                pointData.push_back(*(pointDataPtr[pointDataPtr.size()-1]));
            }
        }
    }

    lpEstimator.leastSquaresEstimate(pointDataPtr,lineParameters);
    //A RANSAC estimate of the line parameters
    usedData = RANSAC<Point2D,double>::compute(lineParameters,
                                                      &lpEstimator ,
                                                      pointData,
                                                      desiredProbabilityForNoOutliers);

    std::cout << "Percent used: " << usedData << std::endl;
    if(lineParameters.size() > 0 && usedData > .05 && usedData < .15)
    {
        //std::cout<<"RANSAC line parameters [n_x,n_y,a_x,a_y]\n\t [ "<<lineParameters[0]<<", "<<lineParameters[1]<<", ";
        //std::cout<<lineParameters[2]<<", "<<lineParameters[3]<<" ]"<<std::endl;
        double rise = -lineParameters[1];
        double run = lineParameters[0];
        double x1 = lineParameters[3] - 1000*run;
        double y1 = lineParameters[2] - 1000*rise;
        double x2 = lineParameters[3] + 1000*run;
        double y2 = lineParameters[2] + 1000*rise;
        p3 = cvPoint(x1,y1);
        p4 = cvPoint(x2,y2);
        cvLine( imgOut, p3, p4, CV_RGB(0,0,0), 80, 8 );
        //cvCircle(imgOut,cvPoint(lineParameters[3],lineParameters[2]),10,CV_RGB(255,255,255));
    }

    cvLine( imgOut, p1, p2, CV_RGB(255,255,255), 1, 8 );
    cvLine( imgOut, p3, p4, CV_RGB(255,255,255), 1, 8 );

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

    CreateMask(intermediate1,mask,NULL);
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
    cvNamedWindow("Perspective", CV_WINDOW_AUTOSIZE );
    cvNamedWindow("Input", CV_WINDOW_AUTOSIZE );
    cvNamedWindow("Output", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Stitched", CV_WINDOW_AUTOSIZE);
    std::vector<IplImage*> processedCells;
    std::vector<std::string> files;
    CxUtils::FileIO::GetFiles(files,"*.png",argv[1],false);

    for(int i = 1; i < files.size(); i++)
    {
        printf("%s:\n",files[i].c_str());
        if( (gImageIn = cvLoadImage(files[i].c_str(), CV_LOAD_IMAGE_COLOR)) == 0 )
        {
            printf("unable to load file \n");
            continue;
        }
        //IplImage * fixed = NULL;
        //FixPerspective(gImageIn,fixed,NULL);
        //cvCopy(fixed,gImageIn);
//        std::vector<IplImage*> cells = Zebulon::Vision::VisionUtils::GetImageCells(gImageIn,1,2);
//        for(int i = 0; i < cells.size(); i++)
//        {
//            key = ProcessImage(cells[i], gImageOut);
//            processedCells.push_back(cvCloneImage(gImageOut));
//            cvReleaseImage(&cells[i]);
//        }
//        IplImage * stitched = Zebulon::Vision::VisionUtils::StitchCells(processedCells,1,2, true);

        cvShowImage("Input", gImageIn);
        key = cvWaitKey(0);
        key = ProcessImage(gImageIn,gImageOut);
        cvShowImage("Output", gImageOut);
        //cvShowImage("Stitched", stitched);
        key = cvWaitKey(0);
        cvReleaseImage(&gImageIn);
        cvReleaseImage(&gImageOut);
        for(int i = 0; i < processedCells.size(); i++)
        {
            cvReleaseImage(&processedCells[i]);
        }
        processedCells.clear();
        if(key == 1048603)
            break;
        
    }
    return 0;
}
