//-DCMAKE_BUILD_TYPE=Debug -DBUILD_EXAMPLES=TRUE
#include <stdio.h>
#include "gx3/gx3.h"
#include <cxutils/cxutils.h>
using namespace Zebulon;
using namespace INS;
using namespace MicroStrain;

class MyCallback : public Callback
{
    public:
    void Initialize()
    {
        starttime=CxUtils::GetTimeSeconds();
        imustream=fopen("imu.log","w");
        insstream=fopen("ins.log","w");
        gpsstream=fopen("gps.log","w");
        velstream=fopen("vel.log","w");
        imucount=inscount=gpscount=velcount=0;
    }

    virtual void ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope,const CxUtils::Point3D acceleration)
    {
        double time=CxUtils::GetTimeSeconds();
        fprintf(imustream,"%lf",time-starttime);
        fprintf(imustream," %lf %lf %lf",attitude.mX,attitude.mY,attitude.mZ);
        fprintf(imustream," %lf %lf %lf",gyroscope.mX,gyroscope.mY,gyroscope.mZ);
        fprintf(imustream," %lf %lf %lf",acceleration.mX,acceleration.mY,acceleration.mZ);
        fprintf(imustream,"\n");
        fflush(imustream);
        imucount++;
        if(imucount%100==0)
        {
            printf("Attitude: ");
            attitude.Print();
            gyroscope.Print();
            acceleration.Print();
        }
    }

    virtual void ProcessINSData(const CxUtils::Point3D LLHposition,const CxUtils::Point3D NEDVelocity,const CxUtils::Point3D Orientation)
    {
        double time=CxUtils::GetTimeSeconds();
        fprintf(insstream,"%lf",time-starttime);
        fprintf(insstream," %lf %lf %lf",LLHposition.mX,LLHposition.mY,LLHposition.mZ);
        fprintf(insstream," %lf %lf %lf",NEDVelocity.mX,NEDVelocity.mY,NEDVelocity.mZ);
        fprintf(insstream," %lf %lf %lf",Orientation.mX,Orientation.mY,Orientation.mZ);
        fprintf(insstream,"\n");
        fflush(insstream);
        inscount++;
        if(inscount%100==0)
        {
            printf("Position INS: ");
            LLHposition.Print();
            NEDVelocity.Print();
            Orientation.Print();
        }
    }

    virtual void ProcessGPSData(double latitude, double longitude, double altitude)
    {
        double time=CxUtils::GetTimeSeconds();
        fprintf(gpsstream,"%lf",time-starttime);
        fprintf(gpsstream," %lf %lf %lf",latitude,longitude,altitude);
        fprintf(gpsstream,"\n");
        fflush(gpsstream);
        gpscount++;
        if(gpscount%4==0)
        {
            printf("Position: ");
            CxUtils::Point3D LLHposition(latitude,longitude,altitude);
            LLHposition.Print();
        }
    }

    virtual void ProcessVelocityData(const CxUtils::Point3D velocity)
    {
        double time=CxUtils::GetTimeSeconds();
        fprintf(velstream,"%lf",time-starttime);
        fprintf(velstream," %lf %lf %lf",velocity.mX,velocity.mY,velocity.mZ);
        fprintf(velstream,"\n");
        fflush(velstream);
        velcount++;
        if(velcount%100==0)
        {
            printf("Velocity IMU: ");
            velocity.Print();
        }
    }

    FILE *imustream;
    FILE *insstream;
    FILE *gpsstream;
    FILE *velstream;
    int imucount;
    int inscount;
    int gpscount;
    int velcount;
    double starttime;

};


int main()
{
    GX3 *gx3=new GX3();

    //gx3->Initialize("/dev/asv-gx3",115200,false);
    gx3->Initialize("/dev/auv-compass",115200,false);
    MyCallback *callback=new MyCallback();
    callback->Initialize();
    gx3->RegisterCallback(callback);
    while(1)
    {
        double diff=CxUtils::GetTimeSeconds()-callback->starttime;
        if(diff>0)
        {
            printf("IMU %d (%lf) INS %d (%lf) GPS %d (%lf)\n",callback->imucount,callback->imucount/diff,callback->inscount,callback->inscount/diff,callback->gpscount,callback->gpscount/diff);
            fflush(stdout);
        }
        CxUtils::SleepMs(1000);
    }
    return 0;
};
