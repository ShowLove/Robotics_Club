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
#include <opencv2/legacy/legacy.hpp>
#include <cxutils/cxutils.h>
#include <vector>
#include "dvision/visionutils.h"
#include "dvision/visionobject.h"
//#include "ransac/RANSAC.h"
//#include "ransac/LineParamEstimator.h"
#include "asvc/surface/vision/pathobject.h"
#include "asvc/surface/vision/bouyobject.h"
#include "asvc/surface/vision/bouybaseobject.h"
#include "asvc/surface/vision/lovelaneobject2.h"
#include "asvc/surface/vision/lovelaneobject.h"

using namespace Zebulon;
using namespace Vision;

IplImage * gImageIn = NULL;
IplImage * gImageOut = NULL;

std::string gHomoXML = "settings/vision/homo.xml";
std::string gIntrinsicXML = "settings/vision/intrinsics.xml";
std::string gDistortXML = "settings/vision/distortion.xml";
std::string gColorFile = "settings/vision/barrelcolor.png";

IplImage* SegmentationMask2(const IplImage * imgIn, IplImage* debugOut)
{
//    CvSize imageSize = cvSize(imgIn->width & -2, imgIn->height & -2 );
//    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 1 );


    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * circleMask = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * src = cvCloneImage(imgIn);
    IplImage * scratch = cvCloneImage(src);
    //IplImage * hist = HistogramMask(imgIn);
    //IplImage * bestMask = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);

    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* comp = NULL;

    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .5,.5);
    std::ostringstream s;

    cvZero(imgOut);
    cvZero(circleMask);
    cvZero(scratch);
    //cvZero(bestMask);

    CvScalar avgColor;
    double bestColor = -1;
    CvRect bestRect;
    double bestDiag = 0;
//    IplImage* hsv = cvCreateImage( cvGetSize(imgIn), 8, 3 );

//    IplImage * chan0 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
//    IplImage * segsum = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);

    //cvCvtColor( imgIn, hsv, CV_BGR2YCrCb );
    //cvCopyImage(imgIn,hsv);
    //cvSplit(hsv,chan0,chan1,chan2, NULL);
    //cvConvertImage(imgIn,src);

    //lower last param for more segments
    //cvPyrSegmentation( hsv, scratch, storage, &comp, 3, 100, 90 );


    cvPyrSegmentation( src, scratch, storage, &comp, 2, 0, 80);
    int n_comp = comp->total;

    std::list<CvBox2D> blobList;
    for( int i = n_comp-1; i>=1; i-- )
    {
        CvConnectedComp* cc = (CvConnectedComp*) cvGetSeqElem( comp, i );
        cvAbsDiffS(scratch,src,cc->value);
        cvNot(src,src);
        cvThreshold(src,src,254,255,CV_THRESH_BINARY);
        cvShowImage("seg2",src);
        cvWaitKey(0);
        //blobList = VisionUtils::GetBlobBoxes(src,0.0008,.95,false);
//        for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
//        {
//            CvRect rect = VisionUtils::ToCvRect(*it);
//            VisionUtils::MakeSquare(rect);
//            double diagonal = sqrt(rect.width * rect.width + rect.height * rect.height);
//            cvDrawCircle(circleMask,cvPoint(rect.x+rect.width/2.,rect.y+rect.height/2),diagonal/2.5,CV_RGB(255,255,255),CV_FILLED);

//            avgColor = cvAvg (hist,circleMask);
//            if((bestColor < 0 || bestColor < avgColor.val[0]) && avgColor.val[0] > mSegment2Threshold)
//            {
//                bestDiag = diagonal;
//                bestColor = avgColor.val[0];
//                bestRect = rect;
//                cvCopy(circleMask,imgOut);
//            }
//            cvMinMaxLoc(imgIn,)
//            cvZero(circleMask);
//        }
    }
//    if(debugOut && bestColor > 0)
//    {
//         s.clear();
//        //s << "bestColor(" << bestColor << ") " << mType;
//        cvPutText(debugOut,s.str().c_str(),cvPoint(bestRect.x+bestRect.width/2.,bestRect.y+bestRect.height/2),&font,CV_RGB(255,255,255));
//        cvDrawCircle(debugOut,cvPoint(bestRect.x+bestRect.width/2.,bestRect.y+bestRect.height/2),bestDiag/2.5,CV_RGB(255,255,255));
//    }
//    cvShowImage("best",bestMask);
//    cvWaitKey(0);

    //VisionUtils::ClearEdges(imgOut);
    cvReleaseImage(&scratch);
    cvReleaseImage(&src);
    //cvReleaseImage(&hist);
    cvReleaseImage(&circleMask);
    cvReleaseMemStorage( &storage );
    return imgOut;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a std linked list of cvBox2D objects sorted from big to small
///          found in source image using a combination of thresholding
///          and edge detection. Original source taken from square.c in opencv
///          example programs.
///
///   \param[in] img Source image, can be color or grayscale.
///   \param[out] dest Image to draw boxes on to, can be NULL
///   \param[in] minsize min percentage of total pixels before square is considred
///   \param[in] maxsize max percentage of total pixels before square is considred
///   \param[in] cannyThreshold threshold used for edge detection
///
///   \return std linked list of cvBox2D objects sorted from bigest to smallest
///
///////////////////////////////////////////////////////////////////////////////////
std::list<CvBox2D> FindSquares(const IplImage* img, IplImage *dest, const double minsize, const double maxsize, const int cannyThreshold, const bool angle3d)
{
    std::list<CvBox2D> ret;
    if(img == NULL) return ret;
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours;
    CvSeq* polyApprox;
    double imgArea = img->height * img->width;
    CvSize imageSize = cvSize(img->width & -2, img->height & -2 );
    IplImage* imgCopy = cvCloneImage( img ); // make a copy of input image
    IplImage* imgGray = cvCreateImage( imageSize, IPL_DEPTH_8U, 1 );
    IplImage * imgGraySum = cvCreateImage( imageSize, IPL_DEPTH_8U, 1 );
    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2,imageSize.height/2), imgCopy->depth, imgCopy->nChannels );
    IplImage* resultImage = cvCreateImage( imageSize, IPL_DEPTH_8U, 1 );
    //if(dest != NULL) cvZero(dest);
    CvBox2D tempBox;

    int thresholdsToTry = 5;
    // create empty sequence that will contain points -
    // 4 points per square (the square's vertices)
    //CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage );


    // select the maximum ROI in the image
    // with the width and height divisible by 2
    //cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));

    // down-scale and upscale the image to filter out the noise
    //cvPyrDown( imgCopy, imgSmallCopy);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //cvMorphologyEx(imgSmallCopy,imgSmallCopy,NULL,CV_SHAPE_RECT,CV_MOP_OPEN,1);

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    // These two lines may work, needs to be tested!!
    IplConvKernel* structureElement = cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_RECT);
    cvMorphologyEx(imgSmallCopy, imgSmallCopy, NULL, structureElement, CV_MOP_OPEN, 1 );

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //cvPyrUp( imgSmallCopy, imgCopy);
    //canny = cvCreateImage( imageSize, IPL_DEPTH_8U, 1 );
    cvZero(imgGraySum);
    for(int colorPlane = 1; colorPlane <= imgCopy->nChannels+1; colorPlane++)
    {
//        if(colorPlane == imgCopy->nChannels +1)
//        {
//            cvCopy(imgGraySum, resultImage);
//        }
//        else if(imgCopy->nChannels > 1)
//        {
//            //colorplane 0 = all channels
//            cvSetImageCOI(imgCopy, colorPlane);
            cvCopy(imgCopy,imgGray);
//        }

        for(int threshLevel = 0; threshLevel < thresholdsToTry; threshLevel++)
        {
            if(colorPlane != imgCopy->nChannels +1)
            {
                switch(threshLevel)
                {
                case 0:

                    // apply Canny. Take the upper threshold from slider
                    // and set the lower to 0 (which forces edges merging)
                    cvShowImage("boxcanny input",imgGray);
                    cvCanny(imgGray, resultImage, 0,cannyThreshold, 3 );
                    cvShowImage("boxcanny output",resultImage);
                    // dilate canny output to remove potential
                    // holes between edge segments

                    //cvDilate( resultImage, resultImage, 0, 1);
                    cvOr(resultImage,imgGraySum,imgGraySum);

                    break;

                default:
                    cvThreshold(imgGray, resultImage, ((threshLevel)*255)/thresholdsToTry, 255, CV_THRESH_BINARY );
                    //cvDilate( resultImage, resultImage, 0, 1 );
                    //cvAdaptiveThreshold(imgGray,resultImage,255);
                    break;
                }
            }
            //cvCanny(imgGray, resultImage, 100, ((threshLevel+1)*255)/thresholdsToTry, 5 );
            //cvDilate( resultImage, resultImage, 0, 1 );
            //cvShowImage("Diff",resultImage);
            //cvWaitKey(10);
            // find contours and store them all as a list

            //cvShowImage("Diff",resultImage);
            //cvWaitKey(0);
            cvFindContours( resultImage, storage, &contours, sizeof(CvContour),
                            CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

            while( contours )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                polyApprox = cvApproxPoly( contours, sizeof(CvContour), storage,
                                           CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );

                //printf("poly points: %d\n", polyApprox->total);
                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                double  contourArea =  fabs(cvContourArea(polyApprox,CV_WHOLE_SEQ));
                if( polyApprox->total == 4 &&               //total points in contour
                        contourArea > (imgArea * minsize) &&    //contour area covers greater than minsize of total image
                        contourArea < (imgArea * maxsize) &&    //contour area covers less than maxsize of total image
                        cvCheckContourConvexity(polyApprox) )   //contour is convex
                {
                    double s = 0;
                    double t = 0;
                    for(int i = 0; i < 5; i++ )
                    {
                        // find minimum angle between joint
                        // edges (maximum of cosine)
                        if( i >= 2 )
                        {
                            t = fabs(VisionUtils::GetAngle(
                                         (CvPoint*)cvGetSeqElem( polyApprox, i ),
                                         (CvPoint*)cvGetSeqElem( polyApprox, i-2 ),
                                         (CvPoint*)cvGetSeqElem( polyApprox, i-1 )));
                            s = s > t ? s : t;
                        }
                    }
                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( s < 0.5 )
                    {
                        tempBox = cvMinAreaRect2(contours,storage);
                        double tempBoxArea = tempBox.size.width * tempBox.size.height;
                        std::list<CvBox2D>::iterator it;
                        for(it = ret.begin(); it != ret.end(); it++)
                        {
                            if(tempBoxArea > (it->size.width * it->size.height))
                            {
                                break;
                            }
                        }
                        if(dest != NULL)
                        {
                            if(angle3d)
                            {
                                VisionUtils::DrawSquare(dest,polyApprox,CV_RGB(255,0,0));
                            }
                            else
                            {

                                VisionUtils::DrawSquare(dest,tempBox,CV_RGB(255,0,0));
                            }

                        }
                        if(angle3d)
                        {

                            VisionUtils::Calc3DSquareAngle(polyApprox,tempBox.angle,dest);
                        }
                        ret.insert(it,tempBox);
                    }
                }
                contours = contours->h_next;
            }
        }
    }


    // release all the temporary images
    cvReleaseImage( &imgCopy );
    cvReleaseImage( &imgSmallCopy );
    cvReleaseImage( &resultImage );
    cvReleaseImage( &imgGray );
    cvReleaseMemStorage(&storage);
    return ret;
}
IplImage* SegmentationMask(const IplImage * imgIn)
{
    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * src = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage* hsv = cvCreateImage( cvGetSize(imgIn), 8, 3 );

    IplImage * chan0 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan1 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan2 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan3 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);

    cvCvtColor( imgIn, hsv, CV_BGR2YCrCb );
    cvSplit(hsv,chan0,chan1,chan2, NULL);
    //cvConvertImage(imgIn,src);

    src = cvCloneImage(chan0);
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* comp = NULL;
    //lower last param for more segments
    cvPyrSegmentation( src, imgOut, storage, &comp, 2, 200, 50 );
    //cvNormalize(imgOut,imgOut,255,0, CV_MINMAX);
    //cvThreshold(imgOut,imgOut,250,255,CV_THRESH_TOZERO);
//        int n_comp = comp->total;
//        for( int i=0; i<n_comp; i++ )
//        {
//            CvConnectedComp* cc = (CvConnectedComp*) cvGetSeqElem( comp, i );
//            cvDrawRect(imgOut,cvPoint(cc->rect.x,cc->rect.y),cvPoint(cc->rect.x+cc->rect.width,cc->rect.y+cc->rect.height),CV_RGB(255,255,255));

//        //do_something_with( cc );
//        }
    cvReleaseImage(&src);
    cvReleaseImage(&hsv);
    cvReleaseImage(&chan0);
    cvReleaseImage(&chan1);
    cvReleaseImage(&chan2);
    cvReleaseImage(&chan3);
    cvReleaseMemStorage( &storage );
    return imgOut;
}

BouyObject white;
//BouyObject green;
//BouyObject yellow;
//BouyBaseObject base;
//PathObject path;
//LoveLaneObject2 lane;
bool Step0(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
    white.Initialize("WhiteSquare");
    //red.Initialize("RedBouy");
    //green.Initialize("GreenBouy");
    //yellow.Initialize("YellowBouy");
    //path.Initialize("Path");
    //base.Initialize("BouyBase");
    //lane.Initialize("LoveLane2");
//    base.Initialize("BouyBase");

    imgOut = cvCloneImage(imgIn);
    IplImage * temp =  NULL;
//    cvZero(imgOut);

    //temp = white.GetMask(imgIn, imgOut);
    imgOut = SegmentationMask(imgIn);
    //cvThreshold(imgOut,imgOut,75,255,CV_THRESH_TOZERO);
    //cvReleaseImage(&temp);
    //temp = green.GetMask(imgIn, imgOut);
    //cvReleaseImage(&temp);
    //temp = red.GetMask(imgIn, imgOut);
    //cvReleaseImage(&temp);
    //temp = path.GetMask(imgIn, imgOut);
    //cvReleaseImage(&temp);
    //temp = lane.GetMask(imgIn, imgOut);
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

    //cvShowImage("squares in",imgIn);
    std::list<CvBox2D> squares = FindSquares(imgIn,imgOut,.0001,.80,10,false);
    return true;
    
}

bool Step2(const IplImage* imgIn, IplImage*& imgOut, const IplImage* original, IplImage* mask)
{
    imgOut = cvCloneImage(imgIn);
    SegmentationMask2(original,imgOut);
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
