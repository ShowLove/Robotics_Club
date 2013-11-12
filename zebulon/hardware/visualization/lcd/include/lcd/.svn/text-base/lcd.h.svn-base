/////////////////////////////////////////////////////////////////////////////////////
///
/// \file LCD.h
/// \brief Generic hardware interface for an lcd screen
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

#include <cxutils/serial.h>
#include <cxutils/mutex.h>
#include <stdio.h>
#include <string.h>
//using namespace CxUtils;

#ifndef __ZEBULON_HARDWARE_LCD_H_
#define __ZEBULON_HARDWARE_LCD_H_

namespace Zebulon
{
    namespace Hardware
    {
        class LCD
        {
        public:
            LCD();
            ~LCD();
            
            virtual bool IsInitialized();
            virtual bool Initialize(const std::string portname, unsigned int baudrate);
            virtual bool Destroy() { };
            
            virtual bool Connect();
            virtual bool Disconnect();
            virtual bool IsConnected();
            
            int GetNumRows();
            int GetNumCols();
            
            void SetPort(std::string port);
            void GetPort(std::string &port);
            
            void SetBaudRate(unsigned int baudrate);
            void GetBaudRate(unsigned int &baudrate);
            
            void GetText(std::string &text);
            virtual void SetText(const std::string text);
            
            virtual void ClearScreen() = 0;
            
            virtual void SetContrast(double contrastPercentage) = 0;
            virtual void BackLightOn(char duration) = 0;
            virtual void BackLightOff() = 0;
            
            virtual void ResetCursor() = 0;
            virtual void SetCursorPosition(char row, char column) = 0;
            virtual void WriteAtPosition(char row, char column, const std::string text) = 0;
            
            virtual void CursorOff() = 0;
            virtual void CursorOn() = 0;
            
        protected:
            int mRows, mCols;
            std::string mText;
            
            std::string mPort;
            unsigned int mBaud;
            CxUtils::Serial* mSerialPort;
            
            CxUtils::Mutex mSerialConMutex;
            bool mInitialized;
        };
    }
}

#endif
/* End of file */
