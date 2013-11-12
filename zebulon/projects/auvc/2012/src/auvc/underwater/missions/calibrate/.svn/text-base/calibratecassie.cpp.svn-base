#include "auvc/underwater/missions/calibrate/calibratecassie.h"

using namespace Zebulon;
using namespace Underwater;
using namespace std;

CalibrateCassie::CalibrateCassie()
{
    ID = "CalibrateCassie";
    
    SetupCV();
    
    displayImage=NULL;
    threshImage=NULL;
    segmentImage=NULL;
    displayCSImage = NULL;

    currThresh=NULL;

#ifdef THRESH
    hold=new Vision::Threshhold();
    currThresh = hold;
#endif
#ifdef GAUSSIAN
    gaussian = new Vision::ThreshGaussian();
    currThresh = gaussian;
#endif
    //fuzzy = new Vision::ThreshFuzzy();
    seg=new Vision::Segment();
    //default to ratio
    
    LoadFlag=1;
    autoFlag=0;
}

CalibrateCassie::~CalibrateCassie()
{

}

int CalibrateCassie::Init()
{
    mState = PutStatesHere;
    //currThresh = hold;
    Camera = 0;
    firstFlag = true;
    
    for(int i = 0; i < 8; i++)
    {
        vector<int> tempVector;
        for(int j = 0; j < 3; j++)
        {
            tempVector.push_back(0);
        }
        
        mixChannels.push_back(tempVector);
    }
    
    std::cout << "Init CalibrateCassie\n";
    CxUtils::SleepMs(1000);
    return 0;
}

int CalibrateCassie::ErrorCheck()
{
    return 0;
}

int CalibrateCassie::FetchProcess()
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
                    printf("ERROR::CalibrateCassie: Couldn't get Forward camera data");
                    displayMutex.Unlock();
                    return 1;
                }
                if(!mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera, displayCSImage))
                {
                    printf("ERROR::CalibrateCassie: Couldn't get Forward camera data");
                    displayMutex.Unlock();
                    return 1;
                }
                break;
            case 1:
                // Down
                if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, displayImage))
                {
                    printf("ERROR::CalibrateCassie: Couldn't get Downward camera data");
                    displayMutex.Unlock();
                    return 1;
                }
                if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, displayCSImage))
                {
                    printf("ERROR::CalibrateCassie: Couldn't get Downward camera data");
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
    if(firstFlag)
    {
        //Video::CvConvertToIplImage(*dest, &displayCSImage);
        //cvdisplayCSImage
        //cout << "HELLOOOOOOOOOOOOOO" << endl;
        colorSpace2.Setup(displayImage->width, displayImage->height, 3);
        firstFlag = false;
    }
    //colorSpace2.SetIndividualChannels(true, true, true);
    
    //colorSpace2.CIEXYZ(displayImage);
    
    /*mixChannels[colorSpace2.GRAY_CS][0] = 0;
    mixChannels[colorSpace2.GRAY_CS][1] = 0;
    mixChannels[colorSpace2.GRAY_CS][2] = 0;*/
    
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            
            //cout << "SET MIX: i - " << i << ", - j: " << j << endl;
            mixChannels[i][j] = tempCS[i][j];
        }
    }
    
    #ifdef GAUSSIAN
        target.mRedMean = tempRedMean/(PRECISION_AMOUNT*1.0);
        target.mRed = cvRound(target.mRedMean);
        target.mGreenMean = tempGreenMean/(PRECISION_AMOUNT*1.0);
        target.mGreen = cvRound(target.mGreenMean);
        target.mBlueMean = tempBlueMean/(PRECISION_AMOUNT*1.0);
        target.mBlue = cvRound(target.mBlueMean);
    
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

    colorSpace2.Mix(displayImage, mixChannels);
    
    colorSpace2.GetImage(displayCSImage, colorSpace2.CHANNEL_FULL);
    
    currThresh->Setup(target,edge);
    currThresh->Process(displayCSImage,threshImage);

    seg->Setup(target,MinArea,ComboDist);
    seg->Process(threshImage,segmentImage);

    seg->GetMaxPosition(ret);
    printf("Valid %d I %lf J %lf Area %d\n",ret.mValid,ret.mCenterI,ret.mCenterJ,ret.mArea);

    displayMutex.Unlock();
    return 0;
}

std::string CalibrateCassie::ExitEarly()
{
    return "KeepRunning";
}

std::string CalibrateCassie::Execute()
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
                //currThresh=ratio;
                break;
            case 104:
            case 1048680:
                currThresh=hold;
                break;
            // P
            //output XML
            case 1048688:
            case 112:
                std::cout << "<Color Red=\"" << target.mRed << "\" Green=\"" << target.mGreen << "\" Blue=\"" << target.mBlue << "\" />\n";
                std::cout << "<ColorThresh Red=\"" << target.mRedRange << "\" Green=\"" << target.mGreenRange << "\" Blue=\"" << target.mBlueRange << "\" />\n";
                cvWaitKey(5000);
                break;
        }
        printf("Key: %d\n",c);
    }
    Utility::Display::DisplayImage("normal", displayImage);
    Utility::Display::DisplayImage("Transformed", displayCSImage);
    Utility::Display::DisplayImage("post",segmentImage);
    return "KeepRunning";
}

void CalibrateCassie::SetupCV()
{
    cvNamedWindow("adj", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("ColorSpace1", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("ColorSpace2", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("ColorSpace3", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("normal", CV_WINDOW_AUTOSIZE);
    //cvNamedWindow("after", CV_WINDOW_AUTOSIZE);
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

    cvCreateTrackbar("Camera","adj",&Camera,1,NULL);
    cvSetTrackbarPos("Camera","adj",0);

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

    cvSetMouseCallback("Transformed",Foo,this); 
    cvWaitKey(100);
}


void CalibrateCassie::Foo(int event, int x, int y, int flags, void* param)
{
    CalibrateCassie *cal=(CalibrateCassie *)param;
    if(event==CV_EVENT_LBUTTONDOWN)
    {
        cal->displayMutex.Lock();
        unsigned char *imageData=(unsigned char *)cal->displayCSImage->imageData;
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
                pos=ty*cal->displayCSImage->widthStep+3*tx;
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
