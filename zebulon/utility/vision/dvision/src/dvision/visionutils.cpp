#include "dvision/visionutils.h"

using namespace Zebulon;
using namespace Vision;

bool VisionUtils::mWindowCreated = false;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a single pixel from a IplImage, with no bounds checking.
///          Code found on google.
///
///   \param[in] img iplimage where we want to get pixel
///   \param[in] row row the pixel is on
///   \param[in] col column the pixel is on
///   \param[in] channel channel the pixel is on, 0 if single channel image.
///
///   \return pixel from image as an unsigned char.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned char VisionUtils::GetPixel(const IplImage*img, int row, int col, int channel )
{
    if(img == NULL) return 0;
    if(row >= img->height || col >= img->width) return 0;
    return ((uchar*)(img->imageData + img->widthStep*row))[col+channel] ;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets a single pixel from a IplImage, with no bounds checking.
///          Code found on google.
///
///   \param[in] img iplimage where we want to get pixel
///   \param[in] row row the pixel is on
///   \param[in] col column the pixel is on
///   \param[in] channel channel the pixel is on, 0 if single channel image.
///
///   \return pixel from image as an unsigned char.
///
////////////////////////////////////////////////////////////////////////////////////
bool VisionUtils::SetPixel(const IplImage*img, int row, int col, int channel, uchar val)
{
    if(img == NULL) return 0;
    if(row >= img->height || col >= img->width) return 0;
    ((uchar*)(img->imageData + img->widthStep*row))[col+channel] = val;
    return true;
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates the squared difference of an image and a scalar, must
///          have the same number of channels.
///
///   \param[in] img1 image we are taking squared diff
///   \param[out] dest the image that will store squared diffs
///   \param[in] val the scalar we are taking quared diff of
///
///////////////////////////////////////////////////////////////////////////////////
void VisionUtils::SquareDiffS(const IplImage * img1, IplImage * dest,const CvScalar val)
{

    for(int i = 0; i < img1->width * img1->height; i++)
    {
        double result = 0;
        for(int j = 0; j < img1->nChannels; j++)
        {
            double a = (uchar)(img1->imageData[(i*img1->nChannels)+j]);
            result += pow(a - val.val[j],2);
        }

        result = sqrt(result);
        if(result > 255) result = 255;
        dest->imageData[i] = (uchar)result;
    }

}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates the squared difference of an image and a scalar, must
///          have the same number of channels.
///
///   \param[in] img1 image we are taking squared diff
///   \param[in] img2 image we are taking squared diff
///   \param[out] dest the image that will store squared diffs
///
///////////////////////////////////////////////////////////////////////////////////
void VisionUtils::SquareDiff(const IplImage * img1,const IplImage * img2, IplImage * dest)
{

    for(int i = 0; i < img1->width * img1->height; i++)
    {
        double result = 0;
        for(int j = 0; j < img1->nChannels; j++)
        {
            double a = (uchar)(img1->imageData[(i*img1->nChannels)+j]);
            double b = (uchar)(img2->imageData[(i*img2->nChannels)+j]);
            result += pow(a-b,2);
        }
        result = sqrt(result);
        if(result > 255) result = 255;
        dest->imageData[i] = (char)result;
    }

}
double VisionUtils::Dist(const CvScalar& p1, const CvScalar& p2, int dimensions)
{
    double dist = 0;
    for(int i = 0; i < dimensions; i++)
    {
        dist += pow(p1.val[i] - p2.val[i],2);
    }
    return sqrt(dist);
    //return dist;
}
double VisionUtils::Dist(const CvPoint& p1, const CvPoint& p2)
{
    double dist = 0;
    dist += pow(p1.x - p2.x,2);
    dist += pow(p1.y - p2.y,2);
    return sqrt(dist);
    //return dist;
}
double VisionUtils::PointLineDistance(const CvScalar& p1, const CvScalar& p2, const CvScalar& p3, int dimensions)
{
    //herons formula from http://www.worsleyschool.net/science/files/linepoint/method4.html
    double a = Dist(p1,p2,dimensions);
    if(a < .0001) return Dist(p1,p3);
    double b = Dist(p1,p3,dimensions);
    double c = Dist(p2,p3,dimensions);
    if(b < .0001 || c < .0001) return 0;
    double s = (a + b + c)/2.;
    double area = sqrt(s * (s-a) * (s-b) * (s-c));
    return area/(.5*a);

    /*double base = Dist(p1,p2,dimensions);
    if(base < .0001) return Dist(p1,p3);
    double area = p1.val[0] * p2.val[1] + p2.val[0]*p3.val[1] + p3.val[0]*p1.val[1] -
                  p2.val[0] * p1.val[1] - p3.val[0]*p2.val[1] - p1.val[0]*p3.val[1];
    area = abs(area);
    if(area < .0001) return 0;
    return  area / base;*/


}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates a new single channel 8 bit image where the color desired is
///          white, and every other color is black.  Image is the same size as the
///          source image. New memory is allocated, so
///          returned image must be released when done with it.
///
///   \param[in] img Source image we want to extract color from. Must be 3 channel
///              8 bit image.
///   \param[in] sampleColor scalar representing the color we want to extract as
///              a 3 channel 8 bit scalar.
///   \param[in] threshold threshold value to apply on the result. Lower is stricter.
///              threshold of 0 will not do any thresholding and return grayscale img.
///   \param[in] dilate dilate the results to increase size of any color found,
///              useful to connect the dots of splotchy images. Higher number will
///              dilate spots more.
///
///   \return    A new image that represents a binary mask of where the color
///              was found in the source image. Must be released when done.
///
///////////////////////////////////////////////////////////////////////////////////
void  VisionUtils::ExtractColor(const IplImage * img,
                                IplImage * dest,
                                const CvScalar sampleStart, const CvScalar sampleEnd,
                                const int threshold,
                                const int dilate)
{

//    float r_ranges[] = {0,255};
//    float * ranges[] = {r_ranges};
//    int r_bins = 256;
//    int hist_size[] = {r_bins};
    //CvHistogram * hist = cvCreateHist(1,hist_size,CV_HIST_ARRAY,ranges,1);

    IplImage *returnImg = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
    IplImage * channels[3];

    //CxUtils::Point3D pixel;
    CvScalar pixel;
    double dist;
    for(int i=0; i< img->height; i ++)
    {
        for(int j=0; j<img->width; j++)
        {
            //CvPoint pt = {100,100};
            char* temp_ptr = &((char*)(img->imageData + img->widthStep*i))[j*3];
            pixel.val[0] = temp_ptr[0];
            pixel.val[1] = temp_ptr[1];
            pixel.val[3] = temp_ptr[3];

            dist = PointLineDistance(sampleStart,sampleEnd,pixel,3);

            ((char*)(returnImg->imageData + returnImg->widthStep*i))[j] = dist;
        }
    }

    if(dest != NULL)
    {
        if(returnImg->nChannels != dest->nChannels ||
                returnImg->depth != dest->depth)
        {
            cvConvertImage(returnImg,dest);
        }
        else
        {
            cvCopyImage(returnImg,dest);
        }

    }

    //cvReleaseHist(&hist);
    cvReleaseImage(&returnImg);

    //return returnImg;
}
IplImage* VisionUtils::RGBToCustom(const IplImage * img)
{
    if(img == NULL || img->nChannels != 3) return NULL;
    IplImage * bgDiff = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
    IplImage * grDiff = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
    IplImage * channels[3];
    IplImage * returnImg = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,3);
    channels[0] = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
    channels[1] = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
    channels[2] = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
    cvSplit(img,channels[0],channels[1],channels[2],NULL);
    cvAbsDiff(channels[0],channels[1],bgDiff);
    cvAbsDiff(channels[1],channels[2],grDiff);
    cvZero(channels[0]);
    //cvAbsDiff(channels[0],channels[2],brDiff);
    cvMerge(bgDiff,grDiff,channels[0],NULL,returnImg);

    cvReleaseImage(&channels[0]);
    cvReleaseImage(&channels[2]);
    cvReleaseImage(&channels[1]);
    cvReleaseImage(&grDiff);
    cvReleaseImage(&bgDiff);
    return returnImg;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates a new single channel 8 bit image where the color desired is
///          white, and every other color is black.  Image is the same size as the
///          source image. New memory is allocated, so
///          returned image must be released when done with it.
///
///   \param[in] img Source image we want to extract color from. Must be 3 channel
///              8 bit image.
///   \param[in] dest Destination image same size and type as source image where
///              binary color mask will be stored.
///   \param[in] sampleColor scalar representing the color we want to extract as
///              a 3 channel 8 bit scalar.
///   \param[in] threshold threshold value to apply on the result. Lower is stricter.
///              threshold of 0 will not do any thresholding and return grayscale img.
///   \param[in] dilate dilate the results to increase size of any color found,
///              useful to connect the dots of splotchy images. Higher number will
///              dilate spots more.
///
///////////////////////////////////////////////////////////////////////////////////
void VisionUtils::ExtractColor(const IplImage * img,
                               IplImage * dest,
                               const CvScalar sampleColor,
                               const int threshold,
                               const int dilate)
{
    if(img == NULL) return;
    IplImage * bgDiff = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
    IplImage * grDiff = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
    IplImage * brDiff = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
    IplImage * result = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);

    CvScalar bgDiffS = cvRealScalar(sampleColor.val[0]-sampleColor.val[1]);
    CvScalar grDiffS = cvRealScalar(sampleColor.val[1]-sampleColor.val[2]);
    CvScalar brDiffS = cvRealScalar(sampleColor.val[0]-sampleColor.val[2]);

    cvZero(result);

    IplImage * channels[3];
    channels[0] = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
    channels[1] = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
    channels[2] = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);

    cvSplit(img,channels[0],channels[1],channels[2],NULL);


    for(int i = 0; i < img->height; i++)
    {
        for(int j= 0; j < img->width; j++)
        {
            int val[3];
            int val2[3];
            val[0] = GetPixel(channels[0],i,j,0) - GetPixel(channels[1],i,j,0);
            val[1] = GetPixel(channels[1],i,j,0) - GetPixel(channels[2],i,j,0);
            val[2] = GetPixel(channels[0],i,j,0) - GetPixel(channels[2],i,j,0);

            val2[0] = (double)(val[0] - bgDiffS.val[0])/2.0;
            val2[1] = (double)(val[1] - grDiffS.val[0])/2.0;
            val2[2] = (double)(val[2] - brDiffS.val[0])/2.0;


            val[0] = abs(val2[0]);
            val[1] = abs(val2[1]);
            val[2] = abs(val2[2]);
            SetPixel(bgDiff,i,j,0,val[0]);
            SetPixel(grDiff,i,j,0,val[1]);
            SetPixel(brDiff,i,j,0,val[2]);
        }
    }


    VisionUtils::CombineMasks(bgDiff,grDiff,result);
    VisionUtils::CombineMasks(result,brDiff,result,2);
    //     cvShowImage("combined",result);
    //    result = VisionUtils::CombineMasks(result,)

    //cvShowImage("Stitched",result);
    //cvWaitKey(0);
    if(threshold > 0)
    {
        cvThreshold(result,result,threshold,255,CV_THRESH_BINARY_INV);
    }
    else
    {
        cvNot(result,result);
    }
    if(dilate > 0) cvDilate( result,result, 0, dilate);

    if(dest != NULL)
    {
        if(result->nChannels != dest->nChannels ||
                result->depth != dest->depth)
        {
            cvConvertImage(result,dest);
        }
        else
        {
            cvCopyImage(result,dest);
        }

    }
    cvReleaseImage(&result);
    cvReleaseImage(&bgDiff);
    cvReleaseImage(&grDiff);
    cvReleaseImage(&brDiff);
    cvReleaseImage(&channels[0]);
    cvReleaseImage(&channels[1]);
    cvReleaseImage(&channels[2]);
}


IplImage * VisionUtils::BackProject(const IplImage * img, const IplImage * sampleHistImg, const int threshold, const int dilate)
{
    CvArr* back_project = (CvArr*)cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);

    IplImage* h_plane = cvCreateImage( cvSize(img->width, img->height), 8, 1 );
    IplImage* s_plane = cvCreateImage( cvSize(img->width, img->height), 8, 1 );
    IplImage* v_plane = cvCreateImage( cvSize(img->width, img->height), 8, 1 );
    IplImage* planes[] = { h_plane, s_plane };
    IplImage* hsv = cvCreateImage( cvSize(img->width, img->height), 8, 3 );

    int h_bins = 30, s_bins = 32;
    int hist_size[] = {h_bins, s_bins};
    /* hue varies from 0 (~0 deg red) to 180 (~360 deg red again) */
    float h_ranges[] = { 0, 180 };
    /* saturation varies from 0 (black-gray-white) to
       255 (pure spectrum color) */
    float s_ranges[] = { 0, 255 };
    float* ranges[] = { h_ranges, s_ranges };

    CvHistogram* hist;

    cvCvtColor( img, hsv, CV_BGR2HSV );
    cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
    hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );

    IplImage* sa_h_plane = cvCreateImage( cvSize(img->width, img->height), 8, 1 );
    IplImage* sa_s_plane = cvCreateImage( cvSize(img->width, img->height), 8, 1 );
    IplImage* sa_v_plane = cvCreateImage( cvSize(img->width, img->height), 8, 1 );
    IplImage* sa_planes[] = { sa_h_plane, sa_s_plane };
    IplImage* sa_hsv = cvCreateImage( cvSize(img->width, img->height), 8, 3 );

    int sa_h_bins = 30, sa_s_bins = 32;
    int sa_hist_size[] = {sa_h_bins, sa_s_bins};
    /* hue varies from 0 (~0 deg red) to 180 (~360 deg red again) */
    float sa_h_ranges[] = { 0, 180 };
    /* saturation varies from 0 (black-gray-white) to
       255 (pure spectrum color) */
    float sa_s_ranges[] = { 0, 255 };
    float* sa_ranges[] = { sa_h_ranges, sa_s_ranges };

    CvHistogram* sa_hist;

    cvCvtColor( img, sa_hsv, CV_BGR2HSV );
    cvCvtPixToPlane( sa_hsv, sa_h_plane, sa_s_plane, sa_v_plane, 0 );
    sa_hist = cvCreateHist( 2, sa_hist_size, CV_HIST_ARRAY, sa_ranges, 1 );

    cvCalcHist( sa_planes, sa_hist, 0, 0 );

    cvCalcBackProject(planes, back_project, sa_hist);

    cvReleaseImage(&h_plane);
    cvReleaseImage(&s_plane);
    cvReleaseImage(&v_plane);

    cvReleaseImage(&sa_h_plane);
    cvReleaseImage(&sa_s_plane);
    cvReleaseImage(&sa_v_plane);

    return (IplImage*)back_project;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Rotates an image in 90 degree increments, adjusting for aspect ratio.
///         returned image may be of different size than source.
///
///   \param[in] img Source image we want to rotate
///   \param[in] angle angle in degrees, any multiple of 90 is valid
///   \param[in] orientation CV_CLOCKWISE or CV_COUNTER_CLOCKWISE indicating
///              direction of rotation.
///
///   \return   IplImage that represents the new rotation. New memory is allocated
///             and must be released when finished with image.
///
///////////////////////////////////////////////////////////////////////////////////
IplImage* VisionUtils::Rotate90(const IplImage* img, const int angle, const int orientation)
{
    if (img == NULL) return NULL;
    //copy of original
    IplImage * imgCopy = cvCloneImage(img);
    //dimensions of original swapped for rectangular images
    IplImage * img90 = cvCreateImage(cvSize(img->height,img->width), img->depth, img->nChannels);
    int i = 0;
    //go up to 270 degrees
    switch (angle%360)
    {
    case 90:
        cvTranspose(imgCopy,img90);
        if (orientation == CV_CLOCKWISE) {
            cvFlip(img90,NULL,1);
        } else {
            cvFlip(img90,NULL,0);
        }
        break;
    case 180:
        cvFlip(imgCopy,NULL,-1);
        break;
    case 270:
        cvTranspose(imgCopy,img90);
        if (orientation == CV_CLOCKWISE) {
            cvFlip(img90,NULL,0);
        } else {
            cvFlip(img90,NULL,1);
        }
        break;
    default:
        break;
    }
    if (angle%180 != 0) {
        cvReleaseImage(&imgCopy);
        //cvShowImage("Output",img90);
        //cvWaitKey(0);
        return img90;
    }
    cvReleaseImage(&img90);
    //cvShowImage("Output",imgCopy);
    //cvWaitKey(0);
    return imgCopy;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns an angle between 3 points. Original source taken from opencv
///          square.c in example programs.
///
///   \param[in] pt1 First point in the angle
///   \param[in] pt2 Second point in angle
///   \param[in] pt0 point at which we are measuring angle from pt1 to pt2
///
///   \return   angle between points, in radians
///
///////////////////////////////////////////////////////////////////////////////////
double VisionUtils::GetAngle(const CvPoint* pt1, const CvPoint* pt2, const CvPoint* pt0 )
{
    double dx1 = pt1->x - pt0->x;
    double dy1 = pt1->y - pt0->y;
    double dx2 = pt2->x - pt0->x;
    double dy2 = pt2->y - pt0->y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Crops a portion of an image given a box of equal or smaller width and
///          height to the source image.  Crop area does not have to be parallel to
///          sides of image.  Allocates new memory for crop and must
///          be release when done with it.
///
///   \param[in] img Source image.
///   \param[in] cropArea Area to crop.
///
///   \return  new image of the crop area.
///
///////////////////////////////////////////////////////////////////////////////////
IplImage*  VisionUtils::Crop(const IplImage* img, const CvBox2D box)
{
    if (!img) return NULL;
    CvBox2D cropArea = box;
    if(img->width < cropArea.size.width) return NULL;
    if(img->height < cropArea.size.height) return NULL;
    if ((cropArea.size.width * cropArea.size.height) <= 0) return NULL;

    IplImage *imgCopy =  cvCloneImage( img );
    IplImage *crop = cvCreateImage(cvSize(cropArea.size.height,cropArea.size.width),img->depth,img->nChannels);
    //do rotation to match the crop area
    CvMat *rotMatrix = cvCreateMat(2, 3, CV_32FC1);
    //always having issues with crop. comment in or out this line or the one below it
    //has something to do with rotated or non rotated blob boxes
//    cv2DRotationMatrix(cropArea.center, -cropArea.angle+CxUtils::CX_HALF_PI, 1.0, rotMatrix);
    cv2DRotationMatrix(cropArea.center, cropArea.angle/*+CxUtils::CX_HALF_PI*/, 1.0, rotMatrix);
    cvWarpAffine(img, imgCopy, rotMatrix);
    //crop area
    cvGetRectSubPix(imgCopy,crop,cropArea.center);
    cvReleaseImage(&imgCopy);
    cvReleaseMat(&rotMatrix);
    return crop;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the sub-images of an image, size and number
///          determined by number of rows and columns desired. Release images when
///          done.
///
///   \param[in] img Original image to sub divide
///   \param[in] number of rows desired.
///   \param[in] number of cols desired.
///   \return A list of image pointers to sub images, must delete this list when done.
///
///////////////////////////////////////////////////////////////////////////////////
std::vector<IplImage*> VisionUtils::GetImageCells(const IplImage* img, const int numRows, const int numCols)
{
    std::vector<IplImage*> result;
    if(img == NULL) return result;
    std::vector<CvBox2D> cells = GetCells(img, numRows, numCols);
    for(int i = 0; i < cells.size(); i++)
    {
        result.push_back(Crop(img,cells[i]));
    }
    return result;

}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the pixel offset of the cell in the image.
///
///   \param[in] index Index of cell in image.
///   \param[in] numRows number of rows.
///   \param[in] numCols number of cols.
///   \param[in] cellImg actual cell image.  For getting width and height.
///   \return On error, an empty point, ambiguous if successful offset is 0,0.
///           On success, an x,y pixel offset to cell. Origins(0,0) are in top
///           left corner of each image.
///
///////////////////////////////////////////////////////////////////////////////////
CxUtils::Point3D VisionUtils::GetCellOffset(int index,
                                            const int numRows, const int numCols,
                                            const IplImage * cellImg)
{
    CxUtils::Point3D result;
    if(cellImg == NULL) return result;
    int cellWidth = cellImg->width;
    int cellHeight = cellImg->height;
    int indexCount = 0;

    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numCols; j++)
        {
            int startx =  j * cellWidth;
            int starty = i * cellHeight;
            if(index == indexCount)
            {
                result.mX = startx;
                result.mY = starty;
                return result;
            }
            indexCount++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Builds an image up from subcells, the reverse of GetImageCells().
///          must delete resulting image when done. Function assumes cells are in
///          the same order as GetImageCells creates them.  If cells are missing,
///          function will append 0 to end of image.
///
///   \param[in] cells image cells to combine
///   \param[in] numRows number of rows desired.
///   \param[in] numCols number of cols desired.
///   \return A new image created from combined sub images.Null if no cells exist
///           or rows/cols out of range.
///
///////////////////////////////////////////////////////////////////////////////////
IplImage* VisionUtils::StitchCells(const std::vector<IplImage*> cells, const int numRows, const int numCols, const bool drawBorders)
{
    if(cells.size() == 0 || numRows <= 0 || numCols <= 0) return NULL;
    int cellWidth = cells[0]->width;
    int cellHeight = cells[0]->height;
    CvSize imageSize = cvSize(cellWidth*numCols & -2, cellHeight*numRows & -2 );
    IplImage* result = cvCreateImage(imageSize,cells[0]->depth, cells[0]->nChannels);
    cvZero(result);

    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numCols; j++)
        {
            int index = i*numCols+j;
            int startx =  j * cellWidth;
            int starty = i * cellHeight;
            if(index < cells.size() && cells[index] != NULL)
            {
                CvRect roi = cvRect(startx, starty , cellWidth, cellHeight);
                cvSetImageROI(result,roi);
                cvAdd(result,cells[index], result, NULL);
                if(drawBorders)
                {
                    cvRectangle(result,cvPoint(0,0),cvPoint(cellWidth,cellHeight),cvScalar(255,255,255));
                }
                cvResetImageROI(result);

            }
        }
    }
    cvResetImageROI(result);
    return result;

}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a list box2d objects representing positions of sub cells in
///          an image. can be used for displaying a grid, etc.
///
///   \param[in] img Original image to sub divide
///   \param[in] number of rows desired.
///   \param[in] number of cols desired.
///   \return A list of box2d objects.
///
///////////////////////////////////////////////////////////////////////////////////
std::vector<CvBox2D> VisionUtils::GetCells(const IplImage* image, const int numRows, const int numCols)
{

    CvBox2D tempBox;
    std::vector<CvBox2D> result;
    if(image == NULL) return result;
    tempBox.angle = 0;
    if(numCols < 1 || numRows < 1 )
    {
        tempBox.size.width = image->width;
        tempBox.size.height = image->height;
        tempBox.center.x = image->width/2,
                tempBox.center.y = image->height/2;
        result.push_back(tempBox);
        return result;
    }

    tempBox.size.width = image->width/numCols;
    tempBox.size.height = image->height/numRows;
    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numCols; j++)
        {
            tempBox.center.x = j * tempBox.size.width + tempBox.size.width/2;
            tempBox.center.y = i * tempBox.size.height + tempBox.size.height/2;
            result.push_back(tempBox);
        }
    }
    return result;
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a measure of similarity of 2 images by comparing contours
///         found in the image. Scale, rotation, translation invariant.
///
///   \param[in] toMatch Source image. Must be a single channel binary 8 bit image.
///   \param[in] tmplt Template to match to, must be single channel binary 8 bit image.
///   \param[in] memory storage to allocated contour data.
///   \param[out] similarity the similarity measure, closer to 1 is more similar.
///
///////////////////////////////////////////////////////////////////////////////////
void VisionUtils::GetContourSimilarity(const IplImage* toMatch, const IplImage* tmplt, double &similarity)
{
    if (toMatch == NULL ||tmplt == NULL) {
        similarity = 0;
        return;
    }

    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq * contour;
    CvSeq * tmpltContour;
    IplImage * imgCopy = cvCloneImage(toMatch);
    IplImage * tmpltCopy = cvCloneImage(tmplt);
    cvFindContours(imgCopy, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
    cvFindContours(tmpltCopy, storage, &tmpltContour, sizeof(CvContour),CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
    if(contour && tmpltContour)
    {
        similarity = 1. - (cvMatchShapes(tmpltContour,contour,CV_CONTOURS_MATCH_I3));
    }
    else
    {
        similarity = 0;
    }
    cvReleaseMemStorage(&storage);
    cvReleaseImage(&imgCopy);
    cvReleaseImage(&tmpltCopy);
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Simple template match that looks at differences between image width/
///          height ratios and 0-255 pixels.  Does not rotate images.  Stretching
///          the image to fit template lowers results.
///
///   \param[in] toMatch Source image. Must be a single channel binary 8 bit image.
///   \param[in] tmplt Template to match to, must be single channel binary 8 bit image.
///   \param[in] useBestRatio Will rotate image to best fit if set to true.
///   \param[out] similarity the similarity measure, closer to 1 is more similar.
///
///////////////////////////////////////////////////////////////////////////////////
void VisionUtils::GetSimpleTemplateSimilarity(const IplImage* toMatch, const IplImage* tmplt, double &similarity, const bool useBestRatio)
{

    if (!IsGrayScale(toMatch))
    {
        //std::cout << "ERROR::GetTemplateSimilarity: toMatch is NULL" << std::endl;
        return;
    }
    if (!IsGrayScale(tmplt))
    {
        //std::cout << "ERROR::GetTemplateSimilarity: tmplt is NULL" << std::endl;
        return;
    }
    double normWidth, normHeight;
    double length = tmplt->width + tmplt->height;
    normWidth = tmplt->width / length;
    normHeight = tmplt->height / length;

    length = toMatch->width + toMatch->height;
    normWidth -= toMatch->width /length;
    normHeight -= toMatch->height / length;

    normWidth = fabs(normWidth);
    normHeight = fabs(normHeight);

    double ratioSimularity = 1.0 - (normWidth+normHeight)/2.0;


    length = tmplt->width + tmplt->height;
    normWidth = tmplt->width / length;
    normHeight = tmplt->height / length;

    length = toMatch->width + toMatch->height;
    normWidth -= toMatch->height /length;
    normHeight -= toMatch->width/ length;

    normWidth = fabs(normWidth);
    normHeight = fabs(normHeight);

    double temp = 1.0 - (normWidth+normHeight)/2.0;


    IplImage * templateN = NULL;// = cvCreateImage(imgSize, IPL_DEPTH_8U, 1 );

    if(useBestRatio)
    {
        if(temp > ratioSimularity)
        {
            templateN = Rotate90(tmplt);
        }
        ratioSimularity = std::max(temp,ratioSimularity);
    }
    else
    {
        //        if(temp < ratioSimularity)
        //        {
        //            templateN = Rotate90(tmplt);
        //        }
        ratioSimularity = std::min(temp,ratioSimularity);
    }
    if(templateN == NULL) templateN = cvCloneImage(tmplt);
    CvSize imgSize = cvSize(templateN->width, templateN->height);
    IplImage * resizedImg = cvCreateImage(imgSize, IPL_DEPTH_8U, 1 );
    IplImage * diffImg = cvCreateImage(imgSize, IPL_DEPTH_8U, 1 );
    IplImage * toMatchN  = cvCreateImage(imgSize, IPL_DEPTH_8U, 1 );

    cvResize(toMatch,resizedImg,CV_INTER_LINEAR);
    cvCopy(resizedImg,toMatchN);
    //cvCopy(tmplt,templateN);
    //cvNormalize(resizedImg, toMatchN, 255, 0, CV_MINMAX);
    //cvNormalize(tmplt, templateN, 255, 0, CV_MINMAX);

    double sum = 0, bestSum = -1;
    double totalWhite = tmplt->width * tmplt->height;

    cvAbsDiff(toMatchN,templateN,diffImg);
    CvScalar white;
    white = cvAvg(diffImg);
    sum = white.val[0];

    if (bestSum < 0 || bestSum > sum)
    {
        bestSum = sum;
//        cvShowImage("tomatch",toMatchN);
//        cvShowImage("template",templateN);
//        cvShowImage("diff",diffImg);
//        cvWaitKey(0);
    }
    cvReleaseImage(&resizedImg);
    cvReleaseImage(&diffImg);
    cvReleaseImage(&toMatchN);
    cvReleaseImage(&templateN);
    similarity =  (1.0 - bestSum/255.) * ratioSimularity;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a measure of similarity of 2 images by comparing pixels
///         found in the image. scale invariant, Only slightly rotation invariant.
///         Not translation invariant.
///
///   \param[in] toMatch Source image. Must be a single channel binary 8 bit image.
///   \param[in] tmplt Template to match to, must be single channel binary 8 bit image.
///   \param[out] similarity the similarity measure, closer to 1 is more similar.
///
///////////////////////////////////////////////////////////////////////////////////
void VisionUtils::GetTemplateSimilarity(const IplImage* toMatch, const IplImage* tmplt, double &similarity)
{
    if (toMatch == NULL)
    {
        //std::cout << "ERROR::GetTemplateSimilarity: toMatch is NULL" << std::endl;
        return;
    }
    if (tmplt == NULL)
    {
        //std::cout << "ERROR::GetTemplateSimilarity: tmplt is NULL" << std::endl;
        return;
    }

    CvSize imgSize = cvSize(tmplt->width, tmplt->height);
    IplImage * resizedImg = cvCreateImage(imgSize, IPL_DEPTH_8U, 1 );
    IplImage * diffImg = cvCreateImage(imgSize, IPL_DEPTH_8U, 1 );
    IplImage * flippedImg = cvCloneImage(toMatch);
    IplImage * rotatedImg = NULL;
    cvFlip(toMatch,flippedImg,0);
    double sum = 0, bestSum = -1;
    int totalWhite = cvCountNonZero(tmplt);
    //try all 90 degree rotations and flips with rotations
    for (int j = 0; j < 8; j++)
    {
        cvReleaseImage(&rotatedImg);
        switch (j)
        {
        case 0:
            rotatedImg = cvCloneImage(toMatch);
            break;
        case 1:
            rotatedImg = Rotate90(toMatch);
            break;
        case 2:
            rotatedImg = Rotate90(toMatch,180);
            break;
        case 3:
            rotatedImg = Rotate90(toMatch,270);
            break;
        case 4:
            rotatedImg = cvCloneImage(flippedImg);
            break;
        case 5:
            rotatedImg = Rotate90(flippedImg,90);
            break;
        case 6:
            rotatedImg = Rotate90(flippedImg,180);
            break;
        case 7:
            rotatedImg = Rotate90(flippedImg,270);
            break;
        default:
            break;
        }
        cvResize(rotatedImg,resizedImg);
        cvXor(resizedImg,tmplt,diffImg);
        sum = cvCountNonZero(diffImg);

        if (bestSum < 0 || bestSum > sum)
        {
            bestSum = sum;
            cvNamedWindow("ToCompare", CV_WINDOW_AUTOSIZE);
            cvShowImage("ToCompare",diffImg);
        }
    }
    cvReleaseImage(&rotatedImg);
    cvReleaseImage(&resizedImg);
    cvReleaseImage(&diffImg);
    cvReleaseImage(&flippedImg);
    similarity =  1. - bestSum/totalWhite;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a cvSequence of points that represents corners of squares
///          found in source image using a combination of thresholding
///          and edge detection. Original source taken from square.c in opencv
///          example programs.
///
///   \param[in] img Source image.
///   \param[in] storage memory to store contour and sequence data.
///   \param[in] cannyThreshold threshold used for edge detection
///
///   \return cvSequence of points that represent corners of squares found in image.
///
///////////////////////////////////////////////////////////////////////////////////
/*CvSeq* VisionUtils::FindSquares(const IplImage* img, CvMemStorage* storage, int cannyThreshold)
{
    CvSeq* contours;
    int i, c, l, N = 11;
    double s, t;
    double imgArea = img->height * img->width;
    CvSize imageSize = cvSize( img->height, img->width  );
    IplImage* imgCopy = cvCloneImage( img ); // make a copy of input image
    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.height/2,imageSize.width/2), IPL_DEPTH_8U, 1 );
    IplImage* resultImage = cvCloneImage( img );
    IplImage* canny = cvCloneImage( img );
    IplImage* testing = cvCloneImage( img );
    CvSeq* result;
    // create empty sequence that will contain points -
    // 4 points per square (the square's vertices)
    CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage );


    // select the maximum ROI in the image
    // with the width and height divisible by 2
    //cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));

    // down-scale and upscale the image to filter out the noise
 //   cvPyrDown( imgCopy, imgSmallCopy);
 //   cvPyrUp( imgSmallCopy, imgCopy);

    // try several threshold levels
    for( l = -1; l < N; l++ )
    {
        // hack: use Canny instead of zero threshold level.
        // Canny helps to catch squares with gradient shading
        if( l == 0 )
        {
            // apply Canny. Take the upper threshold from slider
            // and set the lower to 0 (which forces edges merging)
            cvCanny( imgCopy, resultImage, 0, cannyThreshold, 5 );

            // dilate canny output to remove potential
            // holes between edge segments
            cvDilate( resultImage, resultImage, 0, 1 );
        }
        else if(l == -1)
        {
            cvCanny( imgCopy, resultImage, 0, cannyThreshold, 5 );
        }
        else
        {
            // apply threshold if l!=0:
            //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
            cvThreshold( imgCopy, resultImage, (l+1)*255/N, 255, CV_THRESH_BINARY );
        }
        //cvShowImage("Output", resultImage);
        //cvWaitKey(0);
        // find contours and store them all as a list
        cvFindContours( resultImage, storage, &contours, sizeof(CvContour),
        CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
        // test each contour
        while( contours )
        {
            // approximate contour with accuracy proportional
            // to the contour perimeter
            result = cvApproxPoly( contours, sizeof(CvContour), storage,
                CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
            //printf("poly points: %d\n", result->total);
            // square contours should have 4 vertices after approximation
            // relatively large area (to filter out noisy contours)
            // and be convex.
            // Note: absolute value of an area is used because
            // area may be positive or negative - in accordance with the
            // contour orientation
            double  contourArea =  fabs(cvContourArea(result,CV_WHOLE_SEQ));
            if( result->total == 4 &&               //total points in contour
                contourArea > (imgArea * .05) &&    //contour area covers greater than 5% of total image
                contourArea < (imgArea * .95) &&    //contour area covers less than 95% of total image
                cvCheckContourConvexity(result) )   //contour is convex
            {
                s = 0;

                for( i = 0; i < 5; i++ )
                {
                    // find minimum angle between joint
                    // edges (maximum of cosine)
                    if( i >= 2 )
                    {
                        t = fabs(GetAngle(
                        (CvPoint*)cvGetSeqElem( result, i ),
                        (CvPoint*)cvGetSeqElem( result, i-2 ),
                        (CvPoint*)cvGetSeqElem( result, i-1 )));
                        s = s > t ? s : t;
                    }
                }


                // if cosines of all angles are small
                // (all angles are ~90 degree) then write quandrange
                // vertices to resultant sequence
                if( s < 0.3 )
                {
                    //printf("square found\n");
                    for( i = 0; i < 4; i++ )
                    {
                        cvSeqPush( squares,(CvPoint*)cvGetSeqElem( result, i ));
                    }
                }
            }

            // take the next contour
            contours = contours->h_next;
        }
    }
    //}

    // release all the temporary images
    cvReleaseImage( &imgCopy );
    cvReleaseImage( &imgSmallCopy );
    cvReleaseImage( &resultImage );
    //cvReleaseImage( &timg );
    cvReleaseImage( &canny );
    cvReleaseImage( &testing );
    return squares;
}*/
bool VisionUtils::Calc3DSquareAngle(CvSeq*& polySquareApprox,float& angle, IplImage* dest)
{
    if(polySquareApprox == NULL) return false;
    if(polySquareApprox->total != 4) return false;
    CvPoint * points[4];
    points[0] = (CvPoint*)cvGetSeqElem( polySquareApprox, 0);
    points[1] = (CvPoint*)cvGetSeqElem( polySquareApprox, 1);
    points[2] = (CvPoint*)cvGetSeqElem( polySquareApprox, 2);
    points[3] = (CvPoint*)cvGetSeqElem( polySquareApprox, 3);
    if(points[0] == NULL || points[1] == NULL || points[2] == NULL || points[3] == NULL) return false;

    CvRect boundingbox = cvBoundingRect(polySquareApprox);
    CxUtils::Point3D cxpoints[4];
    CxUtils::Point3D cxboundrypoints[4];

    CxUtils::Point3D topleft,topright,bottomleft,bottomright;
    for(int i = 0; i < 4; i ++)
    {
        cxpoints[i].Set(points[i]->x,points[i]->y,0);
    }
    if(dest) cvRectangle(dest,cvPoint(boundingbox.x,boundingbox.y),cvPoint(boundingbox.width+boundingbox.x,boundingbox.height+boundingbox.y),cvScalar(255,255,255));
    cxboundrypoints[0].Set(boundingbox.x,boundingbox.y,0);
    cxboundrypoints[1].Set(boundingbox.width + boundingbox.x,0,0);
    cxboundrypoints[2].Set(0,boundingbox.height + boundingbox.y,0);
    cxboundrypoints[3].Set(boundingbox.width + boundingbox.x,boundingbox.height + boundingbox.y,0);

    CxUtils::Point3D temp;
    double mindist= -1;
    double tempdist= 0;

    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_PLAIN,1,1);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            tempdist = cxboundrypoints[i].Distance(cxpoints[j]);
            if((mindist == -1 || tempdist <= mindist))
            {
                temp = cxpoints[j];
                mindist = tempdist;
            }
        }
        switch(i)
        {
        case 0:
            if(dest) cvPutText(dest,"topleft",cvPoint(temp.mX,temp.mY-10),&font,cvScalar(255,255,255));
            topleft = temp;
            break;
        case 1:
            if(dest) cvPutText(dest,"topright",cvPoint(temp.mX,temp.mY+10),&font,cvScalar(255,255,255));
            topright = temp;
            break;
        case 2:
            if(dest) cvPutText(dest,"bottomleft",cvPoint(temp.mX,temp.mY-10),&font,cvScalar(255,255,255));
            bottomleft= temp;
            break;
        case 3:
            if(dest) cvPutText(dest,"bottomright",cvPoint(temp.mX,temp.mY+10),&font,cvScalar(255,255,255));
            bottomright = temp;
            break;
        default:
            break;

        }
        mindist = -1;
        tempdist = 0;
        temp.mX = 0;
        temp.mY = 0;
    }


    float leftdist, rightdist, topdist, bottomdist, biggestside;
    if((leftdist = topleft.Distance(bottomleft))< .0001)
    {
        angle =  0;
    }
    else if((rightdist = topright.Distance(bottomright)) < .0001)
    {
        angle = 0;
    }
    else if(fabs(leftdist-rightdist) <  .0001)
    {
        angle =  0;
    }
    else
    {
        topdist = topright.Distance(topleft);
        bottomdist = bottomleft.Distance(bottomright);
        biggestside = std::max(leftdist,rightdist);
        float tbave = (topdist+bottomdist)/2.;
        angle = fabs(biggestside - tbave)/biggestside;
        if(leftdist > rightdist)
        {
            angle *=-1.;
        }

    }
    return true;
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
std::list<CvBox2D> VisionUtils::FindSquares(const IplImage* img, IplImage *dest, const double minsize, const double maxsize, const int cannyThreshold, const bool angle3d)
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
    cvPyrDown( imgCopy, imgSmallCopy);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //cvMorphologyEx(imgSmallCopy,imgSmallCopy,NULL,CV_SHAPE_RECT,CV_MOP_OPEN,1);

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    // These two lines may work, needs to be tested!!
    IplConvKernel* structureElement = cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_RECT);
    cvMorphologyEx(imgSmallCopy, imgSmallCopy, NULL, structureElement, CV_MOP_OPEN, 1 );

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    cvPyrUp( imgSmallCopy, imgCopy);
    //canny = cvCreateImage( imageSize, IPL_DEPTH_8U, 1 );
    cvZero(imgGraySum);
    for(int colorPlane = 1; colorPlane <= imgCopy->nChannels+1; colorPlane++)
    {
        if(colorPlane == imgCopy->nChannels +1)
        {
            cvCopy(imgGraySum, resultImage);
        }
        else if(imgCopy->nChannels > 1)
        {
            //colorplane 0 = all channels
            cvSetImageCOI(imgCopy, colorPlane);
            cvCopy(imgCopy,imgGray);
        }

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
                            t = fabs(GetAngle(
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
                                DrawSquare(dest,polyApprox);
                            }
                            else
                            {

                                DrawSquare(dest,tempBox);
                            }

                        }
                        if(angle3d)
                        {

                            Calc3DSquareAngle(polyApprox,tempBox.angle,dest);
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
// the function draws all the squares in the image
void VisionUtils::DrawSquare( IplImage* img, const CvSeq* square, CvScalar color, int thickness )
{
    if(img == NULL ) return;
    if(square == NULL )return;
    if(square->total !=4) return;
    CvPoint * rect[4];
    rect[0] = (CvPoint*)cvGetSeqElem( square, 0);
    rect[1] = (CvPoint*)cvGetSeqElem( square, 1);
    rect[2] = (CvPoint*)cvGetSeqElem( square, 2);
    rect[3] = (CvPoint*)cvGetSeqElem( square, 3);
    int count = 4;
    // draw the square as a closed polyline
    cvPolyLine( img, rect, &count, 1, 1, color, thickness, CV_AA, 0 );

}

void VisionUtils::BoxPoints( CvBox2D box, CvPoint2D32f pt[4] )
{
    float a = (float)sin(box.angle)*0.5f;
    float b = (float)cos(box.angle)*0.5f;

    pt[0].x = box.center.x - a*box.size.height - b*box.size.width;
    pt[0].y = box.center.y + b*box.size.height - a*box.size.width;
    pt[1].x = box.center.x + a*box.size.height - b*box.size.width;
    pt[1].y = box.center.y - b*box.size.height - a*box.size.width;
    pt[2].x = 2*box.center.x - pt[0].x;
    pt[2].y = 2*box.center.y - pt[0].y;
    pt[3].x = 2*box.center.x - pt[1].x;
    pt[3].y = 2*box.center.y - pt[1].y;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws cvbox2ds on an image.
///
///   \param[in] img Source image. Must be an 8 bit single channel image.
///   \param[in] square squre to draw
///   \param[in] color color of square as cv scalar RGB
///   \param[in] thickness of lines
///
///////////////////////////////////////////////////////////////////////////////////
void VisionUtils::DrawSquare(IplImage* img, const CvBox2D square, const CvScalar color, const int thickness, const bool crosslines)
{
    if(img == NULL) return;
    CvPoint2D32f boxCorners[4];
    cvBoxPoints(square,boxCorners);
    int thick = thickness;
    if(thickness == CV_FILLED)
    {
        thick = 1;
    }
    if(!crosslines)
    {
        cvLine(img,cvPointFrom32f(boxCorners[0]),cvPointFrom32f(boxCorners[1]),color,thick);
        cvLine(img,cvPointFrom32f(boxCorners[1]),cvPointFrom32f(boxCorners[2]),color,thick);
        cvLine(img,cvPointFrom32f(boxCorners[2]),cvPointFrom32f(boxCorners[3]),color,thick);
        cvLine(img,cvPointFrom32f(boxCorners[3]),cvPointFrom32f(boxCorners[0]),color,thick);
    }
    else
    {
        cvLine(img,cvPointFrom32f(boxCorners[0]),cvPointFrom32f(boxCorners[2]),color,thick);
        cvLine(img,cvPointFrom32f(boxCorners[1]),cvPointFrom32f(boxCorners[3]),color,thick);
    }

    if(thickness == CV_FILLED)
    {
        if(square.center.x > 0 && square.center.y > 0 && square.center.x < img->width && square.center.y < img->height)
        {
            cvFloodFill(img,cvPointFrom32f(square.center),color);
        }
    }
    else
    {
        cvCircle(img,cvPointFrom32f(square.center),1,color,thickness);
    }

}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a stl list of cvBox2D objects around blobs found in an image
///          in order of biggest to smallest.
///
///   \param[in] img Source image. Must be a binary image.
///   \param[in] minsize minimum size acceptable to put a box around, as a percentage
///              of the total area of the source image.
///   \param[in] maxsize maximum size acceptable to put a box around, as a percentage
///              of the total area of the source image.
///
///   \return  stl list of cvbox2d objects around blobs.
///
///////////////////////////////////////////////////////////////////////////////////
std::list<CvBox2D> VisionUtils::GetBlobBoxes(const IplImage * img, const double minsize, const double maxsize, const bool allowRotation)
{
    std::list<CvBox2D> blobList;
    if(img == NULL) return blobList;
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours;
    //IplImage* imgCopy = cvCloneImage( img ); // make a copy of input image
    IplImage* resultImage = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
    IplImage* contourImage = cvCloneImage( resultImage );

    CvBox2D blobBox;
    double imgArea = img->height * img->width;
    double contourArea = 0;
    if(img->nChannels != resultImage->nChannels ||
            img->depth != resultImage->depth)
    {
        cvConvertImage(img,resultImage);
    }
    else
    {
        cvCopyImage(img,resultImage);
    }
    cvThreshold(resultImage,resultImage,50,255,CV_THRESH_BINARY);

    cvFindContours( resultImage, storage, &contours,sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

    for (CvSeq * contour = contours; contour!= NULL; contour=contour->h_next) {

        if(allowRotation)
        {
            blobBox = cvMinAreaRect2(contour,storage);
            //blobBox.angle -= 90;
        }
        else
        {
            CvRect rect = cvBoundingRect(contour);
            blobBox.size.width = rect.width;
            blobBox.size.height = rect.height;
            blobBox.angle = 90;
            blobBox.center.x = rect.x+rect.width/2.0;
            blobBox.center.y = rect.y+rect.height/2.0;
        }
        blobBox.size.width = blobBox.size.width + 1;
        blobBox.size.height = blobBox.size.height + 1;
        contourArea = blobBox.size.width * blobBox.size.height;
        if (contourArea > (imgArea * minsize) &&   //contour area covers greater than min of total image
                contourArea < (imgArea * maxsize))//  &&    //contour area covers less than max of total image
            //img->width >= blobBox.size.width &&
            //img->height >= blobBox.size.height)
        {
            double tempBoxArea = blobBox.size.width * blobBox.size.height;
            std::list<CvBox2D>::iterator it;
            for(it = blobList.begin(); it != blobList.end(); it++)
            {
                if(tempBoxArea > (it->size.width * it->size.height))
                {
                    break;
                }
            }
                        if (blobBox.size.width > blobBox.size.height)
                        {
                            blobBox.angle = CxUtils::Orientation::AddToAngle(blobBox.angle,90,false);
                        }
                        else
                        {
                            std::swap(blobBox.size.height,blobBox.size.width);
                        }
            blobList.insert(it,blobBox);
        }
    }
    /*    for (int i = 0; i < blobList.size(); i++) {
        CvPoint2D32f blobBoxCorners[4];
        cvBoxPoints(blobList[i],blobBoxCorners);
        cvLine(resultImage,cvPointFrom32f(blobBoxCorners[0]),cvPointFrom32f(blobBoxCorners[1]),CV_RGB(255,255,255),5);
        cvLine(resultImage,cvPointFrom32f(blobBoxCorners[1]),cvPointFrom32f(blobBoxCorners[2]),CV_RGB(255,255,255),5);
        cvLine(resultImage,cvPointFrom32f(blobBoxCorners[2]),cvPointFrom32f(blobBoxCorners[3]),CV_RGB(255,255,255),5);
        cvLine(resultImage,cvPointFrom32f(blobBoxCorners[3]),cvPointFrom32f(blobBoxCorners[0]),CV_RGB(255,255,255),5);
    }
    cvShowImage("Boxes", resultImage);*/
    //cvWaitKey(0);
    //cvShowImage("Output", croppedImage);
    //cvReleaseImage(&imgCopy);
    cvReleaseImage(&resultImage);
    cvReleaseImage(&contourImage);
    cvReleaseMemStorage(&storage);
    return blobList;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns an image which cropped out of the original on the biggest blob
///
///   \param[in] img Source image. Must be an binary single channel image.
///   \param[out] dest Destination image. Must be same size as img. A box around
///               biggets blob will be drawn to this img.
///               NULL if no destination image is required.
///   \param[out] biggestBox box that describes blob position and orientation.
///
///   \return  Area of blob in pixels
///
///////////////////////////////////////////////////////////////////////////////////
double VisionUtils::ExtractBiggestBlob(const IplImage* img, IplImage* dest, CvBox2D& biggestBox,const double minsize, const double maxsize)
{

    if(img==NULL) return 0;
    CvBox2D blobBox;
    //IplImage * crop = NULL;
    double blobArea = 0;
    std::list<CvBox2D> boxes = GetBlobBoxes(img, minsize, maxsize);
    if (boxes.size() == 0) return 0;
    biggestBox = boxes.front();
    if(dest!=NULL) DrawSquare(dest,biggestBox);


    //IplImage * imgCopy = cvCloneImage(img);
    /*    crop = Crop(img,biggestBox);
    if(crop!=NULL)
    {
        blobArea = cvCountNonZero(crop);
        cvReleaseImage(&crop);
    }*/
    return biggestBox.size.width * biggestBox.size.height;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates the average color of an image under a line. Line determined
///          by start point and end point. Code taken from opencv documentation.
///
///   \param[in] img Source image.
///   \param[in] pt1 start point of line.
///   \param[in] pt2 end point of line.
///
///   \return  Average color in each channel under line, stuffed into a cvscalar.
///
///////////////////////////////////////////////////////////////////////////////////
CvScalar VisionUtils::AveColorUnderLine(const IplImage* image, const CvPoint point1, const CvPoint point2 )
{
    CvLineIterator iterator;
    int blue_sum = 0, green_sum = 0, red_sum = 0;
    CvPoint pt1 = point1;
    CvPoint pt2 = point2;
    cvClipLine(cvSize(image->width, image->height), &pt1,&pt2);
    int count = cvInitLineIterator( image, pt1, pt2, &iterator, 8, 0 );

    for( int i = 0; i < count; i++ ){
        blue_sum += iterator.ptr[0];
        green_sum += iterator.ptr[1];
        red_sum += iterator.ptr[2];
        CV_NEXT_LINE_POINT(iterator);

        /* print the pixel coordinates: demonstrates how to calculate the
                                                      coordinates */
        {
            int offset, x, y;
            /* assume that ROI is not set, otherwise need to take it
                                              into account. */
            offset = iterator.ptr - (uchar*)(image->imageData);
            y = offset/image->widthStep;
            x = (offset - y*image->widthStep)/(3*sizeof(uchar)
                                               /* size of pixel */);
            //printf("(%d,%d)\n", x, y );
        }
    }
    if(count == 0 ) return cvScalar(0,0,0);
    return cvScalar( blue_sum/count, green_sum/count, red_sum/count );
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks if image is an 8-bit single channel image(grayscale).
///   \returns True if it is, false if not.
///
///////////////////////////////////////////////////////////////////////////////////
bool VisionUtils::IsGrayScale(const IplImage* image)
{
    if(image == NULL) return false;
    return (image->nChannels == 1 && image->depth == IPL_DEPTH_8U);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Combines 2 grayscale masks(not necessarily binary), and optionally
///          weights them, puts result in dest(must be initialized as grayscale).
///   \returns A grayscale image of combined masks and weights(0 - 255).
///            True on success, False on failure.
///
///////////////////////////////////////////////////////////////////////////////////
bool VisionUtils::CombineMasks(const IplImage * mask1, const IplImage * mask2, IplImage * dest, const double weight1, const double weight2)
{
    if(!VisionUtils::IsGrayScale(mask1) || !VisionUtils::IsGrayScale(mask2) || !IsGrayScale(dest)) return false;
    if(mask1->width != mask2->width || mask1->height != mask2->height || dest->height != mask1->height || dest->width != mask1->width) return false;
    if(weight1 < 0 || weight2 < 0 ) return false;

    //IplImage * result = cvCreateImage(cvGetSize(mask1),8,1);
    //cvZero(result);
    //double max1 = (weight1/(weight1+weight2)) * 255.0;
    //double max2 = (weight2/(weight1+weight2)) * 255.0;
    IplImage * norm1 = cvCreateImage(cvGetSize(mask1),8,1);
    IplImage * norm2 = cvCreateImage(cvGetSize(mask1),8,1);
//    cvNormalize(mask1, norm1, max1, 0, CV_MINMAX);
//    cvNormalize(mask2, norm2, max2, 0, CV_MINMAX);
//    cvZero(dest);
//    cvAdd(norm1,norm2,dest);


    double max1 = (weight1/(weight1+weight2));
    double max2 = (weight2/(weight1+weight2));
    cvAddWeighted(mask1,max1,mask2,max2,0,dest);


    //    cvShowImage("norm2", norm2);
    //    cvShowImage("norm1", norm1);
    //    cvShowImage("mask1", mask1);
    //    cvShowImage("mask2", mask2);
    //    cvWaitKey(0);

    cvReleaseImage(&norm1);
    cvReleaseImage(&norm2);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a histogram object in HSV colorspace using H and S
///   \param[in] imgIn sample image to use for histogram.
///   \returns Histogram object
///
///////////////////////////////////////////////////////////////////////////////////
CvHistogram *  VisionUtils::CreateHSVHist(const IplImage * imgIn)
{

    IplImage* hsv = cvCreateImage( cvGetSize(imgIn), 8, 3 );
    cvCvtColor( imgIn, hsv, CV_BGR2HSV );
    IplImage* h_plane = cvCreateImage( cvGetSize(imgIn), 8, 1 );
    IplImage* s_plane = cvCreateImage( cvGetSize(imgIn), 8, 1 );
    IplImage* v_plane = cvCreateImage( cvGetSize(imgIn), 8, 1 );
    IplImage* planes[] = { h_plane, s_plane };
    cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
    // Build the histogram and compute its contents.
    //
    int h_bins = 30, s_bins = 32;
    CvHistogram* hist;
    int hist_size[] = { h_bins, s_bins };
    float h_ranges[] = { 0, 180 };
    // hue is [0,180]
    float s_ranges[] = { 0, 255 };
    float* ranges[] = { h_ranges, s_ranges };
    hist = cvCreateHist(2,hist_size,CV_HIST_ARRAY,ranges,1);
    cvCalcHist( planes, hist, 0, 0 ); //Compute histogram
    //cvNormalizeHist( hist, 1.0 ); //Normalize it'
    cvReleaseImage(&h_plane);
    cvReleaseImage(&s_plane);
    cvReleaseImage(&v_plane);
    cvReleaseImage(&hsv);
    return hist;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Should not be used. use cvnot instead.
///
///////////////////////////////////////////////////////////////////////////////////
bool VisionUtils::Invert(const IplImage * imgIn, IplImage * imgOut)
{
    if(!imgIn || !imgOut || imgIn->width != imgOut->width || imgIn->height != imgOut->height ||
       imgOut->nChannels != imgIn->nChannels || imgIn->depth != imgOut->depth) return false;
//    CvScalar white = CV_RGB(255,255,255);
//    cvAbsDiffS(imgIn,imgOut,white);
    cvNot(imgIn,imgOut);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts cvbox2d angle to something more useable in degrees.
///          Will show angle of longest side, 0 vertical, positive clockwise,
///          [-90,90].
///
///////////////////////////////////////////////////////////////////////////////////
double VisionUtils::GetAngle(const CvBox2D& box)
{
    double result = 0;
//    if(box.size.width > box.size.height)
//    {
//        //result = CxUtils::Orientation::AddToAngle(box.angle,-90,false);
//        result = 0;
//    }
//    else
//    {
//        result = box.angle;
//        //result.mAngle = CxUtils::Orientation::AddToAngle(it->angle,90,false);
//    }
//    if(box.angle != 0 && box.angle)
//    {
//        result = fmod(90,box.angle);
//        if(box.angle>0)
//        {
//            result = -result;
//        }
//    }
//    else
//    {
//        result = 90;
//    }

//    return result;
    //result = CxUtils::Orientation::AddToAngle(box.angle,90,false);
    //result = fmod(result,90);
    if(box.angle > 0) result = CxUtils::Orientation::AddToAngle(box.angle,-90,false);
    if(box.angle < 0) result = CxUtils::Orientation::AddToAngle(box.angle,90,false);
    if(box.angle == 0) result = 90;
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Fills in visionreturn object from a cvbox2d. Converts angle to
///          -90,90 where 0 is Y axis(vertical). Does not modify mValid or
///          mConfidence.
///
///////////////////////////////////////////////////////////////////////////////////
Vision::Return VisionUtils::VisionReturn(const CvBox2D& box)
{
    Vision::Return result;

    result.mArea = box.size.width * box.size.height;
    result.mCenterI = box.center.x;
    result.mCenterJ = box.center.y;
    CvPoint2D32f boxCorners[4];
    cvBoxPoints(box,boxCorners);
    result.mMinI = std::min(std::min(boxCorners[0].x,boxCorners[1].x), std::min(boxCorners[2].x,boxCorners[3].x));
    result.mMinJ = std::min(std::min(boxCorners[0].y,boxCorners[1].y), std::min(boxCorners[2].y,boxCorners[3].y));
    result.mMaxI = std::max(std::max(boxCorners[0].x,boxCorners[1].x), std::max(boxCorners[2].x,boxCorners[3].x));
    result.mMaxJ = std::max(std::max(boxCorners[0].y,boxCorners[1].y), std::max(boxCorners[2].y,boxCorners[3].y));
    result.mAngle = GetAngle(box);
    return result;
}
bool VisionUtils::ClearEdges(IplImage* imgOut, int borderWidthPixels, CvScalar color)
{
    if(!imgOut) return false;
    cvDrawRect(imgOut,cvPoint(0,0),cvPoint(imgOut->width,imgOut->height),color,borderWidthPixels);
    return true;
}
bool VisionUtils::ContourLengthFilter(const IplImage* imgIn, IplImage* maskOut, int minLengthPixels, int maxLengthPixels, bool invert)
{
    if(imgIn == NULL || maskOut == NULL) return false;

    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours;
    //IplImage* imgCopy = cvCloneImage( img ); // make a copy of input image
    IplImage* resultImage = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U,1);
    //IplImage* contourImage = cvCloneImage( resultImage );
    CvScalar color;

    if(invert)
    {
        cvSet(resultImage,CV_RGB(255,255,255));
        color = CV_RGB(0,0,0);
    }
    else
    {
        cvZero(resultImage);
        color = CV_RGB(255,255,255);
    }

    cvThreshold(imgIn,resultImage,50,255,CV_THRESH_BINARY);
    cvFindContours( resultImage, storage, &contours,sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

    if(invert)
    {
        cvSet(resultImage,CV_RGB(255,255,255));
    }
    else
    {
        cvZero(resultImage);
    }
    for (CvSeq * contour = contours; contour!= NULL; contour=contour->h_next)
    {
        double length = cvContourPerimeter( contour );
        if(length < maxLengthPixels && length > minLengthPixels)
        {
            cvDrawContours(resultImage,contour,color,color,0);
            cvShowImage("contour",resultImage);
            int duh = cvCheckContourConvexity(contour);
            printf("convexity %d\n",duh);
            cvWaitKey(1);
        }
    }
    cvCopy(resultImage,maskOut);
    cvReleaseImage(&resultImage);
    cvReleaseMemStorage(&storage);
    return true;
}

void VisionUtils::MakeSquare(CvRect& rect)
{
    rect.height = rect.width;
}

CvRect VisionUtils::ToCvRect(const CvBox2D& rect)
{
    CvRect result;
    CvPoint2D32f boxCorners[4];
    cvBoxPoints(rect,boxCorners);
    int startx, starty, width, height;
    startx = std::min(std::min(boxCorners[0].x,boxCorners[1].x), std::min(boxCorners[2].x,boxCorners[3].x));
    starty = std::min(std::min(boxCorners[0].y,boxCorners[1].y), std::min(boxCorners[2].y,boxCorners[3].y));
    width = std::max(std::max(boxCorners[0].x,boxCorners[1].x), std::max(boxCorners[2].x,boxCorners[3].x));
    height = std::max(std::max(boxCorners[0].y,boxCorners[1].y), std::max(boxCorners[2].y,boxCorners[3].y));

    width -= startx;
    height -= starty;
    result.height = height;
    result.width = width;
    result.x = startx;
    result.y = starty;
    return result;

}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Rediculous function to make possible string name to colorspace conversion
///          types. Useful for configuring colorspace conversions from xml.
///   \param[in] imgIn multi channel image to convert. Should match colorspace
///              conversion argument.
///   \param[out] imgOut single channel from converted space. Must not be null.
///               Selected by channel parameter.
///   \param[in] cvColorSpaceNameAsString exact string name of opencv constant to
///              do conversion.
///   \param[in] channel Channel to select from converted colorspace
///   \returns True if successful, false if not.
///
///////////////////////////////////////////////////////////////////////////////////
bool VisionUtils::ConvertAndGetSingleColorChannel(const IplImage * imgIn, IplImage * imgOut, const std::string& cvColorSpaceNameAsString, const uchar channel)
{
    if(imgIn == NULL || imgOut == NULL || channel > 3 || channel < 0) return false;

    IplImage* result = cvCreateImage( cvGetSize(imgIn), 8, 3 );
    bool success = false;

    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR2BGRA") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR2BGRA );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGB2RGBA") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGB2RGBA );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGRA2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGRA2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGBA2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGBA2RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR2RGBA") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR2RGBA );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGB2BGRA") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGB2BGRA );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGBA2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGBA2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGRA2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGRA2RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR2RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGB2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGB2RGBA );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGB2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGRA2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGRA2RGBA") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGRA2RGBA );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGBA2BGRA") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGBA2BGRA );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR2GRAY") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR2GRAY );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGB2GRAY") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGB2GRAY );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_GRAY2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_GRAY2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_GRAY2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_GRAY2RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_GRAY2BGRA") == 0)
    {
        cvCvtColor( imgIn, result, CV_GRAY2BGRA );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_GRAY2RGBA") == 0)
    {
        cvCvtColor( imgIn, result, CV_GRAY2RGBA );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGRA2GRAY") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGRA2GRAY );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGBA2GRAY") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGBA2GRAY );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR2BGR565") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR2BGR565 );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGB2BGR565") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGB2BGR565 );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR5652BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR5652BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR5652RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR5652RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGRA2BGR565") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGRA2BGR565 );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGBA2BGR565") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGBA2BGR565 );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR5652BGRA") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR5652BGRA );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR5652RGBA") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR5652RGBA );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_GRAY2BGR565") == 0)
    {
        cvCvtColor( imgIn, result, CV_GRAY2BGR565 );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR5652GRAY") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR5652GRAY );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR2BGR555") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR2BGR555 );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGB2BGR555") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGB2BGR555 );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR5552BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR5552BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR5552RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR5552RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGRA2BGR555") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGRA2BGR555 );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGBA2BGR555") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGBA2BGR555 );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR5552BGRA") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR5552BGRA );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR5552RGBA") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR5552RGBA );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_GRAY2BGR555") == 0)
    {
        cvCvtColor( imgIn, result, CV_GRAY2BGR555 );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR5552GRAY") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR5552GRAY );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR2XYZ") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR2XYZ );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGB2XYZ") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGB2XYZ );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_XYZ2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_XYZ2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_XYZ2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_XYZ2RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR2YCrCb") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR2YCrCb );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGB2YCrCb") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGB2YCrCb );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_YCrCb2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_YCrCb2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_YCrCb2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_YCrCb2RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR2HSV") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR2HSV );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGB2HSV") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGB2HSV );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR2Lab") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR2Lab );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGB2Lab") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGB2Lab );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BayerBG2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_BayerBG2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BayerGB2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_BayerGB2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BayerRG2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_BayerRG2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BayerGR2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_BayerGR2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BayerBG2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_BayerBG2RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BayerGB2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_BayerGB2RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BayerRG2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_BayerRG2RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BayerGR2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_BayerGR2RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR2Luv") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR2Luv );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGB2Luv") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGB2Luv );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_BGR2HLS") == 0)
    {
        cvCvtColor( imgIn, result, CV_BGR2HLS );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_RGB2HLS") == 0)
    {
        cvCvtColor( imgIn, result, CV_RGB2HLS );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_HSV2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_HSV2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_HSV2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_HSV2RGB );
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_Lab2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_Lab2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_Lab2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_Lab2RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_Luv2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_Luv2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_Luv2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_Luv2RGB );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_HLS2BGR") == 0)
    {
        cvCvtColor( imgIn, result, CV_HLS2BGR );
        success = true;
    }
    if(strcmp(cvColorSpaceNameAsString.c_str(),"CV_HLS2RGB") == 0)
    {
        cvCvtColor( imgIn, result, CV_HLS2RGB );
        success = true;
    }
    if(channel == 0)
    {
        cvCvtPixToPlane( result,imgOut ,0,0,0);
        success = true;
    }
    if(channel == 1)
    {
        cvCvtPixToPlane( result, 0,imgOut,0,0);
        success = true;
    }
    if(channel == 2)
    {
        cvCvtPixToPlane( result, 0,0,imgOut,0);
        success = true;
    }
    if(channel == 3)
    {
        cvCvtPixToPlane( result, 0,0,0,imgOut);
        success = true;
    }
    cvReleaseImage(&result);
    return success;
}

