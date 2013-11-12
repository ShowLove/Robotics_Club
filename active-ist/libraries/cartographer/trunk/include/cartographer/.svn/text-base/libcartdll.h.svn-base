////////////////////////////////////////////////////////////////////////////////////
///
///  \file libcartdll.h
///  \brief Main include file which declares some preprocessor definitions
///         for generation of shared libraries (DLL's).
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 14 November 2008
///  <br>Copyright (c) 2008
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
#ifndef __LIB_CARTOGRAPHER_DLL__H
#define __LIB_CARTOGRAPHER_DLL__H

#ifndef CARTOGRAPHER_DLL
    #ifdef WIN32
        #ifdef ACTIVE_STATIC
            #define CARTOGRAPHER_DLL
        #else
            #define CARTOGRAPHER_DLL  __declspec(dllimport)
        #endif
    #else
        #define CARTOGRAPHER_DLL
    #endif
#endif

#include <cxutils/math/cxmath.h>
#include <cxutils/mutex.h>
#include <cxutils/packet.h>

////////////////////////////////////////////////////////////////////////////////////
///
///   \namespace Cartographer
///   \brief Cartographer is the namespace for all interfaces and structures 
///          within the Cartographer Robot Navigation Library.
///
////////////////////////////////////////////////////////////////////////////////////
namespace Cartographer
{
    typedef CxUtils::Point3D Point3D;
    typedef CxUtils::Segment3D Segment3D;
    typedef CxUtils::Time Time;
    typedef CxUtils::Point3D Orientation;
    typedef CxUtils::Mutex Mutex;
    typedef CxUtils::Packet Packet;
}

#ifdef USE_OPENCV
#include <opencv/cv.h>
#include <opencv/highgui.h>
#endif

#endif
/*  End of File */
