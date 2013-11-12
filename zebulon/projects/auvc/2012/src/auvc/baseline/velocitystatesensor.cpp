#include "auvc/baseline/velocitystatesensor.h"
#include <jaus/core/component.h>
//#include "auvc/baseline/globalposesensor.h"

using namespace Zebulon;
using namespace Underwater;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityStateSensor::VelocityStateSensor()
{
    char filename[1024];
    int time=CxUtils::GetTimeMs();
    sprintf(filename,"velocity.txt");
    mLogFile = fopen(filename,"w");
    mFirstTime=true;
    VelocityCount=0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityStateSensor::~VelocityStateSensor()
{
    fclose(mLogFile);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Callback method generated when new compass data is available.
///
///   \param[in] yaw Yaw in radians (heading) (positive to left, negative to
///                  the right).
///   \param[in] pitch Pitch in radians (positive pitch up).
///   \param[in] roll Roll in radians (positive roll to right).
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateSensor::ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope, const CxUtils::Point3D acceleration)
{
#ifdef LOG_IMU_DATA
    //fprintf(mLogFile, "%lf %lf %lf %lf %lf %lf %lf %lf %lf\n", acceleration.mX, acceleration.mY, acceleration.mZ, rotatedOrientation.mX, rotatedOrientation.mY, rotatedOrientation.mZ, vel.mX, vel.mY, vel.mZ);
#endif
    
    //Check on the Orientation of these -gary
    mVelState.SetRollRate(gyroscope.mX);
    mVelState.SetPitchRate(gyroscope.mY);
    mVelState.SetYawRate(gyroscope.mZ);
    mVelState.SetTimeStamp(JAUS::Time(true));
    
    SetVelocityState(mVelState);

    fflush(mLogFile);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Callback method generated when new compass data is available.
///
///   \param[in] velocity in m/s in the orientation of the compass (local frame)
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateSensor::ProcessVelocityData(const CxUtils::Point3D velocity)
{
#ifdef LOG_IMU_DATA
    //fprintf(mLogFile, "%lf %lf %lf %lf %lf %lf %lf %lf %lf\n", acceleration.mX, acceleration.mY, acceleration.mZ, rotatedOrientation.mX, rotatedOrientation.mY, rotatedOrientation.mZ, vel.mX, vel.mY, vel.mZ);
#endif
    //Check the units of this


    //first time init, assume not moving
    if(mFirstTime)
    {
        LPAccel=velocity;
        mFirstTime=false;
        OldAccel=0;
    }

    mAccel=velocity;
    LPAccel+=(mAccel-LPAccel)/20.0;
    HPAccel=mAccel-LPAccel;

    mVelocity+=(OldAccel+HPAccel)/2.0;
    OldAccel=HPAccel;

    if(HPAccel.Magnitude()<.05)
    {
        VelocityCount++;
    }
    else
    {
        VelocityCount=0;
    }

    /*if(VelocityCount>=10)
    {
        mVelocity(0,0,0);
    }*/







    LPVelocity+=(mVelocity-LPVelocity)/40.0;
    HPVelocity=mVelocity-LPVelocity;


    mVelState.SetVelocityX(HPVelocity.mX);
    mVelState.SetVelocityY(HPVelocity.mY);
    mVelState.SetVelocityZ(HPVelocity.mZ);
    mVelState.SetTimeStamp(JAUS::Time(true));

    SetVelocityState(mVelState);

    //fprintf(mLogFile,"%lf %lf %lf %lf\n",mAccel.mX, HPAccel.mX,mVelocity.mX,HPVelocity.mX);


    fflush(mLogFile);
}

/* End of File */
