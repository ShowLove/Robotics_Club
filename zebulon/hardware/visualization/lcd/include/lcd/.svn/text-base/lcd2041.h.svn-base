#include <cxutils/serial.h>
#include <lcd/lcd.h>

#ifndef __ZEBULON_HARDWARE_LCD2041_H_
#define __ZEBULON_HARDWARE_LCD2041_H_

namespace Zebulon
{
    namespace Hardware
    {
        class LCD2041 : public Zebulon::Hardware::LCD
        {
            public:
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