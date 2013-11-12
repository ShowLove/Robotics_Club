/////////////////////////////////////////////////////////////////////////////
///
/// \file factory.h
/// \brief Contains all messages for the GX3
///
/// Author(s): Gary Stein<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
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
////////////////////////////////////////////////////////////////////////////////////
#ifndef __ZEBULON_GX3_FACTORY__H
#define __ZEBULON_GX3_FACTORY__H

#include <string>
#include <cxutils/packet.h>
#include <map>
#include <cxutils/math/cxmath.h>
#include <iostream>

#include "gx3/message.h"
#include "gx3/basecommandset/ping.h"

#include "gx3/ahrsdataset/eulerangles.h"
#include "gx3/ahrsdataset/scaledaccelerometervector.h"
#include "gx3/ahrsdataset/scaledgyrovector.h"
#include "gx3/ahrsdataset/deltavelocityvector.h"

#include "gx3/gpsdataset/llhposition.h"
#include "gx3/gpsdataset/nedvelocity.h"

#include "gx3/navdataset/estimatedangularrate.h"
#include "gx3/navdataset/estimatedlinearacceleration.h"
#include "gx3/navdataset/estimatedllhposition.h"
#include "gx3/navdataset/estimatednedvelocity.h"
#include "gx3/navdataset/estimatedorientationeulerangles.h"
#include "gx3/navdataset/filterstatus.h"


////////////////////////////////////////////////////////////////////////////////////
///
///  \namespace Zebulon
///  \brief Namespace for all software developed by the Robotics Laboratory and
///         Club at the University of Central Florida.
///
////////////////////////////////////////////////////////////////////////////////////
namespace Zebulon
{
    namespace INS
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \namespace MicroStrain 
        ///  \brief MicroStrain sensors, etc. namespace for the MicroStrain library.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        namespace MicroStrain
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class Factory
            ///  \brief Factory class for looking up all gx3 messages
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class Factory
            {
                public:
                    static std::map<unsigned short,Message*> AllMessages;
                    //Loads all messages to a map for later cloning in a factory
                    static void InitializeMessages()
                    {
                        //Add to factory
                        AllMessages[Message::Ping]=new Ping(Message::Ping,Ping::Length);

                        AllMessages[Message::EulerAngles]=new EulerAngles(Message::EulerAngles,EulerAngles::Length);
                        AllMessages[Message::ScaledAccelerometerVector]=new ScaledAccelerometerVector(Message::ScaledAccelerometerVector,ScaledAccelerometerVector::Length);
                        AllMessages[Message::ScaledGyroVector]=new ScaledGyroVector(Message::ScaledGyroVector,ScaledGyroVector::Length);
                        AllMessages[Message::DeltaVelocityVector]=new DeltaVelocityVector(Message::DeltaVelocityVector,DeltaVelocityVector::Length);

                        AllMessages[Message::LLHPosition]=new LLHPosition(Message::LLHPosition,LLHPosition::Length);
                        AllMessages[Message::NEDVelocity]=new NEDVelocity(Message::NEDVelocity,NEDVelocity::Length);

                        AllMessages[Message::EstimatedAngularRate]=new EstimatedAngularRate(Message::EstimatedAngularRate,EstimatedAngularRate::Length);
                        AllMessages[Message::EstimatedLinearAcceleration]=new EstimatedLinearAcceleration(Message::EstimatedLinearAcceleration,EstimatedLinearAcceleration::Length);
                        AllMessages[Message::EstimatedLLHPosition]=new EstimatedLLHPosition(Message::EstimatedLLHPosition,EstimatedLLHPosition::Length);
                        AllMessages[Message::EstimatedNEDVelocity]=new EstimatedNEDVelocity(Message::EstimatedNEDVelocity,EstimatedNEDVelocity::Length);
                        AllMessages[Message::EstimatedOrientationEulerAngles]=new EstimatedOrientationEulerAngles(Message::EstimatedOrientationEulerAngles,EstimatedOrientationEulerAngles::Length);
                        AllMessages[Message::FilterStatus]=new FilterStatus(Message::FilterStatus,FilterStatus::Length);


                    }

                    //looks up if a message exists and clones it
                    static Message *GetMessage(unsigned short type)
                    {
                        std::map<unsigned short,Message*>::iterator i=AllMessages.find(type);
                        if(i!=AllMessages.end())
                        {
                            return (i->second->Clone());
                        }
                        return NULL;
                    }

            };
            std::map<unsigned short,Message*> Factory::AllMessages;
        }
    }
}


#endif
/*  End of file */
