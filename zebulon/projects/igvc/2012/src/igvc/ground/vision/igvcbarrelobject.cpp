#include "ground/vision/igvcbarrelobject.h"
using namespace Zebulon;
using namespace Vision;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initialize with xml file. Colors, thresholds etc.
///
///   \param[in] xmlFilename path and filename of xml file with settings.
///
///   \return true on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool IGVCBarrelObject::Initialize(const std::string& xmlFilename)
{
    TiXmlDocument doc(xmlFilename.c_str());

    if(doc.LoadFile())
    {
        TiXmlHandle handle(&doc);
        TiXmlElement * element = handle.FirstChild("VisionObjects").FirstChild("IGVCBarrelObject").ToElement();
        if(element)
        {
            TiXmlElement * color = element->FirstChild()->ToElement();
            while(color)
            {
                ColorSetting c;
                if(c.Initialize(color))
                {
                     mColors.push_back(c);
                }
                color = color->NextSiblingElement();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a binary mask of original image.  1 pixels are where object
///          is found, 0 where object is not found. Searches for all colors.
///
///   \param[in] ImgIn Image to process.
///
///   \return Binary mask.
///
////////////////////////////////////////////////////////////////////////////////////
IplImage* IGVCBarrelObject::GetMask(const IplImage * imgIn) const
{
    IplImage * result = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    cvZero(result);
    for(int i = 0; i < mColors.size(); i++)
    {
        IplImage  * temp = GetMask(imgIn,mColors[i]);
        cvOr(result,temp,result);
        //cvShowImage("result",result);
        cvReleaseImage(&temp);
    }
    cvShowImage("mask",result);
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Finds bounding boxes around objects using all known colors. boxes can
///          rotate with object for best fit.  Min/max box size can be specified.
///
///   \param[in] ImgIn Image to process.
///
///   \return A list of boxes surrounding found objects.
///
////////////////////////////////////////////////////////////////////////////////////
std::list<CvBox2D> IGVCBarrelObject::GetBounding(const IplImage * imgIn) const
{

    std::list<CvBox2D> result;
    for(int i = 0; i < mColors.size(); i++)
    {
        std::list<CvBox2D> temp;
        temp = GetBounding(imgIn,mColors[i]);
        for (std::list<CvBox2D>::iterator it = temp.begin(); it != temp.end(); it++)
           result.push_back(*it);
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds a color sample to known colors.  Will load the image file and
///          average it to get color. Not robust.
///
///   \param[in] ImgIn Image that contains a color sample
///
///   \return True if image is loaded, false if not.
///
////////////////////////////////////////////////////////////////////////////////////
bool IGVCBarrelObject::AddColorSetting(const std::string& fileName)
{
    ColorSetting color;
    if(color.SetColor(fileName))
    {
        mColors.push_back(color);
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a binary mask of original image.  1 pixels are where object
///          is found, 0 where object is not found. Searches for specific color.
///
///   \param[in] ImgIn Image to process.
///   \param[in] color RGB Color to search.
///
///   \return Binary mask.
///
////////////////////////////////////////////////////////////////////////////////////
//IplImage * IGVCBarrelObject::GetMask(const IplImage * imgIn, CvScalar color) const
//{
//     return GetMask(imgIn, color.val[0],color.val[1],color.val[2],);
//}

IplImage * IGVCBarrelObject::GetMask(const IplImage * imgIn, ColorSetting color) const
{
    return GetMask(imgIn, color.mColor.val[0], color.mColor.val[1], color.mColor.val[2]
                   ,color.mThreshold, color.mMinSizePercent, color.mMaxSizePercent);
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Finds bounding boxes around objects using specific color. boxes can
///          rotate with object for best fit.  Min/max box size can be specified.
///
///   \param[in] ImgIn Image to process.
///   \param[in] color RGB Color to search.
///
///   \return A list of boxes surrounding found objects.
///
////////////////////////////////////////////////////////////////////////////////////
//std::list<CvBox2D> IGVCBarrelObject::GetBounding(const IplImage * imgIn, CvScalar color) const
//{
//    return GetBounding(imgIn, color.val[0],color.val[1],color.val[2]);
//}


std::list<CvBox2D> IGVCBarrelObject::GetBounding(const IplImage * imgIn, ColorSetting color) const
{
    return GetBounding(imgIn, color.mColor.val[0], color.mColor.val[1], color.mColor.val[2]
                       ,color.mThreshold, color.mMinSizePercent, color.mMaxSizePercent);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Finds bounding boxes around objects using specific color. boxes can
///          rotate with object for best fit.  Min/max box size can be specified.
///
///   \param[in] ImgIn Image to process.
///   \param[in] r red
///   \param[in] g green
///   \param[in] b blue
///
///   \return A list of boxes surrounding found objects.
///
////////////////////////////////////////////////////////////////////////////////////
std::list<CvBox2D> IGVCBarrelObject::GetBounding(const IplImage * imgIn, const int r,  const int g,  const int b, const int threshold, const double minsizepercent, const double maxsizepercent) const
{
    IplImage * temp = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    //rgb reversed in extract color, not intuitive
    Vision::VisionUtils::ExtractColor(imgIn,temp,CV_RGB(b,g,r),threshold);
    //cvShowImage("bounding", temp);
    //cvWaitKey(0);
    std::list<CvBox2D> blobs = Vision::VisionUtils::GetBlobBoxes(temp,minsizepercent,maxsizepercent);
    for(std::list<CvBox2D>::iterator i = blobs.begin(); i != blobs.end(); i++)
    {
        CvBox2D rotatedBox = *i;
        rotatedBox.angle += 90;
        Vision::VisionUtils::DrawSquare(temp,rotatedBox);
    }
    blobs = Vision::VisionUtils::GetBlobBoxes(temp,minsizepercent,maxsizepercent);
    cvReleaseImage(&temp);
    return blobs;

}
///////////////////////////////////////////////////// ///////////////////////////////
///
///   \brief Returns a binary mask of original image.  1 pixels are where object
///          is found, 0 where object is not found. Searches for specific color.
///
///   \param[in] ImgIn Image to process.
///   \param[in] r red
///   \param[in] g green
///   \param[in] b blue
///
///   \return Binary mask.
///
////////////////////////////////////////////////////////////////////////////////////
IplImage *  IGVCBarrelObject::GetMask(const IplImage * imgIn, const int r,  const int g,  const int b, const int threshold, const double minsizepercent, const double maxsizepercent) const
{
    IplImage * temp = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    cvZero(temp);
    std::list<CvBox2D> blobs = GetBounding(imgIn,r,g,b,threshold, minsizepercent, maxsizepercent);
    for(std::list<CvBox2D>::iterator i = blobs.begin(); i != blobs.end(); i++)
    {
        CvPoint2D32f points[4];
        cvBoxPoints(*i,points);
        CvPoint point2d[4];
        for(int i = 0; i < 4; i++)
        {
            point2d[i] = cvPointFrom32f(points[i]);
        }
        cvFillConvexPoly(temp,point2d,4,cvScalar(255,255,255));
        //cvDrawRect(temp,cvPoint(0,0),cvPoint(temp->width,temp->height),cvScalar(255,255,255),50);
    }
    return temp;
}
