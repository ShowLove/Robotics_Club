
#include "auvc/underwater/vision/bouybaseobject.h"

BouyBaseObject::BouyBaseObject()
{
    mNearColor= CV_RGB(0,0,0);
    mFarColor = CV_RGB(0,0,0);
    mNearImage = NULL;
    mFarImage = NULL;
    mNearHist = NULL;
    mFarHist = NULL;
    mTemplate = NULL;
    mMainThreshold = 0;
    mMinNoiseSizePercent = 0;
    mTemplateThreshold = 0;
    mDebug = false;
    mHistWeight = 1;
    mColorWeight = 1;
    mSegmentWeight = 1;
    mGVColorWeight = 1;
    mEnableHist = true;
    mEnableColor = true;
    mEnableSegment = true;
    mEnableGVColor = true;
    mSeg = new Vision::Segment;
    mThresh = new Vision::Threshhold;
    mRatio = new Vision::ThreshRatio;
}

BouyBaseObject::~BouyBaseObject()
{
    Clear();
}

void BouyBaseObject::Clear()
{
    mNearColor= CV_RGB(0,0,0);
    mFarColor = CV_RGB(0,0,0);
    mMainThreshold = 0;
    mMinNoiseSizePercent = 0;
    mTemplateThreshold = 0;
    mDebug = false;
    cvReleaseImage(&mNearImage);
    cvReleaseImage(&mFarImage);
    cvReleaseImage(&mTemplate);
    cvReleaseHist(&mNearHist);
    cvReleaseHist(&mFarHist);
}


bool BouyBaseObject::Initialize(const std::string& parameter)
{
    mType = parameter;
    if(!mXml.Load("settings/vision/visionobjects.xml","VisionObjects"))
    {
        return false;
    }
    Clear();
    std::string file;
    mXml.GetVar(parameter+"@debug",mDebug);
    if(mXml.GetVar(parameter+".Images.Near@file",file))
    {
        mNearImage = cvLoadImage(file.c_str());
        if(mNearImage)
        {
            mNearColor = cvAvg(mNearImage);
            mNearHist = VisionUtils::CreateHSVHist(mNearImage);
        }
    }
    if(mXml.GetVar(parameter+".Images.Far@file",file))
    {
        mFarImage = cvLoadImage(file.c_str());
        if(mFarImage)
        {
            //cvCvtColor(temp, convert, CV_BGR2YCrCb);
            mFarColor = cvAvg(mFarImage);
            mFarHist = VisionUtils::CreateHSVHist(mFarImage);
        }
    }
    if(mXml.GetVar(parameter+".Images.Template@file",file))
    {
        IplImage * temp = cvLoadImage(file.c_str());
        if(temp)
        {
            mTemplate = cvCreateImage( cvGetSize(temp), 8, 1 );
            cvConvertImage(temp,mTemplate);
            //cvThreshold(mBouyTemplate,mTemplate,200,255,CV_THRESH_BINARY);
            cvReleaseImage(&temp);
        }
    }
    bool enabled = false;
    if(mXml.GetVar(parameter+".Colors@enable",enabled))
    {
        if(enabled)
        {
            mXml.GetVar(parameter+".Colors.Near@r",mNearColor.val[0]);
            mXml.GetVar(parameter+".Colors.Near@g",mNearColor.val[1]);
            mXml.GetVar(parameter+".Colors.Near@b",mNearColor.val[2]);
            mXml.GetVar(parameter+".Colors.Far@r",mFarColor.val[0]);
            mXml.GetVar(parameter+".Colors.Far@g",mFarColor.val[1]);
            mXml.GetVar(parameter+".Colors.Far@b",mFarColor.val[2]);
        }
    }
    double val = 0;
    if(mXml.GetVar(parameter+".Options@mainthreshold",val))
    {
        mMainThreshold = val;
    }
    if(mXml.GetVar(parameter+".Options@templatethreshold",val))
    {
        mTemplateThreshold = val;
    }
    if(mXml.GetVar(parameter+".Options@minnoisesizepercent",val))
    {
        mMinNoiseSizePercent = val;
    }


    if(mXml.GetVar(parameter+".Options.HistMask@enable",enabled))
    {
        mEnableHist = enabled;
    }
    if(mXml.GetVar(parameter+".Options.ColorMask@enable",enabled))
    {
        mEnableColor = enabled;
    }
    if(mXml.GetVar(parameter+".Options.SegmentMask@enable",enabled))
    {
        mEnableSegment = enabled;
    }
    if(mXml.GetVar(parameter+".Options.GVColorMask@enable",enabled))
    {
        mEnableGVColor = enabled;
    }

    if(mXml.GetVar(parameter+".Options.HistMask@weight",val))
    {
        mHistWeight = val;
    }
    if(mXml.GetVar(parameter+".Options.ColorMask@weight",val))
    {
        mColorWeight = val;
    }
    if(mXml.GetVar(parameter+".Options.SegmentMask@weight",val))
    {
        mSegmentWeight = val;
    }
    if(mXml.GetVar(parameter+".Options.GVColorMask@weight",val))
    {
        mGVColorWeight= val;
    }
    if(mNearImage  == NULL || mFarImage == NULL || mTemplate == NULL) return false;
    return true;
}

IplImage* BouyBaseObject::GVColorMask(const IplImage * imgIn) const
{
    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * temp = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);

    IplImage * thresh = NULL;
    mThresh->Setup(mNearColor.val[2],mNearColor.val[1],mNearColor.val[0],50,50,50);
    mThresh->Process(imgIn,thresh);
    cvConvertImage(thresh, imgOut);

//    mThresh->Setup(mFarColor.val[2],mFarColor.val[1],mFarColor.val[1],50,50,50);
//    mThresh->Process(imgIn,thresh);
//    cvConvertImage(thresh, temp);

//    VisionUtils::CombineMasks(imgOut,temp,imgOut);
    cvReleaseImage(&temp);
    return imgOut;
//    seg->Setup(target.mRed,target.mGreen,target.mBlue,MinArea,ComboDist);
//    seg->Process(threshImage,segmentImage);
//    seg.
}
IplImage* BouyBaseObject::ColorMask(const IplImage * imgIn) const
{
    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * temp = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
   // IplImage * convert = cvCloneImage(imgIn);
   // cvCvtColor(imgIn, convert, CV_BGR2YCrCb);
    VisionUtils::ExtractColor(imgIn,
                           imgOut,
                           mNearColor,
                           0,
                           1);
//    VisionUtils::ExtractColor(imgIn,
//                           temp,
//                           mFarColor,
//                           0,
//                           1);
//    VisionUtils::CombineMasks(imgOut,temp,imgOut);
    cvReleaseImage(&temp);
    return imgOut;

}
IplImage* BouyBaseObject::EdgeMask(const IplImage * imgIn) const
{

    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * gray =  cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    cvConvertImage(imgIn,gray);
    CvSize imageSize = cvSize(imgIn->width & -2, imgIn->height & -2 );
    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 1 );
    cvPyrDown( imgOut, imgSmallCopy);
    cvPyrUp( imgSmallCopy, imgOut);
    cvSmooth(imgOut, imgOut, CV_GAUSSIAN, 5);
    cvReleaseImage(&imgSmallCopy);

    cvCanny(gray,imgOut,100,100);
    cvDilate(imgOut,imgOut, NULL, 1);
    //cvShowImage("canny", imgOut);
    return imgOut;
}

IplImage* BouyBaseObject::HistogramMask(const IplImage * imgIn) const
{
    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * temp = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    //IplImage * norm = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_32F, 1);
    IplImage* hsv = cvCreateImage( cvGetSize(imgIn), 8, 3 );
    cvCvtColor( imgIn, hsv, CV_BGR2HSV );
    IplImage* h_plane = cvCreateImage( cvGetSize(imgIn), 8, 1 );
    IplImage* s_plane = cvCreateImage( cvGetSize(imgIn), 8, 1 );
    IplImage* v_plane = cvCreateImage( cvGetSize(imgIn), 8, 1 );
    IplImage* planes[] = { h_plane, s_plane };
    cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );

    cvCalcBackProject(planes,imgOut,mNearHist);
    //cvCalcBackProject(planes,temp,mFarHist);
    //VisionUtils::CombineMasks(imgOut,temp,imgOut);
    //cvNormalize(norm,imgOut,255,0,CV_MINMAX);
    cvSmooth(imgOut,imgOut,2);
    //cvShowImage("histo", imgOut);
    cvReleaseImage(&h_plane);
    cvReleaseImage(&s_plane);
    cvReleaseImage(&v_plane);
    cvReleaseImage(&hsv);
    cvReleaseImage(&temp);

    return imgOut;
}
IplImage* BouyBaseObject::SegmentationMask(const IplImage * imgIn) const
{
    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * src = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage* hsv = cvCreateImage( cvGetSize(imgIn), 8, 3 );

    IplImage * chan0 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan1 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan2 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan3 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);

    cvCvtColor( imgIn, hsv, CV_BGR2YCrCb );
    cvSplit(hsv,chan0,chan1,chan2, NULL);

    //cvConvertImage(imgIn,src);

    src = cvCloneImage(chan0);
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* comp = NULL;
    //lower last param for more segments
    cvPyrSegmentation( src, imgOut, storage, &comp, 3, 200, 50 );
    cvAbsDiffS(imgOut,imgOut,CV_RGB(255,255,255));
    //cvNormalize(imgOut,imgOut,255,0, CV_MINMAX);
    //cvThreshold(imgOut,imgOut,250,255,CV_THRESH_TOZERO);
//        int n_comp = comp->total;
//        for( int i=0; i<n_comp; i++ )
//        {
//            CvConnectedComp* cc = (CvConnectedComp*) cvGetSeqElem( comp, i );
//            cvDrawRect(imgOut,cvPoint(cc->rect.x,cc->rect.y),cvPoint(cc->rect.x+cc->rect.width,cc->rect.y+cc->rect.height),CV_RGB(255,255,255));

//        //do_something_with( cc );
//        }
    cvReleaseImage(&src);
    cvReleaseImage(&hsv);
    cvReleaseImage(&chan0);
    cvReleaseImage(&chan1);
    cvReleaseImage(&chan2);
    cvReleaseImage(&chan3);
    cvReleaseMemStorage( &storage );
    return imgOut;
}

IplImage * BouyBaseObject::TemplateMask(const IplImage * imgIn, const IplImage * threshold, const IplImage * tmplt) const
{
    IplImage * imgOut = cvCloneImage(imgIn);
    //cvZero(imgOut);
    std::list<CvBox2D> blobList;
    blobList  = Zebulon::Vision::VisionUtils::GetBlobBoxes(threshold,.005,.95);
    IplImage * crop;
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1,1);

    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    {

        crop = Zebulon::Vision::VisionUtils::Crop(imgIn,*it);
        double score = 0;
        Zebulon::Vision::VisionUtils::GetSimpleTemplateSimilarity(crop,tmplt,score,false);
        std::ostringstream s;
        s << "(" << score << ")";
        cvPutText(imgOut,s.str().c_str(),cvPointFrom32f(it->center),&font,CV_RGB(255,255,255));
        Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
        if(score > mTemplateThreshold)
        {
            cvDrawCircle(imgOut,cvPointFrom32f(it->center),(crop->width/2.0),CV_RGB(255,255,255));
        }
        cvReleaseImage(&crop);
    }
    return imgOut;
}

IplImage* BouyBaseObject::GetMask(const IplImage * imgIn, IplImage * debugOut) const
{
    if(imgIn == NULL) return NULL;
    IplImage* colormask = NULL;
    IplImage* gvcolormask = NULL;
    //IplImage* shapemask = ShapeMask(imgIn);
    IplImage* segmentationmask = NULL;
    IplImage* histogrammask = NULL;
    //IplImage* edgemask = EdgeMask(imgIn);
    IplImage* templatemask = NULL;

//        if(colormask == NULL  || shapemask == NULL ||
//           segmentationmask== NULL || histogrammask == NULL ||
//           edgemask == NULL ) return NULL;

    //cvShowImage("colormask", colormask);
    //cvShowImage("channelmask", channelmask);
    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * threshold = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    cvZero(imgOut);
    if(mEnableHist)
    {
        histogrammask = HistogramMask(imgIn);
    }
    if(mEnableColor)
    {
        colormask = ColorMask(imgIn);
    }
    if(mEnableSegment)
    {
         segmentationmask = SegmentationMask(imgIn);
    }
    if(mEnableGVColor)
    {
         gvcolormask = GVColorMask(imgIn);
    }
    int count = 1;
    if(VisionUtils::CombineMasks(imgOut,histogrammask,imgOut,count,mHistWeight))
    {
        count++;
    }
    if(VisionUtils::CombineMasks(imgOut,colormask,imgOut, count, mColorWeight))
    {
        count++;
    }
    if(VisionUtils::CombineMasks(imgOut,segmentationmask,imgOut,count,mSegmentWeight))
    {
        count++;
    }
    if(VisionUtils::CombineMasks(imgOut,gvcolormask,imgOut,count,mGVColorWeight))
    {
        count++;
    }


    //VisionUtils::CombineMasks(imgOut,edgemask,imgOut,2,1);
    //VisionUtils::CombineMasks(imgOut,histogrammask,imgOut,2,1);
    cvNormalize(imgOut,imgOut,255,0,CV_MINMAX);
    if(mDebug)
    {
        cvShowImage("combined", imgOut);
    }
    cvThreshold(imgOut,threshold,mMainThreshold,255,CV_THRESH_BINARY );
    std::list<CvBox2D> blobList;
    blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(threshold,0,mMinNoiseSizePercent);
    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    {
        CvPoint2D32f boxCorners32[4];
        CvPoint boxCorners[4];
        cvBoxPoints(*it,boxCorners32);
        for(int i = 0; i < 4; i ++)
        {
            boxCorners[i] = cvPointFrom32f(boxCorners32[i]);
        }
        cvFillConvexPoly(threshold,boxCorners,4,cvScalar(0,0,0),4);
        //Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
    }
    //shapemask = FindCircles(imgOut);
    imgOut = TemplateMask(imgOut, threshold, mTemplate);
    //VisionUtils::CombineMasks(imgOut,templatemask,imgOut);
    if(mDebug)
    {
        cvShowImage("clean", threshold);
        cvShowImage("final", imgOut);
        cvShowImage("color", colormask);
        cvShowImage("hist", histogrammask);
        cvShowImage("segment", segmentationmask);
        cvShowImage("template", templatemask);
        cvShowImage("gvcolor", gvcolormask);
    }
    cvReleaseImage(&colormask);
    //cvReleaseImage(&shapemask);
    cvReleaseImage(&segmentationmask);
    cvReleaseImage(&histogrammask);
    cvReleaseImage(&gvcolormask);
    //cvReleaseImage(&edgemask);
    return imgOut;
}

std::list<CvBox2D> BouyBaseObject::GetBounding(const IplImage * imgIn, IplImage * imgOut) const
{

    std::list<CvBox2D> blobList;
    if(imgIn == NULL) return blobList;
    IplImage * imgOut1 = GetMask(imgIn);
    if(imgOut1)
    {
        blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(imgOut1,.001,.95);
        if (imgOut)
        {
            cvCopy(imgOut1, imgOut);
        }
        cvReleaseImage(&imgOut1);
    }
    return blobList;
}
