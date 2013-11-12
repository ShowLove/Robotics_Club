/////////////////////////////////////////////////////////////////////////////////////
///
/// \file gps.cpp
/// \brief A simple GPS class capable of receiving data from a Generic GPS
/// using a serial connection
///
/// Author(s): Gary Stein<br>
/// Created: 2/27/2010<br>
/// Copyright (c) 2010<br>
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
#include "gps/gps.h"
#include <cxutils/math/cxmath.h>
#include <cxutils/cxutils.h>

using namespace std;
using namespace Zebulon;
using namespace GPS;


const unsigned char Callback::DefaultCallback=0xFF;

//////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Default constructor.
///
//////////////////////////////////////////////////////////////////////////////////////
GenericGPS::GenericGPS()
{
    mLatitude = mLongitude = mAltitude = 0.0;
    mError = 0;
    mValidDataFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Destructor.  Disconnects from gps.
///
////////////////////////////////////////////////////////////////////////////////////
GenericGPS::~GenericGPS()
{
    Shutdown();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the interface.
///
////////////////////////////////////////////////////////////////////////////////////
void GenericGPS::Shutdown()
{
    mSerialThread.StopThread();
    mSerial.Disconnect();
    mValidDataFlag = false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the connection to the sensor, and verifies there
///  is incomming data.
///
///  \param port Port to connect on.
///  \param ibaud Initial baud rate to try connect at.  If this fails, other
///         supporting BAUD is attempted.
///  \param timeout How long to wait for incomming data before time out.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int GenericGPS::Initialize(const std::string& port,
                           const unsigned int ibaud,
                           const bool tryOtherBauds,
                           const unsigned int timeout)
{
    Shutdown();
    int result = 0;
    unsigned int baud = 115200;
    CxUtils::Time::Stamp start = 0;
    
    // Start the serial thread which will
    // be used to receive all incomming messages.
    if(mSerialThread.CreateThread(&SerialThreadFunction, this))
    {
#ifdef _DEBUG
        std::cout << "Establishing connection to " << mName << " device on " << port << "...\n";
        std::cout << "Trying baud " << ibaud << "...";
#endif
        //  Now connect at the initial baud rate
        if(mSerial.Connect(port.c_str(), ibaud))
        {

            // Wait and see if we received any messages.
            start = CxUtils::GetTimeMs();
            while(CxUtils::GetTimeMs() - start < timeout && result == 0)
            {
                SetContinuousMode();
                CxUtils::SleepMs(1);
                
                if(mValidDataFlag)
                {
#ifdef _DEBUG
                    std::cout << "Success!\n";
#endif
                    result = 1;
                }
                CxUtils::SleepMs(1);
            }

            if( result == 0 )
            {
                mSerial.Flush();
                mSerial.Shutdown();
#ifdef _DEBUG
                std::cout << "Failure.\n"; 
#endif
                while(tryOtherBauds && baud <= 115200 && result == 0)
                {
                    // Don't retry the default baud
                    if(baud != ibaud)
                    {
#ifdef _DEBUG
                        std::cout << "Trying baud " << baud << "...";
#endif
                        if(mSerial.Connect(port.c_str(), baud))
                        {
                            // Wait and see if we received any messages.
                            start = CxUtils::GetTimeMs();
                            while( CxUtils::GetTimeMs() - start < timeout && result == 0)
                            {
                                SetContinuousMode();
                                CxUtils::SleepMs(1);
                                if(mValidDataFlag)
                                {
#ifdef _DEBUG
                                    std::cout << "Success!\n";
#endif
                                    result = 1;
                                }
                                CxUtils::SleepMs(1);
                            }
                        }
                        if(result == 0)
                        {
#ifdef _DEBUG
                            std::cout << "Failure.\n"; 
#endif
                            // If no connection made, flush and
                            // shutdown the connection.
                            mSerial.Flush();
                            mSerial.Shutdown();
                        }
                    }

                    //  Select a different baudrate.
                    switch(baud)
                    {
                    case 9600:
                        baud = 19200;
                        break;
                    case 19200:
                        baud = 38400;
                        break;
                    case 38400:
                        baud = 57600;
                        break;
                    case 57600:
                        baud = 115200;
                        break;
                    default:
                        baud = 115201;
                        break;
                    }// Switch to different Baud.
                }// While not connected and baud <= 115200.
            }// If not connected yet.
        }// If connection to serial port was made.
        else
        {
            std::cout << "Failure. Could not connect to " << port << "\n";
        }
    }// If thread was started.

    if(result == 0)
    {
        // Flush everything and shutdown.
        Shutdown();
    }
    return result;
}


void GenericGPS::SerialThreadFunction(void *args)
{
    //Pass in as a void *, then cast, then virtuals should work
    GenericGPS* gps = (GenericGPS*)args;
    //call the appropriate real processing function per gps
    gps->ProcessingFunction();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all registered callbacks.
///
////////////////////////////////////////////////////////////////////////////////////
void GenericGPS::ClearCallbacks()
{
    mMutex.Lock();
    mCallbacks.clear();
    mMutex.Unlock();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to register a callback to be run whenever a specific
///   type of message is received.
///
///   \param[in] type The type of message to subscribe to.
///   \param[in] cb The callback to be run when the message is received.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool GenericGPS::RegisterCallback(Callback* cb,const unsigned char type)
{
    if(cb!=NULL)
    {
        mMutex.Lock();
        mCallbacks[type].insert(cb);
        mMutex.Unlock();
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to register a callback to be run whenever a specific
///   type of message is received (default version).
///
///   \param[in] type The type of message to subscribe to (default case.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool GenericGPS::RunDefaultCallback()
{
    std::map<unsigned char,std::set<Callback*> >::iterator cb;
    mMutex.Lock();
    //in default case it should be the default context
    //assume data already loaded in mValues
    cb = mCallbacks.find(Callback::DefaultCallback);
    if(cb != mCallbacks.end())
    {
        std::set<Callback*>::iterator cb2;
        for(cb2 = cb->second.begin();
            cb2 != cb->second.end();
            cb2++)
        {
            (*cb2)->ProcessGPSData(mLatitude, mLongitude, mAltitude);
        }
    }
    mMutex.Unlock();
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to unregister a callback.
///
///   \param[in] type The type of message to subscribe to.
///   \param[in] cb The callback to be run when the message is received.
///
////////////////////////////////////////////////////////////////////////////////////
bool GenericGPS::RemoveCallback(Callback* cb,const unsigned char type)
{
    std::map<unsigned char,std::set<GPS::Callback*> >::iterator cb1;
    mMutex.Lock();
    cb1 = mCallbacks.find(type);
    if(cb1 != mCallbacks.end())
    {
        std::set<Callback*>::iterator cb2;
        cb2 = cb1->second.find(cb);
        if(cb2 != cb1->second.end())
        {
            cb1->second.erase(cb2);
        }
    }
    mMutex.Unlock();
    return true;
}
