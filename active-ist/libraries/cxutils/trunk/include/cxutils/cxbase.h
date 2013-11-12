////////////////////////////////////////////////////////////////////////////////////
///
///  \file cxbase.h
///  \brief Main include file that all CxUtils files must include.  This file
///  include definitions for some constants used at compile time.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 12 April 2009
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
#ifndef __CXUTILS_BASE_H
#define __CXUTILS_BASE_H


////////////////////////////////////////////////////////////////////////////////////
///
///   \def CX_UTILS_DLL
///   \brief Pre-processor definition for export/import of DLL's in Windows for
///          CxUtils library.
///
////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
    #pragma warning(disable: 4251)
    #ifdef CX_UTILS_DLL_EXPORTS
        #define CX_UTILS_DLL __declspec(dllexport)
    #else
        #define CX_UTILS_DLL __declspec(dllimport)
        #ifdef _DEBUG
            #pragma comment( lib, "libcxutils_d.lib" )
        #else
            #pragma comment( lib, "libcxutils.lib" )
        #endif
    #endif
#else
#define CX_UTILS_DLL
#endif

#ifndef _MSC_VER
#define MINGW
#endif


////////////////////////////////////////////////////////////////////////////////////
///
///   \def NULL
///   \brief 0 Value for pointers.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef NULL
#define NULL 0
#endif

#include "cxutils/time.h"

#endif

/*  End of File */
