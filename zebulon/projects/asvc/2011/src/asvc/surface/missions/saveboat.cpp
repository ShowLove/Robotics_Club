//////////////////////////////////////////////////////////////////////////////////////
///
/// \file saveboat.cpp
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
#include "asvc/surface/missions/saveboat.h"

using namespace Zebulon;
using namespace ASVC;

const std::string SaveBoat::Name = "SaveBoat";

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
SaveBoat::SaveBoat()
{
    ID = Name;
    mNextMission = "";
    mAngleFilter = new Utility::AverageFilter(3);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SaveBoat::~SaveBoat()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the mission.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int SaveBoat::Init()
{
    Mission::Init();
    mXmlConf.GetVar("Surface.SaveBoat.NextMission@name", mNextMission);
    //mpGlobalInfo->SetInfo(GlobalInfo::StartingEasting, mStartPosition.mEasting);
    mStartPosition = mpGlobalInfo->GetPosition();
    mCurrentState = DriveSomewhere;
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Error Check method, does any required error checking.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int SaveBoat::ErrorCheck()
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Fetches data required for execution.
///
///  Gets start button state, and checks to see if GPS data is available.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int SaveBoat::FetchProcess()
{
    mXmlConf.GetVar("Surface.SaveBoat.DriveSomewhere@StartAngle", mStartAngle);
    mXmlConf.GetVar("Surface.SaveBoat.DriveSomewhere@TiltAngle", mTiltAngle);
    mXmlConf.GetVar("Surface.SaveBoat.DriveSomewhere@StartDistance", mStartDistance);
    mXmlConf.GetVar("Surface.SaveBoat.DriveSomewhere@StartSpeed", mStartSpeed);
    mXmlConf.GetVar("Surface.SaveBoat.SearchPattern@SearchTime", mSearchTime);
    mXmlConf.GetVar("Surface.SaveBoat.LineUp@LineUpAngleThreshold", mLineUpAngleThreshold);
    mXmlConf.GetVar("Surface.SaveBoat.LineUp@LineUpDistance", mLineUpDistance);
    mXmlConf.GetVar("Surface.SaveBoat.LineUp@DistancefromShoreThreshold", mDistancefromShoreThreshold);
    mXmlConf.GetVar("Surface.SaveBoat.LineUp@ShoreDistanceSF", mShoreDistanceSF);
    mXmlConf.GetVar("Surface.SaveBoat.LineUp@ShoreAngleRotateSF", mShoreAngleRotateSF);
    mXmlConf.GetVar("Surface.SaveBoat.Strafing@StrafeTime", mStrafeTime);
    mXmlConf.GetVar("Surface.SaveBoat.Strafing@CannonPower", mCannonPower);
    mXmlConf.GetVar("Surface.SaveBoat.Strafing@LateralPower", mLateralPower);
    mXmlConf.GetVar("Surface.SaveBoat.BackUp@BackupTime", mBackupTime);
    mXmlConf.GetVar("Surface.SaveBoat.BackUp@BackupSpeed", mBackupSpeed);
    
    mCurrPosition = mpGlobalInfo->GetPosition();
    mpGlobalInfo->GetInfo(AI::Names::Yaw,mCurrHeading);
    mpGlobalInfo->GetInfo(GlobalInfo::Laser,mLaserScan);
    
    //Process (Find angle)
    int scanSize=mLaserScan.size();
    if(scanSize==0)
    {
        std::cout << "SaveBoat: Laser Not Ready\n";
        mShoreDistance=0;
        mShoreAngle=0;
        return 0;
    }
    double centerDistance;
    double leftDistance;
    double rightDistance;
    
    centerDistance=(mLaserScan[scanSize/2].mX+mLaserScan[scanSize/2].mX+mLaserScan[scanSize/2].mX)/3;
    leftDistance=(mLaserScan[scanSize/2 - 50].mX);
    rightDistance=(mLaserScan[scanSize/2 + 50].mX);
    
    mShoreDistance = (leftDistance + rightDistance + centerDistance)/3;
    mShoreAngle = (leftDistance-rightDistance)*2;
    if(mShoreAngle > CxUtils::CxToRadians(60))
    {
        mShoreAngle = CxUtils::CxToRadians(60);
    }
    if(mShoreAngle < CxUtils::CxToRadians(-60))
    {
        mShoreAngle = CxUtils::CxToRadians(-60);
    }
        
    mAngleFilter->Add(mShoreAngle);
    mAngleFilter->Output(mShoreAngle);
    
    std::cout << "SaveBoat Fetch: "
              << "\nLaser Distance " << mShoreDistance
              << "\nLaser Angle " << CxUtils::CxToDegrees(mShoreAngle)
              << std::endl;
            
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief If there is an early exit condition, this method will handle it.
///
///  \return Mission::KeepRunning.
///
////////////////////////////////////////////////////////////////////////////////////
std::string SaveBoat::ExitEarly()
{
    return Mission::KeepRunning;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Executes the mission data.
///
///  \return Mission::KeepRunning.
///
////////////////////////////////////////////////////////////////////////////////////
std::string SaveBoat::Execute()
{
    // Save current starting position.

    double travelSpeed = 0;
    double heading = mpGlobalInfo->GetOrientation().mZ;
    double LateralSpeed=0;
    double tiltAngle = mTiltAngle;
    double cannonPower = 0;
    bool angleReady = false;
    bool distanceReady = false;
    
    std::cout << "=================================\n";
    std::cout << "SaveBoat\n";
    
    switch (mCurrentState)
    {
        case DriveSomewhere:
        {
            std::cout << "DriveSomewhere\n";
            double dist = sqrt(pow(mCurrPosition.mEasting-mStartPosition.mEasting,2) + 
                pow(mCurrPosition.mNorthing-mStartPosition.mNorthing,2));
            std::cout << "Distace Travelled: " << dist << std::endl;
            
            if(dist > mStartDistance || mShoreDistance < 6) //less than 3 meters from shore while driving
            {
                mCurrentState = LineUp;
            }
            else
            {
                travelSpeed = mStartSpeed;
                heading=CxUtils::CxToRadians(mStartAngle);
            }
        }
        break;
        
        case LineUp:
            std::cout << "Lineup\n";
            tiltAngle = mTiltAngle;
            cannonPower = mCannonPower;
            //control angle against shore
            if(fabs(mShoreAngle) > CxUtils::CxToRadians(mLineUpAngleThreshold))
            {
                heading = heading + mShoreAngle*mShoreAngleRotateSF;
                if(fabs(mShoreAngle) > (CxUtils::CxToRadians(mLineUpAngleThreshold))*1.2)
                {
                    angleReady = false;
                }
            }
            else
            {
                heading = heading;
                angleReady = true;
            }
            
            //control distance from shore
            travelSpeed = (mShoreDistance - mLineUpDistance)*mShoreDistanceSF;
            if(travelSpeed > 20)
            {
                travelSpeed = 20;
            }
            if(travelSpeed < -20)
            {
                travelSpeed = -20;
            }
            if(fabs(mShoreDistance - mLineUpDistance) > mDistancefromShoreThreshold)
            {
                
                distanceReady = false;
            }
            else
            {
                //travelSpeed = 0;
                distanceReady = true;
            }
            
            if(angleReady == true && distanceReady == true)
            {
                mStrafeHeading = heading;  //heading of boat once line-up so that the boat can maintain the heading for the duration of the strafe
                mCurrentState = Strafing;
            }
            std::cout << "Heading Correction: " << CxUtils::CxToDegrees(mShoreAngle*mShoreAngleRotateSF) << std::endl;
            std::cout << "Travel Speed: " << travelSpeed << std::endl;
            std::cout << "Angle Ready = " << angleReady << "  Distance Ready = " << distanceReady << std::endl;
            //std::cout << "Heading Correction: " << mShoreAngle*mShoreAngleRotateSF << std::endl;
            //std::cout << "Heading Correction: " << mShoreAngle*mShoreAngleRotateSF << std::endl;
        break;

        case SearchPattern:
            std::cout << "Search Pattern\n";
        break;
        
        case Strafing:
            std::cout << "Strafing\n";
            if(mDriveStopWatch.IsStarted() == false)
           {
                mDriveStopWatch.Initialize(mStrafeTime);
                mDriveStopWatch.Start();
            }
            if(mDriveStopWatch.IsFinished())
            {
                mCurrentState = BackUp;
            }
            heading = mStrafeHeading;
            travelSpeed = 0;
            LateralSpeed = mLateralPower;
            cannonPower = mCannonPower;
            if(fabs(mShoreDistance - mLineUpDistance) > mDistancefromShoreThreshold || fabs(mShoreAngle) > CxUtils::CxToRadians(mLineUpAngleThreshold))
            {
                mCurrentState = LineUp;
                LateralSpeed = mLateralPower/2;
            }

        break;
        
        case BackUp:
            std::cout << "Backup\n";
            if(mDriveStopWatch.IsStarted() == false)
            {
                mDriveStopWatch.Initialize(mBackupTime);
                mDriveStopWatch.Start();
            }
            if(mDriveStopWatch.IsFinished())
            {
                mCurrentState = Exit;
            }
            travelSpeed = mBackupSpeed;
        break;
        
        case Exit:
        
        break;
        
    }
    
    
    mpGlobalCommand->SetWaterCannonTilt(-tiltAngle);
    mpGlobalCommand->SetWaterCannon(cannonPower);
    mpGlobalCommand->SetForwardVelocityPercent(travelSpeed);
    mpGlobalCommand->SetLateralVelocityPercent(LateralSpeed);    
    mpGlobalCommand->SetYaw(heading);
    
    return Mission::KeepRunning;
}
/* End of File */
