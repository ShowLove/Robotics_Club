////////////////////////////////////////////////////////////////////////////////////
///
/// \file laneline.h
/// \brief Class extends Cartographer::Objects::Line, mainly to modify the way it
///         merges identical lines
///
/// Author(s): Michael Scherer<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@knights.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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

#ifndef __ZEBULON_GROUND_LANELINE_H__
#define __ZEBULON_GROUND_LANELINE_H__

#include <cartographer/objectmap.h>
#include <cartographer/objects/line.h>
#include <cxutils/mutex.h>
#include <stdio.h>

namespace Zebulon
{
    namespace IGVC
    {
        class LaneLine : public Cartographer::Objects::Line
        {
        public:
            const static std::string Name;
            LaneLine(const unsigned int sourceID = 0);
            LaneLine(const Cartographer::Segment3D& line, const unsigned int sourceID = 0);
            LaneLine(const LaneLine& line);

            virtual bool IsSameAs(const Cartographer::Object* object,
                                  const double allowedError = 0.01) const;
            LaneLine& operator=(const LaneLine& line);
        };
    }
}

#endif // __ZEBULON_GROUND_LANELINE_H__
