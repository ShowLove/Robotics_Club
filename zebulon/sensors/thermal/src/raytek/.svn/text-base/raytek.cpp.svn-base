//////////////////////////////////////////////////////////////////////////////////////
///
/// \file raytek.cpp
/// \brief Interface to the Raytek thermal sensor
///
/// Author(s): Andrew Watson<br>
/// Created: 5/08/2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: andrewwatson@knights.ucf.edu<br>
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

#include "raytek/raytek.h"
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <cstring>

using namespace Zebulon;
using namespace Raytek;

const std::string Interface::SPADE = "SPADE";
const std::string Interface::HEART = "HEART";
const std::string Interface::DIAMOND = "DIAMOND";
const std::string Interface::CLUB = "CLUB";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Interface::Interface()
{
    mTemperature = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Interface::~Interface()
{
    ShutdownCom();
}



////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes communication.
///
///  \param[in] port Port to connect on.
///  \param[in] serverIP Server IP address.
///
////////////////////////////////////////////////////////////////////////////////////
int Interface::InitializeCom(const unsigned short port,
                         const CxUtils::IP4Address& serverIP)
{
    ShutdownCom();
    return mTcp.InitializeSocket(serverIP, port);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Shutdown communication.
///
////////////////////////////////////////////////////////////////////////////////////
void Interface::ShutdownCom()
{
    this->mTcp.Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends PING message.
///
///  \return True of PONG received = success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Interface::SendPing() const
{
    std::string message = "PING\n";
    CxUtils::Packet response;
    if(mTcp.Send(message.c_str(), (unsigned int)message.size()))
    {
        response.Reserve(512);
        if(mTcp.Recv(response, 4, 100))
        {
            if(strstr((const char *)response.Ptr(), "PONG") != NULL)
            {
                return true;
            }
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends the location of what we believe the "hot" target is.
///
///  \param[in] typeName Type of targe (see name labels in class header).
///  \param[in] position GPS position of target.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Interface::SendLocation(const std::string& typeName,
                            CxUtils::Wgs& position) const
{
    std::stringstream str;

    str << "REPORT;UCF:" << typeName
        << "@" << position.mLatitude << "," << position.mLongitude << "\n";
    CxUtils::Packet response;
    if(mTcp.Send(str.str().c_str(), (unsigned int)str.str().size()))
    {
        response.Reserve(512);
        if(mTcp.Recv(response, 256, 100))
        {
            if(strstr((const char *)response.Ptr(), "SUCCESS") != NULL)
            {
                return true;
            }
            else
            {
                std::cout << "COM Received: " << response.Ptr();
            }
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Establishes a connection to the device by searching different
///   BAUDS on a port.
///
///   \param[in] port The port name to connect on.
///   \param[in] baud The initial/first BAUD to try.
///   \param[in] parity If parity needs to be define, when using FPGA set to even.
///
///   \return True if connection established, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Interface::Initialize(const std::string& port, const unsigned int baud, const bool tryOtherBauds, const unsigned int parity)
{
    bool result = false;

    // Setup the possible baud rates we can talk to
    // Coral with, starting with initial baud rate passed to method.
    if(tryOtherBauds==true)
    {
        std::vector<unsigned short> baudsToTry;
        baudsToTry.push_back(baud);
        for(unsigned int b = 9600; b <= 115200; b*= 2)
        {
            if(b != baud)
            {
                baudsToTry.push_back(b);
            }
        }

        for(unsigned int i = 0; i < (unsigned int)baudsToTry.size(); i++)
        {
            mSerial.Disconnect();
            if(mSerial.Connect(port, baud, 8, parity)) // Connects to serial port at specified baud.
            {
                if(mSerial.IsConnected())
                {
                    mSerialThread.CreateThread(&Interface::SerialThread, this);
                    result = true;
                    break;
                }

            }
        }
    }
    //use fixed baud
    else
    {
        mSerial.Disconnect();
        if(mSerial.Connect(port, baud, 8, parity)) // Connects to serial port at specified baud.
        {
            if(mSerial.IsConnected())
            {
                result = true;
                mSerialThread.CreateThread(&Interface::SerialThread, this);
            }

        }
    }
    CxUtils::SleepMs(100);
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the interface.
///
////////////////////////////////////////////////////////////////////////////////////
void Interface::Shutdown()
{
    mSerialThread.StopThread();
    mSerial.Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks connection status.
///
////////////////////////////////////////////////////////////////////////////////////
bool Interface::IsConnected() const
{
    if(mSerial.IsConnected())
    {
        return true;
    }
        return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends the message over the serial port to the device.
///
///   \param[in] message Pointer to message structure to transmit.
///   \param[in] postSendDelayMs Number of milliseconds to sleep/block after
///                              a send operation has occured.  This is done
///                              to prevent oversending data to the MCU.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Interface::Send(const char* message, const unsigned int postSendDelayMs)
{

    if(mSerial.Send(message))
    {
        CxUtils::SleepMs(postSendDelayMs);
        return true;
    }
    return false;

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a thread which continuously receives and parsed data from
///   serial port.
///
////////////////////////////////////////////////////////////////////////////////////
void Interface::SerialThread(void* arg)
{
    Interface* raytek = (Interface*)arg;
    while(raytek->mSerialThread.QuitThreadFlag() == false)
    {
        if(raytek->mSerial.IsConnected())
        {
            char msg[10];
            char smsg[4];
            double i;
            sprintf(smsg,"?T\r");
            if(raytek->mSerial.Send(smsg,4) > 0)
            {
                if (raytek->mSerial.Recv(msg,10) > 0)
                {
                    double temp = 0;
                    //sscanf seems to alwasy return 0
                    int result = sscanf(msg,"!T%lf\n@",&temp);
                   // if(result > 0)
                    {
                        CxUtils::Mutex::ScopedLock lock(&raytek->mSerialMutex);
                        raytek->mTemperature = temp;
                    }
                }
            }

            CxUtils::SleepMs(50);
        }
        else
        {
            std::cout << "Raytek::Interface::SerialThread::ERROR - Serial port not connected\n";
            return;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Grabs current temperature.
///
////////////////////////////////////////////////////////////////////////////////////
double Interface::GetTemp() const
{
    CxUtils::Mutex::ScopedLock lock(&mSerialMutex);
    return mTemperature;
}
