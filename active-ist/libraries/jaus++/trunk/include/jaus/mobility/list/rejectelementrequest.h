////////////////////////////////////////////////////////////////////////////////////
///
///  \file rejectelementrequest.h
///  \brief This file contains the header of a JAUS message.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 1 March 2010
///  <br>Copyright (c) 2010
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
#ifndef __JAUS_CORE_MOBILITY_REJECT_ELEMENT_REQUEST__H
#define __JAUS_CORE_MOBILITY_REJECT_ELEMENT_REQUEST__H

#include "jaus/mobility/mobilitycodes.h"
#include "jaus/core/message.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class RejectElementRequest
    ///   \brief This message is used to reject an operation on an element list.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL RejectElementRequest : public Message
    {
    public:
        // Response types for rejection of element request.
        enum Response
        {
            InvalidElementID = 1,
            InvalidPreviousElement,
            InvalidNextElement,
            UnsupportedElementType,
            ElementIDNotFound,
            OutOfMemory,
            UnspecifiedError
        };
        RejectElementRequest(const Address& dest = Address(), const Address& src = Address());
        RejectElementRequest(const RejectElementRequest& message);
        ~RejectElementRequest();
        inline Byte SetRequestID(const Byte rid)  { return mRequestID = rid; }
        inline Byte SetResponseCode(const Response code)  { return mResponseCode = code; } 
        inline Byte GetRequestID() const { return mRequestID; }
        inline Byte GetResponseCode() const { return mResponseCode; } 
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new RejectElementRequest(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Reject Element Request"; }
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return false; }
        virtual int RunTestCase() const;
        RejectElementRequest& operator=(const RejectElementRequest& message);
    protected:
        Byte    mRequestID;        ///<  ID of request to be confirmed.
        Response mResponseCode;    ///<  Response code.
    };
}

#endif
/*  End of File */
