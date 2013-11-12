////////////////////////////////////////////////////////////////////////////////////
///
///  \file ip4address.h
///  \brief Structure for storing an IPV4 Address.
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
#include "cxutils/networking/ip4address.h"
#include "cxutils/networking/socket.h"
#include <string.h>

using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
IP4Address::IP4Address()
{
    mString = "0.0.0.0";
    memset(mData, 0, sizeof(unsigned char)*4);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates from IPv4 Address string.
///
////////////////////////////////////////////////////////////////////////////////////
IP4Address::IP4Address(const char* ipAddress)
{
    SetAddress(std::string(ipAddress));
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates from IPv4 Address string.
///
////////////////////////////////////////////////////////////////////////////////////
IP4Address::IP4Address(const std::string& ipAddress)
{
    SetAddress(ipAddress);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
IP4Address::IP4Address(const IP4Address& ipAddress)
{
    mString = ipAddress.mString;
    memcpy(mData, ipAddress.mData, sizeof(unsigned char)*4);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
IP4Address::~IP4Address()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the IP address string and octect data from an IP address
///  string.
///
///   \param[in] ipAddress IP Address data "127.0.0.1", "132.170.190.84", etc.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool IP4Address::SetAddress(const std::string& ipAddress)
{
    int a, b, c, d;
    if(ipAddress.empty() == false &&
       sscanf(ipAddress.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d) == 4)
    {
        mString = ipAddress;
        mData[0] = (unsigned char)a;
        mData[1] = (unsigned char)b;
        mData[2] = (unsigned char)c;
        mData[3] = (unsigned char)d;
        return true;
    }
    else
    {
        // Try lookup IP by hostname.
        if(ipAddress == "localhost")
        {
            mString = "127.0.0.1";
            mData[0] = (unsigned char)127;
            mData[1] = (unsigned char)0;
            mData[2] = (unsigned char)0;
            mData[3] = (unsigned char)1;
            return true;
        }
        else
        {
            struct hostent* h;
            h = gethostbyname(ipAddress.c_str());
            if(h && h->h_addr_list[0] != 0)
            {
                struct in_addr addr;
                memcpy(&addr, h->h_addr_list[0], sizeof(struct in_addr));
                return SetAddress(std::string(inet_ntoa(addr)));
            }
        }
        mString = "0.0.0.0";
        memset(mData, 0, sizeof(unsigned char)*4);
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the IP address values "a.b.c.d".
///
///   \param[in] a Part of IP address.
///   \param[in] b Part of IP address.
///   \param[in] c Part of IP address.
///   \param[in] d Part of IP address.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool IP4Address::SetAddress(const unsigned char a,
                            const unsigned char b,
                            const unsigned char c,
                            const unsigned char d)
{
    mData[0] = a;
    mData[1] = b;
    mData[2] = c;
    mData[3] = d;
    char buffer[128];
    sprintf(buffer, "%d.%d.%d.%d", a, b, c, d);
    mString = buffer;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
IP4Address& IP4Address::operator=(const IP4Address& ipAddress)
{
    mString = ipAddress.mString;
    memcpy(mData, ipAddress.mData, sizeof(unsigned char)*4);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
IP4Address& IP4Address::operator=(const std::string& ipAddress)
{
    SetAddress(ipAddress);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the IP address for sorting.
///
////////////////////////////////////////////////////////////////////////////////////
bool IP4Address::operator<(const IP4Address& ipAddress) const
{
    return mString < ipAddress.mString;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the IP address for sorting.
///
////////////////////////////////////////////////////////////////////////////////////
bool IP4Address::operator<=(const IP4Address& ipAddress) const
{
    return mString <= ipAddress.mString;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief True if equal.
///
////////////////////////////////////////////////////////////////////////////////////
bool IP4Address::operator==(const IP4Address& ipAddress) const
{
    if(memcmp(mData, ipAddress.mData, sizeof(unsigned char)*4) == 0)
    {
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief True if not equal.
///
////////////////////////////////////////////////////////////////////////////////////
bool IP4Address::operator!=(const IP4Address& ipAddress) const
{
    if(memcmp(mData, ipAddress.mData, sizeof(unsigned char)*4) != 0)
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears data.
///
////////////////////////////////////////////////////////////////////////////////////
void IP4Address::Clear()
{
    mString = "0.0.0.0";
    memset(mData, 0, sizeof(unsigned char)*4);
}

/*  End of File */
