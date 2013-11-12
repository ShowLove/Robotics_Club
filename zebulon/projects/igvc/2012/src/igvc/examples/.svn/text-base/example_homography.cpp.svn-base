//#include "ground/vision/homography.h"
#include "homography/homography.h"
#include "cxutils/fileio.h"
using namespace Zebulon;
using namespace Vision;

IplImage * gImageIn = NULL;
IplImage * gImageOut = NULL;
double gZoom = 1;
double gAngle = 0;
double gHorizontal = 0;
double gVertical = 0;
int main(int argc, char** argv)
{
    int key = 0;
    Homography homography;
    homography.LoadSettings();
    if(argc < 2)
    {
        printf("must specify file\nExiting..");
        return -1;
    }
    cvNamedWindow("Perspective", CV_WINDOW_AUTOSIZE );
    cvNamedWindow("Input", CV_WINDOW_AUTOSIZE );
    cvNamedWindow("Output", CV_WINDOW_AUTOSIZE);
    std::vector<std::string> files;
    CxUtils::FileIO::GetFiles(files,"*.png",argv[1],false);

    for(int i = 0; i < files.size(); i++)
    {
        printf("%s:\n",files[i].c_str());
        if( (gImageIn = cvLoadImage(files[i].c_str(), CV_LOAD_IMAGE_COLOR)) == 0 )
        {
            printf("unable to load file \n");
            continue;
        }
        gImageOut = homography.Warp(gImageIn);
        while(key != 1048603)
        {
            cvShowImage("Input", gImageIn);
            cvShowImage("Perspective", gImageOut);
            key = cvWaitKey(0);
            cvReleaseImage(&gImageOut);
            //up arrow, zoom in
            if(key== 1113938)
            {
                homography.SetProperty(Homography::Zoom, .1);
            }
            //down arrow, zoom out
            if(key== 1113940)
            {
                homography.SetProperty(Homography::Zoom, -.1);
            }
            //left arrow, rotate counter clockwise
            if(key == 1113937)
            {
                homography.SetProperty(Homography::Slant, .1);
            }
            //right arrow, rotate counter clockwise
            if(key == 1113939)
            {
                homography.SetProperty(Homography::Slant, -.1);
            }
            //w, translate up
            if(key == 1048695)
            {
                homography.SetProperty(Homography::Vertical, -10);
            }
            //s, translate down
            if(key == 1048691)
            {
                homography.SetProperty(Homography::Vertical, 10);
            }
            //a, translate left
            if(key == 1048673)
            {
                homography.SetProperty(Homography::Horizontal, -10);
            }
            //d, translate right
            if(key == 1048676)
            {
                homography.SetProperty(Homography::Horizontal, 10);
            }
            //space, next image
            if(key == 1048608)
            {
                break;
            }

            if(homography.CheckerBoardCalibrate(gImageIn))
            {
                gImageOut = homography.Warp(gImageIn);
            }
        }
        if(key == 1048603) break;
        cvReleaseImage(&gImageIn);
        cvReleaseImage(&gImageOut);
    }
    return 0;
}
