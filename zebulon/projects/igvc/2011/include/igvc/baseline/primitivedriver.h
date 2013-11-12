////////////////////////////////////////////////////////////////////////////////////
///
///  \file primitivedriver.h
///  \brief This file contains the definition of the PrimitiveDriver class, used
///         as an interface for basic platform mobility.
///
///  <br>Author(s): David Adams
///  <br>Created: 2011
///  <br>Copyright (c) 2011
///  <br>Robotics Club at UCF
///  <br>University of Central Florida (UCF)
///  <br>Email: dadams@ist.ucf.edu
///  <br>All rights reserved.
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
#ifndef __ZEBULON_GROUND_PRIMITIVEDRIVER__H
#define __ZEBULON_GROUND_PRIMITIVEDRIVER__H

#include <jaus/mobility/drivers/primitivedriver.h>
#include <mixer/mixer.h>

namespace Zebulon
{
    namespace IGVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class PrimitiveDriver
        ///   \brief This class is the primitive driver class that inheritis from the 
        ///          JAUS primitive driver, but overloads the SetWrenchEffort function
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class PrimitiveDriver: public JAUS::PrimitiveDriver
        {
        public:
            PrimitiveDriver();
            ~PrimitiveDriver();
            virtual void Shutdown();
            // Sets the wrench effort from a JAUS message.
            virtual bool SetWrenchEffort(const JAUS::SetWrenchEffort* command);
            // Called when the component transitions from Standby to Ready.
            virtual bool Resume();
            // Called when the component transitions to a Standby state.
            virtual bool Standby();
            // Called when the component is set into the Ready state.
            virtual bool Reset();
            // Calculated thrust for side of robot.
            double GetLeftThrust() const;
            // Calculated thrust for side of robot.
            double GetRightThrust() const;
        protected:
            CxUtils::Mutex mThrustMutex;          ///<  For thread protection of data.
            double mLeftThrust;                   ///<  Left side thrust.
            double mRightThrust;                  ///<  Right side thrust.
            Zebulon::Utility::Mixer mMixer;       ///<  Utility to perform mixing.
        };
    }
}
#endif