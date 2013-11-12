#include "asvc/pyloncapture/pyloncapture.h"
#include <opencv/highgui.h>
using namespace Zebulon;
using namespace ASVC;

int main()
{
    PylonCapture capture;
    capture.Start(800,600);
    cv::Mat frame(800,600,CV_8UC3);
    while(1)
    {
        if(capture.GetFrame(frame) != 0)
        {
            cv::imshow("video",frame);
        }
        cv::waitKey(10);
    }
}
