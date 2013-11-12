////////////////////////////////////////////////////////////////////////////////////
///
/// \file libmicrostraindll.h
/// \brief Header file for defining library/DLL export options for the
///  this library.
///
/// Author(s): Daniel Barber<br>
/// Created: 5/27/2008<br>
/// Last Modified: 12/10/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu <br>
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
#ifndef __LIB_MICROSTRAIN_LIBRARY_DLL__H
#define __LIB_MICROSTRAIN_LIBRARY_DLL__H

//////////////////////////////////////////////////////////////////////////////////// 
///  
///  \def ZEB_MICROSTRAIN_DLL
///  \brief Pre-processor definition used for exporting/importing DLL for library.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef ZEB_MICROSTRAIN_DLL
    #ifdef WIN32
        #define ZEB_MICROSTRAIN_DLL __declspec(dllimport)
    #else
        #define ZEB_MICROSTRAIN_DLL
    #endif
#endif

#endif

/*  End of File */
