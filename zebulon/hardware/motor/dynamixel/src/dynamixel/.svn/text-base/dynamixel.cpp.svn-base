/////////////////////////////////////////////////////////////////////////////////////
///
/// \file dynamixel.cpp
/// \brief Dynamixel Servo Interface.
///
/// Author(s): Gary Stein<br>
/// Created: 3/10/2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: gstein@mail.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
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
////////////////////////////////////////////////////////////////////////////////////
#include "dynamixel/dynamixel.h"
//real dynamixel H file
#include <dynamixel.h>

using namespace CxUtils;
using namespace Zebulon;


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///////////////////////////////////////////////////////////////////////////////
Motor::Dynamixel::Dynamixel()
{
    mServoCommand.clear();
    mServoFeedback.clear();
    mDeviceID = -1;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
///////////////////////////////////////////////////////////////////////////////
Motor::Dynamixel::~Dynamixel()
{
    Shutdown();
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Connects to the device and start data acquistion.
///
///  \param[in] port The serial port to connect to.  Must be /dev/ttyUSB#
///                  where # is an integer representing the specific port
///                  number.
///  \param[in] usb2dynamixel The dynamixel adapter used, true if usb2dynamixel
///                           false otherwise
///  \param[in] updateRateHz Frequency to try and receive updated data from
///                          the Dynamixel.
///  \param[in] baudnum The baud num at which to try to connect to first.  If
///                  this baud fails, then others will be automatically
///                  attempted.
///  \return True on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
bool Motor::Dynamixel::Initialize(const std::string& port,const bool usb2dynamixel,const double updateRateHz, const int baudnum)
{
    mPort=port;
    mBaudNum=baudnum;
    mRate=1.0/updateRateHz;
    
    int deviceIndex;

    if(sscanf(port.c_str(),"/dev/ttyUSB%d", &deviceIndex) <= 0)
    {
        printf( "Must used /dev/ttyUSB# format for port\n");
        return false;
    }

    //if using the usb2dynamixel
    if(usb2dynamixel)
    {
        ///////// Open USB2Dynamixel ////////////
        if( dxl_initialize(deviceIndex, baudnum) == 0 )
        {
            printf( "Failed to open USB2Dynamixel!\n" );
            printf( "Press Enter key to terminate...\n" );
            return false;
        }
        else
        {
            printf( "Succeed to open USB2Dynamixel!\n" );
        }
    }
    else
    {
        if( dxl_initialize_adapter(deviceIndex) == 0 )
        {
            printf( "Failed to open Dynamixel with adapter!\n" );
            printf( "Press Enter key to terminate...\n" );
            return false;
        }
        else
        {
            printf( "Succeed to open Dynamixel with adapter!\n" );
        }
    }
    //std::cout << "Number of output arguments = " << numOutputArgs <<std::endl;

    if(!mThread.CreateThread(&ThreadFunction, this))
    {
        return false;
    }
    return true;
}


/** Sends a set servo speed to motor as RPM. */
void Motor::Dynamixel::SetServoSpeed(const int id, const double rpm)
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    int send = (int)(rpm/.111);
    dxl_write_word(id, P_MOVING_SPEED_L, send );
}

/** Sends a set servo torque, 0 low 1023 max. */
void Motor::Dynamixel::SetTorqueLimit(const int id, const int torque)
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    dxl_write_word(id, P_TORQUE_LIMIT_L, torque );
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Connects to the device and start data acquistion.
///
///  \param[in] deviceNum Device/port number.
///  \param[in] updateRateHz Frequency to try and receive updated data from
///                          the Dynamixel.
///  \param[in] usb2dynamixel The dynamixel adapter used, true if usb2dynamixel
///                           false otherwise
///  \param[in] baudnum The baud num at which to try to connect to first.  If
///                  this baud fails, then others will be automatically
///                  attempted.
///  \return True on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
bool Motor::Dynamixel::Initialize(const int deviceNum,const bool usb2dynamixel,const double updateRateHz, const int baudnum)
{
    char buffer[256];
    sprintf(buffer, "/dev/ttyUSB%d", deviceNum);

    return Initialize(buffer, usb2dynamixel, updateRateHz, baudnum);
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Shutsdown the interface.
///
///////////////////////////////////////////////////////////////////////////////
void Motor::Dynamixel::Shutdown()
{
    mThread.StopThread();
    // Close device
    dxl_terminate();
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Continuously receives data from the device, and generates
///         callbacks.
///
///////////////////////////////////////////////////////////////////////////////
void Motor::Dynamixel::ThreadFunction(void *args)
{
    //Pass in as a void *, then cast, then virtuals should work
    Dynamixel* dynamixel= (Dynamixel*)args;
    //call the appropriate real processing function per compass
    dynamixel->ProcessingFunction();
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Converts from a percent value to a servo value that the
///         Dynamixel can use.
///
///  \param[in] val Percent value to send [-100, 100]%.
///
///  \return Servo value to send.
///
///////////////////////////////////////////////////////////////////////////////
int Motor::Dynamixel::ServoConvert(const double val)
{
    int out;
    out=(val/100.0+1.0)/2.0*1023.0;
    if(out>1023)
    {
        out=1023;
    }
    if(out<0)
    {
        out=0;
    }
    return out;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Converts from a servo value to an angle.
///
///  \param[in] val Servo input value.
///
///  \return Percent value to send [-100, 100]%.
///
///////////////////////////////////////////////////////////////////////////////
double Motor::Dynamixel::ServoUnConvert(const int val)
{
    double out;
    out=(val-512.0)/512*100;
    if(out>100.0)
    {
        out=100.0;
    }
    if(out<-100.0)
    {
        out=-100.0;
    }
    return out;
}



///////////////////////////////////////////////////////////////////////////////
///
///  \brief Continuously receives data from the device, and generates
///         callbacks.
///
///////////////////////////////////////////////////////////////////////////////
void Motor::Dynamixel::ProcessingFunction()
{
    int CommStatus;

    while(!mThread.QuitThreadFlag())
    {
        //Self blocking, until data arrives.
        mMutex.Lock();
        
        bool read = true;
        std::map<int,double>::iterator servo;
        for(servo=mServoCommand.begin();servo!=mServoCommand.end();servo++)
        {
            read = false;
            // Write goal position
            int send=ServoConvert(servo->second);
            dxl_write_word( servo->first, P_GOAL_POSITION_L, send );
                
            // Read present position
            int recv=dxl_read_word( servo->first, P_PRESENT_POSITION_L );
            double feedback = ServoUnConvert(recv);
            CommStatus = dxl_get_result();

            if( CommStatus == COMM_RXSUCCESS )
            {
                //PrintErrorCode();
                //basically clear errors, if needed
                ClearErrorCode(servo->first);
                
                mServoFeedback[servo->first]=feedback;
                //printf("Values: %d %d\n",send,recv);
            }
            else
            {
                mFails++;
                //PrintCommStatus(CommStatus);
                break;
            }

            // Trigger callbacks.
            Callback::Set::iterator callback;
            mCallbacksMutex.Lock();
            for(callback = mCallbacks.begin();
                    callback != mCallbacks.end();
                    callback++)
            {
                (*callback)->ProcessServoData(mServoFeedback[servo->first], servo->first);
            }
            mCallbacksMutex.Unlock();
        }
        if(read)
        {
            // Read present position
            int recv=dxl_read_word( 1, P_PRESENT_POSITION_L );
            double feedback = ServoUnConvert(recv);
            CommStatus = dxl_get_result();
            bool data = false;
            if( CommStatus == COMM_RXSUCCESS )
            {
                data = true;

                //PrintErrorCode();
                //basically clear errors, if needed
                ClearErrorCode(1);

                mServoFeedback[1]=feedback;
                //printf("Values: %d %d\n",send,recv);
            }
            else
            {
                mFails++;
                //PrintCommStatus(CommStatus);
            }
            if(data)
            {
                // Trigger callbacks.
                Callback::Set::iterator callback;
                mCallbacksMutex.Lock();
                for(callback = mCallbacks.begin();
                        callback != mCallbacks.end();
                        callback++)
                {
                    (*callback)->ProcessServoData(mServoFeedback[1], 1);
                }
                mCallbacksMutex.Unlock();
            }
        }
        mMutex.Unlock();
        CxUtils::SleepMs(1000*mRate);

    }

}


// Print communication result
void Motor::Dynamixel::PrintCommStatus(int CommStatus)
{
    switch(CommStatus)
    {
    case COMM_TXFAIL:
        printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
        break;

    case COMM_TXERROR:
        printf("COMM_TXERROR: Incorrect instruction packet!\n");
        break;

    case COMM_RXFAIL:
        printf("COMM_RXFAIL: Failed get status packet from device!\n");
        break;

    case COMM_RXWAITING:
        printf("COMM_RXWAITING: Now recieving status packet!\n");
        break;

    case COMM_RXTIMEOUT:
        printf("COMM_RXTIMEOUT: There is no status packet!\n");
        break;

    case COMM_RXCORRUPT:
        printf("COMM_RXCORRUPT: Incorrect status packet!\n");
        break;

    default:
        printf("This is unknown error code!\n");
        break;
    }
}

//Clears an error if one happens
void Motor::Dynamixel::ClearErrorCode(int id)
{
    //Bypass ANY ERROR, not safe
    if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
    {
        dxl_reset(id);
    }
}

// Print error bit of status packet
void Motor::Dynamixel::PrintErrorCode()
{
    if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
        printf("Input voltage error!\n");

    if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
        printf("Angle limit error!\n");

    if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
        printf("Overheat error!\n");

    if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
        printf("Out of range error!\n");

    if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
        printf("Checksum error!\n");

    if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
        printf("Overload error!\n");

    if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
        printf("Instruction code error!\n");
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets a servo value.
///
///  \param[in] id Servo ID value.
///  \param[in] val Value to send [-100, 100]%.
///
///  \return 1 on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
int Motor::Dynamixel::AddServo(const int id,const double val)
{
    SetServo(id,val);
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets a servo value.
///
///  \param[in] id Servo ID value.
///  \param[in] val Value to send [-100, 100]%.
///
///  \return 1 on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
int Motor::Dynamixel::SetServo(const int id,const double val)
{
    if(id<0)
    {
        return 0;
    }
    mMutex.Lock();
    mServoCommand[id]=val;
    mMutex.Unlock();
    return 1;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets a servo value.
///
///  \param[in] id Servo ID value.
///  \param[in] val Degree to send [0, 300].
///
///  \return 1 on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
int Motor::Dynamixel::SetServoDegrees(const int id,const double val)
{
    if(id<0)
    {
        return 0;
    }
    mMutex.Lock();
    mServoCommand[id]=(val-150.0)/150.0*100.0;
    mMutex.Unlock();
    return 1;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the current servo position value.
///
///  \param[in] id Servo ID value.
///
///  \return Servo position as percent [-100, 100]%.
///
///////////////////////////////////////////////////////////////////////////////
double Motor::Dynamixel::GetServo(const int id)
{
    double ret=0.0;
    std::map<int,double>::iterator tmp;
    tmp=mServoFeedback.find(id);
    if(tmp!=mServoFeedback.end())
    {
        mMutex.Lock();
        ret=tmp->second;
        mMutex.Unlock();    
        return ret;
    }
    printf("Dynamixel: Servo Does not Exist\n");
    return 0.0;
}

/** Convert from servo value in dynamixel to degrees. */
double Motor::Dynamixel::ToDegrees(const double value)
{
    return ((value+100)/200*300);
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the current servo position value.
///
///  \param[in] id Servo ID value.
///
///  \return Sero position as a angle.
///
///////////////////////////////////////////////////////////////////////////////
double Motor::Dynamixel::GetServoDegrees(const int id)
{
    double ret=0.0;
    std::map<int,double>::iterator tmp;
    tmp=mServoFeedback.find(id);
    if(tmp!=mServoFeedback.end())
    {
        mMutex.Lock();
        ret=tmp->second;
        mMutex.Unlock();
        return ((ret+100)/200*300);
    }
    printf("Dynamixel: Servo Does not Exist\n");
    return 0.0;
}

//void Motor::Dynamixel::startOscillation(double maxAngle, double minAngle)
//{
//    for(double val=botDegrees;val>=topDegrees;val-=1.0)
//    {
//        double feed = dyna.GetServoDegrees(DYNAMIXEL_SERVO_ID);
//        std::cout << feed << std::endl;
//        dyna.SetServoDegrees(DYNAMIXEL_SERVO_ID, val);
//        CxUtils::SleepMs(50);
//    }
//    for(double val=topDegrees;val<=botDegrees;val+=1.0)
//    {
//        double feed = dyna.GetServoDegrees(DYNAMIXEL_SERVO_ID);
//        std::cout << feed << std::endl;
//        dyna.SetServoDegrees(DYNAMIXEL_SERVO_ID, val);
//        CxUtils::SleepMs(50);
//    }

//}

/*  End of File */
