/////////////////////////////////////////////////////////////////////////////////////
///
/// \file laserfactory.h
/// \brief Generic Laser factory that allows you to make lasers
///
/// Author(s): Gary Stein<br>
/// Created: 2/15/2010<br>
/// Last Modified: 2/15/2010<br>
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
#ifndef ZEBULON_LASER_FACTORY_H
#define ZEBULON_LASER_FACTORY_H
#include <iostream>

#include <string.h>
#include <vector>
#include <set>

#include <laser/laser.h>

namespace Zebulon
{
    namespace Laser
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class LaserFactory
        ///   \brief LaserFactory static class that produces lasers
        ///
        ///   This class is part of the zebsensors library.  This library
        ///   contains interfaces for various sensors used on robots. Such
        ///   sensors include SICK, HOKUYO
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_LASER_DLL LaserFactory
        {
            public:
                const static std::string SICK;
                const static std::string Hokuyo;
                const static std::string HokuyoURG;   ///<  Library for ALL Hokuyo Devices.
                // Creates lasers by string name.
                static GenericLaser *CreateLaser(const std::string& name);
        };

        typedef LaserFactory Factory;
    }
}

#endif
