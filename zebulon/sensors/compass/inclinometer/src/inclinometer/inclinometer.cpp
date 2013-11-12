//////////////////////////////////////////////////////////////////////////////////////
///
/// \file inclinometer.cpp
/// \brief Inclinometer to a generic inclinometer
///
/// Author(s): Gary Stein<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: gstein@mail.ucf.edu <br>
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
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
//////////////////////////////////////////////////////////////////////////////////////
#include "inclinometer/inclinometer.h"
#include <vector>
#include <cxutils/cxutils.h>

using namespace Zebulon;
using namespace Compass;
using namespace INCLINOMETER;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Inclinometer::Inclinometer()
{
    mName = "Inclinometer";
    mImuSupported=true;
    
    //This model needs these lines set
    mSerial.EnableDTR(true);
    mSerial.EnableRTS(true);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Inclinometer::~Inclinometer()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the compass to continuous output mode.
///
////////////////////////////////////////////////////////////////////////////////////
bool Inclinometer::SetContinuousMode()
{
    //Has none, always outputs
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a thread which continuously receives and parsed data from
///   serial port.
///
////////////////////////////////////////////////////////////////////////////////////
void Inclinometer::ProcessingFunction()
{
    CxUtils::Packet p;
    CxUtils::Packet buffer;
    
    p.Clear();
    buffer.Clear();
    while(mSerialThread.QuitThreadFlag() == false)
    {
        if(mSerial.IsConnected())
        {
            int read=mSerial.Recv(p,28,10000);
            if(read>0)
            {
                //printf("%d: %s\n",read,p.Ptr());
                if(*p.Ptr()=='X')
                {
                    buffer.Clear();
                }
                
                if(*p.Ptr()=='\n')
                {
                    //printf("%d: %s\n",buffer.Length(),buffer.Ptr());
                    //X=-0.118 Y=-0.128 Z= 0.68
                    double x,y,z;
                    if(sscanf((const char *)buffer.Ptr(),"X=%lf Y=%lf Z=%lf",&x,&y,&z)==3)
                    {
                        mValidDataFlag = true;
                        
                        mMutex.Lock();
                        //No yaw in an inclinometer
                        mYaw=0.0;
                        //Need to do real math
                        mPitch=atan2(y,1.0)*2.0;
                        mRoll=-atan2(x,1.0)*2.0;
                        mMutex.Unlock();
                        RunDefaultCallback();
                        
                        mMutex.Lock();
                        mAttitude=CxUtils::Point3D(mRoll,mPitch,mYaw);
                        mGyroscope=CxUtils::Point3D(0,0,0);
                        mAcceleration=CxUtils::Point3D(x,y,z);
                        mMutex.Unlock();
                        RunIMUCallback();
                    }
                }
                else
                {
                    buffer+=p;
                }
            }
        }
        CxUtils::SleepMs(1);
    }
}

/*  End of File */
