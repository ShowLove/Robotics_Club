////////////////////////////////////////////////////////////////////////////////////
///
///  \file rangesensorconfig.cpp
///  \brief This file contains the implementation of a JAUS structure.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 8 March 2010
///  <br>Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
#include "jaus/extras/rangesensor/rangesensorconfig.h"
#include <cxutils/math/cxmath.h>

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
RangeSensorConfig::RangeSensorConfig()
{
    mID = 0;
    mMaxRange = 9.0;
    mMinRange = 0.0;
    mAngleIncrement = 0.5;
    mScanAngle = CxUtils::CX_PI;
    mUnitType = MM;
    mName = "Range Sensor";
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
RangeSensorConfig::RangeSensorConfig(const RangeSensorConfig& config)
{
    mID = 0;
    mMaxRange = 9.0;
    mMinRange = 0.0;
    mAngleIncrement = 0.5;
    mScanAngle = CxUtils::CX_PI;
    mUnitType = MM;
    *this = config;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
RangeSensorConfig::~RangeSensorConfig()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
RangeSensorConfig& RangeSensorConfig::operator=(const RangeSensorConfig& config)
{
    mID = config.mID;
    mMaxRange = config.mMaxRange;
    mMinRange = config.mMinRange;
    mAngleIncrement = config.mAngleIncrement;
    mScanAngle = config.mScanAngle;
    mUnitType = config.mUnitType;
    mName = config.mName;
    return *this;
}


/*  End of File */
