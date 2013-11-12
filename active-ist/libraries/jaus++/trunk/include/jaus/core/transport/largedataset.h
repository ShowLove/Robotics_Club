////////////////////////////////////////////////////////////////////////////////////
///
///  \file largedataset.h
///  \brief This file contains an interface for storing and generating
///  Large Data Sets in JAUS.
///
///  Author(s): Daniel Barber
///  Created: 27 September 2009
///  Copyright (c) 2009
///  Applied Cognition and Training in Immersive Virtual Environments
///  (ACTIVE) Laboratory
///  Institute for Simulation and Training (IST)
///  University of Central Florida (UCF)
///  Email: dbarber@ist.ucf.edu
///  Web:  http://active.ist.ucf.edu
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
#ifndef __JAUS_CORE_LARGE_DATA_SET__H
#define __JAUS_CORE_LARGE_DATA_SET__H

#include "jaus/core/header.h"
#include "jaus/core/time.h"
#include <map>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class LargeDataSet
    ///   \brief Data structure for storing multi-packet sequence data and
    ///          merging/splitting up data packets.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL LargeDataSet
    {
    public:        
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Key
        ///   \brief Hash key structure for sorting/organizing LargeDataSets.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL Key
        {
        public:
            Key(const Address& src = Address(),
                const UShort code = 0,
                const UInt pv = 0);
            Key(const Key& key);
            ~Key();
            void Update() const;
            bool operator<(const Key& key) const;
            bool operator<=(const Key& key) const;
            Key& operator=(const Key& key);
            Address mSourceID;           ///<  Message source Address.
            UShort mMessageCode;    ///<  Type of message stored in data set.
            ULong mPresenceVector;  ///<  Presence vector associated with message data.
            Byte mIdentifier;       ///<  Additional identifier for multiple multi-packet streams.
            Packet mKey;            ///<  Serialized version of key data (for fast comparisons).
        };
        typedef std::map<UShort, Packet> Stream;
        typedef std::map<LargeDataSet::Key, LargeDataSet*> Map;
        LargeDataSet();
        ~LargeDataSet();
        void Clear();
        bool AddPacket(const Packet& message);
        bool AddPacket(const Header& header, 
                       const UShort messageCode, 
                       const Packet& packet);
        static void CreateLargeDataSet(const Header& header,
                                       const UShort messageCode,
                                       const Packet& payload,
                                       Packet::List& stream,
                                       Header::List& streamHeaders,
                                       const Packet* transportHeader = NULL,
                                       const UShort maxPayloadSize = 1437,
                                       const UShort startingSequenceNumber = 0);
        static bool MergeLargeDataSet(Header& header,
                                      UShort& messageCode,
                                      Packet& payload,
                                      const Packet::List& stream,
                                      const Packet* transportHeader = NULL);
        static bool MergeLargeDataSet(Header& header,
                                      UShort& messageCode,
                                      Packet& payload,
                                      const Stream& stream,
                                      const Packet* transportHeader = NULL);
        Header mHeader;                 ///<  Message header information.
        UShort mMessageCode;            ///<  Message type.
        bool mHaveLastFlag;             ///<  True if the last packet has been received.
        bool mCompleteFlag;             ///<  If true, the large data set is complete.
        Stream mStream;                 ///<  The multi-packet stream sequence collected.
        std::set<UShort> mMissing;      ///<  Missing packet in in the stream sequence.
        Time::Stamp mUpdateTimeMs;      ///<  The last time data was added to the stream.
    };
}

#endif

/* End of File */
