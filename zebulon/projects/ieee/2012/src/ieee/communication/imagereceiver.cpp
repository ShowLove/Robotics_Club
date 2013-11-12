#include "ieee/communication/imagereceiver.h"

using namespace CxUtils;
using namespace std;
using namespace cv;

ImageReceiver::ImageReceiver()
{
    mValid = false;
}

ImageReceiver::~ImageReceiver()
{
    mThread.StopThread(1000);

}

void ImageReceiver::Initialize(int Port)
{
    mPort=Port;
    mReceiveCount=0;

    if(!mServer.InitializeSocket(mPort))
    {
        cout << "Unable to initialize on port " << mPort << endl;
        return;
    }
    cout << "Initialized UDP Server Socket on port " << mPort << endl;

    if(!mThread.CreateThread(&ImageReceiver::Interface,this))
    {
            cout << "Could not Create Thread" << endl;
    }
}

void ImageReceiver::Interface(void *args)
{
    ImageReceiver *imgr=(ImageReceiver *)args;
    imgr->WorkerThread();
}

void ImageReceiver::WorkerThread()
{
    cout << "Thread Started..." << endl;
    int timeOutCount=0;
    char fullimage[(int)1e6];
    int count=0;
    int size=0;
    int pos=0;
    int fail=0;
    int good=0,bad=0;

    while(mThread.QuitThreadFlag() == false)
    {
        //wait can determine frame rate
        if(mServer.Recv(mData, 5502, 0, &mSource) > 0)
        {
            //cout << mSource.mString << " " << mPort << " Received (" << mData.Size() << " bytes): " << endl;
            unsigned char *tmp=mData.Ptr();
            //printf("Segment: %d %d %d\n",count,(int)tmp[0],(int)tmp[1]);
            if(tmp[0]==0)
            {
                count=1;
                pos=0;
                fail=0;
                int size=mData.Size()-2;
                memcpy(fullimage+pos,mData.Ptr()+2,size);
                pos+=size;
            }
            else
            {
                if(tmp[0]==count)
                {
                    count++;
                    int size=mData.Size()-2;
                    memcpy(fullimage+pos,mData.Ptr()+2,size);
                    pos+=size;
                }
                else if(fail==0)
                {
                    fail=1;
                    bad++;
                    printf("Segment: %d %d %d %d %d\n",count,(int)tmp[0],(int)tmp[1],good,bad);
                }
            }
            //received them all
            if(count==tmp[1])
            {
                good++;
                //printf("Full: %d\n",pos);

                Mat jpgIn(1, pos,CV_8UC1, fullimage);
                Mat image=imdecode(jpgIn,1);
                //Copy image
                mMutex.Lock();
                image.copyTo(mImage);
                mReceiveCount++;
                mMutex.Unlock();
                timeOutCount=0;
                mValid=true;
            }
        }
        else
        {
            timeOutCount++;
            if(timeOutCount>100)
            {
                //std::cout << "TIMED OUT: IMAGE " << timeOutCount << std::endl;
                mValid=false;
            }

        }
    }
}

bool ImageReceiver::GetImage(Mat &Image)
{
    if(mReceiveCount==0)
    {
        return false;
    }
    mMutex.Lock();
    Image=mImage;
    mMutex.Unlock();
    return true;
}

bool ImageReceiver::GetValid()
{
    return mValid;
}
