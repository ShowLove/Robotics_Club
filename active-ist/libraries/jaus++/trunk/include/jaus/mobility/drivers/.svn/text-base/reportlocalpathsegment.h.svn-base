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
#ifndef __JAUS_MOBILITY_REPORT_LOCAL_PATH_SEGMENT__H
#define __JAUS_MOBILITY_REPORT_LOCAL_PATH_SEGMENT__H

#include "jaus/core/message.h"
#include "jaus/mobility/mobilitycodes.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ReportLocalPathSegment
    ///   \brief This message is used to provide the receiver the values of the current
    ///          path segment.  The message data and mapping of the presence vector of
    ///          this message are identical to ID 040Fh: SetLocalPathSegment
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL ReportLocalPathSegment : public Message
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
        ReportLocalPathSegment(const Address& dest = Address(), const Address& src = Address());
        ReportLocalPathSegment(const ReportLocalPathSegment& message);
        ~ReportLocalPathSegment();
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
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new ReportLocalPathSegment(*this); }
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        virtual UInt GetPresenceVectorSize() const { return BYTE_SIZE; }
        virtual UInt GetPresenceVectorMask() const { return 0x07; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Report Local Path Segment"; }        
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return false; }
        virtual int RunTestCase() const;
        ReportLocalPathSegment& operator=(const ReportLocalPathSegment& message);
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
