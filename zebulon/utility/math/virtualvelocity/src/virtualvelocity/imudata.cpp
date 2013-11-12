#include "virtualvelocity/imudata.h"

using namespace Zebulon::Utility;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a new ImuData object.
///
///   \param Orient The compass orientation
///   \param Accel The acceleration data from the IMU (in m/s/s)
///   \param Gyro The gyroscope's data from the IMU (in rad/s)
///
///   \return 1 if created, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
ImuData::ImuData(CxUtils::Point3D Orient, CxUtils::Point3D Accel,
                 CxUtils::Point3D Gyro)
{
    this->mOrient = Orient;
    this->mAccel = Accel;
    this->mGyro = Gyro;
}

ImuData::~ImuData()
{
    // delete this->mOrient;
    // delete this->mAccel;
    // delete this->mGyro;
}