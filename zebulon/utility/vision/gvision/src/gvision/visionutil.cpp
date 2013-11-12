#include "gvision/visionutil.h"


using namespace Zebulon;
using namespace Vision;

bool VisionUtil::ResetImage(IplImage *Image)
{
    if(Image==NULL)
    {
        return false;
    }
    else
    {
        memset(Image->imageData,0,Image->imageSize);
    }
}
bool VisionUtil::CombineImages(IplImage *InputImage, IplImage *OutputImage)
{
    bool result = false;
    if(InputImage == NULL) return result;
    //manual copy
    // copy red seg to proc with green
    for(int i = 0; i < InputImage->imageSize; i++)
    {
        if(InputImage->imageData[i] != 0)
        {
            OutputImage->imageData[i] = InputImage->imageData[i];
            result = true;
        }
    }
    return result;
}
/*  End of File */
