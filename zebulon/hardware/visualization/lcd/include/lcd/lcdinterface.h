/////////////////////////////////////////////////////////////////////////////////////
///
/// \file lcdinterface.h
/// \brief Generic software interface for an lcd screen that updates periodically
///
/// Author(s): Michael Scherer<br>
/// Created: 5/28/2010<br>
/// Last Modified: 5/28/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org <br>
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

#include <lcd/lcd.h>
#include <cxutils/thread.h>
#include <cxutils/mutex.h>

#ifndef __ZEBULON_HARDWARE_LCDINTERFACE_H_
#define __ZEBULON_HARDWARE_LCDINTERFACE_H_

namespace Zebulon
{
    namespace Hardware
    {
        class LCDInterface
        {
        public:
            virtual bool Initialize(LCD* screen, double period);
            
        protected:
            static void SerialThreadFunction(void *args);
            virtual void ProcessingFunction() = 0;
            CxUtils::Thread mSerialThread;              ///< Thread for recieving data
            CxUtils::Mutex mMutex;
            
            bool mShutdown;
            
            LCD* mScreen;
            double mPeriod;
        };
    }
}

#endif
/* End of file */