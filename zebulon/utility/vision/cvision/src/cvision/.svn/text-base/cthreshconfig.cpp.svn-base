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

#include "cvision/threshgaussian.h"
#include "cvision/threshfuzzy.h"
#include "cvision/colorspace.h"

//#define SUBHACK
//#define GAUSSIAN
//#define FUZZY
#define THRESH

#ifdef GAUSSIAN
    #define PRECISION_AMOUNT 1000
#endif
#ifdef FUZZY
    #define PRECISION_AMOUNT 1000
#endif
#ifdef THRESH
    #define PRECISION_AMOUNT 1
#endif

#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 600

#ifdef SUBHACK

//Testing extreme hack for sub
#include <fw/fw1394Camera.h>

int RedBalance,BlueBalance;
int OldRedBalance,OldBlueBalance;
#endif

IplImage *displayImage=NULL;
IplImage *threshImage=NULL;
IplImage *segmentImage=NULL;

IplImage *displayCSImage=NULL;

Vision::Threshhold *currThresh=NULL;


//Vision::ThreshRatio *ratio=NULL;
#ifdef GAUSSIAN
Vision::ThreshGaussian *gaussian=NULL;
#endif
#ifdef FUZZY
Vision::ThreshFuzzy *fuzzy=NULL;
#endif
#ifdef THRESH
Vision::Threshhold *hold=NULL;
#endif

Vision::Segment *seg=NULL;

CxUtils::Mutex displayMutex;

Vision::Target target;

int edge;
int MinArea=DEFAULTMINAREA;
int ComboDist=DEFAULTCOMBODIST;

Vision::ColorSpace colorSpace2;

vector< vector<int> > mixChannels;

#define AVGWIN 3

void Foo(int event, int x, int y, int flags, void* param)
{
    //Vision::ColorSpace colorSpace;
//#endif

    /*colorSpace.Setup(640, 480, 3);
    colorSpace.SetIndividualChannels(true, true, true);
    
    colorSpace.HLS(displayImage);
    colorSpace.GetImage(displayImage, colorSpace.CHANNEL_FULL);*/
    
    if(event==CV_EVENT_LBUTTONDOWN)
    {
        displayMutex.Lock();
        unsigned char *imageData=(unsigned char *)displayCSImage->imageData;
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
                pos=ty*displayCSImage->widthStep+3*tx;
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
        
#ifdef GAUSSIAN
        int avgRed = avgr*PRECISION_AMOUNT;
        int avgGreen = avgg*PRECISION_AMOUNT;
        int avgBlue = avgb*PRECISION_AMOUNT;
        cvSetTrackbarPos("RMean","adj",avgRed);
        cvSetTrackbarPos("GMean","adj",avgGreen);
        cvSetTrackbarPos("BMean","adj",avgBlue);
#endif
#ifdef FUZZY
        int avgRed = avgr*PRECISION_AMOUNT;
        int avgGreen = avgg*PRECISION_AMOUNT;
        int avgBlue = avgb*PRECISION_AMOUNT;
        cvSetTrackbarPos("RMean","adj",avgRed);
        cvSetTrackbarPos("GMean","adj",avgGreen);
        cvSetTrackbarPos("BMean","adj",avgBlue);
#endif
#ifdef THRESH
        /*int red = avgr;
        int green = avgg;
        int blue = avgb;*/
        cvSetTrackbarPos("Red","adj",avgr);
        cvSetTrackbarPos("Green","adj",avgg);
        cvSetTrackbarPos("Blue","adj",avgb);
#endif
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
            if(displayCSImage==NULL)
            {
                Video::CvConvertToIplImage(*dest, &displayCSImage);
                colorSpace2.Setup(IMAGE_WIDTH, IMAGE_HEIGHT, 3);
            }
            //colorSpace2.SetIndividualChannels(true, true, true);
            
            //colorSpace2.CIEXYZ(displayImage);
            
            /*mixChannels[colorSpace2.GRAY_CS][0] = 0;
            mixChannels[colorSpace2.GRAY_CS][1] = 0;
            mixChannels[colorSpace2.GRAY_CS][2] = 0;*/

            colorSpace2.Mix(displayImage, mixChannels);
            
            colorSpace2.GetImage(displayCSImage, colorSpace2.CHANNEL_FULL);
            
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




#ifdef GAUSSIAN
int tempRedMean, tempGreenMean, tempBlueMean;
int tempRedVar, tempGreenVar, tempBlueVar;
#endif
#ifdef FUZZY
int tempRedMean, tempGreenMean, tempBlueMean;
int tempRedTrue, tempGreenTrue, tempBlueTrue;
int tempRedLF, tempGreenLF, tempBlueLF;
int tempRedRF, tempGreenRF, tempBlueRF;
#endif
#ifdef THRESH
int tempRed, tempGreen, tempBlue;
int tempRedRange, tempGreenRange, tempBlueRange;
#endif

/*int tempGrayCh1, tempGrayCh2, tempGrayCh3;
int tempHSVCh1, tempHSVCh2, tempHSVCh3;
int tempHLSCh1, tempHLSCh2, tempHLSCh3;
int tempYCrCbCh1, tempYCrCbCh2, tempYCrCbCh3;
int tempCIEXYZCh1, tempCIEXYZCh2, tempCIEXYZCh3;
int tempCIELuvCh1, tempCIELuvCh2, tempCIELuvCh3;
int tempCIELabCh1, tempCIELabCh2, tempCIELabCh3;
int tempRatioCh1, tempRatioCh2, tempRatioCh3;*/

/*int tempGray[3];
int tempHSV[3];
int tempHLS[3];
int tempYCrCb[3];
int tempCIEXYZ[3];
int tempCIELuv[3];
int tempCIELab[3];
int tempRatio[3];*/

int tempCS[8][3];

void SetupCV()
{
    cvNamedWindow("adj", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("ColorSpace1", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("ColorSpace2", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("ColorSpace3", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("normal", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("after", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Transformed", CV_WINDOW_AUTOSIZE);
    
    //int tempGray
    
    cvCreateTrackbar("Gray1", "ColorSpace1", &tempCS[0][0], 1, NULL);
    cvSetTrackbarPos("Gray1","ColorSpace1", 0);
    cvCreateTrackbar("Gray2", "ColorSpace1", &tempCS[0][1], 1, NULL);
    cvSetTrackbarPos("Gray2","ColorSpace1", 0);
    cvCreateTrackbar("Gray3", "ColorSpace1", &tempCS[0][2], 1, NULL);
    cvSetTrackbarPos("Gray3","ColorSpace1", 0);
    
    cvCreateTrackbar("HSV1", "ColorSpace2", &tempCS[1][0], 1, NULL);
    cvSetTrackbarPos("HSV1","ColorSpace2", 0);
    cvCreateTrackbar("HSV2", "ColorSpace2", &tempCS[1][1], 1, NULL);
    cvSetTrackbarPos("HSV2","ColorSpace2", 0);
    cvCreateTrackbar("HSV3", "ColorSpace2", &tempCS[1][2], 1, NULL);
    cvSetTrackbarPos("HSV3","ColorSpace2", 0);
    
    cvCreateTrackbar("HLS1", "ColorSpace2", &tempCS[2][0], 1, NULL);
    cvSetTrackbarPos("HLS1","ColorSpace2", 0);
    cvCreateTrackbar("HLS2", "ColorSpace2", &tempCS[2][1], 1, NULL);
    cvSetTrackbarPos("HLS2","ColorSpace2", 0);
    cvCreateTrackbar("HLS3", "ColorSpace2", &tempCS[2][2], 1, NULL);
    cvSetTrackbarPos("HLS3","ColorSpace2", 0);
    
    cvCreateTrackbar("YCrCb1", "ColorSpace1", &tempCS[3][0], 1, NULL);
    cvSetTrackbarPos("YCrCb1","ColorSpace1", 0);
    cvCreateTrackbar("YCrCb2", "ColorSpace1", &tempCS[3][1], 1, NULL);
    cvSetTrackbarPos("YCrCb2","ColorSpace1", 0);
    cvCreateTrackbar("YCrCb3", "ColorSpace1", &tempCS[3][2], 1, NULL);
    cvSetTrackbarPos("YCrCb3","ColorSpace1", 0);
    
    cvCreateTrackbar("CIEXYZ1", "ColorSpace3", &tempCS[4][0], 1, NULL);
    cvSetTrackbarPos("CIEXYZ1","ColorSpace3", 0);
    cvCreateTrackbar("CIEXYZ2", "ColorSpace3", &tempCS[4][1], 1, NULL);
    cvSetTrackbarPos("CIEXYZ2","ColorSpace3", 0);
    cvCreateTrackbar("CIEXYZ3", "ColorSpace3", &tempCS[4][2], 1, NULL);
    cvSetTrackbarPos("CIEXYZ3","ColorSpace3", 0);
    
    cvCreateTrackbar("CIELuv1", "ColorSpace3", &tempCS[5][0], 1, NULL);
    cvSetTrackbarPos("CIELuv1","ColorSpace3", 0);
    cvCreateTrackbar("CIELuv2", "ColorSpace3", &tempCS[5][1], 1, NULL);
    cvSetTrackbarPos("CIELuv2","ColorSpace3", 0);
    cvCreateTrackbar("CIELuv3", "ColorSpace3", &tempCS[5][2], 1, NULL);
    cvSetTrackbarPos("CIELuv3","ColorSpace3", 0);
    
    cvCreateTrackbar("CIELab1", "ColorSpace3", &tempCS[6][0], 1, NULL);
    cvSetTrackbarPos("CIELab1","ColorSpace3", 0);
    cvCreateTrackbar("CIELab2", "ColorSpace3", &tempCS[6][1], 1, NULL);
    cvSetTrackbarPos("CIELab2","ColorSpace3", 0);
    cvCreateTrackbar("CIELab3", "ColorSpace3", &tempCS[6][2], 1, NULL);
    cvSetTrackbarPos("CIELab3","ColorSpace3", 0);
    
    cvCreateTrackbar("Ratio1", "ColorSpace1", &tempCS[7][0], 1, NULL);
    cvSetTrackbarPos("Ratio1","ColorSpace1", 0);
    cvCreateTrackbar("Ratio2", "ColorSpace1", &tempCS[7][1], 1, NULL);
    cvSetTrackbarPos("Ratio2","ColorSpace1", 0);
    cvCreateTrackbar("Ratio3", "ColorSpace1", &tempCS[7][2], 1, NULL);
    cvSetTrackbarPos("Ratio3","ColorSpace1", 0);
    
    //cvCreateButton("GrayCH1",callbackButton,"GrayCH1",CV_CHECKBOX,0);
    
    int precision = 255*PRECISION_AMOUNT;
    
#ifdef GAUSSIAN
    cvCreateTrackbar("RMean","adj",&tempRedMean,precision,NULL);
    cvCreateTrackbar("RRange","adj",&tempRedVar,precision,NULL);
    
    cvCreateTrackbar("GMean","adj",&tempGreenMean,precision,NULL);
    cvCreateTrackbar("GRange","adj",&tempGreenVar,precision,NULL);
    
    cvCreateTrackbar("BMean","adj",&tempBlueMean,precision,NULL);
    cvCreateTrackbar("BRange","adj",&tempBlueVar,precision,NULL);
#endif

#ifdef FUZZY
    cvCreateTrackbar("RMean","adj",&tempRedMean,precision,NULL);
    cvCreateTrackbar("RTrue","adj",&tempRedTrue,precision,NULL);
    cvCreateTrackbar("RLeftF","adj",&tempRedLF,precision,NULL);
    cvCreateTrackbar("RRightF","adj",&tempRedRF,precision,NULL);
    
    cvCreateTrackbar("GMean","adj",&tempGreenMean,precision,NULL);
    cvCreateTrackbar("GTrue","adj",&tempGreenTrue,precision,NULL);
    cvCreateTrackbar("GLeftF","adj",&tempGreenLF,precision,NULL);
    cvCreateTrackbar("GRightF","adj",&tempGreenRF,precision,NULL);
    
    cvCreateTrackbar("BMean","adj",&tempBlueMean,precision,NULL);
    cvCreateTrackbar("BTrue","adj",&tempBlueTrue,precision,NULL);
    cvCreateTrackbar("BLeftF","adj",&tempBlueLF,precision,NULL);
    cvCreateTrackbar("BRightF","adj",&tempBlueRF,precision,NULL);
#endif

#ifdef THRESH
    cvCreateTrackbar("Red","adj",&tempRed,255,NULL);
    cvCreateTrackbar("RRange","adj",&tempRedRange,255,NULL);
    
    cvCreateTrackbar("Green","adj",&tempGreen,255,NULL);
    cvCreateTrackbar("GRange","adj",&tempGreenRange,255,NULL);
    
    cvCreateTrackbar("Blue","adj",&tempBlue,255,NULL);
    cvCreateTrackbar("BRange","adj",&tempBlueRange,255,NULL);
#endif
    

    cvCreateTrackbar("Edge","adj",&edge,25,NULL);
    cvCreateTrackbar("MinArea","adj",&MinArea,1000,NULL);
    cvCreateTrackbar("ComboDist","adj",&ComboDist,320,NULL);

#ifdef SUBHACK
    cvCreateTrackbar("RedBalance","adj",&RedBalance,255,NULL);
    cvCreateTrackbar("BlueBalance","adj",&BlueBalance,255,NULL);
    cvSetTrackbarPos("RedBalance","adj",109);
    cvSetTrackbarPos("BlueBalance","adj",71);
    OldRedBalance=RedBalance;
    OldBlueBalance=BlueBalance;
#endif

    /*//int startingColor = 127*PRECISION_AMOUNT;
    cvSetTrackbarPos("Red","adj",startingColor);
    cvSetTrackbarPos("Green","adj",startingColor);
    cvSetTrackbarPos("Blue","adj",startingColor);
    //int startingThresh = 127*PRECISION_AMOUNT;
    cvSetTrackbarPos("RRange","adj",10);
    cvSetTrackbarPos("BRange","adj",10);
    cvSetTrackbarPos("GRange","adj",10);*/

    cvSetTrackbarPos("Edge","adj",0);
    cvSetTrackbarPos("MinArea","adj",DEFAULTMINAREA);
    cvSetTrackbarPos("ComboDist","adj",DEFAULTCOMBODIST);

    cvSetMouseCallback("Transformed",Foo,NULL);  
}



//#define GAUSSIAN



int main(int argc, char *argv[])
{
    int pauseFlag=0;
    Video::CvImageCapture capture;
    Video::CvImageCapture::Image src;
    std::vector<std::string> sources;
    Vision::Return ret;
    char filename[1024];
    int imageCount=0;
    
    for(int i = 0; i < 8; i++)
    {
        vector<int> tempVector;
        for(int j = 0; j < 3; j++)
        {
            tempVector.push_back(0);
        }
        
        mixChannels.push_back(tempVector);
    }
    
    /*mixChannels[colorSpace2.YCrCb_CS][2] = 1;
    mixChannels[colorSpace2.YCrCb_CS][1] = 1;
    mixChannels[colorSpace2.CIELab_CS][2] = 1;*/

    SetupCV();
    
    
    //ratio=new Vision::ThreshRatio();
#ifdef GAUSSIAN
    gaussian=new Vision::ThreshGaussian();
#endif
#ifdef FUZZY
    fuzzy = new Vision::ThreshFuzzy();
#endif
#ifdef THRESH
    hold=new Vision::Threshhold();
#endif

    seg=new Vision::Segment();
    
    //default to ratio
    //currThresh=ratio;
#ifdef GAUSSIAN
    currThresh=gaussian;
#endif
#ifdef FUZZY
    currThresh=fuzzy;
#endif
#ifdef THRESH
    currThresh=hold;
#endif
    
    //Vision::ColorSpace colorSpace;
//#endif

    //colorSpace.Setup(IMAGE_WIDTH, IMAGE_HEIGHT, 3);
    //colorSpace.SetIndividualChannels(true, true, true);

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
    bool autoFlag=false;
    fw1394Camera *camera=(fw1394Camera *)capture.GetRawCapture();
#endif
    while(1)
    {
#ifdef GAUSSIAN
        target.mRedMean = tempRedMean/(PRECISION_AMOUNT*1.0);
        target.mGreenMean = tempGreenMean/(PRECISION_AMOUNT*1.0);
        target.mBlueMean = tempBlueMean/(PRECISION_AMOUNT*1.0);
    
        target.mRedVariance = tempRedVar/(PRECISION_AMOUNT*1.0);
        target.mGreenVariance = tempGreenVar/(PRECISION_AMOUNT*1.0);
        target.mBlueVariance = tempBlueVar/(PRECISION_AMOUNT*1.0);
#endif
#ifdef FUZZY
        target.mRedMean = tempRedMean/(PRECISION_AMOUNT*1.0);
        target.mGreenMean = tempGreenMean/(PRECISION_AMOUNT*1.0);
        target.mBlueMean = tempBlueMean/(PRECISION_AMOUNT*1.0);
    
        target.mRedTrue = tempRedTrue/(PRECISION_AMOUNT*1.0);
        target.mRedLeftFalse = tempRedLF/(PRECISION_AMOUNT*1.0);
        target.mRedRightFalse = tempRedRF/(PRECISION_AMOUNT*1.0);
        
        target.mGreenTrue = tempGreenTrue/(PRECISION_AMOUNT*1.0);
        target.mGreenLeftFalse = tempGreenLF/(PRECISION_AMOUNT*1.0);
        target.mGreenRightFalse = tempGreenRF/(PRECISION_AMOUNT*1.0);
        
        target.mBlueTrue = tempBlueTrue/(PRECISION_AMOUNT*1.0);
        target.mBlueLeftFalse = tempBlueLF/(PRECISION_AMOUNT*1.0);
        target.mBlueRightFalse = tempBlueRF/(PRECISION_AMOUNT*1.0);
#endif
#ifdef THRESH
        target.mRed = tempRed;
        target.mGreen = tempGreen;
        target.mBlue = tempBlue;
        
        target.mRedRange = tempRedRange;
        target.mGreenRange = tempGreenRange;
        target.mBlueRange = tempBlueRange;
        
        /*cout << "target.mRedMean: " << target.mRedMean << endl;
        cout << "target.mRedTrue: " << target.mRedTrue << endl;
        cout << "target.mRedLeftFalse: " << target.mRedLeftFalse << endl;
        cout << "target.mRedRightFalse: " << target.mRedRightFalse << endl;
        cout << "target.mRedMean: " << target.mRedMean << endl;
        cout << "target.mRedMean: " << target.mRedMean << endl;
        cout << "target.mRedMean: " << target.mRedMean << endl;
        cout << "target.mRedMean: " << target.mRedMean << endl;*/
#endif

        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                
                //cout << "SET MIX: i - " << i << ", - j: " << j << endl;
                mixChannels[i][j] = tempCS[i][j];
            }
        }
        //mixChannels[0][0] = ;
        
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
        cvShowImage("normal",displayImage);
#ifdef SAVE
        sprintf(filename,"/home/cassie/monoslam/home_test/image-%06d.png",imageCount);
        cvSaveImage(filename,displayImage);
        imageCount++;
#endif
        //displayCSImage = cvCloneImage(displayImage);
        ////colorSpace.Setup(IMAGE_WIDTH, IMAGE_HEIGHT, 3);
        ////colorSpace.SetIndividualChannels(true, true, true);
        //colorSpace.HLS(displayCSImage);
        //colorSpace.GetImage(displayCSImage, colorSpace.CHANNEL_FULL);
        //threshImage
        
        //cvShowImage("Transformed",displayCSImage);

        currThresh->Setup(target,edge);
        currThresh->Process(displayCSImage, threshImage);
        
        //cvShowImage("inbetween",threshImage);
        
        //seg->Setup(target.mRed,target.mGreen,target.mBlue,MinArea,ComboDist);
        //seg->Setup(target.mRedMean,target.mGreenMean,target.mBlueMean,MinArea,ComboDist);
        seg->Setup(target, MinArea, ComboDist);
        seg->Process(threshImage,segmentImage);
        
        seg->GetMaxPosition(ret);
        
        printf("Valid %d I %lf J %lf Area %d\n",ret.mValid,ret.mCenterI,ret.mCenterJ,ret.mArea);
        
        //cvShowImage("after",threshImage);
        cvShowImage("Transformed",displayCSImage);
        cvShowImage("after",segmentImage);
        
        //cvReleaseImage(&displayCSImage);


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
#endif

        int c=cvWaitKey(50);

        
        if(c>0)
        {
            switch(c)
            {
                case 122:
                case 1048698:
                    mycallback::mLoadflag=0;
                    break;
                case 120:
                case 1048696:
                    mycallback::mLoadflag=1;
                    capture.Resume();
                    pauseFlag=0;
                    break;
                case 114:
                case 1048690:
                    //currThresh=ratio;
                #ifdef GAUSSIAN
                    currThresh=gaussian;
                #endif
                #ifdef FUZZY
                    currThresh=fuzzy;
                #endif
                #ifdef THRESH
                    currThresh=hold;
                #endif
                    break;
                case 104:
                case 1048680:
                #ifdef THRESH
                    currThresh=hold;
                #endif
                    break;
                //output XML
                case 1048688:
                case 112:
                    std::cout << "<Color Red=\"" << target.mRed << "\" Green=\"" << target.mGreen << "\" Blue=\"" << target.mBlue << "\" />\n";
                    std::cout << "<ColorThresh Red=\"" << target.mRedRange << "\" Green=\"" << target.mGreenRange << "\" Blue=\"" << target.mBlueRange << "\" />\n";
                    cvWaitKey(5000);
                    break;
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
                //find non-unicode
                case 1048674:
                    autoFlag=!autoFlag;
                    camera->enableAutoWhite(autoFlag);
                    if(autoFlag==false)
                    {
                        camera->GetWhiteBalance(RedBalance,BlueBalance);
                        std::cout << "<WhiteBalance Red=\"" << RedBalance << "\" Blue=\"" << BlueBalance << "\" />\n";
                        cvWaitKey(5000);
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
