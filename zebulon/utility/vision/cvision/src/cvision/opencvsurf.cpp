#include "cvision/opencvsurf.h"

using namespace Zebulon;
using namespace Vision;
using namespace std;

//#define OPENCVSURF_DISPLAY
//#define OPENCVSURF_VERBOSE

OpenCvSurf::OpenCvSurf()
{
    //mMinPointPairs = 4;
    mMinPointPairs = 20;
}

OpenCvSurf::~OpenCvSurf()
{
    
}

double OpenCvSurf::CompareSURFDescriptors( const float* d1, const float* d2, double best, int length )
{
    double total_cost = 0;
    assert( length % 4 == 0 );
    for( int i = 0; i < length; i += 4 )
    {
        double t0 = d1[i] - d2[i];
        double t1 = d1[i+1] - d2[i+1];
        double t2 = d1[i+2] - d2[i+2];
        double t3 = d1[i+3] - d2[i+3];
        total_cost += t0*t0 + t1*t1 + t2*t2 + t3*t3;
        if( total_cost > best )
            break;
    }
    return total_cost;
}

int OpenCvSurf::NaiveNearestNeighbor( const float* vec, int laplacian,
                    const CvSeq* model_keypoints,
                    const CvSeq* model_descriptors )
{
    int length = (int)(model_descriptors->elem_size/sizeof(float));
    int i, neighbor = -1;
    double d, dist1 = 1e6, dist2 = 1e6;
    CvSeqReader reader, kreader;
    cvStartReadSeq( model_keypoints, &kreader, 0 );
    cvStartReadSeq( model_descriptors, &reader, 0 );

    for( i = 0; i < model_descriptors->total; i++ )
    {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* mvec = (const float*)reader.ptr;
        CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
        CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
        if( laplacian != kp->laplacian )
            continue;
        d = CompareSURFDescriptors( vec, mvec, dist2, length );
        if( d < dist1 )
        {
            dist2 = dist1;
            dist1 = d;
            neighbor = i;
        }
        else if ( d < dist2 )
            dist2 = d;
    }
    if ( dist1 < 0.6*dist2 )
        return neighbor;
    return -1;
}

void OpenCvSurf::FindPairs( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
                    const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, vector<int>& ptpairs )
{
    int i;
    CvSeqReader reader, kreader;
    cvStartReadSeq( objectKeypoints, &kreader );
    cvStartReadSeq( objectDescriptors, &reader );
    ptpairs.clear();

    for( i = 0; i < objectDescriptors->total; i++ )
    {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* descriptor = (const float*)reader.ptr;
        CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
        CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
        int nearest_neighbor = NaiveNearestNeighbor( descriptor, kp->laplacian, imageKeypoints, imageDescriptors );
        if( nearest_neighbor >= 0 )
        {
            ptpairs.push_back(i);
            ptpairs.push_back(nearest_neighbor);
        }
    }
}

int OpenCvSurf::LocatePlanarObject( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
                    const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
                    const CvPoint src_corners[4], CvPoint dst_corners[4] )
{
    double h[9];
    CvMat _h = cvMat(3, 3, CV_64F, h);
    vector<int> ptpairs;
    vector<CvPoint2D32f> pt1, pt2;
    CvMat _pt1, _pt2;
    int i, n;

    FindPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );

    n = ptpairs.size()/2;
    //if( n < 4 )
    if (n < mMinPointPairs)
        return 0;

    pt1.resize(n);
    pt2.resize(n);
    for( i = 0; i < n; i++ )
    {
        pt1[i] = ((CvSURFPoint*)cvGetSeqElem(objectKeypoints,ptpairs[i*2]))->pt;
        pt2[i] = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,ptpairs[i*2+1]))->pt;
    }

    _pt1 = cvMat(1, n, CV_32FC2, &pt1[0] );
    _pt2 = cvMat(1, n, CV_32FC2, &pt2[0] );
    if( !cvFindHomography( &_pt1, &_pt2, &_h, CV_RANSAC, 5 ))
        return 0;

    for( i = 0; i < 4; i++ )
    {
        double x = src_corners[i].x, y = src_corners[i].y;
        double Z = 1./(h[6]*x + h[7]*y + h[8]);
        double X = (h[0]*x + h[1]*y + h[2])*Z;
        double Y = (h[3]*x + h[4]*y + h[5])*Z;
        dst_corners[i] = cvPoint(cvRound(X), cvRound(Y));
    }

    return 1;
}

SurfReturn OpenCvSurf::FindObject(IplImage* src, IplImage* object, double param1, double param2)
{
    CvMemStorage* storage = cvCreateMemStorage(0);
    SurfReturn output;
    
    if( !src || !object )
    {
        cout << "OpenCvSurf: Error Loading Source or Object!" << endl;
        
        exit(-1);
    }
    
#ifdef DRAW_OBJECT
    IplImage* object_color = cvCreateImage(cvGetSize(object), 8, 3);
    cvCvtColor( object, object_color, CV_GRAY2BGR );
    cvNamedWindow("Object", 1);
#endif

#ifdef DRAW
    cvNamedWindow("Object Correspond", 1);
#endif
    
    CvSeq *objectKeypoints = 0, *objectDescriptors = 0;
    CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
    int i;
    CvSURFParams params = cvSURFParams(param1, param2);
    
    double tt = (double)cvGetTickCount();
    cvExtractSURF( object, 0, &objectKeypoints, &objectDescriptors, storage, params );
#ifdef DEBUG
    cout << "Object Descriptors: " << objectDescriptors->total << endl;
#endif
    
#ifdef DRAW_OBJECT
    for( i = 0; i < objectKeypoints->total; i++ )
    {
        CvSURFPoint* r = (CvSURFPoint*)cvGetSeqElem( objectKeypoints, i );
        CvPoint center;
        int radius;
        center.x = cvRound(r->pt.x);
        center.y = cvRound(r->pt.y);
        radius = cvRound(r->size*1.2/9.*2);
        cvCircle( object_color, center, radius, cvScalar(0, 0, 255, 0), 1, 8, 0 );
    }
    cvShowImage( "Object", object_color );
    cvWaitKey(10);
    cvReleaseImage(&object_color);
#endif

    cvExtractSURF( src, 0, &imageKeypoints, &imageDescriptors, storage, params );
#ifdef DEBUG
    cout << "Image Descriptors: " << imageDescriptors->total << endl;
#endif
    //printf("Image Descriptors: %d\n", imageDescriptors->total);
    tt = (double)cvGetTickCount() - tt;
#ifdef DEBUG
    cout << "Extraction time = " << tt/(cvGetTickFrequency()*1000.) << endl;
#endif
    //printf( "Extraction time = %gms\n", tt/(cvGetTickFrequency()*1000.));
    CvPoint src_corners[4] = {{0,0}, {object->width,0}, {object->width, object->height}, {0, object->height}};
    CvPoint dst_corners[4];
#ifdef OPENCVSURF_DISPLAY
    IplImage* correspond = cvCreateImage( cvSize(src->width, object->height+src->height), 8, 1 );
    memset(correspond->imageData,0,correspond->imageSize);
    cvSetImageROI( correspond, cvRect( 0, 0, object->width, object->height ) );
    cvCopy( object, correspond );
    cvSetImageROI( correspond, cvRect( 0, object->height, correspond->width, correspond->height ) );
    cvCopy( src, correspond );
    cvResetImageROI( correspond );
#endif
    
    output.Valid = 0;
    
    if( LocatePlanarObject( objectKeypoints, objectDescriptors, imageKeypoints,
            imageDescriptors, src_corners, dst_corners ))
    {
        
        output.CenterI = 0;
        output.CenterJ = 0;
        
        double tempLowX = 1e6;
        double tempLowY = 1e6;
        
        double tempHighX = -1;
        double tempHighY = -1;
        
        double tempLength = 0;
        double tempAngleX1 = 0;
        double tempAngleY1 = 0;
        double tempAngleX2 = 0;
        double tempAngleY2 = 0;
        
        for( i = 0; i < 4; i++ )
        {
            CvPoint r1 = dst_corners[i%4];
            CvPoint r2 = dst_corners[(i+1)%4];
#ifdef OPENCVSURF_DISPLAY
            cvLine( correspond, cvPoint(r1.x, r1.y+object->height ),
                cvPoint(r2.x, r2.y+object->height ), cvScalar(0, 0, 0, 0) );
#endif
                
            // Quickly done, just for boat
            // center - find average of points
            // find lower left x,y and upper right x,y
#ifdef DEBUG
            cout << " i : " << i << endl;
            cout << "x: " << r1.x << ", y: " << r1.y << endl;
#endif
            
            output.CenterI += r1.x/4.0;
            output.CenterJ += r1.y/4.0;
            
            if(r1.x < tempLowX)
            {
                tempLowX = r1.x;
                tempLowY = r1.y;
            }
            if(r1.y > tempHighY)
            {
                tempHighX = r1.x;
                tempHighY = r1.y;
            }
            
            int x = r1.x - r2.x;
            int y = r1.y - r2.y;
            double len = sqrt(x*x+y*y);
            if(len > tempLength)
            {
                tempLength = len;
                tempAngleX1 = r1.x;
                tempAngleY1 = r1.y;
                tempAngleX2 = r2.x;
                tempAngleY2 = r2.y;
            }
        }
        
        // This can't be right, taking the square root of a square?
        //  I assume it was supposed to be sqrt((x1-x2)*(y1-y2))...
        //  Please check this, returns NAN most of the time.
        //double tempLength2 = sqrt((tempAngleX1 - tempAngleX2)*(tempAngleX1 - tempAngleX2));
        double tempLength2 = sqrt((tempAngleX1 - tempAngleX2)*(tempAngleY1 - tempAngleY2));
        
        output.Angle = (180/M_PI)*acos(tempLength2/tempLength);
        
        // check angle finding and center point
        //cvCircle( correspond, cvPoint(output.CenterI, output.CenterJ), 5, cvScalar(0, 0, 255, 0), 1, 8, 0 );
        //cvLine( correspond, cvPoint(tempAngleX1, tempAngleY1), cvPoint(tempAngleX2, tempAngleY2), cvScalar(127, 127, 127, 0) );
        //cvLine( correspond, cvPoint(tempAngleX1, tempAngleY1), cvPoint(tempAngleX2, tempAngleY1), cvScalar(127, 127, 127, 0) );
        
        output.Area = fabs(tempLowX - tempHighX) * fabs(tempLowY - tempHighY);
        
        output.Valid = 1;
    }
    
    if(output.Valid == 0)
    {
        output.CenterI = 0;
        output.CenterJ = 0;
        output.Area = 0;
    }
    
    vector<int> ptpairs;
    FindPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#ifdef OPENCVSURF_DISPLAY
    for( i = 0; i < (int)ptpairs.size(); i += 2 )
    {
        CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem( objectKeypoints, ptpairs[i] );
        CvSURFPoint* r2 = (CvSURFPoint*)cvGetSeqElem( imageKeypoints, ptpairs[i+1] );

        cvLine( correspond, cvPointFrom32f(r1->pt),
            cvPoint(cvRound(r2->pt.x), cvRound(r2->pt.y+object->height)), cvScalar(255, 0, 0, 0) );
    }
#endif
    
#ifdef DRAW
    cvShowImage( "Object Correspond", correspond );
    cvWaitKey(10);
#endif
#ifdef OPENCVSURF_DISPLAY
    cvReleaseImage(&correspond);
#endif
    cvReleaseMemStorage(&storage);
    
    return output;
}

void OpenCvSurf::CvtBGRtoHSGray(IplImage* src, IplImage* dest)
{
    IplImage* srcHSV;
    srcHSV = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 3);
    cvCvtColor(src, srcHSV, CV_BGR2HSV);

    //IplImage* srcHS;
    //srcHS = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
    for (int y=0; y<src->height; y++)
    {
        for (int x=0; x<src->width; x++)
        {
            ((uchar*)(dest->imageData + dest->widthStep*y))[x] = (((uchar*)(srcHSV->imageData + srcHSV->widthStep*y))[x*3])/2 + (((uchar*)(srcHSV->imageData + srcHSV->widthStep*y))[x*3+1])/2;
        }
    }
    cvReleaseImage(&srcHSV);
}
