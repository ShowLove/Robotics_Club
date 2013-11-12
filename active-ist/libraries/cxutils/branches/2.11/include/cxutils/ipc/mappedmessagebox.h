////////////////////////////////////////////////////////////////////////////////////
///
///  \file mappedmessagebox.h
///  \brief Message box is a data structure that can be used to create a
///  message queue within shared memory for passing messages/packets between
///  multiple processes and threads.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 11 March 2009
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
#ifndef __CXUTILS_IPC_MAPPED_MESSAGE_BOX_H
#define __CXUTILS_IPC_MAPPED_MESSAGE_BOX_H

#include "cxutils/ipc/mappedmemory.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class MappedMessageBox
    ///   \brief Message storage class for sharing packet or string data between
    ///   threads/processes using shared memory.  This MappedMessageBox class is only 
    ///   compatible with other programs using the same from CxUtils.
    ///
    ///   Ass the MappedMessageBox gets full old messages are automatically deleted (buffer
    ///   overflow) to make room for new messages.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL MappedMessageBox
    {
    public:
        static const unsigned int DefaultSize = 2097152;
        static const Time::Stamp DefaultTimeOut = 500;
        typedef MappedMemory::ID ID;                    ///<  ID Field.        

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Header
        ///   \brief Class for storing MappedMessageBox header information that is stored at
        ///   the beginning of shared memory used in MappedMessageBox.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CX_UTILS_DLL Header
        {
        public:
            static const unsigned int Size = 28; ///<  Size of message box header in bytes.
            Header();
            Header(const Header& header);
            ~Header();
            Header& operator=(const Header& header);
            Time::Stamp mWriteTimeMs;       ///<  The last time a message was written to box in ms.
            Time::Stamp mReadTimeMs;        ///<  The last time a process read data from the box in ms.
            unsigned int mCount;            ///<  The current number of messages stored in the box in ms.
            unsigned int mStartBytePos;     ///<  The byte index of the first message in the box (from memory index 0).
            unsigned int mEndBytePos;       ///<  The ending byte index of the last message (from memory index 0).
        };
        MappedMessageBox();
        ~MappedMessageBox();
        bool OpenMessageBox(const ID id);
        bool OpenMessageBox(const std::string& name);
        bool CreateMessageBox(const ID id,
                              const unsigned int size = 500000);
        bool CreateMessageBox(const std::string& name,
                              const unsigned int size = 500000);
        void CloseMessageBox();
        bool IsOpen() const;
        bool IsEmpty() const;
        bool IsActive(const bool readTimeFlag, 
                      const unsigned int thresholdMilliseconds = DefaultTimeOut) const;
        bool Touch();
        bool WriteMessage(const Packet& message);
        bool WriteMessage(const std::string& message);
        bool ReadMessage(Packet& message) const;
        bool ReadMessage(std::string& message) const;
        unsigned int GetBufferSize() const;
        unsigned int GetNumMessages() const;
        unsigned int GetNumMessagesRead() const { return mMessagesRead; }
    private:
        static const unsigned int MessageHeaderSize = sizeof(unsigned int);    /// Size in bytes of header before each message in box.
        Header ReadHeader() const;
        void WriteHeader(const Header& header) const;
        MappedMemory mMessageBox;                           ///<  Shared memory used for storing messages.
        unsigned int mMessagesRead;                         ///<  Number of messages read so far.
    };
}

#endif
/*  End of File */
