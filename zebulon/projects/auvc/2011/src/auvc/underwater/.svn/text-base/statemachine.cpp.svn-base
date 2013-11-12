#include "auvc/underwater/statemachine.h"

//Calibration Fake Mission
#include "auvc/underwater/calibrate.h"
#include "auvc/underwater/calibratecassie.h"
#include "auvc/underwater/fakemission.h"
// Test Misisons
#include "auvc/underwater/testmission.h"
#include "auvc/underwater/testtracking.h"
#include "auvc/underwater/testlasertracking.h"
#include "auvc/underwater/testpath.h"
#include "auvc/underwater/testcameras.h"
// Missions
#include "auvc/underwater/pinger.h"
#include "auvc/underwater/startbutton.h"
#include "auvc/underwater/validationgate.h"
#include "auvc/underwater/visionvalidationgate2.h"
#include "auvc/underwater/buoys.h"
#include "auvc/underwater/buoys2.h"
#include "auvc/underwater/buoysdavid.h"
#include "auvc/underwater/buoysclean.h"
#include "auvc/underwater/buoyscleanlaser.h"
#include "auvc/underwater/lovelane.h"
#include "auvc/underwater/lovelanewidth.h"
#include "auvc/underwater/torpedo.h"
#include "auvc/underwater/dropper.h"

#include "auvc/underwater/joystick.h"

//#include "auvc/underwater/pinger"
// Sentinal Rules


using namespace Zebulon;
using namespace Underwater;

StateMachine::StateMachine()
{
}

StateMachine::~StateMachine()
{
    
}

bool StateMachine::AddMissions()
{
    bool missionsResult = true;
    // Calibration Missions
    //missionsResult &= AddMission(new Calibrate());
    //missionsResult &= AddMission(new CalibrateCassie());
    //missionsResult &= AddMission(new FakeMission());
    
    // Test Missions
    //missionsResult &= AddMission(new TestMission());
    //missionsResult &= AddMission(new TestTracking());
    //missionsResult &= AddMission(new TestLaserTracking());
    //missionsResult &= AddMission(new TestPath());
    //missionsResult &= AddMission(new Joystick());
    //missionsResult &= AddMission(new TestCameras());
    
    // Real Missions
    missionsResult &= AddMission(new StartButton());
    //missionsResult &= AddMission(new ValidationGate());
    missionsResult &= AddMission(new VisionValidationGate2());
    //missionsResult &= AddMission(new Buoys());
    //missionsResult &= AddMission(new Buoys2());
    //missionsResult &= AddMission(new BuoysDavid());
    //missionsResult &= AddMission(new BuoysClean());
    missionsResult &= AddMission(new BuoysCleanLaser());
    //missionsResult &= AddMission(new LoveLane());
    missionsResult &= AddMission(new LoveLaneWidth());
    missionsResult &= AddMission(new Torpedo());
    //missionsResult &= AddMission(new Dropper());
    missionsResult &= AddMission(new Pinger());
    
    // Sentinal Rules
    //missionsResult &= AddMission(new Sentinal_TurnStuck());
    //missionsResult &= AddMission(new Sentinal_DepthStuck());
    
    return missionsResult;
}
