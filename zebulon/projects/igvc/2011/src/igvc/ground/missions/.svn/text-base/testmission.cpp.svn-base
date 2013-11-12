#include "igvc/ground/missions/testmission.h"

using namespace Zebulon;
using namespace IGVC;

TestMission::TestMission()
{
    mSendVectorFlag = 0;
    mCurrentVelocity = mDesiredVelocity = 0;
}

TestMission::~TestMission()
{

}

int TestMission::Init()
{
    mSendVectorFlag = 0;
    mCurrentVelocity = mDesiredVelocity = 0;

    return 0;
}

int TestMission::FetchProcess()
{
    double heading=0;
    if(mXmlConf.GetVar("Ground.TestMission.Vector@Velocity", mDesiredVelocity, false) &&
        mXmlConf.GetVar("Ground.TestMission.Vector@Heading", heading, false))
    {
        mSendVectorFlag = true;
    }
    else
    {
        mSendVectorFlag = false;
    }
    mDesiredOrientation = Orientation(0, 0, heading);

    mCurrentPosition = mpGlobalInfo->GetPosition();
    mCurrentOrientation = mpGlobalInfo->GetOrientation(true);   // get orientation in degrees

    mCurrentVelocity = mpGlobalInfo->GetVelocityX();

    return 0;
}

std::string TestMission::ExitEarly()
{
    return "KeepRunning";
}

std::string TestMission::Execute()
{
    if(mSendVectorFlag)
    {
        mpGlobalCommand->SetVelocity(mDesiredVelocity);
        mpGlobalCommand->SetYawDegrees(mDesiredOrientation.mZ);
    }
    
    printf("TestMission:: Heading: %lf -> %lf \n Velocity: %lf -> %lf\n", mCurrentOrientation.mZ, mDesiredOrientation.mZ, mCurrentVelocity, mDesiredVelocity);

    return "KeepRunning";
}

int TestMission::ErrorCheck()
{
    return 0;
}
