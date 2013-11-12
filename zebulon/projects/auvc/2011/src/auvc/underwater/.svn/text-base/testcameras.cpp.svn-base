#include <auvc/underwater/testcameras.h>
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
    if(!mGlobalInfo->GetInfo("ForwardCamera", mForwardFrame))
    {
        printf("ERROR::TestTracking: Couldn't get forward camera data");
        return 1;
    }
    
    if(!mGlobalInfo->GetInfo("DownwardCamera", mDownwardFrame))
    {
        printf("ERROR::TestTracking: Couldn't get forward camera data");
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
            Utility::Display::DisplayImage("RawForward",mForwardFrame);
            Utility::Display::DisplayImage("RawDownward",mDownwardFrame);
            break;
    }
    
    return "KeepRunning";
}
