////////////////////////////////////////////////////////////////////////////////////
///
/// \file homography.cpp
/// \brief Primitive homography calibration and adjustment class. For adjusting
///        groundplane in regards to perspective.
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu<br>
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
#include "igvc/ground/vision/homography.h"
using namespace Zebulon;
using namespace Vision;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes matrices.
///
////////////////////////////////////////////////////////////////////////////////////
Homography::Homography()
{
    mOriginalMat = NULL;
    mInverseMat = cvCreateMat(3, 3, CV_32F);
    mModifiedMat = cvCreateMat(3, 3, CV_32F);

    mOffsets[Horizontal] = 0;
    mOffsets[Vertical] = 0;
    mOffsets[Rotate] = 0;
    mOffsets[Slant] = 0;
    mOffsets[Zoom] = 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Homography::~Homography()
{
    cvReleaseMat(&mOriginalMat);
    cvReleaseMat(&mInverseMat);
    cvReleaseMat(&mModifiedMat);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Load a matrix from file.  Not required to run.
///
////////////////////////////////////////////////////////////////////////////////////
bool Homography::LoadSettings(const std::string& xmlFileName)
{

    mOriginalMat = (CvMat*)cvLoad(xmlFileName.c_str());
    if(mOriginalMat == NULL) return false;

    cvInvert(mOriginalMat,mInverseMat);
    cvInvert(mInverseMat,mModifiedMat);

}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Saves a modified matrix to a file
///
////////////////////////////////////////////////////////////////////////////////////
void Homography::SaveSettings(const std::string& xmlFileName)
{
    cvSave(xmlFileName.c_str(),mModifiedMat);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Warp an image by the homography matrix.
///
////////////////////////////////////////////////////////////////////////////////////
IplImage * Homography::Warp(const IplImage * imgIn, CvMat * matrix) const
{
    if(matrix == NULL) return NULL;

    IplImage * imgOut = NULL;
    CvMat *H_inv = cvCreateMat(3, 3, CV_32F);
    cvInvert(matrix,H_inv);
    //need to find transformed boundary of new iamge
    double cornerPts[2];
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
//    if(newWidth > 0 && newHeight > 0)
//    {
//        imgOut = cvCreateImage(cvSize(newWidth,newHeight),imgIn->depth,imgIn->nChannels);
//    }else
    {
        imgOut = cvCreateImage(cvSize(imgIn->width,imgIn->height),imgIn->depth,imgIn->nChannels);
    }
    cvWarpPerspective(imgIn,imgOut,matrix,CV_INTER_LINEAR | CV_WARP_INVERSE_MAP | CV_WARP_FILL_OUTLIERS);
    cvReleaseMat(&H_inv);
    cvReleaseMat(&dst);
    return imgOut;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Warps an x,y point by a homography matrix.
///
////////////////////////////////////////////////////////////////////////////////////
CxUtils::Point3D Homography::Warp(const int x, const int y, CvMat * matrix) const
{
    double pointArr[2];
    pointArr[0] = x;
    pointArr[1] = y;
    CxUtils::Point3D result;

    CvMat src = cvMat(1, 2, CV_64FC2,pointArr);
    CvMat* dst = cvCreateMat(1, 2, CV_64FC2);
    //perspective correct
    cvPerspectiveTransform(&src,dst,matrix);
    result.mX =  CV_MAT_ELEM(*dst, double, 0, 0);
    result.mY =  CV_MAT_ELEM(*dst, double, 0, 1);
    cvReleaseMat(&dst);
    return result;
}

CxUtils::Point3D Homography::Warp(CxUtils::Point3D point, WarpType type) const
{
    return Warp(point.mX,point.mY,type);
}

CxUtils::Segment3D Homography::Warp(CxUtils::Segment3D line, WarpType type) const
{
    return CxUtils::Segment3D(Warp(line.mPoint1,type),Warp(line.mPoint2,type));
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets a property of the matrix. Does not do anything currently
///
////////////////////////////////////////////////////////////////////////////////////
void Homography::SetProperty(const WarpProperty type, const double value)
{
//    double val[2][2];
//    double a = 0;
//    switch(type)
//    {
//    case Horizontal:
//        CV_MAT_ELEM(*mModifiedMat, float, 0,0) += value;
//        break;
//    case Vertical:
//        CV_MAT_ELEM(*mModifiedMat, float, 2,0) += 0.001;
//        //CV_MAT_ELEM(*mModifiedMat, float, 0,1) += value;
//        break;
//    case Rotate:
//        val[0][0] = cos(value);
//        val[0][1] = sin(value);
//        val[1][0] = -sin(value);
//        val[1][1] = cos(value);

////        val[0][0] = cos(value) + CV_MAT_ELEM(*mModifiedMat, float, 0, 0);
////        val[0][1] = -sin(value) + CV_MAT_ELEM(*mModifiedMat, float, 0, 1);
////        val[1][0] = sin(value) + CV_MAT_ELEM(*mModifiedMat, float, 1, 0);
////        val[1][1] = cos(value) + CV_MAT_ELEM(*mModifiedMat, float, 1, 1);
//        CV_MAT_ELEM(*mModifiedMat, float, 0,0) = val[0][0];
//        CV_MAT_ELEM(*mModifiedMat, float, 0,1) = val[0][1];
//        CV_MAT_ELEM(*mModifiedMat, float, 1,0) = val[1][0];
//        CV_MAT_ELEM(*mModifiedMat, float, 1,1) = val[1][1];
////        CV_MAT_ELEM(*mModifiedMat, float, 0,0) += value;
////        CV_MAT_ELEM(*mModifiedMat, float, 0,1) += value;
////        CV_MAT_ELEM(*mModifiedMat, float, 1,0) += value;
////        CV_MAT_ELEM(*mModifiedMat, float, 1,1) += value;
//        break;
//    case Slant:
//        break;
//    case Zoom:
//        CV_MAT_ELEM(*mModifiedMat, float, 2,2) = value;
//        break;
//    default:
//        break;
//    }

    mOffsets[type] += value;

}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a property of the matrix.
///
////////////////////////////////////////////////////////////////////////////////////
double Homography::GetProperty(const Homography::WarpProperty type) const
{
    double result = 0;
//    switch(type)
//    {
//    case Horizontal:
//        break;
//    case Vertical:
//        break;
//    case Rotate:
//        result = CV_MAT_ELEM(mModifiedMat, double, 0, 0);
//        break;
//    case Slant:
//        break;
//    case Zoom:
//        result = CV_MAT_ELEM(mModifiedMat, double, 2, 2);
//        break;
//    default:
//        break;
//    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Warp an image by specifying type of matrix to use.
///
////////////////////////////////////////////////////////////////////////////////////
IplImage * Homography::Warp(const IplImage * imgIn, WarpType type) const
{
    switch(type)
    {
        case Original:
            return Warp(imgIn,mOriginalMat);
        case Modified:
            return Warp(imgIn,mModifiedMat);
        case Inverse:
            return Warp(imgIn,mInverseMat);
    default:
        return NULL;

    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Warp a point by specifying type of matrix to use.
///
////////////////////////////////////////////////////////////////////////////////////
CxUtils::Point3D Homography::Warp(const int x, const int y, WarpType type) const
{
    switch(type)
    {
        case Original:
            return Warp(x,y,mOriginalMat);
        case Modified:
            return Warp(x,y,mModifiedMat);
        case Inverse:
            return Warp(x,y,mInverseMat);
    default:
        return CxUtils::Point3D();
    }

}

CvPoint Homography::Warp(const CvPoint cvpoint, WarpType type) const
{
    CxUtils::Point3D cxpoint;
    CvPoint result;
    cxpoint = Warp(cvpoint.x,cvpoint.y,type);
    result.x = cxpoint.mX;
    result.y = cxpoint.mY;
    return result;
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a modified matrix by looking at image of checkerboard pattern.
///          Requires certain parameters to work correctly. Saves results to
///          mModifiedMat. Call warp on an image to show results of calibration.
///   \param[in] imgIn Input image with checkerboard
///   \param[in] widthCM Width between far left and right corners of board in CM.
///   \param[in] heightCM Height between far up and down corners of board in CM.
///   \param[in] hBlocks Number of horizontal blocks(black/white) between left/right corners
///   \param[in] vBlocks Number of vertical blocks between corners top/bottom.
///   \return true if corners found, false if not.
///
////////////////////////////////////////////////////////////////////////////////////
bool  Homography::CheckerBoardCalibrate(const IplImage *imgIn,  double widthCM, double heightCM, int hBlocks, int vBlocks)
{
    IplImage * imgOut;
    CvPoint2D32f points[hBlocks * vBlocks];
    int cornerCount = 0;
    CvSize size = cvSize(hBlocks,vBlocks);
    imgOut = cvCreateImage(cvGetSize(imgIn), 8, 1);
    cvCvtColor(imgIn, imgOut, CV_BGR2GRAY);
    if(!cvFindChessboardCorners(imgOut,size,points,&cornerCount))
    {
        return false;
    }
    cvFindCornerSubPix(imgOut,points,cornerCount,cvSize(11,11), cvSize(-1,-1),
                       cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, .1));


    CvPoint2D32f objPts[4],imgPts[4];
//    objPts[0].x = 0 + mOffsets[Horizontal] + mOffsets[Slant];     objPts[0].y = 0 + mOffsets[Vertical];
//    objPts[1].x = width + mOffsets[Horizontal] - mOffsets[Slant]; objPts[1].y = 0 + mOffsets[Vertical];
//    objPts[2].x = 0 + mOffsets[Horizontal] - mOffsets[Slant];     objPts[2].y = height + mOffsets[Vertical];
//    objPts[3].x = width + mOffsets[Horizontal] + mOffsets[Slant]; objPts[3].y = height + mOffsets[Vertical];

    objPts[0].x = imgIn->width/2 - widthCM/2.;           objPts[0].y = imgIn->height - heightCM;
    objPts[1].x = imgIn->width/2 + widthCM/2.;           objPts[1].y = imgIn->height - heightCM;
    objPts[2].x = imgIn->width/2 - widthCM/2.;           objPts[2].y = imgIn->height;
    objPts[3].x = imgIn->width/2 + widthCM/2.;           objPts[3].y = imgIn->height;

    imgPts[0] = points[0];
    imgPts[1] = points[hBlocks - 1];
    imgPts[2] = points[(vBlocks-1)*hBlocks];
    imgPts[3] = points[(vBlocks-1)*hBlocks + (hBlocks - 1)];

    cvGetPerspectiveTransform(objPts, imgPts, mModifiedMat);
    CV_MAT_ELEM(*mModifiedMat, float, 2,2) = mOffsets[Zoom];
    cvInvert(mModifiedMat,mInverseMat);
    cvReleaseImage(&imgOut);


    return true;

}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Transforms a pixel from the image to where it would lay on the ground.
///          Assumes perspective matrix has been created to 1 pixel per CM. If not
///          function will still do calculation but units will be a function of
///          the calibration.
///
///   \param[in] point Point to transform
///   \param[in] original Image - needed for width and height calculations for offset
///   \param[in] offsetPercentX Percentage of image size to offset new pixel in the
///              x direction.  For moving origin from topleft of image to another spot.
///   \param[in] offsetPercentY Percentage of image size to offset new pixel in the
///              y direction.  For moving origin from topleft of image to another spot.
///   \return Point representing original pixels location on ground plane.
///
////////////////////////////////////////////////////////////////////////////////////
CxUtils::Point3D Homography::FromImageToGround(const CxUtils::Point3D& point, const IplImage* originalImage, double offsetPercentX, double offsetPercentY,const bool invertY)
{
    //call using inverse mat, due to differences opencv calls persepctive correct on image vs point
    CxUtils::Point3D result = Warp(point.mX,point.mY,mInverseMat);
    //adjust origin to center of image
    result.mX -= originalImage->width * offsetPercentX;
    if(invertY)
    {
        result.mY = originalImage->height * offsetPercentY - result.mY;
    }
    else
    {
        result.mY -= originalImage->height * offsetPercentY;
    }
    //1.65 meters offset from center of vehicle to start of image frame
    //adjust origin to bottom of image
    //result.mX = 1.65+(image->height)/100 - result.mX;
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Transforms a segment in the image to where it would lay on the ground.
///          Assumes perspective matrix has been created to 1 pixel per CM. If not
///          function will still do calculation but units will be a function of
///          the calibration.
///
///   \param[in] line Line to transform
///   \param[in] original Image - needed for width and height calculations for offset
///   \param[in] offsetPercentX Percentage of image size to offset new pixel in the
///              x direction.  For moving origin from topleft of image to another spot.
///   \param[in] offsetPercentY Percentage of image size to offset new pixel in the
///              y direction.  For moving origin from topleft of image to another spot.
///   \return Line representing original pixels location on ground plane.
///
////////////////////////////////////////////////////////////////////////////////////
CxUtils::Segment3D Homography::FromImageToGround(const CxUtils::Segment3D& line, const IplImage* originalImage,double offsetPercentX, double offsetPercentY,const bool invertY)
{
    return CxUtils::Segment3D(FromImageToGround(line.mPoint1,originalImage,offsetPercentY,offsetPercentX, invertY), FromImageToGround(line.mPoint2,originalImage,offsetPercentX,offsetPercentY), invertY);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Transforms a pixel from the groundplane to where it would be in the
///          image. Has not been tested, probably doesnt work.
///
///   \param[in] point Point to transform
///   \param[in] original Image - needed for width and height calculations for offset
///   \param[in] offsetPercentX Percentage of image size to offset new pixel in the
///              x direction.  For moving origin from a local origin to top left.
///   \param[in] offsetPercentY Percentage of image size to offset new pixel in the
///              y direction.  For moving origin from topleft of image to another spot.
///   \return Point representing original pixels location on ground plane.
///
////////////////////////////////////////////////////////////////////////////////////
CxUtils::Point3D Homography::FromGroundToImage(const CxUtils::Point3D& point, const IplImage* originalImage, double offsetPercentX, double offsetPercentY,const bool invertY)
{
    CxUtils::Point3D result;
    result = point;
    //adjust origin to center of image
    result.mX += originalImage->width * offsetPercentX;
    if(invertY)
    {
        result.mY = originalImage->height * offsetPercentY + result.mY;
    }
    else
    {
        result.mY += originalImage->height * offsetPercentY;
    }
    //call using original mat, due to differences opencv calls persepctive correct on image vs point
    result = Warp(result.mX,result.mY,mModifiedMat);
    //1.65 meters offset from center of vehicle to start of image frame
    //adjust origin to bottom of image
    //result.mX = 1.65+(image->height)/100 - result.mX;
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Transforms a segment in the image to where it would lay on the ground.
///          Assumes perspective matrix has been created to 1 pixel per CM. If not
///          function will still do calculation but units will be a function of
///          the calibration.
///
///   \param[in] line Line to transform
///   \param[in] original Image - needed for width and height calculations for offset
///   \param[in] offsetPercentX Percentage of image size to offset new pixel in the
///              x direction.  For moving origin from topleft of image to another spot.
///   \param[in] offsetPercentY Percentage of image size to offset new pixel in the
///              y direction.  For moving origin from topleft of image to another spot.
///   \return Line representing original pixels location on ground plane.
///
////////////////////////////////////////////////////////////////////////////////////
CxUtils::Segment3D Homography::FromGroundToImage(const CxUtils::Segment3D& line, const IplImage* originalImage,double offsetPercentX, double offsetPercentY,const bool invertY)
{
    return CxUtils::Segment3D(FromGroundToImage(line.mPoint1,originalImage,offsetPercentY,offsetPercentX,invertY), FromImageToGround(line.mPoint2,originalImage,offsetPercentX,offsetPercentY), invertY);
}

