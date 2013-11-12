//Receives one image from one client at a time
//Set up to a specfic port for transfering

#include<stdio.h>
#include<cxutils/cxutils.h>
#include<iostream>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

#include<ieee/communication/gamepacket.h>

#ifndef DATARECEIVER_H
#define DATARECEIVER_H

class DataReceiver
{
    public:
        DataReceiver();
        ~DataReceiver();

        void Initialize(int Port,int TrailLength=50);

        ProcessedData GetData();

        bool GetValid();

    private:
        static void Interface(void *args);
        void WorkerThread();
        void Process(GamePacket &Data);
        int mPort;
        CxUtils::UdpServer mServer;
        CxUtils::Packet mData;
        CxUtils::IP4Address mSource;

        CxUtils::Mutex mMutex;
        CxUtils::Thread mThread;

        int mReceiveCount;
        int mTrailLength;
        bool mValid;

        //Local Data
        struct ProcessedData mProcData;

};

#endif // DATARECEIVER_H
