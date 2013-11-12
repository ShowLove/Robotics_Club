//Used to pass image data to a waiting receiver
//this is non-threaded as it happens on demand

#include<stdio.h>
#include<cxutils/cxutils.h>
#include<iostream>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<string>


#ifndef IMAGESENDER_H
#define IMAGESENDER_H

class ImageSender
{
    public:
        ImageSender();
        ~ImageSender();

        void Initialize(std::string ServerName,int Port,int Quality=95);

        void SendImage(cv::Mat &Image);
    private:
        int mPort;
        int mQuality;
        std::string mServerName;

        CxUtils::UdpClient mClient;

};

#endif // IMAGESENDER_H
