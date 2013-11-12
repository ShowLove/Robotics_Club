#include<ieee/communication/imagesender.h>

using namespace CxUtils;
using namespace std;
using namespace cv;

ImageSender::ImageSender()
{

}

ImageSender::~ImageSender()
{

}

void ImageSender::Initialize(std::string ServerName,int Port,int Quality)
{
    mPort=Port;
    mServerName=ServerName;
    mQuality=Quality;
    if(!mClient.InitializeSocket(mServerName, mPort))
    {
        cout << "Unable to connect to host " << mServerName << " on Port " << mPort<< endl;
        return;
    }
    cout << "Initialized UDP Client Socket on port " << mPort << endl;

}


void ImageSender::SendImage(Mat &Image)
{
    vector<unsigned char> outJpg;
    vector<int>params = vector<int>(2);
    params[0] = CV_IMWRITE_JPEG_QUALITY;
    params[1] = mQuality;
    imencode(".jpg",Image,outJpg,params);
    //printf("After: %d\n",(int)outJpg.size());
    //waitKey(1);
    const int maxUPD=5500;
    char packets=outJpg.size()/maxUPD+1;
    char outudp[maxUPD+2];
    int pos=0;
    for(int i=0;i<packets;i++)
    {
        outudp[0]=i;
        outudp[1]=packets;
        int size=maxUPD;
        if(pos+maxUPD>outJpg.size())
        {
            size=outJpg.size()-pos;
        }
        memcpy(outudp+2,&outJpg[0]+pos,size);
        mClient.Send(outudp, size+2);
        pos+=maxUPD;
        CxUtils::SleepMs(1);
    }
}
