#include <lcd/lcd2041.h>
#include <stdlib.h>

using namespace Zebulon;
using namespace Hardware;

void LCD2041::ClearScreen()
{
    //, 0xFE, 'S' turns on blinking cursor
    
    // 'X' 'H' does clear screen, send cursor home
    char data[] = {0xFE, 'X', 0xFE, 'H'}; 
    mSerialPort->Send(data, 4);
    
}

///
///	Note: This function will truncate any text longer than 80 characters
///
///
void LCD2041::SetText(const std::string text)
{
    char data[] = {0xFE, 'H'};
    mSerialPort->Send(data, 2);
    LCD::SetText(text);
    
    int i;
    int len = text.size();
    char output[81];
    for(i = 0; i < 81; i ++)
    {
        output[i] = ' ';
        
    }
    for (i=0; i<len && i<20; i++)
        output[i] = text.c_str()[i];
    for (i=20; i<len && i<40; i++)
        output[i+20] = text.c_str()[i];
    for (i=40; i<len && i<60; i++)
        output[i-20] = text.c_str()[i];
    for (i=60; i<len && i<80; i++)
        output[i] = text.c_str()[i];
    
    mSerialPort->Send(output, 80);
}

void LCD2041::SetContrast(double contrastPercentage)
{
    char data[] = {0xFE, 'P', (char)((contrastPercentage/100.0)*255.)};
    mSerialPort->Send(data, 3);
}

void LCD2041::BackLightOn(char duration)
{
    char data[] = {0xFE, 'B', duration};
    mSerialPort->Send(data, 3);
}

void LCD2041::BackLightOff()
{
    char data[] = {0xFE, 'F'};
    mSerialPort->Send(data, 2);
}

void LCD2041::ResetCursor()
{
    char data[] = {0xFE, 'H'};
    mSerialPort->Send(data, 2);
}

///
/// Note: (1,1) is the home cursor position, NOT (0,0)
void LCD2041::SetCursorPosition(char row, char column)
{
    if (row <=4 && column <= 20)
    {
        char data[] = {0xFE, 'G', column, row};
        mSerialPort->Send(data, 4);
    }
}

///
/// Note: (1,1) is the home cursor position, NOT (0,0)
void LCD2041::WriteAtPosition(char row, char column, const std::string text)
{
    int outputlen = 80 - 20*(--row) - (--column);
    outputlen = (outputlen < text.size())? outputlen : text.size();
    
    char output[20];
    
    int i;
    for (i=0; i<20; i++)
        output[i] = ' ';
    
    this->SetCursorPosition(row+1, column+1);
    for (i=0; i<20-column && i<outputlen; i++)
        output[i] = text.c_str()[i];
    mSerialPort->Send(output, (outputlen < 20)? outputlen : 20);
    outputlen -= 20-column;
    
    for (i=0; i<20; i++)
        output[i] = ' ';
    
    this->SetCursorPosition((row+1) % 4 + 1, 0x01);
    for (i=0; i<20 && i<outputlen; i++)
        output[i] = text.c_str()[i+20-column];
    mSerialPort->Send(output, (outputlen < 20)? outputlen : 20);
    outputlen -= 20;
    
    for (i=0; i<20; i++)
        output[i] = ' ';
    
    this->SetCursorPosition((row+2) % 4 + 1, 0x01);
    for (i=0; i<20 && i<outputlen; i++)
        output[i] = text.c_str()[i+40-column];
    mSerialPort->Send(output, (outputlen < 20)? outputlen : 20);
    outputlen -= 20;
    
    for (i=0; i<20; i++)
        output[i] = ' ';
    
    this->SetCursorPosition((row+3) % 4 + 1, 0x01);
    for (i=0; i<20 && i<outputlen; i++)
        output[i] = text.c_str()[i+60-column];
    mSerialPort->Send(output, (outputlen < 20)? outputlen : 20);
    outputlen -= 20;
}

void LCD2041::CursorOff()
{
    char data[] = {0xFE, 'T'};
    mSerialPort->Send(data, 2);
}

void LCD2041::CursorOn()
{
    char data[] = {0xFE, 'S'};
    mSerialPort->Send(data, 2);
}