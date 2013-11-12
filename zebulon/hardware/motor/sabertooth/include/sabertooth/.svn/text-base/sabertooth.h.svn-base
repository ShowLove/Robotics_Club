/////////////////////////////////////////////////////////////////////////////////////
///
///  \file sabertooth.h
///  \brief Interface class for Sabertooth 5 and 10.
///
///  Author(s): Gary Stein<br>
///  Created: 2010<br>
///  Copyright (c) 2010<br>
///  Robotics Laboratory and Club<br>
///  University of Central Florida (UCF) <br>
///  Email: gstein@mail.ucf.edu<br>
///  Web: http://robotics.ucf.edu <br>
///  All Rights Reserved <br>
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
#ifndef _ZEBULON_SABERTOOTH__H
#define _ZEBULON_SABERTOOTH__H

#include <cxutils/thread.h>
#include <cxutils/mutex.h>
#include <cxutils/serial.h>
#include <set>

#ifndef ZEB_SABERTOOTH_DLL

#ifdef WIN32
#define ZEB_SABERTOOTH_DLL __declspec(dllimport)
#else
#define ZEB_SABERTOOTH_DLL
#endif

#endif

namespace Zebulon
{
    namespace Motor
    {
		////////////////////////////////////////////////////////////////////////////////////
		///
		///  \class Sabertooth 
		///  \brief Interface class for talking to a Sabertooth DC Motor Board over a 
		///  serial port.
		///
		////////////////////////////////////////////////////////////////////////////////////
		class ZEB_SABERTOOTH_DLL Sabertooth
		{
		public:
			static const unsigned int MaxChannel = 2;
			Sabertooth();
			~Sabertooth();
			bool Initialize(const std::string& port,
							const unsigned int delay = 20,
							const unsigned int reversed = 0);
			void Shutdown();
			bool SetValue(const unsigned int channel, const double value);
			double GetValue(const unsigned int channel) const;
		private:
			bool WriteData();
			static void WriteThread(void* args);
			CxUtils::Serial mSerial;                    ///<  Interface to serial port.
			CxUtils::Thread mWriteThread;               ///<  Thread object to run thread for writing to serial port.
			CxUtils::Mutex  mMutex;                     ///<  Mutex for thread protection of data.
			unsigned int mDelayTimeMs;                  ///<  How long to wait between write updates.
			
			//Only Two Channels on a single Sabertooth
			double mValues[MaxChannel];               ///<  Values to be sent to the different channels
			
			unsigned char MakeByte(double value);
		};
	}
}

#endif
/*  End of File */
