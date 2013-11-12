#include "auvc/underwater/vision/colorobject.h"
#include "opencv2/legacy/legacy.hpp"
ColorObject::ColorObject()
{
    mNearColor= CV_RGB(0,0,0);
    mFarColor = CV_RGB(0,0,0);
    mNearImage = NULL;
    mFarImage = NULL;
    mNearHist = NULL;
    mFarHist = NULL;
    mBouyTemplate = NULL;
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
    mSegment2Threshold = 20;
}

ColorObject::~ColorObject()
{
    Clear();
}

void ColorObject::Clear()
{
    mNearColor= CV_RGB(0,0,0);
    mFarColor = CV_RGB(0,0,0);
    mMainThreshold = 0;
    mMinNoiseSizePercent = 0;
    mTemplateThreshold = 0;
    mDebug = false;
    cvReleaseImage(&mNearImage);
    cvReleaseImage(&mFarImage);
    cvReleaseImage(&mBouyTemplate);
    cvReleaseHist(&mNearHist);
    cvReleaseHist(&mFarHist);
    mMaskOptions.clear();
}

bool ColorObject::Initialize(const std::string& parameter)
{
    mType = parameter;
    if(!mXml.Load("settings/vision/visionobjects.xml","VisionObjects"))
    {
        return false;
    }
    Clear();
    std::string file;
    //std::cout << mXml.GetPath() << std::endl;
    mXml.GetVar(parameter+"@debug",mDebug);
    if(mXml.GetVar(parameter+".Images.Near@file",file))
    {
        if(mNearImage) cvReleaseImage(&mNearImage);
        mNearImage = cvLoadImage(file.c_str());
        if(mNearImage)
        {
            mNearColor = cvAvg(mNearImage);
            if(mNearHist) cvReleaseHist(&mNearHist);
            mNearHist = VisionUtils::CreateHSVHist(mNearImage);
        }
    }
    if(mXml.GetVar(parameter+".Images.Far@file",file))
    {
        if(mFarImage) cvReleaseImage(&mFarImage);

        mFarImage = cvLoadImage(file.c_str());
        if(mFarImage)
        {
            //cvCvtColor(temp, convert, CV_BGR2YCrCb);
            mFarColor = cvAvg(mFarImage);
            if(mFarHist) cvReleaseHist(&mFarHist);
            mFarHist = VisionUtils::CreateHSVHist(mFarImage);
        }
    }
    if(mXml.GetVar(parameter+".Images.Template@file",file))
    {
        IplImage * temp = cvLoadImage(file.c_str());
        if(temp)
        {
            if(mBouyTemplate) cvReleaseImage(&mBouyTemplate);
            mBouyTemplate = cvCreateImage( cvGetSize(temp), 8, 1 );
            cvConvertImage(temp,mBouyTemplate);
            //cvThreshold(mBouyTemplate,mBouyTemplate,200,255,CV_THRESH_BINARY);
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
    if(mXml.GetVar(parameter+".Options.SegmentMask2@enable",enabled))
    {
        mEnableSegment2 = enabled;
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
    if(mXml.GetVar(parameter+".Options.SegmentMask2@weight",val))
    {
        mSegmentWeight2 = val;
    }
    if(mXml.GetVar(parameter+".Options.SegmentMask2@segment2threshold",val))
    {
        mSegment2Threshold = val;
    }
    if(mXml.GetVar(parameter+".Options.GVColorMask@weight",val))
    {
        mGVColorWeight= val;
    }
    VisionObject::MaskOption maskoption;
    std::string s;
    if(mXml.GetVar(parameter+".Options.ColorSpaceMask1@space",s))
    {
        maskoption.mCvColorConversionName = s;
    }
    if(mXml.GetVar(parameter+".Options.ColorSpaceMask1@enable",enabled))
    {
        maskoption.mEnabledFlag = enabled;
    }
    if(mXml.GetVar(parameter+".Options.ColorSpaceMask1@weight",val))
    {
        maskoption.mWeight = val;
    }
    if(mXml.GetVar(parameter+".Options.ColorSpaceMask1@invert",enabled))
    {
        maskoption.mInvertFlag = enabled;
    }
    if(mXml.GetVar(parameter+".Options.ColorSpaceMask1@channel",val))
    {
        maskoption.mChannelIndex = val;
        mMaskOptions.push_back(maskoption);
    }

    if(mXml.GetVar(parameter+".Options.ColorSpaceMask2@space",s))
    {
        maskoption.mCvColorConversionName = s;
    }
    if(mXml.GetVar(parameter+".Options.ColorSpaceMask2@enable",enabled))
    {
        maskoption.mEnabledFlag = enabled;
    }
    if(mXml.GetVar(parameter+".Options.ColorSpaceMask2@weight",val))
    {
        maskoption.mWeight = val;
    }
    if(mXml.GetVar(parameter+".Options.ColorSpaceMask2@invert",enabled))
    {
        maskoption.mInvertFlag = enabled;
    }
    if(mXml.GetVar(parameter+".Options.ColorSpaceMask2@channel",val))
    {
        maskoption.mChannelIndex = val;
        mMaskOptions.push_back(maskoption);
    }
    std::string tag;
    if(mXml.GetVar(parameter+".NearTarget@tag",tag))
    {
        mNearTarget.Setup(mXml,tag,parameter+".NearTarget");
    }
    if(mXml.GetVar(parameter+".FarTarget@tag",tag))
    {
        mFarTarget.Setup(mXml,tag,parameter+".FarTarget");
    }
    if(mNearImage  == NULL || mFarImage == NULL || mBouyTemplate == NULL) return false;
    return true;
}

IplImage * ColorObject::TemplateMask(const IplImage * imgIn, const IplImage * threshold, const IplImage * tmplt) const
{
    IplImage * imgOut = cvCloneImage(imgIn);
    cvZero(imgOut);
    std::list<CvBox2D> blobList;
    blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(threshold,.001,.95,false);
    //cvShowImage("blob threshold", threshold);
    IplImage * crop;
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1,1);
    CvBox2D best;
    double bestScore = -1;
    double secondbestscore = -1;
    CvBox2D secondbest;
    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    {
        crop = Zebulon::Vision::VisionUtils::Crop(imgIn,*it);
//        cvShowImage("crop", crop);
//        cvWaitKey(100);
        double score = 0;
        Zebulon::Vision::VisionUtils::GetSimpleTemplateSimilarity(crop,tmplt,score,false);
//        std::ostringstream s;
//        s << "(" << score << ")";
//        cvPutText(imgOut,s.str().c_str(),cvPointFrom32f(it->center),&font,CV_RGB(255,255,255));
//        Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
        if(score > mTemplateThreshold)
        {
            if(score > bestScore)
            {
                secondbestscore = bestScore;
                bestScore = score;
                secondbest = best;
                best = *it;
            }
            //cvDrawCircle(imgOut,cvPointFrom32f(it->center),(crop->width/2.0),CV_RGB(255,255,255));
        }
        cvReleaseImage(&crop);
    }
    if(bestScore > -1)
    {
        cvDrawCircle(imgOut,cvPointFrom32f(best.center),(best.size.height/2.0),CV_RGB(255,255,255),CV_FILLED);
    }
    if(secondbestscore > -1)
    {
        cvDrawCircle(imgOut,cvPointFrom32f(secondbest.center),(secondbest.size.height/2.0),CV_RGB(255,255,255),CV_FILLED);
    }
    return imgOut;
}

IplImage* ColorObject::GVColorMask(const IplImage * imgIn) const
{
    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * temp = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);

    IplImage * thresh = NULL;
    //mThresh->Setup(mNearColor.val[2],mNearColor.val[1],mNearColor.val[0],50,50,50);
    mThresh->Setup(mNearTarget,1);
    mThresh->Process(imgIn,thresh);
    //cvShowImage("gv img in",imgIn);
    //cvShowImage("gv thresh",thresh);
    cvConvertImage(thresh, imgOut);
    //mThresh->Setup(mFarColor.val[2],mFarColor.val[1],mFarColor.val[0],50,50,50);
    mThresh->Setup(mFarTarget,1);
    mThresh->Process(imgIn,thresh);
    cvConvertImage(thresh, temp);

    cvOr(imgOut,temp,imgOut);

    cvReleaseImage(&temp);
    return imgOut;
}
IplImage* ColorObject::ColorMask(const IplImage * imgIn) const
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
    VisionUtils::ExtractColor(imgIn,
                           temp,
                           mFarColor,
                           0,
                           1);
    VisionUtils::CombineMasks(imgOut,temp,imgOut);
    cvReleaseImage(&temp);
    return imgOut;

}
IplImage* ColorObject::EdgeMask(const IplImage * imgIn) const
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
    //cvDilate(imgOut,imgOut, NULL, 1);
    //cvShowImage("canny", imgOut);
    return imgOut;
}

IplImage* ColorObject::HistogramMask(const IplImage * imgIn) const
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
    cvCalcBackProject(planes,temp,mFarHist);
    VisionUtils::CombineMasks(imgOut,temp,imgOut);
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
IplImage* ColorObject::SegmentationMask(const IplImage * imgIn) const
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
    cvPyrSegmentation( src, imgOut, storage, &comp, 2, 200, 50 );
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
IplImage* ColorObject::SegmentationMask2(const IplImage * imgIn, IplImage* debugOut) const
{
//    CvSize imageSize = cvSize(imgIn->width & -2, imgIn->height & -2 );
//    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 1 );


    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * circleMask = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * src = cvCloneImage(imgIn);
    IplImage * scratch = cvCloneImage(src);
    IplImage * hist = HistogramMask(imgIn);
    //IplImage * bestMask = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);

    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* comp = NULL;

    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .5,.5);
    std::ostringstream s;

    cvZero(imgOut);
    cvZero(circleMask);
    cvZero(scratch);
    //cvZero(bestMask);

    CvScalar avgColor;
    double bestColor = -1;
    CvRect bestRect;
    double bestDiag = 0;
//    IplImage* hsv = cvCreateImage( cvGetSize(imgIn), 8, 3 );

//    IplImage * chan0 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
//    IplImage * segsum = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);

    //cvCvtColor( imgIn, hsv, CV_BGR2YCrCb );
    //cvCopyImage(imgIn,hsv);
    //cvSplit(hsv,chan0,chan1,chan2, NULL);
    //cvConvertImage(imgIn,src);

    //lower last param for more segments
    //cvPyrSegmentation( hsv, scratch, storage, &comp, 3, 100, 90 );


    cvPyrSegmentation( src, scratch, storage, &comp, 2, 0, 100);
    int n_comp = comp->total;

    std::list<CvBox2D> blobList;
    for( int i = n_comp-1; i>=1; i-- )
    {
        CvConnectedComp* cc = (CvConnectedComp*) cvGetSeqElem( comp, i );
        cvAbsDiffS(scratch,src,cc->value);
        cvNot(src,src);
        cvThreshold(src,src,254,255,CV_THRESH_BINARY);
        blobList = VisionUtils::GetBlobBoxes(src,0.0008,.95,false);
        for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
        {
            CvRect rect = VisionUtils::ToCvRect(*it);
            VisionUtils::MakeSquare(rect);
            double diagonal = sqrt(rect.width * rect.width + rect.height * rect.height);
            cvDrawCircle(circleMask,cvPoint(rect.x+rect.width/2.,rect.y+rect.height/2),diagonal/2.5,CV_RGB(255,255,255),CV_FILLED);

            avgColor = cvAvg (hist,circleMask);
            if((bestColor < 0 || bestColor < avgColor.val[0]) && avgColor.val[0] > mSegment2Threshold)
            {
                bestDiag = diagonal;
                bestColor = avgColor.val[0];
                bestRect = rect;
                cvCopy(circleMask,imgOut);
            }
            //cvMinMaxLoc(imgIn,)
            cvZero(circleMask);
        }
    }
    if(debugOut && bestColor > 0)
    {
         s.clear();
        s << "bestColor(" << bestColor << ") " << mType;
        cvPutText(debugOut,s.str().c_str(),cvPoint(bestRect.x+bestRect.width/2.,bestRect.y+bestRect.height/2),&font,CV_RGB(255,255,255));
        cvDrawCircle(debugOut,cvPoint(bestRect.x+bestRect.width/2.,bestRect.y+bestRect.height/2),bestDiag/2.5,CV_RGB(255,255,255));
    }
//    cvShowImage("best",bestMask);
//    cvWaitKey(0);

    //VisionUtils::ClearEdges(imgOut);
    cvReleaseImage(&scratch);
    cvReleaseImage(&src);
    cvReleaseImage(&hist);
    cvReleaseImage(&circleMask);
    cvReleaseMemStorage( &storage );
    return imgOut;
}
IplImage * ColorObject::FindCircles(const IplImage * imgIn) const
{
    IplImage * imgOut = cvCloneImage(imgIn);
    CvSize imageSize = cvSize(imgIn->width & -2, imgIn->height & -2 );
    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 1 );
    cvPyrDown( imgOut, imgSmallCopy);
    cvPyrUp( imgSmallCopy, imgOut);
    cvSmooth(imgOut, imgOut, CV_GAUSSIAN, 5);
    cvReleaseImage(&imgSmallCopy);
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* results = cvHoughCircles(
                imgOut,
                storage,
                CV_HOUGH_GRADIENT,
                2,
                imgOut->width/10,
                100,
                100);
    cvZero(imgOut);
    for( int i = 0; i < results->total; i++ ) {
        float* p = (float*) cvGetSeqElem( results, i );
        CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
        cvCircle(   imgOut,
                    pt,
                    cvRound( p[2] ),
                    CV_RGB(0xff,0xff,0xff),CV_FILLED);
    }
    cvReleaseMemStorage(&storage);
    return imgOut;
}

IplImage* ColorObject::ShapeMask(const IplImage * imgIn) const
{
    IplImage * hsv = cvCloneImage(imgIn);
    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    //cvEqualizeHist( imgIn, hsv);
    IplImage * chan0 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan1 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan2 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan3 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    cvCvtColor(imgIn, hsv, CV_BGR2YCrCb);

    cvSplit(hsv,chan0,chan1,chan2, NULL);

    CvScalar white = cvRealScalar(255);
    //imgOut = SegmentationMask(imgIn);
    imgOut = cvCloneImage(chan2);
    //invert black and white
    cvAbsDiffS(imgOut, imgOut, white);
//        cvShowImage("hue",chan0);
//        cvShowImage("sat",chan1);
//        cvShowImage("val",chan2);
//        cvShowImage("inv",imgOut);
    //cvWaitKey(0);

    cvReleaseImage(&hsv);
    cvReleaseImage(&chan0);
    cvReleaseImage(&chan1);
    cvReleaseImage(&chan2);
    cvReleaseImage(&chan3);

    CvSize imageSize = cvSize(imgIn->width & -2, imgIn->height & -2 );
    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 1 );
    cvPyrDown( imgOut, imgSmallCopy);
    cvPyrUp( imgSmallCopy, imgOut);
    cvSmooth(imgOut, imgOut, CV_GAUSSIAN, 5);
    cvReleaseImage(&imgSmallCopy);

    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* results = cvHoughCircles(
                imgOut,
                storage,
                CV_HOUGH_GRADIENT,
                2,
                imgOut->width/10,
                100,
                100);
    cvZero(imgOut);
    for( int i = 0; i < results->total; i++ ) {
        float* p = (float*) cvGetSeqElem( results, i );
        CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
        cvCircle(   imgOut,
                    pt,
                    cvRound( p[2] ),
                    CV_RGB(0xff,0xff,0xff),CV_FILLED);
    }
    cvReleaseMemStorage(&storage);
    return imgOut;
}
IplImage* ColorObject::GetMask(const IplImage * imgIn, IplImage * debugOut) const
{
//    if(imgIn == NULL) return NULL;
//    CvSize imageSize = cvSize(imgIn->width & -2, imgIn->height & -2 );
//    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 3 );
//    IplImage* smallDebug = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 3 );
//    IplImage* mask = NULL;
//    IplImage* result = cvCreateImage(cvGetSize(imgIn), IPL_DEPTH_8U, 1);
//    cvPyrDown( imgIn, imgSmallCopy);
//    if(debugOut)
//    {
//        cvPyrDown( debugOut, smallDebug);
//    }

//    mask = SegmentationMask2(imgSmallCopy,smallDebug);

//    cvPyrUp(mask,result);

//    if(debugOut)
//    {
//        cvPyrUp(smallDebug,debugOut);
//    }
//    cvReleaseImage(&smallDebug);
//    cvReleaseImage(&mask);
//    cvReleaseImage(&imgSmallCopy);
//    return result;

    if(imgIn == NULL) return NULL;
    //CvSize imageSize = cvSize(imgIn->width & -2, imgIn->height & -2 );
    CvSize imageSize = cvSize(imgIn->width, imgIn->height );
    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width, imageSize.height), IPL_DEPTH_8U, 3 );
    //cvPyrDown( imgIn, imgSmallCopy);
    cvConvertImage(imgIn,imgSmallCopy);
    IplImage* colormask = NULL;
    IplImage* gvcolormask = NULL;
    //IplImage* shapemask = ShapeMask(imgIn);
    IplImage* segmentationmask = NULL;
    IplImage* segmentationmask2 = NULL;
    IplImage* histogrammask = NULL;
    //IplImage* edgemask = EdgeMask(imgIn);
    IplImage* templatemask = NULL;
    IplImage* channelmask = NULL;

//        if(colormask == NULL  || shapemask == NULL ||
//           segmentationmask== NULL || histogrammask == NULL ||
//           edgemask == NULL ) return NULL;

    //cvShowImage("colormask", colormask);
    //cvShowImage("channelmask", channelmask);
    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * imgOutSmall = cvCreateImage(cvGetSize(imgSmallCopy),IPL_DEPTH_8U, 1);
    IplImage * threshold = cvCreateImage(cvGetSize(imgSmallCopy),IPL_DEPTH_8U, 1);
    cvZero(imgOut);
    cvZero(imgOutSmall);
    cvZero(threshold);
    if(mEnableHist)
    {
        histogrammask = HistogramMask(imgSmallCopy);
    }
    if(mEnableColor)
    {
        colormask = ColorMask(imgSmallCopy);
    }
    if(mEnableSegment)
    {
         segmentationmask = SegmentationMask(imgSmallCopy);
    }
    if(mEnableSegment2)
    {
         segmentationmask2 = SegmentationMask2(imgSmallCopy,debugOut);
    }
    if(mEnableGVColor)
    {
         gvcolormask = GVColorMask(imgSmallCopy);
    }
    int count = 1;
    if(VisionUtils::CombineMasks(imgOutSmall,histogrammask,imgOutSmall,count,mHistWeight))
    {
        count++;
    }
    if(VisionUtils::CombineMasks(imgOutSmall,colormask,imgOutSmall, count, mColorWeight))
    {
        count++;
    }
    if(VisionUtils::CombineMasks(imgOutSmall,segmentationmask,imgOutSmall,count,mSegmentWeight))
    {
        count++;
    }
    if(VisionUtils::CombineMasks(imgOutSmall,segmentationmask2,imgOutSmall,count,mSegmentWeight2))
    {
        count++;
    }
    if(VisionUtils::CombineMasks(imgOutSmall,gvcolormask,imgOutSmall,count,mGVColorWeight))
    {
        count++;
    }

    for(unsigned int i = 0; i < mMaskOptions.size(); i++)
    {
        if(mMaskOptions[i].mEnabledFlag)
        {
            channelmask = cvCreateImage(cvGetSize(imgSmallCopy),IPL_DEPTH_8U, 1);
            if(VisionUtils::ConvertAndGetSingleColorChannel(imgSmallCopy,channelmask,mMaskOptions[i].mCvColorConversionName, mMaskOptions[i].mChannelIndex))
            {
                if(mMaskOptions[i].mInvertFlag)
                {
                    VisionUtils::Invert(channelmask,channelmask);
                }
                if(VisionUtils::CombineMasks(imgOutSmall,channelmask,imgOutSmall,count,mMaskOptions[i].mWeight))
                {
                    count++;
                }
            }
            if(mDebug)
            {
                cvShowImage("channelmask", channelmask);
                //cvWaitKey(0);
            }
            cvReleaseImage(&channelmask);
        }
    }
    //VisionUtils::CombineMasks(imgOut,edgemask,imgOut,2,1);
    //VisionUtils::CombineMasks(imgOut,histogrammask,imgOut,2,1);
    //cvNormalize(imgOut,imgOut,255,0,CV_MINMAX);
    if(mDebug)
    {
        cvShowImage("combined", imgOutSmall);
    }
    //if(debugOut) cvConvertImage(imgOut,debugOut);
    cvThreshold(imgOutSmall,threshold,mMainThreshold,255,CV_THRESH_BINARY );
    if(mDebug)
    {
        cvShowImage("threshold", threshold);
    }
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
    IplImage * tempImage = cvCloneImage(imgOutSmall);
    //shapemask = FindCircles(imgOut);
    if(mTemplateThreshold > 0)
    {
        if(tempImage) cvReleaseImage(&tempImage);
      tempImage = TemplateMask(imgOutSmall, threshold, mBouyTemplate);
      cvConvertImage(tempImage,imgOut);
      if(debugOut) cvSet(debugOut,mNearColor,tempImage);
    }
    else
    {
        cvConvertImage(threshold,imgOut);
    }
    if(mDebug) cvShowImage("imgOut",imgOut);
    //cvPyrUp( tempImage, imgOut);;
    //cvWaitKey(1);
    //cvShowImage("template output",imgOut);
    //VisionUtils::CombineMasks(imgOut,templatemask,imgOut);
    if(mDebug)
    {
        if(threshold) cvShowImage("clean", threshold);
        if(imgOut) cvShowImage("final", imgOut);
        if(colormask) cvShowImage("color", colormask);
        if(histogrammask) cvShowImage("hist", histogrammask);
        if(segmentationmask) cvShowImage("segment", segmentationmask);
        if(templatemask) cvShowImage("template", templatemask);
        if(gvcolormask) cvShowImage("gvcolor", gvcolormask);
    }
    cvReleaseImage(&colormask);
    cvReleaseImage(&segmentationmask);
    cvReleaseImage(&segmentationmask2);
    cvReleaseImage(&histogrammask);
    cvReleaseImage(&gvcolormask);
    cvReleaseImage(&channelmask);
    cvReleaseImage(&templatemask);
    cvReleaseImage(&threshold);
    cvReleaseImage(&tempImage);
    cvReleaseImage(&imgSmallCopy);
    cvReleaseImage(&imgOutSmall);
    return imgOut;
}
std::list<CvBox2D> ColorObject::GetBounding(const IplImage * imgIn, IplImage * debugOut) const
{

    std::list<CvBox2D> blobList;
    if(imgIn == NULL) return blobList;
    IplImage * imgOut1 = GetMask(imgIn);
    //cvShowImage("mask output", imgOut1);
    if(imgOut1)
    {
        blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(imgOut1,mMinNoiseSizePercent,.95,false);
        if (debugOut)
        {
            //cvConvertImage(imgOut1, debugOut);
            if(blobList.size() > 0)
            {
                VisionUtils::DrawSquare(debugOut,blobList.front(),mNearColor);
            }
        }
        cvReleaseImage(&imgOut1);
    }
    return blobList;
}


bool ColorObject::GetVisionReturn(const IplImage * imgIn, Vision::Return& result, IplImage * debugOut) const
{
    //cvShowImage("right before bounding",imgIn);
    std::list<CvBox2D> blobList = GetBounding(imgIn);
    result.mValid = false;
    if(blobList.size() == 0) return false;
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1,1);
    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    {
        result.mCenterI = it->center.x;
        result.mCenterJ = it->center.y;
        result.mArea = it->size.width * it->size.height;
        result.mMaxJ = result.mCenterJ + it->size.height/2.0;
        result.mMinJ = result.mCenterJ - it->size.height/2.0;
        result.mMaxI = result.mCenterI + it->size.width/2.0;
        result.mMinI = result.mCenterI - it->size.width/2.0;
        result.mValid = true;
        //result.mAngle = VisionUtils::GetAngle(*it);
        //result.mAngle = -result.mAngle;
        if(debugOut)
        {
            std::ostringstream s;
            s << "Area(" << result.mArea << ")";
            cvPutText(debugOut,s.str().c_str(),cvPointFrom32f(it->center),&font,mNearColor);
            Zebulon::Vision::VisionUtils::DrawSquare(debugOut,*it,mNearColor);
        }
        break;
    }
    return true;
//    double mCenterI;    ///< I is like X 0 to width = left to right
//    double mCenterJ;    ///< J is like Y 0 to height  top to bottom
//    int mArea;          ///< Area in pixels, either pixel count or width*height of bounding box
//    double mAngle;      ///< Angle in degrees [-90, 90] positive to the right, negative left.

//    int mValid;         ///< Valid is an on or off (or boolean) if something is detected
//    double mConfidence; ///< Confidence in identification of target, higher is better

//    double mMinI;   ///< Bounding Box furthest left column index.
//    double mMinJ;   ///< Bounding Box lowest row value (lower value higher up).
//    double mMaxI;   ///< Bounding Box furthest right column index (higher value right).
//    double mMaxJ;   ///< Bounding Box highest row value (higher value is towards picture bottom).
}

bool ColorObject::GetVisionReturn(const IplImage * imgIn, std::vector<Vision::Return>& result, IplImage * debugOut, int uptocount) const
{
    //cvShowImage("right before bounding",imgIn);
    std::list<CvBox2D> blobList = GetBounding(imgIn);
    if(blobList.size() > 1)
    {
        std::cout << "hooray";
    }
    result.clear();
    Vision::Return temp;
    temp.mValid = false;
    if(blobList.size() == 0) return false;
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1,1);
    int count = 0;
    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    {
        temp.mCenterI = it->center.x;
        temp.mCenterJ = it->center.y;
        temp.mArea = it->size.width * it->size.height;
        temp.mMaxJ = temp.mCenterJ + it->size.height/2.0;
        temp.mMinJ = temp.mCenterJ - it->size.height/2.0;
        temp.mMaxI = temp.mCenterI + it->size.width/2.0;
        temp.mMinI = temp.mCenterI - it->size.width/2.0;
        temp.mValid = true;
        result.push_back(temp);
        //result.mAngle = VisionUtils::GetAngle(*it);
        //result.mAngle = -result.mAngle;
        if(debugOut)
        {
            std::ostringstream s;
            s << "Area(" << temp.mArea << ")";
            cvPutText(debugOut,s.str().c_str(),cvPointFrom32f(it->center),&font,mNearColor);
            Zebulon::Vision::VisionUtils::DrawSquare(debugOut,*it,mNearColor);
        }
        if(count == uptocount) break;
    }
    return true;
//    double mCenterI;    ///< I is like X 0 to width = left to right
//    double mCenterJ;    ///< J is like Y 0 to height  top to bottom
//    int mArea;          ///< Area in pixels, either pixel count or width*height of bounding box
//    double mAngle;      ///< Angle in degrees [-90, 90] positive to the right, negative left.

//    int mValid;         ///< Valid is an on or off (or boolean) if something is detected
//    double mConfidence; ///< Confidence in identification of target, higher is better

//    double mMinI;   ///< Bounding Box furthest left column index.
//    double mMinJ;   ///< Bounding Box lowest row value (lower value higher up).
//    double mMaxI;   ///< Bounding Box furthest right column index (higher value right).
//    double mMaxJ;   ///< Bounding Box highest row value (higher value is towards picture bottom).
}
