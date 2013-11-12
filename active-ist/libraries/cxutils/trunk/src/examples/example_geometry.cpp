////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_matrix.cpp
///  \brief This is a sample program for testing/demonstrating use of the
///  Matrix class.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 17 November 2008
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

using namespace std;
using namespace CxUtils;

int main(int argc, char **argv)
{
    Segment3D line1, line2;
    Point3D intersection;

    line1.mPoint1(1, 2, 0);
    line1.mPoint2(2, 2, 0);

    line2.mPoint1(1, 1, 0);
    line2.mPoint2(2, 1, 0);

    // Should be parallel.
    if(Segment3D::IsParallel(line1, line2) == false)
    {
        cout << "MATH Error!\n";
    }

    line2.mPoint2(2, 1.5, 0);
    // Should not be parallel
    if(Segment3D::IsParallel(line1, line2) == true)
    {
        cout << "MATH Error!\n";
    }

    line1.mPoint1(1, 2, 0);
    line1.mPoint2(1, 1, 0);

    line2.mPoint1(2, 2, 0);
    line2.mPoint2(2, 1, 0);

    // Should be parallel.
    if(Segment3D::IsParallel(line1, line2) == false)
    {
        cout << "MATH Error!\n";
    }

    line2.mPoint2(2, 1.5, 0);
    // Should not be parallel
    if(Segment3D::IsParallel(line1, line2) == true)
    {
        cout << "MATH Error!\n";
    }

    line1.mPoint1(1, 2, 0.5);
    line1.mPoint2(2, 2, 0.5);

    line2.mPoint1(1.5, 1.5, 0.5);
    line2.mPoint2(2, 3, 0.5);

    // Should intersect
    if(Segment3D::DoesIntersect(line1, line2, intersection) == false)
    {
        cout << "MATH Error!\n";
    }

    line1.mPoint1(0, 0, 0);
    line1.mPoint2(1, 0, 0);
    line2 = line1.Rotate(-90, Segment3D::Z, true);

    // line2 should be [0,0,0], [0,-1,0] after rotation by -90.
    if(CX_FLOAT_EQUAL(line2.mPoint2.mX, 0, 0.1) == false ||
        CX_FLOAT_EQUAL(line2.mPoint2.mY, -1, 0.1) == false)
    {
        cout << "MATH Error!\n";
    }

    return 0;
}

/*  End of File */
