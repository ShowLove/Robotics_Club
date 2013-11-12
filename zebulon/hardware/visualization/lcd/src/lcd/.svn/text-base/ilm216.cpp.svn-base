/*
ASCII Value 	Control Code 	ASCII Name 	Function
0 	ctrl-@ 	NUL 	Null; ignored prior to buffer
1 	ctrl-A 	SOH 	Send cursor home (position 0, upper left corner)
2 	ctrl-B 	STX 	Begin big characters (-420 only)
3 	ctrl-C 	ETX 	End big chars (-420 only) or end right-align (both)
4 	ctrl-D 	ETO 	Turn off cursor
5 	ctrl-E 	ENQ 	Ignored
6 	ctrl-F 	ACK 	Turn on block cursor
7 	ctrl-G 	BEL 	Beep buzzer on J1, pins 7&8
8 	ctrl-H 	BS 	Backspace
9 	ctrl-I 	HTAB 	Tab to next multiple-of-4 column
10 	ctrl-J 	LF 	Smart linefeed; move down one row
11 	ctrl-K 	VTAB 	Vertical tab; move up one row
12 	ctrl-L 	FF 	Formfeed; clear the screen
13 	ctrl-M 	CR 	Carriage return; move to beginning of next line
14 	ctrl-N 	SO 	Turn on after power-down
15 	ctrl-O 	SI 	Power-down the VFD
16 	ctrl-P 	DLE 	Accept cursor-position data
17 	ctrl-Q 	DC1 	Ignored
18 	ctrl-R 	DC2 	Accept right-alignment data
19-26 	- 	- 	All ignored
27 	ctrl-[ 	ESC 	Escape; begin special instruction
*/

#include <lcd/ilm216.h>

using namespace Zebulon;
using namespace Hardware;

bool ILM216::Initialize(const std::string portname, unsigned int baudrate)
{
    mRows = 2;
    mCols = 20;
    LCD::Initialize(portname, baudrate);
}

void ILM216::ClearScreen()
{
    mText = "";
    char data[] = { 0x0C };
    mSerialPort->Send(data, 1);
}

void ILM216::SetText(const std::string text)
{
    ClearScreen();
    mText = text;
    mSerialPort->Send(text.c_str(), (text.size() < 80)? text.size() : 80);
}

void ILM216::SetContrast(double contrastPercentage)
{
    char data[2] = {27, '0'};
    if (contrastPercentage <= 25)
    {
        data[1] = '0';
    }
    else if (contrastPercentage <= 50)
    {
        data[1] = '1';
    }
    else if (contrastPercentage <= 75)
    {
        data[1] = '2';
    }
    else
    {
        data[1] = '3';
    }
    
    mSerialPort->Send(data, 2);
}

void ILM216::BackLightOn(char duration)
{
    char data[] = { 14 };
    mSerialPort->Send(data, 1);
}

void ILM216::BackLightOff()
{
    char data[] = { 15 };
    mSerialPort->Send(data, 1);
}

void ILM216::ResetCursor()
{
    char data[] = { 0x01 };
    mSerialPort->Send(data, 1);
}

void ILM216::SetCursorPosition(char row, char column)
{
    row--;
    column--;
    
    if (row*20+column > 32)
        return;
    
    char data[] = { 16, 64 + row*20 + column};
    mSerialPort->Send(data, 2);
}

void ILM216::WriteAtPosition(char row, char column, const std::string text)
{
    int tlen = text.size();
    int mlen = mText.size();
    row = row-1;
    column = column-1;
    
    if (row*20+column > 32)
        return;
    
    char str[33]; str[32] = '\0';
    for (int i=0; i<32; i++)
    {
        if (i >= row*20+column && i-row*20-column < tlen)
        {
            str[i] = text.c_str()[i-row*20-column];
        }
        else if (i<mlen)
        {
            str[i] = mText.c_str()[i];
        }
        else
        {
            str[i] = ' ';
        }
    }
    
    ILM216::SetText(str);
}

void ILM216::CursorOff()
{
    char data[] = { 0x04 };
    mSerialPort->Send(data, 1);
}

void ILM216::CursorOn()
{
    char data[] = { 0x06 };
    mSerialPort->Send(data, 1);
}