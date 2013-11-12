#include <auvc/underwater/path.h>
#include <statemachine/utility/utility.h>

using namespace Zebulon;
using namespace Underwater;

#define AUVC_PATH_COLORS
//#define AUVC_PATH_BOXES

double PathFinder::ImageWidth = 640;
double PathFinder::ImageHeight = 480;

PathFinder::PathFinder()
{
    PathFinder::Initialize();
}

PathFinder::~PathFinder()
{
}

/*<Color Red="211" Green="151" Blue="122" />
 * <ColorThresh Red="14" Green="30" Blue="191" />
 */
void PathFinder::Initialize()
{
    mSegment = new Vision::Segment();
    //mThreshold =  new Vision::Threshhold();
    mThreshold = new Vision::ThreshRatio();
    
    /*mOrangePath.mRed = 211;
    mOrangePath.mGreen = 151;
    mOrangePath.mBlue = 122;
    
    mOrangePath.mRedRange = 14;
    mOrangePath.mGreenRange = 30;
    mOrangePath.mBlueRange = 191;*/
    
    mSegFrame = NULL;
    mThreshFrame = NULL;
    mProcessedImage = NULL;
    
    /*mTargetRatio = .8;
    mTargetRatioSigma = .05;*/
    
    /*mTargetMinArea = 150;
    mTargetMaxArea = 10000;*/
    
    //mMaxPaths = 4;
    
    //mDefaultDepth = 4;
    
    //mBoxMinSize = .01;
    //mBoxMaxSize = .1;
    
    //mBoxCannyThresh = 50;
    
    //mCenterIThresh = 50;
    //mCenterJThresh = 50;
    
    //mAngleCloseThresh = 5.0;
    
    //mPixelScale = 20.0/640.0;
    
    Mission::mXmlConf.GetVar("Underwater.Path.Orange@Red", mOrangePath.mRed);
    Mission::mXmlConf.GetVar("Underwater.Path.Orange@Green", mOrangePath.mGreen);
    Mission::mXmlConf.GetVar("Underwater.Path.Orange@Blue", mOrangePath.mBlue);
    Mission::mXmlConf.GetVar("Underwater.Path.OrangeThresh@Red", mOrangePath.mRedRange);
    Mission::mXmlConf.GetVar("Underwater.Path.OrangeThresh@Green", mOrangePath.mGreenRange);
    Mission::mXmlConf.GetVar("Underwater.Path.OrangeThresh@Blue", mOrangePath.mBlueRange);
    
    Mission::mXmlConf.GetVar("Underwater.Path.Target@Ratio", mTargetRatio);
    Mission::mXmlConf.GetVar("Underwater.Path.Target@RatioSigma", mTargetRatioSigma);
    
    Mission::mXmlConf.GetVar("Underwater.Path.Target@MinArea", mTargetMinArea);
    Mission::mXmlConf.GetVar("Underwater.Path.Target@MaxArea", mTargetMaxArea);
    Mission::mXmlConf.GetVar("Underwater.Path.Target@CenterIThresh", mCenterIThresh);
    Mission::mXmlConf.GetVar("Underwater.Path.Target@CenterJThresh", mCenterJThresh);
    Mission::mXmlConf.GetVar("Underwater.Path.Target@AngleCloseThresh", mAngleCloseThresh);
    Mission::mXmlConf.GetVar("Underwater.Path.Target@PathAreaDeviationThresh", mPathAreaDeviationThresh);
    
    Mission::mXmlConf.GetVar("Underwater.Path.Limits@MaxPaths", mMaxPaths);
    
    Mission::mXmlConf.GetVar("Underwater.Path.Default@Depth", mDefaultDepth);
    
    Mission::mXmlConf.GetVar("Underwater.Path.BoxFinding@MinSize", mBoxMinSize);
    Mission::mXmlConf.GetVar("Underwater.Path.BoxFinding@MaxSize", mBoxMaxSize);
    double val;
    Mission::mXmlConf.GetVar("Underwater.Path.BoxFinding@CannyThresh", val);
    mBoxCannyThresh = (int)val;
    
    double num, denom;
    Mission::mXmlConf.GetVar("Underwater.Path.LatPixelScale@Num", num);
    Mission::mXmlConf.GetVar("Underwater.Path.LatPixelScale@Denom", denom);
    mLatPixelScale = num/denom;
    Mission::mXmlConf.GetVar("Underwater.Path.AxialPixelScale@Num", num);
    Mission::mXmlConf.GetVar("Underwater.Path.AxialPixelScale@Denom", denom);
    mAxialPixelScale = num/denom;
    
    Mission::mXmlConf.GetVar("Underwater.Path.Cameras@FrontCameraOffsetJ", mFrontPathOffsetJ);
    
    mPathObject.Initialize("Path");
}

/** Gets all visible paths, in order of confidence */
std::vector<Path> PathFinder::GetPaths(const Controls current, IplImage* image, IplImage * output)
{
    std::vector<Path> paths;
    
    mMutex.Lock();
    PathFinder::ImageWidth = image->width;
    PathFinder::ImageHeight = image->height;
    mMutex.Unlock();
    
    // Process the image...
    if(output != NULL)
    {
        //output = cvCreateImage(cvSize(image->width,image->height), image->depth, image->nChannels);
        Vision::VisionUtil::ResetImage(output);
    }
    
    mSegment->Setup(mOrangePath, 200);
    mThreshold->Setup(mOrangePath);
    
    mThreshold->Process(image, mThreshFrame);
    //cvRectangle(mThreshFrame, cvPoint(0,0), cvPoint(640, 100), cvScalar(0,0,0), -1);
    
    mSegment->Process(mThreshFrame, mSegFrame);
    
    Vision::VisionUtil::CombineImages(mSegFrame, output);
    
    // Loop Through
    double confidence;
    mSegment->GetMaxPosition(mOrangeReturn);
    for (int i=0; mOrangeReturn.mValid && i < mMaxPaths; ++i, mSegment->GetMaxPosition(mOrangeReturn, i))
    {
        // Determine confidence values...
        confidence = 1 - (mOrangeReturn.mArea*2 - mTargetMaxArea + mTargetMinArea)/(mTargetMaxArea - mTargetMinArea);
        
        // If it's okay, then add it to the list...
        if (mOrangeReturn.mArea >= mTargetMinArea
            && mOrangeReturn.mArea <= mTargetMaxArea)
        {
            Path p;
            p.mAngle = mOrangeReturn.mAngle;
            p.mI = mOrangeReturn.mCenterI;
            p.mJ = mOrangeReturn.mCenterJ;
            
            p.mConfidence = confidence;
            
            if (output !=NULL)
            {
                cvRectangle(output, cvPoint(mOrangeReturn.mMinI, mOrangeReturn.mMinJ), cvPoint(mOrangeReturn.mMaxI, mOrangeReturn.mMaxJ), cvScalar(255,0,0), 3);
                //cvLine(Output, cvPoint(mOrangeReturn.mCenterI, mOrangeReturn.mCenterJ), cvPoint(mOrangeReturn.mCenterI + 50*sin(mOrangeReturn.mAngle), mOrangeReturn.mCenterJ + 50*cos(mOrangeReturn.mAngle)), cvScalar(255,0,0));
            }
            
            paths.push_back(p);
        }
    }
    
    if (paths.size() > 1 && fabs((paths[0].mArea - paths[1].mArea)/2.0) < mPathAreaDeviationThresh)
    {
        //cvCircle(Output, cvPoint(Output->width/2,Output->height/2), 100, cvScalar(255,0,255), 4);
        
        Path p0 = paths[0];
        Path p1 = paths[1];
        
        //std::cout << "pre  p0: " << p0.mAngle << " p1: " << p1.mAngle << std::endl;
        
        if (fabs(AI::Utility::AngleDiff(p0.mAngle, p1.mAngle)) > 90)
        {
            if (p1.mJ > p0.mJ)
            {
                p1.mAngle = AI::Utility::AngleDiff(p0.mAngle, 180);
            }
            else
            {
                p0.mAngle = AI::Utility::AngleDiff(p1.mAngle, 180);
            }
        }
        
        if (p1.mAngle > p0.mAngle)
        {
            paths.clear();
            paths.push_back(p1);
            paths.push_back(p0);
        }
        
        //std::cout << "post p0: " << p0.mAngle << " p1: " << p1.mAngle << std::endl;
    }
    
    if (output != NULL && paths.size() > 0)
    {
        cvCircle(output, cvPoint(paths[0].mI,paths[0].mJ), 50, cvScalar(0,0,255), 2);
    }
    
    for (int i=0; i<paths.size(); i++)
    {
        paths[i].mAngle = CxUtils::Orientation::AddToAngle(current.mYaw, paths[i].mAngle,false);
    }
    // List needs to be sorted at some point...
    return paths;
}

/** Gets all visible paths, in order of confidence */
std::vector<Path> PathFinder::GetPathsBox(IplImage* image, IplImage* output)
{
    std::vector<Path> paths;
    
    mMutex.Lock();
    PathFinder::ImageWidth = image->width;
    PathFinder::ImageHeight = image->height;
    mMutex.Unlock();
    
    // Process the image...
    if(output != NULL)
    {
        //Output = cvCreateImage(cvSize(image->width,image->height), 8, 3);
        Vision::VisionUtil::ResetImage(output);
    }
    
    mSegment->Setup(mOrangePath, 50);
    mThreshold->Setup(mOrangePath);
    
    mThreshold->Process(image, mThreshFrame);
    mSegment->Process(mThreshFrame, mSegFrame);
    
    //Vision::VisionUtil::CombineImages(mSegFrame, Output);
    
    std::list<CvBox2D> list = Zebulon::Vision::DVision::FindSquares(mThreshFrame, output, mBoxMinSize, mBoxMaxSize, mBoxCannyThresh, true);
    
    // Loop Through
    double confidence, ratio;
    std::list<CvBox2D>::const_iterator iter;
    for (iter=list.begin(); iter != list.end(); ++iter)
    {
        // Determine confidence values...
        if (iter->size.width < iter->size.height)
        {
            confidence = fabs((iter->size.width/iter->size.height - mTargetRatio)/mTargetRatioSigma);
        }
        else
        {
            confidence = fabs((iter->size.height/iter->size.width - mTargetRatio)/mTargetRatioSigma);
        }
        
        // If it's okay, then add it to the list...
        if (confidence < 2*mTargetRatioSigma)
        {
            Path p;
            p.mAngle = iter->angle;
            p.mI = iter->center.x;
            p.mJ = iter->center.y;
            p.mRatio = confidence * mTargetRatioSigma;
            
            p.mConfidence = 1 - confidence;
            
            paths.push_back(p);
        }
    }
    
    // List needs to be sorted at some point...
    
    return paths;
}

std::vector<Path> PathFinder::GetPathsDavid(const Controls current, IplImage* image, IplImage * output)
{
    std::vector<Vision::Return> objects = mPathObject.GetVisionReturn(image, output);
    std::vector<Path> paths;
    std::vector<Vision::Return>::const_iterator iter;
    for (iter = objects.begin(); iter != objects.end(); iter++)
    {
        Path p;
        p.mI = iter->mCenterI;
        p.mJ = iter->mCenterJ;
        p.mAngle = CxUtils::Orientation::AddToAngle(current.mYaw, iter->mAngle, false);
        p.mArea = iter->mArea;
        paths.push_back(p);
    }
    
    /*std::list<CvBox2D> objects = mPathObject.GetBounding(image, output);
    //std::list<CvBox2D> objects = mPathObject.GetBounding(image);
    std::vector<Path> paths;
    std::list<CvBox2D>::const_iterator iter;
    int i=1;
    for (iter = objects.begin(); iter != objects.end(); iter++)
    {
        Path p;
        p.mI = iter->center.x;
        p.mJ = iter->center.y;
        p.mAngle = CxUtils::Orientation::AddToAngle(current.mYaw, iter->angle, false);
            //CxUtils::Orientation::AddToAngle(current.mYaw, CxUtils::Orientation::AngleDiff(90, iter->angle, false),false);
        p.mArea = iter->size.width * iter->size.height;
        std::cout << i++ << "i: " << p.mI << " j: " << p.mJ << " ang " << p.mAngle << std::endl;
        paths.push_back(p);
        
        if (output != NULL)
        {
            double angle = CxUtils::CxToRadians(p.mAngle);
            //CxUtils::CxToRadians(CxUtils::Orientation::AngleDiff(90, p.mAngle, false));
            cvCircle(output, cvPoint(p.mI,p.mJ), 50, cvScalar(0,0,255), 2);
            cvLine(output, cvPoint(p.mI, p.mJ), 
                           cvPoint(p.mI + 50*sin(angle),
                                   p.mJ - 50*cos(angle)),
                           cvScalar(255,0,255));
        }
    }
    /*
    if (output != NULL && paths.size() > 0)
    {
        cvCircle(output, cvPoint(paths[0].mI,paths[0].mJ), 50, cvScalar(0,0,255), 2);
        cvLine(output, cvPoint(paths[0].mI, paths[0].mJ), 
//                       cvPoint(paths[0].mI + 50*cos(paths[0].mAngle),
//                               paths[0].mJ + 50*sin(paths[0].mAngle)),
                       cvPoint(output->width/2 + 50*cos(paths[0].mAngle),
                               output->height/2 + 50*sin(paths[0].mAngle)),
                       cvScalar(255,0,255));
    }*/
    
    return paths;
}

/** Align to the given path */
bool PathFinder::StepPath(const Path* path, const Controls input, Controls &output)
{
    bool ready = false;
    
    output = input;
    
    output.mDepthEnable = true;
    output.mDepth = mDefaultDepth;
    
    mMutex.Lock();
    if (path->mI > PathFinder::ImageWidth/2-mCenterIThresh
        && path->mI < PathFinder::ImageWidth/2+mCenterIThresh
        && path->mJ > PathFinder::ImageHeight/2-mCenterJThresh
        && path->mJ < PathFinder::ImageHeight/2+mCenterJThresh)
    {
        output.mYawEnable = true;
        output.mYaw = path->mAngle;
        
        output.mAxialThrustEnable = false;
        output.mLateralThrustEnable = false;
        
        if (fabs(input.mYaw - path->mAngle) < mAngleCloseThresh)
        {
            ready = true;
        }
    }
    else
    {
        output.mAxialThrustEnable = true;
        output.mAxialThrust = (PathFinder::ImageHeight/2 - path->mJ)*mAxialPixelScale;
        
        output.mLateralThrustEnable = true;
        output.mLateralThrust = (path->mI - PathFinder::ImageWidth/2)*mLatPixelScale;
    }
    mMutex.Unlock();
    
    return ready;
}