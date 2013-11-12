//////////////////////////////////////////////////////////////////////////////////////
///
/// \file saveboat.h
/// \brief Mission 1.* Find boat on shore, shoot with water cannon
///
/// Author(s): Boat Team<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:ucfroboticsclubnews@gmail.com<br>
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
#ifndef _ZEBULON_SURFACE_SAVEBOAT_H
#define _ZEBULON_SURFACE_SAVEBOAT_H

#include "asvc/surface/missions/mission.h"
#include <statemachine/utility/debounce.h>
#include <statemachine/utility/stopwatch.h>
#include <filter/filter.h>

namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class SaveBoat
        ///  \brief First mission to always be run.  It waits until the start button
        ///  is pressed, then exits.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class SaveBoat : public Mission
        {
        public:
            static const std::string Name; ///<  Name of the mission.
            SaveBoat();
            ~SaveBoat();
            virtual int Init();
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
        private:
            std::string mNextMission;   ///< The next mission to go to on exit.
            
            CxUtils::Utm mStartPosition;///< The starting position of the vehicle.
            CxUtils::Utm mCurrPosition;///< The starting position of the vehicle.
            
            double mCurrHeading;
            CxUtils::Point3D::List mLaserScan;
            Utility::Filter *mAngleFilter;
            
            enum States
            {
                DriveSomewhere = 0,
                SearchPattern,
                LineUp,
                Strafing,
                BackUp,
                Exit
            };
            
            States mCurrentState;                      ///<  Current sub-state
            double mTiltAngle;
            
            AI::StopWatch mDriveStopWatch;           ///<  How long to wait in time for driving through.
            
            //Drive Somewhere
            double mStartAngle;
            double mStartDistance;
            double mTravelSpeed;                       ///<  Travel speed (percent of max).
            double mStartSpeed;
            
            //Search Time
            double mSearchTime;
            
            //Lineup
            double mLineUpAngleThreshold;
            double mLineUpDistance;
            double mDistancefromShoreThreshold;
            double mShoreDistanceSF;
            double mShoreAngleRotateSF;
            
            //Strafing
            double mStrafeHeading;
            double mStrafeTime;
            double mCannonPower;
            double mLateralPower;
            
            //Backup
            double mBackupTime;
            double mBackupSpeed;
            
            //Shore
            double mShoreDistance; //distance from shore
            double mShoreAngle;    //angle from shore
        };
    }
}


#endif //SAVEBOAT_H
/* End of File */
