////////////////////////////////////////////////////////////////////////////////////
///
///  \file jausmobilitydll.h
///  \brief Header file for defining library/DLL export options for the
///   JAUS++ movility library.  All JAUS++ Mobility Library header files
///   must include this file to create DLL's correctly in 
///   Windows.
///
///  Author(s): Daniel Barber
///  Created: 22 November 2009
///  Copyright (c) 2009
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
#ifndef __JAUS_MOBILITY_LIBRARY_DLL__H
#define __JAUS_MOBILITY_LIBRARY_DLL__H

//////////////////////////////////////////////////////////////////////////////////// 
///  
///  \def JAUS_MOBILITY_DLL
///  \brief Pre-processor definition needed to export JAUS library to 
///  DLL's in Windows.
///
//////////////////////////////////////////////////////////////////////////////////// 
#ifdef WIN32
    #ifdef JAUS_MOBILITY_DLL_EXPORT
        #define JAUS_MOBILITY_DLL __declspec(dllexport)
        #pragma warning( disable : 4251)
    #else
        #define JAUS_MOBILITY_DLL __declspec(dllimport)
        #pragma warning( disable : 4251)
        #ifdef _DEBUG
            #pragma comment(lib, "libjausmobility_d.lib" )
        #else
            #pragma comment(lib, "libjausmobility.lib" )
        #endif
    #endif
#else
#define JAUS_MOBILITY_DLL
#endif

#endif
/*  End of File */
