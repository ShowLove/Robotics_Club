////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_circulararray.cpp
///  \brief This is a sample program for testing/demonstrating use of the
///  circular array template.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 20 May 2009
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
#include "cxutils/cxutils.h"

using namespace std;
using namespace CxUtils;

void TestConstant(const CircularArray<int>& carray)
{
    cout << "Testing Constant Iterator.\n";
    CircularArray<int>::const_iterator c_itr;
    for(c_itr = carray.begin(); c_itr != carray.end(); c_itr++)
    {
        cout << *c_itr << endl;
    }

    cout << "Testing Constant Reverse Iterator.\n";
    CircularArray<int>::const_reverse_iterator c_ritr;
    for(c_ritr = carray.rbegin(); c_ritr != carray.rend(); c_ritr++)
    {
        cout << *c_ritr << endl;
    }
}

int main(int argc, char **argv) 
{
    CircularArray<int> carray;

    carray.Reserve(5);
    // After we insert 5, we will start 
    // writing over the oldest data and at the
    // end we should only have 6-10 left in the array.
    carray.PushBack(1);
    carray.PushBack(2);
    carray.PushBack(3);
    carray.PushBack(4);
    carray.PushBack(5);
    carray.PushBack(6);
    carray.PushBack(7);
    carray.PushBack(8);
    carray.PushBack(9);
    carray.PushBack(10);
    // Test resize.  Since we are enlarging the array
    // we should not lose any data.
    carray.Resize(10);

    cout << "Testing Iterator.\n";
    CircularArray<int>::iterator itr;
    for(itr = carray.begin(); itr != carray.end(); itr++)
    {
        cout << *itr << endl;
    }

    cout << "Testing Reverse Iterator.\n";
    CircularArray<int>::reverse_iterator ritr;
    for(ritr = carray.rbegin(); ritr != carray.rend(); ritr++)
    {
        cout << *ritr << endl;
    }

    TestConstant(carray);
    
    return 0;
}

/*  End of File */
