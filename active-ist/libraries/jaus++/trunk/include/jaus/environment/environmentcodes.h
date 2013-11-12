////////////////////////////////////////////////////////////////////////////////////
///
///  \file environmentcodes.h
///  \brief Header file listing all message codes for Environment Sensing.
///
///  Author(s): Daniel Barber
///  Created: 11 February 2012
///  Copyright (c) 2012
///  Applied Cognition and Training in Immersive Virtual Environments
///  (ACTIVE) Laboratory
///  Institute for Simulation and Training (IST)
///  University of Central Florida (UCF)
///  Email: dbarber@ist.ucf.edu
///  Web:  http://active.ist.ucf.edu
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
#ifndef _JAUS_ENVIRONMENT_SENSING_MESSAGE_CLASS_CODES_H
#define _JAUS_ENVIRONMENT_SENSING_MESSAGE_CLASS_CODES_H

#include "jaus/core/types.h"
#include "jaus/environment/jausenvironmentdll.h"
#include "jaus/mobility/mobilitycodes.h"
#include <string>

namespace JAUS
{
    // Command Messages
    const UShort CONFIRM_SENSOR_CONFIGURATION                          =   0x0801;  ///<  Message code.
    const UShort SET_RANGE_SENSOR_CONFIGURATION                        =   0x0802;  ///<  Message code.
    const UShort SET_VISUAL_SENSOR_CONFIGURATION                       =   0x0803;  ///<  Message code.
    const UShort SET_DIGITAL_VIDEO_SENSOR_CONFIGURATION                =   0x0804;  ///<  Message code.
    const UShort CONTROL_DIGITAL_VIDEO_SENSOR_STREAM                   =   0x0805;  ///<  Message code.
    const UShort SET_ANALOG_VIDEO_SENSOR_CONFIGURATION                 =   0x0806;  ///<  Message code.
    const UShort SET_STILL_IMAGE_SENSOR_CONFIGURATION                  =   0x0807;  ///<  Message code.
    
    // Query Messages.
    const UShort QUERY_RANGE_SENSOR_CAPABILITIES                       =   0x2801;  ///<  Message code.
    const UShort QUERY_RANGE_SENSOR_CONFIGURATION                      =   0x2802;  ///<  Message code.
    const UShort QUERY_RANGE_SENSOR_DATA                               =   0x2803;  ///<  Message code.
    const UShort QUERY_RANGE_SENSOR_COMPRESSED_DATA                    =   0x2804;  ///<  Message code.
    const UShort QUERY_SENSOR_GEOMETRIC_PROPERTIES                     =   0x2805;  ///<  Message code.
    const UShort QUERY_VISUAL_SENSOR_CAPABILITIES                      =   0x2806;  ///<  Message code.
    const UShort QUERY_VISUAL_SENSOR_CONFIGURATION                     =   0x2807;  ///<  Message code.
    const UShort QUERY_DIGITAL_VIDEO_SENSOR_CAPABILITIES               =   0x2808;  ///<  Message code.
    const UShort QUERY_DIGITAL_VIDEO_SENSOR_CONFIGURATION              =   0x2809;  ///<  Message code.
    const UShort QUERY_ANALOG_VIDEO_SENSOR_CAPABILITIES                =   0x2810;  ///<  Message code.
    const UShort QUERY_ANALOG_VIDEO_SENSOR_CONFIGURATION               =   0x2811;  ///<  Message code.
    const UShort QUERY_STILL_IMAGE_SENSOR_CAPABILITIES                 =   0x2812;  ///<  Message code.
    const UShort QUERY_STILL_IMAGE_SENSOR_CONFIGURATION                =   0x2813;  ///<  Message code.
    const UShort QUERY_STILL_IMAGE_DATA                                =   0x2814;  ///<  Message code.

    // Inform Messages.
    const UShort REPORT_RANGE_SENSOR_CAPABILITIES                      =   0x4801;  ///<  Message code.
    const UShort REPORT_RANGE_SENSOR_CONFIGURATION                     =   0x4802;  ///<  Message code.
    const UShort REPORT_RANGE_SENSOR_DATA                              =   0x4803;  ///<  Message code.
    const UShort REPORT_RANGE_SENSOR_COMPRESSED_DATA                   =   0x4804;  ///<  Message code.
    const UShort REPORT_SENSOR_GEOMETRIC_PROPERTIES                    =   0x4805;  ///<  Message code.
    const UShort REPORT_VISUAL_SENSOR_CAPABILITIES                     =   0x4806;  ///<  Message code.
    const UShort REPORT_VISUAL_SENSOR_CONFIGURATION                    =   0x4807;  ///<  Message code.
    const UShort REPORT_DIGITAL_VIDEO_SENSOR_CAPABILITIES              =   0x4808;  ///<  Message code.
    const UShort REPORT_DIGITAL_VIDEO_SENSOR_CONFIGURATION             =   0x4809;  ///<  Message code.
    const UShort REPORT_ANALOG_VIDEO_SENSOR_CAPABILITIES               =   0x4810;  ///<  Message code.
    const UShort REPORT_ANALOG_VIDEO_SENSOR_CONFIGURATION              =   0x4811;  ///<  Message code.
    const UShort REPORT_STILL_IMAGE_SENSOR_CAPABILITIES                =   0x4812;  ///<  Message code.
    const UShort REPORT_STILL_IMAGE_SENSOR_CONFIGURATION               =   0x4813;  ///<  Message code.
    const UShort REPORT_STILL_IMAGE_DATA                               =   0x4814;  ///<  Message code.
}

#endif
/*  End of File */
