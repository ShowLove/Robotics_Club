/////////////////////////////////////////////////////////////////////////////////////
///
/// \file laserfactory.cpp
/// \brief Generic class for creation of lasers
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
#include "laser/laserfactory.h"
#include <sick/sick.h>
#include <hokuyo/hokuyo.h>
#include <hokuyourg/hokuyourg.h>

using namespace std;
using namespace Zebulon;
using namespace Laser;

const std::string LaserFactory::SICK = "SICK";
const std::string LaserFactory::Hokuyo = "Hokuyo";
const std::string LaserFactory::HokuyoURG = "HokuyoURG";

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates a new laser based on name
///
///  \param name of a laser to attach to
///
///  \return instance of a laser or NULL
///
////////////////////////////////////////////////////////////////////////////////////
GenericLaser *LaserFactory::CreateLaser(const std::string& name)
{
    //Do this fancier with statics
    if(name.compare(LaserFactory::SICK)==0)
    {
        return new SICK::Sick();
    }
#ifndef NO_WIN32_HOKUYO
    else if(name.compare(LaserFactory::Hokuyo)==0)
    {
        return new Laser::Hokuyo();
    }
#endif
    else if(name.compare(LaserFactory::HokuyoURG) == 0)
    {
        return new Laser::HokuyoURG();
    }
    else
    {
        cout << "Tried to create a laser that is not implemented...yet..." << endl;
        return NULL;
    }
}

/*  End of File */

