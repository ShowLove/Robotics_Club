////////////////////////////////////////////////////////////////////////////////////
///
/// \file encoder.cpp
/// \brief Data structure for storing encoder information.
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
#include <encoders/encoder.h>
#include <cxutils/math/cxmath.h>

using namespace Zebulon;
using namespace Utility;
using namespace Encoders;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Encoder::Encoder() : mRelativeFlag(true), mCount(0), mValuesPerRevolution(1), mShaftRadius(.01), mSamplePeriodMs(0)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Encoder::Encoder(const Encoder& encoder) : mRelativeFlag(true), mCount(0), mValuesPerRevolution(1), mShaftRadius(.01), mSamplePeriodMs(0)
{
    *this = encoder;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Encoder::~Encoder()
{
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all values.
///
////////////////////////////////////////////////////////////////////////////////////
void Encoder::Clear()
{
    mRelativeFlag = true;
    mCount = 0;
    mSamplePeriodMs = 0;
    mTimeStamp.Clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the encoder values to meters at a specific radius
///          from the motor shaft (like the endge of a wheel).
///
///   \param[in] radius Shaft/Wheel radius.
///   \param[in] ticksPerRevolution Number of encoder values in 1 revolution of
///                                 the motor shaft.
///
////////////////////////////////////////////////////////////////////////////////////
double Encoder::ToMeters(const double radius,
                         const long long int ticksPerRevolution) const
{
    return (mCount*radius*CxUtils::CX_TWO_PI)/ticksPerRevolution;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the encoder data to a linear velocity in m/s.  This would
///          equate to the velocity on the ground based on wheel movement.
///
///   For rotational velocity, see ToRotationalVelocity which is in rad/s.
///
///   \param[in] radius Shaft/Wheel radius.
///   \param[in] ticksPerRevolution Number of encoder values in 1 revolution of
///                                 the motor shaft.
///
////////////////////////////////////////////////////////////////////////////////////
double Encoder::ToLinearVelocity(const double radius,
                                 const long long int ticksPerRevolution) const
{
    return (mCount*radius*CxUtils::CX_TWO_PI)/(ticksPerRevolution*mSamplePeriodMs/1000.0);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the encoder data to a rotational velocity in radians/s.
///
///   \param[in] ticksPerRevolution Number of encoder values in 1 revolution of
///                                 the motor shaft.
///
////////////////////////////////////////////////////////////////////////////////////
double Encoder::ToRotationalVelocity(const long long int ticksPerRevolution) const
{
    return (mCount*CxUtils::CX_TWO_PI)/(ticksPerRevolution*mSamplePeriodMs/1000.0);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds encoder data together. TimeStamp is set to the less recent
///          time of the two to mark the beginning of samples.
///
////////////////////////////////////////////////////////////////////////////////////
Encoder Encoder::operator + (const Encoder& encoder) const
{
    Encoder result;
    result.mShaftRadius = this->mShaftRadius;
    result.mValuesPerRevolution = this->mValuesPerRevolution;
    result.mRelativeFlag = this->mRelativeFlag;
    if(result.mRelativeFlag)
    {
        result.mCount = mCount + encoder.mCount;
    }
    else
    {
        result.mCount = encoder.mCount;
    }
    result.mSamplePeriodMs = mSamplePeriodMs + encoder.mSamplePeriodMs;
    result.mTimeStamp = mTimeStamp < encoder.mTimeStamp ? mTimeStamp : encoder.mTimeStamp;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds encoder data together. TimeStamp is set to the more recent
///          time of the two.
///
////////////////////////////////////////////////////////////////////////////////////
Encoder& Encoder::operator += (const Encoder& encoder)
{
    return *this = *this + encoder;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Encoder& Encoder::operator=(const Encoder& encoder)
{
    if(this != &encoder)
    {
        mRelativeFlag = encoder.mRelativeFlag;
        mCount = encoder.mCount;
        mValuesPerRevolution = encoder.mValuesPerRevolution;
        mShaftRadius = encoder.mShaftRadius;
        mSamplePeriodMs = encoder.mSamplePeriodMs;
        mTimeStamp = encoder.mTimeStamp;
    }

    return *this;
}

/*  End of File */
