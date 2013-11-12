#include "auvc/underwater/calibrate.h"

using namespace Zebulon;
using namespace Underwater;

Calibrate::Calibrate()
{
    ID = "Calibrate";
    
    SetupCV();
    
    displayImage=NULL;
    threshImage=NULL;
    segmentImage=NULL;

    currThresh=NULL;

    hold=new Vision::Threshhold();
    ratio=new Vision::ThreshRatio();
    seg=new Vision::Segment();
    //default to ratio
    currThresh=ratio;
    
    LoadFlag=1;
    autoFlag=0;
}

Calibrate::~Calibrate()
{

}

int Calibrate::Init()
{
    mState = PutStatesHere;
    std::cout << "Init Calibrate\n";

    mXmlConf.GetVar("Underwater.Calibrate.Target@Red", target.mRed);
    mXmlConf.GetVar("Underwater.Calibrate.Target@Green", target.mGreen);
    mXmlConf.GetVar("Underwater.Calibrate.Target@Blue", target.mBlue);
    mXmlConf.GetVar("Underwater.Calibrate.TargetThresh@Red", target.mRedRange);
    mXmlConf.GetVar("Underwater.Calibrate.TargetThresh@Green", target.mGreenRange);
    mXmlConf.GetVar("Underwater.Calibrate.TargetThresh@Blue", target.mBlueRange);


    cvSetTrackbarPos("Red","adj",target.mRed);
    cvSetTrackbarPos("Green","adj",target.mGreen);
    cvSetTrackbarPos("Blue","adj",target.mBlue);

    cvSetTrackbarPos("RRange","adj",target.mRedRange);
    cvSetTrackbarPos("BRange","adj",target.mGreenRange);
    cvSetTrackbarPos("GRange","adj",target.mBlueRange);
    
    
    CxUtils::SleepMs(1000);
    return 0;
}

int Calibrate::ErrorCheck()
{
    return 0;
}

int Calibrate::FetchProcess()
{
    displayMutex.Lock();
    if(LoadFlag==1)
    {
        switch(Camera)
        {
            case 0:
                // Forward
                if(!mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera, displayImage))
                {
                    printf("ERROR::Calibrate: Couldn't get Forward camera data");
                    displayMutex.Unlock();
                    return 1;
                }
                break;
            case 1:
                // Down
                if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, displayImage))
                {
                    printf("ERROR::Calibrate: Couldn't get Downward camera data");
                    displayMutex.Unlock();
                    return 1;
                }
                break;
        }
    }
    if(displayImage == NULL)
    {
        displayMutex.Unlock();
        return 1;
    }
    
    currThresh->Setup(target,edge);
    currThresh->Process(displayImage,threshImage);

    seg->Setup(target.mRed,target.mGreen,target.mBlue,MinArea,ComboDist);
    seg->Process(threshImage,segmentImage);

    seg->GetMaxPosition(ret);
    printf("Valid %d I %lf J %lf Area %d\n",ret.mValid,ret.mCenterI,ret.mCenterJ,ret.mArea);

    displayMutex.Unlock();
    return 0;
}

std::string Calibrate::ExitEarly()
{
    return "KeepRunning";
}

std::string Calibrate::Execute()
{

    int c=cvWaitKey(1);
    if(c>0)
    {
        switch(c)
        {
            case 122:
            case 1048698:
                LoadFlag=0;
                break;
            case 120:
            case 1048696:
                LoadFlag=1;
                break;
            case 114:
            case 1048690:
                currThresh=ratio;
                break;
            case 104:
            case 1048680:
                currThresh=hold;
                break;
            // P
            //output XML
            case 1048688:
            case 112:
                std::cout << "<Target Red=\"" << target.mRed << "\" Green=\"" << target.mGreen << "\" Blue=\"" << target.mBlue << "\" />\n";
                std::cout << "<TargetThresh Red=\"" << target.mRedRange << "\" Green=\"" << target.mGreenRange << "\" Blue=\"" << target.mBlueRange << "\" />\n";
                cvWaitKey(5000);
                break;
        }
        printf("Key: %d\n",c);
    }
    Utility::Display::DisplayImage("normal", displayImage);
    Utility::Display::DisplayImage("post",segmentImage);
    return "KeepRunning";
}

void Calibrate::SetupCV()
{
    cvNamedWindow("adj", CV_WINDOW_AUTOSIZE);
    Utility::Display::CreateWindow("normal");
    Utility::Display::CreateWindow("post");
    cvCreateTrackbar("Red","adj",&target.mRed,255,NULL);
    cvCreateTrackbar("RRange","adj",&target.mRedRange,255,NULL);
    cvCreateTrackbar("Green","adj",&target.mGreen,255,NULL);
    cvCreateTrackbar("GRange","adj",&target.mGreenRange,255,NULL);
    cvCreateTrackbar("Blue","adj",&target.mBlue,255,NULL);
    cvCreateTrackbar("BRange","adj",&target.mBlueRange,255,NULL);

    cvCreateTrackbar("Edge","adj",&edge,25,NULL);
    cvCreateTrackbar("MinArea","adj",&MinArea,1000,NULL);
    cvCreateTrackbar("ComboDist","adj",&ComboDist,320,NULL);

    cvCreateTrackbar("Camera","adj",&Camera,2,NULL);
    cvSetTrackbarPos("Camera","adj",0);

    cvSetTrackbarPos("Red","adj",127);
    cvSetTrackbarPos("Green","adj",127);
    cvSetTrackbarPos("Blue","adj",127);

    cvSetTrackbarPos("RRange","adj",10);
    cvSetTrackbarPos("BRange","adj",10);
    cvSetTrackbarPos("GRange","adj",10);

    cvSetTrackbarPos("Edge","adj",0);
    cvSetTrackbarPos("MinArea","adj",DEFAULTMINAREA);
    cvSetTrackbarPos("ComboDist","adj",DEFAULTCOMBODIST);

    cvSetMouseCallback("normal",Foo,this);
    cvWaitKey(100);
}


void Calibrate::Foo(int event, int x, int y, int flags, void* param)
{
    Calibrate *cal=(Calibrate *)param;
    if(event==CV_EVENT_LBUTTONDOWN)
    {
        cal->displayMutex.Lock();
        unsigned char *imageData=(unsigned char *)cal->displayImage->imageData;
        int pos=y*cal->displayImage->widthStep+3*x;

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
        if(endy>=cal->displayImage->height)
        {
            endy=cal->displayImage->height;
        }

        int startx=x-AVGWIN;
        int endx=x+AVGWIN;
        if(startx<0)
        {
            startx=0;
        }
        if(endx>=cal->displayImage->width)
        {
            endx=cal->displayImage->width;
        }

        for(int ty=starty;ty<endy;ty++)
        {
            for(int tx=startx;tx<endx;tx++)
            {
                size++;
                pos=ty*cal->displayImage->widthStep+3*tx;
                avgr+=(int)imageData[pos+2];
                avgg+=(int)imageData[pos+1];
                avgb+=(int)imageData[pos+0];
            }
        }
        avgr/=size;
        avgg/=size;
        avgb/=size;
        cal->displayMutex.Unlock();
        printf("%d %d - %d %d %d\n",x,y,avgb,avgg,avgr);

        //cvSetTrackbarPos("Red","adj",imageData[pos+2]);
        //cvSetTrackbarPos("Green","adj",imageData[pos+1]);
        //cvSetTrackbarPos("Blue","adj",imageData[pos+0]);
        cvSetTrackbarPos("Red","adj",avgr);
        cvSetTrackbarPos("Green","adj",avgg);
        cvSetTrackbarPos("Blue","adj",avgb);
    }
}