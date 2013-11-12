#include "virtualvelocity/virtualvelocity.h"

//#define DEBUG_VIRTUALVELOCITY
#define DEBUG_VIRTUALVELOCITY_OUT

#define DEBUG_VIRTUALVELOCITY_LOG

using namespace Zebulon;
using namespace Compass;
using namespace Utility;
using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a new VirtualVelocity object.
///
////////////////////////////////////////////////////////////////////////////////////
VirtualVelocity::VirtualVelocity()
{
    this->time = 0;
    
    double dt = .01;
    double rc = .00015;
    filterX = new LowPassFilter(dt, rc);  //new MedianFilter(3);
    filterY = new LowPassFilter(dt, rc);  //new MedianFilter(3);
    filterZ = new LowPassFilter(dt, rc);  //new MedianFilter(3);
    mOffset.Set(0.0,0.0,0.0);
    this->mTimeStep = 0.0;
    this->filterFlag=0;
    
    
    dt = .01;
    rc = .00015;
    filterWX = new LowPassFilter(dt, rc);  //new AverageFilter(3);
    filterWY = new LowPassFilter(dt, rc);  //new AverageFilter(3);
    filterWZ = new LowPassFilter(dt, rc);  //new AverageFilter(3);
    
    ///////////
    dt = .01;
    rc = .15;
    filterX2 = new HighPassFilter(dt, rc);  //new MedianFilter(3);
    filterY2 = new HighPassFilter(dt, rc);  //new MedianFilter(3);
    filterZ2 = new HighPassFilter(dt, rc);  //new MedianFilter(3);
    mOffset.Set(0.0,0.0,0.0);
    this->mTimeStep = 0.0;
    this->filterFlag=0;
    
    
    dt = .01;
    rc = .15;
    filterWX2 = new HighPassFilter(dt, rc);  //new AverageFilter(3);
    filterWY2 = new HighPassFilter(dt, rc);  //new AverageFilter(3);
    filterWZ2 = new HighPassFilter(dt, rc);  //new AverageFilter(3);
}

VirtualVelocity::~VirtualVelocity()
{
    delete filterX;
    delete filterY;
    delete filterZ;
    
    delete filterWX;
    delete filterWY;
    delete filterWZ;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the VirtualVelocity sensor
///
///   \param Gravity 1 if gravity subtraction is enabled, 0 otherwise
///
///   \return 1 if operation was successful, 0 otherwise
///
////////////////////////////////////////////////////////////////////////////////////
bool VirtualVelocity::Initialize(bool Gravity,double DT)
{
    this->mGravityEnable = Gravity;
    this->mDT = DT;
    if(Gravity)
    {
        mOffset.mZ = gravity;
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the current IMU state, removes rotational noise from
///           acceleration data, and removes gravity from acceleration data
///
///   \param Data the IMU data
///
///   \return 1 if operation was successful, 0 otherwise
///
////////////////////////////////////////////////////////////////////////////////////
bool VirtualVelocity::SetImuData(ImuData* Data)
{
    #ifdef DEBUG_VIRTUALVELOCITY
    printf("DEBUG_VirtualVelocity: accelprero:%.3lf %.3lf %.3lf\n", Data->mAccel.mX, Data->mAccel.mY, Data->mAccel.mZ);
    #endif

    CxUtils::Point3D preAccel = this->mAcceleration;
    CxUtils::Point3D accel = InvRotationMatrix(Data->mAccel, Data->mOrient);

    #ifdef DEBUG_VIRTUALVELOCITY
    printf("DEBUG_VirtualVelocity: accelpostro:%.3lf %.3lf %.3lf\n", accel.mX, accel.mY, accel.mZ);
    #endif

    if (this->mGravityEnable)
    {
        accel.mZ += 9.8;
    }
    
    #ifdef DEBUG_VIRTUALVELOCITY
    printf("DEBUG_VirtualVelocity: accelpostre:%.3lf %.3lf %.3lf\n", accel.mX, accel.mY, accel.mZ);
    #endif

    this->mAcceleration = accel;
    
    // if time == 0, then this is the first pass, and
    //  the data is essentially useless, so let's just start
    //  fresh with 0.
    if (time == 0)
    {
        // Zero the velocity
        this->mVelocity.mX = 0;
        this->mVelocity.mY = 0;
        this->mVelocity.mZ = 0;
    }
    else
    {
        // Integrate, set velocity
        this->mVelocity.mX += .5*(accel.mX + preAccel.mX) * (CxUtils::GetTimeMs() - time);
        this->mVelocity.mY += .5*(accel.mY + preAccel.mY) * (CxUtils::GetTimeMs() - time);
        this->mVelocity.mZ += .5*(accel.mZ + preAccel.mZ) * (CxUtils::GetTimeMs() - time);
    }
    
    // reset the clock, return true
    time = CxUtils::GetTimeMs();
    return 1;
}

void VirtualVelocity::ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope, const CxUtils::Point3D acceleration)
{
    double fx, fy, fz;
    
    // Rate Gyros (angular velocity)
    filterWX->Add(gyroscope.mX);
    filterWX->Output(fx);
    mAngularVelocity.mX = fx;
    
    filterWY->Add(gyroscope.mY);
    filterWY->Output(fy);
    mAngularVelocity.mY = fy;
    
    filterWZ->Add(gyroscope.mZ);
    filterWZ->Output(fz);
    mAngularVelocity.mZ = fz;
    
    // Accelerometers (velocity)
    // Filter
    filterX->Add(acceleration.mX);
    filterFlag+=filterX->Output(fx);
    
    filterY->Add(acceleration.mY);
    filterY->Output(fy);
    
    filterZ->Add(acceleration.mZ);
    filterZ->Output(fz);
    
    //acceleration.Set(fx, fy, fz);
    CxUtils::Point3D filteredAccel;
    filteredAccel.Set(fx, fy, fz);
    
    // End filter
    
    #ifdef DEBUG_VIRTUALVELOCITY_LOG
    static FILE *outstream=fopen("velocity.log","w");
    #endif
    
    #ifdef DEBUG_VIRTUALVELOCITY
    printf("DEBUG_VirtualVelocity: accelprero:%.3lf %.3lf %.3lf\n", filteredAccel.mX, filteredAccel.mY, filteredAccel.mZ);
    #endif
    
    #ifdef DEBUG_VIRTUALVELOCITY_LOG
    fprintf(outstream,"%lf ",mTimeStep);
    fprintf(outstream,"%lf %lf %lf ", filteredAccel.mX, filteredAccel.mY, filteredAccel.mZ);
    fprintf(outstream,"%lf %lf %lf ", attitude.mX, attitude.mY, attitude.mZ);
    #endif

    CxUtils::Point3D preAccel = this->mAcceleration;
    CxUtils::Point3D accel = RotationMatrix(filteredAccel, attitude);

    #ifdef DEBUG_VIRTUALVELOCITY
    printf("DEBUG_VirtualVelocity: accelpostro:%.3lf %.3lf %.3lf\n", accel.mX, accel.mY, accel.mZ);
    #endif
    
    #ifdef DEBUG_VIRTUALVELOCITY_LOG
    fprintf(outstream,"%lf %lf %lf ", accel.mX, accel.mY, accel.mZ);
    #endif
    
    //if (this->mGravityEnable)
    //{
    //    //printf("%d\n", this->mGravityEnable);
    //    accel.mZ -= gravity;
    //}
    
    #ifdef DEBUG_VIRTUALVELOCITY
    printf("DEBUG_VirtualVelocity: accelpostre:%.3lf %.3lf %.3lf\n", accel.mX, accel.mY, accel.mZ);
    #endif

    if(filterFlag == 1)
    {
        mOffset=accel;
    }
    else
    {
        DCFilter(accel.mX,mOffset.mX);
        DCFilter(accel.mY,mOffset.mY);
        DCFilter(accel.mZ,mOffset.mZ);
    }

    #ifdef DEBUG_VIRTUALVELOCITY_LOG
    fprintf(outstream,"%lf %lf %lf ", mOffset.mX, mOffset.mY, mOffset.mZ);
    #endif

    
    this->mAcceleration = accel - mOffset;


    
    long long int CurrTime=CxUtils::GetTimeMs();
    // if time == 0, then this is the first pass, and
    //  the data is essentially useless, so let's just start
    //  fresh with 0.
    
    double tX, tY, tZ;
    tX = this->mVelocity.mX;
    tY = this->mVelocity.mY;
    tZ = this->mVelocity.mZ;
    if (filterFlag < 10)
    {
        // Zero the velocity
        tX = this->mVelocity.mX = 0;filterX2->Add(tX);
        tY = this->mVelocity.mY = 0;filterY2->Add(tY);
        tZ = this->mVelocity.mZ = 0;filterZ2->Add(tZ);
    }
    else
    {
        // Integrate, set velocity
        tX += .5*(mAcceleration.mX + preAccel.mX) * mDT; // (CurrTime - time)/1000.0;
        tY += .5*(mAcceleration.mY + preAccel.mY) * mDT; // (CurrTime - time)/1000.0;
        tZ += .5*(mAcceleration.mZ + preAccel.mZ) * mDT; // (CurrTime - time)/1000.0;
        
        filterX2->Add(tX);
        filterX2->Output(this->mVelocity.mX);
        
        filterY2->Add(tY);
        filterY2->Output(this->mVelocity.mY);
        
        filterZ2->Add(tZ);
        filterZ2->Output(this->mVelocity.mZ);
    }
    
    #ifdef DEBUG_VIRTUALVELOCITY_OUT
    printf("velocity: %2.3lf %2.3lf %2.3lf acceleration:  %2.3lf %2.3lf %2.3lf\n\n"
        , this->mVelocity.mX, this->mVelocity.mY, this->mVelocity.mZ
        , this->mAcceleration.mX, this->mAcceleration.mY, this->mAcceleration.mZ);
    /*printf(" preAccel:  %2.3lf %2.3lf %2.3lf\n\n"
        , preAccel.mX, preAccel.mY, preAccel.mZ);*/

    //mOffset.Print();
    #endif
    
    #ifdef DEBUG_VIRTUALVELOCITY_LOG
    fprintf(outstream,"%lf %lf %lf ", mVelocity.mX, mVelocity.mY, mVelocity.mZ);
    #endif
    
    #ifdef DEBUG_VIRTUALVELOCITY_LOG
        fprintf(outstream,"\n");
        fflush(outstream);
    #endif
    time=CurrTime;
    mTimeStep+=mDT;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the velocity
///
///   \param Orient The velocity of the unit
///
///   \return 1 if operation was successful, 0 otherwise
///
////////////////////////////////////////////////////////////////////////////////////
bool VirtualVelocity::GetVelocity(CxUtils::Point3D &Velocity)
{
    Velocity.mX = this->mVelocity.mX;
    Velocity.mY = this->mVelocity.mY;
    Velocity.mZ = this->mVelocity.mZ;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the acceleration
///
///   \param Acceleration The acceleration of the unit
///
///   \return 1 if operation was successful, 0 otherwise
///
////////////////////////////////////////////////////////////////////////////////////
bool VirtualVelocity::GetAcceleration(CxUtils::Point3D &Acceleration)
{
    Acceleration.mX = this->mAcceleration.mX;
    Acceleration.mY = this->mAcceleration.mY;
    Acceleration.mZ = this->mAcceleration.mZ;
    
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the angular velocity
///
///   \param AngularVelocity The angular velocity of the unit
///
///   \return 1 if operation was successful, 0 otherwise
///
////////////////////////////////////////////////////////////////////////////////////
bool VirtualVelocity::GetAngularVelocity(CxUtils::Point3D &AngularVelocity)
{
    AngularVelocity.mX = this->mAngularVelocity.mX;
    AngularVelocity.mY = this->mAngularVelocity.mY;
    AngularVelocity.mZ = this->mAngularVelocity.mZ;
    
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Zeros out the velocity integration -- I reccomend you do this whenever
///           you know that you are stopped in order to minimize propegation of
///           integration error.
///
///   \return 1 if operation was successful, 0 otherwise
///
////////////////////////////////////////////////////////////////////////////////////
bool VirtualVelocity::ZeroVelocity()
{
    this->mVelocity.mX = 0;
    this->mVelocity.mY = 0;
    this->mVelocity.mZ = 0;
    
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Rotates a vector -- Thanks Gary
///
///   \param Vector The vector
///   \param Orientation The way you want to rotate the vector
///
///   \return the vector, rotated
///
////////////////////////////////////////////////////////////////////////////////////
Point3D VirtualVelocity::RotationMatrix(Point3D Vector, Point3D Orientation)
{
    Point3D Output;
    //Point3D uses Quaternions...not sure I trust it for now
    Matrix RotationR(3,3);
    Matrix RotationP(3,3);
    Matrix RotationY(3,3);
    Matrix MatrixVec(3,1);
    Matrix MatrixOut(3,1);
    
    //Direct access faster, so far cleaner?
    MatrixVec(0,0)=Vector.mX;
    MatrixVec(1,0)=Vector.mY;
    MatrixVec(2,0)=Vector.mZ;
    
    
    //assume Radians
    //Roll
    RotationR(0,0)=1.0;
    RotationR(1,1)=cos(Orientation.mX);
    RotationR(1,2)=-sin(Orientation.mX);
    RotationR(2,1)=sin(Orientation.mX);
    RotationR(2,2)=cos(Orientation.mX);

    //Pitch
    RotationP(0,0)=cos(Orientation.mY);
    RotationP(0,2)=sin(Orientation.mY);
    RotationP(1,1)=1.0;
    RotationP(2,0)=-sin(Orientation.mY);
    RotationP(2,2)=cos(Orientation.mY);
    
    //Yaw
    RotationY(0,0)=cos(Orientation.mZ);
    RotationY(0,1)=-sin(Orientation.mZ);
    RotationY(1,0)=sin(Orientation.mZ);
    RotationY(1,1)=cos(Orientation.mZ);
    RotationY(2,2)=1.0;
    
    MatrixOut=RotationY*RotationP*RotationR*MatrixVec;
    
    Output.mX=MatrixOut(0,0);
    Output.mY=MatrixOut(1,0);
    Output.mZ=MatrixOut(2,0);

    return Output;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Rotates a vector, opposite to the orientation -- Thanks Gary
///
///   \param Vector The vector
///   \param Orientation The way you want to rotate the vector
///
///   \return the vector, rotated
///
////////////////////////////////////////////////////////////////////////////////////
Point3D VirtualVelocity::InvRotationMatrix(Point3D Vector, Point3D Orientation)
{
    Point3D Output;
    //Point3D uses Quaternions...not sure I trust it for now
    Matrix RotationR(3,3);
    Matrix RotationP(3,3);
    Matrix RotationY(3,3);
    Matrix MatrixVec(3,1);
    Matrix MatrixOut(3,1);
    
    //Direct access faster, so far cleaner?
    MatrixVec(0,0)=Vector.mX;
    MatrixVec(1,0)=Vector.mY;
    MatrixVec(2,0)=Vector.mZ;
    
    
    //assume Radians
    //Roll
    RotationR(0,0)=1.0;
    RotationR(1,1)=cos(Orientation.mX);
    RotationR(1,2)=sin(Orientation.mX);
    RotationR(2,1)=-sin(Orientation.mX);
    RotationR(2,2)=cos(Orientation.mX);

    //Pitch
    RotationP(0,0)=cos(Orientation.mY);
    RotationP(0,2)=-sin(Orientation.mY);
    RotationP(1,1)=1.0;
    RotationP(2,0)=sin(Orientation.mY);
    RotationP(2,2)=cos(Orientation.mY);
    
    //Yaw
    RotationY(0,0)=cos(Orientation.mZ);
    RotationY(0,1)=sin(Orientation.mZ);
    RotationY(1,0)=-sin(Orientation.mZ);
    RotationY(1,1)=cos(Orientation.mZ);
    RotationY(2,2)=1.0;
    
    MatrixOut=RotationR*RotationP*RotationY*MatrixVec;
    
    Output.mX=MatrixOut(0,0);
    Output.mY=MatrixOut(1,0);
    Output.mZ=MatrixOut(2,0);

    return Output;
}


void VirtualVelocity::DCFilter(double Value,double &Offset)
{
    if(Value>Offset)
    {
        Offset+=RAMP;
    }
    else if(Value<Offset)
    {
        Offset-=RAMP;
    }
}
