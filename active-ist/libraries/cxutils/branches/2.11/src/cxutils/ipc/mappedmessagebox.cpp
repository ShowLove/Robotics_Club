////////////////////////////////////////////////////////////////////////////////////
///
///  \file mappedmessagebox.cpp
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
#include "cxutils/ipc/mappedmessagebox.h"
#include "cxutils/time.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes all values to 0.
///
////////////////////////////////////////////////////////////////////////////////////
MappedMessageBox::Header::Header() : mWriteTimeMs(0),
                                     mReadTimeMs(0),
                                     mCount(0),
                                     mStartBytePos(Header::Size),
                                     mEndBytePos(Header::Size)
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
MappedMessageBox::Header::Header(const MappedMessageBox::Header& header) :  mWriteTimeMs(0),
                                                                            mReadTimeMs(0),
                                                                            mCount(0),
                                                                            mStartBytePos(Header::Size),
                                                                            mEndBytePos(Header::Size)
{
    *this = header;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
MappedMessageBox::Header::~Header()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
MappedMessageBox::Header& MappedMessageBox::Header::operator =(const MappedMessageBox::Header& header)
{
    if(this != &header)
    {
        mWriteTimeMs = header.mWriteTimeMs;
        mReadTimeMs = header.mReadTimeMs;
        mCount = header.mCount;
        mStartBytePos = header.mStartBytePos;
        mEndBytePos = header.mEndBytePos;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
MappedMessageBox::MappedMessageBox() : mMessagesRead(0)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
MappedMessageBox::~MappedMessageBox()
{
    CloseMessageBox();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Opens a view to a message box that is already created.  Once open
///   you may read/write to the message box.
///
///   \param[in] id ID number of the shared memory location.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMessageBox::OpenMessageBox(const ID id)
{
    bool result = false;

    CloseMessageBox();

    if(mMessageBox.OpenMappedMemory(id) && mMessageBox.Length() > Header::Size)
    {
        result = true;
    }

    if(result == false)
    {
        CloseMessageBox();
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Opens a view to a message box that is already created.  Once open
///   you may read/write to the message box.
///
///   \param[in] name The name used to reference the shared memory. Must be less
///                   than 20 characters in width. <b>To maximize
///                   portability between windows and linux, this should be an
///                   integer number.</b>
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMessageBox::OpenMessageBox(const std::string& name)
{
    bool result = false;

    CloseMessageBox();

    if(mMessageBox.OpenMappedMemory(name) && mMessageBox.Length() > Header::Size)
    {
        result = true;
    }

    if(result == false)
    {
        CloseMessageBox();
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Attempts to create a shared memory message box with a specific
///          unique name and size in bytes.  If a message box with the same name
///          and size already exists, this method will open a view of, otherwise
///          it will fail.
///
///   \param[in] id ID number of the shared memory location.
///   \param[in] size Size available for storage of your messages in bytes.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMessageBox::CreateMessageBox(const ID id,
                                        const unsigned int size)
{
    std::ostringstream input;
    input << id;
    return CreateMessageBox(input.str(), size);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Attempts to create a shared memory message box with a specific
///          unique name and size in bytes.  If a message box with the same name
///          and size already exists, this method will open a view of, otherwise
///          it will fail.
///
///   \param[in] name The name used to reference the shared memory. Must be less
///                   than 20 characters in width. <b>To maximize
///                   portability between windows and linux, this should be an
///                   integer number.</b>
///   \param[in] size Size available for storage of your messages in bytes.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMessageBox::CreateMessageBox(const std::string& name,
                                        const unsigned int size)
{
    bool result = false;
    Header header;

    CloseMessageBox();
    
    if(mMessageBox.CreateMappedMemory(name, size + Header::Size))
    {
        result = true;        
        mMessageBox.Lock();
        header.mStartBytePos = Header::Size;
        header.mEndBytePos = Header::Size;
        header.mWriteTimeMs = GetTimeMs();
        header.mReadTimeMs = GetTimeMs();
        WriteHeader(header);
        mMessageBox.Unlock();
    }
    else if(mMessageBox.OpenMappedMemory(name))
    {
        // Check to see how active the
        // message box is, and if it is not
        // active, flush the contents
        mMessageBox.Lock();
        header = ReadHeader();
        if(mMessageBox.Length() < size + Header::Size)
        {
            if(GetTimeMs() - header.mWriteTimeMs > DefaultTimeOut && 
               GetTimeMs() - header.mReadTimeMs > DefaultTimeOut)
            {
                mMessageBox.Unlock();
                mMessageBox.CloseMappedMemory();
                MappedMemory::DeleteMappedMemory(name);
                CxUtils::SleepMs(DefaultTimeOut);
                // Try to create again.
                if(mMessageBox.CreateMappedMemory(name, size + Header::Size))
                {
                    result = true;        
                    mMessageBox.Lock();
                    header.mStartBytePos = Header::Size;
                    header.mEndBytePos = Header::Size;
                    header.mWriteTimeMs = GetTimeMs();
                    header.mReadTimeMs = GetTimeMs();
                    WriteHeader(header);
                    mMessageBox.Unlock();
                }
            }
        }
        else
        {
            Time::Stamp currentTimeMs = GetTimeMs();
#ifndef WIN32 // LINUX
            // This if statement is needed because sometimes in Linux shared memory is not closed/released
            // when a process exits, so we need to be prepared for memory used later in the previous day
            if(header.mWriteTimeMs > currentTimeMs && header.mWriteTimeMs - currentTimeMs >= DefaultTimeOut)
            {
                result = false;
            }
#endif
            if(currentTimeMs > header.mWriteTimeMs && currentTimeMs - header.mWriteTimeMs >= DefaultTimeOut)
            {
                result = true;
            } 
        }
        if(result)
        {
            header.mWriteTimeMs = GetTimeMs();
            WriteHeader(header);
        }
        mMessageBox.Unlock();
    }
    

    if(result == false)
    {
        CloseMessageBox();
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Closes the view of the message box.  Once the last process
///   exits, the shared memory is released.
///
////////////////////////////////////////////////////////////////////////////////////
void MappedMessageBox::CloseMessageBox()
{
    mMessageBox.CloseMappedMemory();
    mMessagesRead = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if messsage box is open, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMessageBox::IsOpen() const
{
    if(mMessageBox.IsOpen())
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method checks to see if there are no message in the box, or if there
///          are no new messages to read.
///
///   \return True if box is empty or has no new messages, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMessageBox::IsEmpty() const
{
    if(mMessageBox.IsOpen())
    {
        Header header;
        mMessageBox.Lock();
        header = ReadHeader();
        mMessageBox.Unlock();

        if(header.mCount == 0)
        {
            return true;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method checks to see if there are threads/processes writing messages
///          by checking to see if a thread/process touched/wrote to the message
///          box within a threshold.
///
///   \param[in] readTimeFlag If true, the read time is checked for activity, if
///                           false, the write time to mssage box is checkd for
///                           activity.
///   \param[in] thresholdMilliseconds The threshold for determining if there
///              is an active writer to shared memory.
///
///   \return True if active, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMessageBox::IsActive(const bool readTimeFlag, const unsigned int thresholdMilliseconds) const
{
    if(mMessageBox.IsOpen())
    {
        Header header;
        Time::Stamp timeMs = 0;
        mMessageBox.Lock();
        header = ReadHeader();
        mMessageBox.Unlock();
        Time::Stamp currentTimeMs = GetTimeMs();
        timeMs = readTimeFlag ? header.mReadTimeMs : header.mWriteTimeMs;
#ifndef WIN32 // LINUX
        // This if statement is needed because sometimes in Linux shared memory is not closed/released
        // when a process exits, so we need to be prepared for memory used later in the previous day
        if(timeMs >= currentTimeMs && timeMs - currentTimeMs >= thresholdMilliseconds)
        {
            return false;
        }
#endif
        if(currentTimeMs >= timeMs && currentTimeMs - timeMs <= thresholdMilliseconds)
        {
            return true;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs a "touch" of shared memory by only updating the
///   write time value of shared memory header used for MappedMessageBox.  This is used
///   to let other processes know that someone is still writing to shared memory
///   even if they have not written any new messages.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMessageBox::Touch()
{
    if(mMessageBox.IsOpen())
    {
        Header header;
        mMessageBox.Lock();
        header = ReadHeader();
        header.mWriteTimeMs = GetTimeMs();
        WriteHeader(header);
        mMessageBox.Unlock();

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the message/packet data to the MappedMessageBox.
///
///   The message will be inserted at the end of the message box.
///
///   \param[in] message Packet data to write into message box for others to 
///              read.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMessageBox::WriteMessage(const Packet& message)
{
    bool result = false;
    if(mMessageBox.IsOpen())
    {
        Header header;
        mMessageBox.Lock();
        header = ReadHeader();
        // See if there is room in shared memory.
        unsigned int availableBytes = mMessageBox.Length() - Header::Size;
        unsigned int messageSize = message.Length() + MessageHeaderSize;
        if(messageSize < availableBytes)
        {
            // OK, so we know the message can fit in this box, but first
            // lets see if we need to delete old data.
            if(messageSize + header.mEndBytePos >= mMessageBox.Length() && header.mStartBytePos > Header::Size)
            {
                unsigned int dataSize = header.mEndBytePos - header.mStartBytePos;
                memmove(mMessageBox->Ptr() + Header::Size, 
                        mMessageBox->Ptr() + header.mStartBytePos, 
                        dataSize);
                header.mEndBytePos = dataSize + Header::Size;
                header.mStartBytePos = Header::Size;
            }

            if(messageSize + header.mEndBytePos < mMessageBox.Length() - 1)
            {
                mMessageBox->SetWritePos(header.mEndBytePos);
                if(mMessageBox->Write(((unsigned int)(message.Length()))) &&
                   mMessageBox->Write(message))
                {
                    result = true;
                    header.mEndBytePos += message.Length() + MessageHeaderSize;
                    header.mCount++;
                }
            }
        }
        header.mWriteTimeMs = GetTimeMs();
        WriteHeader(header);
        mMessageBox.Unlock();
    }
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the message/packet data to the MappedMessageBox.
///
///   The message will be inserted at the end of the message box.
///
///   \param[in] message String data to write into message box for others to 
///              read.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMessageBox::WriteMessage(const std::string& message)
{
    Packet::Wrapper wrapper((unsigned char *)message.c_str(), (unsigned int)message.size());
    return WriteMessage( *wrapper.GetData() );
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads a message out of the message box.
///
///   This method will read the first message that has not been read yet by this
///   instance of the MappedMessageBox.
///
///   \param[in] message Packet data to read from message box.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMessageBox::ReadMessage(Packet& message) const
{
    bool result = false;
    message.Clear();
    if(mMessageBox.IsOpen())
    {
        Header header;
        if(mMessageBox.Lock() == 0)
        {
            return result;
        }
        header = ReadHeader();
        
        if(header.mCount > 0 && header.mStartBytePos < header.mEndBytePos)
        {
            // Advance to the position of the next message we want to read.
            unsigned int messageSize = 0;
            unsigned int messagePosition = header.mStartBytePos;
            
            // Read the message size in the box.
            mMessageBox->Read(messageSize, messagePosition); 
            
            if(messageSize + MessageHeaderSize + messagePosition <= header.mEndBytePos)
            {
                if(mMessageBox->Read(message, messageSize, messagePosition + MessageHeaderSize) == (int)messageSize)
                {
                     (*( (unsigned *)(&mMessagesRead) ))++;
                    // Advance the start byte position in the array.
                    header.mStartBytePos = messagePosition + MessageHeaderSize + messageSize;
                    // Decrease the number of messages in the box.
                    header.mCount--;
                    result = true;
                }
            }
            if(messageSize + MessageHeaderSize > mMessageBox->Length())
            {
                assert("Corrupted Shared Memory Message Box" && 0);
            }
        }
        // Reset position values.
        if(header.mCount == 0)
        {
            header.mStartBytePos = header.mEndBytePos = Header::Size;
        }
        // Update the read time (shows we are actively checking for messages).
        header.mReadTimeMs = GetTimeMs();
        // Save updated header data.
        WriteHeader(header);
        // Release critical section.
        mMessageBox.Unlock();
    }
        
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads a message out of the message box.
///
///   This method will read the first message that has not been read yet by this
///   instance of the MappedMessageBox.
///
///   \param[in] message String data to read from message box.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMessageBox::ReadMessage(std::string& message) const
{
    Packet pmessage;
    message.clear();
    if(ReadMessage(pmessage))
    {
        for(unsigned int i = 0; i < pmessage.Length(); i++)
        {
            message.push_back(pmessage.Ptr()[i]);
        }
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Total amount of memory available for storing messages in bytes.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int MappedMessageBox::GetBufferSize() const
{
    if(mMessageBox.IsOpen())
    {
        return mMessageBox.Length() - Header::Size;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Number of messages inside of the shared memory queue/box.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int MappedMessageBox::GetNumMessages() const
{
    if(mMessageBox.IsOpen())
    {
        Header header;
        mMessageBox.Lock();
        header = ReadHeader();
        mMessageBox.Unlock();
        return header.mCount;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the message box header data.
///
///   \return The read header.
///
////////////////////////////////////////////////////////////////////////////////////
MappedMessageBox::Header MappedMessageBox::ReadHeader() const
{
    Header header;
    if(mMessageBox.GetData() == NULL)
    {
        return header;
    }
    mMessageBox->SetReadPos(0);
    if(mMessageBox->Length() < Header::Size)
    {
        return header;
    }
    mMessageBox->Read(header.mWriteTimeMs);
    mMessageBox->Read(header.mReadTimeMs);
    mMessageBox->Read(header.mCount);
    mMessageBox->Read(header.mStartBytePos);
    mMessageBox->Read(header.mEndBytePos);
    return header;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes over the current header of the message box.
///
///   \param[in] header New header data to write to shared memory.
///
////////////////////////////////////////////////////////////////////////////////////
void MappedMessageBox::WriteHeader(const MappedMessageBox::Header& header) const
{
    MappedMemory* memory = ((MappedMemory*)(&mMessageBox));
    if(memory->IsOpen() == false)
    {
        return;
    }
    (*memory)->SetWritePos(0);
    unsigned int totalWritten = 0;
    totalWritten += (*memory)->Write(header.mWriteTimeMs);
    totalWritten += (*memory)->Write(header.mReadTimeMs);
    totalWritten += (*memory)->Write(header.mCount);
    totalWritten += (*memory)->Write(header.mStartBytePos);
    totalWritten += (*memory)->Write(header.mEndBytePos);
}


/*  End of File */
