#include "cvision/colorspace.h"

using namespace Zebulon;
using namespace Vision;
using namespace std;

ColorSpace::ColorSpace()
{
    
}

ColorSpace::~ColorSpace()
{
    
}

void ColorSpace::Setup(int sizeX, int sizeY, int channelNum)
{
    mImageVector.clear();
    mChannels = channelNum;
    
    for(int i = 0; i < 4; i++)
    {
        if(i == 0 && channelNum == 3)
        {
            IplImage *img = NULL;
            img = cvCreateImage(cvSize(sizeX, sizeY), IPL_DEPTH_8U, channelNum);
            mImageVector.push_back(img);
        }
        else
        {
            IplImage *img = NULL;
            img = cvCreateImage(cvSize(sizeX, sizeY), IPL_DEPTH_8U, 1);
            mImageVector.push_back(img);
        }
    }
}

void ColorSpace::SetIndividualChannels(bool getChannelOne, bool getChannelTwo, bool getChannelThree, bool invertOne, bool invertTwo, bool invertThree, bool invertFull)
{
    /*memset(mImageVector[0]->imageData,0,mImageVector[0]->imageSize);
    memset(mImageVector[1]->imageData,0,mImageVector[1]->imageSize);
    memset(mImageVector[2]->imageData,0,mImageVector[2]->imageSize);
    memset(mImageVector[3]->imageData,0,mImageVector[3]->imageSize);*/
    
    /*cvZero(mImageVector[0]);
    cvZero(mImageVector[1]);
    cvZero(mImageVector[2]);
    cvZero(mImageVector[3]);*/
    
    mChannelOne = getChannelOne;
    mChannelTwo = getChannelTwo;
    mChannelThree = getChannelThree;
    
    mInvertChannelOne = invertOne;
    mInvertChannelTwo = invertTwo;
    mInvertChannelThree = invertThree;
    mInvertFull = invertFull;
    
    //cout << "CH1: " << mChannelOne << ", CH2: " << mChannelTwo << ", CH3: " << mChannelThree << endl;
}

void ColorSpace::GetImage(IplImage *dest, int imageInfo)
{
    if(dest == NULL)
    {
        cout << "Image passed is NULL" << endl;
        return;
    }
    
    switch(imageInfo)
    {
        case CHANNEL_FULL:
            cvCopyImage(mImageVector[0], dest);
            break;
        case CHANNEL_ONE:
            cvCopyImage(mImageVector[1], dest);
            break;
        case CHANNEL_TWO:
            cvCopyImage(mImageVector[2], dest);
            break;
        case CHANNEL_THREE:
            cvCopyImage(mImageVector[3], dest);
            break;
        case MIX_CHANNEL_ONE:
            cvCopyImage(mMixedImageVector[0], dest);
            break;
        case MIX_CHANNEL_TWO:
            cvCopyImage(mMixedImageVector[1], dest);
            break;
        case MIX_CHANNEL_THREE:
            cvCopyImage(mMixedImageVector[2], dest);
            break;
    }
}

void ColorSpace::Gray(IplImage *src)
{
    if(mChannelOne == true || mChannelTwo == true || mChannelThree == true)
    {
        int pos = 0;
        int posGray = 0;
        for(int j=0;j<src->height;j++)
        {
            for(int i=0;i<src->width;i++)
            {
                pos=j*src->widthStep+3*i;
                
                // pos+0 - blue, pos+1 - green, pos+2 - red
                if(mChannelOne)
                {
                    mImageVector[1]->imageData[posGray] = src->imageData[pos+0];
                }
                if(mChannelTwo)
                {
                    mImageVector[2]->imageData[posGray] = src->imageData[pos+1];
                }
                if(mChannelThree)
                {
                    mImageVector[3]->imageData[posGray] = src->imageData[pos+2];
                }
                posGray++;
            }
        }
    }
    
    //cvCvtColor(src, mImageVector[0], CV_BGR2GRAY);
}

void ColorSpace::HSV(IplImage *src)
{
    cvCvtColor(src, mImageVector[0], CV_BGR2HSV);
    
    IndividualChannels();
}

void ColorSpace::HLS(IplImage *src)
{
    cvCvtColor(src, mImageVector[0], CV_BGR2HLS);
    
    IndividualChannels();
}

void ColorSpace::YCrCb(IplImage *src)
{
    cvCvtColor(src, mImageVector[0], CV_BGR2YCrCb);
    
    IndividualChannels();
}

void ColorSpace::CIEXYZ(IplImage *src)
{
    cvCvtColor(src, mImageVector[0], CV_BGR2XYZ);
    
    IndividualChannels();
}

void ColorSpace::CIELuv(IplImage *src)
{
    cvCvtColor(src, mImageVector[0], CV_BGR2Luv);
    
    IndividualChannels();
}

void ColorSpace::CIELab(IplImage *src)
{
    cvCvtColor(src, mImageVector[0], CV_BGR2Lab);
    
    IndividualChannels();
}

void ColorSpace::Ratio(IplImage *src)
{
    unsigned char *imageData=(unsigned char *)src->imageData;
    int pos;
    int gray;

    for(int j=0;j<src->height;j++)
    {
        for(int i=0;i<src->width;i++)
        {
            pos=j*src->widthStep+3*i;
            
            //calculate the gray value
            gray=((int)imageData[pos+2]+(int)imageData[pos+1]+(int)imageData[pos+0])/3;
            
            //offset of color from gray, to cancel lighting conditions
            //value between 0 and 254 still
            mImageVector[0]->imageData[pos+2] = ((int)imageData[pos+2]-gray)/2+127;
            mImageVector[0]->imageData[pos+1] = ((int)imageData[pos+1]-gray)/2+127;
            mImageVector[0]->imageData[pos+0] = ((int)imageData[pos+0]-gray)/2+127;
        }
    }
    
    IndividualChannels();
}

void ColorSpace::SetIndividualChannelHisto(bool getChannelFull, bool getChannelOne, bool getChannelTwo, bool getChannelThree)
{
    mChannelHistoFull = getChannelFull;
    mChannelHistoOne = getChannelOne;
    mChannelHistoTwo = getChannelTwo;
    mChannelHistoThree = getChannelThree;
}

void ColorSpace::HistogramEqualization(IplImage *src)
{
    cvEqualizeHist(src, src);
}

void ColorSpace::HistogramEqualization()
{
    if(mChannelHistoOne)
    {
        cvEqualizeHist(mImageVector[1], mImageVector[1]);
    }
    if(mChannelHistoTwo)
    {
        cvEqualizeHist(mImageVector[2], mImageVector[2]);
    }
    if(mChannelHistoThree)
    {
        cvEqualizeHist(mImageVector[3], mImageVector[3]);
    }
    
    if(mChannelHistoFull == true && mChannels == 3)
    {
        // Recombine
        int pos = 0;
        int posSingle = 0;
        for(int j = 0; j < mImageVector[0]->height; j++)
        {
            for(int i = 0; i < mImageVector[0]->width; i++)
            {
                // Calculate position
                pos = j*mImageVector[0]->widthStep+3*i;
                
                mImageVector[0]->imageData[pos+2] = mImageVector[3]->imageData[posSingle];
                mImageVector[0]->imageData[pos+1] = mImageVector[2]->imageData[posSingle];
                mImageVector[0]->imageData[pos+0] = mImageVector[1]->imageData[posSingle];
                posSingle++;
            }
        }
    }
    else if(mChannelHistoFull == true && mChannels == 1)
    {
        // Recombine
        int pos = 0;
        int posSingle = 0;
        int tempPixelVal = 0;
        for(int j = 0; j < mImageVector[0]->height; j++)
        {
            for(int i = 0; i < mImageVector[0]->width; i++)
            {
                // Calculate position
                //pos = j*mImageVector[0]->widthStep+3*i;
                
                tempPixelVal = (mImageVector[3]->imageData[posSingle] + mImageVector[2]->imageData[posSingle] + mImageVector[1]->imageData[posSingle])/3;
                
                mImageVector[0]->imageData[posSingle] = tempPixelVal;
                posSingle++;
            }
        }
    }
}

void ColorSpace::Invert(IplImage *src, int channelNum)
{
    unsigned char *imageData=(unsigned char *)src->imageData;
    
    if(channelNum == 1)
    {
        // Recombine
        //int pos = 0;
        int posSingle = 0;
        int tempPixelVal = 0;
        for(int j = 0; j < src->height; j++)
        {
            for(int i = 0; i < src->width; i++)
            {
                // Calculate position
                //pos = j*mImageVector[0]->widthStep+3*i;
                
                src->imageData[posSingle] = 255 - src->imageData[posSingle];
                //tempPixelVal = (mImageVector[3]->imageData[posSingle] + mImageVector[2]->imageData[posSingle] + mImageVector[1]->imageData[posSingle])/3;
                
                //mImageVector[0]->imageData[posSingle] = tempPixelVal;
                posSingle++;
            }
        }
    }
    else if(channelNum == 3)
    {
        // Recombine
        int pos = 0;
        int posSingle = 0;
        int tempPixelVal = 0;
        for(int j = 0; j < src->height; j++)
        {
            for(int i = 0; i < src->width; i++)
            {
                // Calculate position
                pos = j*src->widthStep+3*i;
                
                src->imageData[pos+2] = 255 - imageData[pos+2];
                src->imageData[pos+1] = 255 - imageData[pos+1];
                src->imageData[pos+0] = 255 - imageData[pos+0];
                
                //tempPixelVal = (mImageVector[3]->imageData[posSingle] + mImageVector[2]->imageData[posSingle] + mImageVector[1]->imageData[posSingle])/3;
                
                //mImageVector[0]->imageData[posSingle] = tempPixelVal;
                //posSingle++;
            }
        }
    }
}

void ColorSpace::IndividualChannels()
{
    if(mChannelOne == true || mChannelTwo == true || mChannelThree == true)
    {
        int pos = 0;
        int posSingle = 0;
        
        for(int j=0;j<mImageVector[0]->height;j++)
        {
            for(int i=0;i<mImageVector[0]->width;i++)
            {
                pos=j*mImageVector[0]->widthStep+3*i;
                
                // pos+0 - blue, pos+1 - green, pos+2 - red
                if(mChannelOne)
                {
                    //cout << "IndividualChannel: One" << endl;
                    if(mInvertChannelOne)
                    {
                        //cout << "Inverting" << endl;
                        mImageVector[1]->imageData[posSingle] = 255 - mImageVector[0]->imageData[pos+0];
                        
                        if(mInvertFull)
                        {
                            mImageVector[0]->imageData[pos+0] = 255 - mImageVector[0]->imageData[pos+0];
                        }
                    }
                    else
                    {
                        //cout << "Non-inverting" << endl;
                        mImageVector[1]->imageData[posSingle] = mImageVector[0]->imageData[pos+0];
                    }
                }
                if(mChannelTwo)
                {
                    //cout << "IndividualChannel: Two" << endl;
                    if(mInvertChannelTwo)
                    {
                        //cout << "Inverting" << endl;
                        mImageVector[2]->imageData[posSingle] = 255 - mImageVector[0]->imageData[pos+1];
                        
                        if(mInvertFull)
                        {
                            mImageVector[0]->imageData[pos+1] = 255 - mImageVector[0]->imageData[pos+1];
                        }
                    }
                    else
                    {
                        //cout << "Non-Inverting" << endl;
                        mImageVector[2]->imageData[posSingle] = mImageVector[0]->imageData[pos+1];
                    }
                }
                if(mChannelThree)
                {
                    //cout << "IndividualChannel: Two" << endl;
                    if(mInvertChannelThree)
                    {
                        //cout << "Inverting" << endl;
                        mImageVector[3]->imageData[posSingle] = 255 - mImageVector[0]->imageData[pos+2];
                        
                        if(mInvertFull)
                        {
                            mImageVector[0]->imageData[pos+2] = 255 - mImageVector[0]->imageData[pos+2];
                        }
                    }
                    else
                    {
                        //cout << "Non-Inverting" << endl;
                        mImageVector[3]->imageData[posSingle] = mImageVector[0]->imageData[pos+2];
                    }
                }
                posSingle++;
            }
        }
    }
}

void ColorSpace::Combine(std::vector<IplImage*> src, IplImage *dst)
{
    int pos = 0;
    int posSingle = 0;
    int tempPixelVal = 0;
    for(int j = 0; j < dst->height; j++)
    {
        for(int i = 0; i < dst->width; i++)
        {
            // Calculate position
            //pos = j*mImageVector[0]->widthStep+3*i;
            
            //for(vector<IplImage*>::iterator k = src.begin(); k != src.end(); k++)
            for(int k = 0; k < src.size(); k++)
            {
                tempPixelVal = src[k]->imageData[posSingle];
            }
            tempPixelVal /= src.size();
            
            dst->imageData[posSingle] = tempPixelVal;
            
            posSingle++;
        }
    }
}

void ColorSpace::SetMix(int channels)
{
    mMixedImageVector.clear();
    //mChannels = channels;
    
    for(int i = 0; i < channels; i++)
    {
        /*if(i == 0 && channelNum == 3)
        {
            IplImage *img = NULL;
            img = cvCreateImage(cvSize(sizeX, sizeY), IPL_DEPTH_8U, channelNum);
            mImageVector.push_back(img);
        }
        else*/
        {
            IplImage *img = NULL;
            img = cvCreateImage(cvSize(mImageVector[0]->width, mImageVector[0]->height), IPL_DEPTH_8U, 1);
            mMixedImageVector.push_back(img);
        }
    }
}

//void ColorSpace::SetMix(IplImage* src, int gray, int hsv, int hls, int ycrcb, int xyz, int luv, int lab, int ratio)
void ColorSpace::Mix(IplImage *src, std::vector< std::vector<int> > mixType)
{
    for(int i = 0; i < mMixedImageVector.size(); i++)
    {
        //cout << "IM RELEASING MEMORY" <<endl;
        cvReleaseImage(&mMixedImageVector[i]);
    }
    mMixedImageVector.clear();
    int channelCount = 0;
    
    for(int i = 0; i < mixType.size(); i++)
    {
        // switch on mixtype
        //int mixCS = 0;
        switch(i)
        {
            case HSV_CS:
                //cout << "MIX: HSV" << endl;
                if(mixType[i][0] || mixType[i][1] || mixType[i][2])
                {
                    SetIndividualChannels(mixType[i][0], mixType[i][1], mixType[i][2]);
                    HSV(src);
                }
                break;
            case HLS_CS:
                //cout << "MIX: HLS" << endl;
                if(mixType[i][0] || mixType[i][1] || mixType[i][2])
                {
                    SetIndividualChannels(mixType[i][0], mixType[i][1], mixType[i][2]);
                    HLS(src);
                }
                break;
            case YCrCb_CS:
                //cout << "MIX: YCrCb" << endl;
                if(mixType[i][0] || mixType[i][1] || mixType[i][2])
                {
                    SetIndividualChannels(mixType[i][0], mixType[i][1], mixType[i][2]);
                    YCrCb(src);
                }
                break;
            case CIEXYZ_CS:
                //cout << "MIX: CIEXYZ" << endl;
                if(mixType[i][0] || mixType[i][1] || mixType[i][2])
                {
                    SetIndividualChannels(mixType[i][0], mixType[i][1], mixType[i][2]);
                    CIEXYZ(src);
                }
                break;
            case CIELuv_CS:
                //cout << "MIX: CIELuv" << endl;
                if(mixType[i][0] || mixType[i][1] || mixType[i][2])
                {
                    SetIndividualChannels(mixType[i][0], mixType[i][1], mixType[i][2]);
                    CIELuv(src);
                }
                break;
            case CIELab_CS:
                //cout << "MIX: CIELab" << endl;
                if(mixType[i][0] || mixType[i][1] || mixType[i][2])
                {
                    SetIndividualChannels(mixType[i][0], mixType[i][1], mixType[i][2]);
                    CIELab(src);
                }
                break;
            case Ratio_CS:
                //cout << "MIX: Ratio" << endl;
                if(mixType[i][0] || mixType[i][1] || mixType[i][2])
                {
                    SetIndividualChannels(mixType[i][0], mixType[i][1], mixType[i][2]);
                    Ratio(src);
                }
                break;
            case GRAY_CS:
                //cout << "MIX: Gray" << endl;
                if(mixType[i][0] || mixType[i][1] || mixType[i][2])
                {
                    SetIndividualChannels(mixType[i][0], mixType[i][1], mixType[i][2]);
                    Gray(src);
                }
                break;
        }
        
        IplImage *tempImg = NULL;
        //cv
        //tempImg = cvCreateImage(cvSize(mImageVector[0]->width,mImageVector[0]->height),IPL_DEPTH_8U,1);
        
        for(int k = 0; k < 3; k++)
        {
            if(mixType[i][k])
            {
                //cout << "K: " << k+1 << endl;
                // have to do cvclone
                //cvEqualizeHist(mImageVector[k+1], mImageVector[k+1]);
                tempImg = cvCloneImage(mImageVector[k+1]);
                //cvEqualizeHist(tempImg, tempImg);
                //cvShowImage("TempSave", tempImg);
                //cvWaitKey(1000);
                // then free
                mMixedImageVector.push_back(tempImg);
                
                //cvReleaseImage(&tempImg);
                channelCount++;
                //cout << "Size: " << mMixedImageVector.size() << endl;
            }
        }
    }
    
    memset(mImageVector[0]->imageData,0,mImageVector[0]->imageSize);
    memset(mImageVector[1]->imageData,0,mImageVector[1]->imageSize);
    memset(mImageVector[2]->imageData,0,mImageVector[2]->imageSize);
    memset(mImageVector[3]->imageData,0,mImageVector[3]->imageSize);
    
    int pos = 0;
    int posSingle = 0;
    int tempPixelVal = 0;
    for(int j = 0; j < src->height; j++)
    {
        for(int i = 0; i < src->width; i++)
        {
            // Calculate position
            pos = j*src->widthStep+3*i;
            
            if(mMixedImageVector.size() >= 3)
            {
                mImageVector[0]->imageData[pos+2] = mMixedImageVector[2]->imageData[posSingle];
                mImageVector[0]->imageData[pos+1] = mMixedImageVector[1]->imageData[posSingle];
                mImageVector[0]->imageData[pos+0] = mMixedImageVector[0]->imageData[posSingle];
            }
            
            //tempPixelVal = (mImageVector[3]->imageData[posSingle] + mImageVector[2]->imageData[posSingle] + mImageVector[1]->imageData[posSingle])/3;
            
            //mImageVector[0]->imageData[posSingle] = tempPixelVal;
            posSingle++;
        }
    }
    
    //cvShowImage("TempSaveFull", mImageVector[0]);
    //            cvWaitKey(1000);
    /*switch(mixType)
    {
        case mMixOne:
            switch(hsv)
            {
                case ONE:
                    SetIndividualChannels(true, false, false);
                    HSV(src);
                    break;
                case TWO:
                    break;
                case THREE:
                    break;
                case ONE_TWO:
                    break;
                case ONE_THREE:
                    break;
                case TWO_THREE:
                    break;
            }
    }*/
}

/*void ColorSpace::Mix(IplImage *src)
{
    
}*/

/*void ColorSpace::Combine(IplImage *dst, IplImage *src1, IplImage *src2, IplImage *src3, IplImage *src4, IplImage *src5, IplImage *src6, IplImage *src7)
{
    // Recombine
        int pos = 0;
        int posSingle = 0;
        int tempPixelVal = 0;
        for(int j = 0; j < dst->height; j++)
        {
            for(int i = 0; i < dst->width; i++)
            {
                // Calculate position
                //pos = j*mImageVector[0]->widthStep+3*i;
                
                for(int k = 0; k < 7; k++)
                {
                    if()
                }
                tempPixelVal = (src1->imageData[posSingle] + src2->imageData[posSingle] + src3->imageData[posSingle])/3;
                
                mImageVector[0]->imageData[posSingle] = tempPixelVal;
                posSingle++;
            }
        }
}*/



