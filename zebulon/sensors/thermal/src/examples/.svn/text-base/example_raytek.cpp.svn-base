#include <iostream>
#include <stdio.h>
#include <cxutils/serial.h>
#include "raytek/raytek.h"

CxUtils::Serial mSerial;
using namespace Zebulon;

bool CheckError(char* msg, int size)
{
    char test = msg[1];

    for(int i = 2; i < 11 && i < size; i++)
    {
        if(msg[i] != test)
            return true; //mismatch error
    }

    return false; //no error
}

int main(int argc, char * argv[])
{
//    char msg[20];
//    char key = 0;
//    //char smsg[4];
//    //double i;
//    std::cout << "Connecting\n" << std::flush;
//    if(mSerial.Connect("/dev/ttyUSB0",9600,8,0,1))
//    {
//        std::cout << "Connected\n" << std::flush;

//        while(1)
//        {
//            //sprintf(smsg,"?T\r");
//            //mSerial.Send(smsg,4);
//            if(mSerial.Recv(msg,20))
//            {
//                std::cout << std::hex << msg << "\n" << std::flush;

//                if(!CheckError(msg, 20))
//                {
//                    key = msg[6];
//                    //std::cout << "0x" <<  <<   << "\n\n" << std::flush;
//                    std::cout << "0x" << ((key >> 4) & 0x0F) << (key & 0x0F) << "\n\n" << std::flush;
//                }
//            }
//            //std::cout << std::endl;
//            //sscanf(msg,"",&key);
//            //std::cout << key << std::endl << std::flush;
//            for(int i=0;i<21;i++)
//                msg[i]=0;
//            CxUtils::SleepMs(500);


//        }
//        //mSerial.Disconnect();
//    }
    Raytek::Interface raytek;

    /*Initialize communitcation with course A (Left) Server*/
    raytek.InitializeCom(5555,"10.11.12.208");
    if (raytek.SendPing())
    {
        std::cout << "PONG Course A" << std::endl;
    }


    /*Initialize communitcation with course B (right) Server*/
    raytek.InitializeCom(5556,"10.11.12.208");
    if (raytek.SendPing())
    {
        std::cout << "PONG Course B" << std::endl;
    }

    CxUtils::Wgs position;
    position.mLatitude = 36.802613;
    position.mLongitude = -76.191458;
    if(raytek.SendLocation(raytek.SPADE, position))
    {
        std::cout << "Success" << std::endl;
    }


    if(raytek.Initialize("/dev/asv-thermal",9600,false))
    {
        std::cout << " Connection Successful " << std::endl;

    }
    else
    {
        std::cout << "Unable to connect"<< std::endl;
        return 0;
    }


    while(1)
    {
        std::cout << raytek.GetTemp() << std::endl << std::flush;
        CxUtils::SleepMs(100);
    }
}
