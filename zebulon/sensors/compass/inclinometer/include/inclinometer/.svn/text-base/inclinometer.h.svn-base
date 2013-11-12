/////////////////////////////////////////////////////////////////////////////////////
///
/// \file inclinometer.h
/// \brief Simplified "compass" using an inclinometer
///
/// Author(s): Gary Stein<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: gstein@mail.ucf.edu <br>
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
#ifndef ZEBULON_INCLINOMETER_COMPASS_H
#define ZEBULON_INCLINOMETER_COMPASS_H

#include <cstdio>
#include <cstring>
#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <compass/compass.h>

//////////////////////////////////////////////////////////////////////////////////// 
///  
///  \def ZEB_INCLINOMETER_DLL
///  \brief Pre-processor definition used for exporting/importing DLL for library.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef ZEB_INCLINOMETER_DLL
    #ifdef WIN32
        #define ZEB_INCLINOMETER_DLL __declspec(dllimport)
    #else
        #define ZEB_INCLINOMETER_DLL
    #endif
#endif

namespace Zebulon
{
    namespace Compass
    {
        namespace INCLINOMETER
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class Inclinometer
            ///   \brief Interface class for a generic inclinometer
            ///
            ///   This class is part of the zebsensors library.  This library
            ///   contains interfaces for various sensors used on robots. Such
            ///   sensors include TCM250, DGPS, SICK Laser, etc.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_INCLINOMETER_DLL Inclinometer : public GenericCompass
            {
            public:
                Inclinometer();
                virtual ~Inclinometer();
                unsigned int ExtractMessage(const unsigned char * buffer, const unsigned int len);
            protected:   
                virtual void ProcessingFunction();     ///<  Thread used to receive data from sensor.
                virtual bool SetContinuousMode();
            };
        }
    }
}
#endif