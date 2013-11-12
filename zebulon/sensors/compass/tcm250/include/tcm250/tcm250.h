/////////////////////////////////////////////////////////////////////////////////////
///
/// \file tcm250.h
/// \brief A simple Compas class capable of receiving data from TCM 250
/// using a serial connection
///
/// Author(s): David Adams<br>
/// Created: 10/16/2008<br>
/// Last Modified: 10/16/2008<br>
/// Copyright (c) 2008<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu <br>
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
/////////////////////////////////////////////////////////////////////////////////////
#ifndef ZEBULON_TCM250_COMPASS_H
#define ZEBULON_TCM250_COMPASS_H

#include <stdio.h>
#include <string.h>
#include <vector>
#include <set>
#include <map>
#include <cxutils/serial.h>
#include <cxutils/thread.h>

#include <compass/compass.h>

//////////////////////////////////////////////////////////////////////////////////// 
///  
///  \def ZEB_COMPASS_DLL
///  \brief Pre-processor definition used for exporting/importing DLL for library.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef ZEB_TCM250_DLL
    #ifdef WIN32
        #define ZEB_TCM250_DLL __declspec(dllimport)
    #else
        #define ZEB_TCM250_DLL
    #endif
#endif

namespace Zebulon
{
    namespace Compass
    {
        namespace TCM
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Overload this callback and functions to recieve events from tcm
            ///
            ////////////////////////////////////////////////////////////////////////////////////
           class Callback : public Compass::Callback
            {
            public:
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///   \brief Event called when new magentometer data is available.
                ///
                ///   \params bx Updated x value
                ///           by Updated y value
                ///           bz Updated z value
                ///             
                ////////////////////////////////////////////////////////////////////////////////////
                virtual void ProcessMagnetometerData(const double bx, const double by, const double bz) =0 ;
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///   \brief Event called when new temperature is available.
                ///   
                ///   \params temp Updated temperature value.
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                virtual void ProcessTemperatureData(const double temp) = 0;
                static const unsigned char MagnetometerType = 0xA0;
                static const unsigned char TemperatureType = 0xA1;
            };
            
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class TCM250
            ///   \brief Interface class for the TCM2-50 Digital Compass System.
            ///
            ///   This class contains functions for connecting to, configuring, and reading
            ///   and parsing information from the TCM2-50 Digital Compass.
            ///
            ///   This class is part of the zebsensors library.  This library
            ///   contains interfaces for various sensors used on robots. Such
            ///   sensors include TCM250, DGPS, SICK Laser, etc.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_TCM250_DLL TCM250 : public GenericCompass
            {
            public:
                TCM250();
                virtual ~TCM250();
                int Halt();
                int StartContinuousOutput();
                int SetTrueNorth();
                int SetMagneticNorth();
                int SetClockRate(const unsigned int freq = 40);
                int SetTimeConstant(const unsigned int t = 8);
                int SetDeviceBaudRate(const unsigned int r = 5);
                int SetOutputWord(const bool tcm = true);
                int SetQuerySkip(const unsigned int i = 0);
                int SetSamplingPeriodDivisor(const unsigned int i = 1);
                int EnableFastSampling(const bool enable = true);
                int EnableCompassData(const bool enable = true);
                int EnableMagnetometerData(const bool enable = true);
                int EnableMagneticDistortionAlarm(const bool enable = true);
                int EnablePitchData(const bool enable = true);
                int EnableRollData(const bool enable = true);
                int EnableTemperatureData(const bool enable = true);
                int EnableDigitalDamping(const bool enable = true);
                int SendReboot();
                unsigned int ExtractMessage(const unsigned char * buffer, const unsigned int len);
                void  GetMagnetometerData(double &bx, double &by, double &bz);
                double GetYaw() const;
                double GetPitch() const;
                double GetRoll() const;
                double GetTemp() const;
                double GetBx() const;
                double GetBy() const;
                double GetBz() const;
                int  GetErrorCode()const;
                bool RunCallback(const unsigned char type);
            protected:   
                virtual void ProcessingFunction();     ///<  Thread used to receive data from sensor.
                virtual bool SetContinuousMode();
                unsigned int IsValidData(const unsigned char * buff, const unsigned int size) const;
                bool mStandbyFlag;                          ///<  Standbye mode
                double mTemp;                               ///<  Temperature
                double mBx;                                 ///<  Magnetic sensors X value
                double mBy;                                 ///<  Magnetic sensors Y value
                double mBz;                                 ///<  Magnetic sensors Z value
                std::vector<CxUtils::Packet*> mPackets;     ///< Packet list

            };
        }
    }
}
#endif