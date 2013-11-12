/////////////////////////////////////////////////////////////////////////////////////
///
/// \file tcm250.cpp
/// \brief A simple Compass class capable of receiving data from TCM 250
/// using a serial connection
///
/// Author(s): David Adams<br>
/// Created: 10/16/2008<br>
/// Last Modified: 10/16/2008<br>
/// Copyright (c) 2008<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu <br>
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
/////////////////////////////////////////////////////////////////////////////////////
#include "tcm250/tcm250.h"
#include <cxutils/math/cxmath.h>
#include <cxutils/cxutils.h>
using namespace Zebulon;
using namespace Compass;
using namespace TCM;

unsigned int DELAY_TIME_MS = 100;


//////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Default constructor.
///
//////////////////////////////////////////////////////////////////////////////////////
TCM250::TCM250()
{

    mName="TCM250";
    mYaw = mPitch = mRoll = mTemp = mBx = mBy = mBz = 0.0;
    mError = 0;
    mStandbyFlag = false;
    mValidDataFlag = false;

}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Destructor.  Disconnects from compass.
///
////////////////////////////////////////////////////////////////////////////////////
TCM250::~TCM250()
{
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Instructs TCM2 to exit Continuous Sampling mode and enter the Standby
///  mode. If this command is received while the TCM2 is transmitting an
///  output word, the remainder of the output word will be sent before the TCM2
///  changes modes.
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::Halt()
{
    if(mSerial.IsConnected())
    {
        mSerial.Send("h\r\n",3);
        CxUtils::SleepMs(DELAY_TIME_MS);
        mSerial.Send("h\r\n",3);
        CxUtils::SleepMs(DELAY_TIME_MS);
        mStandbyFlag = true;
        return 1;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Instructs TCM2 to enter Continuous Sampling mode from the Standby
///  mode. The TCM2 will immediately begin sampling sensors at the rate
///  specified (by the clock command). The TCM2 will automatically transmit
///  data at the sampling rate, and according to the output word format
///  currently specified. To exit the TCM2 and return to Standby mode, issue
///  the Halt Continuous Sampling (h) command.
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::StartContinuousOutput()
{
    if(mSerial.Send("go\r\n",4) == 0)
    {
        return 0;
    }
    
    mStandbyFlag = false;
    CxUtils::SleepMs(DELAY_TIME_MS);
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Sets the compass to true north (default setting).
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::SetTrueNorth()
{
    if(!mStandbyFlag)
        Halt();

    if(mSerial.Send("sn=n\r\n",6) == 0)
    {
        return 0;
    }
    CxUtils::SleepMs(DELAY_TIME_MS);
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Tells the TCM250 to use magnetic north.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::SetMagneticNorth()
{
    if(!mStandbyFlag)
        Halt();

    if(!mSerial.Send("sn=m\r\n",6))
    {
        printf("ERROR:  Could not send command\n");
        return 0;
    }
    CxUtils::SleepMs(DELAY_TIME_MS);
    return 1;
}
////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  This command is used with the Set Digital Damping command. This
///  command will allow the TCM2 to take continuous measurements but
///  output at a slower rate. For instance you could set the time constant to
///  100 in the digital damping mode and set %skip=50 and output every 50th
///  measurement.
///
///  \param i Number of measurements to skip
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::SetQuerySkip(const unsigned int i)
{
   
    if(!mStandbyFlag)
        Halt();

    if(!mSerial.Send("%skip=0\r\n",9))
    {
        printf("ERROR:  Could not send skip command\n");
        return 0;
    }
    CxUtils::SleepMs(DELAY_TIME_MS);
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Divides the clock rate set by "clock=". For the TCM2, this command is
///  meant to be use in conjunction with the clock= command to set the
///  sampling period below 5 Hz. In the TCM2, this command was used to
///  divide down the 8 Hz sampling rate. It can be used as in the TCM2 if the
///  clock rate is set to 8 Hz.
///
///  \param i Divisor. 1 to 480.
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::SetSamplingPeriodDivisor(const unsigned int i)
{
    if(i<1 || i>5)
    {
        printf("ERROR:  Invalid Divisor\n");
        return 0;
    }
    if(!mStandbyFlag)
        Halt();

    if(!mSerial.Send("sp=1\r\n", 6))
    {
        printf("ERROR:  Could not send sampling period divisor command\n");
        return 0;
    }
    CxUtils::SleepMs(DELAY_TIME_MS);
    return 1;
}
////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Enables the user to have fine control of the clock rate. The ability to
///  sample at the higher speeds must be matched with the amount of data
///  being output at each sample. At 9600 baud, each character requires about
///  1 millisecond to output, so at 30 Hz, at most 30 characters can be output
///  which is not enough for all data outputs to be enabled in the output word.
///
///  \param freq The new clock rate [5hz, 40Hz]
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::SetClockRate(const unsigned int freq)
{
    if(freq < 5 || freq > 40)
    {
        printf("ERROR:  Invalid frequency\n");
        printf("TCM250::setClockRate(int freq)\n");
        return 0;
    }
    else
    {
        char temp[11];

        if(!mStandbyFlag)
            Halt();

        sprintf(temp, "clock=%d\r\n", freq);
        if(!mSerial.Send(temp,sizeof(temp)))
        {
            printf("ERROR:  Could not send command\n");
            return 0;
        }
        CxUtils::SleepMs(DELAY_TIME_MS);
        return 1;
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Sets the new baud rate for the compass, disconnects and reconnects.
///
///  \param r The new baud rate (Values or r:  5 = 9600, 6 = 19200, 7 = 38400)
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::SetDeviceBaudRate(const unsigned int r)
{
    char temp[6];
    int  baud;

    if(r < 5 || r > 7)
    {
        printf("ERROR:  Invalid baud rate option\n");
        printf("TCM250::setBaudRate(int r)\n");
        return 0;
    }

    switch(r)
    {
    case 5:
        baud = 9600;
        break;
    case 6:
        baud = 19200;
        break;
    case 7:
        baud = 38400;
        break;
    default:
        baud = 9600;
    }

    if(baud == mSerial.GetBaudRate())
        return 1;
    
    if(!mStandbyFlag)
        Halt();

    sprintf(temp, "b=%d\r\n", r);
    
    if(!mSerial.Send(temp,sizeof(temp)))
    {
        printf("ERROR:  Could not send command\n");
        printf("TCM250::setBaudRate(int r)\n");
        return 0;
    }

    //  Now do a software reset
    SendReboot();

    //  Now we must disconnect
    //  and reconnect at a different
    //  baud rate
    
    mSerial.Disconnect();

    mSerial.Connect(mSerial.GetPortName(), mSerial.GetBaudRate());
    
    return Halt();
}



////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  This command is used with the digital damping command. It sets the
///   time constant to nn samples for the digital damping. Refer to Set Digital
///   Damping command.
///
///  \param t The number of values to sample between 1-255. Default 8.
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::SetTimeConstant(const unsigned int t)
{
    char temp[20];
    int time = 8;
    time = t;
    if(t < 1 || t > 255)
        time = 8;

    if(!mStandbyFlag)
        Halt();

    sprintf(temp, "timeconst=%d\r\n", (unsigned char)time);
    if(!mSerial.Send(temp,sizeof(temp)))
    {
        return 0;
    }
    CxUtils::SleepMs(DELAY_TIME_MS);
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Sets the output word format to be used in response to the Single 
///  update output word command and in continuous output mode. For a full 
///  description of TCM2 output words.
///
///  \param enable If true, uses standard output word, otherwise raw data.
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::SetOutputWord(bool enable)
{
    if(!mStandbyFlag)
        Halt();

    if(enable)
    {
        if(mSerial.Send("sdo=t\r\n",7))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
    else
    {
        if(mSerial.Send("sdo=r\r\n",7))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Enable Fast sampling. This command in conjunction with the clock
///  command will allow the unit to sample up to 30Hz with less accurate
///  magnetometer and compass measurements. The random noise in the
///  compass heading measurements increases to about + 0.3 degrees in this
///  mode. Use this command at sample rates greater than 16 Hz.
///
///  \param enable Enable or disable the feature.
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::EnableFastSampling(bool enable)
{
    if(!mStandbyFlag)
        Halt();

    if(enable)
    {
        if(mSerial.Send("fast=e\r\n",9))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
    else
    {
        if(mSerial.Send("fast=d\r\n",9))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
}
////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Instructs the TCM2 to either enable or disable compass data for 
///  inclusion in the TCM2 standard output word. For a description of the TCM2 
///  standard output word format.
///
///  \param enable Enable or disable the feature.
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::EnableCompassData(bool enable)
{
    if(!mStandbyFlag)
        Halt();

    if(enable)
    {
        if(mSerial.Send("ec=e\r\n",6))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
    else
    {
        if(mSerial.Send("ec=d\r\n",6))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Instructs the TCM2 to either enable or disable magnetometer data for
///  inclusion in the TCM2 standard output word. For a description of the TCM2
///  standard output word format
///
///  \param enable Enable or disable the feature.
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////////
int TCM250::EnableMagnetometerData(bool enable)
{
    if(!mStandbyFlag)
        Halt();

    if(enable)
    {
        if(mSerial.Send("em=e\r\n",6))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
    else
    {
        if(mSerial.Send("em=d\r\n",6))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Enables or disables the magnetic distortion alarm output feature. If
///  enabled, the TCM2 will report the magnetic distortion condition when
///  detected. The magnetic distortion alarm is reported as an error flag in the
///  error code response in TCM2 output messages.
///
///  \param enable Enable or disable the feature.
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::EnableMagneticDistortionAlarm(bool enable)
{
    if(!mStandbyFlag)
        Halt();

    if(enable)
    {
        if(mSerial.Send("ed=e\r\n",6))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
    else
    {
        if(mSerial.Send("ed=d\r\n",6))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Instructs the TCM2 to either enable or disable pitch data for 
///  inclusion in the TCM2 standard output word. For a description of the TCM2 
///  standard output word format.
///
///  \param enable Enable or disable the feature.
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::EnablePitchData(bool enable)
{
    if(!mStandbyFlag)
        Halt();

    if(enable)
    {
        if(mSerial.Send("ep=e\r\n",6))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
    else 
    {
        if(mSerial.Send("ep=d\r\n",6))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Instructs the TCM2 to either enable or disable roll data for inclusion 
///  in the TCM2 standard output word. For a description of the TCM2 standard 
///  output word format.
///
///  \param enable Enable or disable the feature.
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::EnableRollData(bool enable)
{
    if(!mStandbyFlag)
        Halt();

    if(enable)
    {
        if(mSerial.Send("er=e\r\n",6))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
    else
    {
        if(mSerial.Send("er=d\r\n",6))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Instructs the TCM2 to either enable or disable temperature data for
///  inclusion in the TCM2 standard output word. For a description of the TCM2
///  standard output word format.
///
///  \param enable Enable or disable the feature.
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::EnableTemperatureData(bool enable)
{
    if(!mStandbyFlag)
        Halt();

    if(enable)
    {
        if(mSerial.Send("et=e\r\n",6))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
    else
    {
        if(mSerial.Send("et=d\r\n",6))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  This mode can be set to filter the output. The damping uses an 
///  IIR filter on the measured data. It is only enabled during continuous mode.
///
///  \param enable Enable or disable the feature.
///
///  \return 0 on failure, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::EnableDigitalDamping(bool enable)
{
    if(!mStandbyFlag)
        Halt();

    if(enable)
    {
        if(mSerial.Send("damping=e\r\n",11))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
    else
    {
        if(mSerial.Send("damping=d\r\n",11))
        {
            CxUtils::SleepMs(DELAY_TIME_MS);
            return 1;
        }
        else
            return 0;
    }
}
////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Instructs TCM2 to reset. The TCM2 will halt its current activity, 
///  and reset to the operational mode last specified. Thus, the output mode 
///  selected before the "ax" command will remain the same after the warm reboot. 
///  Before resetting, the TCM2 will return a :<cr> to indicate that is has 
///  received the command properly.
///
///  \returns 0 on failure, 1 on success.
/// 
////////////////////////////////////////////////////////////////////////////////////
int TCM250::SendReboot()
{

    if(!mSerial.Send("ax\r\n",4))
    {
        return 0;
    }
    CxUtils::SleepMs(DELAY_TIME_MS);
    return 1;
}




////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  If buff points to the start of a TCM2-50 NMEA formatted message,
///  this method will extract values from it and return the length of the message
///  in bytes.
///
///  \param buff Pointer to a NMEA message (starts with '$').
///  \param size Length of buffer in bytes.
///
///  \return If message present, the length of the message in bytes, otherwise
///          0.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int TCM250::ExtractMessage(const unsigned char *buff, const unsigned int size)
{
    char   *ptr;
    unsigned int result = 0;
    
    //  Check for valid data
    if(buff == NULL || size == 0)
    {
        return 0;
    }
    if( (result = IsValidData(buff, size)) > 0)
    {
        CxUtils::Packet message;
        // Copy the data so we don't accidently
        // get values from another message.
        message.Write(buff, result);
        bool compassCallback = false,
             magnetometerCallback = false,
             temperatureCallback = false;
        mMutex.Lock();
        ptr = (char *)strpbrk((const char*)message.Ptr(), "C");
        if(ptr)
        {
            sscanf(ptr, "C%lf", &mYaw);
            // Convert yaw to between [-pi,pi].
            compassCallback = true;
            if(mYaw >= 180 && mYaw <= 360.0)
            {
                mYaw -= 360;
            }
            // Convert to radians.
            mYaw = CxUtils::CxToRadians(mYaw);
        }

        ptr = (char *)strpbrk((const char*)message.Ptr(), "P");
        if(ptr)
        {
            compassCallback = true;
            sscanf(ptr, "P%lf", &mPitch);
            // Convert to radians.
            mPitch = CxUtils::CxToRadians(mPitch);
        }

        ptr = (char *)strpbrk((const char*)message.Ptr(), "R");
        if(ptr)
        {
            compassCallback = true;
            char *ePtr = (char *)strpbrk(ptr, "E");
            if(ePtr != NULL)
            {
                *ePtr = 0;
            }
            sscanf(ptr, "R%lf", &mRoll);
            // Convert to radians.
            mRoll = CxUtils::CxToRadians(mRoll);
            if(ePtr != NULL)
            {
                *ePtr = 'E';
            }
        }

        // Check for magnetometer data
        ptr = (char *)strpbrk((const char*)message.Ptr(), "X");
        if(ptr != NULL)
        {
            magnetometerCallback = true;
            sscanf(ptr, "X%lf", &mBx);
        }
        ptr = (char *)strpbrk((const char*)message.Ptr(), "Y");
        if(ptr != NULL)
        {
            magnetometerCallback = true;
            sscanf(ptr, "Y%lf", &mBy);
        }
        ptr = (char *)strpbrk((const char*)message.Ptr(), "Z");
        if(ptr != NULL)
        {
            char *ePtr = (char *)strpbrk(ptr, "E");
            if(ePtr != NULL)
            {
                *ePtr = 0;
            }
            magnetometerCallback = true;
            sscanf(ptr, "Z%lf", &mBz);
            if(ePtr != NULL)
            {
                *ePtr = 'E';
            }
        }


        // Check for temperature data
        ptr = (char *)strpbrk((const char*)message.Ptr(), "T");
        if(ptr != NULL)
        {
            char *ePtr = (char *)strpbrk(ptr, "E");
            if(ePtr != NULL)
            {
                *ePtr = 0;
            }
            temperatureCallback = true;
            sscanf(ptr, "T%lf", &mTemp);
            if(ePtr != NULL)
            {
                *ePtr = 'E';
            }

        }

        // Lets look for an error code
        ptr = (char *)strpbrk((const char*)message.Ptr(), "E");
        if(ptr != NULL)
        {
            sscanf(ptr, "E%d", &mError);
        }
        mMutex.Unlock();

        //Assume data above is set in mValues
        if(compassCallback)
        {
            RunDefaultCallback();
        }

        //little complicated, but set flag then allows for the proper
        //callback in the generic list to work
        if(magnetometerCallback)
        {
            RunCallback(Callback::MagnetometerType);
        }
        if(temperatureCallback)
        {
            RunCallback(Callback::TemperatureType);
        }
    }
    
    return result;

}



////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Access function for retrieving data.
///
///  \param bx X dimension magnetometer data
///  \param by Y dimension magnetometer data
///  \param bz Z dimension magnetometer data
///
////////////////////////////////////////////////////////////////////////////////////
void TCM250::GetMagnetometerData(double &bx, double &by, double &bz)
{
    bx = mBx;
    by = mBy;
    bz = mBz;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Returns pitch in degrees.
///
////////////////////////////////////////////////////////////////////////////////////
double TCM250::GetPitch()const
{
    return mPitch;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Returns roll in degrees.
///
////////////////////////////////////////////////////////////////////////////////////
double TCM250::GetRoll() const
{
    return mRoll;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Returns temperature.
///
////////////////////////////////////////////////////////////////////////////////////
double TCM250::GetTemp() const
{
    return mTemp;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Returns X dimension magnetometer data.
///
////////////////////////////////////////////////////////////////////////////////////
double TCM250::GetBx()const
{
    return mBx;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Returns Y dimension magnetometer data.
///
////////////////////////////////////////////////////////////////////////////////////
double TCM250::GetBy()const
{
    return mBy;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Returns Z dimension magnetometer data.
///
////////////////////////////////////////////////////////////////////////////////////
double TCM250::GetBz() const
{
    return mBz;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Returns the last error code received.
///
////////////////////////////////////////////////////////////////////////////////////
int TCM250::GetErrorCode() const
{
    return mError;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Looks at the data in buff and returns number of bytes read if data 
///           is valid.  0 if data is invalid.
///
///  \param buff Pointer to buffer containing message data.
///  \param size Length of buffer in bytes.
///
///  \return 0 if no message in buffer, otherwise message length in bytes.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int TCM250::IsValidData(const unsigned char *buff, const unsigned int size) const
{
    unsigned int result = 0;
    const char* data = (const char *)buff;
    char *ptr = NULL;
    unsigned int len = size;
    unsigned int astrk = 0;
    unsigned int byte1, byte2, check = 0;
    // At a minimum include "$*CC"
    if(len <= 4)
        return result;
    
    if(*data != '$')
        return result;


    // If no '*' then no check sum data present.
    for(unsigned int i = 1; i < len; i++)
    {
        if(data[i] == '*')
        {
            astrk = i;
            break;
        }
    }
    // Asterisk not found.
    if(astrk == 0 || astrk + 2 >= len)
    {
        return result;
    }
    
    // Convert the ASCII Characters for checksum
    // to those readable for comparison.
    byte2 = isdigit(data[astrk + 1]) ? (data[astrk + 1] - '0') : (data[astrk + 1] - 'A' + 10);
    byte1 = isdigit(data[astrk + 2]) ? (data[astrk + 2] - '0') : (data[astrk + 2] - 'A' + 10);

    // Now calculate what the checsum should be.
    for(unsigned int i = 1; i < astrk && i < len; i++)
    {
        check ^= data[i];
    }

    if( byte1 == (check & 0x0F) && 
        byte2 == (check >> 4) )
    {
        result = astrk + 3;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Thread that polls the serial connection for data from the compass.
///
///  \param args A TCM250 object.
///
////////////////////////////////////////////////////////////////////////////////////
void TCM250::ProcessingFunction()
{
    CxUtils::Packet buffer;
    CxUtils::Packet data;
    const unsigned char * ptr;
    unsigned int endPos = 0;
    buffer.Reserve(1000);

    while(mSerialThread.QuitThreadFlag() == false)
    {
        if(mSerial.IsConnected())
        {
            if(mSerial.ReadBytesAvailable())
            {
                mSerial.Recv(data,mSerial.ReadBytesAvailable());
                buffer += data;

                ptr = buffer.Ptr();
                endPos = 0;
                for(unsigned int i = 0; i < buffer.Length();)
                {
                    if(ptr[i] == '$')
                    {
                        unsigned int result = 0;
                        if( (result = ExtractMessage(&ptr[i], buffer.Length() - i)) > 0)
                        {
                            i += result;
                            endPos = i;
                            mValidDataFlag = true;
                            continue;
                        }
                    }
                    i++;
                }
                if(endPos > 0)
                {
                    buffer.Delete(endPos);
                }
                if(buffer.Length() >= 1000)
                {
                    buffer.Clear();
                }
            }
        }
        CxUtils::SleepMs(1);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Runs callbacks for the type of data.
///
///  \param type Type of callback.
///
////////////////////////////////////////////////////////////////////////////////////
bool TCM250::RunCallback(const unsigned char type)
{
    std::map<unsigned char,std::set<Compass::Callback*> >::iterator cb;
    mMutex.Lock();
    cb = mCallbacks.find(type);
    if(cb != mCallbacks.end())
    {
        std::set<Compass::Callback*>::iterator cb2;
        for(cb2 = cb->second.begin();
            cb2 != cb->second.end();
            cb2++)
        {
            if(type==Callback::MagnetometerType)
            {
                ((TCM::Callback *)*cb2)->ProcessMagnetometerData(mBx, mBy, mBz);
            }
            else if(type==Callback::TemperatureType)
            {
                ((TCM::Callback *)*cb2)->ProcessTemperatureData(mTemp);
            }
        }
    }
    mMutex.Unlock();
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Starts continuous output of data from sensor.
///
////////////////////////////////////////////////////////////////////////////////////
bool TCM250::SetContinuousMode()
{
    return StartContinuousOutput() > 0 ? true : false;
}

/*  End of File */
