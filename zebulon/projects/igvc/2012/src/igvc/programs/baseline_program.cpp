#include "igvc/baseline/baseline.h"
using namespace Zebulon;

int main(int argc, char** argv)
{
    IGVC::Baseline baseline;
    bool success = false;
    if(argc > 1)
    {
        success = baseline.Initialize(argv[1]);
    }
    else
    {
        success = baseline.Initialize("settings/baseline.xml");
    }
    if(!success)
    {
        std::cout << "Unable to initialize Baseline" << std::endl;
        baseline.Shutdown();
        return 0;
    }
    // If waypoint list driver is shutdown, we need to shutdown
    // our program (for JAUS Challenge)
    while(baseline.GetLocalWaypointListDriverComponent()->ManagementService()->GetStatus() !=
          JAUS::Management::Status::Shutdown)
    {
        baseline.PrintStatus();
        baseline.Update();
        CxUtils::SleepMs(100);
    }
    baseline.Shutdown();
    return 0;
}
