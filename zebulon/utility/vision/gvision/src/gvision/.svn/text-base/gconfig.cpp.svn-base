///
/// Written by Gary Stein
///

///////////////////////////////////////////////////////////////////
/// Key     Usage
/// ===============================================================
/// X       Resumes playback
/// R       Use ThreshRatio
/// H       Use Threshhold
/// P       output XML
/// Z       Pauses playback
/// ------------------------ SUB ONLY -----------------------------
/// W       White-balance
/// S       Adjust Shutter
///////////////////////////////////////////////////////////////////

#include <cvcapture/cvimagecapture.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <cxutils/mutex.h>

#ifdef WIN32
//#include "vld.h"
#endif


#include <stdio.h>
#include <vector>
#include <string>
#include <time.h>

using namespace std;
using namespace Zebulon;

#include "gvision/target.h"
#include "gvision/return.h"

#include "gvision/threshratio.h"
#include "gvision/segment.h"

#define SUBHACK

#ifdef SUBHACK
//Testing extreme hack for sub
#include <fw/fw1394Camera.h>

int RedBalance,BlueBalance;
int OldRedBalance,OldBlueBalance;

int Shutter;
int OldShutter;
int Avg;
int OldAvg;
#endif

IplImage *displayImage=NULL;
IplImage *threshImage=NULL;
IplImage *segmentImage=NULL;

Vision::Threshhold *currThresh=NULL;

Vision::Threshhold *hold=NULL;
Vision::ThreshRatio *ratio=NULL;

Vision::Segment *seg=NULL;

CxUtils::Mutex displayMutex;

Vision::Target target;

int edge;
int MinArea=DEFAULTMINAREA;
int ComboDist=DEFAULTCOMBODIST;


#define AVGWIN 3

void Foo(int event, int x, int y, int flags, void* param)
{
    if(event==CV_EVENT_LBUTTONDOWN)
    {
        displayMutex.Lock();
        unsigned char *imageData=(unsigned char *)displayImage->imageData;
        int pos=y*displayImage->widthStep+3*x;

        printf("%d %d - %d %d %d\n",x,y,imageData[pos+0],imageData[pos+1],imageData[pos+2]);
        int size=0;
        int avgr=0;
        int avgg=0;
        int avgb=0;

        int starty=y-AVGWIN;
        int endy=y+AVGWIN;
        if(starty<0)
        {
            starty=0;
        }
        if(endy>=displayImage->height)
        {
            endy=displayImage->height;
        }

        int startx=x-AVGWIN;
        int endx=x+AVGWIN;
        if(startx<0)
        {
            startx=0;
        }
        if(endx>=displayImage->width)
        {
            endx=displayImage->width;
        }

        for(int ty=starty;ty<endy;ty++)
        {
            for(int tx=startx;tx<endx;tx++)
            {
                size++;
                pos=ty*displayImage->widthStep+3*tx;
                avgr+=(int)imageData[pos+2];
                avgg+=(int)imageData[pos+1];
                avgb+=(int)imageData[pos+0];
            }
        }
        avgr/=size;
        avgg/=size;
        avgb/=size;
        displayMutex.Unlock();
        printf("%d %d - %d %d %d\n",x,y,avgb,avgg,avgr);

        //cvSetTrackbarPos("Red","adj",imageData[pos+2]);
        //cvSetTrackbarPos("Green","adj",imageData[pos+1]);
        //cvSetTrackbarPos("Blue","adj",imageData[pos+0]);
        cvSetTrackbarPos("Red","adj",avgr);
        cvSetTrackbarPos("Green","adj",avgg);
        cvSetTrackbarPos("Blue","adj",avgb);
    }
}

class mycallback : public Video::CvImageCapture::Callback
{
    virtual void ProcessCameraData(Video::CvImageCapture::Image* dest)
    {
        if(mLoadflag==1)
        {
            displayMutex.Lock();
            Video::CvConvertToIplImage(*dest, &displayImage);
            displayMutex.Unlock();
            mLoadCount++;
        }
    }
    public:
       static int mLoadflag;
       static int mLoadCount;
};
int mycallback::mLoadflag=1;
int mycallback::mLoadCount=0;


#define DRAW
#define CAMERA
//#define SAVE

void SetupCV()
{
    cvNamedWindow("adj", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("normal", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("after", CV_WINDOW_AUTOSIZE);
    cvCreateTrackbar("Red","adj",&target.mRed,255,NULL);
    cvCreateTrackbar("RRange","adj",&target.mRedRange,255,NULL);
    cvCreateTrackbar("Green","adj",&target.mGreen,255,NULL);
    cvCreateTrackbar("GRange","adj",&target.mGreenRange,255,NULL);
    cvCreateTrackbar("Blue","adj",&target.mBlue,255,NULL);
    cvCreateTrackbar("BRange","adj",&target.mBlueRange,255,NULL);

    cvCreateTrackbar("Edge","adj",&edge,25,NULL);
    cvCreateTrackbar("MinArea","adj",&MinArea,1000,NULL);
    cvCreateTrackbar("ComboDist","adj",&ComboDist,320,NULL);

#ifdef SUBHACK
    cvCreateTrackbar("RedBalance","adj",&RedBalance,568,NULL);
    cvCreateTrackbar("BlueBalance","adj",&BlueBalance,568,NULL);
    cvSetTrackbarPos("RedBalance","adj",0);
    cvSetTrackbarPos("BlueBalance","adj",0);
    OldRedBalance=RedBalance;
    OldBlueBalance=BlueBalance;

    cvCreateTrackbar("Shutter","adj",&Shutter,4095,NULL);
    cvSetTrackbarPos("Shutter","adj",0);
    OldShutter=Shutter;

    cvCreateTrackbar("Avg","adj",&Avg,255,NULL);
    cvSetTrackbarPos("Avg","adj",128);
    OldAvg=Avg;

#endif

    cvSetTrackbarPos("Red","adj",127);
    cvSetTrackbarPos("Green","adj",127);
    cvSetTrackbarPos("Blue","adj",127);

    cvSetTrackbarPos("RRange","adj",10);
    cvSetTrackbarPos("BRange","adj",10);
    cvSetTrackbarPos("GRange","adj",10);

    cvSetTrackbarPos("Edge","adj",0);
    cvSetTrackbarPos("MinArea","adj",DEFAULTMINAREA);
    cvSetTrackbarPos("ComboDist","adj",DEFAULTCOMBODIST);

    cvSetMouseCallback("normal",Foo,NULL);  
}

//#define IMAGE_WIDTH 1280
//#define IMAGE_HEIGHT 960
//#define IMAGE_WIDTH 1024
//#define IMAGE_HEIGHT 768
#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 600

int main(int argc, char *argv[])
{
    int pauseFlag=0;
    Video::CvImageCapture capture;
    Video::CvImageCapture::Image src;
    std::vector<std::string> sources;
    Vision::Return ret;
    char filename[1024];
    int imageCount=0;

    SetupCV();
    
    hold=new Vision::Threshhold();
    ratio=new Vision::ThreshRatio();
    seg=new Vision::Segment();
    
    //default to ratio
    currThresh=ratio;

    printf("Press <enter> key to exit. Press the <c> key to save picture\n");
    printf("Initializing Image Capture...\n");
    capture.RegisterCallback(new mycallback());
    if(argc > 1)
    {
        if(argc > 2)
        {
            capture.Start(argv[1], IMAGE_WIDTH, IMAGE_HEIGHT, false, false);
        }
        else
        {
            capture.Start(argv[1], IMAGE_WIDTH, IMAGE_HEIGHT, false, true);
        }
    }
    else
    {
        printf("Needs to have a camera\n");
        return -1;
    }
    
#ifdef SUBHACK
    //start off as true
    bool autoWhiteFlag=true;
    bool autoShutterFlag=true;
    fw1394Camera *camera=(fw1394Camera *)capture.GetRawCapture();
    camera->enableAutoWhite(autoWhiteFlag);
    camera->enableAutoShutter(autoShutterFlag);

    //Values for darker front camera
    //camera->SetShutter(150);
    //camera->SetWhiteBalance(465,492);

    //Bottom Camera
    //camera->SetShutter(150);
    //camera->SetWhiteBalance(465,492);

    camera->setPTerm(.05);
    //camera->debugDisplay(true);
#endif
    while(1)
    {

        /*if(mycallback::mLoadflag==1)
        {
            capture.GetFrame(&src);
            CvConvertToIplImage(src, &displayImage);
        }*/
        if(pauseFlag==1)
        {
            cvWaitKey(50);
            capture.Pause();
            pauseFlag=0;
        }

        if(displayImage==NULL)
        {
            cvWaitKey(50);
            continue;
        }
        //printf("Width %d, Height %d\n", displayImage->width, displayImage->height);
        cvShowImage("normal",displayImage);
#ifdef SAVE
        //cout << "In SAVE: " << endl;
        //sprintf(filename,"/home/robotics/zebulon/v4/software/projects/auvc/2011/bin/logs/image-%06d.png",imageCount);
        sprintf(filename,"logs/extra/image-%06d.png",imageCount);
        
        //cout << "to: " << filename << endl;
        cvSaveImage(filename,displayImage);
        imageCount++;
#endif

        currThresh->Setup(target,edge);
        currThresh->Process(displayImage,threshImage);
        
        seg->Setup(target.mRed,target.mGreen,target.mBlue,MinArea,ComboDist);
        seg->Process(threshImage,segmentImage);
        
        seg->GetMaxPosition(ret);
        
        //printf("Width %d, Height %d\n", displayImage->width, displayImage->height);
        printf("Valid %d I %lf J %lf Area %d\n",ret.mValid,ret.mCenterI,ret.mCenterJ,ret.mArea);
        
        //cvShowImage("after",threshImage);
        cvShowImage("after",segmentImage);


#ifdef SUBHACK
    if(RedBalance!=0 || BlueBalance!=0)
    {
        if(OldRedBalance!=RedBalance || OldBlueBalance!=BlueBalance)
        {
            camera->SetWhiteBalance(RedBalance,BlueBalance);
            OldRedBalance=RedBalance;
            OldBlueBalance=BlueBalance;
        }
    }
    
    if(Shutter!=0)
    {
        if(OldShutter!=Shutter)
        {
            camera->SetShutter(Shutter);
            OldShutter=Shutter;
        }
    }

    if(Avg!=0)
    {
        if(OldAvg!=Avg)
        {
            camera->SetDesiredAvg(Avg);
            OldAvg=Avg;
        }
    }

    
    
#endif

        int c=cvWaitKey(25);

        
        if(c>0)
        {
            switch(c)
            {
                case 122:
                case 1048698:
                    mycallback::mLoadflag=0;
                    break;
                // X
                //  Resumes playback
                case 120:
                case 1048696:
                    mycallback::mLoadflag=1;
                    capture.Resume();
                    pauseFlag=0;
                    break;
                // R
                //  Use ThreshRatio
                case 114:
                case 1048690:
                    currThresh=ratio;
                    break;
                // H
                //  Use Threshhold
                case 104:
                case 1048680:
                    currThresh=hold;
                    break;
                // P
                //  output XML
                case 1048688:
                case 112:
                    std::cout << "<Target Red=\"" << target.mRed << "\" Green=\"" << target.mGreen << "\" Blue=\"" << target.mBlue << "\" />\n";
                    std::cout << "<TargetThresh Red=\"" << target.mRedRange << "\" Green=\"" << target.mGreenRange << "\" Blue=\"" << target.mBlueRange << "\" />\n";
#ifdef SUBHACK
                        camera->GetWhiteBalance(RedBalance,BlueBalance);
                        camera->GetShutter(Shutter);
                        std::cout << "<WhiteBalance Red=\"" << RedBalance << "\" Blue=\"" << BlueBalance << "\" />\n";
                        std::cout << "<Shutter Value=\"" << Shutter << "\" />\n";
#endif

                    cvWaitKey(5000);
                    break;
                // Z
                //  Pauses playback
                case 1048608:
                case 32:
                    if(pauseFlag==0)
                    {
                        mycallback::mLoadflag=1;
                        capture.Resume();
                        pauseFlag=1;
                    }
                break;
                
#ifdef SUBHACK
                // W
                //  White balance
                //find non-unicode
                case 1048695:
                case 119:
                    autoWhiteFlag=!autoWhiteFlag;
                    camera->enableAutoWhite(autoWhiteFlag);
                    if(autoWhiteFlag==false)
                    {
                        camera->GetWhiteBalance(RedBalance,BlueBalance);
                        std::cout << "<WhiteBalance Red=\"" << RedBalance << "\" Blue=\"" << BlueBalance << "\" />\n";
                        cvSetTrackbarPos("RedBalance","adj",RedBalance);
                        cvSetTrackbarPos("BlueBalance","adj",BlueBalance);
                        camera->debugDisplay(false);
                        cvWaitKey(5000);
                    }
                    else
                    {
                        camera->debugDisplay(true);
                        cvSetTrackbarPos("RedBalance","adj",0);
                        cvSetTrackbarPos("BlueBalance","adj",0);
                    }
                    break;
                    
                // S
                //  Shutter adjustment
                //find non-unicode
                case 1048691:
                case 115:
                    autoShutterFlag=!autoShutterFlag;
                    camera->enableAutoShutter(autoShutterFlag);
                    if(autoShutterFlag==false)
                    {
                        camera->GetShutter(Shutter);
                        std::cout << "<Shutter Value=\"" << Shutter << "\" />\n";
                        cvSetTrackbarPos("Shutter","adj",Shutter);
                        camera->debugDisplay(false);
                        cvWaitKey(5000);
                    }
                    else
                    {
                        cvSetTrackbarPos("Shutter","adj",0);
                        camera->debugDisplay(true);
                    }
                    break;
#endif
            }
            printf("Key: %d\n",c);
        }
    }

    cvDestroyAllWindows();

    return 0;
}
