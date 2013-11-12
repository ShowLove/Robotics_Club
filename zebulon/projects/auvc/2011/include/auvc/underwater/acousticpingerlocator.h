/////////////////////////////////////////////////////////////////////////////
///
/// \file acousticpingerlocator.h
/// \brief Interface for interacting with the acoustic pinger locator via
///         the FPGA
///
/// Author(s): Michael Scherer<br>
/// Created: 04/08/2010<br>
/// Copyright (c) 2008<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
/// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
/// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
/// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
/// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
/// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
/// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
/// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
/// OF SUCH DAMAGE.
///
/////////////////////////////////////////////////////////////////////////////

#ifndef __ZEBULON_UNDERWATER_ACOUSTICPINGERLOCATOR__H
#define __ZEBULON_UNDERWATER_ACOUSTICPINGERLOCATOR__H

#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <cxutils/mutex.h>

#include <apl/interface.h>
#include <mcu/interface.h>

#include "auvc/underwater/globalinfo.h"
#include <statemachine/utility/utility.h>

#include <xmlconfig/xmlconfigurator.h>

#include <grapher/grapher.h>
#include <interpolate/interpolate.h>

#include <filter/filter.h>
#include <filter/rcfilter.h>
#include <vector>

namespace Zebulon
{
    namespace Underwater
    {
        class AcousticPingerLocator : public MCU::Interface::Callback, public APL::Callback
        {
        public:
            AcousticPingerLocator();
            virtual ~AcousticPingerLocator();
            
            virtual void Initialize(GlobalInfo* globalinfo);

            //Takes in messages from the MCU and sets the appropriate member variable (digita/analog)
            virtual void ProcessMessage(const MCU::Message* message);
            
            virtual int LockYaw();
            virtual int SetChannel(double *Real,double *Imag,double *Freq,double Gain);
            
        private:
            static void Thread(void *args);
            void ProcessingThread();
            
            CxUtils::Thread mThread;
            CxUtils::Mutex mMutex;
            
            MCU::Interface mMCU;
            APL::Interface mAPL;
            
            GlobalInfo* mGlobalInfo;

            //Constants for Processing
            double mFreqThresh;
            double mMinMagnitude;
            
            //Internal data stucture for storage
            //static const int NumChannels=5;
            //hack till other channels work
            static const int NumChannels=5;
            static const double NumSamples=511;
            static const double SpeedOfSound=1497;
            //Radius of hydrophone mount
            static const double HydroRadius;
            struct Channel
            {
                double mReal;
                double mImag;
                double mMag;
                double mPhase;
                int mCount[2];
            };
            Channel mChannels[NumChannels];
            int mCurrCount;
            int mCurrFreqIndex;
            double mCurrFreq;
            double mSampleFreq;
            
            //Difference information for math
            struct DiffChannel
            {
                double mPhaseDiff;
                double mRawDistDiff;
                double mDistDiff;   //fixed for max value
                double mAngle;
            };
            DiffChannel mDiffChannels[NumChannels-1];
            
            bool mFinishedRead;
            
            void CalcChannel();
            void CalcDiff();
            void CalcPosition();
            
            double mCurrYaw;
            double mYaw;
            double mDist;
            double mDepth;
            bool mDisplay;
            double mPitch;
            double mGain;
            Utility::Grapher graph;
            Utility::Interpolate *inter;
            
            CxUtils::Mutex mMathMutex;
            
            void LoadXML(int Pinger=0);
            int mPinger;
            double mStartTime;
            Utility::Filter *mYawFilter;
            Utility::LowPassFilter *mYawLPFilter;

            Utility::Filter *mDistFilter;
            Utility::LowPassFilter *mDistLPFilter;
            double mGlobalYaw;
            bool mYawFlag;
            
            int mPingVote[2];
            int mPingCount;
            int mPingCountGood;
            int mPingerSelected;
            
            // XML Configuration
            
            Configuration::XmlConfigurator mXmlConfig;  ///<  XML config data.
            std::string mAPLPort;
            std::string mAPLRawPort;
            
            //Constants for setting the pinger up
            struct PingerSettings
            {
                double mFrequency;
                double mQ;
                int mDCDecay;
                int mPktoPkDecay;
                int mSampleMode;
                int mPingThreshold;
                int mPingCount;
                int mArbiterMode;
                double mMin;
                double mMax;
            };
            
            int mAuto;
            PingerSettings PingerSet[2];

            //Monte Carlo Method
            class MC
            {
                public:
                    MC() {};
                    ~MC() {};
                    
                    //X Forward, Y right
                    struct HydroPhone
                    {
                        double x;
                        double y;
                        double z;
                        
                        double dist;
                        double phase;
                    };
                    struct HydroPhone HydroPhones[NumChannels];

                    struct DiffPhone
                    {
                        double PhaseDiff;
                        double RawDistDiff;
                        double DistDiff;   //fixed for max value
                        double Angle;
                        
                        double IAngle;
                    };
                    struct DiffPhone DiffPhones[NumChannels-1];
                    struct DiffPhone MeasPhones[NumChannels-1];
                    
                    void PlaceHydro();
                    void CalcDiff();
                    void CalcPhase(double x,double y,double z);
                    
                    double WaveLength;
                    double FindPinger();
            };
            MC Monte;
        };
    }
}

#endif // __ZEBULON_UNDERWATER_ACOUSTICPINGERLOCATOR__H
/* End of file */
