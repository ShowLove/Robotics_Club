//////////////////////////////////////////////////////////////////////////////////////
///
/// \file underwater_example.cpp
/// \brief Example/Test Program for MCU Interface
///
/// Author(s): Cassondra Puklavage<br>
/// Created: 04/02/2010<br>
/// Copyright (c) 2009<br>
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
#include <iostream>
//#include <cxutils/math/cxmath.h>
#include <cxutils/cxutils.h>
#include "mcu/interface.h"
#include "mcu/messagefactory.h"

using namespace std;
using namespace Zebulon;
std::map<unsigned short,bool> allwork;

class MyCallback : public MCU::Interface::Callback
{
public:
    MyCallback() {}
    ~MyCallback() {}
    virtual void ProcessMessage(const MCU::Message* message)
    {
        //message->Print();
        if(((MCU::ReportMotorStatus *)message)->mPort != 0xFE)
        {
            allwork[((MCU::ReportMotorStatus *)message)->mPort]=true;
            cout << "Recieving Message: \n";
            message->Print();
        }
    }
};

int main(int argc, char * argv[])
{
    
    bool running = false;
    
    MCU::Interface mcu;
    std::vector<unsigned short> alladdresses;
    
    /*alladdresses.push_back(0x50);
    alladdresses.push_back(0x52);
    alladdresses.push_back(0x60);
    alladdresses.push_back(0x62);*/
    //alladdresses.push_back(0x70);
    //alladdresses.push_back(0x72);
    
    unsigned char address0 = 0x62;// should be 62
    
    unsigned char oldAddress = 0x52;
    unsigned char newAddress = 0x62;
    
    //#define CHANGEADDRESS
    #define MOTORSTATUS
    #define OUTPUTMOTORCOMMAND
    
    MCU::SetMotorAddress changeAddress;
    changeAddress.mPort = oldAddress;
    changeAddress.mNewAddress = newAddress;
    
    MCU::SetServoOutputs motor0;
    motor0.mChannel = address0;
    unsigned char currVal0 = 0xD0;
    motor0.mValue = currVal0;
    
    MCU::ReportMotorStatus motorStatus0;
    motorStatus0.mPort = address0;
    
    MCU::ReportMcuId mcuMessage;
    
    MCU::Interface hydro;

    unsigned char dpVGA1 = 0x11;
    unsigned char dpVGA2 = 0x21;
    unsigned char dpVGA3 = 0x31;
    unsigned char dpVGA4 = 0x41;
    unsigned char dpVGA5 = 0x51;

    unsigned char dpVREF1 = 0x12;
    unsigned char dpVREF2 = 0x22;
    unsigned char dpVREF3 = 0x32;
    unsigned char dpVREF4 = 0x42;
    unsigned char dpVREF5 = 0x52;

    unsigned char dpValue = 0x80;

    MCU::SetAnalogOutputs digitalPot;
    digitalPot.mPort = dpVGA1;

    MCU::Ping ping;
    
    MyCallback callback;
    
    cout << "Connecting to MCU...";
    if(argc > 1)
    {
        //running = mcu.Initialize(argv[1], 19200);
        running = hydro.Initialize(argv[1], 19200);
    }
    else
    {
        cout << "Default Baud\n";
        //running = mcu.Initialize("/dev/auv-mtr1-board", 57600, false, CxUtils::CX_SERIAL_NO_PARITY); //57600
        //running = hydro.Initialize("/dev/auv-mtr1-board", 57600, false, CxUtils::CX_SERIAL_NO_PARITY); //57600
        running = hydro.Initialize("/dev/ttyUSB0", 57600, false, CxUtils::CX_SERIAL_NO_PARITY); //57600
    }
    
    mcu.RegisterCallback(MCU::Message::All, &callback);
    
    if(running)
    {
        cout << "Success!\n";
        char key = 0;
        cout << "Press <escape key> To Exit!\n";
        
        //cout << "adjust motor\n";
        //motor0.mChannel = 0x52;
        ////motor0.mValue = 0x80;
        ////mcu.Send(&motor0);
        
        /*cout << "adjust motor\n";
        motor1.mChannel = 0x50;
        motor1.mValue = 0x80;
        mcu.Send(&motor1);*/
        
            /*CxUtils::SleepMs(10000);
            
            // Ping
            cout << "PING \n";
            mcu.Send(&ping);
            
            unsigned short address = 0x62;
            
            cout << "Change address\n";
            motorAddress0.mPort = 0x52;
            motorAddress0.mNewAddress = address;
            mcu.Send(&motorAddress0);*/
            
            
        while(key != 27)
        {
            cout << "PING \n";
            hydro.Send(&ping);

            //Test for AVR on Hydroboard

#if 0
            mcu.Send(&mcuMessage);
            
#ifdef MOTORSTATUS
            std::cout << "Motor Status: " << std::endl;
            mcu.Send(&motorStatus0);
#endif
            
#ifdef CHANGEADDRESS
            cout << "Re-addressing the motor from " << std::hex << (int)oldAddress << std::hex << " to " << std::hex << (int)newAddress << std::hex << endl;
            mcu.Send(&changeAddress);
#endif

#ifdef OUTPUTMOTORCOMMAND
            cout << "Command Motor: " << std::hex << (int)address0 << std::hex << std::endl;
            mcu.Send(&motor0);
#endif

#endif

//OUTPUTMOTORCOMMAND
            /*cout << hex << (int)motorStatus0.mPort << endl; 
            cout << "Read motor 0: \n";
            for(int i=0;i<20;i++)
            {
                cout << "Write motor 0: \n";
                cout << std::hex << (int)motorStatus0.mPort << endl;
                //mcu.Send(&motor0);
                mcu.Send(&motorStatus0);
                CxUtils::SleepMs(10);
            }
            motorStatus0.mPort += 0x02;
            //motorStatus0.mPort = 0x70;
            CxUtils::SleepMs(500);*/
            
            //cout << "MOTORADDRESSCHANGE" << endl;
            //mcu.Send(&changeAddress);

            /*MCU::SetServoOutputs motor0;
            //motor0.mChannel = 0x72;
            motor0.mChannel = 0x52;
            unsigned char currVal = 0xC0;
            motor0.mValue = currVal;
            sadf*/
            
#if 0
            for(int i=0;i<(int)alladdresses.size();i++)
            {
                cout << hex << "0x" << (int)alladdresses[i] << " " << dec << allwork[alladdresses[i]] << " ";
                allwork[alladdresses[i]]=false;
            }
            cout << endl;
            //cout << "PING \n";
            mcu.Send(&mcuMessage);
            for(int i=0;i<(int)alladdresses.size();i++)
            {
                motor1.mChannel=(unsigned char)alladdresses[i];
                motorStatus1.mPort=(unsigned char)alladdresses[i];
                //cout << "Write motor: " << hex << (int)motor1.mChannel << dec <<  "\n";
                mcu.Send(&motor1);
                //cout << "Read motor: " << hex << (int)motorStatus1.mPort << dec << "\n";
                mcu.Send(&motorStatus1);
                CxUtils::SleepMs(25);
            }
#endif
            
            /*for(char i = 0; i < 0x20; i++)
            {
                currVal0++;
                currVal1++;
                motor0.mValue = currVal0;
                motor1.mValue = currVal1;
                mcu.Send(&motor0);
                mcu.Send(&motor1);
                cout << "currVal : " << (int)currVal0 << endl;
                //mcu.Send(&motorStatus0);
                CxUtils::SleepMs(100);
            }

            for(char i = 0; i < 0x20; i++)
            {
                currVal0--;
                currVal1--;
                motor0.mValue = currVal0;
                motor1.mValue = currVal1;
                mcu.Send(&motor0);
                mcu.Send(&motor1);
                cout << "currVal : " << (int)currVal0 << endl;
                mcu.Send(&motorStatus0);
                CxUtils::SleepMs(100);
            }*/

            

            //mcu.Send(&motorStatus0);

            //mcu.Send(&motor0);
            //mcu.Send(&motor1);

            //CxUtils::SleepMs(100);

            //cout << "check motor status\n";
            //motorStatus0.mPort = address0;
            //mcu.Send(&motorStatus0);
            //CxUtils::SleepMs(5);
            //mcu.Send(&motorStatus1);

            /*MCU::SetServoOutputs motor0;
            //motor0.mChannel = 0x72;
            motor0.mChannel = 0x52;
            unsigned char currVal = 0x80;
            motor0.mValue = currVal;

            MCU::ReportMotorStatus motorStatus0;
            //motorStatus0.mPort = 0x72;
            motorStatus0.mPort = 0x52;

            MCU::SetMotorAddress motorAddress0;*/



            /*mcu.Send(&motorAddress0);

            mcu.Send(&motor0);
            //CxUtils::SleepMs(100);
            mcu.Send(&motorStatus0);
            cout << "Change address: \n";
            mcu.Send(&motorAddress0);*/

            /*cout << "Send new Info\n";
            motor0.mChannel = 0x70;
            motor0.mValue = 0x80;
            motorStatus0.mPort = 0x70;*/

            //motor0.mChannel = 0x52;
            //motor0.mValue = 0x80;
            //motorStatus0.mPort = 0x52;





            /// Send message to motors
            /*cout << "Motor command\n";
            mcu.Send(&motor0);

            mcu.Send(&motorStatus0);
            //CxUtils::SleepMs(500);

            mcu.Send(&motorAddress0);
            //CxUtils::SleepMs(500);

            //motor0.mChannel = 0x50;
            //motor0.mValue = 0x80;

            //mcu.Send(&motor0);

            mcu.Send(&motorStatus0);*/
            //CxUtils::SleepMs(1000);

            /*for(char i = 0; i < 0x20; i++)
            {
                currVal++;
                motor0.mValue = currVal;
                mcu.Send(&motor0);
                cout << "currVal : " << (int)currVal << endl;
                mcu.Send(&motorStatus0);
                CxUtils::SleepMs(100);
            }

            for(char i = 0; i < 0x20; i++)
            {
                currVal--;
                motor0.mValue = currVal;
                mcu.Send(&motor0);
                cout << "currVal : " << (int)currVal << endl;
                mcu.Send(&motorStatus0);
                CxUtils::SleepMs(100);
            }*/

            key = CxUtils::GetChar();
            CxUtils::SleepMs(100);
        }
    }

}



