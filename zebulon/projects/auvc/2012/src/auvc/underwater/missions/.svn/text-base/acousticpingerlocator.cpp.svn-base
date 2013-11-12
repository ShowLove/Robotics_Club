//#include <auvc/underwater/acousticpingerlocator.h>

//using namespace Zebulon;
//using namespace Underwater;

//const double AcousticPingerLocator::HydroRadius=(1.5/100.0);

////#define APL_VERBOSE

//AcousticPingerLocator::AcousticPingerLocator()
//{
//    mSampleFreq=694444;
//    graph.Setup("Pinger",8,20);
//    graph.Range(0,-M_PI,M_PI);
//    graph.Range(1,-M_PI,M_PI);
//    graph.Range(2,-M_PI,M_PI);
//    graph.Range(3,-M_PI,M_PI);
//    graph.Range(4,-180,180);
//    graph.Range(5,-180,180);
//    graph.Range(6,0,15);
//    graph.Range(7,0,15);
    
//    inter=new Utility::CaseBasedInter(NumChannels-1,8,5);
//    //inter=new Utility::LinearRegInter(4,8);
//    inter->Setup("../doc/hydrosimp.log");
//    mCurrYaw=0.0;
//    mYawFilter=new Utility::MedianFilter(3);
//    mYawLPFilter=new Utility::LowPassFilter(.25,1);
//    mYawFilter->Setup(-180,180);
//    mYawLPFilter->Setup(-180,180);
//    mYaw=0;
//    mDist=0;
//    mPinger=0;
    
//    mDistFilter=new Utility::MedianFilter(3);
//    mDistLPFilter=new Utility::LowPassFilter(.25,1);
    
//    mYawFlag=false;
//    mXmlConfig.Load("settings/apl.xml", "APL");
//    LoadXML();
//    Monte.PlaceHydro();

//    mPingCount=0;
//    mPingerSelected=-1;
//    mPingVote[0]=0;
//    mPingVote[1]=0;
//}

//AcousticPingerLocator::~AcousticPingerLocator()
//{
//    delete inter;
//    delete mYawFilter;
//}

//void AcousticPingerLocator::Initialize(GlobalInfo* globalinfo)
//{
//    mGlobalInfo = globalinfo;
//    mGlobalInfo->SetInfo(GlobalInfo::PingerSelected, 0);
    
//    printf("MCU: %s\n",mAPLPort.c_str());
    
//    if (!mMCU.Initialize(mAPLPort, 115200, false, CxUtils::CX_SERIAL_ODD_PARITY))
//    {
//        printf("AcousticPingerLocator::Error: Failure initializing MCU for APL");
//        return;
//    }
    
//    //display is true/false
//    if (!mAPL.Initialize(&mMCU, mAPLRawPort, mDisplay, 115200))
//    {
//        printf("AcousticPingerLocator::Error: Failure initializing APL");
//        return;
//    }
    
//    mCurrCount=0;
//    mFinishedRead=0;
//    for(int i=0;i<NumChannels;i++)
//    {
//        mChannels[i].mCount[0]=0;
//        mChannels[i].mCount[1]=0;
//    }
    
//    mMCU.RegisterCallback(MCU::Message::All,this);
//    mAPL.SetCallback(this);

//    //Time since ping, originally 0.0
//    mGlobalInfo->SetInfo(GlobalInfo::PingerUpdateTime, 0.0);
//    LoadXML();
//    if(mThread.CreateThread(&Thread,this) == false)
//    {
//        // Error
//    }
//}

//void AcousticPingerLocator::Thread(void *args)
//{
//    AcousticPingerLocator *_apl=(AcousticPingerLocator *)args;
//    _apl->ProcessingThread();
//}

//void AcousticPingerLocator::ProcessingThread()
//{
//    mStartTime=CxUtils::GetTimeSeconds();
//    double currTime;

//    mGlobalInfo->SetInfo(GlobalInfo::PingerUpdateTime, CxUtils::GetTimeSeconds());
//    mGlobalInfo->SetInfo(GlobalInfo::PingerYaw,0.0);
//    mGlobalInfo->SetInfo(GlobalInfo::PingerDistance,10.0);
//    mGlobalInfo->SetInfo(GlobalInfo::PingerGain,122.0);

//    //Do the first one
//    if(mAuto==0)
//    {
//        mAPL.SetPinger(PingerSet[0].mFrequency,PingerSet[0].mSampleMode,PingerSet[0].mPingThreshold,PingerSet[0].mPingCount,PingerSet[0].mQ,PingerSet[0].mDCDecay,PingerSet[0].mPktoPkDecay, PingerSet[0].mArbiterMode,PingerSet[0].mMin,PingerSet[0].mMax);
//        //if not auto, always selected
//        mPingerSelected=0;
//    }
//    //Auto find for awhile
//    else
//    {
//        //average freq (good as any)?
//        //Arbitor forced to 1 to run auto gains
//        mAPL.SetPinger((PingerSet[0].mFrequency+PingerSet[1].mFrequency)/2,PingerSet[0].mSampleMode,PingerSet[0].mPingThreshold,PingerSet[0].mPingCount,PingerSet[0].mQ,PingerSet[0].mDCDecay,PingerSet[0].mPktoPkDecay, 1,PingerSet[0].mMin,PingerSet[0].mMax);
//    }
//    while(mThread.QuitThreadFlag() == false)
//    {

//        if(mFinishedRead)
//        {
//            mFinishedRead=false;
//            //mAPL.Print();
//            //do math stuff here!
//        }
//        currTime=CxUtils::GetTimeSeconds();
//        if(currTime-mStartTime>=0.5)
//        {
//            mAPL.RequestDebugData();
//            mStartTime=currTime;
//            int newpinger;
//            if(mGlobalInfo->GetInfo(GlobalInfo::PingerSelected,newpinger)==true)
//            {
//                if(newpinger!=mPinger)
//                {
//                    mPinger=newpinger;
//                    //Switch which pinger
//                    if(mPinger==0 && mPingerSelected==0)
//                    {
//                        mAPL.SetPinger(PingerSet[0].mFrequency,PingerSet[0].mSampleMode,PingerSet[0].mPingThreshold,PingerSet[0].mPingCount,PingerSet[0].mQ,PingerSet[0].mDCDecay,PingerSet[0].mPktoPkDecay, PingerSet[0].mArbiterMode,PingerSet[0].mMin,PingerSet[0].mMax);
//                    }
//                    if(mPinger==0 && mPingerSelected==1)
//                    {
//                        mAPL.SetPinger(PingerSet[1].mFrequency,PingerSet[1].mSampleMode,PingerSet[1].mPingThreshold,PingerSet[1].mPingCount,PingerSet[1].mQ,PingerSet[1].mDCDecay,PingerSet[1].mPktoPkDecay, PingerSet[1].mArbiterMode,PingerSet[1].mMin,PingerSet[1].mMax);
//                    }

//                    if(mPinger==1 && mPingerSelected==0)
//                    {
//                        mAPL.SetPinger(PingerSet[1].mFrequency,PingerSet[1].mSampleMode,PingerSet[1].mPingThreshold,PingerSet[1].mPingCount,PingerSet[1].mQ,PingerSet[1].mDCDecay,PingerSet[1].mPktoPkDecay, PingerSet[1].mArbiterMode,PingerSet[1].mMin,PingerSet[1].mMax);
//                    }
//                    if(mPinger==1 && mPingerSelected==1)
//                    {
//                        mAPL.SetPinger(PingerSet[0].mFrequency,PingerSet[0].mSampleMode,PingerSet[0].mPingThreshold,PingerSet[0].mPingCount,PingerSet[0].mQ,PingerSet[0].mDCDecay,PingerSet[0].mPktoPkDecay, PingerSet[0].mArbiterMode,PingerSet[0].mMin,PingerSet[0].mMax);
//                    }
//                }
//            }
//        }
        
//        CxUtils::SleepMs(10);
//    }
//}

//void AcousticPingerLocator::ProcessMessage(const MCU::Message* message)
//{
//    //message->Print();
//    if(message->mMessageCode == MCU::CustomMessage::REPORT_SAMPLE_FREQ)
//    {
//        MCU::ReportSampleFreq *freqmsg=(MCU::ReportSampleFreq *)message;
//        mSampleFreq=freqmsg->mSampleFrequency;
//    }
//}


//void AcousticPingerLocator::CalcChannel()
//{
//    for(int i=0;i<NumChannels;i++)
//    {
//        mChannels[i].mMag=sqrt(mChannels[i].mReal*mChannels[i].mReal+mChannels[i].mImag*mChannels[i].mImag);
//        mChannels[i].mPhase=atan2(mChannels[i].mImag,mChannels[i].mReal);
//    }
//}
//void AcousticPingerLocator::CalcDiff()
//{
//    //Should base it relative to the center one
//    for(int i=0;i<NumChannels-1;i++)
//    {
//        mDiffChannels[i].mPhaseDiff=mChannels[i].mPhase-mChannels[NumChannels-1].mPhase;
//        //force -pi to pi
//        while(mDiffChannels[i].mPhaseDiff <= -M_PI)
//        {
//            mDiffChannels[i].mPhaseDiff += 2*M_PI;
//        }
//        while(mDiffChannels[i].mPhaseDiff > M_PI)
//        {
//            mDiffChannels[i].mPhaseDiff -= 2*M_PI;
//        }
//        //Find the distance away based on wavelength
//        //speed=wavelength*freq
//        mDiffChannels[i].mRawDistDiff = SpeedOfSound/mCurrFreq*mDiffChannels[i].mPhaseDiff/(2*M_PI);
//        mDiffChannels[i].mDistDiff = mDiffChannels[i].mRawDistDiff;
//        //fudge to fix for math
//        if(mDiffChannels[i].mDistDiff>HydroRadius)
//        {
//            mDiffChannels[i].mDistDiff=HydroRadius;
//        }
//        if(mDiffChannels[i].mDistDiff<-HydroRadius)
//        {
//            mDiffChannels[i].mDistDiff=-HydroRadius;
//        }
//        mDiffChannels[i].mAngle=asin(mDiffChannels[i].mDistDiff/HydroRadius);
//    }
//}
//void AcousticPingerLocator::CalcPosition()
//{
//    //Do Magic!
//    for(int i=0;i<NumChannels;i++)
//    {
//    #ifdef APL_VERBOSE
//        /*std::cout <<  "Channel " << i
//                    << " Real " << mChannels[i].mReal
//                    << " Imag " << mChannels[i].mImag
//                    << " Mag " << mChannels[i].mMag
//                    << " Phase " << mChannels[i].mPhase
//                    << " Count " << mChannels[i].mCount[0] << " " << mChannels[i].mCount[1]
//                    << std::endl;*/
//    #endif
//    }
//#define LOOKUP
//#ifdef LOOKUP
//    //ask interpolator
//    std::vector<double> inputData;
//    for(int i=0;i<NumChannels-1;i++)
//    {
//    #ifdef APL_VERBOSE
//        std::cout <<  "ChannelDiff " << i
//            << " PhaseDiff " << mDiffChannels[i].mPhaseDiff
//            << " DistDiff " << mDiffChannels[i].mDistDiff
//            << " RawDistDiff " << mDiffChannels[i].mRawDistDiff
//            << " mAngle " << mDiffChannels[i].mAngle
//            << std::endl;
//    #endif
//        if(i==2)
//        {
//            mDiffChannels[i].mDistDiff=0;
//            mDiffChannels[i].mAngle=0;
//        }
//        if(mDisplay)
//        {
//            if((fabs(mCurrFreq-PingerSet[0].mFrequency)<mFreqThresh || fabs(mCurrFreq-PingerSet[1].mFrequency)<mFreqThresh))
//            {
//                graph.AddValue(i,mDiffChannels[i].mAngle);
//            }
//        }
//        //inputData.push_back(mDiffChannels[i].mAngle);
//        inputData.push_back(mDiffChannels[i].mDistDiff);
//    }
    
//    std::vector<double> outputData;
//    outputData=inter->GetOutput(inputData);
//#ifdef APL_VERBOSE
//    std::cout <<  "Output "
//        << " X " << outputData[0]
//        << " Y " << outputData[1]
//        << " Z " << outputData[2]
//        << " Angle " << outputData[3]
//        << std::endl;
//#endif
    
//    //Count that a ping happened
//    mPingCount++;

//    //if either ping happened
//    if((fabs(mCurrFreq-PingerSet[0].mFrequency)<mFreqThresh || fabs(mCurrFreq-PingerSet[1].mFrequency)<mFreqThresh))
//    {
//        //check which
//        if(fabs(mCurrFreq-PingerSet[0].mFrequency)<fabs(mCurrFreq-PingerSet[1].mFrequency))
//        {
//            mPingVote[0]++;
//            if(mPingVote[0]>=mPingCountGood && mAuto==1)
//            {
//                mPingerSelected=0;
//                mAPL.SetPinger(PingerSet[0].mFrequency,PingerSet[0].mSampleMode,PingerSet[0].mPingThreshold,PingerSet[0].mPingCount,PingerSet[0].mQ,PingerSet[0].mDCDecay,PingerSet[0].mPktoPkDecay, PingerSet[0].mArbiterMode,PingerSet[0].mMin,PingerSet[0].mMax);
//                mAuto=0;
//            }
//        }
//        else
//        {
//            mPingVote[1]++;
//            if(mPingVote[1]>=mPingCountGood && mAuto==1)
//            {
//                mPingerSelected=1;
//                mAPL.SetPinger(PingerSet[1].mFrequency,PingerSet[1].mSampleMode,PingerSet[1].mPingThreshold,PingerSet[1].mPingCount,PingerSet[1].mQ,PingerSet[1].mDCDecay,PingerSet[1].mPktoPkDecay, PingerSet[1].mArbiterMode,PingerSet[1].mMin,PingerSet[1].mMax);
//                mAuto=0;
//            }
//        }
//        printf("Pinger Count: %d Vote: %d %d\n",mPingCount,mPingVote[0],mPingVote[1]);
        
//        //
        
//        //Change up metrics
//        mDepth=outputData[2];
//        double tmpYaw=CxUtils::CxToDegrees(outputData[3]);
//        //Hack - sometimes 180 out of phase, swap?
//        /*if(mYawFlag==true)
//        {
//            //turned around
//            if(fabs(AI::Utility::AngleDiff(tmpYaw,mYaw))>90)
//            {
//                //facing northish and pinger says southish
//                if(fabs(AI::Utility::AngleDiff(mCurrYaw,0))<90 && fabs(AI::Utility::AngleDiff(tmpYaw,0))>90)
//                {
//                    std::cout << "North Flip " << tmpYaw << " " << mYaw << std::endl;
//                    tmpYaw=AI::Utility::AngleSum(tmpYaw,180);
//                }
//                //opposite
//                else if(fabs(AI::Utility::AngleDiff(mCurrYaw,0))>90 && fabs(AI::Utility::AngleDiff(tmpYaw,0))<90)
//                {
//                    std::cout << "South Flip " << tmpYaw << " " << mYaw << std::endl;
//                    tmpYaw=AI::Utility::AngleSum(tmpYaw,180);
//                }
//            }
//        }*/
//        mYaw=tmpYaw;
//        mYawFlag=true;

//        //find absolute angle
//        double tmpGlobal=CxUtils::Orientation::AddToAngle(mCurrYaw,mYaw,false);

//        double Sin=outputData[4];
//        double Cos=outputData[5];
//        mDist=outputData[6];
//        mPitch=outputData[7];
//#ifdef APL_VERBOSE
//        std::cout << "Direct Angle: " << mYaw
//                    << " Indirect Angle: " << CxUtils::CxToDegrees(atan2(Sin,Cos))
//                    << " XY Angle: " << CxUtils::CxToDegrees(atan2(outputData[1],outputData[0]))
//                    << std::endl;
//        std::cout << "Extra: Dist: " << mDist << " Depth: " << mDepth << " Pitch: " << mPitch << std::endl;
//#endif
//    }
//#endif

////#define MCMETHOD
//#ifdef MCMETHOD
//    if(fabs(mCurrFreq-mFrequency)<mFreqThresh)
//    {
//        //mYaw=MC();
//        Monte.WaveLength=SpeedOfSound/mCurrFreq;
//        //get phase directly
//        for(int i=0;i<NumChannels;i++)
//        {
//            Monte.HydroPhones[i].phase=mChannels[i].mPhase;
//        }
//        //process by same method as MC
//        Monte.CalcDiff();
//        //output of hydrodiff, make copy
//        for(int i=0;i<NumChannels-1;i++)
//        {
//            Monte.MeasPhones[i]=Monte.DiffPhones[i];
//            if(mDisplay)
//            {
//                if(fabs(mCurrFreq-mFrequency)<mFreqThresh)
//                {
//                    graph.AddValue(i,Monte.MeasPhones[i].Angle);
//                }
//            }
//            //printf("%lf\n",Monte.MeasPhones[i].Angle);
//        }
//        //perform MC looking for a match
//        mYaw=Monte.FindPinger()/M_PI*180.0;
//        printf("Yaw: %lf\n",mYaw);
//    }
//#endif

//    //remove singularities
//    if(
//    ((fabs(mCurrFreq-PingerSet[0].mFrequency)<mFreqThresh && mPingerSelected==0)
//        || (fabs(mCurrFreq-PingerSet[1].mFrequency)<mFreqThresh && mPingerSelected==1))
//        && mYaw!=0)
//    {
//        //filter on global yaw?
//        mGlobalYaw=CxUtils::Orientation::AddToAngle(mCurrYaw,mYaw,false);

//        mYawFilter->Add(mGlobalYaw);
//        mYawFilter->Output(mGlobalYaw);
        
//        //low pass to clean up
//        mYawLPFilter->Add(mGlobalYaw);
//        mYawLPFilter->Output(mGlobalYaw);
        
        
//        mDistFilter->Add(mDist);
//        mDistFilter->Output(mDist);
        
//        //low pass to clean up
//        mDistLPFilter->Add(mDist);
//        mDistLPFilter->Output(mDist);
        
//#ifdef APL_VERBOSE
//        static double lastTime=CxUtils::GetTimeSeconds();
//        std::cout << "Time " << (CxUtils::GetTimeSeconds()-lastTime) << " PINGER YAW: " << mYaw << " OPP: " << AI::Utility::AngleSum(mYaw,180) << " Heading : " << mCurrYaw << " Global Yaw: " << mGlobalYaw << std::endl;
//        lastTime=CxUtils::GetTimeSeconds();
//#endif
        
//#ifdef APL_VERBOSE
//        std::cout << "APL: Set Global Yaw " << " " << mGlobalYaw << std::endl;
//#endif

//        std::cout.flush();
//        //Send to global info
//        mGlobalInfo->SetInfo(GlobalInfo::PingerUpdateTime, CxUtils::GetTimeSeconds());
//        mGlobalInfo->SetInfo(GlobalInfo::PingerYaw,mGlobalYaw);
//        mGlobalInfo->SetInfo(GlobalInfo::PingerDistance,mDist);
//        mGlobalInfo->SetInfo(GlobalInfo::PingerGain,mGain);
//        if(mDisplay)
//        {
//            graph.AddValue(4,mYaw);
//            graph.AddValue(5,mGlobalYaw);

//            graph.AddValue(6,mDist);
//            graph.AddValue(7,0.0);
            
//            graph.Graph();
//        }
//    }
//    else
//    {
//        //Out of frequency range, do something else??
//    }
    
//}
//void AcousticPingerLocator::LoadXML(int Pinger)
//{
///*#ifdef APL_VERBOSE
//    std::cout << "LoadXML\n";
//#endif*/
//    //Assume by this point XML has loaded (global)
//    mXmlConfig.GetVar("PingerSet@Frequency",PingerSet[0].mFrequency);
//    mXmlConfig.GetVar("PingerSet@Q",PingerSet[0].mQ);
//    mXmlConfig.GetVar("PingerSet@DCDecay",PingerSet[0].mDCDecay);
//    mXmlConfig.GetVar("PingerSet@PktoPkDecay",PingerSet[0].mPktoPkDecay);
//    mXmlConfig.GetVar("PingerSet@SampleMode",PingerSet[0].mSampleMode);
//    mXmlConfig.GetVar("PingerSet@PingThreshold",PingerSet[0].mPingThreshold);
//    mXmlConfig.GetVar("PingerSet@PingCount",PingerSet[0].mPingCount);
//    mXmlConfig.GetVar("PingerSet@ArbiterMode",PingerSet[0].mArbiterMode);
//    mXmlConfig.GetVar("PingerSet@Min",PingerSet[0].mMin);
//    mXmlConfig.GetVar("PingerSet@Max",PingerSet[0].mMax);

//    mXmlConfig.GetVar("PingerSet1@Frequency",PingerSet[1].mFrequency);
//    mXmlConfig.GetVar("PingerSet1@Q",PingerSet[1].mQ);
//    mXmlConfig.GetVar("PingerSet1@DCDecay",PingerSet[1].mDCDecay);
//    mXmlConfig.GetVar("PingerSet1@PktoPkDecay",PingerSet[1].mPktoPkDecay);
//    mXmlConfig.GetVar("PingerSet1@SampleMode",PingerSet[1].mSampleMode);
//    mXmlConfig.GetVar("PingerSet1@PingThreshold",PingerSet[1].mPingThreshold);
//    mXmlConfig.GetVar("PingerSet1@PingCount",PingerSet[1].mPingCount);
//    mXmlConfig.GetVar("PingerSet1@ArbiterMode",PingerSet[1].mArbiterMode);

//    mXmlConfig.GetVar("PingerProc@FreqThresh",mFreqThresh);
//    mXmlConfig.GetVar("PingerProc@MinMagnitude",mMinMagnitude);
    
//    mXmlConfig.GetVar("PingerPorts@Mcu",mAPLPort);
//    mXmlConfig.GetVar("PingerPorts@Raw",mAPLRawPort);

//    mXmlConfig.GetVar("PingerDisplay@Display",mDisplay);
    
//    mXmlConfig.GetVar("PingerFind@Auto",mAuto);
//    mXmlConfig.GetVar("PingerFind@Good",mPingCountGood);
//}

//int AcousticPingerLocator::LockYaw()
//{
//    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees,mCurrYaw))
//    {
//    #ifdef APL_VERBOSE
//        std::cout << "APL: Heading not stable before ping\n";
//    #endif
//        mCurrYaw=0.0;
//    }
//    else
//    {
//#ifdef APL_VERBOSE
//        std::cout << " APL: Heading Locked to " << mCurrYaw << std::endl;
//#endif
//    }
//}
//int AcousticPingerLocator::SetChannel(double *Real,double *Imag,double *Freq,double Gain)
//{
//    mMathMutex.Lock();
//#ifdef APL_VERBOSE
//    std::cout << "PC FFT Version:\n";
//#endif
//    mCurrFreq=Freq[0];
//    mGain=Gain;
//    for(int i=0;i<NumChannels;i++)
//    {
//        mChannels[i].mReal=Real[i];
//        mChannels[i].mImag=Imag[i];
//    }
//    CalcChannel();
//    CalcDiff();
//    CalcPosition();
//    mMathMutex.Unlock();
//}




////MC class
//void AcousticPingerLocator::MC::PlaceHydro()
//{
//    HydroPhones[0].x=HydroRadius;
//    HydroPhones[0].y=0;
//    HydroPhones[0].z=0;
    
//    HydroPhones[1].x=0;
//    HydroPhones[1].y=HydroRadius;
//    HydroPhones[1].z=0;
    
//    HydroPhones[2].x=-HydroRadius;
//    HydroPhones[2].y=0;
//    HydroPhones[2].z=0;
    
//    HydroPhones[3].x=0;
//    HydroPhones[3].y=-HydroRadius;
//    HydroPhones[3].z=0;
    
//    HydroPhones[4].x=0;
//    HydroPhones[4].y=0;
//    HydroPhones[4].z=0;
//}

//void AcousticPingerLocator::MC::CalcPhase(double x,double y,double z)
//{
//    int i;
//    for(i=0;i<NumChannels;i++)
//    {
//        HydroPhones[i].dist=sqrt(pow(HydroPhones[i].x-x,2)+pow(HydroPhones[i].y-y,2)+pow(HydroPhones[i].z-z,2));
//        HydroPhones[i].phase=fmod(HydroPhones[i].dist,WaveLength)/WaveLength*2*M_PI;
//        if(HydroPhones[i].phase>M_PI)
//        {
//            HydroPhones[i].phase-=2*M_PI;
//        }
        
//    }
//}

//void AcousticPingerLocator::MC::CalcDiff()
//{
//    int i;
//    for(i=0;i<NumChannels-1;i++)
//    {
//        DiffPhones[i].PhaseDiff=HydroPhones[i].phase-HydroPhones[NumChannels-1].phase;
//        //DiffPhones[i].PhaseDiff=HydroPhones[i].phase-HydroPhones[(i+1)%NumChannels].phase;
//        /*if(i<2)
//        {
//            DiffPhones[i].PhaseDiff=HydroPhones[i].phase-HydroPhones[3].phase;
//        }
//        else
//        {
//            DiffPhones[i].PhaseDiff=HydroPhones[i+1].phase-HydroPhones[3].phase;
//        }*/
//        //force -pi to pi
//        while(DiffPhones[i].PhaseDiff <= -M_PI)
//        {
//            DiffPhones[i].PhaseDiff += 2*M_PI;
//        }
//        while(DiffPhones[i].PhaseDiff > M_PI)
//        {
//            DiffPhones[i].PhaseDiff -= 2*M_PI;
//        }
        
//        DiffPhones[i].RawDistDiff = WaveLength*DiffPhones[i].PhaseDiff/(2*M_PI);
//        DiffPhones[i].DistDiff = DiffPhones[i].RawDistDiff;
//        //fudge to fix for math
//        if(DiffPhones[i].DistDiff>HydroRadius)
//        {
//            DiffPhones[i].DistDiff=HydroRadius;
//        }
//        if(DiffPhones[i].DistDiff<-HydroRadius)
//        {
//            DiffPhones[i].DistDiff=-HydroRadius;
//        }
        
        
        
//        DiffPhones[i].Angle=asin(DiffPhones[i].DistDiff/HydroRadius);
        
        
        
//        /*DiffPhones[i].Angle=acos(DiffPhones[i].DistDiff/HydroRadius);
//        if(HydroPhones[NUMHYDRO-1].dist==0.0)
//        {
//            DiffPhones[i].IAngle=0.0;
//        }
//        else
//        {
//            DiffPhones[i].IAngle=M_PI-acos((pow(HydroPhones[NUMHYDRO-1].dist,2)+pow(HydroRadius,2)-pow(HydroPhones[i].dist,2))/(2*HydroPhones[NUMHYDRO-1].dist*HydroRadius));
//        }*/
        
//        //printf("%lf vs %lf = %lf\n",DiffPhones[i].Angle,DiffPhones[i].IAngle,fabs(DiffPhones[i].Angle-DiffPhones[i].IAngle));
//    }
//}

//double AcousticPingerLocator::MC::FindPinger()
//{
//    double ba=0;
//    double bdist=1e6;
//    double x,y,z;
//    double a;
//    int i;
//    double step=0.25;
//    double dist;
//    double vangle;
//    ba=0;
//    bdist=1e6;
//    z=5;
//    //Loop through posistion
//    //for(z=5;z<=5;z+=step)
//    //for(z=0.0;z<=0.0;z+=step)
//    int tryc=10000;
//    while(tryc>0)
//    {
//        tryc--;
        
//        x=(rand()%300-150)/10.0;
//        y=(rand()%300-150)/10.0;
//        //for(y=-15.0;y<=15.0;y+=step)
//        {
//            //for(x=-15.0;x<=15.0;x+=step)
//            {
//                a=atan2(y,x);
//                dist=sqrt(x*x+y*y);
//                vangle=atan2(z,dist);
//                CalcPhase(x,y,z);
//                CalcDiff();
//                double dist=0;
//                //output of hydrodiff
//                for(i=0;i<NumChannels-1;i++)
//                {
//                    //printf("%lf,",DiffPhones[i].PhaseDiff);
//                    //printf("%lf,",DiffPhones[i].DistDiff);
//                    if(i!=2)
//                    {
//                        dist+=sqrt(pow(sin(DiffPhones[i].PhaseDiff)-sin(MeasPhones[i].PhaseDiff),2)+pow(cos(DiffPhones[i].PhaseDiff)-cos(MeasPhones[i].PhaseDiff),2));
//                    }
//                    //dist+=fabs(DiffPhones[i].DistDiff-MeasPhones[i].DistDiff);
//                    //dist+=sqrt(pow(sin(DiffPhones[i].Angle)-sin(MeasPhones[i].Angle),2)+pow(cos(DiffPhones[i].Angle)-cos(MeasPhones[i].Angle),2));
//                    //printf("%lf,",DiffPhones[i].Angle);
//                }
//                //printf("\n");
//                if(dist<bdist)
//                {
//                    bdist=dist;
//                    ba=a;
//                }
//                //placement of pinger
//                //printf("%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",x,y,z,a,sin(a),cos(a),dist,vangle);
//                //printf("\n");
//            }
//        }
//    }
//    printf("BA: %lf %lf\n",bdist,ba);
//    return ba;
    
//    if(bdist>.2 && bdist<0.9 && fabs(fmod(ba,M_PI/2.0))>0.01)
//    {
//        return ba;
//    }
//    else
//    {
//        //bad return
//        return 0.0;
//    }
//}
