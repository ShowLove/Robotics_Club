/////////////////////////////////////////////////////////////////////////////////////
///
/// \file compassfactory.cpp
/// \brief Generic class for creation of compasses
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
#include "compass/compassfactory.h"

using namespace std;
using namespace Zebulon;
using namespace Compass;

const std::string CompassFactory::CoralAHRS = "CoralAHRS";
const std::string CompassFactory::GX1 = "3DM-GX1";
const std::string CompassFactory::GX2 = "3DM-GX2";
const std::string CompassFactory::TCM250 = "TCM250";
const std::string CompassFactory::Inclinometer = "Inclinometer";


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates a new compass based on name
///
///  \param name of a compass to attach to
///
///  \return instance of a compass or NULL
///
////////////////////////////////////////////////////////////////////////////////////
GenericCompass *CompassFactory::CreateCompass(std::string name)
{
    //Do this fancier with statics
    if(name.compare(CoralAHRS)==0)
    {
        return new CoralAHRS::Interface();
    }
    else if(name.compare(GX1)==0)
    {
        return new MicroStrain::GX1();
    }
    else if(name.compare(GX2)==0)
    {
        return new MicroStrain::GX2();
    }
    else if(name.compare(TCM250)==0)
    {
        return new TCM::TCM250();
    }
    else if(name.compare(Inclinometer)==0)
    {
        return new INCLINOMETER::Inclinometer();
    }
    else
    {
        if(name == "GX1")
        {
            return new MicroStrain::GX1();
        }
        else if(name == "GX2")
        {
            return new MicroStrain::GX2();
        }
        cout << "Tried to create a compass that is not implemented...yet..." << endl;
        return NULL;
    }

}
