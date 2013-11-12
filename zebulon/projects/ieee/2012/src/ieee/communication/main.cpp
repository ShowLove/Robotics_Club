#if 0
#include<stdio.h>
#include<cxutils/cxutils.h>
#include<iostream>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

#include<ieee/communication/imagesender.h>
#include<ieee/communication/datasender.h>

#include<ieee/communication/gameserver.h>

using namespace CxUtils;
using namespace std;
using namespace cv;
//#define PORT      3794
#define HOSTNAME "localhost"



int main(int argc, char *argv[])
{
    Mat img=imread("failwhale.jpg");
    //ImageReceiver ImgR;
    //ImgR.Initialize(PORT);
    GameServer Server;

    Server.Initialize(2);

    ImageSender ImgS;
    ImgS.Initialize(HOSTNAME,3000,95);
    DataSender DataS;
    DataS.Initialize(HOSTNAME,4000);

    Mat imgIn=imread("failwhale.jpg");

    GamePacket Data;

    Data.mTimeMs=0;
    while(1)
    {
        ImgS.SendImage(imgIn);
        Data.mTimeMs+=100;
        DataS.SendData(Data);

        Mat imgOut=Server.GetImage(0);

        imshow("Image",imgOut);
        waitKey(100);
    }

    return 0;
}
#endif
