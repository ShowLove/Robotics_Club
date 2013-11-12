#include "auvc/underwater/statemachine.h"

//Calibration Fake Mission
#include "auvc/underwater/missions/calibrate/calibrate.h"
#include "auvc/underwater/missions/calibrate/calibratecassie.h"
#include "auvc/underwater/missions/test/fakemission.h"
// Test Misisons
#include "auvc/underwater/missions/test/testmission.h"
#include "auvc/underwater/missions/test/testtracking.h"
#include "auvc/underwater/missions/test/testlasertracking.h"
#include "auvc/underwater/missions/test/testpath.h"
#include "auvc/underwater/missions/test/testcameras.h"
#include "auvc/underwater/missions/test/testsearch.h"
// Missions
#include "auvc/underwater/missions/pinger.h"
#include "auvc/underwater/missions/startbutton.h"
#include "auvc/underwater/missions/validationgate.h"
#include "auvc/underwater/missions/visionvalidationgate2.h"
#include "auvc/underwater/missions/buoys.h"
#include "auvc/underwater/missions/buoys2.h"
#include "auvc/underwater/missions/buoysdavid.h"
#include "auvc/underwater/missions/buoysclean.h"
#include "auvc/underwater/missions/buoyscleanlaser.h"
#include "auvc/underwater/missions/lovelane.h"
#include "auvc/underwater/missions/lovelanewidth.h"
#include "auvc/underwater/missions/torpedo.h"
#include "auvc/underwater/missions/dropper.h"
#include "auvc/underwater/missions/2012/training.h"
// 2012
#include "auvc/underwater/missions/2012/startswitch.h"
#include "auvc/underwater/missions/2012/obstaclecourse.h"
#include "auvc/underwater/missions/2012/ettubrute.h"
#include "auvc/underwater/missions/2012/wreath.h"

#include "auvc/underwater/missions/2012/simplestartgate.h"
#include "auvc/underwater/missions/2012/startgate.h"

#include "auvc/underwater/missions/2012/training.h"

#include "auvc/underwater/missions/test/joystick.h"

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
//        missionsResult &= AddMission(new Training());
//    missionsResult &= AddMission(new TestMission());
    //missionsResult &= AddMission(new TestTracking());
    //missionsResult &= AddMission(new TestLaserTracking());
//    missionsResult &= AddMission(new TestPath());
    //missionsResult &= AddMission(new Joystick());
//    missionsResult &= AddMission(new TestCameras());
//    missionsResult &= AddMission(new TestSearch());

    // 2012
    missionsResult &= AddMission(new StartSwitch());
    missionsResult &= AddMission(new StartGate());
    missionsResult &= AddMission(new Training());
    missionsResult &= AddMission(new ObstacleCourse());
    missionsResult &= AddMission(new EtTuBrute());
    missionsResult &= AddMission(new Wreath());

    //missionsResult &= AddMission(new SimpleStartGate());

    // Real Missions
//    missionsResult &= AddMission(new StartButton());
    //missionsResult &= AddMission(new ValidationGate());
    //missionsResult &= AddMission(new VisionValidationGate2());
    //missionsResult &= AddMission(new Buoys());
    //missionsResult &= AddMission(new Buoys2());
//    missionsResult &= AddMission(new BuoysDavid());
    //missionsResult &= AddMission(new BuoysClean());
    //missionsResult &= AddMission(new BuoysCleanLaser());
    //missionsResult &= AddMission(new LoveLane());
    //missionsResult &= AddMission(new LoveLaneWidth());
    //missionsResult &= AddMission(new Torpedo());
    //missionsResult &= AddMission(new Dropper());
    //missionsResult &= AddMission(new Pinger());
    
    // Sentinal Rules
    //missionsResult &= AddMission(new Sentinal_TurnStuck());
    //missionsResult &= AddMission(new Sentinal_DepthStuck());

    return missionsResult;
}
