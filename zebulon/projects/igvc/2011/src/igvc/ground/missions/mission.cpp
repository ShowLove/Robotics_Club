
#include "igvc/ground/missions/mission.h"

using namespace Zebulon;
using namespace IGVC;

IGVC::GlobalInfo* Mission::mpGlobalInfo = NULL;
IGVC::GlobalCommand* Mission::mpGlobalCommand = NULL;

Mission::Mission()
{
    
}

Mission::~Mission()
{
    
}


void Mission::SetGlobalCommand(AI::GlobalCommand* globalCommand)
{
    mpGlobalCommand = (IGVC::GlobalCommand*)globalCommand;
}

void Mission::SetGlobalInfo(AI::GlobalInfo* globalInfo)
{
    mpGlobalInfo = (IGVC::GlobalInfo*)globalInfo;
}
