//////////////////////////////////////////////////////////////////////////////////////
///
/// \file hotsuit.h
/// \brief Mission identifies and and reports a 'hot' target (a card suit in this case)
///
/// Author(s): Andrew Watson<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:dbarber@ist.ucf.edu<br>
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
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _ZEBULON_SURFACE_HOTSUIT_MISSION_H
#define _ZEBULON_SURFACE_HOTSUIT_MISSION_H

#include "asvc/surface/missions/mission.h"
#include <cxutils/networking/tcpclient.h>
#include <cxutils/math/coordinates.h>
#include "surface/mapbuilder.h"

namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class Hotsuit
        ///  \brief Hotsuit mission of Roboboat 2012.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Hotsuit : public MapBuilder
        {
        public:
            static const std::string Name; ///<  Name of the mission.
            static const std::string SPADE;
            static const std::string HEART;
            static const std::string DIAMOND;
            static const std::string CLUB;
            enum States
            {
                ApproachTarget,
                FindSuit
            };
            Hotsuit();
            ~Hotsuit();
            virtual int Init();
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            int InitializeCom(const unsigned short mPort,
                           const CxUtils::IP4Address& mReportIP);
            void ShutdownCom();
            bool IsConnectedCom() const { return mTcp.IsValid(); }
            bool SendPing() const;
            bool SendLocation(const std::string& typeName,
                              CxUtils::Wgs& position) const;
        protected:
            virtual void LoadSettings();
        private:
            std::string mNextMission;                  ///<  The next mission to go to on exit.
            CxUtils::TcpClient mTcp;                   ///<  TCP connection.
            CxUtils::Wgs mPosition;                    ///<  Expected Decimal format position of hot target
            std::string mPortLeftCourse;               ///<  Port to use on left course
            std::string mPortRightCourse;              ///<  Port to use on right course
            double mReportPort;                        ///<  Port to report hot target
            std::string mReportIP;                     ///<  IPAddress to report hot target
            States mCurrentState;                      ///<  Current mission state

            double mDesiredHeadingDeg;
            double mDesiredThrust;
            double mDesiredLateral;
        };
    }
}


#endif
/* End of File */
