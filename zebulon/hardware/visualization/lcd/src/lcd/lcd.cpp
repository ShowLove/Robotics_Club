#include <lcd/lcd.h>

using namespace Zebulon;
using namespace Hardware;

LCD::LCD()
{
    mSerialPort = new CxUtils::Serial();
    mInitialized = false;
}

LCD::~LCD()
{
    delete mSerialPort;
}

bool LCD::Initialize(const std::string portname, unsigned int baudrate)
{
    SetPort(portname);
    SetBaudRate(baudrate);
    
    return mInitialized = true;
}

bool LCD::IsInitialized()
{
    return mInitialized;
}

bool LCD::Connect()
{
    if (mSerialPort->IsConnected() == true)
        return true;
    
    return mSerialPort->Connect(mPort.c_str(), mBaud);
}

bool LCD::Disconnect()
{
    return mSerialPort->Disconnect();
}

bool LCD::IsConnected()
{
    return mSerialPort->IsConnected();
}

int LCD::GetNumRows()
{
    return mRows;
}

int LCD::GetNumCols()
{
    return mCols;
}

void LCD::SetPort(std::string port)
{
    mSerialConMutex.Lock();
    mPort = port;
    mSerialConMutex.Unlock();
}

void LCD::GetPort(std::string &port)
{
    mSerialConMutex.Lock();
    port = mPort;
    mSerialConMutex.Unlock();
}

void LCD::SetBaudRate(unsigned int baudrate)
{
    mSerialConMutex.Lock();
    mBaud = baudrate;
    mSerialConMutex.Unlock();
}

void LCD::GetBaudRate(unsigned int &baudrate)
{
    mSerialConMutex.Lock();
    baudrate = mBaud;
    mSerialConMutex.Unlock();
}

void LCD::SetText(const std::string text)
{
    mSerialConMutex.Lock();
    mText = text;
    mSerialConMutex.Unlock();
}

void LCD::GetText(std::string &text)
{
    mSerialConMutex.Lock();
    text = mText;
    mSerialConMutex.Unlock();
}
