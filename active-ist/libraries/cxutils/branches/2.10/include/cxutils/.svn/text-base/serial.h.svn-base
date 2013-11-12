////////////////////////////////////////////////////////////////////////////////////
///
///  \file serial.h
///  \brief Serial (RS232) communication interface.
///   
///  <br>Author(s): Daniel Barber
///  <br>Created: 1 June 2007
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
#ifndef __CXUTILS_SERIAL_H
#define __CXUTILS_SERIAL_H

#include <string>
#include "packet.h"
#include "mutex.h"

namespace CxUtils
{
    const int CX_SERIAL_NO_PARITY   = 0;        ///<  No parity on serial data.
    const int CX_SERIAL_EVEN_PARITY = 1;        ///<  Even parity serial data.
    const int CX_SERIAL_ODD_PARITY  = 2;        ///<  Odd parity on serial data.

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Serial
    ///   \brief Serial (RS232) communication interface.
    ///
    ///   This interface uses a mutex so that it is thread safe for parallel
    ///   reading and writing.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Serial
    {
    public:
        Serial();
        virtual ~Serial();
        int Connect(const std::string& port, 
                    const unsigned int baud = 9600, 
                    const unsigned int bits = 8, 
                    const unsigned int partiy = 0, 
                    const unsigned int stop = 1); 
        int Initialize(const std::string& port,
                       const unsigned int baud = 9600,
                       const unsigned int bits = 8,
                       const unsigned int parity = 0,
                       const unsigned int stop = 1)
        {
            return Connect(port, baud, bits, parity, stop);
        }
        int Disconnect();   
        int Shutdown() { return Disconnect(); }
        int Reconnect();            
        int Send(const char *buff, const unsigned int size);  
        int Send(const std::string& message);
        int Send(const Packet& p);                
        int Recv(char* buff, unsigned int size, const unsigned int waitTimeMs = 1000);        
        int Recv(Packet& p, const unsigned int len, const unsigned int waitTimeMs = 1000);
        void Reset(void);
        void Flush(void);
        void FlushReadBuffer(void);
        void FlushWriteBuffer(void);
        void EnableRTS(const bool enable) { mEnableRTS = enable; }
        void EnableDTR(const bool enable) { mEnableDTR = enable; }
        unsigned int ReadBytesAvailable(void);
        unsigned int WriteBytesPending(void);
#ifdef WIN32
        unsigned long GetSystemError(void) const { return mLastError; };
#endif
        inline int  GetBaudRate() const { return mBaudRate; }      
        inline int  GetStopBits() const { return mStopBits; }
        inline int  GetParity() const { return mParity; }
        inline int  GetDataBits() const { return mDataBits; }
        inline const char* GetPortName() const { return mPortName.c_str(); }        
        inline bool IsConnected() const { return mConnected; }
    protected:
    #ifdef WIN32
        void* mCom;                 ///<  Windows handle.
        unsigned long  mErrors;     ///<  Generic Error value
        unsigned long  mLastError;  ///<  System Error code related to last error, undefined when no error reported
    #else        
        int    mCom;                ///<  Handle to port.
    #endif
        std::string mPortName;      ///< Port name
        char mParity;               ///< Parity
        int  mBaudRate;             ///< Current baudrate
        int  mStopBits;             ///< Stop bits
        int  mDataBits;             ///< Data bits
        bool   mConnected;          ///< Active connection?
        Mutex mSerialMutex;         ///< Mutex for thread protection of serial port.
        bool mEnableRTS;            ///< If true RTS line is enabled, default is false.
        bool mEnableDTR;            ///< If true DTS line is enabled, default is false.
    };
}

#endif

/*  End of File */
