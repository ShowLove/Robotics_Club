////////////////////////////////////////////////////////////////////////////////////
///
///  \file deleteelement.h
///  \brief This file contains the header of a JAUS message.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 23 November 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: bsun@ist.ucf.edu
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
#ifndef __JAUS_CORE_MOBILITY_DELETE_ELEMENT__H
#define __JAUS_CORE_MOBILITY_DELETE_ELEMENT__H

#include "jaus/mobility/mobilitycodes.h"
#include "jaus/core/message.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class DeleteElement
    ///   \brief This message is used to delete one or more elements from a list.
    ///          Each element is uniquely identified by the UID.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL DeleteElement : public Message
    {
    public:
        typedef std::vector<UShort> List;
        DeleteElement(const Address& dest = Address(), const Address& src = Address());
        DeleteElement(const DeleteElement& message);
        ~DeleteElement();
        inline Byte SetRequestID(const Byte rid)  { return mRequestID = rid; }
        inline Byte GetRequestID() const { return mRequestID; } 
        inline List* GetElementList() { return &mElementUIDs; }
        inline const List* GetElementList() const { return &mElementUIDs; }        
        virtual bool IsCommand() const { return true; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new DeleteElement(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return CONFIRM_ELEMENT_REQUEST; }
        virtual std::string GetMessageName() const { return "Delete Element"; }
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return BYTE_SIZE + USHORT_SIZE + mElementUIDs.size()*USHORT_SIZE > maxPayloadSize ? true : false; }
        virtual int RunTestCase() const;
        DeleteElement& operator=(const DeleteElement& message);
    protected:
        Byte    mRequestID;        ///< ID of request to be returned in response message.
        List    mElementUIDs;      ///< UIDs of the element to be removed (65535 represents all).
    };
}

#endif
/*  End of File */
