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
#include "ransac/RANSAC.h"
#include "ransac/Point2D.h"
#include "ransac/LineParamEstimator.h"
using namespace Zebulon;
using namespace Vision;

class LoveLaneObject : public VisionObject
{
public:
    LoveLaneObject();
    ~LoveLaneObject();
    virtual bool Initialize(const std::string& parameter);
    virtual IplImage* GetMask(const IplImage * imgIn, IplImage * debugOut = NULL) const;
    virtual std::list<CvBox2D> GetBounding(const IplImage * imgIn, IplImage * debugOut = NULL) const;
    bool GetVisionReturn(const IplImage * imgIn, Vision::Return& horizontal, Vision::Return& vertical, IplImage * debugOut) const;
protected:
    IplImage* ColorMask(const IplImage * imgIn) const;
    IplImage* GVColorMask(const IplImage * imgIn) const;
    IplImage* EdgeMask(const IplImage * imgIn) const;
    IplImage* HistogramMask(const IplImage * imgIn) const;
    IplImage* SegmentationMask(const IplImage * imgIn) const;
    IplImage * TemplateMask(const IplImage * imgIn, const IplImage * threshold, const IplImage * tmplt) const;
    bool RansacFit(const IplImage* imgIn, CxUtils::Segment3D& line, IplImage * mask,  const bool invertMask = true, const int lineWidth = 40) const;

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
    bool mEnableEdge;
    bool mEnableRansac;

    double mHistWeight;
    double mColorWeight;
    double mSegmentWeight;
    double mGVColorWeight;
    double mEdgeWeight;
    double mRansacWeight;
    double mMinValidAreaPixels;  ///< Varialbe to try to lower false positives when finding lines.

    bool mDebug;

    MaskOption::Vector mMaskOptions;
    Vision::Segment * mSeg;
    Vision::Threshhold * mThresh;
    Vision::ThreshRatio * mRatio;
    Vision::Target mNearTarget;
    Vision::Target mFarTarget;
};
