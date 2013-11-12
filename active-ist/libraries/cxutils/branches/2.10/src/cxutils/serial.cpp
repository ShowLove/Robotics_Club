////////////////////////////////////////////////////////////////////////////////////
///
///  \file serial.cpp
///  \brief Serial (RS232) communication interface.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 4 June 2007
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
#include "cxutils/serial.h"
#include <iostream>
#include <string.h>

#ifdef WIN32
    #include <winsock.h>
    #include <windows.h>
    #ifndef MINGW
        #include <atlbase.h>
    #else
        #define USES_CONVERSION
        #define A2W(x) (x)
        #define W2A(x) (x)
    #endif
#else
    #include <sys/types.h>
    #include <sys/ioctl.h>
    #include <fcntl.h>
    #include <termios.h>
    #include <unistd.h>
#endif

using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Default constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Serial::Serial()
{
    mConnected = false;
    mBaudRate = 9600;
    mDataBits = 8;
    mStopBits = 1;
    mParity = 'N';
    mPortName = "";
    mEnableRTS = mEnableDTR = false;
#ifdef WIN32
    mCom = NULL;
#else
    mCom = 0;
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Destructor.
///
///  Disconnects from any active connection.
///
////////////////////////////////////////////////////////////////////////////////////
Serial::~Serial()
{
    if(mConnected)
        Disconnect();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Connects to a serial port.  This will close any existing connection.
///
///  \param port The port to connect to.  In Windows this would be "com1",
///  "com2", etc.  In Linux it is the path to the port.
///  \param baud The baud rate to connect at
///  \param bits The number of data bits (default = 8). [5,8].
///  \param parity The type of parity (CX_SERIAL_NO_PARITY,
///                                    CX_SERIAL_EVEN_PARITY,
///                                    CX_SERIAL_ODD_PARITY)
///  \param stop The number of stop bits (0, 1, 2).
///
///  \return 1 on success, 0 on failure
///
////////////////////////////////////////////////////////////////////////////////////
int Serial::Connect(const std::string& port,
                    const unsigned int baud,
                    const unsigned int bits,
                    const unsigned int parity,
                    const unsigned int stop)
{
    int result = 0;

    if(mConnected)
        Disconnect();

    if(parity > (unsigned int)(CX_SERIAL_ODD_PARITY) || port.empty())
        return 0;

    mBaudRate = baud;
    mParity = parity;
    mStopBits = stop;
    mDataBits = bits;

#ifdef WIN32

    DCB comDCB;
    mLastError = ERROR_SUCCESS;

    //  Timeouts for file reading
    //  and writing
    COMMTIMEOUTS timeouts;

    mPortName = "\\\\.\\";
    mPortName += port;

    mCom = CreateFileA( mPortName.c_str(),
                        GENERIC_READ  | GENERIC_WRITE,
                        0,
                        0,
                        OPEN_EXISTING,
                        FILE_FLAG_OVERLAPPED,
                        0);

    if(mCom == NULL || mCom == INVALID_HANDLE_VALUE)
    {
        mLastError = GetLastError();
        return result;
    }

    ZeroMemory(&comDCB, sizeof(DCB));
    //  Allocate memory
    comDCB.DCBlength = sizeof(DCB);
    //  Get the current comm state
    GetCommState(mCom, &comDCB);

    comDCB.BaudRate = mBaudRate;
    comDCB.ByteSize = mDataBits;
    comDCB.fBinary = TRUE;
    comDCB.fDsrSensitivity = false;
    comDCB.fAbortOnError = TRUE;
    comDCB.fOutxCtsFlow = FALSE;
    comDCB.fOutxDsrFlow = FALSE;
    comDCB.fOutX = FALSE;
    comDCB.fInX = FALSE;
    comDCB.ByteSize = mDataBits;
    comDCB.fRtsControl = mEnableRTS ? RTS_CONTROL_ENABLE : RTS_CONTROL_DISABLE;
    comDCB.fDtrControl = mEnableDTR ? DTR_CONTROL_ENABLE : DTR_CONTROL_DISABLE;
    
    switch(mStopBits)
    {
    case 1:
        comDCB.StopBits = ONESTOPBIT;
        break;
    case 2:
        comDCB.StopBits = ONE5STOPBITS;
        break;
    case 3:
        comDCB.StopBits = TWOSTOPBITS;
        break;
    default:
        comDCB.StopBits = ONESTOPBIT;
        break;
    };

    comDCB.fParity = TRUE;
    switch(mParity)
    {
    case 0:
        comDCB.Parity = NOPARITY;
        break;
    case 1:
        comDCB.Parity = EVENPARITY;
        break;
    case 2:
        comDCB.Parity = ODDPARITY;
        break;
    default:
        comDCB.Parity = NOPARITY;
        break;
    };

    SetCommState(mCom, &comDCB);


    //  Get the timeouts
    GetCommTimeouts(mCom, &timeouts);

    // Set to blocking receive call.
    timeouts.ReadIntervalTimeout = 4294967295UL;
    timeouts.ReadTotalTimeoutConstant = 4294967295UL;
    timeouts.ReadTotalTimeoutMultiplier = 500;
    timeouts.WriteTotalTimeoutConstant = 1;
    timeouts.WriteTotalTimeoutMultiplier = 500;

    //  Set the timeouts
    SetCommTimeouts(mCom, &timeouts);

    mConnected = true;

    result = 1;

#else

    struct termios options;

    bzero(&options, sizeof(struct termios));

    mPortName = port;

    mCom = open(mPortName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if(mCom == -1)
    {
        std::cout << "Error: Could not open port " << mPortName << std::endl;
        return 0;
    }
    else
    {
        fcntl(mCom, F_SETOWN, getpid());
        //fcntl(mCom, F_SETFL, FASYNC);
    }

    options.c_cflag |= (CLOCAL | CREAD);

    //for unbuffered reading, remove? -gary
    //options.c_cc[VMIN] = 0;
    //options.c_cc[VTIME] = 0;

    //tcgetattr(mCom, &options);
    // Set the baud rate
    switch(mBaudRate)
    {
        case 9600:
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
        break;
        case 19200:
            cfsetispeed(&options, B19200);
            cfsetospeed(&options, B19200);
        break;
        case 38400:
            cfsetispeed(&options, B38400);
            cfsetospeed(&options, B38400);
        break;
        case 57600:
            cfsetispeed(&options, B57600);
            cfsetospeed(&options, B57600);
        break;

        case 115200:
            cfsetispeed(&options, B115200);
            cfsetospeed(&options, B115200);
        break;
        case 230400:
            cfsetispeed(&options, B230400);
            cfsetospeed(&options, B230400);
        break;
        case 460800:
            cfsetispeed(&options, B460800);
            cfsetospeed(&options, B460800);
        break;
        case 921600:
            cfsetispeed(&options, B921600);
            cfsetospeed(&options, B921600);
        break;
        default:
            cfsetispeed(&options, mBaudRate);
            cfsetospeed(&options, mBaudRate);
        break;
    }

    // Disable flow control.
    options.c_cflag &= ~CRTSCTS;

    // clear character size, then set it
    options.c_cflag &= ~CSIZE;
    switch(mDataBits)
    {
    case 8:
        options.c_cflag |= CS8;
        break;
    case 7:
        options.c_cflag |= CS7;
        break;
    case 6:
        options.c_cflag |= CS6;
        break;
    case 5:
        options.c_cflag |= CS5;
        break;
    default:
        options.c_cflag |= CS8;
        break;
    }

    if(mParity)
    {
        if(mParity == CX_SERIAL_EVEN_PARITY)
        {
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;  //  Enable even parity
            //options.c_iflag |= INPCK;
        }
        else
        {
            options.c_cflag |= PARENB;
            options.c_cflag |= PARODD;  //  Enable odd parity
            //options.c_iflag |= INPCK;
        }
    }
    else
    {
        options.c_cflag &= ~PARENB;
        options.c_iflag |= IGNPAR;
    }


    if(mStopBits == 2)
    {
        options.c_cflag |= CSTOPB;
    }
    else
    {
        options.c_cflag &= ~CSTOPB;
    }

    tcflush(mCom, TCIFLUSH);
    if(tcsetattr(mCom, TCSANOW, &options) >= 0)
    {
        mConnected = true;
        result = 1;
    }

    //Set RTS/DTR lines like windows
    //make outside settable? -gary
    int status;
    ioctl(mCom, TIOCMGET, &status);
    if(mEnableDTR)
    {
        status |= TIOCM_DTR;  //on
    }
    else
    {
        status &= ~TIOCM_DTR;   //off
    }

    if(mEnableRTS)
    {
        status |= TIOCM_RTS;  //on
    }
    else
    {
        status &= ~TIOCM_RTS;   //off
    }

    ioctl(mCom, TIOCMSET, &status);

#endif
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Disconnects from the port.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Serial::Disconnect()
{
#ifdef WIN32
    PurgeComm(mCom, PURGE_RXABORT);
    CloseHandle(mCom);
    mConnected = false;
    mCom = NULL;
    mPortName = "";
    return 1;
#else
    if(mCom != 0)
    {
        close(mCom);
    }
    mConnected = false;
    mCom = 0;
    mPortName = "";
    return 1;
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Closes the current connection and reconnects with the same
///  settings.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Serial::Reconnect()
{
    return Connect(mPortName.c_str(), mBaudRate, mDataBits, mParity, mStopBits);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Writes to the serial port.
///
///  \param buff Buffer containing data to write.
///  \param size Number of bytes/size of buffer to write.
///
///  \return Number of bytes sent.
///
////////////////////////////////////////////////////////////////////////////////////
int Serial::Send(const char *buff, const unsigned int size)
{
    Mutex::ScopedLock lock(&mSerialMutex);
#ifdef WIN32
    OVERLAPPED osWrite = {0};
    DWORD dwWritten = 0;
    DWORD dwRes = WAIT_TIMEOUT;
    BOOL fRes;

    // Create this write operation's OVERLAPPED structure's hEvent.
    osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (osWrite.hEvent == NULL)
    {
        // error creating overlapped event handle
        return 0;
    }
    // Issue write.
    if (!WriteFile(mCom, buff, size, &dwWritten, &osWrite)) 
    {
        if (GetLastError() != ERROR_IO_PENDING)
        { 
            // WriteFile failed, but isn't delayed. Report error and abort.
            fRes = FALSE;
        }
        else
        {
            // Write is pending.
            dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
            switch(dwRes)
            {
                // OVERLAPPED structure's event has been signaled. 
            case WAIT_OBJECT_0:
                if (!GetOverlappedResult(mCom, &osWrite, &dwWritten, FALSE))
                {
                    fRes = FALSE;
                }
                else
                {
                    // Write operation completed successfully.
                    fRes = TRUE;
                }
                break;
            default:
                // An error has occurred in WaitForSingleObject.
                // This usually indicates a problem with the
                // OVERLAPPED structure's event handle.
                fRes = FALSE;
                break;
            }
        }
    }
    else
    {
        // WriteFile completed immediately.
        fRes = TRUE;
    }

    CloseHandle(osWrite.hEvent);
    return (int)dwWritten;

    /* OLD
    unsigned long sum = 0;
    BOOL writeOK;

    if(mConnected)
    {
        writeOK = WriteFile(mCom, (void *)buff, size, &sum, 0);
        //  Error checking.
        if (!writeOK || sum < size ) 
        {
            mLastError = GetLastError();
            return 0;
        }
        return (int)sum;
    }
    */
#else

    if(mConnected)
    {
        int sum;
        sum = write(mCom, buff, size);
        //  Error checking.
        if( sum < 0 )
            return 0;
        return (int)sum;
    }

#endif
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Writes to the serial port.
///
///  \param p Packet data to send.
///
///  \return Number of bytes sent.
///
////////////////////////////////////////////////////////////////////////////////////
int Serial::Send(const Packet& p)
{
    Mutex::ScopedLock lock(&mSerialMutex);
    return Send((const char *)p.Ptr(), (unsigned int)p.Length());
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Writes to the serial port.
///
///  \param message Character string to send.
///
///  \return Number of bytes sent.
///
////////////////////////////////////////////////////////////////////////////////////
int Serial::Send(const std::string& message)
{
    Mutex::ScopedLock lock(&mSerialMutex);
    return Send(message.c_str(), ((unsigned int)message.size()));
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Reads from the serial port.
///
///  Function returns the number of bytes actually read.
///
///  \param buff Buffer for storing read data.
///  \param size The size of the buffer for storing read data.
///  \param waitTimeMs Maximum time to wait for data in ms (default is 1000).
///
///  \return Number of bytes received.
///
////////////////////////////////////////////////////////////////////////////////////
int Serial::Recv(char *buff, unsigned int size, const unsigned int waitTimeMs)
{
#ifdef WIN32
    DWORD sum = 0;
    BOOL readOK = FALSE;
    if(mConnected)
    {
        char* ptr = buff;
        unsigned int totalRead = 0;
        
        Time::Stamp startTimeMs = Time::GetUtcTimeMs();
        for(totalRead = 0; totalRead < size && Time::GetUtcTimeMs() - startTimeMs <= waitTimeMs;)
        {
            sum = 0;
            DWORD dwRead = 0;
            BOOL fWaitingOnRead = FALSE;
            OVERLAPPED osReader = {0};
            char temp[5];

            // Create the overlapped event. Must be closed before exiting
            // to avoid a handle leak.
            osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

            if (osReader.hEvent == NULL)
            {
            }
            // Error creating overlapped event; abort.
            if (!fWaitingOnRead) 
            {
                // Issue read operation.
                if (!ReadFile(mCom, temp, 1, &dwRead, &osReader)) 
                {
                    if (GetLastError() != ERROR_IO_PENDING)     // read not delayed?
                    {
                        // Error in communications; report it.
                    }
                    else
                    {
                        fWaitingOnRead = TRUE;
                    }
                }
                else 
                {   
                    sum = dwRead;
                }
            }

            // Try again.
            DWORD dwRes = WAIT_TIMEOUT;

            if (fWaitingOnRead) 
            {
                dwRes = WaitForSingleObject(osReader.hEvent, waitTimeMs);
                switch(dwRes)
                {
                    // Read completed.
                case WAIT_OBJECT_0:
                    if (!GetOverlappedResult(mCom, &osReader, &dwRead, FALSE))
                    {
                        // Error in communications; report it.
                    }
                    else
                    {
                        sum = dwRead;
                        // Read completed successfully.
                    }

                    //  Reset flag so that another opertion can be issued.
                    fWaitingOnRead = FALSE;
                    break;
                case WAIT_TIMEOUT:
                    // Operation isn't complete yet. fWaitingOnRead flag isn't
                    // changed since I'll loop back around, and I don't want
                    // to issue another read until the first one finishes.
                    
                    break;                     
                default:
                    // Error in the WaitForSingleObject; abort.
                    // This indicates a problem with the OVERLAPPED structure's
                    // event handle.
                    break;
                }
            }

            if(sum > 0)
            {
                memcpy(&buff[totalRead], temp, sum);
                totalRead += (unsigned int)sum;                
                //ptr += sum;
            }
        }

        return (int)totalRead;
    }

    /* OLD 
    unsigned long sum = 0;
    BOOL readOK;

    if(mConnected)
    {
        mLastError = ERROR_SUCCESS;
        readOK = ReadFile(mCom, buff, size, &sum, 0);
        return (int)sum;
    }
    */
#else
    if(mConnected)
    {
        int sum;

        sum = read(mCom, buff, size);

        if( sum < 0 )
            return 0;
        return sum;
    }
#endif
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Reads from the serial port.
///
///  Function returns the number of bytes actually read.
///
///  \param p Packet to receive data into.
///  \param len The number of bytes to try receive.
///  \param waitTimeMs Maximum time to wait for data in ms (default is 1000).
///
///  \return Number of bytes received.
///
////////////////////////////////////////////////////////////////////////////////////
int Serial::Recv(Packet& p, unsigned int len, const unsigned int waitTimeMs)
{
    p.Clear();
#ifdef WIN32
    p.Reserve(len + 256);
    int bytesRead = 0;
    if( (bytesRead = Recv((char *)p.Ptr(), len, waitTimeMs)) > 0)
    {
        p.SetWritePos(0);
        p.SetReadPos(0);
        p.SetLength((unsigned int)bytesRead);

        return bytesRead;
    }
#else

    if(mConnected) 
    {
        int sum;
        p.Reserve(len);

        sum = read(mCom, (char*)p.Ptr(), len);
        if(sum > 0)
        {
            p.SetWritePos(0);
            p.SetReadPos(0);
            p.SetLength(sum);
        }
        else
            p.Clear();
        //  Error checking...
        if( sum < 0 )
            return 0;
        return sum;
    }
#endif

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Flushes all data in the read and write buffers.
///
////////////////////////////////////////////////////////////////////////////////////
void Serial::Flush()
{
#ifdef WIN32
    FlushReadBuffer();
    FlushWriteBuffer();
    /*
    if( mCom)
    {
        FlushFileBuffers(mCom);
    }
    */
#else
    if(mCom)
    {
        tcflush(mCom, TCIOFLUSH );
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Flushes all data in the read buffer if the device driver supports
///          the operation.
///
////////////////////////////////////////////////////////////////////////////////////
void Serial::FlushReadBuffer()
{
#ifdef WIN32
    if( mCom)
    {
        PurgeComm(mCom, PURGE_RXCLEAR);
    }
#else
    if(mCom)
    {
        tcflush(mCom, TCIFLUSH);
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Flushes all data in the write buffer if the device driver supports
///          the operation.
///
////////////////////////////////////////////////////////////////////////////////////
void Serial::FlushWriteBuffer()
{
#ifdef WIN32
    if( mCom)
    {
        PurgeComm(mCom, PURGE_TXCLEAR);
    }
#else
    if(mCom)
    {
        tcflush(mCom, TCIOFLUSH );
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Resets the serial port: Flushes all queues and interrupt/terminate
///             any pending i/o operations.
///
////////////////////////////////////////////////////////////////////////////////////
void Serial::Reset()
{
#ifdef WIN32
    if( mCom)
    {
        PurgeComm(mCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
    }
#else
    if(mCom)
    {
        tcflush(mCom, TCIOFLUSH);
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Reports number of bytes available in serial port read-queue
///
///  \return Number of bytes available to be read.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Serial::ReadBytesAvailable(void)
{
#ifdef WIN32
    COMSTAT comStat;
    if (ClearCommError(mCom, &mErrors, &comStat))
    {
        return comStat.cbInQue;
    }
#else
    int bytes = 0;
    ioctl(mCom, FIONREAD, &bytes);
    return (unsigned int)bytes;
#endif
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Reports number of bytes pending in serial port write-queue
///
///  \return Number of bytes remaining to be written. (0 IN LINUX).
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Serial::WriteBytesPending(void)
{
#ifdef WIN32
    COMSTAT comStat;
    if (ClearCommError(mCom, &mErrors, &comStat))
    {
        return comStat.cbOutQue;
    }
#else
    int bytes = 0;
    //ioctl(mCom, FIONWRITE, &bytes);
    return (unsigned int)bytes;
#endif
    return 0;
}

/*  End of File */
