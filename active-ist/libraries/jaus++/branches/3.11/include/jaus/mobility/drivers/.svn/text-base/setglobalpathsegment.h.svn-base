////////////////////////////////////////////////////////////////////////////////////
///
///  \file setglobalpathsegment.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 1 December 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: bsun@ist.ucf.edu
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
#ifndef __JAUS_MOBILITY_SET_GLOBAL_PATH_SEGMENT__H
#define __JAUS_MOBILITY_SET_GLOBAL_PATH_SEGMENT__H

#include "jaus/core/message.h"
#include "jaus/mobility/mobilitycodes.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class SetGlobalPathSegment
    ///   \brief This message is used to set the path segment data on the global
    ///          coordinate system.  A global path segment is defined in this message
    ///          three points, P_0, P_1, and P_2 and a weighting factor.  For the first
    ///          path segment, i.e. the first element in a list of path segments, P_0
    ///          is assumed to be the current location of the platform as defined by
    ///          Report Global Pose.  For each successive path segment, i.e. where the
    ///          path segment number is greater than zero, P_0 is equal to the
    ///          previous path segment's P_2.  Therefore, for each message, only
    ///          P_1, P_2, and a weighting factor must be set in order to define
    ///          a path segment.  Each point is defined in the Global Coordinate System
    ///          by setting its Lattitude, Longitude, and Altitude, as specified by
    ///          the WGS 84 standard.  Both the Lattitude and Longitude are required
    ///          fields, but the Altitude field is optional.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL SetGlobalPathSegment : public Message
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class PresenceVector
        ///   \brief This class contains bit masks for bitwise operations on the
        ///          presence vector for this message.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_MOBILITY_DLL PresenceVector : public JAUS::PresenceVector
        {
        public:
            const static Byte P1Altitude = 0x01;
            const static Byte P2Altitude = 0x02;
            const static Byte PathTolerance = 0x04;
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Limits
        ///   \brief Contains constants for limit values of data members of class.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_MOBILITY_DLL Limits : public JAUS::Limits
        {
        public:
            const static double MinLatitude;            ///<  Minimum latitude. (-90)
            const static double MaxLatitude;            ///<  Maximum latitude. (90)
            const static double MinLongitude;           ///<  Minimum longitude. (-180)
            const static double MaxLongitude;           ///<  Maximum longitude. (180)
            const static double MinElevation;           ///<  Minimum altitude. (-10000)
            const static double MaxElevation;           ///<  Maximum altitude. (35000)
            const static double MinWeightingFactor;     ///<  Minimum weighting factor (0)
            const static double MaxWeightingFactor;     ///<  Maximum weighting factor (500)
            const static double MinPathTolerance;       ///< Minimum path tolerance in meters. (0)
            const static double MaxPathTolerance;       ///< Maximum path tolerance in meters. (100000)
        };
        SetGlobalPathSegment(const Address& dest = Address(), const Address& src = Address());
        SetGlobalPathSegment(const SetGlobalPathSegment& message);
        ~SetGlobalPathSegment();
        bool SetP1Latitude(const double degrees);
        bool SetP1Longitude(const double degrees);
        bool SetP1Altitude(const double value);
        bool SetP2Latitude(const double degrees);
        bool SetP2Longitude(const double degrees);
        bool SetP2Altitude(const double value);
        bool SetWeightingFactor(const double value);
        bool SetPathTolerance(const double value);
        inline double GetP1Latitude() const { return mP1Latitude; }
        inline double GetP1Longitude() const { return mP1Longitude; }
        inline double GetP1Altitude() const { return mP1Altitude; }
        inline double GetP2Latitude() const { return mP2Latitude; }
        inline double GetP2Longitude() const { return mP2Longitude; }
        inline double GetP2Altitude() const { return mP2Altitude; }
        inline double GetWeightingFactor() const { return mWeightingFactor; }
        inline double GetPathTolerance() const { return mPathTolerance; }
        virtual bool IsCommand() const { return true; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new SetGlobalPathSegment(*this); }
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        virtual UInt GetPresenceVectorSize() const { return BYTE_SIZE; }
        virtual UInt GetPresenceVectorMask() const { return 0x07; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Set Global Path Segment"; }        
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return false; }
        virtual int RunTestCase() const;
        SetGlobalPathSegment& operator=(const SetGlobalPathSegment& message);
    protected:
        Byte mPresenceVector;                   ///<  Bit vector for fields present.
        double mP1Latitude;                     ///<  P_1 Latitude in degrees [-90, 90].
        double mP1Longitude;                    ///<  P_1 Longitude in degrees [-90, 90].
        double mP1Altitude;                     ///<  P_1 Altitude in meters [-10000, 35000]
        double mP2Latitude;                     ///<  P_2 Latitude in degrees [-90, 90].
        double mP2Longitude;                    ///<  P_2 Longitude in degrees [-90, 90].
        double mP2Altitude;                     ///<  P_2 Altitude in meters [-10000, 35000]
        double mWeightingFactor;                ///<  Waypoint tolerance in meters [0, 500], 0 is a straight line.
        double mPathTolerance;                  ///<  Path tolerance in meters [0, 100000], 0 used for infinite tolerance.

    };
}

#endif
/*  End of File */
