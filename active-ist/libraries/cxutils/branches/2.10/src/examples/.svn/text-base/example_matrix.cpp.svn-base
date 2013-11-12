////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_matrix.cpp
///  \brief This is a sample program for testing/demonstrating use of the
///  Matrix class.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 9 May 2007
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
    Matrix mat1, mat2, mat3, mat4;

    mat1.Create(3, 5);
    mat2.Create(5, 3);
    mat3.Create(4, 4);
    mat4.Create(4, 4);

    for(unsigned int i = 0; i < mat2.Rows(); i++)
        for(unsigned int j = 0; j < mat2.Cols(); j++)
            mat2[i][j] = (i + j);

    for(unsigned int i = 0; i < mat1.Rows(); i++)
        for(unsigned int j = 0; j < mat1.Cols(); j++)
            mat1(i, j) = (i + j);

    mat3[0][0] = 3; mat3[0][1] = 2; mat3[0][2] = 0; mat3[0][3] = 1;
    mat3[1][0] = 4; mat3[1][1] = 0; mat3[1][2] = 1; mat3[1][3] = 2;
    mat3[2][0] = 3; mat3[2][1] = 0; mat3[2][2] = 2; mat3[2][3] = 1;
    mat3[3][0] = 9; mat3[3][1] = 2; mat3[3][2] = 3; mat3[3][3] = 1;

    mat1.Print();
    //mat1.Transpose().Print();
    mat2.Print();
    mat3.Print();

    cout << "M1 * M2 = \n";
    mat1 *= mat2;
    mat1.Print();

    double d;
    mat3.GetDeterminant(d);
    cout << "Det of M3 = " << d << "\n";

    mat4[0][0] = 1; mat4[0][1] = 0; mat4[0][2] = 0; mat4[0][3] = 0;
    mat4[1][0] = 0; mat4[1][1] = 1; mat4[1][2] = 0; mat4[1][3] = 0;
    mat4[2][0] = 0; mat4[2][1] = 0; mat4[2][2] = 1; mat4[2][3] = 0;
    mat4[3][0] = 0; mat4[3][1] = 0; mat4[3][2] = 0; mat4[3][3] = 1;

    Matrix imat3;

    Matrix::GaussJordan(mat3, mat4, imat3);
    cout << "Matrix A:\n";
    mat3.Print();
    cout << "Inverse of Matrix A:\n";
    imat3.Print();
    cout << "A*A^-1\n";       
    (mat3*imat3).Print();

    cout << "Checking inverse\n";
    Matrix iden = mat3.Inverse()*mat3;
    iden.Print();

    Vector3D vector;
    Quaternion q;
    vector.mX = 1;
    double theta, pitch;
    theta = -30;
    pitch = -20;
    q.CreateFromEuler(theta,  pitch, theta, true);
    vector = q.Rotate(vector);
    vector.Print();
    Quaternion a1, a2;

    a1.CreateFromEuler(0, 0, 30, true);
    a2.CreateFromEuler(0, 0, 90, true);

    a1 = a1 - a2;

    a1.Print(true, true);

    return 0;
}

/*  End of File */
