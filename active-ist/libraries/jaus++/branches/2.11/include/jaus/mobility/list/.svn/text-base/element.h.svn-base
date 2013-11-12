////////////////////////////////////////////////////////////////////////////////////
///
///  \file element.h
///  \brief This file contains the definition of the Element class,
///         encapsulating a Jaus AS6009 Element. 
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 4 February 2010
///  <br>Copyright (c) 2010
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
#ifndef __JAUS_MOBILITY_LIST_ELEMENT__H
#define __JAUS_MOBILITY_LIST_ELEMENT__H

#include "jaus/core/message.h"
#include "jaus/mobility/jausmobilitydll.h"
#include <map>
#include <vector>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Element
    ///   \brief This class encapsulates a Jaus AS6009 Element used in the List Manager
    ///          Service.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL Element
    {
    public:
        typedef std::map<UShort, Element> Map; ///<  Type definition for STL map of Elements.
        typedef std::vector<Element> List;     ///<  List of elements.
        Element(const UShort id = 0, 
                const UShort next = 0, 
                const UShort prev = 0);
        Element(const Element& element);
        ~Element();
        void Clear();
        Element& operator=(const Element& element);
        UShort mID;         ///<  ID of the element.
        UShort mNextID;     ///<  ID of the next element in a list.
        UShort mPrevID;     ///<  ID of the previous element in a list.
        Message* mpElement; ///<  Pointer to actual element data (Message Payload).
        Packet mPayload;    ///<  Payload data (serialized message data).
    };
}


#endif
/*  End of File */
