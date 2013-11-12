/////////////////////////////////////////////////////////////////////////////////////
///
/// \file LCD.h
/// \brief Hardware interface for the VFD-420 lcd screen
///         (http://seetron.com/vfdmnl/mnl.htm)
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
#include <lcd/lcd.h>

#ifndef __ZEBULON_HARDWARE_VFD420_H_
#define __ZEBULON_HARDWARE_VFD420_H_

namespace Zebulon
{
    namespace Hardware
    {
        class VFD420 : public Zebulon::Hardware::LCD
        {
        public:
            bool Initialize(const std::string portname, unsigned int baudrate);
            
            void ClearScreen();
            
            void SetText(const std::string text);
            
            void SetContrast(double contrastPercentage);
            void BackLightOn(char duration);
            void BackLightOff();
            
            void ResetCursor();
            void SetCursorPosition(char row, char column);
            void WriteAtPosition(char row, char column, const std::string text);
            
            void CursorOff();
            void CursorOn();
        };
    }
}
#endif