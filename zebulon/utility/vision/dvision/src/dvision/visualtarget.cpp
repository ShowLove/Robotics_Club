#include "dvision/visualtarget.h"
#include "opencv/highgui.h"
#include <iostream>

using namespace Zebulon;
using namespace Vision;

VisualTarget::VisualTarget()
{
    SetID("VisualTarget");
    mTemplateFilename = "";
    mColor.red = mColor.green = mColor.blue = 0;
    mThreshold.red = mThreshold.green = mThreshold.blue = 0;
    mSize = 0;
    mDilate = 0;
    mThresholdAll = 0;
    mTemplate = NULL;
    mAlgorithm = 0;

    /*Add(mColor.red, "Color.red");
    Add(mColor.green, "Color.green");
    Add(mColor.blue, "Color.blue");
    Add(mThreshold.red, "Threshold.red");
    Add(mThreshold.green, "Threshold.green");
    Add(mThreshold.blue, "Threshold.blue");
    Add(mThresholdAll,"Threshold.all");
    Add(mSize,"Threshold.size");
    Add(mDilate,"Threshold.dilate");
    Add(mMinSize,"Threshold.minsize");
    Add(mTemplateFilename,"Template.filename");
    Add(mMinConfidence,"Template.minconfidence");
    Add(mAlgorithm,"Algorithm");*/
}

VisualTarget::VisualTarget(std::string id)
{
    mID = id;
    mTemplateFilename = "";
    mColor.red = mColor.green = mColor.blue = 0;
    mThreshold.red = mThreshold.green = mThreshold.blue = 0;
    mSize = 0;
    mDilate = 0;
    mThresholdAll = 0;
    mTemplate = NULL;
    mAlgorithm = 0;
}

VisualTarget::~VisualTarget()
{
    if(mTemplate) cvReleaseImage(&mTemplate);
}

void VisualTarget::SetID(std::string id)
{
    mID = id;
    //SetArchiveName(mID);
}

void VisualTarget::SetColor(const unsigned int r, const unsigned int g, const unsigned int b)
{
    mColor.red = r;
    mColor.green = g;
    mColor.blue = b;
}

void VisualTarget::SetThreshold(const unsigned int r, const unsigned int g, const unsigned int b)
{
    mThreshold.red = r;
    mThreshold.green = g;
    mThreshold.blue = b;
}

bool VisualTarget::Load(const std::string& filename)
{
    // TODO:: FILL IN XML STUFF BASED ON NEW STRUCTURE
    Configuration::XmlConfigurator* config = new Configuration::XmlConfigurator();
    config->Load(filename,"VisualTarget");
    
    int tmp;
    config->GetVar("VisualTarget.Color@Red", tmp);   mColor.red = tmp;
    config->GetVar("VisualTarget.Color@Green", tmp); mColor.green = tmp;
    config->GetVar("VisualTarget.Color@Blue", tmp);  mColor.blue = tmp;
    
    config->GetVar("VisualTarget.Threshold@Red", tmp);   mThreshold.red = tmp;
    config->GetVar("VisualTarget.Threshold@Green", tmp); mThreshold.green = tmp;
    config->GetVar("VisualTarget.Threshold@Blue", tmp);  mThreshold.blue = tmp;
    config->GetVar("VisualTarget.Threshold@All", mThresholdAll);
    config->GetVar("VisualTarget.Threshold@Size", mSize);
    config->GetVar("VisualTarget.Threshold@Dilate", mDilate);
    config->GetVar("VisualTarget.Threshold@MinSize", mMinSize);
    
    config->GetVar("VisualTarget.Template@Filename", mTemplateFilename);
    config->GetVar("VisualTarget.Template@MinConfidence", mMinConfidence);
    
    config->GetVar("VisualTarget.Algorithm@Algorithm", tmp); mAlgorithm = tmp;
    
    //std::cout << "IMAGE IMAGE IMAGE" << std::endl;
    if(mTemplateFilename.size() > 1)
    {
        //std::cout << "WE HAS AN IMAGE" << std::endl;
        IplImage * temp;
        CvBox2D box;
        if((temp = cvLoadImage(mTemplateFilename.c_str(),CV_LOAD_IMAGE_GRAYSCALE)) == 0)
        {
            std::cout << "Unable to load template image " << mTemplateFilename << "\n";
        }
        else
        {
            std::cout << "Success loading template image " << mTemplateFilename << "\n";
            
            //cvShowImage("To Compare",temp);
            //cvWaitKey(0);
            
            VisionUtils::ExtractBiggestBlob(temp,NULL,box,0,100000000);
            //std::cout << "box: height: " << box.size.height << " width: " << box.size.width << " centerx: " << box.center.x << " centery " << box.center.y << std::endl;
            this->mTemplate = Zebulon::Vision::VisionUtils::Crop(temp,box);
            cvShowImage("Template",this->mTemplate);
            cvReleaseImage(&temp);
            
            //mTemplate = temp;
            //cvShowImage("Template",mTemplate);
        }
    }
    
    delete config;
    
    return true;
}

std::string VisualTarget::ToString()
{
    char buffer[512];
    //std::string ret = "";
    sprintf(buffer,"Target: %s\nAlgorithm %d Color R %d G %d B %d\nThresh R %d G %d B %d All %d\nDilate %d Size %lf Template %s",
    mID.c_str(),mAlgorithm,mColor.red,mColor.green,mColor.blue,mThreshold.red,mThreshold.green,mThreshold.blue,mThresholdAll,mDilate,mSize,mTemplateFilename.c_str());
    return buffer;
}

