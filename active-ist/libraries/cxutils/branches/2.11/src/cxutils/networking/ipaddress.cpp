////////////////////////////////////////////////////////////////////////////////////
///
///  \file ipaddress.cpp
///  \brief Structure for storing an IP Address.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: March 3 2009
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
#include "cxutils/networking/ipaddress.h"
#include <cstdio>

using namespace CxUtils;

IPAddress::IPAddress() {}

IPAddress::~IPAddress() {}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the string passed is an IP4 address, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool IPAddress::IsIP4(const std::string& address)
{
    int a, b, c, d;
    if(sscanf(address.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d) == 4 &&
        a >= 0 && a <= 255 &&
        b >= 0 && b <= 255 &&
        c >= 0 && c <= 255 &&
        d >= 0 && d <= 255)
    {
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the string passed is an IP6 address, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool IPAddress::IsIP6(const std::string& address)
{
    int a, b, c, d, e, f;
    if((sscanf(address.c_str(), "%d.%d.%d.%d.%d.%d", &a, &b, &c, &d, &e, &f) == 6 ||
        sscanf(address.c_str(), "%d:%d:%d:%d:%d:%d", &a, &b, &c, &d, &e, &f) == 6)&&
        a >= 0 && a <= 255 &&
        b >= 0 && b <= 255 &&
        c >= 0 && c <= 255 &&
        d >= 0 && d <= 255 &&
        e >= 0 && e <= 255 &&
        f >= 0 && f <= 255 )
    {
        return true;
    }

    return false;
}

/* End of File */
