#include <iostream>
#include <cxutils/math/cxmath.h>
#include <cxutils/cxutils.h>
#include "mcu/interface.h"
#include "mcu/messagefactory.h"
#include "apl/interface.h"

using namespace std;
using namespace Zebulon;

class MyCallback : public MCU::Interface::Callback
{
public:
    MyCallback()
    {
        for(int i=0;i<11;i++)
        {
            mCount[i]=0;
        }
        mRequest=false;
    }
    ~MyCallback() {}
    virtual void ProcessMessage(const MCU::Message* message)
    {
        //message->Print();
        //cout << "Recieving Message: \n";
#if 0
        if (message->mMessageCode == MCU::CustomMessage::REPORT_FFT)
        {
            MCU::ReportFFT *fftmsg=(MCU::ReportFFT *)message;
            mCount[fftmsg->mDataType]++;
            if(fftmsg->mDataType>0)
            {
                if(fftmsg->mDataType%2==1)
                {
                    mReal=(int)(fftmsg->mData.mUInt);
                    mRealM[(fftmsg->mDataType-1)/2]=mReal;
                }
                else
                {
                    mImag=(int)(fftmsg->mData.mUInt);
                    mImagM[(fftmsg->mDataType-1)/2]=mImag;
                    std::cout << mReal << " " << mImag << " " << sqrt((double)mReal*(double)mReal+(double)mImag*(double)mImag) << " " << CxUtils::CxToDegrees(atan2(mImag,mReal)) << " Deg" << std::endl;
                }
                //printf("%d: %02X %02X %02X %02X\n", fftmsg->mDataType,(int)fftmsg->mData.mUChars.mByte3,(int)fftmsg->mData.mUChars.mByte2,(int)fftmsg->mData.mUChars.mByte1,(int)fftmsg->mData.mUChars.mByte0);
            }
            else
            {
                mFreqIndex=(int)fftmsg->mData.mUInt;
                std::cout << "\n\nFreq: " << (int)fftmsg->mData.mUInt << " = " << fftmsg->mData.mUInt*694444.0/511.0 << std::endl;
            }
            
            if(fftmsg->mDataType==10)
            {
                //printf("All\n");
                double sumR=0.0;
                double sumI=0.0;
                for(int i=1;i<5;i++)
                {
                    //printf("%lf %lf\n",mRealM[i],mImagM[i]);
                    sumR+=mRealM[i]/4.0;
                    sumI+=mImagM[i]/4.0;
                }

                static double BR=0.0;
                static double BI=0.0;

                if(fabs(mRealM[0]-sumR)>BR)
                {
                    BR=fabs(mRealM[0]-sumR);
                }
                if(fabs(mImagM[0]-sumI)>BI)
                {
                    BI=fabs(mImagM[0]-sumI);
                }
                printf("Diff: %lf %lf\n",fabs(mRealM[0]-sumR),fabs(mImagM[0]-sumI));
                printf("Big: %lf %lf\n",BR,BI);

                mRequest=true;
            }
        }
#endif
        mRequest=true;
    }
    int mFreqIndex;
    int mCount[11];
    double mRealM[5];
    double mImagM[5];
    int mRequest;
    int mReal;
    int mImag;
};

int main(int argc, char * argv[])
{
    MCU::Interface mcu;
    APL::Interface apl;
    MCU::SetAPL rawDataMode;
    rawDataMode.mField = MCU::SetAPL::RawDataMode;
    rawDataMode.mValue = 1;

    int i = 0;
    MyCallback callback;
    
    MCU::Ping ping;
    
    bool running = false;

    int baud=115200;
    cout << "Connecting to MCU...\n";
    if(argc > 1)
    {
        running = mcu.Initialize(argv[1], 19200);
    }
    else
    {
        cout << "Default Baud\n";
        running = mcu.Initialize("/dev/auv-fpga", baud, false, CxUtils::CX_SERIAL_ODD_PARITY);
    }
    
    if(running)
    {
        mcu.RegisterCallback(MCU::Message::All, &callback);
        apl.Initialize(&mcu, "/dev/auv-fpga-raw", true, baud);
        
        cout << "Success!\n";
        char key = 0;
        cout << "Press <escape key> To Exit!\n";
        
        //apl.SetPinger(37000, 1, 400);
        //SetPinger(37000, int SampleMode=1,int PingThreshold=50,int PingCount=10,double Q=64,int DCDecay=1,int PktoPkDecay=1, int ArbiterMode=0);
        //apl.SetPinger(37000, 0, 400,20,20,20,20,1);
        //apl.SetPinger(37000, 1, 800, 20, 20, 1, 1, 1);
        //apl.SetPinger(37000, 1, 800, 10, 20, 1, 1, 1);

        //SetPinger(37000, int SampleMode=1,int PingThreshold=50,int PingCount=10,double Q=64,int DCDecay=1,int PktoPkDecay=1, int ArbiterMode=0);
        //<PingerSet Frequency="22000"
        //SampleMode="2"
        //PingThreshold="800"
        //PingCount="20"
        //Q="100"
        //DCDecay="1"
        //PktoPkDecay="1"
        //ArbiterMode="1" />
        //apl.SetPinger(27000, 1, 800, 20, 100, 1, 1, 2);

        apl.SetPinger(27000, 2, 800, 20, 100, 1, 1, 2);
        
        rawDataMode.mValue =0;
        mcu.Send(&rawDataMode);
        int zone = 0;
        
        while(key != 27)
        {
            //cout << "Sending Message: \n";
            
            //Ping
            //ping.Print();
            //mcu.Send(&ping);
            
            static int count=0;
            CxUtils::SleepMs(10);
            //0apl.Print();
            if(0 && count%100==0)
            {
                std::cout << "Message Count: ";
                for(int i=0;i<11;i++)
                {
                    std::cout << callback.mCount[i] << " ";
                }
                std::cout << std::endl;
            }
            /*if(callback.mRequest)
            {
                apl.Print();
                apl.RequestDebugData();
                callback.mRequest=false;
            }*/
            if(count%50==0)
            {
                //apl.Print();
                apl.RequestDebugData();
            }
            count++;
            
            /*rawDataMode.mValue = 0;
            mcu.Send(&rawDataMode);
            CxUtils::SleepMs(4000);
            
            rawDataMode.mValue = 1;
            mcu.Send(&rawDataMode);
            
            key = CxUtils::GetChar();
            CxUtils::SleepMs(10000);*/
        }
    }
    return 0;
}
