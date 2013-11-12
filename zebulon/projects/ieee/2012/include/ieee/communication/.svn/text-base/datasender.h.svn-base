//Used to pass game data to a waiting receiver
//this is non-threaded as it happens on demand

#include<stdio.h>
#include<cxutils/cxutils.h>
#include<iostream>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<string>

#include<ieee/communication/gamepacket.h>

#ifndef DATASENDER_H
#define DATASENDER_H

class DataSender
{
    public:
        DataSender();
        ~DataSender();

        void Initialize(std::string ServerName,int Port);

        void SendData(GamePacket Data);
    private:
        int mPort;
        std::string mServerName;
        GamePacket mData;

        CxUtils::UdpClient mClient;

};

#endif // DATASENDER_H
