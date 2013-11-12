#include "igvc/ground/statemachine.h"

using namespace Zebulon;
using namespace IGVC;

StateMachine::StateMachine()
{
}

StateMachine::~StateMachine()
{

}

bool StateMachine::AddMissions()
{
    bool missionsResult = true;
    //Zebulon::AI::Mission::LoadWgsWaypointsFromFile("settings/waypoints/navigation_challenge.xml");

    //missionsResult &= AddMission(new LaneLineMission());
    //missionsResult &= AddMission(new TestMission());
    //missionsResult &= AddMission(new CartographerBeam());
    //missionsResult &= AddMission(new WayPointList());
    //missionsResult &= AddMission(new Persistent());
    //missionsResult &= AddMission(new Autonomous());
    //missionsResult &= AddMission(new AutonomousSimple());
    //missionsResult &= AddMission(new Navigation());
    missionsResult &= AddMission(new TemplateMission());

    return missionsResult;
}
