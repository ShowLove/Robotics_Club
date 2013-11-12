#include "auvc/underwater/vision/lovelaneobject2.h"

LoveLaneObject2::LoveLaneObject2()
{
   // Initialize("LoveLane");
    mNearColor= CV_RGB(0,0,0);
    mFarColor = CV_RGB(0,0,0);
    mNearImage = NULL;
    mFarImage = NULL;
    mNearHist = NULL;
    mFarHist = NULL;
    mTemplate = NULL;
    mDebug = true;
}
LoveLaneObject2::~LoveLaneObject2()
{
    Clear();
}

void LoveLaneObject2::Clear()
{
    mNearColor= CV_RGB(0,0,0);
    mFarColor = CV_RGB(0,0,0);
    mRatioThreshold = 0;
    mSegment2EnabledFlag = false;
    mSegment2Weight = 0;
    mDebug = false;
    cvReleaseImage(&mNearImage);
    cvReleaseImage(&mFarImage);
    cvReleaseImage(&mTemplate);
    cvReleaseHist(&mNearHist);
    cvReleaseHist(&mFarHist);
}

bool LoveLaneObject2::Initialize(const std::string& parameter)
{
    mType = parameter;
    if(!mXml.Load("settings/vision/visionobjects.xml","VisionObjects"))
    {
        return false;
    }
    std::string file;
    Clear();
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
            //cvThreshold(mTemplate,mTemplate,200,255,CV_THRESH_BINARY);
            cvReleaseImage(&temp);
        }
    }
    double val = 0;
    bool enabled = false;
    if(mXml.GetVar(parameter+".Options.SegmentMask2@ratiothreshold",val))
    {
        mRatioThreshold = val;
    }
    if(mXml.GetVar(parameter+".Options.SegmentMask2@enable",enabled))
    {
        mSegment2EnabledFlag = enabled;
    }
    if(mXml.GetVar(parameter+".Options.SegmentMask2@weight",val))
    {
        mSegment2Weight = val;
    }
    if(mNearImage  == NULL || mFarImage == NULL || mTemplate == NULL) return false;
    return true;
}

IplImage* LoveLaneObject2::HistogramMask(const IplImage * imgIn) const
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

IplImage* LoveLaneObject2::SegmentationMask2(const IplImage * imgIn, IplImage* debugOut) const
{
//    CvSize imageSize = cvSize(imgIn->width & -2, imgIn->height & -2 );
//    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 1 );


    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * circleMask = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * src = cvCloneImage(imgIn);
    IplImage * scratch = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, imgIn->nChannels);
    IplImage * sum = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, imgIn->nChannels);
    //IplImage * hist = HistogramMask(imgIn);
    //IplImage * bestMask = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);

    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* comp = NULL;

    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .5,.5);
    std::ostringstream s;

    cvZero(imgOut);
    cvZero(circleMask);
    cvZero(scratch);
    cvZero(sum);
    //cvZero(bestMask);
    double targetRatio = mTemplate->width/mTemplate->height;
    CvScalar avgColor;
    double bestColor = -1;
    CvBox2D bestRect;
    double bestRatio = 10000000000;
    CvBox2D largest;
    double largestArea = -1;
//    IplImage* hsv = cvCreateImage( cvGetSize(imgIn), 8, 3 );

//    IplImage * chan0 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
//    IplImage * segsum = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);

    //cvCvtColor( imgIn, hsv, CV_BGR2YCrCb );
    //cvCopyImage(imgIn,hsv);
    //cvSplit(hsv,chan0,chan1,chan2, NULL);
    //cvConvertImage(imgIn,src);

    //lower last param for more segments
    //cvPyrSegmentation( hsv, scratch, storage, &comp, 3, 100, 90 );


    cvPyrSegmentation( src, scratch, storage, &comp, 2, 0, 75);
    int n_comp = comp->total;

    std::list<CvBox2D> blobList;
    for( int i = n_comp-1; i>=1; i-- )
    {
        CvConnectedComp* cc = (CvConnectedComp*) cvGetSeqElem( comp, i );

        cvAbsDiffS(scratch,src,cc->value);
        cvNot(src,src);
        cvThreshold(src,src,254,255,CV_THRESH_BINARY);
        cvOr(src,sum,sum);
        blobList = VisionUtils::GetBlobBoxes(src,0.0008,.95,false);
//        cvShowImage("sum",sum);
//        cvWaitKey(0);
        for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
        {
            //CvRect rect = VisionUtils::ToCvRect(*it);
            //VisionUtils::MakeSquare(rect);
            //double diagonal = sqrt(rect.width * rect.width + rect.height * rect.height);
            //cvDrawCircle(circleMask,cvPoint(rect.x+rect.width/2.,rect.y+rect.height/2),diagonal/2.5,CV_RGB(255,255,255),CV_FILLED);
            VisionUtils::DrawSquare(circleMask,*it,CV_RGB(255,255,255),CV_FILLED);
            //avgColor = cvAvg (hist,circleMask);
            //cvShowImage("rectmask",circleMask);
            //cvWaitKey(0);
            double ratio1 = it->size.height / it->size.width;
            //double ratio2 = it->size.height / it->size.width;
            //VisionUtils::DrawSquare(debugOut,*it,CV_RGB(255,255,255),1);
            //double ratio = std::min(ratio1,ratio2);
            double area = it->size.width * it->size.height;
            if( /*bestRatio > ratio &&*/ ratio1 < mRatioThreshold)
            //if((bestColor < 0 || bestColor < avgColor.val[0]) && avgColor.val[0] > mSegment2Threshold)
            {
                if(largestArea < 0 && area > largestArea)
                {
                    //bestDiag = diagonal;
                    bestColor = avgColor.val[0];
                    bestRect = *it;
                    bestRatio = ratio1;
                    largestArea = area;
                    //cvCopy(circleMask,imgOut);
                }
            }
            else
            {
                VisionUtils::DrawSquare(src,*it,CV_RGB(0,0,0),2,true);
//                cvShowImage("crossed1",src);
//                cvWaitKey(0);
            }
            //cvMinMaxLoc(imgIn,)
            cvZero(circleMask);
        }
        blobList = VisionUtils::GetBlobBoxes(sum,0.0008,.95,false);
        //cvShowImage("componests",src);
        //cvWaitKey(0);
        for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
        {
            //CvRect rect = VisionUtils::ToCvRect(*it);
            //VisionUtils::MakeSquare(rect);
            //double diagonal = sqrt(rect.width * rect.width + rect.height * rect.height);
            //cvDrawCircle(circleMask,cvPoint(rect.x+rect.width/2.,rect.y+rect.height/2),diagonal/2.5,CV_RGB(255,255,255),CV_FILLED);
            VisionUtils::DrawSquare(circleMask,*it,CV_RGB(255,255,255),CV_FILLED);
            //avgColor = cvAvg (hist,circleMask);
            //cvShowImage("rectmask",circleMask);
            //cvWaitKey(0);
            double ratio1 = it->size.height / it->size.width;
            //double ratio2 = it->size.height / it->size.width;
            //VisionUtils::DrawSquare(debugOut,*it,CV_RGB(255,255,255),1);
            //double ratio = std::min(ratio1,ratio2);
            double area = it->size.width * it->size.height;
            if( /*bestRatio > ratio &&*/ ratio1 < mRatioThreshold)
            //if((bestColor < 0 || bestColor < avgColor.val[0]) && avgColor.val[0] > mSegment2Threshold)
            {
                if(largestArea < 0 && area > largestArea)
                {
                    //bestDiag = diagonal;
                    bestColor = avgColor.val[0];
                    bestRect = *it;
                    bestRatio = ratio1;
                    largestArea = area;
                    //cvCopy(circleMask,imgOut);
                }
            }
            else
            {
                VisionUtils::DrawSquare(sum,*it,CV_RGB(0,0,0),2,true);
//                cvShowImage("crossed2",sum);
//                cvWaitKey(0);

            }
            cvZero(circleMask);
        }
        blobList = VisionUtils::GetBlobBoxes(src,0.0008,.95,false);
        for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
        {
            //CvRect rect = VisionUtils::ToCvRect(*it);
            //VisionUtils::MakeSquare(rect);
            //double diagonal = sqrt(rect.width * rect.width + rect.height * rect.height);
            //cvDrawCircle(circleMask,cvPoint(rect.x+rect.width/2.,rect.y+rect.height/2),diagonal/2.5,CV_RGB(255,255,255),CV_FILLED);
            VisionUtils::DrawSquare(circleMask,*it,CV_RGB(255,255,255),CV_FILLED);
            //avgColor = cvAvg (hist,circleMask);
            //cvShowImage("rectmask",circleMask);
            //cvWaitKey(0);
            double ratio1 = it->size.height / it->size.width;
            //double ratio2 = it->size.height / it->size.width;
            //VisionUtils::DrawSquare(debugOut,*it,CV_RGB(255,255,255),1);
            //double ratio = std::min(ratio1,ratio2);
            double area = it->size.width * it->size.height;
//            cvShowImage("mask1",circleMask);
//            cvWaitKey(0);
            if( /*bestRatio > ratio &&*/ ratio1 < mRatioThreshold)
            //if((bestColor < 0 || bestColor < avgColor.val[0]) && avgColor.val[0] > mSegment2Threshold)
            {
                if(largestArea < 0 && area > largestArea)
                {
                    //bestDiag = diagonal;
                    bestColor = avgColor.val[0];
                    bestRect = *it;
                    bestRatio = ratio1;
                    largestArea = area;
                }
            }
            //cvMinMaxLoc(imgIn,)
            cvZero(circleMask);
        }
        blobList = VisionUtils::GetBlobBoxes(sum,0.0008,.95,false);
        //cvShowImage("componests",src);
        //cvWaitKey(0);
        for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
        {
            //CvRect rect = VisionUtils::ToCvRect(*it);
            //VisionUtils::MakeSquare(rect);
            //double diagonal = sqrt(rect.width * rect.width + rect.height * rect.height);
            //cvDrawCircle(circleMask,cvPoint(rect.x+rect.width/2.,rect.y+rect.height/2),diagonal/2.5,CV_RGB(255,255,255),CV_FILLED);
            VisionUtils::DrawSquare(circleMask,*it,CV_RGB(255,255,255),CV_FILLED);
            //avgColor = cvAvg (hist,circleMask);
            //cvShowImage("rectmask",circleMask);
            //cvWaitKey(0);
            double ratio1 = it->size.height / it->size.width;
            //double ratio2 = it->size.height / it->size.width;
            //VisionUtils::DrawSquare(debugOut,*it,CV_RGB(255,255,255),1);
            //double ratio = std::min(ratio1,ratio2);
            double area = it->size.width * it->size.height;
//            cvShowImage("mask2",circleMask);
//            cvWaitKey(0);
            if( /*bestRatio > ratio &&*/ ratio1 < mRatioThreshold)
            //if((bestColor < 0 || bestColor < avgColor.val[0]) && avgColor.val[0] > mSegment2Threshold)
            {
                if(largestArea < 0 && area > largestArea)
                {
                    //bestDiag = diagonal;
                    bestColor = avgColor.val[0];
                    bestRect = *it;
                    bestRatio = ratio1;
                    largestArea = area;
                    //cvCopy(circleMask,imgOut);
                }
            }
            cvZero(circleMask);
        }
    }
    if(debugOut && bestColor > 0)
    {
        s.clear();
        s << "ratio(" << bestRatio << ") " << mType;
        VisionUtils::DrawSquare(debugOut,bestRect,CV_RGB(255,255,255),1);
        cvPutText(debugOut,s.str().c_str(),cvPoint(bestRect.center.x,bestRect.center.y),&font,CV_RGB(255,255,255));
//        cvDrawCircle(debugOut,cvPoint(bestRect.x+bestRect.width/2.,bestRect.y+bestRect.height/2),bestDiag/2.5,CV_RGB(255,255,255));
    }
    VisionUtils::DrawSquare(imgOut,bestRect,CV_RGB(255,255,255),CV_FILLED);
//    cvShowImage("best",imgOut);
//    cvShowImage("debg",debugOut);
//    cvWaitKey(0);

    //VisionUtils::ClearEdges(imgOut);
    cvReleaseImage(&scratch);
    cvReleaseImage(&sum);
    cvReleaseImage(&src);
    //cvReleaseImage(&hist);
    cvReleaseImage(&circleMask);
    cvReleaseMemStorage( &storage );
    return imgOut;
}
IplImage* LoveLaneObject2::GetMask(const IplImage * imgIn, IplImage * debugOut) const
{
    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    CvSize imageSize = cvSize(imgIn->width & -2, imgIn->height & -2 );
    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 3 );
    IplImage* smallDebug = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 3 );
    IplImage* mask = NULL;
    //IplImage* result = cvCreateImage(cvGetSize(imgIn), IPL_DEPTH_8U, 1);
    cvPyrDown( imgIn, imgSmallCopy);
    cvZero(imgOut);
    if(debugOut)
    {
        cvPyrDown( debugOut, smallDebug);
    }
    mask = SegmentationMask2(imgSmallCopy,smallDebug);

    cvPyrUp(mask,imgOut);

    if(debugOut)
    {
        cvPyrUp(smallDebug,debugOut);
    }
    cvThreshold(imgOut,imgOut,100,255,CV_THRESH_BINARY);
    //cvShowImage("imgout",imgOut);
    cvReleaseImage(&smallDebug);
    cvReleaseImage(&mask);
    cvReleaseImage(&imgSmallCopy);
    return imgOut;
}

std::list<CvBox2D> LoveLaneObject2::GetBounding(const IplImage * imgIn, IplImage * debugOut) const
{

    std::list<CvBox2D> blobList;
    if(imgIn == NULL) return blobList;
    IplImage * mask = GetMask(imgIn,debugOut);
    blobList = VisionUtils::GetBlobBoxes(mask,0,1);

    cvReleaseImage(&mask);
    return blobList;

}

bool LoveLaneObject2::GetVisionReturn(const IplImage * imgIn, Vision::Return& horizontal, IplImage * debugOut) const
{

    std::list<CvBox2D> blobList;
    if(imgIn == NULL) return false;
    Vision::Return result;
    CvFont font;
    blobList = GetBounding(imgIn);
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .25,1);
    std::stringstream s;
    std::stringstream s2;
    std::stringstream s3;
    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    {
        result = VisionUtils::VisionReturn(*it);

        if (result.mAngle< -75. || result.mAngle > 75.)
        {
            horizontal = result;
            horizontal.mValid = true;
            if(debugOut)
            {
                Zebulon::Vision::VisionUtils::DrawSquare(debugOut,*it,mNearColor);
                s << "(Horizontal) " << mType ;
                cvPutText(debugOut,s.str().c_str(),cvPoint(result.mMinI,result.mMinJ-20),&font,CV_RGB(255,255,255));
                s2 << "Width: " << result.mMaxI - result.mMinI;
                cvPutText(debugOut,s2.str().c_str(),cvPoint(result.mMinI,result.mMinJ),&font,CV_RGB(255,255,255));
                s3 << "Angle:" << result.mAngle;
                cvPutText(debugOut,s3.str().c_str(),cvPoint(result.mMinI,result.mMinJ+20),&font,CV_RGB(255,255,255));
            }
            break;
        }
    }
    return true;

}
