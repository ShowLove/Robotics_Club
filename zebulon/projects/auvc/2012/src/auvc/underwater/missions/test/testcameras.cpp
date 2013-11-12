#include <auvc/underwater/missions/test/testcameras.h>
#include <dvision/visionutils.h>

using namespace Zebulon;
using namespace Underwater;

TestCameras::TestCameras()
{
    ID = "TestCameras";
    
    mForwardFrame = NULL;
    mDownwardFrame = NULL;
}

TestCameras::~TestCameras()
{
    cvReleaseImage(&mForwardFrame);
    cvReleaseImage(&mDownwardFrame);
}

int TestCameras::Init()
{
    mState = DisplayCameras;
    return 0;
}

int TestCameras::ErrorCheck()
{
    return 0;
}

int TestCameras::FetchProcess()
{
    
    // Forward
    if(!mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera, mForwardFrame))
    {
        printf("ERROR::TestCameras: Couldn't get forward camera data");
        return 1;
    }
    
    if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDownwardFrame))
    {
        printf("ERROR::TestCameras: Couldn't get downward camera data");
        return 1;
    }
    
    return 0;
}

std::string TestCameras::ExitEarly()
{
    return "KeepRunning";
}

std::string TestCameras::Execute()
{
    switch (mState)
    {
        case DisplayCameras:

        //std::cout << "Forward Camera Info: " << std::endl;
        //std::cout << "Width: " << mForwardFrame->width << ", Height: " << mForwardFrame->height << std::endl;
        //std::cout << "Downward Camera Info: " << std::endl;
        //std::cout << "Width: " << mDownwardFrame->width << ", Height: " << mDownwardFrame->height << std::endl;

            Utility::Display::DisplayImage("RawForward",mForwardFrame);
            Utility::Display::DisplayImage("RawDownward",mDownwardFrame);
            break;
    }
    
    return "KeepRunning";
}
