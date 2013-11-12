////////////////////////////////////////////////////////////////////////////////////
///
///  \file vehicle.h
///  \brief This file contains the Vehicle class used to store information about
///         subsystems (e.g. a Vehicle/Vehicle) that has been discovered.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 15 November 2009
///  <br>Copyright (c) 2009
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
#ifndef __JAUS_CORE_DISCOVERY_VEHICLE__H
#define __JAUS_CORE_DISCOVERY_VEHICLE__H

#include "jaus/core/service.h"
#include "jaus/core/discovery/subsystem.h"
#include <cxutils/math/coordinates.h>

namespace JAUS
{
    typedef CxUtils::Wgs Wgs;            ///<  Typedef to more easily reference Wgs data.
    typedef CxUtils::Point3D Attitude;   ///<  Typedef to more easily reference Point3D data as orientation.
    typedef CxUtils::Point3D Point3D;    ///<  Typedef to more easily reference Point3D data.

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Vehicle
    ///   \brief Describes a Vehicle Subsystem.  A Vehicle has position and 
    ///          orientation information.
    ///
    ///   Within the Core library, vehicle positions and orientations are not
    ///   set.  They are only updated when the Discovery Service is used in
    ///   conjunction with the Mobility Service Set library.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Vehicle : public Subsystem
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class PresenceVector
        ///   \brief This class contains bit masks for bitwise operations on the
        ///          presence vector for this message.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL PresenceVector : public JAUS::PresenceVector
        {
        public:
            const static UInt Position          = 0x0001;   ///< Presence of the Position field
            const static UInt Attitude          = 0x0002;   ///< Presence of the Attitude field
            const static UInt LinearVelocity    = 0x0004;   ///< Presence of the LinearVelocity field
            const static UShort TimeStamp       = 0x0008;   ///< Presence of the TimeStamp field
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Info
        ///   \brief Abstract class for setting additional information you want to
        ///          assign to a vehicle.  
        ///
        ///   Overload this classes methods, and then you can call the SetAdditionalInfo
        ///   methods of the Vehicle class to set custom virtual data.
        ///
        ///   Although this data structure can store information for position, attitude,
        ///   etc. this data is not collected by the Core library.  An example of a
        ///   service which does attempt to populate this information is the 
        ///   SubsystemCommand class of the JAUS++ Extras library.  That interface
        ///   uses the data from discovery to automatically create subscriptions to
        ///   Global Pose and Velocity data.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL Info
        {
        public:
            Info() {}
            virtual ~Info() {}
            virtual bool Copy(const Info* toCopy) = 0;
            virtual Info* Clone() const = 0;
        };
        typedef boost::shared_ptr<JAUS::Vehicle> Ptr;
        typedef std::map<UShort, boost::shared_ptr<JAUS::Vehicle> > Map;
        Vehicle();
        Vehicle(const Vehicle& vehicle);
        ~Vehicle();
        void Update(const Wgs& position,
                    const Attitude& attitude,
                    const Time& time);
        void Update(const Point3D& velocity,
                    const Time& time);
        virtual Subsystem* Clone() const;
        void SetAdditionalInfo(const Info* info);
        void DeleteAdditionalInfo();
        Info* GetAdditionalInfo();
        const Info* GetAdditionalInfo() const;
        Vehicle& operator=(const Vehicle& vehicle);
        static void DeleteVehicleMap(Vehicle::Map& list);
        static void CopyVehicleMap(const Vehicle::Map& src, Vehicle::Map& dest);
        UInt mPresenceVector;           ///<  Signals what type of vehicle data is valid. 
        Wgs mPosition;                  ///<  Position of Vehicle.
        Attitude mAttitude;             ///<  Attitude of Vehicle.
        Point3D mLinearVelocity;        ///<  Linear velocity of vehicles.
        int mAuthorityLevel;            ///<  Authority level needed for control of the Vehicle (-1 is not set).
    private:
        Info* mpInfo;                   ///<  Additional info for the vehicle.
    };
}

#endif
/*  End of File */
