////////////////////////////////////////////////////////////////////////////////////
///
///  \file corecodes.h
///  \brief This file lists all message codes for messages in the JAUS Core
///         Service Set
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 30 September 2009
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
#ifndef _JAUS_CORE_MESSAGE_CLASS_CODES_H
#define _JAUS_CORE_MESSAGE_CLASS_CODES_H

#include "jaus/core/types.h"
#include <string>

namespace JAUS
{
    // Command Messages
    const UShort SET_AUTHORITY                                        =   0x0001; ///<  Message code.
    const UShort SHUTDOWN                                             =   0x0002; ///<  Message code.
    const UShort STANDBY                                              =   0x0003; ///<  Message code.
    const UShort RESUME                                               =   0x0004; ///<  Message code.
    const UShort RESET                                                =   0x0005; ///<  Message code.
    const UShort SET_EMERGENCY                                        =   0x0006; ///<  Message code.
    const UShort CLEAR_EMERGENCY                                      =   0x0007; ///<  Message code.    
    const UShort REQUEST_CONTROL                                      =   0x000D; ///<  Message code.
    const UShort RELEASE_CONTROL                                      =   0x000E; ///<  Message code.
    const UShort CONFIRM_CONTROL                                      =   0x000F; ///<  Message code.
    const UShort REJECT_CONTROL                                       =   0x0010; ///<  Message code.
    const UShort SET_TIME                                             =   0x0011; ///<  Message code.
    const UShort CREATE_EVENT                                         =   0x01F0; ///<  Message code.
    const UShort UPDATE_EVENT                                         =   0x01F1; ///<  Message code.
    const UShort CANCEL_EVENT                                         =   0x01F2; ///<  Message code.
    const UShort CONFIRM_EVENT_REQUEST                                =   0x01F3; ///<  Message code.
    const UShort REJECT_EVENT_REQUEST                                 =   0x01F4; ///<  Message code.
    const UShort REGISTER_SERVICES                                    =   0x0B00; ///<  Message code.

    // Query Messages.
    const UShort QUERY_AUTHORITY                                      =   0x2001; ///<  Message code.
    const UShort QUERY_STATUS                                         =   0x2002; ///<  Message code.
    const UShort QUERY_TIMEOUT                                        =   0x2003; ///<  Message code.
    const UShort QUERY_TIME                                           =   0x2011; ///<  Message code.
    const UShort QUERY_CONTROL                                        =   0x200D; ///<  Message code.
    const UShort QUERY_EVENTS                                         =   0x21F0; ///<  Message code.
    const UShort QUERY_HEARTBEAT_PULSE                                =   0x2202; ///<  Message code.
    const UShort QUERY_IDENTIFICATION                                 =   0x2B00; ///<  Message code.
    const UShort QUERY_CONFIGURATION                                  =   0x2B01; ///<  Message code.
    const UShort QUERY_SUBSYSTEM_LIST                                 =   0x2B02; ///<  Message code.
    const UShort QUERY_SERVICES                                       =   0x2B03; ///<  Message code.

    // Inform Messages.
    const UShort REPORT_AUTHORITY                                     =   0x4001; ///<  Message code.
    const UShort REPORT_STATUS                                        =   0x4002; ///<  Message code.
    const UShort REPORT_TIMEOUT                                       =   0x4003; ///<  Message code.
    const UShort REPORT_TIME                                          =   0x4011; ///<  Message code.
    const UShort REPORT_CONTROL                                       =   0x400D; ///<  Message code.
    const UShort REPORT_EVENTS                                        =   0x41F0; ///<  Message code.
    const UShort EVENT                                                =   0x41F1; ///<  Message code.
    const UShort REPORT_HEARTBEAT_PULSE                               =   0x4202; ///<  Message code.    
    const UShort REPORT_IDENTIFICATION                                =   0x4B00; ///<  Message code.
    const UShort REPORT_CONFIGURATION                                 =   0x4B01; ///<  Message code.
    const UShort REPORT_SUBSYSTEM_LIST                                =   0x4B02; ///<  Message code.
    const UShort REPORT_SERVICES                                      =   0x4B03; ///<  Message code.
}

#endif
/*  End of File */
