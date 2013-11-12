////////////////////////////////////////////////////////////////////////////////////
///
///  \file joystick.cpp
///  \brief Simple Joystick class for interfacing to a joystick.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 21 May 2007
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
#include "cxutils/joystick.h"
#include <assert.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <iomanip>

#if defined (WIN32)
#include <windows.h>
#include <mmsystem.h>
#else
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <stdio.h>
#include <sys/ioctl.h>
#endif

#include <map>

const short int JOYSTICK_X_AXIS   =  0;         ///<  Axis on joystick.
const short int JOYSTICK_Y_AXIS   =  1;         ///<  Axis on joystick.
const short int JOYSTICK_Z_AXIS   =  2;         ///<  Axis on joystick.
const short int JOYSTICK_R_AXIS   =  3;         ///<  Axis on joystick.
const short int JOYSTICK_U_AXIS   =  4;         ///<  Axis on joystick.
const short int JOYSTICK_V_AXIS   =  5;         ///<  Axis on joystick.
const short int JOYSTICK_POV_AXIS =  6;         ///<  Axis on joystick (Point of View (POV) Hat.
const short int JOYSTICK_POV_X_AXIS = 7;        ///<  POV X.
const short int JOYSTICK_POV_Y_AXIS = 8;        ///<  POV Y.
   
    
using namespace std;

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class JoystickInterface
    ///   \brief Internal wrapper interface for the Joystick class implementation
    ///          to encapsulate Windowns and Linux code.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JoystickInterface
    {
    public:
        JoystickInterface()
        {
    #ifdef WIN32
    #else
            mJoyFD = 0;
            mJSCalibratorFlag = false;
    #endif
        }
        ~JoystickInterface()
        {
    #ifdef WIN32
    #else
            mJoyFD = 0;
    #endif
        }
    #ifdef WIN32
        JOYINFOEX mJoyInfoEx;               ///<  Joystick data structure.
    #else
        int mJoyFD;                         ///<  Joystick file descriptor.
        struct js_event mJoystickEvent;     ///<  Joystick event data.
        bool mJSCalibratorFlag;             ///<  If JSCalibrator is installed.
        std::map<int, int> mAxisMinimum;    ///<  Minimum axis calibration value.
        std::map<int, int> mAxisMaximum;    ///<  Maximum axis calibration value.
        std::map<int, int> mAxisCenter;     ///<  Center point for axis calibration dat
    #endif
    };
}

using namespace CxUtils;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Joystick::Joystick()
{
    mpAxis = new int[9];
    assert(mpAxis);
    memset(mpAxis, 0, sizeof(int)*9);
    mButtons = 0;
    mID = 0;
    mUpdateFrequency = 25;
    mpJoyInterface = new JoystickInterface();
    mJoystickConnectedFlag = false;
    mJoystickInitializedFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Joystick::~Joystick()
{
    Shutdown();
    if(mpAxis) 
    {
        delete[] mpAxis;
        mpAxis = NULL;
    }
    if(mpJoyInterface)
    {
        delete ((JoystickInterface *)(mpJoyInterface));
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the joystick.
///
///   \param num The joystick ID number.  If set to AnyJoystick (default) then
///   function connects to first available joystick. [0,10].
///   \param freq Frequency for updates in Hz. [1,50].
///   \param calibrationFile Used only in linux with jscalibrator.  Specify
///                          the calibration file data to correct joystick
///                          values. (Optional Field).
///
///   \return 1 if connection made, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Joystick::Initialize(const unsigned int num, 
                         const unsigned int freq,
                         const std::string& calibrationFile)
{
    // Close any previously open connection.
    Shutdown();
    // Get a pointer to our interface wrapper class.
    JoystickInterface* joyInfo = (JoystickInterface*)(mpJoyInterface);
    
    // Try connect to any joystick available.
    if(num == AnyJoystick)
    {
        for(unsigned int i = 0; i <= 10; i++)
        {
            if(Initialize(i, freq, calibrationFile))
            {
                mJoystickInitializedFlag = true;
                return 1;
            }
        }
        return 0;
    }
    
    // Check for invalid frequency.
    if(freq == 0 || freq > 50)
    {
        return 0;
    }
    mUpdateFrequency = freq;
#ifdef WIN32

    joyInfo->mJoyInfoEx.dwSize = sizeof(JOYINFOEX);
    joyInfo->mJoyInfoEx.dwFlags = JOY_RETURNALL;

    if(joyGetPosEx(num, &joyInfo->mJoyInfoEx) != JOYERR_NOERROR)
    {
        return 0;
    }
    mID = num;
    
#else
    char port[256];

    sprintf(port, "/dev/js%d", num);
    joyInfo->mJoyFD = open(port, O_RDONLY);
    if(joyInfo->mJoyFD > 0) 
    {
        mID = num;
        mJoystickInitializedFlag = true;
    }
    else
    {
        sprintf(port, "/dev/input/js%d", num);
        joyInfo->mJoyFD = open(port, O_RDONLY);
        if(joyInfo->mJoyFD > 0) 
        {
            mID = num;
        }
    }
    
    // Check for failure to open port.
    if(joyInfo->mJoyFD <= 0)
    {
        return 0;
    }
    
    /// Check for calibratrion data
    if(joyInfo->mJoyFD > 0 && calibrationFile.empty() == false)
    {
        char name[128];
        if (ioctl(joyInfo->mJoyFD, JSIOCGNAME(sizeof(name)), name) < 0)
            strncpy(name, "Unknown", sizeof(name));
        //cout << "Joystick: " << name << endl;
        FILE *instream = fopen(calibrationFile.c_str(),"r");
        if(instream)
        {
            char numButtons = 0;
            //request number of buttons
            ioctl(joyInfo->mJoyFD, JSIOCGBUTTONS, &numButtons);

            //request number of axes
            int numAxes = 0;
            ioctl(joyInfo->mJoyFD, JSIOCGAXES, &numAxes);;
        
            //cout << "Has configuration File" << endl;
            joyInfo->mJSCalibratorFlag = true;

            char tmpline[1024];            
            int axis=-1;
            ///there is a whole format, I'm just grabbing what I need
            while(fgets(tmpline,1024,instream)!=NULL)
            {
                //cout << tmpline << endl;
                if(strstr(tmpline,"BeginAxis"))
                {
                    axis++;
                }
                if(axis >= 0 && axis < numAxes)
                {
                    if(strstr(tmpline,"        Minimum"))
                    {
                        joyInfo->mAxisMinimum[axis] = atoi(strstr(tmpline,"=") + 2);
                        //cout << "Axis " << axis << " Minimum: " << joyInfo->mAxisMinimum[axis] << endl;
                    }
                    else if(strstr(tmpline,"        Center"))
                    {
                        joyInfo->mAxisCenter[axis] = atoi(strstr(tmpline,"=") + 2);
                        //cout << "Axis " << axis << " Center: " << joyInfo->mAxisCenter[axis] << endl;
                    }
                    else if(strstr(tmpline,"        Maximum"))
                    {
                        joyInfo->mAxisMaximum[axis] = atoi(strstr(tmpline,"=") + 2);
                        //cout << "Axis " << axis << " Maximum: " << joyInfo->mAxisMaximum[axis] << endl;
                    }
                }
            }
            fclose(instream);
        }
        else
        {
            joyInfo->mJSCalibratorFlag = false;
            joyInfo->mAxisMinimum.clear();
            joyInfo->mAxisMaximum.clear();
            joyInfo->mAxisCenter.clear();
        }
    }
    
    if(mCallbackThread.CreateThread(&Joystick::CallbackThreadFunction, this) == 0)
    {
        Shutdown();
        return 0;
    }
    
#endif

    mJoystickConnectedFlag = true;
    // Create thread to poll for joystick data.
    if(mJoystickThread.CreateThread(&Joystick::JoystickThreadFunction, this) == 1)
    {
        return 1;
    }

    mJoystickConnectedFlag = false;
    // If we got here, close any open connections.
    Shutdown();
    
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the joystick interface.
///
///   \return 1.
///
////////////////////////////////////////////////////////////////////////////////////
int Joystick::Shutdown()
{
    // Get a pointer to our interface wrapper class.
    JoystickInterface* joyInfo = (JoystickInterface*)(mpJoyInterface);
    
#ifndef WIN32
    if(joyInfo->mJoyFD != 0)
    {
        close(joyInfo->mJoyFD);
    }
    mCallbackThread.StopThread();
#endif
    mJoystickThread.StopThread();
    
    mUpdateFrequency = 15;
    memset(mpAxis, 0, sizeof(int)*7);
    mButtons = 0;
    mID = 0;
#ifdef WIN32
#else
    joyInfo->mJoyFD = 0;
#endif
    mJoystickConnectedFlag = false;
    mJoystickInitializedFlag = false;
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the value of the joystick axis.
///
///   \param axis The axis number, see Axes enumeration.
///
///   \return Joystick axis value.
///
////////////////////////////////////////////////////////////////////////////////////
int Joystick::GetAxis(const short int axis) const
{
    if(!IsConnected())
        return 0;

    int result = 0;
    if(axis >= 0 && axis < 9) 
    {
        mJoystickMutex.Lock();
        result = mpAxis[axis];
        mJoystickMutex.Unlock();
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the Point of View (POV) HAT values.
///
///   \return Angles of the POV north is 0, east is 90, south is 180, and
///   west is 270.  A value of 65535 is when nothing pressed
///   on POV.
///
////////////////////////////////////////////////////////////////////////////////////
int Joystick::GetPOV() const
{
    int result = 0;

    mJoystickMutex.Lock();

#ifdef WIN32
    result = mpAxis[JOYSTICK_POV_AXIS];
    if(result != PovOff)
        result /= 100;
#else
    result = mpAxis[JOYSTICK_POV_AXIS];
#endif
    mJoystickMutex.Unlock();
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the number of the current button being pressed.
///
///   \return 0 If not button pressed otherwise [1,32].
///
////////////////////////////////////////////////////////////////////////////////////
int Joystick::GetButtonNumber() const
{
    int result = 0, buttons = 0;
    
    // copy buttons vector
    mJoystickMutex.Lock();
    buttons = mButtons;
    mJoystickMutex.Unlock();

    int bnumber = 1;
    for(int i = 0; i < 32; i++) 
    {
        if( (buttons&bnumber) > 0) 
        {
            result = i + 1;
            break;
        }

        bnumber <<= 1;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a callback to receive joystick updates on at a fixed
///   frequency.  This will clear any other callbacks, since only one may
///   be registered at a time.
///
///   \param cb Callback pointer.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Joystick::RegisterCallback(JoystickCallback* cb)
{
    if(!cb)
        return 0;

    mJoyCallbackMutex.Lock();

    Callback::Data cbData;
    
    cbData.mpJoyCallback  = cb;
    cbData.mpJoyFunc = NULL;
    cbData.mpJoyFuncArgs = NULL;
    

    Callback::Data::List::iterator it;
    bool added = false;
    for(it = mCallbacks.begin();
        it != mCallbacks.end();
        it++)
    {
        if(it->mpJoyCallback == cbData.mpJoyCallback &&
           it->mpJoyFunc == cbData.mpJoyFunc &&
           it->mpJoyFuncArgs == cbData.mpJoyFuncArgs)
        {
            added = true;
        }
    }

    if(!added)
    {
        mCallbacks.push_back(cbData);
    }
    
    mJoyCallbackMutex.Unlock();

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a callback to receive joystick updates on at a fixed
///   frequency.  This will clear any other callbacks, since only one may
///   be registered at a time.
///
///   \param func Function pointer callback to use when new joystick data is
///               read.
///   \param args Additional arugments for the function callback.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Joystick::RegisterCallback(void (*func)(const Joystick& jstick, void* args), void* args)
{
    if(!func)
        return 0;

    mJoyCallbackMutex.Lock();
    
    Callback::Data cbData;
    
    cbData.mpJoyCallback  = NULL;
    cbData.mpJoyFunc = func;
    cbData.mpJoyFuncArgs = args;
    
    mCallbacks.push_back(cbData);

    mJoyCallbackMutex.Unlock();

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a callback to receive joystick updates on at a fixed
///   frequency.  This will clear any other callbacks, since only one may
///   be registered at a time.
///
///   \param func Function pointer callback to use when buttons are pressed and
///               released.
///   \param args Additional arugments for the function callback.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Joystick::RegisterCallback(void (*func)(const unsigned int buttonNumber, const Joystick::Event type, void* args), 
                               void* args)
{
    if(!func)
        return 0;

    mJoyCallbackMutex.Lock();
    
    Callback::Data cbData;
    
    cbData.mpJoyCallback  = NULL;
    cbData.mpJoyButtonFunc = func;
    cbData.mpJoyFuncArgs = args;
    
    mCallbacks.push_back(cbData);

    mJoyCallbackMutex.Unlock();

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears callback for automatic updates from joystick.
///
////////////////////////////////////////////////////////////////////////////////////
void Joystick::ClearCallbacks()
{
    mJoyCallbackMutex.Lock();
    mCallbacks.clear();
    mJoyCallbackMutex.Unlock();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints joystick values to console.
///
////////////////////////////////////////////////////////////////////////////////////
void Joystick::PrintJoystickInfo() const
{
    cout << "ID: " << mID << " - ";
    for(int i = 0; i < 6; i++) 
    {
        cout << Joystick::GetLetterFromAxis(i) << ":" << setw(4) << (int)GetAxisPercentage(i) << " ";
    }
    cout << "POV-X: " << GetAxisPercentage(PovX) << " ";
    cout << "POV-Y: " << GetAxisPercentage(PovY) << " ";
    cout << "B: " << GetButtonNumber() << endl;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Find out if a button is pressed or a specific button by number.
///
///   \param num Button numberr [1,32], 0 for any button.
///
///   \return True if the specific button or any button is pressed, otherwise
///   false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Joystick::IsButtonPressed(const unsigned int num) const
{
    bool result = false;
    
    mJoystickMutex.Lock();
    
    if(num == 0 && mButtons != 0)
    {
        result = true;
    }
    else 
    {
        int x = 1;
        x <<= (num - 1);
        if( (x&mButtons) != 0 )
        {
            result = true;
        }
    }
    mJoystickMutex.Unlock();
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if connected to joystick, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Joystick::IsConnected() const
{
    return mJoystickConnectedFlag;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the value of the joystick axis.
///
///   \param axis The axis number, see Axes enumeration.
///   \param deadzone Joystick deadzone percentage. [0,100].
///
///   \return Joystick axis value as a percentage [-100,100].
///
////////////////////////////////////////////////////////////////////////////////////
double Joystick::GetAxisPercentage(const short int axis, const short int deadzone) const
{
    int val = GetAxis(axis);
    double dead = ( abs(deadzone)*MaxAxisValue )/100.0;
    double perc = 0.0;
    if(dead > MaxAxisValue)
        dead = MaxAxisValue;

    if(axis == POV || axis == PovX || axis == PovY)
    {
        return (double)val;
    }

    perc = (abs(val) <= dead) ? 0.0 : ((abs(val) - dead)*100.0)/(MaxAxisValue - dead);

    if(val < 0)
        perc *= -1.0;

    if(perc > 100.0)
        perc = 100.0;
    else if(perc < -100.0)
        perc = -100.0;

    return perc;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Function performed within a thread which continuously reads data
///          from the joystick based on update frequency.
///
////////////////////////////////////////////////////////////////////////////////////
void Joystick::JoystickThreadFunction(void *args)
{
    Joystick* joystick = (Joystick*)(args);
    // Get a pointer to our interface wrapper class.
    JoystickInterface* joyInfo = (JoystickInterface*)(joystick->mpJoyInterface);
    
    std::vector<int> buttonsDown;
    std::vector<int> buttonsUp;
#ifndef WIN32
    int failCount = 0;
#endif
    while(joystick &&
          joystick->mJoystickThread.QuitThreadFlag() == false &&
          joystick->mJoystickConnectedFlag == true)
    {
        buttonsDown.clear();
        buttonsUp.clear();

#if defined(WIN32)

        unsigned int delay;
        delay = (unsigned int)(1000.0/(joystick->mUpdateFrequency + .000000000000000001));
        if(delay < 1)
        {
            delay = 1;         
        }
        SleepMs(delay);
        
        int prevButtons;
        joystick->mJoystickMutex.Lock();
        
        prevButtons = joystick->mButtons;

        //  First get updated information
        ::MMRESULT getPosExResult = JOYERR_UNPLUGGED;
        getPosExResult = joyGetPosEx(joystick->mID, &joyInfo->mJoyInfoEx);

        if(getPosExResult == JOYERR_UNPLUGGED)
        {
            // Unplugged joystick, handle...
            joystick->mJoystickConnectedFlag = false;
        }
        else
        {
            joystick->mJoystickConnectedFlag = true;
            //  Now copy the updated information
            //  Get the button value.  In windows
            //  the button value is returned as
            //  a power of two
            joystick->mButtons = joyInfo->mJoyInfoEx.dwButtons;
            //  Copy Axis Information and Subtract
            //  32767 so that the middle is 0
            joystick->mpAxis[JOYSTICK_X_AXIS] = joyInfo->mJoyInfoEx.dwXpos - 32767;
            joystick->mpAxis[JOYSTICK_Y_AXIS] = joyInfo->mJoyInfoEx.dwYpos - 32767;
            joystick->mpAxis[JOYSTICK_Z_AXIS] = joyInfo->mJoyInfoEx.dwRpos - 32767;
            joystick->mpAxis[JOYSTICK_R_AXIS] = joyInfo->mJoyInfoEx.dwZpos - 32767;
            joystick->mpAxis[JOYSTICK_U_AXIS] = joyInfo->mJoyInfoEx.dwUpos - 32767;
            joystick->mpAxis[JOYSTICK_V_AXIS] = joyInfo->mJoyInfoEx.dwVpos - 32767;        
            joystick->mpAxis[JOYSTICK_POV_AXIS] = joyInfo->mJoyInfoEx.dwPOV;
            if(joyInfo->mJoyInfoEx.dwPOV == PovOff)
            {
                joystick->mpAxis[PovX] = 0;
                joystick->mpAxis[PovY] = 0;
            }
            if(joyInfo->mJoyInfoEx.dwPOV == 0)
            {
                joystick->mpAxis[PovY] = 100;
            }
            if(joyInfo->mJoyInfoEx.dwPOV == 18000)
            {
                joystick->mpAxis[PovY] = -100;
            }
            if(joyInfo->mJoyInfoEx.dwPOV == 9000)
            {
                joystick->mpAxis[PovX] = 100;
            }
            if(joyInfo->mJoyInfoEx.dwPOV == 27000)
            {
                joystick->mpAxis[PovX] = -100;
            }
            if(joyInfo->mJoyInfoEx.dwPOV == 4500)
            {
                joystick->mpAxis[PovX] = 100;
                joystick->mpAxis[PovY] = 100;
            }
            if(joyInfo->mJoyInfoEx.dwPOV == 13500)
            {
                joystick->mpAxis[PovX] = 100;
                joystick->mpAxis[PovY] = -100;
            }
            if(joyInfo->mJoyInfoEx.dwPOV == 22500)
            {
                joystick->mpAxis[PovX] = -100;
                joystick->mpAxis[PovY] = -100;
            }
            if(joyInfo->mJoyInfoEx.dwPOV == 31500)
            {
                joystick->mpAxis[PovX] = -100;
                joystick->mpAxis[PovY] = 100;
            }

            // Now check for button events.
            for(unsigned int i = 0; i < MaxButtons; i++)
            {
                int mask = 1 << i;
                // Check for down
                if( (prevButtons&mask) == 0 && (joystick->mButtons&mask) > 0)
                {
                    buttonsDown.push_back(i + 1);
                }
                // Check for up
                if( (prevButtons&mask) > 0 && (joystick->mButtons&mask) == 0)
                {
                    buttonsUp.push_back(i + 1);
                }
            }
        }
        joystick->mJoystickMutex.Unlock();

#else            
        //  Read for any events
        if(read (joyInfo->mJoyFD, &joyInfo->mJoystickEvent, sizeof(struct js_event)) != -1)
        {
            joystick->mJoystickConnectedFlag = true;
            //printf("Event!\n");
            //  Check the type of event
            if(joyInfo->mJoystickEvent.type == JS_EVENT_BUTTON)
            {
                joystick->mJoystickMutex.Lock();
                //  First see what the button
                //  change too
                if(joyInfo->mJoystickEvent.value == 0)  //  Button released
                {
                    // Get the number of the button release, and clear the
                    // bit for the button in the button vector.
                    joystick->mButtons &= ~(1 << joyInfo->mJoystickEvent.number);
                    buttonsUp.push_back(joyInfo->mJoystickEvent.number + 1);
                }
                else  //  Button pressed
                {
                    //  Get the number pressed and OR with button vector
                    joystick->mButtons |= 1 << joyInfo->mJoystickEvent.number;
                    buttonsDown.push_back(joyInfo->mJoystickEvent.number + 1);
                }
                //printf("Pressed Button %d\n", joystick->mButtons);
                joystick->mJoystickMutex.Unlock();
            }
            else if(joyInfo->mJoystickEvent.type == JS_EVENT_AXIS)
            {
                short int value;
                int axis = 0;
                value = joyInfo->mJoystickEvent.value;
                axis = joyInfo->mJoystickEvent.number;
                joystick->mJoystickMutex.Lock();
                //  First find out which axis
                switch(axis)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        if(joyInfo->mJSCalibratorFlag)
                        {
                            if(value < joyInfo->mAxisCenter[axis])
                            {
                                joystick->mpAxis[axis]=(short int)((MaxAxisValue*(double)(value - joyInfo->mAxisCenter[axis])/(double)(joyInfo->mAxisCenter[axis]-joyInfo->mAxisMinimum[axis])));
                            }
                            else
                            {                                   
                               joystick->mpAxis[axis]=(short int)((MaxAxisValue*(double)(value - joyInfo->mAxisCenter[axis])/(double)(joyInfo->mAxisMaximum[axis]-joyInfo->mAxisCenter[axis])));
                            }
                        }
                        else
                        {
                            joystick->mpAxis[axis] = value;
                        }
                        if(axis == 3)
                        {
                            if(value == 255)
                            {
                                joystick->mpAxis[axis] = 0;
                            }
                            joystick->mpAxis[axis] *= -1;   
                            joystick->mpAxis[axis] = (int)((joystick->mpAxis[axis] - MaxAxisValue/2.0)*MaxAxisValue/ (MaxAxisValue/2.0));
                        }
                        break;
                    case 4:
                        joystick->mpAxis[JOYSTICK_POV_X_AXIS] = value;
                        break;
                    case 5:
                        joystick->mpAxis[JOYSTICK_POV_Y_AXIS] = value;
                        break;
                    case 6:
                        joystick->mpAxis[JOYSTICK_U_AXIS] = value;
                        break;
                    case 7:
                        joystick->mpAxis[JOYSTICK_V_AXIS] = value;
                    default:
                        break;
                }

                double angle = atan2(-(double)joystick->mpAxis[JOYSTICK_POV_Y_AXIS] , (double)joystick->mpAxis[JOYSTICK_POV_X_AXIS]);
                angle = CX_RAD2DEG(angle);
                if(joystick->mpAxis[JOYSTICK_POV_X_AXIS] == 0 &&
                   joystick->mpAxis[JOYSTICK_POV_Y_AXIS] == 0)
                {
                   angle = PovOff;
                }
                joystick->mpAxis[JOYSTICK_POV_AXIS] = angle;

                joystick->mJoystickMutex.Unlock();
            }
        }


#endif

#ifdef WIN32
        // The callbacks are used in in this thread in Windows only
        // because reading from the joystick is non-blocking in Windows.
        joystick->mJoyCallbackMutex.Lock();
        Callback::Data::List::iterator cb;
        for(cb = joystick->mCallbacks.begin();
            cb != joystick->mCallbacks.end();
            cb++)
        {
            if(cb->mpJoyCallback)
            {
                cb->mpJoyCallback->ProcessJoystickEvent(*joystick);
                for(unsigned int i = 0; i < (unsigned int)buttonsDown.size(); i++)
                {
                    cb->mpJoyCallback->ProcessButtonEvent(buttonsDown[i], Joystick::ButtonDown);
                }
                for(unsigned int i = 0; i < (unsigned int)buttonsUp.size(); i++)
                {
                    cb->mpJoyCallback->ProcessButtonEvent(buttonsUp[i], Joystick::ButtonUp);
                }
            }
            else if(cb->mpJoyFunc)
            {
                cb->mpJoyFunc(*joystick, cb->mpJoyFuncArgs);
            }
            else if(cb->mpJoyButtonFunc)
            {
                for(unsigned int i = 0; i < (unsigned int)buttonsDown.size(); i++)
                {
                    cb->mpJoyButtonFunc(buttonsDown[i], Joystick::ButtonDown, cb->mpJoyFuncArgs);
                }
                for(unsigned int i = 0; i < (unsigned int)buttonsUp.size(); i++)
                {
                    cb->mpJoyButtonFunc(buttonsUp[i], Joystick::ButtonUp, cb->mpJoyFuncArgs);
                }
            }
        }
        joystick->mJoyCallbackMutex.Unlock();
#endif
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the character letter represening the axis ('x', 'X') to
///   a number identifier such as JOYSTICK_X_AXIS.
///
///   \param ch The character represening the axis.  Values can be
///             upper or lower case. ('x', 'y', 'z', etc.)
///
///   \return The axis number.  For example 'x' -> JOYSTICK_X_AXIS.
///
////////////////////////////////////////////////////////////////////////////////////
short int Joystick::GetAxisFromLetter(const char ch)
{
    short int result = 0;
    switch(tolower(ch))
    {
    case 'x':
        result = JOYSTICK_X_AXIS;
        break;
    case 'y':
        result = JOYSTICK_Y_AXIS;
        break;
    case 'z':
        result = JOYSTICK_Z_AXIS;
        break;
    case 'r':
        result = JOYSTICK_R_AXIS;
        break;
    case 'u':
        result = JOYSTICK_U_AXIS;
        break;
    case 'v':
        result = JOYSTICK_V_AXIS;
        break;
    default:
        result = 0;
        break;
    };

    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the axis number to a charcter.
///
///   \param axis The axis number. X, Y, Z, R, U, V, etc.
///
///   \return The letter representing the axis, 0 if unknown.
///
////////////////////////////////////////////////////////////////////////////////////
char Joystick::GetLetterFromAxis(const short int axis)
{
    char result = 0;
    switch(axis)
    {
    case JOYSTICK_X_AXIS:
        result = 'X';
        break;
    case JOYSTICK_Y_AXIS:
        result = 'Y';
        break;
    case JOYSTICK_Z_AXIS:
        result = 'Z';
        break;
    case JOYSTICK_R_AXIS:
        result = 'R';
        break;
    case JOYSTICK_U_AXIS:
        result = 'U';
        break;
    case JOYSTICK_V_AXIS:
        result = 'V';
        break;
    default:
        result = 0;
        break;
    };

    return result;
}


#ifndef WIN32

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method that generates Joystick callbacks in Linux.  This is done
///   differently than the windows version of this code because reading from
///   the joystick is a blocking function call.
///
////////////////////////////////////////////////////////////////////////////////////
void Joystick::CallbackThreadFunction(void* args)
{
    Joystick* joystick = (Joystick*)args;
    std::vector<int> buttonsDown;
    std::vector<int> buttonsUp;
    int prevButtons = 0;
    while(joystick && joystick->mCallbackThread.QuitThreadFlag() == false)
    {

        char port[256];
        int fd=-1;
        //JoystickInterface* joyInfo = (JoystickInterface*)joystick->mpJoyInterface;
        sprintf(port, "/dev/js%d", joystick->mID);
        fd = open(port, O_RDONLY);
        if(fd < 0) 
        {
            sprintf(port, "/dev/input/js%d", joystick->mID);
            fd = open(port, O_RDONLY);
            // Check for failure to open port.
            if(fd < 0)
            {
                joystick->mJoystickConnectedFlag = false;
            }
            else
            {
                close(fd);
            }
        }
        
        int curButtons;
        
        joystick->mJoystickMutex.Lock();
        curButtons = joystick->mButtons;
        joystick->mJoystickMutex.Unlock();
        
        buttonsDown.clear();
        buttonsUp.clear();
        
         // Now check for button events.
        for(unsigned int i = 0; i < MaxButtons; i++)
        {
            int mask = 1 << i;
            // Check for down
            if( (prevButtons&mask) == 0 && (curButtons&mask) > 0)
            {
                buttonsDown.push_back(i + 1);
            }
            // Check for up
            if( (prevButtons&mask) > 0 && (curButtons&mask) == 0)
            {
                buttonsUp.push_back(i + 1);
            }
        }
        
        prevButtons = curButtons;
        
        joystick->mJoyCallbackMutex.Lock();
        Callback::Data::List::iterator cb;
        for(cb = joystick->mCallbacks.begin();
            cb != joystick->mCallbacks.end();
            cb++)
        {
            if(cb->mpJoyCallback)
            {
                cb->mpJoyCallback->ProcessJoystickEvent(*joystick);
                for(unsigned int i = 0; i < (unsigned int)buttonsDown.size(); i++)
                {
                    cb->mpJoyCallback->ProcessButtonEvent(buttonsDown[i], Joystick::ButtonDown);
                }
                for(unsigned int i = 0; i < (unsigned int)buttonsUp.size(); i++)
                {
                    cb->mpJoyCallback->ProcessButtonEvent(buttonsUp[i], Joystick::ButtonUp);
                }
            }
            else if(cb->mpJoyFunc)
            {
                cb->mpJoyFunc(*joystick, cb->mpJoyFuncArgs);
            }
            else if(cb->mpJoyButtonFunc)
            {
                for(unsigned int i = 0; i < (unsigned int)buttonsDown.size(); i++)
                {
                    cb->mpJoyButtonFunc(buttonsDown[i], Joystick::ButtonDown, cb->mpJoyFuncArgs);
                }
                for(unsigned int i = 0; i < (unsigned int)buttonsUp.size(); i++)
                {
                    cb->mpJoyButtonFunc(buttonsUp[i], Joystick::ButtonUp, cb->mpJoyFuncArgs);
                }
            }
        }
        joystick->mJoyCallbackMutex.Unlock();

        unsigned int delay;
        delay = (unsigned int)(1000.0/(joystick->mUpdateFrequency + .000000000000000001));
        if(delay < 1)
        {
            delay = 1;         
        }
        SleepMs(delay);
    }
}

#endif


/*  End of File */
