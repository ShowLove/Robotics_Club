//
// The full "Square Detector" program.
// It loads several images subsequentally and tries to find squares in
// each image
//
#ifdef _CH_
#pragma package <opencv>
#endif

#define CV_NO_BACKWARD_COMPATIBILITY

#define PATH_RATIO 0.15
#define ANGLE_RADIANS 1.57079633

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <cvision/colorspace.h>
#include <vector>

using namespace std;
using namespace Zebulon;
using namespace Vision;

int thresh = 100;
IplImage* img = 0;
IplImage* img0 = 0;
CvMemStorage* storage = 0;
const char* wndname = "Square Detection Demo";

IplImage* contourImage = 0;
Vision::ColorSpace colorSpace;

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 )
{
    double dx1 = pt1->x - pt0->x;
    //cout << "dx1: " << dx1 << endl;
    double dy1 = pt1->y - pt0->y;
    double dx2 = pt2->x - pt0->x;
    double dy2 = pt2->y - pt0->y;
    double tempAngle = (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
    /*cout << "tempAngle: " << (tempAngle*180)/M_PI << endl;
    if((tempAngle*180)/M_PI > 70)
    {
        cout << "??????????????????" << endl;
    }*/
    return tempAngle;
}

double distance( CvPoint* pt0, CvPoint* pt1)
{
    double tempDist = sqrt((pt0->x - pt1->x)*(pt0->x - pt1->x) + (pt0->y - pt1->y)*(pt0->y - pt1->y));
    //double tempDist = sqrt((pt0->x - pt1->x)(pt0->x - pt1->x) + (pt0->y - pt1->y)(pt0->y - pt1->y));
    //cout << "tempDist: " << tempDist << endl;
    return tempDist;
}

/*double distanceY( CvPoint* pt0, CvPoint* pt1)
{
    double tempDist = fabs(pt0 - pt1);
    cout << "tempDist: " << tempDist << endl;
    return tempDist;
}*/

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
CvSeq* findSquares4( IplImage* img, CvMemStorage* storage, const double minsize, const double maxsize, const double minCanny, const double maxCanny)
{
    CvSeq* contours;
    //int i, c, l, N = 11;
    int i, c, l, N = 25;
    CvSize sz = cvSize( img->width & -2, img->height & -2 );
    IplImage* timg = cvCloneImage( img ); // make a copy of input image
    IplImage* gray = cvCreateImage( sz, 8, 1 );
    IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );
    IplImage* tgray;
    CvSeq* result;
    double s, t;
    
    // My stuff
    IplImage* outputAfterNoiseRemoval = cvCloneImage( img );
    IplImage* outputAfterGaussNoiseRemoval = cvCloneImage( img );
    CvSeq* drawContours;
    double imgArea = img->height * img->width;
    
    
    // create empty sequence that will contain points -
    // 4 points per square (the square's vertices)
    CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage );

    // select the maximum ROI in the image
    // with the width and height divisible by 2
    //cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));
    
    
    

    // down-scale and upscale the image to filter out the noise
    //cvSmooth(timg, timg, CV_GAUSSIAN, 3, 3);
    cvPyrDown( timg, pyr, 7 );
    // works will good lighting
    //cvMorphologyEx(pyr,pyr,NULL,CV_SHAPE_RECT,CV_MOP_CLOSE,1);
    cvPyrUp( pyr, timg, 7 );
    
    tgray = cvCreateImage( sz, 8, 1 );
    
    //// my stuff
    ////cvSmooth(timg, outputAfterGaussNoiseRemoval, CV_GAUSSIAN, 7, 7);
    //cvPyrDown( outputAfterGaussNoiseRemoval, pyr, CV_GAUSSIAN_5x5 );
    //cvPyrUp( pyr, outputAfterGaussNoiseRemoval, CV_GAUSSIAN_5x5 );
    ////cvShowImage("AfterGaussNoise", outputAfterGaussNoiseRemoval);
    
    //cvMorphologyEx(pyr,pyr,NULL,CV_SHAPE_RECT,CV_MOP_OPEN,1);
    cvPyrUp( pyr, outputAfterNoiseRemoval, 7 );
    ////cvShowImage("AfterNoise", outputAfterNoiseRemoval);
    
    //colorSpace.HistogramEqualization();
    /*colorSpace.GetImage(tgray, colorSpace.CHANNEL_FULL);
    cvSmooth(tgray, tgray, CV_GAUSSIAN, 9, 9);*/
    
    
    // find squares in every color plane of the image
    for( c = 0; c < 3; c++ )
    {
        //c = 0;
        if(c == 0)
        {
            colorSpace.HSV(timg);
            colorSpace.GetImage(tgray, colorSpace.CHANNEL_ONE);
        }
        else if(c == 1)
        {
            colorSpace.CIEXYZ(timg);
            colorSpace.GetImage(tgray, colorSpace.CHANNEL_ONE);
        }
        else if(c == 2)
        {
            colorSpace.Ratio(timg);
            colorSpace.GetImage(tgray, colorSpace.CHANNEL_THREE);
        }
        /*if(c == 0)
        {
            
            colorSpace.GetImage(tgray, 2);
        }
        else if(c == 1)
        {
            colorSpace.GetImage(tgray, 1);
        }
        else
        {
            colorSpace.GetImage(tgray, 0);
        }*/
        //colorSpace.Gray(timg);
        ////colorSpace.GetImage(tgray, colorSpace.CHANNEL_ONE);
        cvShowImage("tgray Mine", tgray);
        
        // extract the c-th color plane
        ////cvSetImageCOI( timg, c+1 );
        ////cvCopy( timg, tgray, 0 );
        ////cvShowImage("tgray", tgray);

        // try several threshold levels
        for( l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            
            // apply Canny. Take the upper threshold from slider
            // and set the lower to 0 (which forces edges merging)
            //cvCanny( tgray, gray, 100, thresh, 5 );
            int edge = 25; //75 works well
            int minCannyTest = l*edge;
            int maxCannyTest = l*edge + 25;
            cvCanny( tgray, gray, minCannyTest, maxCannyTest, 5 );
            cvShowImage("CANNY", gray);
            // dilate canny output to remove potential
            // holes between edge segments
            cvDilate( gray, gray, 0, 1 );
            cvShowImage("DIALATE", gray);
            
            /*if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                //cvCanny( tgray, gray, 100, thresh, 5 );
                cvCanny( tgray, gray, minCanny, maxCanny, 5 );
                cvShowImage("CANNY", gray);
                // dilate canny output to remove potential
                // holes between edge segments
                cvDilate( gray, gray, 0, 1 );
                cvShowImage("DIALATE", gray);
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                cvThreshold( tgray, gray, (l+1)*255/N, 255, CV_THRESH_BINARY );
                //cvThreshold( tgray, gray, 5, 255, CV_THRESH_BINARY );
                cvShowImage("THRESHOLD", gray);
            }*/

            // find contours and store them all as a list
            cvFindContours( gray, storage, &contours, sizeof(CvContour),
                CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
                
            //drawContours = contours;
            cvFindContours( gray, storage, &drawContours, sizeof(CvContour),
                CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
                
            for( ; drawContours != 0; drawContours = drawContours->h_next )
            {
                CvScalar color = CV_RGB( rand()&255, rand()&255, rand()&255 );
                /* replace CV_FILLED with 1 to see the outlines */
                cvDrawContours( contourImage, drawContours, color, color, -1, CV_FILLED, 8 );
            }
            

            
            cvShowImage("Contours", contourImage);

            // test each contour
            int count = 0;
            while( contours )
            {
                /*cout << "//////////////////////" << endl;
                cout << "Count Total: " << count << endl;
                cout << "Contours Total: " << contours->total << endl;
                cout << "Contours Elem-size: " << contours->elem_size << endl;
                count++;*/
                // approximate contour with accuracy proportional
                // to the contour perimeter
                //cout << "cvContourPerimeter(contours): " << cvContourPerimeter(contours) << endl;
                /*result = cvApproxPoly( contours, sizeof(CvContour), storage,
                    CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );*/
                result = cvApproxPoly( contours, sizeof(CvContour), storage,
                    CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.0175, 0 );
                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( result->total >= 4 &&
                    //cvContourArea(result,CV_WHOLE_SEQ,0) > 1000 &&
                    cvContourArea(result,CV_WHOLE_SEQ,0) > (imgArea * minsize) &&    //contour area covers greater than minsize of total image
                    cvContourArea(result,CV_WHOLE_SEQ,0) < (imgArea * maxsize) &&
                    cvCheckContourConvexity(result) )
                {
                    
                    

                    //cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
                    //cvPutText(img, "Hello World!", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
 
                    // my stuff
                    CvFont font;
                    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 2, CV_AA);
                    CvScalar colors = CV_RGB( rand()&255, rand()&255, rand()&255 );
                    char pointNum[10];
                    for(i = 0; i < result->total; i++)
                    {
                        sprintf(pointNum, "%01d", i);
        
                        cvPutText(img, pointNum, cvPoint(((CvPoint*)cvGetSeqElem( result, i ))->x, ((CvPoint*)cvGetSeqElem( result, i ))->y), &font, colors);
                        cvCircle(img, cvPoint(((CvPoint*)cvGetSeqElem( result, i ))->x, ((CvPoint*)cvGetSeqElem( result, i ))->y), 2, colors, 2, 8, 0); //int thickness=1, int lineType=8, int shift=0
                        //cout << "x: " << ((CvPoint*)cvGetSeqElem( result, i ))->x << ", y: " << ((CvPoint*)cvGetSeqElem( result, i ))->y << endl;
                    }
                    
                    cout << "////// A CONTOR //////" << endl;
                    s = 0;
                    
                    vector< double > angleVector;
                    vector< double > lengthVector;

                    int index = 0; // i
                    int indexOne = index+1; // i+1
                    int indexTwo = index+2; // i+2
                    cout << "Result Total: " << result->total << endl;
                    for(i = 0; i < result->total; i++)
                    {
                        if(indexOne >= result->total)
                        {
                            indexOne = 0;
                        }
                        if(indexTwo >= result->total)
                        {
                            indexTwo = 0;
                        }
                        
                        cout << "i: " << i << endl;
                        cout << "index: " << index << endl;
                        cout << "indexOne: " << indexOne << endl;
                        cout << "indexTwo: " << indexTwo << endl;
                        
                        double tempAngle = fabs(angle(
                        (CvPoint*)cvGetSeqElem( result, indexTwo ),
                        (CvPoint*)cvGetSeqElem( result, index ),
                        (CvPoint*)cvGetSeqElem( result, indexOne )));
                        
                        cout << "Recording Angle: " << (180*tempAngle)/M_PI << endl;
                        angleVector.push_back(tempAngle);
                        
                        double distanceOne = 0;
                        double distanceTwo = 0;

                        distanceOne = distance((CvPoint*)cvGetSeqElem( result, index ), (CvPoint*)cvGetSeqElem( result, indexOne ));
                        distanceTwo = distance((CvPoint*)cvGetSeqElem( result, indexOne ), (CvPoint*)cvGetSeqElem( result, indexTwo ));
                        cout << "Recording Length One: " << distanceOne << endl;
                        cout << "Recording Length Two: " << distanceTwo << endl;
                        
                        lengthVector.push_back(distanceOne);
                        lengthVector.push_back(distanceTwo);
                        
                        index++;
                        indexOne++;
                        indexTwo++;
                    }
                    
                    
                    
                    for( i = 0; i < result->total; i++ )
                    {
                        // find minimum angle between joint
                        // edges (maximum of cosine)
                        if( i >= 2 )
                        {
                            /*double tempAngle = fabs(angle(
                            (CvPoint*)cvGetSeqElem( result, i ),
                            (CvPoint*)cvGetSeqElem( result, i-2 ),
                            (CvPoint*)cvGetSeqElem( result, i-1 )));
                            
                            cout << "Recording Angle: " << (180*tempAngle)/M_PI << endl;
                            angleVector.push_back(tempAngle);
                            
                            double distanceOne = 0;
                            double distanceTwo = 0;
                            if(i < result->total - 1)
                            {
                                distanceOne = distance((CvPoint*)cvGetSeqElem( result, i-2 ), (CvPoint*)cvGetSeqElem( result, i-1 ));
                                distanceTwo = distance((CvPoint*)cvGetSeqElem( result, i-1 ), (CvPoint*)cvGetSeqElem( result, i ));
                                lengthVector.push_back(distanceOne);
                                lengthVector.push_back(distanceTwo);
                            }
                            else
                            {
                                
                            }*/
                            
                            t = fabs(angle(
                            (CvPoint*)cvGetSeqElem( result, i ),
                            (CvPoint*)cvGetSeqElem( result, i-2 ),
                            (CvPoint*)cvGetSeqElem( result, i-1 )));
                            s = s > t ? s : t;
                            //cout << "s: " << s << endl;
                            //cout << "t: " << t << endl;
                        }
                    }
                    
                    double previousAngleDiff = 1e6;
                    double closestAngle = 0;
                    double closestRatioDiff = 1e6;
                    double closestRatio = 0;
                    double ratio = 0;
                    double positionAngle = 0;
                    double positionLengthOne = 0;
                    double positionLengthTwo = 0;
                    CvSeq* correctSquarePointer;
                    bool isPath = false;
                    
                    // want 1.57 radian
                    for(i = 0; i < angleVector.size(); i++)
                    {
                        // PATH_RATIO
                        
                        cout << "Angle " << i << " : " << (180*angleVector[i])/M_PI << endl;
                        //cout << "Angle " << i << " : " << angleVector[i] << endl;
                        if(i == angleVector.size())
                        {
                            cout << "Length " << i << " : " << lengthVector[i] << endl;
                            cout << "Length " << i << " : " << lengthVector[0] << endl;
                        }
                        else
                        {
                            cout << "Length " << i << " : " << lengthVector[i] << endl;
                            cout << "Length " << i+1 << " : " << lengthVector[i+1] << endl;
                        }
                        
                        if(fabs(angleVector[i] - ANGLE_RADIANS) < previousAngleDiff)
                        {
                            previousAngleDiff = fabs(angleVector[i] - ANGLE_RADIANS);
                            closestAngle = angleVector[i];
                            positionAngle = i;
                        }
                        if(lengthVector[i] < lengthVector[i+1])
                        {
                            positionLengthOne = lengthVector[i];
                            if(i == angleVector.size())
                            {
                                positionLengthTwo = lengthVector[0];
                            }
                            else
                            {
                                positionLengthTwo = lengthVector[i+1];
                            }
                        }
                        else
                        {
                            if(i == angleVector.size())
                            {
                                positionLengthOne = lengthVector[0];
                            }
                            else
                            {
                                positionLengthOne = lengthVector[i+1];
                            }
                            positionLengthTwo = lengthVector[i];
                        }
                        
                        if((fabs(positionLengthOne/positionLengthTwo) - PATH_RATIO) < closestRatioDiff)
                        {
                            cout << "THIS IS A PATH!!!!!!!!!!!!!!!!!!!!!" << endl;
                            closestRatioDiff = fabs(positionLengthOne/positionLengthTwo) - PATH_RATIO;
                            closestRatio = fabs(positionLengthOne/positionLengthTwo);
                            correctSquarePointer = contours;
                        }
                    }
                    cout << "closestAngle: " << (180*closestAngle)/M_PI << endl;
                    cout << "closestRatio: " << closestRatio << endl;
                    
                    if(previousAngleDiff < 1.0 && closestRatioDiff < 1.0)
                    {
                        cout << "TTTTTTTTTTTTTTRRRRRRRRRRRUEEEEEEEEEEEEEE!!!!!!!!!!!" << endl;
                    }
                    
                    /*if(lengthVector[positionAngle] < lengthVector[positionAngle+1])
                    {
                        positionLengthOne = lengthVector[positionAngle];
                        if(positionAngle == angleVector.size())
                        {
                            positionLengthTwo = lengthVector[0];
                        }
                        else
                        {
                            positionLengthTwo = lengthVector[positionAngle+1];
                        }
                    }
                    else
                    {
                        if(positionAngle == angleVector.size())
                        {
                            positionLengthOne = lengthVector[0];
                        }
                        else
                        {
                            positionLengthOne = lengthVector[positionAngle+1];
                        }
                        positionLengthTwo = lengthVector[positionAngle];
                    }
                    
                    
                    if(fabs(positionLengthOne/positionLengthTwo) - PATH_RATIO < closestRatio)
                    {
                        cout << "THIS IS A PATH!!!!!!!!!!!!!!!!!!!!!" << endl;
                        correctSquarePointer = contours;
                    }*/
                    
                    //isPath

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( s < 0.5 )
                    {
                        for( i = 0; i < 4; i++ )
                        {
                            cvSeqPush( squares, (CvPoint*)cvGetSeqElem( result, i ));
                        }
                    }
                    
                    for( i = 0; i < 4; i++ )
                    {
                        if((fabs(closestAngle) - 0.5) && (fabs(positionLengthOne/positionLengthTwo) - PATH_RATIO < closestRatio))
                        {
                            cvSeqPush( squares, (CvPoint*)cvGetSeqElem( correctSquarePointer, i ));
                        }
                    }
                }

                // take the next contour
                contours = contours->h_next;
                
                //cvWaitKey(100);
            }
        }
    }

    // release all the temporary images
    cvReleaseImage( &gray );
    cvReleaseImage( &pyr );
    cvReleaseImage( &tgray );
    cvReleaseImage( &timg );

    return squares;
}


// the function draws all the squares in the image
void drawSquares( IplImage* img, CvSeq* squares )
{
    CvSeqReader reader;
    IplImage* cpy = cvCloneImage( img );
    int i;

    // initialize reader of the sequence
    cvStartReadSeq( squares, &reader, 0 );

    // read 4 sequence elements at a time (all vertices of a square)
    for( i = 0; i < squares->total; i += 4 )
    {
        CvPoint pt[4], *rect = pt;
        int count = 4;

        // read 4 vertices
        CV_READ_SEQ_ELEM( pt[0], reader );
        CV_READ_SEQ_ELEM( pt[1], reader );
        CV_READ_SEQ_ELEM( pt[2], reader );
        CV_READ_SEQ_ELEM( pt[3], reader );

        // draw the square as a closed polyline
        cvPolyLine( cpy, &rect, &count, 1, 1, CV_RGB(0,255,0), 3, CV_AA, 0 );
    }

    // show the resultant image
    cvShowImage( wndname, cpy );
    cvReleaseImage( &cpy );
}


//char* names[] = { "pic1.png", "pic2.png", "pic3.png",
//                  "pic4.png", "pic5.png", "pic6.png", 0 };

//#define BUOYS
//#define PATH
//#define PATH2
//#define PATHLONG
#define NEW_TWO_PATH

//#define GRAY_CS
#define HSL_CS


#define WIDTH 800
#define HEIGHT 600

int main(int argc, char** argv)
{
    int i, c;
    // create memory storage that will contain all the dynamic data
    storage = cvCreateMemStorage(0);
    
    //Vision::ColorSpace colorSpace;

    //cout << "GRAY" << endl;
#ifdef GRAY_CS
    colorSpace.Setup(WIDTH, HEIGHT, 1);
    IplImage* outputFull = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
    contourImage = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
    colorSpace.SetIndividualChannels(true, true, true);
    colorSpace.SetIndividualChannelHisto(true, true, true, true);
#else
    colorSpace.Setup(WIDTH, HEIGHT, 3);
    IplImage* outputAfterNoiseRemoval = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
    IplImage* outputFull = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
    IplImage* outputSingleChannel1 = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
    IplImage* outputSingleChannel2 = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
    IplImage* outputSingleChannel3 = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
    //IplImage* outputFull = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
    contourImage = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
    colorSpace.SetIndividualChannels(true, true, true);
    colorSpace.SetIndividualChannelHisto(true, true, true, true);
#endif

    int fileNum = 0;
    char fileTrainingRaw[256];

    while(1)
    {
    #ifdef BUOYS
        sprintf(fileTrainingRaw, "FWCamera/%06d.png", fileNum);
        fileNum++;
        if(fileNum >= 401)
        {
            fileNum = 0;
        }
    #endif
    #ifdef PATH
        sprintf(fileTrainingRaw, "DWCamera2/%06d.png", fileNum);
        fileNum++;
        if(fileNum >= 304)
        {
            fileNum = 0;
        }
    #endif
    #ifdef PATH2
        sprintf(fileTrainingRaw, "path/%06d.png", fileNum);
        fileNum++;
        if(fileNum >= 29)
        {
            fileNum = 0;
        }
    #endif
    #ifdef PATHLONG
        sprintf(fileTrainingRaw, "pathlong/%06d.png", fileNum);
        fileNum++;
        if(fileNum >= 1082)
        {
            fileNum = 0;
        }
    #endif
    #ifdef NEW_TWO_PATH
        sprintf(fileTrainingRaw, "new_two_paths/DownwardCamera/%06d.png", fileNum);
        fileNum++;
        if(fileNum > 163)
        {
            fileNum = 0;
        }
    #endif
    
        
        cout << fileTrainingRaw << endl;
        // load i-th image
        img0 = cvLoadImage(fileTrainingRaw, CV_LOAD_IMAGE_COLOR);
        if( !img0 )
        {
            cout << "Error Loading Image: Raw Image" << endl;
            return 0;
        }
        
        /*for( i = 0; names[i] != 0; i++ )
        {
        // load i-th image
        img0 = cvLoadImage( names[i], 1 );
        if( !img0 )
        {
            printf("Couldn't load %s\n", names[i] );
            continue;
        }*/
        
        img = cvCloneImage( img0 );
        contourImage = cvCloneImage( img0 );

        // create window and a trackbar (slider) with parent "image" and set callback
        // (the slider regulates upper threshold, passed to Canny edge detector)
        cvNamedWindow( wndname, 1 );
        
        

        // find and draw the squares
        drawSquares( img, findSquares4( img, storage, 0.001, 1, 400, 500) );

        // wait for key.
        // Also the function cvWaitKey takes care of event processing
        cvWaitKey(100);
        //cvWaitKey(25);
        // release both images
        
        cvReleaseImage( &contourImage );
        
        cvReleaseImage( &img );
        cvReleaseImage( &img0 );
        // clear memory storage - reset free space position
        cvClearMemStorage( storage );
        

        
    }
    
    cvDestroyWindow( wndname );

    return 0;
}