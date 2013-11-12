////////////////////////////////////////////////////////////////////////////////////
///
///  \file ip4address.h
///  \brief Structure for storing an IPv4 Address.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 1 June 2007
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
#ifndef __CXUTILS_IPV4_ADDRESS_H
#define __CXUTILS_IPV4_ADDRESS_H

#include "cxutils/networking/ipaddress.h"
#include <list>

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class IP4Address
    ///   \brief Simple IPv4 Network Address structure.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL IP4Address : public IPAddress
    {
    public:
        typedef std::set<IP4Address> Set;
        typedef std::vector<IP4Address> List;
        typedef std::list<IP4Address> Queue;
        IP4Address();
        IP4Address(const char* ipAddress);
        IP4Address(const std::string& ipAddress);
        IP4Address(const IP4Address& ipAddress);
        ~IP4Address();
        bool SetAddress(const std::string& ipAddress);
        bool SetAddress(const unsigned char a, 
                        const unsigned char b,
                        const unsigned char c,
                        const unsigned char d);
        IP4Address& operator=(const IP4Address& ipAddress);
        IP4Address& operator=(const std::string& ipAddress);
        //operator std::string() const { return mString; }
        bool operator==(const IP4Address& ipAddress) const;
        bool operator!=(const IP4Address& ipAddress) const;
        bool operator<(const IP4Address& ipAddress) const;
        bool operator<=(const IP4Address& ipAddress) const;
        virtual void Clear();
        std::string mString;
        unsigned char mData[4];
    };
}

/*inline std::ostream& operator<<(std::ostream& output, const CxUtils::IP4Address& ip)
{
    output << ip.mString;
    return output;
}*/

#endif

/*  End of File */
