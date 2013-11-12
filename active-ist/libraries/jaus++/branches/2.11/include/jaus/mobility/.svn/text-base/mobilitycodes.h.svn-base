////////////////////////////////////////////////////////////////////////////////////
///
///  \file mobilitycodes.h
///  \brief This file lists all message codes for messages in the JAUS Mobility
///         Service Set
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 22 November 2009
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
#ifndef _JAUS_MOBILITY_MESSAGE_CLASS_CODES_H
#define _JAUS_MOBILITY_MESSAGE_CLASS_CODES_H

#include "jaus/core/types.h"
#include "jaus/mobility/jausmobilitydll.h"
#include <cxutils/math/coordinates.h>
#include <string>

namespace JAUS
{
    typedef CxUtils::Wgs Wgs;           ///<  Typedef for WGS data.
    typedef CxUtils::Utm Utm;           ///<  Typedef for UTM data.
    typedef CxUtils::Point3D Point3D;   ///<  Typedef for Point3D data.

    // Command Messages
    const UShort SET_GLOBAL_POSE                                          =   0x0402;  ///<  Message code.
    const UShort SET_LOCAL_POSE                                           =   0x0403;  ///<  Message code.
    const UShort SET_WRENCH_EFFORT                                        =   0x0405;  ///<  Message code.
    const UShort SET_GLOBAL_VECTOR                                        =   0x0407;  ///<  Message code.
    const UShort SET_LOCAL_VECTOR                                         =   0x0408;  ///<  Message code.
    const UShort SET_TRAVEL_SPEED                                         =   0x040A;  ///<  Message code.
    const UShort SET_GLOBAL_WAYPOINT                                      =   0x040C;  ///<  Message code.
    const UShort SET_LOCAL_WAYPOINT                                       =   0x040D;  ///<  Message code.
    const UShort SET_GLOBAL_PATH_SEGMENT                                  =   0x040F;  ///<  Message code.
    const UShort SET_LOCAL_PATH_SEGMENT                                   =   0x0410;  ///<  Message Code;
    const UShort SET_GEOMAGNETIC_PROPERTY                                 =   0x0412;  ///<  Message code.
    const UShort SET_VELOCITY_COMMAND                                     =   0x0415;  ///<  Message code.
    const UShort SET_ACCELERATION_LIMIT                                   =   0x0416;  ///<  Message code.
    const UShort SET_ELEMENT                                              =   0x041A;  ///<  Message code.
    const UShort DELETE_ELEMENT                                           =   0x041B;  ///<  Message code.
    const UShort CONFIRM_ELEMENT_REQUEST                                  =   0x041C;  ///<  Message code.
    const UShort REJECT_ELEMENT_REQUEST                                   =   0x041D;  ///<  Message code.
    const UShort EXECUTE_LIST                                             =   0x041E;  ///<  Message code.
    
    // Query Messages.
    const UShort QUERY_GLOBAL_POSE                                        =   0x2402;  ///<  Message code.
    const UShort QUERY_LOCAL_POSE                                         =   0x2403;  ///<  Message code.
    const UShort QUERY_VELOCITY_STATE                                     =   0x2404;  ///<  Message code.
    const UShort QUERY_WRENCH_EFFORT                                      =   0x2405;  ///<  Message code.
    const UShort QUERY_GLOBAL_VECTOR                                      =   0x2407;  ///<  Message code.
    const UShort QUERY_LOCAL_VECTOR                                       =   0x2408;  ///<  Message code.
    const UShort QUERY_TRAVEL_SPEED                                       =   0x240A;  ///<  Message code.
    const UShort QUERY_GLOBAL_WAYPOINT                                    =   0x240C;  ///<  Message code.
    const UShort QUERY_LOCAL_WAYPOINT                                     =   0x240D;  ///<  Message code.
    const UShort QUERY_GLOBAL_PATH_SEGMENT                                =   0x240F;  ///<  Message code.
    const UShort QUERY_LOCAL_PATH_SEGMENT                                 =   0x2410;  ///<  Message code.
    const UShort QUERY_GEOMAGNETIC_PROPERTY                               =   0x2412;  ///<  Message code.
    const UShort QUERY_VELOCITY_COMMAND                                   =   0x2415;  ///<  Message code.
    const UShort QUERY_ACCELERATION_LIMIT                                 =   0x2416;  ///<  Message code.
    const UShort QUERY_ACCELERATION_STATE                                 =   0x2417;  ///<  Message code.
    const UShort QUERY_ELEMENT                                            =   0x241A;  ///<  Message code.
    const UShort QUERY_ELEMENT_LIST                                       =   0x241B;  ///<  Message code.
    const UShort QUERY_ELEMENT_COUNT                                      =   0x241C;  ///<  Message code.
    const UShort QUERY_ACTIVE_ELEMENT                                     =   0x241E;  ///<  Message code.

    // Inform Messages.
    const UShort REPORT_GLOBAL_POSE                                       =   0x4402;  ///<  Message code.
    const UShort REPORT_LOCAL_POSE                                        =   0x4403;  ///<  Message code.
    const UShort REPORT_VELOCITY_STATE                                    =   0x4404;  ///<  Message code.
    const UShort REPORT_WRENCH_EFFORT                                     =   0x4405;  ///<  Message code.
    const UShort REPORT_GLOBAL_VECTOR                                     =   0x4407;  ///<  Message code.
    const UShort REPORT_LOCAL_VECTOR                                      =   0x4408;  ///<  Message code.
    const UShort REPORT_TRAVEL_SPEED                                      =   0x440A;  ///<  Message code.
    const UShort REPORT_GLOBAL_WAYPOINT                                   =   0x440C;  ///<  Message code.
    const UShort REPORT_LOCAL_WAYPOINT                                    =   0x440D;  ///<  Message code.
    const UShort REPORT_GLOBAL_PATH_SEGMENT                               =   0x440F;  ///<  Message code.
    const UShort REPORT_LOCAL_PATH_SEGMENT                                =   0x4410;  ///<  Message code.
    const UShort REPORT_GEOMAGNETIC_PROPERTY                              =   0x4412;  ///<  Message code.
    const UShort REPORT_VELOCITY_COMMAND                                  =   0x4415;  ///<  Message code.
    const UShort REPORT_ACCELERATION_LIMIT                                =   0x4416;  ///<  Message code.
    const UShort REPORT_ACCELERATION_STATE                                =   0x4417;  ///<  Message code.
    const UShort REPORT_ELEMENT                                           =   0x441A;  ///<  Message code.
    const UShort REPORT_ELEMENT_LIST                                      =   0x441B;  ///<  Message code.
    const UShort REPORT_ELEMENT_COUNT                                     =   0x441C;  ///<  Message code.
    const UShort REPORT_ACTIVE_ELEMENT                                    =   0x441E;  ///<  Message code.
}

#endif
/*  End of File */
