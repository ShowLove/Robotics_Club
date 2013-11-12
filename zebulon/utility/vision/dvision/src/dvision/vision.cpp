#include <dvision/vision.h>

using namespace Zebulon;
using namespace Vision;

const std::string Zebulon::Vision::DVision::NotFound = "NULL";
std::map<std::string, int> DVision::mIDMap;

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
std::list<CvBox2D> Zebulon::Vision::DVision::FindSquares(const IplImage* img, IplImage *dest, const double minsize, const double maxsize, const int cannyThreshold, const bool angle3d)
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

    int thresholdsToTry = 1;
    // create empty sequence that will contain points -
    // 4 points per square (the square's vertices)
    //CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage );


    // select the maximum ROI in the image
    // with the width and height divisible by 2
    //cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));

    // down-scale and upscale the image to filter out the noise
    cvPyrDown( imgCopy, imgSmallCopy);

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // This line isn't liked by the new opencv
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
                        cvCanny(imgGray, resultImage, 0,cannyThreshold, 3 );
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
            
            if (dest != NULL)
            {
                Zebulon::Vision::VisionUtil::CombineImages(resultImage, dest);
                //cvCopy(resultImage, dest);
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
void Zebulon::Vision::DVision::DrawSquare(IplImage* img, CvBox2D square, CvScalar color, int thickness)
{
    if(img == NULL) return;
    CvPoint2D32f boxCorners[4];
    cvBoxPoints(square,boxCorners);
    cvLine(img,cvPointFrom32f(boxCorners[0]),cvPointFrom32f(boxCorners[1]),color,5);
    cvLine(img,cvPointFrom32f(boxCorners[1]),cvPointFrom32f(boxCorners[2]),color,5);
    cvLine(img,cvPointFrom32f(boxCorners[2]),cvPointFrom32f(boxCorners[3]),color,5);
    cvLine(img,cvPointFrom32f(boxCorners[3]),cvPointFrom32f(boxCorners[0]),color,5);
    cvCircle(img,cvPointFrom32f(square.center),1,color,thickness);

}

// the function draws all the squares in the image
void Zebulon::Vision::DVision::DrawSquare(IplImage* img, const CvSeq* square, CvScalar color, int thickness )
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

bool Zebulon::Vision::DVision::Calc3DSquareAngle(CvSeq*& polySquareApprox,float& angle, IplImage* dest)
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
double Zebulon::Vision::DVision::GetAngle(const CvPoint* pt1, const CvPoint* pt2, const CvPoint* pt0 )
{
    double dx1 = pt1->x - pt0->x;
    double dy1 = pt1->y - pt0->y;
    double dx2 = pt2->x - pt0->x;
    double dy2 = pt2->y - pt0->y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

std::list<VisionReturn> Zebulon::Vision::DVision::BombingObject(std::list<VisualTarget*> target,IplImage *input,IplImage *&output)
{
    //std::cout<<"ZOMG"<<std::endl;
    
    std::list<VisionReturn> results;
    VisionReturn found;
    InitializeVisionReturn(found);
    if(input == NULL )return results;
    std::list<CvBox2D> possibleTargets;
    double confidence = 0;
    double bestConfidence = 0;
    VisualTarget * bestTemplate = NULL;
    if(mFiltered == NULL) mFiltered = cvCreateImage(cvGetSize(output),8,1);
    IplImage* TempTemp = cvCreateImage(cvGetSize(output),8,1);
    //cvConvertImage(input,output);
    VisionUtils::ExtractColor(input,TempTemp,cvScalar(target.front()->Red(),target.front()->Green(),target.front()->Blue()),
                                                       target.front()->mThresholdAll,target.front()->mDilate);
    
    
    cvNot(TempTemp, mFiltered);
    
    possibleTargets = VisionUtils::GetBlobBoxes(mFiltered,target.front()->mMinSize);
    
    if(output!=NULL) cvConvertImage(mFiltered,output);
    for(std::list<CvBox2D>::iterator blob = possibleTargets.begin(); blob != possibleTargets.end(); blob++)
    {
        IplImage * crop = VisionUtils::Crop(mFiltered,*blob);
        
        if(output!=NULL)
        {
            VisionUtils::DrawSquare(output,*blob,cvScalar(255,0,0));
        }
        
        //cvNamedWindow("after", CV_WINDOW_AUTOSIZE);
        //cvShowImage("after", crop);
        //cvWaitKey(0);
        for(std::list<VisualTarget*>::iterator vt = target.begin(); vt !=target.end(); vt++)
        {
            if ((*vt)->mTemplate == NULL)
            {
                std::cout << "ERROR::GetTemplateSimilarity: tmplt " << (*vt)->mID << " is NULL" << std::endl;
            }
            //cvNamedWindow("template", CV_WINDOW_AUTOSIZE);
            //cvShowImage("template", (*vt)->mTemplate);
            
            Zebulon::Vision::VisionUtils::GetTemplateSimilarity(crop,(*vt)->mTemplate,confidence);
            //std::cout << confidence << std::endl;
            //cvShowImage("afterthat",(*vt)->mTemplate);
            //cvWaitKey(0);
            //Zebulon::VisionUtils::GetContourSimilarity(crop,(*vt)->mTemplate,confidence);
            if(confidence > (*vt)->mMinConfidence && confidence > bestConfidence)
            {
                bestConfidence = confidence;
                bestTemplate = *vt;
            }
        }
        if(bestTemplate)
        {
            if(output!=NULL)
            {
                VisionUtils::DrawSquare(output,*blob,cvScalar(0,0,255));
                CvFont font;
                cvInitFont(&font,CV_FONT_HERSHEY_PLAIN,1,1);
                cvPutText(output,bestTemplate->mID.c_str(),cvPoint(0,output->height-10),&font,cvScalar(255,255,255));
            }
            InitializeVisionReturn(found,&(*blob),bestConfidence, Zebulon::Vision::DVision::ConvertID(bestTemplate->mID));
            std::list<VisionReturn>::iterator it;
            for(it = results.begin(); it != results.end(); it++)
            {
                if(confidence > it->Confidence)
                {
                    break;
                }
            }
            results.insert(it,found);
        }
        bestConfidence = confidence = 0;
        bestTemplate = NULL;
        cvReleaseImage(&crop);
    }
    
    cvReleaseImage(&TempTemp);
    return results;
}

void Zebulon::Vision::DVision::InitializeVisionReturn(VisionReturn& result, const CvBox2D* box, const double confidence, const int value) const
{
    if(box == NULL)
    {
        result.CenterI = 0;
        result.CenterJ = 0;
        result.Angle = 0;
        result.Area = 0;
        result.Confidence = 0;
        result.Valid = 0;
        return;
    }
    result.CenterI = box->center.x;
    result.CenterJ = box->center.y;
    result.Angle = box->angle;
    result.Area = box->size.width * box->size.height;
    result.Confidence = confidence;
    result.Valid = value;
}
