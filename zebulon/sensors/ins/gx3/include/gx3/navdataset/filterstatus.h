/////////////////////////////////////////////////////////////////////////////
///
/// \file filterstatus.h
/// \brief A simple class fordefining message structures/data for
///        MicroStrain gx3
///
/// Author(s): Gary Stein<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: gstein@mail.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
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
////////////////////////////////////////////////////////////////////////////////////
#ifndef __ZEBULON_GX3_MESSAGE_FILTERSTATUS__H
#define __ZEBULON_GX3_MESSAGE_FILTERSTATUS__H

#include <string>
#include <cxutils/packet.h>
#include <list>
#include <cxutils/math/cxmath.h>
#include <iostream>

#include "gx3/message.h"

////////////////////////////////////////////////////////////////////////////////////
///
///  \namespace Zebulon
///  \brief Namespace for all software developed by the Robotics Laboratory and
///         Club at the University of Central Florida.
///
////////////////////////////////////////////////////////////////////////////////////
namespace Zebulon
{
    namespace INS
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \namespace MicroStrain
        ///  \brief MicroStrain sensors, etc. namespace for the MicroStrain library.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        namespace MicroStrain
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class EstimatedAngularRate
            ///  \brief EstimatedAngularRate is a type of message for the GX3
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class FilterStatus : public Message
            {
            public:

                FilterStatus(const Message::Type type,const unsigned char length);
                virtual ~FilterStatus();
                virtual void Print() const;
                virtual unsigned int Write(CxUtils::Packet& msg) const;
                virtual unsigned int Read(const unsigned char* buffer,
                                          unsigned int length);
                virtual Message* Clone() const;
                const static unsigned char Length=8;

                //three angle rates in radians/sec
                unsigned short mFilterState;
                unsigned short mDynamicsMode;
                unsigned short mStatusFlags;

            };
        }
    }
}


#endif
/*  End of file */
