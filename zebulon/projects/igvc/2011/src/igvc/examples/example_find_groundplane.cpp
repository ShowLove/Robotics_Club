/////////////////////////////////////////////////////////////////////////////////////
///
/// \file example_find_groundplane.h
/// \brief A program to test some opencv homography.
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
#include "ground/vision/homography.h"
using namespace Zebulon;
std::string gImgDir = "/home/developer/Documents/checker1.jpg";
std::string gSaveName = "settings/vision/homo.xml";
IplImage * gImageIn = NULL;
IplImage * gImageOut = NULL;

Vision::Homography gHomo;
CxUtils::Point3D ConvertPixelToMeters(const CxUtils::Point3D& perspectivePixelPoint, const IplImage* original, CvMat * mat)
{
    //hardcoded values, must improve
//    double pixelsPerCM = 48./(5. * 20.);
//    double pixelsPerM = pixelsPerCM*100;
//    CxUtils::Point3D result;
//    if(original == NULL) return result;
//    int width = original->width;
//    int height = original->height;

//    double pixel[2], lowerBound[2], upperBound[2];

//    pixel[0] = perspectivePixelPoint.mX;
//    pixel[1] = perspectivePixelPoint.mY;

//    lowerBound[0] = width/2.;
//    lowerBound[1] = height;

//    upperBound[0] = width/2.;
//    upperBound[1] = 0;

//    CvMat startPixel = cvMat(1, 2, CV_64FC2,pixel);
//    CvMat* persPixel = cvCreateMat(1, 2, CV_64FC2);

//    CvMat startLowerBound = cvMat(1, 2, CV_64FC2,lowerBound);
//    CvMat* persLowerBound = cvCreateMat(1, 2, CV_64FC2);

//    CvMat startUpperBound = cvMat(1, 2, CV_64FC2,upperBound);
//    CvMat* persUpperBound = cvCreateMat(1, 2, CV_64FC2);

//    cvPerspectiveTransform(&startPixel,persPixel,mat);
//    cvPerspectiveTransform(&startLowerBound,persLowerBound,mat);
//    cvPerspectiveTransform(&startUpperBound,persUpperBound,mat);

//    double x1,x2,y1,y2;
//    x1 = CV_MAT_ELEM(*persLowerBound, double, 0, 0);
//    y1 = CV_MAT_ELEM(*persLowerBound, double, 0, 1);
//    x2 = CV_MAT_ELEM(*persPixel, double, 0, 0);
//    y2 = CV_MAT_ELEM(*persPixel, double, 0, 1);

//    CxUtils::Segment3D segment(CxUtils::Point3D(x1,y1),CxUtils::Point3D(x2,y2));
//    result.mX = segment.GetDistanceX() / pixelsPerM;
//    result.mY = segment.GetDistanceY() / pixelsPerM;
//    result.mZ = 0;

//    cvReleaseMat(&persPixel);
//    cvReleaseMat(&persLowerBound);
//    cvReleaseMat(&persUpperBound);

//    return result;
}

void circlePoints(CvMat *H,  IplImage * imgIn, CvPoint2D32f point, CvScalar color, CvPoint2D32f* transPoint)
{
    if(H==NULL)
    {

        cvCircle(imgIn,cvPointFrom32f(point), 9, color,3);
        return;
    }
    CvPoint2D32f result;
    double cornerPts[2];
    cornerPts[0] = point.x;
    cornerPts[1] = point.y;
    CvMat src = cvMat(1, 2, CV_64FC2,cornerPts);
    CvMat* dst = cvCreateMat(1, 2, CV_64FC2);
//    cornerPts[0] = gImageIn->width/2.;
//    cornerPts[1] = gImageIn->height;
    cvPerspectiveTransform(&src,dst,H);
    result.x = CV_MAT_ELEM(*dst, double, 0, 0);
    result.y = CV_MAT_ELEM(*dst, double, 0, 1);
    if(transPoint)
    {
        *transPoint = result;
    }
    cvCircle(imgIn,cvPointFrom32f(result), 9, color,3);
}

//5 pixel units maps to 48 cm
int main(int argc, char** argv)
{
    gHomo.LoadSettings();
    int key = 0;
    if(argc < 2)
    {
        printf("must specify file\nExiting..");
        return -1;
    }
    //cvNamedWindow("Homo", CV_WINDOW_AUTOSIZE );
    cvNamedWindow("Input", CV_WINDOW_AUTOSIZE );
    cvNamedWindow("Output", CV_WINDOW_AUTOSIZE);
    for(int i = 1; i < argc; i++)
    {
        printf("%s:\n",argv[i]);
        if( (gImageIn = cvLoadImage( argv[i], CV_LOAD_IMAGE_COLOR)) == 0 )
        {
            printf("unable to load file \n");
            continue;
        }
    }
    cvShowImage("original",gImageIn);
    int board_w = 8;
    int board_h = 5;
    float Z = 1;
    double widthM = 76.8;
    double heightM = 48;
    CvPoint2D32f points[board_w * board_h];
    int cornerCount = 0;
    CvSize size = cvSize(board_w,board_h);
    gImageOut = cvCreateImage(cvGetSize(gImageIn), 8, 1);
    cvCvtColor(gImageIn, gImageOut, CV_BGR2GRAY);
    if(cvFindChessboardCorners(gImageIn,size,points,&cornerCount))
    {
        cvDrawChessboardCorners(gImageOut,size,points,cornerCount,true);
    }
    else
    {
        return 0;
    }
    cvFindCornerSubPix(gImageOut,points,cornerCount,cvSize(11,11), cvSize(-1,-1),
                       cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, .1));

    CvPoint2D32f objPts[4],imgPts[4];
    objPts[0].x = gImageIn->width/2 - widthM/2. - 75;           objPts[0].y = gImageIn->height - heightM;
    objPts[1].x = gImageIn->width/2 + widthM/2. - 75;           objPts[1].y = gImageIn->height - heightM;
    objPts[2].x = gImageIn->width/2 - widthM/2. - 75;           objPts[2].y = gImageIn->height;
    objPts[3].x = gImageIn->width/2 + widthM/2. - 75;           objPts[3].y = gImageIn->height;

//    objPts[0].x = gImageIn->width/2 - widthM/2.;           objPts[0].y = 0;
//    objPts[1].x = gImageIn->width/2 + widthM/2.;           objPts[1].y = 0;
//    objPts[2].x = gImageIn->width/2 - widthM/2.;           objPts[2].y = heightM;
//    objPts[3].x = gImageIn->width/2 + widthM/2.;           objPts[3].y = heightM;

//    objPts[0].x = -widthM/2.;           objPts[0].y = -heightM/2.;
//    objPts[1].x = widthM/2.;           objPts[1].y = -heightM/2.;
//    objPts[2].x = - widthM/2.;           objPts[2].y = heightM/2.;
//    objPts[3].x = widthM/2.;           objPts[3].y = heightM/2.;

//    objPts[0].x = 0;           objPts[0].y = 0;
//    objPts[1].x = widthM;      objPts[1].y = 0;
//    objPts[2].x = 0;           objPts[2].y = heightM;
//    objPts[3].x = widthM;      objPts[3].y = heightM;

    imgPts[0] = points[0];
    imgPts[1] = points[board_w - 1];
    imgPts[2] = points[(board_h-1)*board_w];
    imgPts[3] = points[(board_h-1)*board_w + (board_w - 1)];

//    CvPoint2D32f p;
//    p.x = 0;
//    p.y = 0;
//    imgPts[0] = p;
//    p.x = board_w;
//    p.y = 0;
//    imgPts[1] = p;
//    p.x = 0;
//    p.y = board_h;
//    imgPts[2] = p;
//    p.x = board_w;
//    p.y = board_h;
//    imgPts[3] = p;

//    cvCircle(gImageIn,cvPointFrom32f(imgPts[3]), 9, CV_RGB(0,0,255),3);
//    cvCircle(gImageIn,cvPointFrom32f(imgPts[2]), 9, CV_RGB(0,255,0),3);
//    cvCircle(gImageIn,cvPointFrom32f(imgPts[1]), 9, CV_RGB(255,0,0),3);
//    cvCircle(gImageIn,cvPointFrom32f(imgPts[0]), 9, CV_RGB(0,255,255),3);

//    circlePoints(NULL, gImageIn,imgPts[0], CV_RGB(0,0,255));
//    circlePoints(NULL, gImageIn,imgPts[1], CV_RGB(0,255,0));
//    circlePoints(NULL, gImageIn,imgPts[2], CV_RGB(255,0,0));
//    circlePoints(NULL, gImageIn,imgPts[3], CV_RGB(0,255,255));

    CvMat *H = cvCreateMat(3, 3, CV_32F);
    CvMat *H_inv = cvCreateMat(3, 3, CV_32F);
    cvGetPerspectiveTransform(objPts, imgPts, H);
    CV_MAT_ELEM(*H, float, 2,2) = Z;
    cvInvert(H,H_inv);
    /////////getting transofrmed ends of image/////////////
    double cornerPts[2];

    CvMat src = cvMat(1, 2, CV_64FC2,cornerPts);
    CvMat* dst = cvCreateMat(1, 2, CV_64FC2);

    double newWidth = 0;
    double newHeight = 0;

//    cornerPts[0] = 0;
//    cornerPts[1] = 0;
//    cvPerspectiveTransform(&src,dst,H_inv);
//    newWidth = fabs(CV_MAT_ELEM(*dst, double, 0, 0));
//    newHeight = fabs(CV_MAT_ELEM(*dst, double, 0, 1));

    cornerPts[0] = gImageIn->width;
    cornerPts[1] = 0;
    cvPerspectiveTransform(&src,dst,H_inv);
    newWidth += CV_MAT_ELEM(*dst, double, 0, 0);
    //newHeight = CV_MAT_ELEM(*dst, double, 0, 1);

    cornerPts[0] = 0;
    cornerPts[1] = gImageIn->height;
    cvPerspectiveTransform(&src,dst,H_inv);
    newHeight += CV_MAT_ELEM(*dst, double, 0, 1);

    ////////////////////////////
    IplImage *homo_image = cvCreateImage(cvSize(std::ceil(newWidth),std::ceil(newHeight)),gImageIn->depth,gImageIn->nChannels);
    //IplImage *homo_image = cvCreateImage(cvSize(gImageIn->width,gImageIn->height),gImageIn->depth,gImageIn->nChannels);
    cvWarpPerspective(gImageIn,homo_image,H,CV_INTER_LINEAR | CV_WARP_INVERSE_MAP | CV_WARP_FILL_OUTLIERS);


    CvPoint p1, p2;
    cornerPts[0] = imgPts[0].x;
    cornerPts[1] = imgPts[0].y;
//    cornerPts[0] = gImageIn->width/2.;
//    cornerPts[1] = gImageIn->height;
    cvPerspectiveTransform(&src,dst,H_inv);
    p1.x =  CV_MAT_ELEM(*dst, double, 0, 0);
    p1.y =  CV_MAT_ELEM(*dst, double, 0, 1);

    cornerPts[0] = imgPts[3].x;
    cornerPts[1] = imgPts[3].y;
//    cornerPts[0] = gImageIn->width/2.;
//    cornerPts[1] = 0;
    cvPerspectiveTransform(&src,dst,H_inv);
    p2.x =  CV_MAT_ELEM(*dst, double, 0, 0);
    p2.y =  CV_MAT_ELEM(*dst, double, 0, 1);
    //cvLine(homo_image,p1,p2,CV_RGB(255,0,0),10);
    //cvCircle(homo_image,cvPointFrom32f(cvPoint2D32f(newWidth,newHeight)), 10, CV_RGB(0,255,255),10);
    CxUtils::Point3D point = ConvertPixelToMeters(CxUtils::Point3D(0,100,0),gImageIn, H_inv);

    CvPoint2D32f transPts[4];
    //translates original checker corner points, draws them on perspective corrected image
    circlePoints(H_inv, homo_image,imgPts[0], CV_RGB(0,0,255),&transPts[0]);
    circlePoints(H_inv, homo_image,imgPts[1], CV_RGB(0,255,0),&transPts[1]);
    circlePoints(H_inv, homo_image,imgPts[2], CV_RGB(255,0,0),&transPts[2]);
    circlePoints(H_inv, homo_image,imgPts[3], CV_RGB(0,255,255),&transPts[3]);

    //takes perspective corrected points, draws them on original non corrected image
    circlePoints(H, gImageIn,transPts[0], CV_RGB(0,0,255), NULL);
    circlePoints(H, gImageIn,transPts[1], CV_RGB(0,255,0), NULL);
    circlePoints(H, gImageIn,transPts[2], CV_RGB(255,0,0), NULL);
    circlePoints(H, gImageIn,transPts[3], CV_RGB(0,255,255), NULL);

    CxUtils::Point3D cxpoint;
    CxUtils::Point3D cxpoint2;
    cxpoint.mX = (imgPts[0].x + imgPts[1].x)/2.0;
    cxpoint.mY = imgPts[0].y;
    cxpoint2 = gHomo.FromImageToGround(cxpoint,homo_image,0,0,false);
    cxpoint = gHomo.FromGroundToImage(cxpoint2,gImageIn,0,0,false);

    circlePoints(NULL, homo_image,cvPoint2D32f(cxpoint2.mX,cxpoint2.mY), CV_RGB(128,128,255), NULL);
    circlePoints(NULL, gImageIn,cvPoint2D32f(cxpoint.mX,cxpoint.mY), CV_RGB(255,128,0), NULL);

    cvShowImage("Input",gImageIn);
    cvShowImage("Output",homo_image);
    //cvShowImage("Homo",homo_image);
    cvSave(gSaveName.c_str(),H);
    cvWaitKey(0);
    return 0;
}
