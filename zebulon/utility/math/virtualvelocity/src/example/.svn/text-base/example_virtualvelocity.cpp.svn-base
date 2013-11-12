//////////////////////////////////////////////////////////////////////////////////////
///
/// \file example_virtualvelocity.cpp
/// \brief Example/Test Program for the VirtualVelocity class
///
/// Author(s): Michael Scherer<br>
/// Created: 5/1/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the Robotics Laboratory, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREJAUS global pose sensorMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
//////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <cxutils/cxutils.h>
#include <cxutils/math/point3d.h>
#include <compass/compass.h>
#include <compass/compassfactory.h>
#include "virtualvelocity/imudata.h"
#include "virtualvelocity/virtualvelocity.h"
#include "virtualvelocity/gpsvelocity.h"

using namespace Zebulon;
using namespace Compass;
using namespace Utility;

#define USE_RANDOM 0
#define USE_CORAL 1
#define USE_GPS 0

int main(int argc, char **argv)
{
    //printf("Virtual Velocity Example!\n");
    
    CxUtils::Point3D orient;// = new CxUtils::Point3D();
    CxUtils::Point3D accel;// = new CxUtils::Point3D();
    CxUtils::Point3D gyro;// = new CxUtils::Point3D();
    
    ImuData* data = new ImuData(orient, accel, gyro);
    
    VirtualVelocity sensor;
    
#if USE_RANDOM == 1 && !USE_CORAL && !USE_GPS
    accel.Set(0, 0, -9.8);
    orient.Set(0, 0, 0);
    gyro.Set(0, 0, 0);
    
    double p = 0;
    double y = 0;
    double r = 0;
    double dp, dy, dr;
    
    sensor.Initialize(true);
    long long int time = CxUtils::GetTimeMs();
    
    for (int i=0; i<10; i++)
    {
        CxUtils::Timer::Pause(15);
        
        dp = p;
        dy = y;
        dr = r;
        
        p = fmod(p + .2, CxUtils::CX_TWO_PI);
        y = fmod(y + .2, CxUtils::CX_TWO_PI);
        r = fmod(r + .2, CxUtils::CX_TWO_PI);
        
        orient.Set(p, y, r);
        
        long long int curtime = CxUtils::GetTimeMs();
        
        dp = (p - dp)/(curtime - time);
        dy = (y - dy)/(curtime - time);
        dr = (r - dr)/(curtime - time);
        
        time = curtime;
        
        gyro.Set(dp, dy, dr);
        
        accel.Set(0, 0, -9.8);
        accel = VirtualVelocity::RotationMatrix(accel, orient);
        //accel.mY += 1; // add some imaginary forward motion
        
        /*accel->Rotate(p, 0);
        *accel = accel->Rotate(y, 1);
        *accel = accel->Rotate(r, 2);*/
        
        printf("= Raw ========================\n");
        printf("Orient: %2.2lf %2.2lf %2.2lf ", p, y, r);
        printf("Gyro: %2.3lf %2.3lf %2.3lf ", dp, dy, dr);
        
        accel.Get(dp, dy, dr);
        printf("Accel: %2.3lf %2.3lf %2.3lf\n", dp, dy, dr);
        data->mAccel = accel;
        data->mGyro = gyro;
        data->mOrient = orient;
        
        
        printf("- Clean ----------------------\n");
        sensor.SetImuData(data);
        CxUtils::Point3D output;
        
        sensor.GetVelocity(output);
        output.Get(dp, dy, dr);
        printf("Vel: %2.3lf %2.3lf %2.3lf\n", dp, dy, dr);
        
        sensor.GetAcceleration(output);
        output.Get(dp, dy, dr);
        printf("Accel: %2.3lf %2.3lf %2.3lf\n", dp, dy, dr);
        
        printf("\n");
        
    }
#elif USE_CORAL == 1 && !USE_RANDOM && !USE_GPS
    
    Compass::GenericCompass *comp_sensor=NULL;
    //MyCallBackPrint callback;
    
    printf("Attempting to connect to compass\n");

    if(argc >1)
    {
        //generic compass
        //comp_sensor=CompassFactory::CreateCompass("3DM-GX1");
        //comp_sensor=CompassFactory::CreateCompass("CoralAHRS");
        comp_sensor=CompassFactory::CreateCompass("3DM-GX2");
        if(comp_sensor!=NULL)
        {
            if(comp_sensor->Initialize(argv[1],115200,true,1000))
            {
                sensor.Initialize(true);
                comp_sensor->RegisterCallback(&sensor,Compass::Callback::IMUCallback);
                std::cout << "Enter <q> to Exit.\n";
                CxUtils::SleepMs(1000);
                
                /*std::string enter;
                cin >> enter;*/
                char c;
                scanf("%c", &c);
                
                comp_sensor->Shutdown();
            }
        }
        else
        {
            printf("Compass did not exist\n");
        }
    }
    else
    {
        printf("Specify a serial port\n");
    }
    
#elif !USE_CORAL && !USE_RANDOM && USE_GPS
    
    GPSVelocity gpssensor;
    double dt = 1.0/72;
    
    double easting = 0;
    double northing = 0;
    double time = 0;
    double heading = CxUtils::PI/4.0;//CxUtils::PI/4.0;
    for (int i=0; i<200; i++)
    {
        gpssensor.SetValues(time, easting, northing, heading);
        printf("%lf %lf\n", gpssensor.GetAxialVelocity(), gpssensor.GetLateralVelocity());
        //fprintf
        
        //heading += .03;
        time += dt;
        
        easting  -= (.01) + .0005*cos(time);
        northing += (.01) + .0005*cos(time);
    }
    printf("\n");
    
#else
    printf("Error: Data choice must be exclusively specified");
#endif
    
    delete data;
    
    return 0;
}