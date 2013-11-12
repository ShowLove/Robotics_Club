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
using namespace Zebulon;
using namespace Vision;


class BouyBaseObject : public VisionObject
{
public:
    BouyBaseObject();
    ~BouyBaseObject();
    virtual bool Initialize(const std::string& parameter);
    virtual IplImage* GetMask(const IplImage * imgIn, IplImage * debugOut=NULL) const;
    virtual std::list<CvBox2D> GetBounding(const IplImage * imgIn, IplImage * imgOut=NULL) const;
protected:
    IplImage * TemplateMask(const IplImage * imgIn, const IplImage * threshold, const IplImage * tmplt) const;
    IplImage* ColorMask(const IplImage * imgIn) const;
    IplImage* GVColorMask(const IplImage * imgIn) const;
    IplImage* EdgeMask(const IplImage * imgIn) const;
    IplImage* HistogramMask(const IplImage * imgIn) const;
    IplImage* SegmentationMask(const IplImage * imgIn) const;
    IplImage * FindCircles(const IplImage * imgIn) const;
    IplImage* ShapeMask(const IplImage * imgIn) const;
    void Clear();
    CvScalar mNearColor;
    CvScalar mFarColor;
    IplImage * mNearImage;
    IplImage * mFarImage;
    CvHistogram * mNearHist;
    CvHistogram * mFarHist;
    IplImage * mTemplate;
    Configuration::XmlConfigurator mXml;
    std::string mType;

    double mMainThreshold;
    double mMinNoiseSizePercent;
    double mTemplateThreshold;

    bool mEnableHist;
    bool mEnableColor;
    bool mEnableSegment;
    bool mEnableGVColor;

    double mHistWeight;
    double mColorWeight;
    double mSegmentWeight;
    double mGVColorWeight;

    bool mDebug;


    Vision::Segment * mSeg;
    Vision::Threshhold * mThresh;
    Vision::ThreshRatio * mRatio;
};
