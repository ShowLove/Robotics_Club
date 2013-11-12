////////////////////////////////////////////////////////////////////////////////////
///
/// \file rigidbody.h
/// \brief Data structure for calculating linear and rotational velocities
///        of a rigid body from encoder data.
///
/// Author(s): Daniel Barber<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF)<br>
/// Email: dbarber@ist.ucf.edu<br>
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
#ifndef __ZEBULON_UTILITY_ENCODERS_RIGID_BODY_H
#define __ZEBULON_UTILITY_ENCODERS_RIGID_BODY_H

#include "encoders/encoder.h"

namespace Zebulon
{
    namespace Utility
    {
        namespace Encoders
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class RigidBody
            ///   \brief Simple rigid body data strucutre for calculation of linear and
            ///          rotational velocities using relative encoder data.
            ///
            ///   This class is helpful for ground robots with wheel encoders where you
            ///   want to use them to calculate motion in 2D space.
            ///
            ///   This simple model assumes body centroid is at 0,0.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_ENCODERS_DLL RigidBody
            {
            public:
                RigidBody();
                RigidBody(const RigidBody& encoder);
                virtual ~RigidBody();
                // Returns true if data has been added, but Update hasn't been called.
                bool IsDirty() const { return mDirtyFlag; }
                // Update relative encoder values for left side of platform.
                void UpdateLeftEncoder(const unsigned int encoderID,
                                       const Encoder& encoderData);
                // Update relative encoder values for right side of platform.
                void UpdateRightEncoder(const unsigned int encoderID,
                                        const Encoder& encoderData);
                // Set platform width.
                void SetWidth(const double width) { mWidth = fabs(width); }
                // Clears all data.
                void Reset();
                // Update calculations using current data.
                bool Update();
                // Get width value being used.
                double GetWidth() const { return mWidth; }
                // Gets the linear velocity (m/s) based previous call to Update method.
                double GetLinearVelocity() const;
                // Gets the rotational velocity (rad/s) based previous call to Update method.
                double GetRotationalVelocity() const;
                // Gets the change in meters for left and right side of vehicle.
                void GetPositionChange(double& left, double& right) { left = mLeftSideDist; right = mRightSideDist; }
                // Gets the number of times Update has been called.
                unsigned int GetUpdateCount() const { return mUpdateCounter; }
                // Gets the number of encoders added to system.
                unsigned int GetEncoderCount() const { return (unsigned int)(mLeftSide.size() + mRightSide.size()); }
                // Sets equal to.
                RigidBody& operator=(const RigidBody& encoder);
            protected:
                bool mDirtyFlag;                ///<  If true, new data has been added but not used in calculations yet.
                Encoder::Map mLeftSide;         ///<  Encoders on the left side of the body.
                Encoder::Map mRightSide;        ///<  Encoders on the right side of the body.
                double mWidth;                  ///<  Width of rigid body in meters (between left and right side).
                unsigned int mUpdateCounter;    ///<  Keeps track of number of calls to update method.
                double mLeftSideDist;           ///<  Left side distance change in meters.
                double mRightSideDist;          ///<  Right side distance change in meters.
                double mLeftSideVelocity;       ///<  Left side velocity in m/s.
                double mRightSideVelocity;      ///<  Right side velocity in m/s.
                CxUtils::Time mUpdateTime;      ///<  The last time update was called.
                double mSamplePeriodSeconds;    ///<  Sampling period in ms.
            };
        }
    }
}
#endif  //__ZEBULON_UTILITY_ENCODER

/* End of File */
