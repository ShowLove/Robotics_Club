/////////////////////////////////////////////////////////////////////////////////////
///
/// \file sick.cpp
/// \brief LIDAR Interface.
///
/// Author(s): Daniel Barber<br>
/// Created: 3/22/2008<br>
/// Last Modified: 4/17/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu<br>
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
#include "sick/sick.h"
#include "sick/messages/settingsmode.h"
#include "sick/messages/requeststatusmode.h"
#include "sick/messages/configure.h"
#include "sick/messages/switchscanning.h"
#include "sick/messages/switchoperatingmoderesponse.h"
#include "sick/messages/confirmvariantswitching.h"
#include "sick/messages/confirmationconfiguration.h"
#include "sick/messages/continuousoutputresponse.h"
#include "sick/messagefactory.h"
#include <string.h>
#include <cxutils/cxutils.h>

using namespace CxUtils;
using namespace Zebulon;
using namespace Laser;
using namespace SICK;

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///////////////////////////////////////////////////////////////////////////////
Sick::Sick() : GenericLaser("SICK"),
               mLaserID(0x00),
               mMaxQueueSize(10),
               mLaserAngle(0)
{
    mDistanceMode = 0.001;
    mScanningAngle = CxUtils::CxToRadians(180.0);
    mStartAngle = CxUtils::CxToRadians(-90.0);
    mAngularResolution = CxUtils::CxToRadians(0.5);
    mPossibleBaudRates.push_back(460800);
    mPossibleBaudRates.push_back(38400);
    mPossibleBaudRates.push_back(19200);
    mPossibleBaudRates.push_back(9600);
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
///////////////////////////////////////////////////////////////////////////////
Sick::~Sick()
{
    Shutdown();
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Connects to the serial port of the SICK laser.
///
///  This function attempts to connect to the SICK laser over various baud rates
///  If the SICK laser is connected at a standard baud rate of 500K, 38400,
///  19200 or 9600, it should be detected (in that order). You can also specify
///  a baud rate to try first (if you think you know what baud rate to connect
///  at, trying this one first should speed things along in the initialization)
///
///  \param[in] port The serial port to connect to.
///  \param[in] baud The baud rate at which to try to connect to first.  If
///                  this baud fails, then others will be automatically 
///                  attempted.
///  \param[in] tries The number of tries to "ping" the laser and wait for a
///                   response (default is 1, try more if you need it)
///
///  \return True on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
bool Sick::Initialize(const std::string& port,
                      const int baud,
                      const unsigned char id,
                      const int tries)
{
    unsigned int timeToWaitForResponse = 5000;
    mPort = port;
    std::vector<int> possibleBauds;
    bool connected = false;

    SettingsMode settingsModeMessage;
    Packet settingsModeResponse;

    // Make sure we are shutdown first.
    Shutdown();

    // Create receiving thread which acquires all
    // incomming message data.
    if(mReceiveThread.CreateThread(Sick::ReceiveThread, this) == 0)
    {
        return 0; // Could not create thread for receiving.
    }

    // Name threads for debugging purposes.
    mReceiveThread.SetThreadName("SICK_RX");

    // Allow time for thread(s) to start.
    SleepMs(100);

    possibleBauds = mPossibleBaudRates;
    possibleBauds.insert(possibleBauds.begin(), baud);

    settingsModeMessage.SetAddress(id);
    Message::Queue::iterator message;

    for(int tryNumber = 0; tryNumber < tries && connected == false; tryNumber++ )
    {
        std::vector<int>::iterator baudIter;

        for ( baudIter = possibleBauds.begin();
            baudIter != possibleBauds.end();
            baudIter++ )
        {
            connected = false;
            if (mSerial.Connect(port, *baudIter) )
            {
                // Test our connection to the laser but telling it to
                // stop continuous output.  This will generated a confirmation response.
                SettingsMode stopContinuousOutput(SettingsMode::StopContinuousOutput);
                stopContinuousOutput.SetAddress(id); // Set ID of the laser.
                Message* responseMessage = NULL;
                if((responseMessage = Transmit(&stopContinuousOutput, Message::SwitchOperatingModeResponse, 5000)) != NULL)
                {
                    // Confirm information.
                    SwitchOperatingModeResponse* confirm = dynamic_cast<SwitchOperatingModeResponse*>(responseMessage);
                    if(confirm && confirm->GetResponse() == SwitchOperatingModeResponse::Success)
                    {
                        connected = true;
                        mLaserID = id;
                        delete responseMessage;
                        break;
                    }
                    // Make sure to delete response message.
                    delete responseMessage;
                }
                // Disconnect and try again at new BAUD.
                mSerial.Disconnect();
            }
        }
    }

    if(connected == false)
    {
        Shutdown();
    }

    return connected;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes a connection to a SICK Lidar with default
///         settings (180 degrees, .5 angle increment, MM distance mode).
///
///  \param[in] port The serial port to connect to.
///  \param[in] baud The baud rate at which to try to connect to first.  If
///                  this baud fails, then others will be automatically 
///                  attempted.
///
///  \return True on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
bool Sick::InitializeWithDefaults(const std::string& port, const int baud)
{
    if(Initialize(port, baud))
    {
        if(SetDistanceMode(Sick::MM))
        {
            if(SetScanningMode(OneEighty, Half))
            {
                if(RequestContinuousOutput())
                {
                    return true;
                }
            }
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes a connection to a SICK Lidar with parameters.
///
///  \param[in] params parameters structure.
///
///  \return True on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
bool Sick::Initialize(const GenericLaser::Parameters* params)
{
    if(Initialize(params->mSerialPort,
                  params->mBaudRate,
                  0x00,
                  params->mTries))
    {
        if(SetDistanceMode(Sick::MM))
        {
            if(SetScanningMode(OneEighty, Half))
            {
                if(RequestContinuousOutput())
                {
                    return true;
                }
            }
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Cleans up the state of the SICK Laser.
///
///  This function sends a command to the laser so it stops sending
///  continuous output messages. It also stops the message listener thread
///  and clears/frees the messages in the message queue.
///
///////////////////////////////////////////////////////////////////////////////
void Sick::Shutdown()
{
    if(mSerial.IsConnected())
    {
        StopContinuousOutput();
    }

    mReceiveThread.StopThread();

    // Clean up memory.
    mMessageQueueMutex.Lock();
    Message* msg;
    while(mMessageQueue.size() != 0)
    {
        msg = mMessageQueue.front();
        delete msg;
        mMessageQueue.pop_front();
    }
    while(mContinousOutputMessageQueue.size() != 0)
    {
        msg = mContinousOutputMessageQueue.front();
        delete msg;
        mContinousOutputMessageQueue.pop_front();
    }
    mMessageQueueMutex.Unlock();

    mLaserID = 0x00;
    mSerial.Disconnect();
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends a request for the laser to begin sending scanning output
///         messages as soon as they are received.
///
///  \param[in] timeoutMs How long to wait in milliseconds before timeout waiting
///                       for LMS2xx response.
///
///  \return True on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
bool Sick::RequestContinuousOutput(const unsigned int timeoutMs)
{
    bool result = false;

    SettingsMode request(SettingsMode::ContinuousOutput);
    request.SetAddress(mLaserID);
    Message* responseMessage = NULL;
    if((responseMessage = Transmit(&request, Message::SwitchOperatingModeResponse, 5000)) != NULL)
    {
        // Confirm information.
        SwitchOperatingModeResponse* confirm = dynamic_cast<SwitchOperatingModeResponse*>(responseMessage);
        if(confirm && confirm->GetResponse() == SwitchOperatingModeResponse::Success)
        {
            result = true;
        }

    }
    // Make sure to delete response message.
    if(responseMessage)
    {
        delete responseMessage;
    }

    return result;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends a request to the laser to stop sending continuous output
///
///  \param[in] timeoutMs How long to wait in milliseconds before timeout waiting
///                       for LMS2xx response.
///
///  \return True on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
bool Sick::StopContinuousOutput(const unsigned int timeoutMs)
{
    bool result = false;

    SettingsMode request(SettingsMode::StopContinuousOutput);
    request.SetAddress(mLaserID);
    Message* responseMessage = NULL;
    if((responseMessage = Transmit(&request, Message::SwitchOperatingModeResponse, 5000)) != NULL)
    {
        // Confirm information.
        SwitchOperatingModeResponse* confirm = dynamic_cast<SwitchOperatingModeResponse*>(responseMessage);
        if(confirm && confirm->GetResponse() == SwitchOperatingModeResponse::Success)
        {
            result = true;
        }

    }
    // Make sure to delete response message.
    if(responseMessage)
    {
        delete responseMessage;
    }

    return result;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends the command to change baud rate, and then reconnects at that 
///  new baud rate.
///
///  Most high speed cards use non-standard baudrates.  Therefore the extra
///  parameter cbaud has been added.  The first parameter is what baud rate
///  you want to tell the SICK to operate at, 9600, 19200, 38400, & 500Kbs.
///  The second parameter is the baud rate to set your serial port to.  
///  For example, on the Quatech SSP200/300 card, 430.7Kbps = 500Kbps. 
///  Go figure.
///
///  \param[in] baud What baud rate to configure the SICK to.
///  \param[in] cbaud What baud to configure the PC serial port to.  
///                   If zero, then the value of baud is used.  This
///                   value may be different than baud if you are using
///                   a custom serial card that has been modified.
///
///  \return True on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
bool Sick::ChangeBaudRate(int baud, int cbaud)
{    
    bool result = false;
    SettingsMode request;

    //check if we are connected.
    if(!mSerial.IsConnected())
    {
        return result;
    }
    //if baud rate is already set to desired, return true
    if(mSerial.GetBaudRate() == baud)
    {
        return true;
    }
    
    //  Create our settings mode message with desired baud
    switch(baud)
    {
    case 9600:
        request = SettingsMode(SettingsMode::SetBaud9600);
        break;
    case 19200:
        request =  SettingsMode(SettingsMode::SetBaud19200);
        break;
    case 38400:
        request =  SettingsMode(SettingsMode::SetBaud38400);
        break;
    case 500000:
        request =  SettingsMode(SettingsMode::SetBaud460800);
        break;
    default:
        baud = 9600;
        request =  SettingsMode(SettingsMode::SetBaud9600);
        break;
    }
    int connectBaud;
    if(cbaud == 0)
    {
        connectBaud = baud;
    }
    else
    {
        connectBaud = cbaud;
    }
    // Send the messages, and try to reinitialize at the new speed.
    Message* responseMessage = NULL;
    if((responseMessage = Transmit(&request, Message::SwitchOperatingModeResponse, 5000)) != NULL)
    {
        // Confirm information.
        SwitchOperatingModeResponse* confirm = dynamic_cast<SwitchOperatingModeResponse*>(responseMessage);
        if(confirm && confirm->GetResponse() == SwitchOperatingModeResponse::Success)
        {
            result = true;
        }
    }
    if(result)
    {
        return Initialize(mPort, connectBaud, mLaserID, 1);
    }

    return result;

}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends a request to the laser to change the distance mode.  This
///         method may take up to 8 seconds to complete because of response
///         time from sensor.
///
///  This method will only work correctly before continuous output is 
///  requested.
///
///  \param[in] mode The new distance mode that the Laser will use.
///
///  \return true if the message was sent. false otherwise.
///
///////////////////////////////////////////////////////////////////////////////
bool Sick::SetDistanceMode(const Sick::DistanceModes mode)
{
    bool result = false;

    
    SettingsMode settingsModeMessage;
    Message* responseMessage1 = NULL;
    
    settingsModeMessage.SetAddress(mLaserID);
    if((responseMessage1 = Transmit(&settingsModeMessage, Message::SwitchOperatingModeResponse, 5000)) != NULL)
    {
        // Confirm information mode switch.
        SwitchOperatingModeResponse* confirm = dynamic_cast<SwitchOperatingModeResponse*>(responseMessage1);
        if(confirm && confirm->GetResponse() == SwitchOperatingModeResponse::Success)
        {
            Configure configure;
            configure.SetDistanceMode((unsigned char)mode);
            configure.SetAddress(mLaserID);

            Message* responseMessage2 = Transmit(&configure, Message::ConfirmationConfiguration, 10000);
            if(responseMessage2)
            {
                ConfirmationConfiguration* confirm = dynamic_cast<ConfirmationConfiguration*>(responseMessage2);
                if(confirm && confirm->GetDistanceMode() == mode)
                {
                    switch(mode)
                    {
                    case CM:
                        mDistanceMode = 0.01;
                        break;
                    case MM:
                        mDistanceMode = 0.001;
                        break;
                    }
                    result = true;
                }
                delete responseMessage2;
            }
        }
        // Make sure to delete response message.
        delete responseMessage1;
    }

    return result;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends a request to the laser to change the scanning angle
///         and the angular resolution.
///
///  This method will only work correctly before continuous output is 
///  requested.
///
///  Function seems currently broken, unable to SetScanning.
///
///  \param[in] angle The new scanning angle that the Laser will use.
///  \param[in] resolution The new angular resolution that the Laser will use.
///
///  \return True on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
bool Sick::SetScanningMode(const Sick::ScanningAngle angle, 
                           const Sick::AngularResolution resolution)
{
    bool result = false;

    if (mSerial.IsConnected())
    {
        SwitchScanning switchScanning(angle, resolution);
        Message* responseMessage = NULL;
        if((responseMessage = Transmit(&switchScanning, Message::ConfirmVariantSwitching, 5000)) != NULL)
        {
            result = true;

            switch(angle)
            {
            case OneEighty:
                mScanningAngle = CxUtils::CxToRadians(180.0);
                mStartAngle = CxUtils::CxToRadians(-90.0);
                break;
            case OneHundred:
                mScanningAngle = CxUtils::CxToRadians(100.0);
                mStartAngle = CxUtils::CxToRadians(-40.0);
                break;
            }

            switch(resolution)
            {
            case One:
                mAngularResolution = CxUtils::CxToRadians(1.0);
                break;
            case Half:
                mAngularResolution = CxUtils::CxToRadians(0.5);
                break;
            case Quarter:
                mAngularResolution = CxUtils::CxToRadians(0.25);
                break;
            }
            delete responseMessage;
        }
    }

    return result;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief This method is used to send a SICK LIDAR message to the laser
///         and wait for the response to arrive.
///
///  You must delete the response message received or you will cause a
///  memory leak.
///
///  \param[in] message The message to send to the laser.
///  \param[in] desiredResponse The message type of the response message
///                             you expect to receive.
///  \param[in] waitTimeMs How long to wait in ms for the response to
///                        be received.
///
///  \returns On success, a pointer to the response message is returned
///           (you must delete when done).  A NULL pointer on failure.
///
///////////////////////////////////////////////////////////////////////////////
Message* Sick::Transmit(const Message *message, 
                        Message::Type desiredResponse, 
                        const unsigned int waitTimeMs)
{
    Packet messagePayload;
    Message* responseMessage = NULL;
    // Convert from data structure to byte formmated data.
    message->Write(messagePayload);
    // Now try send the data.
    if(mSerial.Send(messagePayload))
    {
        // Now we must wait for the desired response message to arrive.
        Time::Stamp startTimeMs = Time::GetUtcTimeMs();
        while((unsigned int)(Time::GetUtcTimeMs() - startTimeMs) <= waitTimeMs && 
            responseMessage == NULL)
        {
            Message::Queue::iterator r;
            mMessageQueueMutex.Lock();
            for(r = mMessageQueue.begin();
                r != mMessageQueue.end();
                r++)
            {
                if( (*r)->GetMessageType() == desiredResponse)
                {
                    // SUCCESS!
                    responseMessage = (*r);
                    mMessageQueue.erase(r);
                    break;
                }
            }
            mMessageQueueMutex.Unlock();
            SleepMs(1);
        }
    }
    return responseMessage;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Scans a packet for a valid message and creates the message
///
///  This function attempts to create a message by iterating through the
///  packet and passing it to the message creator.
///
///  \param[in] data A packet to scan for a complete message
///
///  \return A pointer to the message that was found. NULL otherwise.
///
///////////////////////////////////////////////////////////////////////////////
Message* Sick::FindAndCreateMessage(const Packet& data)
{
    Message* msg = NULL;

    int pos = 0;
    int stopPos = data.Length() - Message::MIN_MESSAGE_SIZE;
    while ( pos < stopPos &&
        NULL == msg )
    {
        data.SetReadPos(pos);
        msg = MessageFactory::CreateMessage(data);
        pos++;
    }

    return msg;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief The function used by the thread to receive incoming messages
///
///  This function reads data from the Laser's serial connection until
///  the QuitThreadFlag of the thread is set to true from outside the thread.
///  Any data that is received is scanned for the STX character which signifies
///  the beginning of a message. Then a message is attempted to be created
///  from that data. The message is then stored, deleted from the buffer, and
///  the process is repeated.
///
///  \param args A pointer to the Laser object to receive data from.
///
///////////////////////////////////////////////////////////////////////////////
void Sick::ReceiveThread(void* args)
{
    Sick* laser = reinterpret_cast<Sick*>(args);

    unsigned char* streamBuffer = NULL;
    Packet buffer, receivedData, messagePacket;
    buffer.Reserve(5000);
    receivedData.Reserve(5000);

    while ( !laser->mReceiveThread.QuitThreadFlag() )
    {
        if( laser->mSerial.IsConnected() )
        {
            if ( laser->mSerial.ReadBytesAvailable() > 0 )
            {
                memset((void *)buffer.Ptr(), 0, buffer.Length());
                buffer.Clear();
                if(laser->mSerial.Recv(buffer, laser->mSerial.ReadBytesAvailable()) > 0)
                {
                    receivedData += buffer;
                }

                // Get a pointer to the data buffer.
                streamBuffer = (unsigned char*)receivedData.Ptr();
                // searchEndPosition indicates the last point in
                // the receivedDataBuffer where no new messages
                // were discovered (or the end of the last message found
                // in the buffer).
                unsigned int searchEndPosition = 0;
                // Search through this buffer and look for
                // well formatted SICK LMS messages.
                for(unsigned int i = 0; i < receivedData.Length() - 1;)
                {
                    // If STX is found (signals start of all messages), then
                    // try to extract a complete message.
                    if(streamBuffer[i] == Message::STX)
                    {
                        if(Message::ExtractMessage(&streamBuffer[i], receivedData.Length() - i, messagePacket) )
                        {
                            laser->mValidDataFlag = true;
                            // Advance the search position by the number
                            // of bytes read
                            i += messagePacket.Length();
                            // Try to de-serialize the message data.
                            Message* finalMessage = MessageFactory::CreateMessage(messagePacket);
                            if(finalMessage != NULL)
                            {
                                // Add read message to our queue
                                finalMessage->SetMessageTimeStamp(GetTimeMs());
                                // If a continous output message, update the timestamp.
                                ContinuousOutputResponse * cor = dynamic_cast<ContinuousOutputResponse*>( finalMessage );
                                CxUtils::Point3D::List scan;
                                if(cor)
                                {
                                    if(laser->mSerial.GetBaudRate() != 0)
                                    {
                                        cor->mTimeStamp.SetCurrentTime();
                                        // Adjust the time of the data collected based on how long it
                                        // takes to receive the message.
                                        double scantime = ((double)(cor->mPoints.size() * 32))/((double)(laser->mSerial.GetBaudRate()));
                                        cor->mTimeStamp = CxUtils::Time::Subtract(CxUtils::Time(true), CxUtils::Time(scantime));
                                    }
                                    // Build scan data in polar coordinates.
                                    std::vector<unsigned short>::reverse_iterator range;
                                    double z = laser->mStartAngle;
                                    for(range = cor->mPoints.rbegin();
                                        range != cor->mPoints.rend();
                                        range++)
                                    {
                                        scan.push_back(CxUtils::Point3D((*range)*laser->mDistanceMode,
                                                                        0,
                                                                        z));
                                        z += laser->mAngularResolution;
                                    }
                                }
                                
                                laser->mLaserMutex.Lock();
                                laser->mLaserScan=scan;
                                laser->mLaserMutex.Unlock();
                                
                                // Trigger callbacks.
                                Callback::Set::iterator callback;
                                laser->mCallbacksMutex.Lock();
                                for(callback = laser->mCallbacks.begin();
                                    callback != laser->mCallbacks.end();
                                    callback++)
                                {
                                    if(cor)
                                    {
                                        (*callback)->ProcessLaserData(scan, cor->mTimeStamp, laser->mID);
                                    }
                                    Sick::Callback* sickCallback = dynamic_cast<Sick::Callback*>(*callback);
                                    if(sickCallback)
                                    {
                                        sickCallback->ProcessMessage(finalMessage);
                                    }
                                }
                                laser->mCallbacksMutex.Unlock();
                                // Add message to queue.
                                laser->mMessageQueueMutex.Lock();
                                laser->mMessageQueue.push_back(finalMessage);
                                finalMessage = NULL;
                                // If we reach the maximum queue size desired, start
                                // deleting the oldest data first.
                                if(laser->mMessageQueue.size() > laser->mMaxQueueSize)
                                {
                                    delete laser->mMessageQueue.front();
                                    laser->mMessageQueue.pop_front();
                                }
                                laser->mMessageQueueMutex.Unlock();
                            }

                            if(finalMessage != NULL)
                            {
                                delete finalMessage;
                            }
                            searchEndPosition = i;
                        }
                        else
                        {
                            i++;
                        }
                    }
                    else
                    {
                        i++;
                    }
                }

                if(searchEndPosition != 0)
                {
                    receivedData.Delete(searchEndPosition, 0);
                }
            }
            // If we have received XXX bytes of data, but
            // not been able to read a valid message from
            // that stream, delete the data.
            if( receivedData.Length() >= 10000)
            {
                memset((void *)receivedData.Ptr(), 0, receivedData.Length());
                receivedData.Clear();
            }
        }
        SleepMs(1);
    }

    //std::cout << "SICK:: Exiting Receive Thread\n";
}


/*  End of File */
