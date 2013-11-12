////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_packets.cpp
///  \brief This is a sample program for testing/demonstrating use of the
///  Packet data structure for serializing and de-serializing data.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 28 April 2007
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
#include <iostream>
#include <vector>
#include "cxutils/cxutils.h"

#include <string.h>

using namespace std;
using namespace CxUtils;

int main(int argc, char **argv) 
{
    Packet packet;    
    int b, c;
    double d;
    unsigned char a;

    /*  Test encoding and decoding
        that automatically keeps track
        of encode and decode positions in
        the packet. */
    a = 3;
    b = 100;
    c = -33;
    d = 3.145;

    cout << "Data being written.\n";
    cout << "\t" << (int)a << endl;
    cout << "\t" << b << endl;
    cout << "\t" << c << endl;
    cout << "\t" << d << endl << endl;
    //  Write data into packet.
    packet.Write(a);
    packet.Write(b);
    packet.Write(c);
    packet.Write(d);

    //  Clear values.
    a = 0;
    b = c = 0;
    d = 0;

    //  Start decoding from beginning of packet.
    packet.SetReadPos(0);
    packet.Read(a);
    packet.Read(b);
    packet.Read(c);
    packet.Read(d);

    cout << "Data read.\n";
    cout << "\t" << (int)a << endl;
    cout << "\t" << b << endl;
    cout << "\t" << c << endl;
    cout << "\t" << d << endl << endl;

    /*  Testing writing and reading in
        packet using provided byte positions. */

    //  Clear packet.
    packet.Clear();

    int pos = 0;
    a = 127;
    b = 23;
    c = -503;
    d = -3.145;

    cout << "Data being written.\n";
    cout << "\t" << (int)a << endl;
    cout << "\t" << b << endl;
    cout << "\t" << c << endl;
    cout << "\t" << d << endl << endl;
    //  Write data into packet.
    pos += packet.Write(a, pos);
    pos += packet.Write(b, pos);
    pos += packet.Write(c, pos);
    pos += packet.Write(d, pos);

    //  Clear values.
    a = 0;
    b = c = 0;
    d = 0;

    //  Start reading from beginning of packet.
    pos = 0;
    pos += packet.Read(a, pos);
    pos += packet.Read(b, pos);
    pos += packet.Read(c, pos);
    pos += packet.Read(d, pos);

    cout << "Data read.\n";
    cout << "\t" << (int)a << endl;
    cout << "\t" << b << endl;
    cout << "\t" << c << endl;
    cout << "\t" << d << endl << endl;

    //  Double packet size and repeat.
    packet += packet;

    //  Clear values.
    a = 0;
    b = c = 0;
    d = 0;

    //  Start decoding from beginning of packet.
    pos = 0;
    /*  Read from a data buffer. */
    pos += Packet::Read(packet.Ptr(), packet.Length(), pos, a);
    pos += Packet::Read(packet.Ptr(), packet.Length(), pos, b);
    pos += Packet::Read(packet.Ptr(), packet.Length(), pos, c);
    pos += Packet::Read(packet.Ptr(), packet.Length(), pos, d);

    cout << "Data read.\n";
    cout << "\t" << (int)a << endl;
    cout << "\t" << b << endl;
    cout << "\t" << c << endl;
    cout << "\t" << d << endl << endl;
    
    pos = 0;
    pos += packet.Read(a, pos);
    pos += packet.Read(b, pos);
    pos += packet.Read(c, pos);
    pos += packet.Read(d, pos);

    cout << "Data read.\n";
    cout << "\t" << (int)a << endl;
    cout << "\t" << b << endl;
    cout << "\t" << c << endl;
    cout << "\t" << d << endl << endl;

    char str[256];
    memset(str, 0, 256);
    
    sprintf(str, "Testing Packet.");
    
    cout << "Inserting into the following string: " << str << endl;

    packet.Clear();
    packet.Write( (unsigned char *)str, (unsigned int)strlen(str) );

    //  Now insert a value somewhere in the packet.
    //  Insert the value 36, 5 times, starting at position7.
    packet.InsertCharacter( '$', 5, 7 );
    packet.Read((unsigned char *)str, (unsigned int)strlen(str) + 5);
    
    cout << "Read the following string after insert testing: " << str << endl;

    std::string testString = "Hello World";
    packet.Clear();
    packet.Write(testString);
    testString.clear();
    packet.Read(testString, packet.Length());

    unsigned char buffer[512];
    
    Packet::Wrapper wrapper(buffer, 512);

    cout << "Data being written using wrapper.\n";
    cout << "\t" << (int)a << endl;
    cout << "\t" << b << endl;
    cout << "\t" << c << endl;
    cout << "\t" << d << endl << endl;
    
    //  Write data into wrapper
    pos += wrapper->Write(a);
    pos += wrapper->Write(b);
    pos += wrapper->Write(c);
    pos += wrapper->Write(d);

    //  Clear values.
    a = 0;
    b = c = 0;
    d = 0;

    //  Start reading from beginning of wrapper->
    pos = 0;
    pos += wrapper->Read(a);
    pos += wrapper->Read(b);
    pos += wrapper->Read(c);
    pos += wrapper->Read(d);
    
    cout << "Data wread using wrapper.\n";
    cout << "\t" << (int)a << endl;
    cout << "\t" << b << endl;
    cout << "\t" << c << endl;
    cout << "\t" << d << endl << endl;

    return 0;
}

/*  End of File */
