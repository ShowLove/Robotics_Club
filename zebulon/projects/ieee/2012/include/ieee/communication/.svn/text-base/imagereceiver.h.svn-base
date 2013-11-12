//Receives one image from one client at a time
//Set up to a specfic port for transfering

#include<stdio.h>
#include<cxutils/cxutils.h>
#include<iostream>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>


#ifndef IMAGERECEIVER_H
#define IMAGERECEIVER_H

class ImageReceiver
{
    public:
        ImageReceiver();
        ~ImageReceiver();

        void Initialize(int Port);

        bool GetImage(cv::Mat &Image);

        bool GetValid();

    private:
        static void Interface(void *args);
        void WorkerThread();
        int mPort;
        CxUtils::UdpServer mServer;
        CxUtils::Packet mData;
        CxUtils::IP4Address mSource;

        CxUtils::Mutex mMutex;
        CxUtils::Thread mThread;

        cv::Mat mImage;
        int mReceiveCount;
        bool mValid;


};

#endif // IMAGERECEIVER_H
