#include "cvision/opencvsurf.h"
#include <stdio.h>

using namespace std;
using namespace Zebulon;
using namespace Vision;

std::string object_path;
std::string image_seq_path;

int main(int argc, char** argv)
{
    OpenCvSurf surf;
    surf.SetMinPointPairs(28);
    SurfReturn surfInfo;
    
    IplImage* object = NULL;
    IplImage* objectHLS = NULL;
    IplImage* objectHS = NULL;

    IplImage* src = NULL;
    IplImage* srcHLS = NULL;
    IplImage* srcHS = NULL;
    
    char loadImageName[256];
    //int fileNum = 293;
    int fileNum = 0;
    IplImage *img = NULL;
    IplImage *correspond_color = NULL;
    
    if (argc > 2)
    {
        object_path = argv[1];
        image_seq_path = argv[2];
    }
    else
    {
        object_path = "path_object.png";
        image_seq_path = "pathlong/";
    }

    //object = cvLoadImage( "path_object.png", CV_LOAD_IMAGE_GRAYSCALE );
    object = cvLoadImage( object_path.c_str() );
    objectHLS = cvCreateImage(cvSize(object->width, object->height), IPL_DEPTH_8U, 3);
    //cvCvtColor(object, objectHLS, CV_BGR2HLS);
    cvCvtColor(object, objectHLS, CV_BGR2HSV);

    objectHS = cvCreateImage(cvSize(object->width, object->height), IPL_DEPTH_8U, 1);
    OpenCvSurf::CvtBGRtoHSGray(object, objectHS);

    cvShowImage( "Object", objectHS );
    
    while(1)
    {
        //sprintf(loadImageName, "pathlong/%06d.png", fileNum);
        sprintf(loadImageName, "%s%06d.png", image_seq_path.c_str(), fileNum);
        fileNum++;
        if(fileNum >= 1081)
        {
            fileNum = 0;
        }
        
        
        src = cvLoadImage( loadImageName );
        srcHS = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
        OpenCvSurf::CvtBGRtoHSGray(src, srcHS);
        cvShowImage( "Pic", srcHS );

        surfInfo = surf.FindObject(srcHS, objectHS, 50, 5);
        
        cout << "Valid: " << surfInfo.Valid << endl;
        cout << "CenterI: " << surfInfo.CenterI << endl;
        cout << "CenterJ: " << surfInfo.CenterJ << endl;
        cout << "Angle: " << surfInfo.Angle << endl;
        cout << "Area: " << surfInfo.Area << endl;

        int x = surfInfo.CenterI;
        int y = surfInfo.CenterJ;
        if (surfInfo.Valid)
        {
            cvDrawRect(src, cvPoint(x-100, y-100), cvPoint(x+100, y+100), cvScalar(0), 3);
        }

        cvShowImage( "Output", src );

        cvWaitKey(10);
        
        cvReleaseImage(&src);
        cvReleaseImage(&srcHS);
    }
    
    cvReleaseImage(&object);
    
    return 0;
}
