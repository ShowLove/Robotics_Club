/////////////////////////////////////////////////////////////////////////////////////
///
/// \file interface.cpp
/// \brief Acoustic Pinger Locator Interface.
///
/// Author(s): Jonathan Mohlenhoff<br>
/// Created: 4/12/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: jmohlenh@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////

#include "apl/interface.h"
#include <iostream>
using namespace Zebulon;
using namespace APL;

Interface::Interface()
{
    mADPos = 0;
    mNewData = 0;
    mNumTranfers = 0;
    mDisplay = false;
    
    mTimeImage = cvCreateImage(cvSize(512,500), 8, 3);
    mFreqImage = cvCreateImage(cvSize(512,500), 8, 3);
    mPingTimeImage = cvCreateImage(cvSize(500,500), 8, 3);
    
    //mZoneFilter = new Utility::AverageFilter(1);
    mZoneFilter = new Utility::KalmanLinearFilter(4);
    //((Utility::KalmanLinearFilter *)mZoneFilter)->KalmanConst(0.1,5.0);
    mZoneFilter->Setup(0.0,360.0);
    
    mAD0Time = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * MaxSamples);
    mAD1Time = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * MaxSamples);
    mAD2Time = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * MaxSamples);
    mAD3Time = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * MaxSamples);
    mAD4Time = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * MaxSamples);
    
    mAD0Freq = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * MaxSamples);
    mAD1Freq = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * MaxSamples);
    mAD2Freq = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * MaxSamples);
    mAD3Freq = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * MaxSamples);
    mAD4Freq = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * MaxSamples);
    
    mFFTPlan0 = fftw_plan_dft_1d(MaxSamples, mAD0Time, mAD0Freq, FFTW_FORWARD, FFTW_ESTIMATE);
    mFFTPlan1 = fftw_plan_dft_1d(MaxSamples, mAD1Time, mAD1Freq, FFTW_FORWARD, FFTW_ESTIMATE);
    mFFTPlan2 = fftw_plan_dft_1d(MaxSamples, mAD2Time, mAD2Freq, FFTW_FORWARD, FFTW_ESTIMATE);
    mFFTPlan3 = fftw_plan_dft_1d(MaxSamples, mAD3Time, mAD3Freq, FFTW_FORWARD, FFTW_ESTIMATE);
    mFFTPlan4 = fftw_plan_dft_1d(MaxSamples, mAD4Time, mAD4Freq, FFTW_FORWARD, FFTW_ESTIMATE);
    
    mRequestFlag = false;
    
    mCallback = NULL;
    mOldError=0.0;
    mGain=50;
    mMax=75;
    mMin=25;
    
}


Interface::~Interface()
{
    Shutdown();
    
    fftw_free(mAD0Time);
    fftw_free(mAD1Time);
    fftw_free(mAD2Time);
    fftw_free(mAD3Time);
    fftw_free(mAD4Time);

    fftw_free(mAD0Freq);
    fftw_free(mAD1Freq);
    fftw_free(mAD2Freq);
    fftw_free(mAD3Freq);
    fftw_free(mAD4Freq);
    
    fftw_destroy_plan(mFFTPlan0);
    fftw_destroy_plan(mFFTPlan1);
    fftw_destroy_plan(mFFTPlan2);
    fftw_destroy_plan(mFFTPlan3);
    fftw_destroy_plan(mFFTPlan4);

    cvReleaseImage(&mTimeImage);
    cvReleaseImage(&mFreqImage);
    cvReleaseImage(&mPingTimeImage);
}


void Interface::Shutdown()
{
    mRawDataThread.StopThread();
    mProcessThread.StopThread();
    mRequestDataThread.StopThread();
    mSerial.Disconnect();
    mADPos=0;
}


bool Interface::Initialize(MCU::Interface* mcu,
                           std::string rawPort,
                           bool display,
                           int rawBaud)
{
    if(!mcu)
    {
        std::cout << "Invalid pointer received for APL MCU\n";
        return false;
    }
    mMCU = mcu;
    mMCU->RegisterCallback(MCU::Message::All, this);
    
    if(rawPort != "")
    {
        if(mSerial.Connect(rawPort, rawBaud, 8, CxUtils::CX_SERIAL_ODD_PARITY, 1))
        {
            mDisplay = display;
            CxUtils::SleepMs(50);
            mRawDataThread.CreateThread(Interface::RawDataThread, this);
            mProcessThread.CreateThread(Interface::ProcessThread, this);
            mRequestDataThread.CreateThread(Interface::RequestDataThread, this);
        }
        else
        {
            std::cout << "Unable to connect to APL Raw Data Port\n";
            return false;
        }
    }
    return true;
}


void Interface::RawDataThread(void* arg)
{
    Interface* apl = (Interface*)arg;
    
    CxUtils::Packet mLargeBuffer;           ///< Packet buffer to store received byte data in.
    CxUtils::Packet mSmallBuffer;           ///< Packet buffer to store received byte data in.
    
    double startTime=CxUtils::GetTimeSeconds();
    double pingTime=CxUtils::GetTimeSeconds();
    while(apl->mRawDataThread.QuitThreadFlag() == false)
    {
        if(apl->mSerial.IsConnected())
        {
            if(apl->mSerial.Recv(mSmallBuffer, 512) > 0)
            {
                // Add data to larger buffer.
                mLargeBuffer += mSmallBuffer;
                
                // Look for a message within the buffered data.
                unsigned int endPos = 0;
                unsigned char* buffer = (unsigned char*)mLargeBuffer.Ptr();
                for(unsigned int pos = 0; pos < mLargeBuffer.Length(); pos++)
                {
                    if(apl->mADPos>0 && mLargeBuffer.Length()-pos>4 && buffer[pos]==0xAA && buffer[pos+1]==0xAA && buffer[pos+2]==0xAA && buffer[pos+3]==0xAA)
                    {
                        //Sampled the correct number
                        if(apl->mADPos==MaxSamples)
                        {
                            //Do a transfer to process thread
                            apl->TransferToBuffer();
                        }
                        apl->mADPos=0;
                        double pingEndTime=CxUtils::GetTimeSeconds();
                        //fprintf(stderr,"Ping Time: %lf\n",pingEndTime-pingTime);
                        pingTime=pingEndTime;
                    }
                    
                    if(buffer[pos] == 0xFF)
                    {
                        CxUtils::Packet extractedMessage;
                        
                        if(apl->mADPos==0)
                        {
                            startTime=CxUtils::GetTimeSeconds();
                        }
                        if(apl->ExtractMessage(&buffer[pos], mLargeBuffer.Length() - pos, extractedMessage))
                        {
                            pos += extractedMessage.Length() - 1;
                            endPos = pos + 1;
                        }
                        if(apl->mADPos==512)
                        {
                            double endTime=CxUtils::GetTimeSeconds();
                            //fprintf(stderr,"Time Passed: %lf\n",endTime-startTime);                            
                        }
                    }
                }
                
                // Clear out all data in buffer up until after the last message read.
                if(endPos > 0)
                {
                    mLargeBuffer.Delete(endPos, 0);
                }
                // Clear bad data.
                if(mLargeBuffer.Length() >= 2000)
                {
                    mLargeBuffer.Clear();
                }
            }
        }
        CxUtils::SleepMs(1);
    }
}


unsigned int Interface::ExtractMessage(const unsigned char* buffer, const unsigned int length, CxUtils::Packet& message)
{
    unsigned int bytesRead = 0;
    message.Clear();
    
    if(buffer[0] == 0xFF && length >= PacketSize)
    {
        unsigned char checksum = 0xFF;
        for(int i=1;i<PacketSize-1;i++)
        {
            checksum^=buffer[i];
        }

        if(checksum == buffer[PacketSize-1])
        {
            bytesRead = PacketSize;
            ExtractDataFromMessage(buffer);
            message.Write(buffer, bytesRead);
        }
    }
    
    return bytesRead;
}


int Interface::ExtractDataFromMessage(const unsigned char *Buffer)
{
    //Already passed the Checksum
    mDataMutex.Lock();
    //First Read
    if(mADPos==0)
    {
        if(mCallback!=NULL)
        {
            mCallback->LockYaw();
        }
    }
    if(mADPos<MaxSamples)
    {
        //front
        mAD0[mADPos]=((unsigned int)(Buffer[1]))<<8 | (unsigned int)Buffer[2];
        //right
        mAD1[mADPos]=((unsigned int)(Buffer[3]))<<8 | (unsigned int)Buffer[4];
        //back
        mAD2[mADPos]=((unsigned int)(Buffer[5]))<<8 | (unsigned int)Buffer[6];
        //left
        mAD3[mADPos]=((unsigned int)(Buffer[7]))<<8 | (unsigned int)Buffer[8];
        //center
        mAD4[mADPos]=((unsigned int)(Buffer[9]))<<8 | (unsigned int)Buffer[10];
    }
    mADPos++;
    mDataMutex.Unlock();
}


void Interface::ProcessThread(void* arg)
{
    int updatedisplay=0;
    Interface* apl = (Interface*)arg;
    
    apl->mLastTime=CxUtils::GetTimeSeconds();
    while(apl->mProcessThread.QuitThreadFlag() == false)
    {
        apl->mProcessMutex.Lock();
        if(apl->mNewData == 1)
        {
            fftw_execute(apl->mFFTPlan0);
            fftw_execute(apl->mFFTPlan1);
            fftw_execute(apl->mFFTPlan2);
            fftw_execute(apl->mFFTPlan3);
            fftw_execute(apl->mFFTPlan4);
            apl->Calc();
            updatedisplay=1;
            apl->mNewData=0;
        }
        apl->mProcessMutex.Unlock();
        
        if(updatedisplay)
        {
            if(apl->mDisplay)
            {
                apl->DisplayTime();
                apl->DisplayFreq();
            }
            updatedisplay=0;
        }
        //check every couple of seconds
        if(CxUtils::GetTimeSeconds()-apl->mLastTime>2.0)
        {
            printf("Did not find a pinger, increase %lf\n",apl->mGain);
            apl->mGain+=1.0;
            apl->UpdateGain();
            apl->mLastTime=CxUtils::GetTimeSeconds();
        }
        
        CxUtils::SleepMs(1);
    }
}


int Interface::TransferToBuffer()
{
    if(mDisplay)
    {
        //printf("Transfer: %d\n",mNumTranfers);
        fflush(stdout);
    }
    mDataMutex.Lock();
    memcpy(mDBAD0,mAD0,sizeof(unsigned int)*MaxSamples);
    memcpy(mDBAD1,mAD1,sizeof(unsigned int)*MaxSamples);
    memcpy(mDBAD2,mAD2,sizeof(unsigned int)*MaxSamples);
    memcpy(mDBAD3,mAD3,sizeof(unsigned int)*MaxSamples);
    memcpy(mDBAD4,mAD4,sizeof(unsigned int)*MaxSamples);
    mDataMutex.Unlock();
    
    
    static int onetime=0;
    mProcessMutex.Lock();
    
    
    MakeComplex(mDBAD0,mAD0Time);
    MakeComplex(mDBAD1,mAD1Time);
    MakeComplex(mDBAD2,mAD2Time);
    MakeComplex(mDBAD3,mAD3Time);
    MakeComplex(mDBAD4,mAD4Time);
    
    static FILE *tmpstream;
    if(onetime==0)
    {
        onetime=1;
        char tmp[1024];
        sprintf(tmp,"logs/hydro/hydro%d.log",(int)time(NULL));
        tmpstream=fopen(tmp,"w");
    }
    
    fprintf(tmpstream,"Transfer: %d\n",mNumTranfers);
    for(int i=0;i<MaxSamples;i++)
    {
        fprintf(tmpstream,"%lf %lf %lf %lf %lf\n",mAD0Time[i][0],mAD1Time[i][0],mAD2Time[i][0],mAD3Time[i][0],mAD4Time[i][0]);
        //fprintf(tmpstream,"%u %u %u %u\n",mDBAD0[i],mDBAD1[i],mDBAD2[i],mDBAD3[i]);
    }
    fflush(tmpstream);
    //fclose(tmpstream);
    
    mNewData=1;
    mNumTranfers++;
    mProcessMutex.Unlock();
    
    return 1;
}


void Interface::MakeComplex(unsigned int *mAD,fftw_complex *mADTime)
{
    for(int i=0;i<MaxSamples;i++)
    {
        //Should be reversed
        //mADTime[i][0]=((double)mAD[MaxSamples-1-i]-CenterValue)/CenterValue*NormalRange;
        //Try not reversing
        mADTime[i][0]=((double)mAD[i]-CenterValue)/CenterValue*NormalRange;
        mADTime[i][1]=0.0;
    }
    return;
}


void Interface::RequestDataThread(void* arg)
{
    Interface* apl = (Interface*)arg;
    
    MCU::ReportDCOffset dc;
    MCU::ReportPkToPk pk;
    MCU::ReportDPotValues pot;
    MCU::ReportPingTime pingTime;
    MCU::ReportSampleFreq sampleFreq;
    MCU::ReportSamplePeriod samplePer;
    
    while(apl->mRequestDataThread.QuitThreadFlag() == false)
    {
        apl->mRequestMutex.Lock();
        if(apl->mRequestFlag)
        {
            apl->mRequestFlag=false;
            apl->mRequestMutex.Unlock();
            for(int i=0; i<Channel::NumChannels; i++)
            {
                dc.mChannel = i;
                apl->mMCU->Send(&dc, 25);
                
                pk.mChannel = i;
                apl->mMCU->Send(&pk, 25);
                
                pingTime.mChannel = i;
                apl->mMCU->Send(&pingTime, 25);
                
                for(int j=0; j<2; j++)
                {
                    pot.mChannel = i*2 + j;
                    apl->mMCU->Send(&pot, 25);
                }
            }
            apl->mMCU->Send(&sampleFreq, 25);
            apl->mMCU->Send(&samplePer, 25);
            //std::cout << "\n\nMessages Sent in: " << (CxUtils::GetTimeMs() - startTime) << "ms.\n\n";
            //apl->Print();
        }
        else
        {
            apl->mRequestMutex.Unlock();
        }

        CxUtils::SleepMs(100);
    }
}

void Interface::DisplayTime()
{
    cvRectangle( mTimeImage, cvPoint(0,0), cvPoint(mTimeImage->width,mTimeImage->height), CV_RGB(255,255,255), -1);
    cvLine(mTimeImage,cvPoint(0,50),cvPoint(mTimeImage->width,50),CV_RGB(0,0,0),1);
    cvLine(mTimeImage,cvPoint(0,150),cvPoint(mTimeImage->width,150),CV_RGB(0,0,0),1);
    cvLine(mTimeImage,cvPoint(0,250),cvPoint(mTimeImage->width,250),CV_RGB(0,0,0),1);
    cvLine(mTimeImage,cvPoint(0,350),cvPoint(mTimeImage->width,350),CV_RGB(0,0,0),1);
    cvLine(mTimeImage,cvPoint(0,450),cvPoint(mTimeImage->width,450),CV_RGB(0,0,0),1);
    
    for(int i=0;i<MaxSamples-1;i++)
    {
        cvLine(mTimeImage,cvPoint(i,-mAD0Time[i][0]/NormalRange*50+50),cvPoint(i+1,-mAD0Time[i+1][0]/NormalRange*50+50),CV_RGB(255,0,0),1);
        cvLine(mTimeImage,cvPoint(i,-mAD1Time[i][0]/NormalRange*50+150),cvPoint(i+1,-mAD1Time[i+1][0]/NormalRange*50+150),CV_RGB(0,255,0),1);
        cvLine(mTimeImage,cvPoint(i,-mAD2Time[i][0]/NormalRange*50+250),cvPoint(i+1,-mAD2Time[i+1][0]/NormalRange*50+250),CV_RGB(0,0,255),1);
        cvLine(mTimeImage,cvPoint(i,-mAD3Time[i][0]/NormalRange*50+350),cvPoint(i+1,-mAD3Time[i+1][0]/NormalRange*50+350),CV_RGB(255,0,255),1);
        cvLine(mTimeImage,cvPoint(i,-mAD4Time[i][0]/NormalRange*50+450),cvPoint(i+1,-mAD4Time[i+1][0]/NormalRange*50+450),CV_RGB(0,255,255),1);
    }
    Utility::Display::DisplayImage("HydroTime",mTimeImage);
}

void Interface::DisplayFreq()
{
    cvRectangle( mFreqImage, cvPoint(0,0), cvPoint(mFreqImage->width,mFreqImage->height), CV_RGB(255,255,255), -1);
    cvLine(mFreqImage,cvPoint(0,100),cvPoint(mFreqImage->width,100),CV_RGB(0,0,0),1);
    cvLine(mFreqImage,cvPoint(0,200),cvPoint(mFreqImage->width,200),CV_RGB(0,0,0),1);
    cvLine(mFreqImage,cvPoint(0,300),cvPoint(mFreqImage->width,300),CV_RGB(0,0,0),1);
    cvLine(mFreqImage,cvPoint(0,400),cvPoint(mFreqImage->width,400),CV_RGB(0,0,0),1);
    cvLine(mFreqImage,cvPoint(0,500),cvPoint(mFreqImage->width,500),CV_RGB(0,0,0),1);
    
    if(mDisplay)
    {
        //printf("Freq: %lf %lf %lf %lf\n",Hydro0.Freq,Hydro1.Freq,Hydro2.Freq,Hydro3.Freq);
        //printf("Phase: %lf %lf %lf %lf\n",Hydro0.Phase*180/M_PI,Hydro1.Phase*180/M_PI,Hydro2.Phase*180/M_PI,Hydro3.Phase*180/M_PI);
    }
    
    fflush(stdout);
    for(int i=0;i<MaxSamples/2;i++)
    {
        double tmpmag;
        
        /*tmpmag=sqrt(mAD0Freq[i][0]*mAD0Freq[i][0]+mAD0Freq[i][1]*mAD0Freq[i][1])/150.0*100;
        cvLine(mFreqImage,cvPoint(i*2,100),cvPoint(i*2,-tmpmag+100),CV_RGB(255,0,0),1);
        
        tmpmag=sqrt(mAD1Freq[i][0]*mAD1Freq[i][0]+mAD1Freq[i][1]*mAD1Freq[i][1])/150.0*100;
        cvLine(mFreqImage,cvPoint(i*2,200),cvPoint(i*2,-tmpmag+200),CV_RGB(0,255,0),1);
        
        tmpmag=atan2(mAD1Freq[i][1],mAD1Freq[i][0])/M_PI*200;
        cvLine(mFreqImage,cvPoint(i*2,300),cvPoint(i*2,-tmpmag+300),CV_RGB(0,0,255),1);      

        //tmpmag=mAD1Freq[i][0]/300.0*100;
        //cvLine(mFreqImage,cvPoint(i*2,200),cvPoint(i*2,-tmpmag+200),CV_RGB(0,255,0),1);
        
        //tmpmag=mAD1Freq[i][1]/300.0*100;
        //cvLine(mFreqImage,cvPoint(i*2,300),cvPoint(i*2,-tmpmag+300),CV_RGB(255,0,0),1);  

        //tmpmag=sqrt(mAD2Freq[i][0]*mAD2Freq[i][0]+mAD2Freq[i][1]*mAD2Freq[i][1])/150.0*100;
        //cvLine(mFreqImage,cvPoint(i*2,300),cvPoint(i*2,-tmpmag+300),CV_RGB(0,0,255),1);

        tmpmag=sqrt(mAD3Freq[i][0]*mAD3Freq[i][0]+mAD3Freq[i][1]*mAD3Freq[i][1])/150.0*100;
        cvLine(mFreqImage,cvPoint(i*2,400),cvPoint(i*2,-tmpmag+400),CV_RGB(255,255,0),1);*/
        
        double scale=300;
        tmpmag=sqrt(mAD0Freq[i][0]*mAD0Freq[i][0]+mAD0Freq[i][1]*mAD0Freq[i][1])/scale*100;
        if(tmpmag>100)
        {
            tmpmag=100;
        }
        cvLine(mFreqImage,cvPoint(i*2,100),cvPoint(i*2,-tmpmag+100),CV_RGB(255,0,0),1);

        tmpmag=sqrt(mAD1Freq[i][0]*mAD1Freq[i][0]+mAD1Freq[i][1]*mAD1Freq[i][1])/scale*100;
        if(tmpmag>100)
        {
            tmpmag=100;
        }
        cvLine(mFreqImage,cvPoint(i*2,200),cvPoint(i*2,-tmpmag+200),CV_RGB(0,255,0),1);
        
        tmpmag=sqrt(mAD2Freq[i][0]*mAD2Freq[i][0]+mAD2Freq[i][1]*mAD2Freq[i][1])/scale*100;
        if(tmpmag>100)
        {
            tmpmag=100;
        }
        cvLine(mFreqImage,cvPoint(i*2,300),cvPoint(i*2,-tmpmag+300),CV_RGB(0,0,255),1);

        tmpmag=sqrt(mAD3Freq[i][0]*mAD3Freq[i][0]+mAD3Freq[i][1]*mAD3Freq[i][1])/scale*100;
        if(tmpmag>100)
        {
            tmpmag=100;
        }
        cvLine(mFreqImage,cvPoint(i*2,400),cvPoint(i*2,-tmpmag+400),CV_RGB(255,0,255),1);

        tmpmag=sqrt(mAD4Freq[i][0]*mAD4Freq[i][0]+mAD4Freq[i][1]*mAD4Freq[i][1])/scale*100;
        if(tmpmag>100)
        {
            tmpmag=100;
        }
        cvLine(mFreqImage,cvPoint(i*2,500),cvPoint(i*2,-tmpmag+500),CV_RGB(0,255,255),1);
    }
    Utility::Display::DisplayImage("HydroFreq",mFreqImage);
}


void Interface::DisplayPingTime(int zone)
{
    mZoneFilter->Add(zone*45.0);
    cvRectangle(mPingTimeImage, cvPoint(0,0), cvPoint(mTimeImage->width,mTimeImage->height), CV_RGB(255,255,255), -1);
    cvCircle(mPingTimeImage, cvPoint(250,250), 225, CV_RGB(0,0,0), -1);
    double angle;
    mZoneFilter->Output(angle);
    std::cout << "Zone: " << zone << ", Angle: " << angle << std::endl;
    cvEllipse(mPingTimeImage, cvPoint(250,250), cvSize(225,225), 90.0, angle, angle+45.0, CV_RGB(255,255,0), -1);
    cvLine(mPingTimeImage, cvPoint(250,250), cvPoint(250,25), CV_RGB(255,0,0), 3);
    cvLine(mPingTimeImage, cvPoint(250,25), cvPoint(225,50), CV_RGB(255,0,0), 3);
    cvLine(mPingTimeImage, cvPoint(250,25), cvPoint(275,50), CV_RGB(255,0,0), 3);

    cvCircle(mPingTimeImage, cvPoint(250, 100), 20, CV_RGB(138,43,226), -1);    // blue violet
    cvCircle(mPingTimeImage, cvPoint(100, 250), 20, CV_RGB(0,0,205), -1); //Medium blue
    cvCircle(mPingTimeImage, cvPoint(250, 400), 20, CV_RGB(255,69,0), -1); //orange
    cvCircle(mPingTimeImage, cvPoint(400, 250), 20, CV_RGB(124,252,0), -1);    //lawn green


    CvFont font;
    double hScale =1.0;
    double vScale =1.0;
    int lineWidth =2;

    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);

    cvPutText(mPingTimeImage, "A", cvPoint(240, 105), &font, cvScalar(0,0,0)); // blue violet
    cvPutText(mPingTimeImage, "B", cvPoint(90, 255), &font, cvScalar(0,0,0));    //Medium blue
    cvPutText(mPingTimeImage, "C", cvPoint(240, 405), &font, cvScalar(0,0,0));    //orange
    cvPutText(mPingTimeImage, "D", cvPoint(390, 255), &font, cvScalar(0,0,0));    //lawn green

    Utility::Display::DisplayImage("PingTime", mPingTimeImage);


}


void Interface::SetPinger(double Frequency, int SampleMode, int PingThreshold, int PingCount, double Q,int DCDecay,int PktoPkDecay, int ArbiterMode,double Min,double Max)
{
    MCU::SetPingerFrequency setpinger;
    setpinger.mFrequency = Frequency;
    setpinger.mQ = Q;
    mFrequency=Frequency;

    MCU::SetAPL setdcoffset;
    setdcoffset.mField = MCU::SetAPL::DcCalcDecay;
    setdcoffset.mValue = DCDecay;

    MCU::SetAPL setpk;
    setpk.mField = MCU::SetAPL::PkToPkDecay;
    setpk.mValue = PktoPkDecay;

    MCU::SetAPL rawOutput;
    rawOutput.mField = MCU::SetAPL::RawMode;
    //rawOutput.mValue = 0; //Continuous mode
    //rawOutput.mValue = 1; //Pings or'ed
    //rawOutput.mValue = 2; //Pings and'ed
    rawOutput.mValue = SampleMode;

    MCU::SetAPL setThreshold;
    setThreshold.mField = MCU::SetAPL::PingThresholdAbsFromDC;
    //setThreshold.mField = MCU::SetAPL::PingThresholdRelative;
    setThreshold.mValue = PingThreshold;

    MCU::SetAPL setPingCount;
    setPingCount.mField = MCU::SetAPL::PingCount;
    setPingCount.mValue = PingCount;
    
    MCU::SetAPL setArbiterMode;
    setArbiterMode.mField = MCU::SetAPL::ArbiterMode;
    setArbiterMode.mValue = ArbiterMode; // 0 Channel gains are set together, 1 gains are set independently, 2 gains manual

    //reset gain to 50, start the process over again, switched frequency or just starting
    MCU::SetAPL setGainValue;
    setGainValue.mField = MCU::SetAPL::VariableGain;
    mMin=Min;
    mMax=Max;
    mGain=(mMax+mMin)/2.0;
    setGainValue.mValue = floor(mGain);


    //setdcoffset.Print();
    mMCU->Send(&setdcoffset);
        
    //setpk.Print();
    mMCU->Send(&setpk);

    //setpinger.Print();
    mMCU->Send(&setpinger);

    mMCU->Send(&rawOutput);

    mMCU->Send(&setThreshold);

    mMCU->Send(&setPingCount);
    
    mMCU->Send(&setArbiterMode);

    mMCU->Send(&setGainValue);
}


void Interface::Print()
{
    for(int i=0; i<Channel::NumChannels; i++)
    {
        std::cout << "Channel " << i << ":\n";
        mChannel[i].Print();
    }
    std::cout << "-------------------------------------------\n";
}

void Interface::ProcessMessage(const MCU::Message* message)
{
    switch(message->mMessageCode)
    {
        case MCU::CustomMessage::REPORT_DC_OFFSET:
        {
            mChannel[((MCU::ReportDCOffset*)message)->mChannel].DCOffsetReceived((MCU::ReportDCOffset*) message);
            break;
        }
        
        case MCU::CustomMessage::REPORT_D_POT_VALUES:
        {
            mChannel[((MCU::ReportDPotValues*)message)->mChannel/2].DPotValuesReceived((MCU::ReportDPotValues*) message);
            break;
        }
        
        case MCU::CustomMessage::REPORT_PING_TIME:
        {
            mChannel[((MCU::ReportPingTime*)message)->mChannel].PingTimeReceived((MCU::ReportPingTime*) message);
            if(((MCU::ReportPingTime*)message)->mChannel == (Channel::NumChannels-1))
            {
                for(int i=0;i<Channel::NumChannels;i++)
                {
                    mTimes[i] = mChannel[i].GetPingTime();
                }
                /*std::cout << mTimeA << ", " << mTimeB << ", " << mTimeC << ", " << mTimeD << "\n";
                if((mTimeA < mTimeB) && (mTimeB < mTimeD) && (mTimeD < mTimeC))
                {
                    DisplayPingTime(0);
                }
                else if((mTimeB < mTimeA) && (mTimeA < mTimeC) && (mTimeC < mTimeD))
                {
                    DisplayPingTime(1);
                }
                else if((mTimeB < mTimeC) && (mTimeC < mTimeA) && (mTimeA < mTimeD))
                {
                    DisplayPingTime(2);
                }
                else if((mTimeC < mTimeB) && (mTimeB < mTimeD) && (mTimeD < mTimeA))
                {
                    DisplayPingTime(3);
                }
                else if((mTimeC < mTimeD) && (mTimeD < mTimeB) && (mTimeB < mTimeA))
                {
                    DisplayPingTime(4);
                }
                else if((mTimeD < mTimeC) && (mTimeC < mTimeA) && (mTimeA < mTimeB))
                {
                    DisplayPingTime(5);
                }
                else if((mTimeD < mTimeA) && (mTimeA < mTimeC) && (mTimeC < mTimeB))
                {
                    DisplayPingTime(6);
                }
                else if((mTimeA < mTimeD) && (mTimeD < mTimeB) && (mTimeB < mTimeC))
                {
                    DisplayPingTime(7);
                }
                else
                {
                    std::cout << "Invalid Combination\n";
                }*/
            }
            break;
        }
        
        case MCU::CustomMessage::REPORT_PK_TO_PK:
        {
            mChannel[((MCU::ReportPkToPk*)message)->mChannel].PkToPkReceived((MCU::ReportPkToPk*) message);
            break;
        }
        
        case MCU::CustomMessage::REPORT_SAMPLE_FREQ:
        {
            mChannel[0].SampleFreqReceived((MCU::ReportSampleFreq*) message);
            break;
        }
        
        case MCU::CustomMessage::REPORT_SAMPLE_PERIOD:
        {
            mChannel[0].SamplePeriodReceived((MCU::ReportSamplePeriod*) message);
            break;
        }
    }

}

void Interface::RequestDebugData()
{
    CxUtils::Mutex::ScopedLock lock(&mRequestMutex);
    mRequestFlag = true;
}

void Interface::SetCallback(APL::Callback *callback)
{
    mCallback=callback;
}

void Interface::Calc()
{
    double Max[APL::Channel::NumChannels];
    double Real[APL::Channel::NumChannels];
    double Imag[APL::Channel::NumChannels];
    int Index[APL::Channel::NumChannels];
    double Freq[APL::Channel::NumChannels];

    double MaxTime[APL::Channel::NumChannels];


    double tmpmag;
    for(int i=0;i<APL::Channel::NumChannels;i++)
    {
        Max[i]=0;
        Index[i]=0;
        MaxTime[i]=0;
    }
    for(int i=0;i<APL::Channel::MaxSamples;i++)
    {
        double tmp;
        
        tmp=fabs(mAD0Time[i][0]);
        if(tmp>MaxTime[0])
        {
            MaxTime[0]=tmp;
        }
        tmp=fabs(mAD1Time[i][0]);
        if(tmp>MaxTime[1])
        {
            MaxTime[1]=tmp;
        }
        tmp=fabs(mAD2Time[i][0]);
        if(tmp>MaxTime[2])
        {
            MaxTime[2]=tmp;
        }
        tmp=fabs(mAD3Time[i][0]);
        if(tmp>MaxTime[3])
        {
            MaxTime[3]=tmp;
        }
        tmp=fabs(mAD4Time[i][0]);
        if(tmp>MaxTime[4])
        {
            MaxTime[4]=tmp;
        }
    }
    
    //scan looking for max, skip DC
    for(int i=5;i<APL::Channel::MaxSamples/2;i++)
    {
        tmpmag=sqrt(mAD0Freq[i][0]*mAD0Freq[i][0]+mAD0Freq[i][1]*mAD0Freq[i][1]);
        if(tmpmag>Max[0])
        {
            Max[0]=tmpmag;
            Index[0]=i;
            Real[0]=mAD0Freq[i][0];
            Imag[0]=mAD0Freq[i][1];
        }
        
        tmpmag=sqrt(mAD1Freq[i][0]*mAD1Freq[i][0]+mAD1Freq[i][1]*mAD1Freq[i][1]);
        if(tmpmag>Max[1])
        {
            Max[1]=tmpmag;
            Index[1]=i;
            Real[1]=mAD1Freq[i][0];
            Imag[1]=mAD1Freq[i][1];
        }
        
        tmpmag=sqrt(mAD2Freq[i][0]*mAD2Freq[i][0]+mAD2Freq[i][1]*mAD2Freq[i][1]);
        if(tmpmag>Max[2])
        {
            Max[2]=tmpmag;
            Index[2]=i;
            Real[2]=mAD2Freq[i][0];
            Imag[2]=mAD2Freq[i][1];
        }
        
        tmpmag=sqrt(mAD3Freq[i][0]*mAD3Freq[i][0]+mAD3Freq[i][1]*mAD3Freq[i][1]);
        if(tmpmag>Max[3])
        {
            Max[3]=tmpmag;
            Index[3]=i;
            Real[3]=mAD3Freq[i][0];
            Imag[3]=mAD3Freq[i][1];
        }
        
        tmpmag=sqrt(mAD4Freq[i][0]*mAD4Freq[i][0]+mAD4Freq[i][1]*mAD4Freq[i][1]);
        if(tmpmag>Max[4])
        {
            Max[4]=tmpmag;
            Index[4]=i;
            Real[4]=mAD4Freq[i][0];
            Imag[4]=mAD4Freq[i][1];
        }
        
    }
    
    for(int i=0;i<APL::Channel::NumChannels;i++)
    {
        Freq[i]=Index[i]*Channel::GetSampleFrequency()/(APL::Channel::MaxSamples-1);
        //std::cout << "Channel: " << i << " Index " << Index[i] << " Freq " << Freq[i] << " Real " << Real[i] << " Imag " << Imag[i] << std::endl;
    }
    
    
    //Add internal calculation for gain
    int mFrequencyGood=0;
    double AvgFreq=0;
    double AvgMag=0;
    double AvgMagTime=0;
    
    for(int i=0;i<APL::Channel::NumChannels;i++)
    {
        //close enough
        if((fabs(mFrequency-Freq[i])<2000.0) && Max[i]>20)
        {
            mFrequencyGood++;
            AvgFreq+=Freq[i];
            AvgMag+=Max[i];
            AvgMagTime+=MaxTime[i];
        }
    }
    //printf("Num Freq Good: %d\n",mFrequencyGood);
    if(mFrequencyGood>=3)
    {
        AvgFreq/=mFrequencyGood;
        AvgMag/=mFrequencyGood;
        AvgMagTime/=mFrequencyGood;
        //double Error=(500.0-AvgMag);
        double Error=(3.0-AvgMagTime);
        double dError=Error-mOldError;
        
        //printf("Mag Error: %lf %lf\n",Error,dError);
        //mGain+=Error/500.0;//+dError/2000.0;
        mGain+=Error/2.0;//+dError/2000.0;

        mOldError=Error;

        printf("Avg: %lf %lf %lf Gain: %lf\n",AvgFreq,AvgMag,AvgMagTime,mGain);

        mLastTime=CxUtils::GetTimeSeconds();
        
        UpdateGain();

    }

    
    if(mCallback!=NULL)
    {
        mCallback->SetChannel(Real,Imag,Freq,mGain);
    }
}

void Interface::UpdateGain()
{
    /*if(mGain>122)
    {
        mGain=122;
    }*/
    //max for now
    if(mGain>mMax)
    {
        mGain=mMax;
    }
    
    if(mGain<mMin)
    {
        mGain=mMin;
    }
    MCU::SetAPL setGainValue;
    setGainValue.mField = MCU::SetAPL::VariableGain;
    setGainValue.mValue = floor(mGain);
    mMCU->Send(&setGainValue);
}