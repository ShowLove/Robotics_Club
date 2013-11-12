#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cxutils/cxutils.h>
#include <vector>
#include "dvision/visionutils.h"
#include "dvision/visionobject.h"
#include "xmlconfig/xmlconfigurator.h"
#include "gvision/segment.h"
#include "gvision/threshhold.h"
#include "gvision/threshratio.h"
#include "gvision/visionutil.h"

#ifndef __ZEBULON_UNDERWATER_VISION_BUOYOBJECT_
#define __ZEBULON_UNDERWATER_VISION_BUOYOBJECT_

using namespace Zebulon;
using namespace Vision;

class BouyObject : public VisionObject
{
public:
    BouyObject();
    ~BouyObject();
    virtual bool Initialize(const std::string& parameter);
    virtual IplImage* GetMask(const IplImage * imgIn, IplImage * debugOut=NULL) const;
    virtual std::list<CvBox2D> GetBounding(const IplImage * imgIn, IplImage * debugOut=NULL) const;
    bool GetVisionReturn(const IplImage * imgIn, Vision::Return& result, IplImage * debugOut) const;
protected:
    IplImage * TemplateMask(const IplImage * imgIn, const IplImage * threshold, const IplImage * tmplt) const;
    IplImage* ColorMask(const IplImage * imgIn) const;
    IplImage* GVColorMask(const IplImage * imgIn) const;
    IplImage* EdgeMask(const IplImage * imgIn) const;
    IplImage* HistogramMask(const IplImage * imgIn) const;
    IplImage* SegmentationMask(const IplImage * imgIn) const;
    IplImage* SegmentationMask2(const IplImage * imgIn, IplImage * debugOut = NULL) const;
    IplImage * FindCircles(const IplImage * imgIn) const;
    IplImage* ShapeMask(const IplImage * imgIn) const;
    void Clear();
    CvScalar mNearColor;
    CvScalar mFarColor;
    IplImage * mNearImage;
    IplImage * mFarImage;
    CvHistogram * mNearHist;
    CvHistogram * mFarHist;
    IplImage * mBouyTemplate;
    Configuration::XmlConfigurator mXml;
    std::string mType;

    double mMainThreshold;
    double mMinNoiseSizePercent;
    double mTemplateThreshold;
    double mSegment2Threshold;

    bool mEnableHist;
    bool mEnableColor;
    bool mEnableSegment;
    bool mEnableGVColor;
    bool mEnableSegment2;

    double mHistWeight;
    double mColorWeight;
    double mSegmentWeight;
    double mGVColorWeight;
    double mSegmentWeight2;

    bool mDebug;

    MaskOption::Vector mMaskOptions;

    Vision::Segment * mSeg;
    Vision::Threshhold * mThresh;
    Vision::ThreshRatio * mRatio;
    Vision::Target mNearTarget;
    Vision::Target mFarTarget;
};

#endif // __ZEBULON_UNDERWATER_VISION_BUOYOBJECT_
/* End of file */

