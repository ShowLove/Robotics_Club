////////////////////////////////////////////////////////////////////////////////////
///
///  \file vehicle.cpp
///  \brief This file contains the Vehicle class used to store information about
///         subsystems (e.g. a Vehicle/Vehicle) that has been discovered.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 15 November 2009
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
#include "jaus/core/discovery/vehicle.h"

using namespace JAUS;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Vehicle::Vehicle() { mAuthorityLevel = -1; mpInfo = NULL; mPresenceVector = 0; }

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Vehicle::Vehicle(const Vehicle& vehicle) : Subsystem() 
{ 
    mpInfo = NULL; *this = vehicle; 
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Vehicle::~Vehicle() 
{ 
    if(mpInfo) { delete mpInfo; } 
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to set position, attitude, and time on one line.
///
////////////////////////////////////////////////////////////////////////////////////
void Vehicle::Update(const Wgs& position,
                     const Attitude& attitude,
                     const Time& time)
{
    mPosition = position;
    mAttitude = attitude;
    mUpdateTime = time;
    mPresenceVector |= PresenceVector::Position;
    mPresenceVector |= PresenceVector::Attitude;
    mPresenceVector |= PresenceVector::TimeStamp;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to set velocity info with a single line.
///
////////////////////////////////////////////////////////////////////////////////////
void Vehicle::Update(const Point3D& linearVelocity,
                     const Time& time)
{
    mLinearVelocity = linearVelocity;
    mUpdateTime = time;
    mPresenceVector |= PresenceVector::LinearVelocity;
    mPresenceVector |= PresenceVector::TimeStamp;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clones the data.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem* Vehicle::Clone() const { return new Vehicle(*this); }


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to set additional 3rd party data into the data structure.
///          This can be any type of data you want to add based on a derived
///          class called Info.
///
////////////////////////////////////////////////////////////////////////////////////
void Vehicle::SetAdditionalInfo(const Vehicle::Info* info)
{
    if(mpInfo == NULL)
    {
        mpInfo = info->Clone();
    }
    else
    {
        mpInfo->Copy(info);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes additinal 3rd party data.
///
////////////////////////////////////////////////////////////////////////////////////
void Vehicle::DeleteAdditionalInfo()
{
    if(mpInfo)
    {
        delete mpInfo;
    }
    mpInfo = NULL;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to custom data.
///
////////////////////////////////////////////////////////////////////////////////////
Vehicle::Info* Vehicle::GetAdditionalInfo() { return mpInfo; }


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to custom data.
///
////////////////////////////////////////////////////////////////////////////////////
const Vehicle::Info* Vehicle::GetAdditionalInfo() const { return mpInfo; }


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Vehicle& Vehicle::operator=(const Vehicle& vehicle)
{
    if(this != &vehicle)
    {
        *((Subsystem *)this) = *((Subsystem *)&vehicle);
        mPosition = vehicle.mPosition;
        mAttitude = vehicle.mAttitude;
        mAuthorityLevel = vehicle.mAuthorityLevel;
        mLinearVelocity = vehicle.mLinearVelocity;
        mPresenceVector = vehicle.mPresenceVector;
        if(vehicle.mpInfo)
        {
            if(mpInfo && mpInfo->Copy(vehicle.mpInfo) == false)
            {
                DeleteAdditionalInfo();
                SetAdditionalInfo(vehicle.mpInfo);
            }
            else
            {
                SetAdditionalInfo(vehicle.mpInfo);
            }
        }
        else if(mpInfo && !vehicle.mpInfo)
        {
            DeleteAdditionalInfo();
        }
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes all data in the list.
///
////////////////////////////////////////////////////////////////////////////////////
void Vehicle::DeleteVehicleMap(Vehicle::Map& list)
{
    list.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copies the data.
///
////////////////////////////////////////////////////////////////////////////////////
void Vehicle::CopyVehicleMap(const Vehicle::Map& src, Vehicle::Map& dest)
{
    Map::const_iterator s;
    DeleteVehicleMap(dest);
    for(s = src.begin();
        s != src.end();
        s++)
    {
        dest[s->first].reset((Vehicle *)s->second->Clone());
    }
}


/*  End of File */
