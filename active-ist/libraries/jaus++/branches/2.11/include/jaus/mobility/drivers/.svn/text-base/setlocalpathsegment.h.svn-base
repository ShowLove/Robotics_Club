////////////////////////////////////////////////////////////////////////////////////
///
///  \file setlocalpathsegment.h
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
#ifndef __JAUS_MOBILITY_SET_LOCAL_PATH_SEGMENT__H
#define __JAUS_MOBILITY_SET_LOCAL_PATH_SEGMENT__H

#include "jaus/core/message.h"
#include "jaus/mobility/mobilitycodes.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class SetLocalPathSegment
    ///   \brief This message is used to set the path segment data on the local
    ///          coordinate system.  A global path segment is defined in this message
    ///          three points, P_0, P_1, and P_2 and a weighting factor.  For the first
    ///          path segment, i.e. the first element in a list of path segments, P_0
    ///          is assumed to be the current location of the platform as defined by
    ///          Report Global Pose.  For each successive path segment, i.e. where the
    ///          path segment number is greater than zero, P_0 is equal to the
    ///          previous path segment's P_2.  Therefore, for each message, only
    ///          P_1, P_2, and a weighting factor must be set in order to define
    ///          a path segment.  Each point is defined in the Local Coordinate System
    ///          by setting its X, Y, and Z.  Both X and Y are required
    ///          fields, but the Z field is optional.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL SetLocalPathSegment : public Message
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
            const static Byte P1Z = 0x01;
            const static Byte P2Z = 0x02;
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
            const static double MinX;                   ///<  Minimum X. (-100000)
            const static double MaxX;                   ///<  Maximum X. (100000)
            const static double MinY;                   ///<  Minimum Y. (-100000)
            const static double MaxY;                   ///<  Maximum Y. (100000)
            const static double MinZ;                   ///<  Minimum Z. (-10000)
            const static double MaxZ;                   ///<  Maximum Z. (35000)
            const static double MinWeightingFactor;     ///<  Minimum weighting factor (0)
            const static double MaxWeightingFactor;     ///<  Maximum weighting factor (500)
            const static double MinPathTolerance;       ///< Minimum path tolerance in meters. (0)
            const static double MaxPathTolerance;       ///< Maximum path tolerance in meters. (100000)
        };
        SetLocalPathSegment(const Address& dest = Address(), const Address& src = Address());
        SetLocalPathSegment(const SetLocalPathSegment& message);
        ~SetLocalPathSegment();
        bool SetP1X(const double degrees);
        bool SetP1Y(const double degrees);
        bool SetP1Z(const double value);
        bool SetP2X(const double degrees);
        bool SetP2Y(const double degrees);
        bool SetP2Z(const double value);
        bool SetWeightingFactor(const double value);
        bool SetPathTolerance(const double value);
        inline double GetP1X() const { return mP1X; }
        inline double GetP1Y() const { return mP1Y; }
        inline double GetP1Z() const { return mP1Z; }
        inline double GetP2X() const { return mP2X; }
        inline double GetP2Y() const { return mP2Y; }
        inline double GetP2Z() const { return mP2Z; }
        inline double GetWeightingFactor() const { return mWeightingFactor; }
        inline double GetPathTolerance() const { return mPathTolerance; }
        virtual bool IsCommand() const { return true; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new SetLocalPathSegment(*this); }
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        virtual UInt GetPresenceVectorSize() const { return BYTE_SIZE; }
        virtual UInt GetPresenceVectorMask() const { return 0x07; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Set Local Path Segment"; }        
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return false; }
        virtual int RunTestCase() const;
        SetLocalPathSegment& operator=(const SetLocalPathSegment& message);
    protected:
        Byte mPresenceVector;                   ///<  Bit vector for fields present.
        double mP1X;                            ///<  P_1 X in meters [-100000, 100000].
        double mP1Y;                            ///<  P_1 Y in meters [-100000, 100000].
        double mP1Z;                            ///<  P_1 Z in meters [-10000, 35000]
        double mP2X;                            ///<  P_2 X in meters [-100000, 100000].
        double mP2Y;                            ///<  P_2 X in meters [-100000, 100000].
        double mP2Z;                            ///<  P_2 Z in meters [-10000, 35000]
        double mWeightingFactor;                ///<  Waypoint tolerance in meters [0, 500], 0 is a straight line.
        double mPathTolerance;                  ///<  Path tolerance in meters [0, 100000], 0 used for infinite tolerance.
    };
}

#endif
/*  End of File */
