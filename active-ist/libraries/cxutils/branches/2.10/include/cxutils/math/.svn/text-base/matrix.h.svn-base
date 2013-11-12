////////////////////////////////////////////////////////////////////////////////////
///
///  \file matrix.h
///  \brief Simple matrix class for performing most common matrix operations.
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
#ifndef __CXUTILS_MATH_MATRIX_H
#define __CXUTILS_MATH_MATRIX_H

#include "cxutils/cxbase.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Matrix
    ///   \brief Simple matrix data structure.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Matrix
    {
    public:
        Matrix();
        Matrix(const Matrix& m);
        Matrix(const unsigned int rows,
               const unsigned int cols);
        Matrix(const unsigned int rows,
               const unsigned int cols,
               const double v);
        ~Matrix();
        int Create(const unsigned int rows,
                   const unsigned int cols);
        int Create(const unsigned int rows,
                   const unsigned int cols,
                   const double v);
        int GetDeterminant(double& d) const;
        int GetMinor(const unsigned int row, 
                     const unsigned int col, 
                     Matrix& minor) const;
        int GetInverse(Matrix& i) const;
        static int GaussJordan(Matrix& a, Matrix& b);
        static int GaussJordan(const Matrix& a, const Matrix& b, Matrix& x);
        void Clear();
        void Destroy();
        void Print() const;
        inline bool IsSquare() const { return mRows == mCols; }
        inline unsigned int Rows() const { return mRows; }
        inline unsigned int Cols() const { return mCols; }
        static Matrix CreateRotationX(const double val,
                                      const bool degres = false);
        static Matrix CreateRotationY(const double val,
                                      const bool degres = false);
        static Matrix CreateRotationZ(const double val,
                                      const bool degres = false);
        static Matrix CreateScalingMatrix(const double x,
                                          const double y,
                                          const double z);
        static Matrix CreateTranslationMatrix(const double x,
                                              const double y,
                                              const double z);
        static void Transpose(const Matrix& original, Matrix& transpose);
        Matrix Inverse() const;
        Matrix Transpose() const;
        Matrix& operator=(const Matrix& m);
        Matrix& operator=(const double v);
        Matrix operator+(const Matrix& m) const;
        Matrix& operator+=(const Matrix& m);
        Matrix operator-(const Matrix& m) const;
        Matrix& operator-=(const Matrix& m);
        Matrix operator*(const Matrix& m) const;
        Matrix operator/(const Matrix& m) const;
        Matrix& operator*=(const Matrix& m);
        Matrix& operator/=(const Matrix& m);
        double* operator[](const unsigned int i);
        double* operator[](const unsigned int i) const;
        double& operator()(const unsigned int row, const unsigned int col);
        double operator()(const unsigned int row, const unsigned int col) const;
    protected:
        double *mpMatrix;
        unsigned int mRows;         ///<  Number of rows in matrix.
        unsigned int mCols;         ///<  Number of columns in matrix.
        unsigned int mSize;         ///<  Columns*Rows (size of array).
    };
}

#endif
/*  End of File */
