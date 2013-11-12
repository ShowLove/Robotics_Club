////////////////////////////////////////////////////////////////////////////////////
///
///  \file extrascodes.h
///  \brief This file lists all message codes for messages in the JAUS Extras
///         Service Set
///
///  <br>Author(s): Daniel Barber
///  Created: 15 January 2010
///  Copyright (c) 2010
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
#ifndef _JAUS_EXTRAS_MESSAGE_CLASS_CODES_H
#define _JAUS_EXTRAS_MESSAGE_CLASS_CODES_H

#include "jaus/core/types.h"
#include "jaus/extras/jausextrasdll.h"
#include <string>

namespace JAUS
{
    // Custom messages all have values greater than 0xD000

    // Command Messages
    const UShort SET_MICROCONTROLLER_STATE                        =   0xD005; ///<  Message code.
    const UShort SET_VELOCITY_VECTOR                              =   0xD006; ///<  Message code.

    // Query Messages.
    const UShort QUERY_IMAGE                                      =   0xD300; ///<  Message code.
    const UShort QUERY_CAMERA_COUNT                               =   0xD301; ///<  Message code.
    const UShort QUERY_MICROCONTROLLER_STATE                      =   0xD305; ///<  Message code.
    const UShort QUERY_RANGE_SENSOR_CONFIGURATION                 =   0xD30A; ///<  Message code.
    const UShort QUERY_LOCAL_RANGE_SCAN                           =   0xD30B; ///<  Message code.
    const UShort QUERY_AUDIO                                      =   0xD400; ///<  Message code.
    const UShort QUERY_VELOCITY_VECTOR                            =   0xD501; ///<  Message code.
    // Inform Messages.
    const UShort REPORT_IMAGE                                     =   0xD900; ///<  Message code.
    const UShort REPORT_CAMERA_COUNT                              =   0xD901; ///<  Message code.
    const UShort REPORT_MICROCONTROLLER_STATE                     =   0xD905; ///<  Message code.
    const UShort REPORT_RANGE_SENSOR_CONFIGURATION                =   0xD90A; ///<  Message code.
    const UShort REPORT_LOCAL_RANGE_SCAN                          =   0xD90B; ///<  Message code.
    const UShort REPORT_AUDIO                                     =   0xDA00; ///<  Message code.
    const UShort REPORT_VELOCITY_VECTOR                           =   0xDB01; ///<  Message code.
}

#endif
/*  End of File */
