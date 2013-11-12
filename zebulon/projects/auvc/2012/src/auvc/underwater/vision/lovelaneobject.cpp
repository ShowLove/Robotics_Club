#include "auvc/underwater/vision/lovelaneobject.h"
#include "opencv2/legacy/legacy.hpp"
LoveLaneObject::LoveLaneObject()
{
   // Initialize("LoveLane");
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
    mSeg = new Vision::Segment;
    mThresh = new Vision::Threshhold;
    mRatio = new Vision::ThreshRatio;
}
LoveLaneObject::~LoveLaneObject()
{
    Clear();
}

void LoveLaneObject::Clear()
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
    mMaskOptions.clear();
}

bool LoveLaneObject::Initialize(const std::string& parameter)
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
    if(mXml.GetVar(parameter+".Options@minvalidareapixels",val))
    {
        mMinValidAreaPixels = val;
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
    if(mXml.GetVar(parameter+".Options.EdgeMask@enable",enabled))
    {
        mEnableEdge = enabled;
    }
    if(mXml.GetVar(parameter+".Options.RansacMask@enable",enabled))
    {
        mEnableRansac = enabled;
    }

    if(mXml.GetVar(parameter+".Options.HistMask@weight",val))
    {
        mHistWeight = val;
    }
    if(mXml.GetVar(parameter+".Options.ColorMask@weight",val))
    {
        mColorWeight = val;
    }
    if(mXml.GetVar(parameter+".Options.RansacMask@weight",val))
    {
        mSegmentWeight = val;
    }
    if(mXml.GetVar(parameter+".Options.GVColorMask@weight",val))
    {
        mGVColorWeight= val;
    }
    if(mXml.GetVar(parameter+".Options.EdgeMask@weight",val))
    {
        mEdgeWeight= val;
    }
    if(mXml.GetVar(parameter+".Options.GVColorMask@weight",val))
    {
        mRansacWeight= val;
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

//    if(mXml.GetVar(parameter+".Options.ColorSpaceMask2@space",s))
//    {
//        maskoption.mCvColorConversionName = s;
//    }
//    if(mXml.GetVar(parameter+".Options.ColorSpaceMask2@enable",enabled))
//    {
//        maskoption.mEnabledFlag = enabled;
//    }
//    if(mXml.GetVar(parameter+".Options.ColorSpaceMask2@weight",val))
//    {
//        maskoption.mWeight = val;
//    }
//    if(mXml.GetVar(parameter+".Options.ColorSpaceMask2@invert",enabled))
//    {
//        maskoption.mInvertFlag = enabled;
//    }
//    if(mXml.GetVar(parameter+".Options.ColorSpaceMask2@channel",val))
//    {
//        maskoption.mChannelIndex = val;
//        mMaskOptions.push_back(maskoption);
//    }
    std::string tag;
    if(mXml.GetVar(parameter+".NearTarget@tag",tag))
    {
        mNearTarget.Setup(mXml,tag,parameter+".NearTarget");
    }
    if(mXml.GetVar(parameter+".FarTarget@tag",tag))
    {
        mFarTarget.Setup(mXml,tag,parameter+".FarTarget");
    }
    if(mXml.GetVar(parameter+".NearTarget@tag",tag))
    {
        mNearTarget.Setup(mXml,tag,parameter+".NearTarget");
    }

    if(mNearImage  == NULL || mFarImage == NULL || mTemplate == NULL) return false;
    return true;
}

IplImage * LoveLaneObject::TemplateMask(const IplImage * imgIn, const IplImage * threshold, const IplImage * tmplt) const
{
    IplImage * imgOut = cvCloneImage(imgIn);
    cvZero(imgOut);
    std::list<CvBox2D> blobList;
    blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(threshold,.001,.95);
    IplImage * crop;
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1,1);

    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    {
        crop = Zebulon::Vision::VisionUtils::Crop(threshold,*it);
        //cvShowImage("crop",crop);
        double score = 0;
        Zebulon::Vision::VisionUtils::GetSimpleTemplateSimilarity(crop,tmplt,score);
//        std::ostringstream s;
//        s << "(" << score << ")";
//        cvPutText(imgOut,s.str().c_str(),cvPointFrom32f(it->center),&font,CV_RGB(255,255,255));
//        Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
        if(score > mTemplateThreshold)
        {
            //cvDrawCircle(imgOut,cvPointFrom32f(it->center),(crop->width/2.0),CV_RGB(255,255,255));
            VisionUtils::DrawSquare(imgOut,*it,CV_RGB(255,255,255));
        }
        cvReleaseImage(&crop);
    }
    return imgOut;
}

bool LoveLaneObject::RansacFit(const IplImage* imgIn, CxUtils::Segment3D& line, IplImage * mask, const bool invertMask, const int lineWidth ) const
{
    if(imgIn == NULL || mask == NULL) return false;
    CvSize imageSize = cvSize(imgIn->width & -2, imgIn->height & -2 );
    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), IPL_DEPTH_8U, 1 );
    cvPyrDown( imgIn, imgSmallCopy);
    CvPoint p1, p2, p3, p4;
    //cvAddS(mask,CV_RGB(255,255,255),mask);
    bool result = false;
    std::vector<double> lineParameters;
    std::vector<Point2D *> pointDataPtr;
    std::vector<Point2D> pointData;
    LineParamEstimator lpEstimator(0.5);
    double desiredProbabilityForNoOutliers = 0.999;
    double usedData = 0;
    //populate point2d vector with image
    for(int x = 0; x < imgSmallCopy->width; x++)
    {
        for(int y = 0; y < imgSmallCopy->height; y++)
        {
            unsigned char val = Zebulon::Vision::VisionUtils::GetPixel(imgSmallCopy,y,x,0);
            if(val > 100)
            {
                pointDataPtr.push_back(new Point2D(y,x));
                pointData.push_back(*(pointDataPtr[pointDataPtr.size()-1]));
            }
        }
    }
    if(pointData.size() < 8000)
    {
        lpEstimator.leastSquaresEstimate(pointDataPtr,lineParameters);
        //A RANSAC estimate of the line parameters
        //usedData = mRansac.compute(lineParameters,
        usedData = RANSAC<Point2D,double>::compute(lineParameters,
                                                   &lpEstimator ,
                                                   pointData,
                                                   desiredProbabilityForNoOutliers);

        if(lineParameters.size() > 0 /*&& usedData > mMinRansacUsedData && usedData < mMaxRansacUsedData*/)
        {
            //std::cout<<"RANSAC line parameters [n_x,n_y,a_x,a_y]\n\t [ "<<lineParameters[0]<<", "<<lineParameters[1]<<", ";
            //std::cout<<lineParameters[2]<<", "<<lineParameters[3]<<" ]"<<std::endl;
            double rise = -lineParameters[1];
            double run = lineParameters[0];
            double x1 = lineParameters[3] - 1000*run;
            double y1 = lineParameters[2] - 1000*rise;
            double x2 = lineParameters[3] + 1000*run;
            double y2 = lineParameters[2] + 1000*rise;
            p1 = cvPoint(x1*2,y1*2);
            p2 = cvPoint(x2*2,y2*2);
            cvClipLine(cvSize(imgIn->width,imgIn->height),&p1,&p2);
            if(invertMask)
            {
                cvLine( mask, p1, p2, CV_RGB(0,0,0), lineWidth, 8 );
            }
            else
            {
                cvLine( mask, p1, p2, CV_RGB(255,255,255), lineWidth, 8 );
            }
            //cvShowImage("blah",binaryImage);
            line = CxUtils::Segment3D(CxUtils::Point3D(p1.x,p1.y), CxUtils::Point3D(p2.x,p2.y));
            result = true;
        }
    }
    for(int i = 0; i < pointDataPtr.size(); i++)
    {
        delete pointDataPtr[i];
        pointDataPtr[i] = NULL;
    }
    pointData.clear();
    pointDataPtr.clear();
    cvReleaseImage(&imgSmallCopy);
    return result;

}
IplImage* LoveLaneObject::GVColorMask(const IplImage * imgIn) const
{
    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * temp = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);

    IplImage * thresh = NULL;
    //mThresh->Setup(mNearColor.val[2],mNearColor.val[1],mNearColor.val[0],50,50,50);
    mThresh->Setup(mNearTarget);
    mThresh->Process(imgIn,thresh);
    cvConvertImage(thresh, imgOut);

    //mThresh->Setup(mFarColor.val[2],mFarColor.val[1],mFarColor.val[0],50,50,50);
    mThresh->Setup(mNearTarget);
    mThresh->Process(imgIn,thresh);
    cvConvertImage(thresh, temp);

    VisionUtils::CombineMasks(imgOut,temp,imgOut);

    cvReleaseImage(&temp);
    return imgOut;
}
IplImage* LoveLaneObject::ColorMask(const IplImage * imgIn) const
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
IplImage* LoveLaneObject::EdgeMask(const IplImage * imgIn) const
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
    std::list<CvBox2D> blobList;
    blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(imgOut,0,mMinNoiseSizePercent);
    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    {
        CvPoint2D32f boxCorners32[4];
        CvPoint boxCorners[4];
        cvBoxPoints(*it,boxCorners32);
        for(int i = 0; i < 4; i ++)
        {
            boxCorners[i] = cvPointFrom32f(boxCorners32[i]);
        }
        cvFillConvexPoly(imgOut,boxCorners,4,cvScalar(0,0,0),4);
        //Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
    }
    return imgOut;
}

IplImage* LoveLaneObject::SegmentationMask(const IplImage * imgIn) const
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

IplImage* LoveLaneObject::HistogramMask(const IplImage * imgIn) const
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
IplImage* LoveLaneObject::GetMask(const IplImage * imgIn, IplImage * debugOut) const
{
    if(imgIn == NULL) return NULL;
    IplImage* colormask = NULL;
    IplImage* gvcolormask = NULL;
    //IplImage* ransacmask = NULL;
    IplImage* segmentationmask = NULL;
    IplImage* histogrammask = NULL;
    IplImage* edgemask = NULL;
    IplImage* templatemask = NULL;
    IplImage* channelmask = NULL;
//        if(colormask == NULL  || shapemask == NULL ||
//           segmentationmask== NULL || histogrammask == NULL ||
//           edgemask == NULL ) return NULL;

    //cvShowImage("colormask", colormask);
    //cvShowImage("channelmask", channelmask);
    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * threshold = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    //ransacmask = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
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
    if(mEnableGVColor)
    {
         gvcolormask = GVColorMask(imgIn);
    }
    if(mEnableEdge)
    {
         edgemask = EdgeMask(imgIn);
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
    if(VisionUtils::CombineMasks(imgOut,edgemask,imgOut,count,mEdgeWeight))
    {
        count++;
    }
    for(unsigned int i = 0; i < mMaskOptions.size(); i++)
    {
        if(mMaskOptions[i].mEnabledFlag)
        {
            channelmask = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
            if(VisionUtils::ConvertAndGetSingleColorChannel(imgIn,channelmask,mMaskOptions[i].mCvColorConversionName, mMaskOptions[i].mChannelIndex))
            {
                if(mMaskOptions[i].mInvertFlag)
                {
                    VisionUtils::Invert(channelmask,channelmask);
                }
                if(VisionUtils::CombineMasks(imgOut,channelmask,imgOut,count,mMaskOptions[i].mWeight))
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
    cvNormalize(imgOut,imgOut,255,0,CV_MINMAX);
    if(mDebug)
    {
        cvShowImage("combined", imgOut);
    }
    cvThreshold(imgOut,threshold,mMainThreshold,255,CV_THRESH_BINARY );

    cvCopy(threshold, imgOut);
    std::list<CvBox2D> blobList;
    //blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(threshold,0,1);
    blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(imgOut,0,mMinNoiseSizePercent);
    int count1 = 0;
    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    {
//        count1++;
//        if(count1 <= 2) continue;
        CvPoint2D32f boxCorners32[4];
        CvPoint boxCorners[4];
        cvBoxPoints(*it,boxCorners32);
        for(int i = 0; i < 4; i ++)
        {
            boxCorners[i] = cvPointFrom32f(boxCorners32[i]);
        }
        cvFillConvexPoly(imgOut,boxCorners,4,cvScalar(0,0,0),4);
        //Zebulon::Vision::VisionUtils::DrawSquare(imgOut,*it);
    }
    if(debugOut) cvSet(debugOut,mNearColor,imgOut);

    //shapemask = FindCircles(imgOut);
    //imgOut = TemplateMask(imgOut, threshold, mTemplate);
    //VisionUtils::CombineMasks(imgOut,templatemask,imgOut);
    if(mDebug)
    {
        //cvShowImage("clean", threshold);
        cvShowImage("channelmask", channelmask);
        cvShowImage("final", imgOut);
        cvShowImage("color", colormask);
        cvShowImage("hist", histogrammask);
        cvShowImage("segment", segmentationmask);
        cvShowImage("template", templatemask);
        cvShowImage("gvcolor", gvcolormask);
        cvShowImage("edge", edgemask);
    }
    cvReleaseImage(&channelmask);
    //cvReleaseImage(&shapemask);
    cvReleaseImage(&colormask);
    cvReleaseImage(&histogrammask);
    cvReleaseImage(&gvcolormask);
    cvReleaseImage(&segmentationmask);
    cvReleaseImage(&templatemask);
    cvReleaseImage(&histogrammask);
    cvReleaseImage(&gvcolormask);
    cvReleaseImage(&edgemask);
    return imgOut;
}

std::list<CvBox2D> LoveLaneObject::GetBounding(const IplImage * imgIn, IplImage * debugOut) const
{

    std::list<CvBox2D> blobList;
    std::list<CvBox2D> blobList2;
    if(imgIn == NULL) return blobList;

    IplImage* ransacmask1 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage* ransacmask2 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage* temp1 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage* temp2 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage* invert = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage* mask = NULL;

    //using histogram for ransac as it returns sparse points consitently
    //must be careful that there arent too many points or ransac will run too slow
    IplImage * hist = NULL;
    //binary mask of object
    mask = GetMask(imgIn);

    cvZero(ransacmask1);
    cvZero(ransacmask2);
    cvZero(temp1);
    cvZero(temp2);
    cvZero(invert);

    CxUtils::Segment3D vertical;
    CxUtils::Segment3D horizontal;
    CxUtils::Segment3D line1;
    CxUtils::Segment3D line2;
    CxUtils::Point3D intersection;

    bool horizontalfound = false;
    bool verticalfound = false;

    CvRect rect1;
    CvRect rect2;
    double area1 = -1;
    double area2 = -1;
    int found = 0;
    //hist = HistogramMask(imgIn);
    hist = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    cvCopy(mask,hist);
    VisionUtils::ClearEdges(hist);

    if(RansacFit(hist,line1,ransacmask1,false))
    {
        //found a line
        found++;
    }
    //run ransac for second bar, must erase previously used lines
    //by inverting ransacmask1 and 'anding' it with original histogram
    cvNot(ransacmask1,invert);
    cvAnd(invert,hist,hist);

    //if no line was found before, skip this
    if(found > 0 && RansacFit(hist,line2,ransacmask2,false))
    {
        //found another line
        found++;

    }
    bool intersects = false;
    if(found == 2)
    {
        intersects = CxUtils::Segment3D::DoesIntersectXY(line1,line2,intersection);
    }
    //create final mask lines using ransac and segmentation
    if(found > 0)
    {
        //erasing evrything outside the line before we run segmentation
        cvAnd(mask,ransacmask1, temp1);
        //add our intersection point if it exists
//        if(intersects)w
        {
            cvCircle(temp1,cvPoint(intersection.mX,intersection.mY),5,CV_RGB(255,255,255), CV_FILLED);
        }
        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* comp = NULL;

        //lower last param for more segments
        //trick to get bounding box around all pixels left
        cvPyrSegmentation( temp1, temp2, storage, &comp, 2, 255, 250 );
        int n_comp = comp->total;
        cvZero(temp1);
        //there shouldnt be more than 2 connected components
        //use second one, first one is entire image
        for( int i = 1; i < n_comp, i < 2; i++ )
        {
            CvConnectedComp* cc = (CvConnectedComp*) cvGetSeqElem( comp, i );
            rect1 = cc->rect;
            area1 = cc->area;
            //fill this area
            cvDrawRect(temp1,cvPoint(cc->rect.x,cc->rect.y),cvPoint(cc->rect.x+cc->rect.width,cc->rect.y+cc->rect.height),CV_RGB(255,255,255),CV_FILLED);
        }
        //"and" conncted component mask and ransac mask to get final line
        //bound with blobfinding later
        cvAnd(temp1,ransacmask1,ransacmask1);
        cvReleaseMemStorage( &storage );
    }

    //zero helper images
    cvZero(temp1);
    cvZero(temp2);
    if(found > 1)
    {
        cvAnd(mask,ransacmask2, temp1);
        if(intersects)
        {
            cvCircle(temp1,cvPoint(intersection.mX,intersection.mY),5,CV_RGB(255,255,255), CV_FILLED);
        }
        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* comp = NULL;
        //lower last param for more segments
        cvPyrSegmentation( temp1, temp2, storage, &comp, 2, 255, 250 );
        int n_comp = comp->total;
        cvZero(temp1);
        //there shouldnt be more than 2 connected components
        //use second one, first one is entire image
        for( int i = 1; i < n_comp, i < 2; i++ )
        {
            CvConnectedComp* cc = (CvConnectedComp*) cvGetSeqElem( comp, i );
            rect2 = cc->rect;
            area2 = cc->area;
            //fill this area
            cvDrawRect(temp1,cvPoint(cc->rect.x,cc->rect.y),cvPoint(cc->rect.x+cc->rect.width,cc->rect.y+cc->rect.height),CV_RGB(255,255,255),CV_FILLED);
        }
        //"and" conncted component mask and ransac mask to get final line
        //bound with blobfinding later
        cvAnd(temp1,ransacmask2,ransacmask2);
        cvReleaseMemStorage( &storage );
    }
    if(mDebug)
    {
        cvShowImage("ransacmask1",ransacmask1);
        cvShowImage("ransacmask2",ransacmask2);
        cvShowImage("mask",mask);
    }
    //now we have a mask for 0, 1 or both bars
    //do other checks on lines for safety

    //more than one bar found and they intersect
    if(found == 2)
    {
        double angle1 = line1.GetAngleXY(true);
        double angle2 = line2.GetAngleXY(true);
        double anglediff = fabs(CxUtils::Orientation::AngleDiff(angle1,angle2,false));
        //check if angles are within reason for a right angle
        if(anglediff > 45 && anglediff < 135)
        {
            //should be safe to assume these are the right lines
            blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(ransacmask1,.001,.95,true);
            blobList2 = Zebulon::Vision::VisionUtils::GetBlobBoxes(ransacmask2,.001,.95,true);
            blobList.push_back(blobList2.front());
        }
        //if they arent, they are probably part of same line, just grab biggest
        else
        {
            if(area1 > area2)
            {
                blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(ransacmask1,.001,.95,true);
            }
            else
            {
                blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(ransacmask2,.001,.95,true);
            }
        }
    }
    //2 lines are parallel, choose leftmost or bottom most line
    else if(found == 2 && !intersects)
    {
        blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(ransacmask1,.001,.95,true);
        blobList2 = Zebulon::Vision::VisionUtils::GetBlobBoxes(ransacmask2,.001,.95,true);
        CvBox2D blob1 = blobList.front();
        CvBox2D blob2 = blobList2.front();
        double angle1 = Vision::VisionUtils::GetAngle(blob1);
        double angle2 = Vision::VisionUtils::GetAngle(blob2);
        double ave = (angle1+angle2)/2.;
        //line is horizontal
        if (ave < -45. || ave > 45)
        {
            //take highest y point(rightmost line)
            if(blob1.center.y < blob2.center.y)
            {
                blob1 = blob2;
            }
        }
        //line is vertical
        else
        {
            //take lowest x point(leftmost line)
            if(blob1.center.x > blob2.center.x)
            {
                blob1 = blob2;
            }
        }
        blobList.clear();
        blobList.push_back(blob1);
    }
    //only one bar found, easiest case
    else if(found == 1)
    {
        blobList = Zebulon::Vision::VisionUtils::GetBlobBoxes(ransacmask1,.001,.95,true);
    }
    if (debugOut)
    {
        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1,1);
        //cvConvertImage(imgOut1, debugOut);
        for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
        {
            VisionUtils::DrawSquare(debugOut,*it,mNearColor);
            std::ostringstream s;
            s << "(Lovelane)" << it->angle;
            cvPutText(debugOut,s.str().c_str(),cvPointFrom32f((*it).center),&font,CV_RGB(255,255,255));
            Zebulon::Vision::VisionUtils::DrawSquare(debugOut,*it,mNearColor);
        }
    }
    cvReleaseImage(&temp1);
    cvReleaseImage(&temp2);
    cvReleaseImage(&ransacmask1);
    cvReleaseImage(&ransacmask2);
    cvReleaseImage(&invert);
    cvReleaseImage(&mask);
    cvReleaseImage(&hist);
    return blobList;

}

bool LoveLaneObject::GetVisionReturn(const IplImage * imgIn, Vision::Return& horizontal, Vision::Return& vertical, IplImage * debugOut) const
{

    std::list<CvBox2D> blobList;
    if(imgIn == NULL) return false;

    horizontal.mValid = false;
    vertical.mValid = false;
    blobList = GetBounding(imgIn);
    bool horizontalFound = false;
    bool verticalFound = false;
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1,1);
    for(std::list<CvBox2D>::iterator it = blobList.begin(); it != blobList.end(); it++)
    {
        std::ostringstream s;
        double angle = Vision::VisionUtils::GetAngle(*it);
        double area = it->size.width * it->size.height;
        if(area < mMinValidAreaPixels)
        {
            if (debugOut)
            {
                s << "(Invalid) Area " << area;
                cvPutText(debugOut,s.str().c_str(),cvPointFrom32f((*it).center),&font,CV_RGB(255,255,255));
                Zebulon::Vision::VisionUtils::DrawSquare(debugOut,*it,mNearColor);
            }
            continue;
        }
        //VisionUtils::DrawSquare(debugOut,*it,mNearColor);
        //horzontal line
        if (angle < -75. || angle > 75.)
        {
            if(!horizontalFound)
            {
                if (debugOut)
                {
                    s << "(Horizontal) " << angle;
                    cvPutText(debugOut,s.str().c_str(),cvPointFrom32f((*it).center),&font,CV_RGB(255,255,255));
                    Zebulon::Vision::VisionUtils::DrawSquare(debugOut,*it,mNearColor);
                }
                horizontalFound = true;
                horizontal = VisionUtils::VisionReturn(*it);
                horizontal.mValid = true;
            }
            //its possible 2 horizontal boxes were found, ignore the smallest one
            else
            {
                if(horizontal.mArea < (it->size.width * it->size.height))
                {
                    if (debugOut)
                    {
                        s << "(Horizontal) " << angle;
                        cvPutText(debugOut,s.str().c_str(),cvPointFrom32f((*it).center),&font,CV_RGB(255,255,255));
                        Zebulon::Vision::VisionUtils::DrawSquare(debugOut,*it,mNearColor);
                    }
                    horizontal = VisionUtils::VisionReturn(*it);
                }
            }
        }
        //line is vertical
        else if(angle > -15. && angle < 15.)
        {
            if(!verticalFound)
            {
                verticalFound = true;
                vertical = VisionUtils::VisionReturn(*it);
                vertical.mValid = true;
                if (debugOut)
                {
                    s << "(Vertical) " << angle;
                    cvPutText(debugOut,s.str().c_str(),cvPointFrom32f((*it).center),&font,CV_RGB(255,255,255));
                    Zebulon::Vision::VisionUtils::DrawSquare(debugOut,*it,mNearColor);
                }
            }
            //its possible 2 vertical boxes were found, ignore the smallest one
            else
            {
                if(vertical.mArea < (it->size.width * it->size.height))
                {
                    if (debugOut)
                    {
                        s << "(Vertical) " << angle;
                        cvPutText(debugOut,s.str().c_str(),cvPointFrom32f((*it).center),&font,CV_RGB(255,255,255));
                        Zebulon::Vision::VisionUtils::DrawSquare(debugOut,*it,mNearColor);
                    }
                    vertical = VisionUtils::VisionReturn(*it);
                }
            }
        }


    }
    return true;

}
