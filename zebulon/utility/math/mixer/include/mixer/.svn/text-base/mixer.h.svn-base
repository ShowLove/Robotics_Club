////////////////////////////////////////////////////////////////////////////////////
///
/// \file mixer.h
/// \brief A mixer class used to convert values of forward and turning to left
/// and right motor thrusts.
///
/// Author(s): Gary Stein, Jonathan Mohlenhoff, Michael Scherer<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF)<br>
/// Email: gstein@mail.ucf.edu, jmohlenh@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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
#ifndef __ZEBULON_UTILITY_MIXER_H
#define __ZEBULON_UTILITY_MIXER_H

#include <math.h>
#include <cxutils/mutex.h>

#ifndef ZEB_MIXER_DLL

#ifdef WIN32
#define ZEB_MIXER_DLL __declspec(dllimport)
#else
#define ZEB_MIXER_DLL
#endif

#endif


namespace Zebulon
{
    namespace Utility
    {
        class ZEB_MIXER_DLL Mixer
        {
        public:
            typedef enum
            {
                Halved = 0,
                Saturated = 1
            } MixingType;
        
            Mixer();
            virtual ~Mixer();
            
            void Setup(MixingType Type=Halved,double Min=-100.0,double Max=100.0);
            
            // A and B is like Forward and Turn
            int SetA(double A);
            int SetB(double B);
            
            // X and Y is like Left and Right
            double GetX();
            double GetY();
            
        protected:
            MixingType mType;
            double mX,mY;
            double mA,mB;
            
            double mMin;
            double mMax;
            
            void Mix();
            
            double Saturate(double Val);
            
            CxUtils::Mutex mutex;
        };
    }
}
#endif  //__ZEBULON_UTILITY_MIXER

/* End of File */
