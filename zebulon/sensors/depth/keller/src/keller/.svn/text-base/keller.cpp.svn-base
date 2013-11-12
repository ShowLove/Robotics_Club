/////////////////////////////////////////////////////////////////////////////////////
///
/// \file keller.cpp
/// \brief Keller America Depth Sensor Interface.
///
/// Author(s): Gary Stein<br>
/// Created: 4/16/2011<br>
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
#include "keller/keller.h"
#include <cxutils/math/cxmath.h>
#include <cxutils/cxutils.h>

using namespace std;
using namespace Zebulon;
using namespace Depth;


const unsigned char Callback::DefaultCallback=0xFF;


//////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Default constructor.
///
//////////////////////////////////////////////////////////////////////////////////////
Keller::Keller()
{
    mDepth = 0;
    mValidDataFlag = false;
    //Later configurable
    mAddress = 250;
    ReqPerSec=0;
    RecPerSec=0;
    mDepthOffset=0;
    mDepthOffsetCount=0;
    mFailCount=0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Destructor.  Disconnects from compass.
///
////////////////////////////////////////////////////////////////////////////////////
Keller::~Keller()
{
    Shutdown();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the interface.
///
////////////////////////////////////////////////////////////////////////////////////
void Keller::Shutdown()
{
    mSerialThread.StopThread();
    mRequestThread.StopThread();
    mSerial.Disconnect();
    mValidDataFlag = false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the connection to the sensor, and verifies there
///  is incomming data.  By default, it requests continuous sending of Data
///
///  \param port Port to connect on.
///  \param ibaud Initial baud rate to try connect at.  If this fails, other
///         supporting BAUD is attempted.
///  \param timeout How long to wait for incomming data before time out.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Keller::Initialize(const std::string& port, const int Rate,
                               const unsigned int ibaud,
                               const bool tryOtherBauds,
                               const unsigned int timeout)
{
    Shutdown();
    int result = 0;
    unsigned int baud = 9600;
    CxUtils::Time::Stamp start = 0;
    
    mPeriod=(1000.0)/Rate;
    
    // Start the serial thread which will
    // be used to receive all incomming messages.
        
    if(mSerialThread.CreateThread(&SerialThreadFunction, this) && mRequestThread.CreateThread(&SerialRequestFunction, this))
    {
#ifdef _DEBUG
        std::cout << "Establishing connection to " << mName << " device on " << port << "...\n";
        std::cout << "Trying baud " << ibaud << "...";
#endif
        mSerial.EnableRTS(false);
        mSerial.EnableDTR(false);
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
                //SetContinuousMode();

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
                        mSerial.EnableRTS(false);
                        mSerial.EnableDTR(false);
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
                                //SetContinuousMode();
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
///   \brief Static method called by runtime thread.
///
////////////////////////////////////////////////////////////////////////////////////
void Keller::SerialThreadFunction(void *args)
{
    //Pass in as a void *, then cast, then virtuals should work
    Keller* depth = (Keller*)args;
    //call the appropriate real processing function per compass
    depth->ProcessingFunction();
}

void Keller::SerialRequestFunction(void *args)
{
    //Pass in as a void *, then cast, then virtuals should work
    Keller* depth = (Keller*)args;
    //call the appropriate real processing function per compass
    depth->RequestingFunction();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all registered callbacks.
///
////////////////////////////////////////////////////////////////////////////////////
void Keller::ClearCallbacks()
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
bool Keller::RegisterCallback(Callback* cb,const unsigned char type)
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
bool Keller::RunDefaultCallback()
{
    mMutex.Lock();
    std::map<unsigned char,std::set<Callback*> >::iterator cb;
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
            (*cb2)->ProcessDepthData(mDepth);
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
bool Keller::RemoveCallback(Callback* cb,const unsigned char type)
{
    std::map<unsigned char,std::set<Depth::Callback*> >::iterator cb1;
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

//real function that does work, called by static
void Keller::ProcessingFunction()
{
    CxUtils::Packet buffer, packet;
    //assume it works for now
    mValidDataFlag=true;
    unsigned int cpos = 0;
    unsigned char* ptr = NULL;
    int endPos=-1;
    int messageCount=0;
    double startTime=CxUtils::GetTimeSeconds();
    
    buffer.Reserve(512);
    
    while(!mSerialThread.QuitThreadFlag())
    {
        if(mSerial.IsConnected())
        {
            if(mSerial.Recv(packet, 4) > 0)
            {
                // Add data to the end of our buffer
                buffer += packet;
                ptr = (unsigned char *)buffer.Ptr();
                //search packet for data
                //printf("%d\n",buffer.Length());
                //Scan looking for data
                for(cpos = 0; cpos < buffer.Length();)
                {

                    endPos=-1;
                    //printf("%d\n",*(ptr+cpos));
                    //The message is from the right chip
                    if(ptr[cpos]==mAddress)
                    {
                        switch(ptr[cpos+1])
                        {
                            //firmware
                            case 48:
                                //two cases, echo and real
                                //echo
                                if((int)buffer.Length()-cpos>=4+10)
                                {
                                    if(CheckCRC(ptr+cpos,4))
                                    {
                                        //printf("echo\n");
                                        //fflush(stdout);

                                        //look for real message
                                        if(CheckCRC(ptr+cpos+4,10))
                                        {
                                            //printf("Found Message\n");
                                            messageCount++;
                                            RecPerSec=messageCount/(CxUtils::GetTimeSeconds()-startTime);
                                            /*if(messageCount%10==0)
                                            {
                                                printf("Receive Per Second: %lf\n",messageCount/(CxUtils::GetTimeSeconds()-startTime));
                                            }*/
                                            endPos=cpos+4+10;
                                        }
                                        else
                                        {
                                            //printf("Crc Failed\n");
                                        }
                                    }
                                }
                                break;

                            case 73:
                                //two cases, echo and real
                                //echo
                                if((int)buffer.Length()-cpos>=5+9)
                                {
                                    if(CheckCRC(ptr+cpos,5))
                                    {
                                        //printf("echo\n");
                                        //fflush(stdout);
                                        //printf("%d\n",*(ptr+cpos+5));
                                        //look for real message
                                        if(CheckCRC(ptr+cpos+5,9))
                                        {
                                            //printf("Found Message\n");
                                            //fflush(stdout);
                                            messageCount++;
                                            RecPerSec=messageCount/(CxUtils::GetTimeSeconds()-startTime);
                                            /*if(messageCount%10==0)
                                            {
                                                printf("Receive Per Second: %lf\n",messageCount/(CxUtils::GetTimeSeconds()-startTime));
                                            }*/
                                            float temp;
                                            CxUtils::Packet::Read(ptr+cpos, 5+9, 5+2, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
                                            /*for(int j=0;j<9;j++)
                                            {
                                                printf("%d ",ptr[cpos+5+j]);
                                            }
                                            printf("\n");*/
                                            
                                            //ATM per Meter
                                            //double rawDepth=(temp-1.0)/(0.09920)*39.37;
                                            double rawDepth=(temp-1.007366)/0.09920;//*39.37;
                                            //printf("%f\n",temp);

                                            /*if(mDepthOffsetCount<50)
                                            {
                                                mDepthOffset=(mDepthOffset*mDepthOffsetCount+rawDepth)/(mDepthOffsetCount+1);
                                                mDepthOffsetCount++;
                                                mDepth=0;
                                            }
                                            else
                                            {
                                                mDepth=rawDepth-mDepthOffset;
                                            }*/
                                            mMutex.Lock();
                                            mDepth=rawDepth;
                                            mMutex.Unlock();
                                            RunDefaultCallback();
                                            
                                            //mDepth=*(float *)(ptr+cpos+5+2);
                                            //printf("Depth: %lf\n",mDepth);
                                            endPos=cpos+5+9;
                                        }
                                        else
                                        {
                                            //printf("Crc Failed\n");
                                        }
                                    }
                                }
                                break;
                            
                        }
                    }
                    if(endPos > 0)
                    {
                        // Delete all data up to the end of the last message read.
                        //printf("Buffer: %d %d\n",buffer.Length(),endPos);
                        buffer.Delete((unsigned int)endPos, 0);
                        //printf("Buffer: %d\n",buffer.Length());
                        cpos=0;
                    }
                    else
                    {
                        cpos++;
                    }
                }
                //printf("Length: %d %d %d\n",cpos,endPos,buffer.Length());
                
                if(buffer.Length()>100)
                {
                    buffer.Clear();
                }

            }
        }
        CxUtils::SleepMs(1);
    }
}


bool Keller::CheckCRC(unsigned char *buffer,int length)
{
    int RemoteCrc=buffer[length-2]<<8+buffer[length-1];
    unsigned char CRC_H,CRC_L;
    CalcCRC16(buffer,length-2,&CRC_H,&CRC_L);
    int LocalCrc=CRC_H<<8+CRC_L;

    if(RemoteCrc==LocalCrc)
    {
        return true;
    }
    else
    {
        /*for(int i=0;i<length;i++)
        {
            printf("%d ",(int)buffer[i]);
        }
        printf("\n");
        printf("%d %d vs %d %d\n",buffer[length-2],buffer[length-1],CRC_H,CRC_L);
        //printf("%d vs %d\n",RemoteCrc,LocalCrc);*/
        mFailCount++;
        if(mFailCount%1000==0)
        {
            printf("---------------------------------------------------Keller: CheckSum Failed: %d\n",mFailCount);
        }

        return false;
    }
    
}

void Keller::RequestingFunction()
{
    //mkae function to make packets
    unsigned char out[1024];
    double startTime=CxUtils::GetTimeSeconds();
    int messageCount=0;
    int init=0;
    
    //assume it works for now
    while(!mRequestThread.QuitThreadFlag())
    {
        if(mSerial.IsConnected())
        {
            if(init<100)
            {
                out[0]=mAddress;
                out[1]=48;
                CalcCRC16(out,2,&out[2],&out[3]);
                mSerial.Send((char *)out,4);
                init++;
            }
            else
            {
                out[0]=mAddress;
                out[1]=73;
                out[2]=1;
                CalcCRC16(out,3,&out[3],&out[4]);
                mSerial.Send((char *)out,5);
            }
            
        }
        
        messageCount++;
        ReqPerSec=messageCount/(CxUtils::GetTimeSeconds()-startTime);
        /*if(messageCount%10==0)
        {
            printf("Request Per Second: %lf\n",messageCount/(CxUtils::GetTimeSeconds()-startTime));
        }*/
        
        CxUtils::SleepMs(mPeriod);
    }
}

//////////////////////////////////////////////////////////////////////////
// CRC-16 calculation in C
//
// Calculation of CRC-16 checksum over an amount of bytes in the serial buffer.
// The calculation is done without the 2byte from crc16 (receive-mode).
// SC_Buffer[]: Byte-Buffer for the serial interface. Type: unsigned char (8bit)
// SC_Amount : Amount of Bytes which should be transmitted or are received (without CRC16)
//
//////////////////////////////////////////////////////////////////////////
void Keller::CalcCRC16(unsigned char *SC_Buffer,int SC_Amount,unsigned char* CRC_H, unsigned char* CRC_L)
{
// locals
    unsigned int Crc;
    unsigned char n, m, x;
// initialisation
    Crc= 0xFFFF;
    m= SC_Amount;
    x= 0;
// loop over all bits
    while(m>0)
    {
        Crc^= SC_Buffer[x];
        for(n=0; n<8; n++)
        {
            if(Crc&1)
            {
                Crc>>= 1;
                Crc^= 0xA001;
            }
            else
                Crc>>= 1;
        }
        m--;
        x++;
    }
// result
    *CRC_H= (Crc>>8)&0xFF;
    *CRC_L= Crc&0xFF;
}// end CalcCRC16



/*  End of File */
