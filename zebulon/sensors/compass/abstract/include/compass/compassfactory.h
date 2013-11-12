/////////////////////////////////////////////////////////////////////////////////////
///
/// \file compassfactory.h
/// \brief Generic compass factory that allows you to make compasses
///
/// Author(s): Gary Stein<br>
/// Created: 2/6/2010<br>
/// Last Modified: 2/6/2010<br>
/// Copyright (c) 2010<br>
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
/////////////////////////////////////////////////////////////////////////////////////
#ifndef ZEBULON_COMPASS_FACTORY_H
#define ZEBULON_COMPASS_FACTORY_H
#include <iostream>

#include <string.h>
#include <vector>
#include <set>

#include <compass/compass.h>
#include <microstrain/gx1.h>
#include <microstrain/gx2.h>
#include <tcm250/tcm250.h>
#include <coralahrs/interface.h>
#include <inclinometer/inclinometer.h>

namespace Zebulon
{
    namespace Compass
    {
        class ZEB_COMPASS_DLL CompassFactory
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class CompassFactory
            ///   \brief CompassFactory static class that produces compasses
            ///
            ///   This class is part of the zebsensors library.  This library
            ///   contains interfaces for various sensors used on robots. Such
            ///   sensors include TCM250, DGPS, SICK Laser, etc.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            public:
                const static std::string CoralAHRS;
                const static std::string GX1;
                const static std::string GX2;
                const static std::string TCM250;
                const static std::string Inclinometer;
                
                //creates a compass, could be more generic
                static GenericCompass *CreateCompass(std::string name);
        };
        typedef CompassFactory Factory;
    }
}

#endif
