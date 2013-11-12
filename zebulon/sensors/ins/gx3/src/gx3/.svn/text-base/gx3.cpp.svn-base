/////////////////////////////////////////////////////////////////////////////////////
///
/// \file gx3.cpp
/// \brief A bastardized version of a compass/gps class
/// using a serial connection
///
/// Author(s): Gary Stein<br>
/// Created: 10/2/2011<br>
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
#include "gx3/gx3.h"
#include "gx3/factory.h"
#include <cxutils/math/cxmath.h>
#include <cxutils/cxutils.h>

using namespace std;
using namespace Zebulon;
using namespace INS;
using namespace MicroStrain;

//////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Default constructor.
///
//////////////////////////////////////////////////////////////////////////////////////
GX3::GX3()
{
    mValidDataFlag = false;
    Factory::InitializeMessages();
    mINSInit=false;
    mINSFirstData = true;
    mYawOffset = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Destructor.  Disconnects from compass.
///
////////////////////////////////////////////////////////////////////////////////////
GX3::~GX3()
{
    Shutdown();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the interface.
///
////////////////////////////////////////////////////////////////////////////////////
void GX3::Shutdown()
{
    mSerialThread.StopThread();
    mSerial.Disconnect();
    mValidDataFlag = false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the connection to the sensor, and verifies there
///  is incomming data.  By default, it requests continuous sending of
///  Gyro-Stablised Euler Angles.
///
///  \param port Port to connect on.
///  \param ibaud Initial baud rate to try connect at.  If this fails, other
///         supporting BAUD is attempted.
///  \param timeout How long to wait for incomming data before time out.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int GX3::Initialize(const std::string& port,
                               const unsigned int ibaud,
                               const bool tryOtherBauds,
                               const unsigned int timeout)
{
    mPort=port;
    mBaud=ibaud;
    Shutdown();
    int result = 0;
    unsigned int baud = 9600;
    CxUtils::Time::Stamp start = 0;

    // Start the serial thread which will
    // be used to receive all incomming messages.
    if(mSerialThread.CreateThread(&SerialThreadFunction, this))
    {
#ifdef _DEBUG
        std::cout << "Establishing connection to " << mName << " device on " << port << "...\n";
        std::cout << "Trying baud " << ibaud << "...";
#endif

        //std::cout << (long int)&mSerial << std::endl;
        //  Now connect at the initial baud rate
        if(mSerial.Connect(port.c_str(), ibaud))
        {
            // Wait and see if we received any messages.
            start = CxUtils::GetTimeMs();
            while(CxUtils::GetTimeMs() - start < timeout && result == 0)
            {
                if(mValidDataFlag)
                {
#ifdef _DEBUG
                    std::cout << "Success!\n";
#endif
                    result = 1;
                }
                SetContinuousMode();

                CxUtils::SleepMs(100);
            }

            if( result == 0 )
            {
                //mSerial.Flush();
                mSerial.Shutdown();

#ifdef _DEBUG
                std::cout << "Failure.\n";
#endif
                while(tryOtherBauds && baud <= 115200 && result == 0)
                {
                    // Don't retry the default baud
                    //if(baud != ibaud)
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
                                if(mValidDataFlag)
                                {
#ifdef _DEBUG
                                    std::cout << "Success!\n";
#endif
                                    result = 1;
                                }
                                SetContinuousMode();
                                CxUtils::SleepMs(100);
                            }
                        }
                        else
                        {
#ifdef _DEBUG
                            std::cout << "Could not even open the port\n";
#endif
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


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to set yaw declination offset.
///
///   \param declination Declination offset in radians.
///
////////////////////////////////////////////////////////////////////////////////////
void GX3::SetDeclination(const double declination)
{
    mYawOffset=  declination;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates declation offset using offset in degrees and minutes.
///
///   \param declinationDeg Declination offset degrees.
///   \param declinationMin Declination offset minutes.
///
////////////////////////////////////////////////////////////////////////////////////
void GX3::SetDeclination(const double declinationDeg,
                         const double declinationMin)
{
    double declination = CX_DEG2RAD(declinationDeg + declinationMin/60.0);
    SetDeclination(declination);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Declination offset for heading.
///
////////////////////////////////////////////////////////////////////////////////////
double GX3::GetDeclination() const
{
    return mYawOffset;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Static method called by runtime thread.
///
////////////////////////////////////////////////////////////////////////////////////
void GX3::SerialThreadFunction(void *args)
{
    //Pass in as a void *, then cast, then virtuals should work
    GX3* gx3 = (GX3*)args;
    //call the appropriate real processing function per compass
    gx3->ProcessingFunction();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all registered callbacks.
///
////////////////////////////////////////////////////////////////////////////////////
void GX3::ClearCallbacks()
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
bool GX3::RegisterCallback(Callback* cb,const unsigned char type)
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
bool GX3::RunCallbacks()
{
    mMutex.Lock();
    std::map<unsigned char,std::set<Callback*> >::iterator cb;
    //in default case it should be the default context
    //assume data already loaded in mValues

    for(cb = mCallbacks.begin();cb!=mCallbacks.end();cb++)
    {
        std::set<Callback*>::iterator cb2;
        for(cb2 = cb->second.begin();
            cb2 != cb->second.end();
            cb2++)
        {
            switch(cb->first)
            {
                case Callback::CompassCallback:
                    if(mNewIMUData)
                    {
                        (*cb2)->ProcessCompassData(mAttitude.mZ, mAttitude.mY, mAttitude.mX);
                    }
                    break;
                //check order of these
                case Callback::GPSCallback:
                    if(mNewGPSData)
                    {
                        (*cb2)->ProcessGPSData(mLLHPositionGPS.mX, mLLHPositionGPS.mY, mLLHPositionGPS.mZ);
                    }
                    break;
                case Callback::IMUCallback:
                    if(mNewIMUData)
                    {
                        (*cb2)->ProcessIMUData(mAttitude,mGyroscope,mAcceleration);
                    }
                    break;
                case Callback::INSCallback:
                    if(mNewINSData)
                    {
                        (*cb2)->ProcessINSData(mLLHPosition,mNEDVelocity,mOrientation);
                    }
                    break;
                case Callback::VELCallback:
                    if(mNewIMUData)
                    {
                        (*cb2)->ProcessVelocityData(mVelocity);
                    }
                    break;
                //default calls all
                case Callback::DefaultCallback:
                    if(mNewIMUData)
                    {
                        (*cb2)->ProcessCompassData(mAttitude.mZ, mAttitude.mY, mAttitude.mX);
                        (*cb2)->ProcessIMUData(mAttitude, mGyroscope,mAcceleration);
                        (*cb2)->ProcessVelocityData(mVelocity);
                    }
                    if(mNewGPSData)
                    {
                        (*cb2)->ProcessGPSData(mLLHPositionGPS.mX, mLLHPositionGPS.mY, mLLHPositionGPS.mZ);
                    }
                    if(mNewINSData)
                    {
                        (*cb2)->ProcessINSData(mLLHPosition,mNEDVelocity,mOrientation);
                    }
                    break;
            }

        }
    }
    //reset that data was handled (double buffer or thread?)
    if(mNewIMUData)
    {
        mNewIMUData=false;
    }
    if(mNewGPSData)
    {
        mNewGPSData=false;
    }
    if(mNewINSData)
    {
        mNewINSData=false;
    }
    //check if any new data
    if(mNewIMUData ||mNewGPSData || mNewINSData)
    {
        mINSFirstData=false;
        mLastDataTime=CxUtils::GetTimeSecondsSince1Jan1970();
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
bool GX3::RemoveCallback(Callback* cb,const unsigned char type)
{
    std::map<unsigned char,std::set<Callback*> >::iterator cb1;
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

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Does the processing and setup of data from serial
///
////////////////////////////////////////////////////////////////////////////////////
void GX3::ProcessingFunction()
{
    unsigned int count = 0;
    Message* newMessage = NULL;
    unsigned int cpos = 0;
    unsigned char* ptr = NULL;
    unsigned int readCount = 0;
    unsigned int readFailure = 0;
    unsigned int tickFailure = 0;
    int endPos = -1;
    unsigned int continueCount = 0;
    unsigned int prevTimerTicks = 0;
    CxUtils::Packet buffer, packet;
    std::vector<unsigned short> tickTimers;
    buffer.Reserve(512);
    CxUtils::Time::Stamp updateTimeMs = 0;

    int fakeCount=0;
    int commandOut=0;
    int dataIn=0;

    mNewIMUData=mNewGPSData=mNewINSData=false;

    //hack take out
    while(!mSerialThread.QuitThreadFlag())
    {
        double DataGapTime=CxUtils::GetTimeSecondsSince1Jan1970()-mLastDataTime;
        //if no data for a bit
        if(!mINSFirstData && DataGapTime>0.5)
        {
            printf("Fail Data Missing: %lf\n",DataGapTime);
            /*mSerial.Disconnect();
            CxUtils::SleepMs(100);
            mSerial.Connect(mPort.c_str(), mBaud);
            SetContinuousMode();
            CxUtils::SleepMs(100);*/
        }

        if(mSerial.IsConnected())
        {
            mValidDataFlag=true;
            if(fakeCount==1000)
            {
                commandOut++;
                /*printf("Command Ping: %d\n",commandOut);
                fflush(stdout);
                CxUtils::Packet command;
                command.Clear();
                command.WriteByte(0x75);
                command.WriteByte(0x65);
                command.WriteByte(0x01);
                command.WriteByte(0x02);
                command.WriteByte(0x02);
                command.WriteByte(0x01);
                command.WriteByte(0xE0);
                command.WriteByte(0xC6);
                mSerial.Send(command);*/
                /*printf("Command Poll: %d\n",commandOut);
                fflush(stdout);
                CxUtils::Packet command;
                command.Clear();
                command.WriteByte(0x75);
                command.WriteByte(0x65);
                command.WriteByte(0x0C);
                command.WriteByte(0x04);
                command.WriteByte(0x04);
                command.WriteByte(0x01);
                command.WriteByte(0x00);
                command.WriteByte(0x00);
                command.WriteByte(0xEF);
                command.WriteByte(0xDA);
                mSerial.Send(command);*/
                //SetContinuousMode();

                fakeCount=0;
            }
            fakeCount++;


            packet.Clear();
            // Receive data from the serial port.
            int bytesAvail = mSerial.ReadBytesAvailable();

            if(bytesAvail > 5 &&
                mSerial.Recv(packet, mSerial.ReadBytesAvailable()) > 0)
            //if (mSerial.Recv(packet, mSerial.ReadBytesAvailable()) > 5)
            {
                int newData=0;
                readFailure = 0;
                // Add data to the end of our buffer
                buffer += packet;
                // Now search the buffer for any messages.
                ptr = (unsigned char *)buffer.Ptr();

                endPos = -1;
                for(cpos = 0; cpos < buffer.Length();)
                {
                    CxUtils::Time::Stamp time;

                    int leftOver=buffer.Length()-cpos;
                    //will never find anything
                    if(leftOver<6)
                    {
                        break;
                    }

                    //need a cpos++

                    // See if we recognize a command byte, if not skip
                    if(!(ptr[cpos]==Message::GX3Sync1 && ptr[cpos+1]==Message::GX3Sync2))
                    {
                        cpos++;
                        continue;
                    }

                    //Start is now found, look at rest of message (not data)
                    unsigned char descSetSelector=ptr[cpos+2];
                    int payloadLength=(int)ptr[cpos+3];

                    int packetLength=4+payloadLength+2;
                    //start, payload, and checksum
                    //make sure it is enough data (else skip)
                    if(packetLength>leftOver)
                    {
                        break;
                    }
                    //grab msb and lsb (ignoring endianess)
                    //unsigned short checkSum=((unsigned short)ptr[cpos+3+payloadLength])<<8 || (unsigned short)ptr[cpos+3+payloadLength+1];
                    unsigned short checkSum=256*ptr[cpos+4+payloadLength]+ptr[cpos+4+payloadLength+1];
                    //checksum plus preamble
                    unsigned short calcCheckSum=Message::CalcCheckSum(ptr+cpos,packetLength-2);
                    //printf("%04X %04X %02X %02X\n",checkSum,calcCheckSum,ptr[cpos+4+payloadLength],ptr[cpos+4+payloadLength+1]);

                    /*printf("%d: ",packetLength);
                    for(int i=0;i<packetLength;i++)
                    {
                        printf("%02X ",ptr[cpos+i]);
                    }
                    printf("\n");*/

                    //if good, process, if bad skip
                    if(checkSum!=calcCheckSum)
                    {
                        cpos++;
                        continue;
                    }
                    dataIn++;
                    //printf("Receive: %d\n",dataIn);

                    //Now break it into fields
                    int fieldPos=0;
                    //should be start of first field
                    int fpos=cpos+4;
                    while(fieldPos<payloadLength)
                    {
                        int fieldLength=(int)ptr[fpos];
                        /*printf("%d: ",fieldLength);
                        for(int i=0;i<fieldLength;i++)
                        {
                            printf("%02X ",ptr[fpos+i]);
                        }
                        printf("\n");*/


                        unsigned char fieldDesc=ptr[fpos+1];
                        //decode the fields
                        unsigned short messageType=256*descSetSelector + fieldDesc;
                        //printf("Type: %04X %02X %02X\n",messageType,descSetSelector,fieldDesc);

                        //get type from factory
                        newMessage=Factory::GetMessage(messageType);


                        //load up the packet with data
                        if(newMessage)
                        {
                            updateTimeMs = CxUtils::GetTimeMs();

                            newMessage->SetMessageTimeStamp(time);
                            //process the message, check for errors
                            if((readCount = newMessage->Read(&ptr[fpos], fieldLength)) > 0)
                            {
                                mValidDataFlag = true;
                            }
                            else
                            {
                                delete newMessage;
                                newMessage = NULL;
                            }
                        }

                        if(newMessage)
                        {
                            newData++;
                            mValidDataFlag = true;
                            //newMessage->Print();

                            switch(newMessage->GetType())
                            {
                                //IMU
                                case Message::EulerAngles:
                                {
                                    EulerAngles *tmp=(EulerAngles *)newMessage;
                                    mMutex.Lock();
                                    mAttitude.mX=tmp->mRoll;
                                    mAttitude.mY=tmp->mPitch;
                                    mAttitude.mZ=tmp->mYaw;
                                    //printf("%lf %lf %lf\n",mAttitude.mX,mAttitude.mY,mAttitude.mZ);
                                    mNewIMUData=true;
                                    // Fix attitude values with declination angle offset
                                    mAttitude.mZ += mYawOffset;
                                    if(mAttitude.mZ <= -CxUtils::CX_PI)
                                    {
                                        mAttitude.mZ += 2.0*CxUtils::CX_PI;
                                    }
                                    if(mAttitude.mZ > CxUtils::CX_PI)
                                    {
                                        mAttitude.mZ -= 2.0*CxUtils::CX_PI;
                                    }
                                    mMutex.Unlock();
                                    break;
                                }
                                case Message::ScaledGyroVector:
                                {
                                    ScaledGyroVector *tmp=(ScaledGyroVector *)newMessage;
                                    mMutex.Lock();
                                    mGyroscope.mX=tmp->mXGyro;
                                    mGyroscope.mY=tmp->mYGyro;
                                    mGyroscope.mZ=tmp->mZGyro;
                                    mNewIMUData=true;
                                    mMutex.Unlock();
                                    break;
                                }
                            case Message::ScaledAccelerometerVector:
                            {
                                ScaledAccelerometerVector *tmp=(ScaledAccelerometerVector*)newMessage;
                                mMutex.Lock();
                                mAcceleration.mX=9.8*tmp->mXAccel;
                                mAcceleration.mY=9.8*tmp->mYAccel;
                                mAcceleration.mZ=9.8*tmp->mZAccel;
                                mNewIMUData=true;
                                mMutex.Unlock();
                                break;
                            }
                            case Message::DeltaVelocityVector:
                            {
                                DeltaVelocityVector *tmp=(DeltaVelocityVector*)newMessage;
                                mMutex.Lock();
                                //I think this is wrong in the GX3 documentation
                                mVelocity.mX=9.8*tmp->mXVelocity;
                                mVelocity.mY=9.8*tmp->mYVelocity;
                                mVelocity.mZ=9.8*tmp->mZVelocity;

                                mNewIMUData=true;
                                mMutex.Unlock();
                                break;
                            }
                                //GPS
                                case Message::LLHPosition:
                                {
                                    LLHPosition *tmp=(LLHPosition *)newMessage;
                                    //tmp->Print();
                                    if(tmp->mValidFlags==0x1F)
                                    {
                                        mMutex.Lock();
                                        mLLHPositionGPS.mX=tmp->mLatitude;
                                        mLLHPositionGPS.mY=tmp->mLongitude;
                                        mLLHPositionGPS.mZ=tmp->mHeightEllipsoid;
                                        mNewGPSData=true;
                                        mMutex.Unlock();
                                    }
                                    break;
                                }
                                case Message::NEDVelocity:
                                {
                                    NEDVelocity *tmp=(NEDVelocity *)newMessage;
                                    //tmp->Print();
                                    if(tmp->mValidFlags==0x3F)
                                    {
                                        mMutex.Lock();
                                        mNEDVelocityGPS.mX=tmp->mNorth;
                                        mNEDVelocityGPS.mY=tmp->mEast;
                                        mNEDVelocityGPS.mZ=tmp->mDown;
                                        mNewGPSData=true;
                                        mMutex.Unlock();
                                    }
                                    break;
                                }
                                //INS
                                case Message::EstimatedAngularRate:
                                {
                                    EstimatedAngularRate *tmp=(EstimatedAngularRate *)newMessage;
                                    if(tmp->mValidFlags==1)
                                    {

                                        mMutex.Lock();
                                        //Use Instead?
                                        mGyroscope.mX=tmp->mX;
                                        mGyroscope.mY=tmp->mY;
                                        mGyroscope.mZ=tmp->mZ;
                                        mNewINSData=true;
                                        mMutex.Unlock();


                                    }
                                    break;
                                }
                                case Message::EstimatedLinearAcceleration:
                                {
                                    EstimatedLinearAcceleration *tmp=(EstimatedLinearAcceleration *)newMessage;
                                    if(tmp->mValidFlags==1)
                                    {

                                        mMutex.Lock();
                                        //Use Instead?
                                        /*mAcceleration.mX=tmp->mX;
                                        mAcceleration.mY=tmp->mY;
                                        mAcceleration.mZ=tmp->mZ;*/
                                        mNewINSData=true;
                                        mMutex.Unlock();

                                    }
                                    break;
                                }
                                case Message::EstimatedLLHPosition:
                                {
                                    EstimatedLLHPosition *tmp=(EstimatedLLHPosition *)newMessage;
                                    //tmp->Print();
                                    if(tmp->mValidFlags==1)
                                    {
                                        mMutex.Lock();
                                        mLLHPosition.mX=tmp->mLatitude;
                                        mLLHPosition.mY=tmp->mLongitude;
                                        mLLHPosition.mZ=tmp->mHeight;
                                        mNewINSData=true;
                                        mMutex.Unlock();
                                    }
                                    break;
                                }
                                case Message::EstimatedNEDVelocity:
                                {
                                    EstimatedNEDVelocity *tmp=(EstimatedNEDVelocity *)newMessage;
                                    if(tmp->mValidFlags==1)
                                    {
                                        mMutex.Lock();
                                        mNEDVelocity.mX=tmp->mNorth;
                                        mNEDVelocity.mY=tmp->mEast;
                                        mNEDVelocity.mZ=tmp->mDown;
                                        mNewINSData=true;
                                        mMutex.Unlock();
                                    }
                                    break;
                                }
                                case Message::EstimatedOrientationEulerAngles:
                                {
                                    EstimatedOrientationEulerAngles *tmp=(EstimatedOrientationEulerAngles *)newMessage;
                                    if(tmp->mValidFlags==1)
                                    {
                                        mMutex.Lock();
                                        mOrientation.mX=tmp->mRoll;
                                        mOrientation.mY=tmp->mPitch;
                                        mOrientation.mZ=tmp->mYaw;
                                        mNewINSData=true;
                                        // Fix attitude values with declination angle offset
                                        //does it do this automatically?
                                        mOrientation.mZ += mYawOffset;
                                        if(mOrientation.mZ <= -CxUtils::CX_PI)
                                        {
                                            mOrientation.mZ += 2.0*CxUtils::CX_PI;
                                        }
                                        if(mOrientation.mZ > CxUtils::CX_PI)
                                        {
                                            mOrientation.mZ -= 2.0*CxUtils::CX_PI;
                                        }
                                        mMutex.Unlock();
                                    }
                                    break;
                                }
                                case Message::FilterStatus:
                                {
                                    FilterStatus *tmp=(FilterStatus *)newMessage;
                                    //tmp->Print();
                                    break;
                                }
                            }
                            delete newMessage;
                            newMessage = NULL;
                        }

                        //track length and position for clarity
                        fieldPos+=fieldLength;
                        fpos+=fieldLength;
                    }

                    //Once reading is done, skip the rest of the data
                    cpos+=packetLength;

                    //if it got to here, it is the end of a packet
                    endPos=cpos-1;


                } // Looping through buffered data.

                //if there was new data, technically wrong since
                //it will update everything at the fastest rate
                if(newData>0)
                {
                    RunCallbacks();
                }
                if(endPos > 0)
                {
                    // Delete all data up to the end of the last message read.
                    buffer.Delete((unsigned int)endPos, 0);
                }
                // If buffer is full, flush it.
                if(buffer.Length() >= 5000)
                {
                    buffer.Clear();
                }
            }
            else
            {
                readFailure++;

                /*if (readFailure > 25)
                {
                    mSerial.Flush();
                }//*/

                //some code boat team added, freaks out compass port
                /*if(readFailure > 1000)
                {
                    printf("Doing something here!!!!\n");
                    char buff[] = {0x75,0x65,0x01,0x02,0x02,0x7E,0x5D,0x43};
                    mSerial.Send(buff,8);
                    prevTimerTicks = 0;
                    mValidDataFlag = false;
                    CxUtils::SleepMs(100);
                }*/

                //should probably just ask for it again
                /*if(readFailure > 1000)
                {
                    SetContinuousMode();
                }*/
                /*static int readFailureMax=0;
                if(readFailureMax<readFailure)
                {
                    printf("Fail Count: %d\n",readFailure);
                    readFailureMax=readFailure;
                }
                printf("Fail Count: %d %d\n",readFailure,readFailureMax);*/
                if(readFailure > 1000)
                {
                    printf("Fail No Data read for a bit\n");
                }
            }
        }
        else
        {
            // Not connected on serial
            // port or anything.
            mSerial.Reconnect();
            mValidDataFlag = false;
        }
        #ifdef WIN32
        if(++count == 100)
        {
            CxUtils::SleepMs(1);
            count = 0;
        }
        #else
        usleep(100);
        #endif
    }

    if(newMessage)
    {
        delete newMessage;
        newMessage = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends the Set Continuous Mode command for a type of message.
///
///  \return True on success, otherwise failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool GX3::SetContinuousMode()
{
    CxUtils::Packet cmd;
    std::vector<unsigned short> commands;

    commands.clear();
    cmd.Clear();
    commands.push_back(Message::EulerAngles);
    commands.push_back(Message::ScaledAccelerometerVector);
    commands.push_back(Message::ScaledGyroVector);
    commands.push_back(Message::DeltaVelocityVector);
    cmd=Message::ConstructCommand(commands,100);
    mSerial.Send(cmd);

    commands.clear();
    cmd.Clear();
    commands.push_back(Message::LLHPosition);
    commands.push_back(Message::NEDVelocity);
    cmd=Message::ConstructCommand(commands,4);
    mSerial.Send(cmd);

    commands.clear();
    cmd.Clear();
    commands.push_back(Message::EstimatedAngularRate);
    commands.push_back(Message::EstimatedLinearAcceleration);
    commands.push_back(Message::EstimatedLLHPosition);
    commands.push_back(Message::EstimatedNEDVelocity);
    commands.push_back(Message::EstimatedOrientationEulerAngles);
    commands.push_back(Message::FilterStatus);
    cmd=Message::ConstructCommand(commands,100);
    mSerial.Send(cmd);

    InitializeINS();

#if 0
    if(cmd.Length()>0)
    {
        unsigned char *ptr=cmd.Ptr();
        printf("Cmd: ");
        for(int i=0;i<cmd.Length();i++)
        {
            printf("%02X ",ptr[i]);
        }
        printf("\n");
        mSerial.Send(cmd);
    }
 #endif
#if 0
    Message::Type type = Message::Null;
    if(type != Message::EnableDisableDeviceContinuousDataStream && type != Message::Null)
    {
        CxUtils::Packet command;
        Message *response = NULL;
        //command.SetByteOrder(CxUtils::CX_PACKET_BIG_ENDIAN);
        command.WriteByte(Message::GX3Sync1);
        command.WriteByte(Message::GX3Sync2);
        unsigned char c=(Message::EnableDisableDeviceContinuousDataStream&0xFF00)>>8;
        unsigned char f=Message::EnableDisableDeviceContinuousDataStream&0x00FF;
        command.WriteByte(c);
        //package length
        command.WriteByte(5);
        //field length
        command.WriteByte(5);
        command.WriteByte(f);
        command.WriteByte(1);
        command.WriteByte(1);
        command.WriteByte(1);

        /*if(TransmitMessage(command, &response))
        {
            mContinuousModeType = type;
            delete response;
            return true;
        }*/
    }
#endif

    return false;
}

bool GX3::InitializeINS()
{
    //Magic command to initialize INS
    CxUtils::Packet command;
    //command=Message::ConstructCommand(std::string("7565 0D02 0201 ECF6"));
    //mSerial.Send(command);
    //command=Message::ConstructCommand(std::string("7565 0D0E 0E02 0000 0000 0000 0000 0000 0000 0000 0000 0583"));
    command=Message::ConstructCommand(std::string("7565 0D06 0604 0000 0000 F7E9"));
    /*unsigned char *ptr=command.Ptr();
    printf("Cmd: ");
    for(int i=0;i<command.Length();i++)
    {
        printf("%02X ",ptr[i]);
    }
    printf("\n");
    printf("%04X",Message::CalcCheckSum(command.Ptr(),command.Length()-2));*/
    mSerial.Send(command);
    //command=Message::ConstructCommand(std::string("7565 0D04 0419 0101 0A2B"));
    //mSerial.Send(command);


}

/*  End of File */

