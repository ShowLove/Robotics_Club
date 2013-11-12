////////////////////////////////////////////////////////////////////////////////////
///
///  \file matrix.cpp
///  \brief Simple matrix class for performing most common matrix operations.
///   
///  <br>Author(s): Daniel Barber
///  <br>Created: 9 May 2007
///  <br>Last Modified: 9 May 2007
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
#include "cxutils/math/matrix.h"
#include "cxutils/math/cxmath.h"
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <string.h>

using namespace std;
using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix::Matrix()
{
    mpMatrix = NULL;
    mRows = mCols = mSize = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix::Matrix(const Matrix& m)
{
    mRows = m.mRows;
    mCols = m.mCols;
    mSize = m.mSize;

    if(mSize != 0) 
    {
        mpMatrix = new double[mSize];
        assert(mpMatrix);
        memcpy(mpMatrix, m.mpMatrix, sizeof(double)*mSize);
    }
    else
    {
        mpMatrix = NULL;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a matrix and initializes all values to zero.
///
///   \param rows Number of rows in the matrix.
///   \param cols Number of columns in the matrix.
///
///   \return 0 If failed to create matrix, otherwise 1.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix::Matrix(const unsigned int rows, const unsigned int cols)
{
    mpMatrix = NULL;
    mRows = mCols = mSize = 0;
    Create(rows, cols);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a matrix and initializes all values to a default.
///
///   \param rows Number of rows in the matrix.
///   \param cols Number of columns in the matrix.
///   \param v The default value to have at each point in matrix.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix::Matrix(const unsigned int rows, const unsigned int cols, const double v)
{
    mpMatrix = NULL;
    mRows = mCols = mSize = 0;
    Create(rows, cols, v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix::~Matrix() 
{
    Destroy();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a matrix and initializes all values to zero.
///
///   \param rows Number of rows in the matrix.
///   \param cols Number of columns in the matrix.
///
///   \return 0 If failed to create matrix, otherwise 1.
///
////////////////////////////////////////////////////////////////////////////////////
int Matrix::Create(const unsigned int rows, const unsigned int cols)
{
    if(rows == mRows && cols == mCols)
    {
        if(mSize > 0)
        {
            memset(mpMatrix, 0, sizeof(double)*mSize);
        }
        return 1;
    }
    Destroy();
    if(rows == 0 || cols == 0)
    {
        return 0;
    }

    mSize = rows*cols;
    mRows = rows;
    mCols = cols;

    mpMatrix = new double[mSize];
    memset(mpMatrix, 0, sizeof(double)*mSize);
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a matrix and initializes all values to a default.
///
///   \param rows Number of rows in the matrix.
///   \param cols Number of columns in the matrix.
///   \param v The default value to have at each point in matrix.
///
///   \return 0 If failed to create matrix, otherwise 1.
///
////////////////////////////////////////////////////////////////////////////////////
int Matrix::Create(const unsigned int rows, const unsigned int cols, const double v)
{
    if(rows == mRows && cols == mCols)
    {
        double *ptr = mpMatrix;
        for(unsigned int i = 0; i < mSize; i++)
            *(ptr++) = v;

        return 1;
    }

    Destroy();
    if(rows == 0 || cols == 0) 
    {
        return 0;
    }

    mSize = rows*cols;
    mRows = rows;
    mCols = cols;

    mpMatrix = new double[mSize];
    assert(mpMatrix);
    double *ptr = mpMatrix;
    for(unsigned int i = 0; i < mSize; i++)
        *(ptr++) = v;
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates the determinant of the matrix.  
///
///   The matrix must be square for this to work.
///
///   \param d The calculated determinant.
///
///   \return 1 if able calculate determinant, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Matrix::GetDeterminant(double& d) const
{
    if(mRows == mCols && mSize > 0) 
    {
        if(mRows == 1) 
        {
            d = *mpMatrix;
        }
        else if(mRows == 2) 
        {
            d = mpMatrix[0]*mpMatrix[3] - mpMatrix[1]*mpMatrix[2];
        }
        else if(mRows == 3) 
        {
            d =  mpMatrix[0]*
                  ( mpMatrix[4]*mpMatrix[8]  - 
                    mpMatrix[7]*mpMatrix[5]) -
                  mpMatrix[1]*
                  ( mpMatrix[3]*mpMatrix[8]  - 
                    mpMatrix[6]*mpMatrix[5]) +
                   mpMatrix[0*mCols + 1]*
                  ( mpMatrix[3]*mpMatrix[7]  - 
                    mpMatrix[6]*mpMatrix[4]);
        }
        else 
        {
            d = 0;
            Matrix minor;
            double sign = -1.0;
            for(unsigned int i = 0; i < mRows; i++) 
            {
                double det = 0;
                sign  = pow(-1.0f, (float)i);
                GetMinor(0, i, minor);
                minor.GetDeterminant(det);
                d += sign*det*mpMatrix[i];
            }
        }
        return 1;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the minor of the matrix.  The minor is the matrix with the
///   row and column selected blocked out.  It used for finding determinants and
///   other things.
///
///   <b>WARNING:</b>  The matrix dimensions must be equal (square).
///
///   \param row The row to block for minor formation.
///   \param col The column to block for minor formation.
///   \param m The resulting minor matrix.
///
///   \return 1 If minor was extracted, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
int Matrix::GetMinor(const unsigned int row, const unsigned int col, Matrix& m) const
{
    assert(mRows == mCols && "Matrix::Error - Dimensions must be equal to get a minor.");
    
    if(row >= mRows || col >= mCols)
        return 0;  //  Out of bounds row/col.

    if(m.mRows != mRows - 1 || m.mCols != mCols - 1)
        m.Create(mRows - 1, mCols - 1);

    double *ptr = m.mpMatrix;
    double *mat = mpMatrix;
    for(unsigned int r = 0; r < mRows; r++) 
    {
        if(r != row) 
        {
            mat = mpMatrix + r*mCols;
            for(unsigned int c = 0; c < mCols; c++, mat++) 
            {
                if(c != col) 
                {
                    *ptr++ = *mat;
                }
            }
        }
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the inverse of the matrix.  Matrix must be a square.
///
///   \param i The inverse of the matrix.
///
///   \return 1 If process completed successfully, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Matrix::GetInverse(Matrix& i, ErrorCode* error) const
{
    if(mRows != mCols || mRows == 0)
    {
        std::cout << "Matrix::Error Cannot Invert, (mRows != mCols || mRows == 0)\n";
        *error = MatrixNotSquare;
        return 0;
    }

    Matrix identity(mRows, mCols);
    for(unsigned int k = 0; k < mRows; k++)
        identity.mpMatrix[k*mRows + k] = 1.0;
    return GaussJordan(*this, identity, i, error);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs Gauss-Jordan elimination to the two square matrices.
///
///   Performs the operation with the matrix in the format of
///   a*x = b.
///   After operation completes b is equal to the solution (x), and a is
///   replaced by its matrix inverse.  
///
///   \param a The a matrix being multiplied against the solution x.  A is
///   replaced by it's matrix inverse after successful operation of function.
///            This matrix must be a square.
///   \param b The solution to a*x.  After successful operation b is replaced
///   with the solution x.
///
///   \return 1 If process completed successfully, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Matrix::GaussJordan(Matrix& a, Matrix& b, ErrorCode* error)
{
    unsigned int iRow, iCol;
    unsigned int n = 0, m = 0;
    unsigned int *cIndex, *rIndex, *iPivots;
    double max, pInv = 0.0;

    //  a and b must be square matrices
    if(a.mCols != a.mRows)
    {
        *error = MatrixNotSquare;
        return 0;
    }

    /*  Result of a*x = b means b should have the
        same number of rows as a. */   
    if(a.mRows != b.mRows) 
    {
        *error = MatrixNotSquare;
        return 0;
    }
    
    n = a.mRows;
    m = b.mCols;

    cIndex = new unsigned int[n];
    rIndex = new unsigned int[n];
    iPivots = new unsigned int[n];
    assert(cIndex && rIndex && iPivots);
    //  Initiliaze values.
    memset(cIndex, 0, sizeof(unsigned int)*n);
    memset(rIndex, 0, sizeof(unsigned int)*n);
    memset(iPivots, 0, sizeof(unsigned int)*n);
    iRow = iCol = 0;
    
    for(unsigned int i = 0; i < n; i++) 
    {
        max = 0.0;
        for(unsigned int j = 0; j < n; j++) 
        {
            if(iPivots[j] != 1) 
            {
                for(unsigned int k = 0; k < n; k++) 
                {
                    if(iPivots[k] == 0) 
                    {
                        double val = fabs(a.mpMatrix[j*a.mCols + k]);
                        if(val >= max) 
                        {
                            max = val;
                            iRow = j;
                            iCol = k;
                        }
                    }
                }
            }
        }
        ++(iPivots[iCol]);
        if(iRow != iCol) 
        {
            for(unsigned int l = 0; l < n || l < m; l++) 
            {
                double t;
                double *p1, *p2;
                if(l < n) 
                {
                    p1 = &(a.mpMatrix[iRow*a.mCols + l]);
                    p2 = &(a.mpMatrix[iCol*a.mCols + l]);
                    t = *p1;
                    *p1 = *p2;
                    *p2 = t;
                }
                if(l < m) 
                {
                    p1 = &(b.mpMatrix[iRow*b.mCols + l]);
                    p2 = &(b.mpMatrix[iCol*b.mCols + l]);
                    t = *p1;
                    *p1 = *p2;
                    *p2 = t;
                }
            }
        }

        rIndex[i] = iRow;
        cIndex[i] = iCol;
        unsigned int pos = iCol*a.mCols + iCol;
        double t = a.mpMatrix[pos];
        if( fabs(t) < CX_EPSILON ) 
        {
            //  Singular matrix
            *error = Matrix::MatrixSingular;
            if(cIndex)
                delete[] cIndex;
            if(rIndex)
                delete[] rIndex;
            if(iPivots)
                delete[] iPivots;
            return 0;
        }
        pInv = 1.0/t;
        a.mpMatrix[pos] = 1.0;
        //  Divid by pivot element
        for(unsigned int l = 0; l < n || l < m; l++) 
        {
            if(l < n)
                a.mpMatrix[iCol*a.mCols + l] *= pInv;
            if(l < m)
                b.mpMatrix[iCol*b.mCols + l] *= pInv;
        }
        //  Reduce rows
        for(unsigned int l = 0; l < n; l++) 
        {
            if(l != iCol) 
            {
                t = a.mpMatrix[l*a.mCols + iCol];
                a.mpMatrix[l*a.mCols + iCol] = 0.0;
                for(unsigned int k = 0; k < n || k < m; k++) 
                {
                    if(k < n) 
                    {
                        a.mpMatrix[l*a.mCols + k] -= a.mpMatrix[iCol*a.mCols + k]*t;
                    }
                    if(k < m) 
                    {
                        b.mpMatrix[l*b.mCols + k] -= b.mpMatrix[iCol*b.mCols + k]*t;
                    }
                }
            }
        }

        for(int l = (int)(n - 1); l >= 0; l--) 
        {
            if(rIndex[l] != cIndex[l]) 
            {
                double *p1, *p2;
                for(unsigned int k = 0; k < n; k++) 
                {
                    p1 = &(a.mpMatrix[k*a.mCols + rIndex[l]]);
                    p2 = &(a.mpMatrix[k*a.mCols + cIndex[l]]);
                    t = *p1;
                    *p1 = *p2;
                    *p2 = t;
                }
            }
        }
    }

    if(cIndex)
        delete[] cIndex;
    if(rIndex)
        delete[] rIndex;
    if(iPivots)
        delete[] iPivots;

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs Gauss-Jordan elimination to the two square matrices and puts
///   the resulting solution in x.
///
///   Performs the operation with the matrix in the format of
///   a*x = b.
///   After operation completes b is equal to the solution (x), and a is
///   replaced by its matrix inverse.
///
///   \param a The a matrix being multiplied against the solution x. This matrix 
///            must be a square.
///   \param b The solution to a*x.
///   \param x The solution of Gauss-Jordan elimination.
///
///   \return 1 If process completed successfully, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Matrix::GaussJordan(const Matrix& a, const Matrix& b, Matrix& x, ErrorCode* error)
{
    x = b;
    Matrix aCopy = a;
    if(GaussJordan(aCopy, x, error)) 
    {
        return 1;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets all values to 0.
///
////////////////////////////////////////////////////////////////////////////////////
void Matrix::Clear()
{
    if(mpMatrix) 
    {
        memset(mpMatrix, 0, sizeof(double)*mSize);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes all matrix data.
///
////////////////////////////////////////////////////////////////////////////////////
void Matrix::Destroy()
{
    if(mpMatrix) 
    {
        delete[] mpMatrix;
        mpMatrix = NULL;
    }
    mRows = mCols = mSize = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints matrix data to console window.
///
////////////////////////////////////////////////////////////////////////////////////
void Matrix::Print() const
{
    double *ptr = mpMatrix;
    cout << "\n";
    if(ptr == NULL) 
    {
        cout << "[ ]\n";
        return;
    }
    
    for(unsigned int i = 0; i < mRows; i++) 
    {
        cout << "| ";
        for(unsigned int j = 0; j < mCols; j++) 
        {
            if(fabs(*ptr) < .000000000001) 
            {
                cout << setw(10) << 0 << " ";
                ptr++;
            }
            else
                cout << setw(10) << *ptr++ << " ";
        }
        cout << setw(10) << " |\n";
    }
    cout << "\n";
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method for getting tranpose matrix.  More memory efficient than
///          using the other method which returns a temporary object.
///
////////////////////////////////////////////////////////////////////////////////////
void Matrix::Transpose(const Matrix& original, Matrix& transpose)
{
    transpose.Create(original.mCols, original.mRows);
    for(unsigned int i = 0; i < original.mCols; i++)
    {
        for(unsigned int j = 0; j < original.mRows; j++)
        {
            unsigned int posA = j*original.mCols + i;
            unsigned int posB = i*original.mRows + j;
            transpose.mpMatrix[posB] = original.mpMatrix[posA];
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the inverse of the matrix if it is a square.
///
///   \return Matrix inverse.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix Matrix::Inverse() const
{
    Matrix inverse(mRows, mCols);
    Matrix::ErrorCode error;
    int result = GetInverse(inverse, &error);
    if(error == MatrixNotSquare)
    {
        assert("Matrix: Can only invert square matrix.");
    }
    if(error == MatrixSingular)
    {
        assert("Matrix: Cannot invert singular matrix.");
    }
    return inverse;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The transpose of the current matrix.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix Matrix::Transpose() const
{
    Matrix t(mCols, mRows);
    for(unsigned int i = 0; i < mCols; i++)
    {
        for(unsigned int j = 0; j < mRows; j++)
        {
            unsigned int posA = j*mCols + i;
            unsigned int posB = i*mRows + j;
            t.mpMatrix[posB] = mpMatrix[posA];
        }
    }
    return t;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a 4x4 rotation matrix around the axis.
///
///   \param val The amount to rotate.
///   \param degrees If true, val is in degrees, otherwise radians.
///
///   \return The new matrix.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix Matrix::CreateRotationX(const double val,
                               const bool degrees)
{
    Matrix m(4,4);
    m.mpMatrix[0] = 1.0;
    m.mpMatrix[5]  = degrees ? cos(CX_DEG2RAD(val)) : cos(val);
    m.mpMatrix[6]  = degrees ? -sin(CX_DEG2RAD(val)) : -sin(val);
    m.mpMatrix[9]  = -m.mpMatrix[6];
    m.mpMatrix[10] = m.mpMatrix[5];
    m.mpMatrix[15] = 1.0;
    return m;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a 4x4 rotation matrix around the axis.
///
///   \param val The amount to rotate.
///   \param degrees If true, val is in degrees, otherwise radians.
///
///   \return The new matrix.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix Matrix::CreateRotationY(const double val,
                               const bool degrees)
{
    Matrix m(4,4);
    m.mpMatrix[0]  = degrees ? cos(CX_DEG2RAD(val)) : cos(val);
    m.mpMatrix[2]  = degrees ? sin(CX_DEG2RAD(val)) : sin(val);
    m.mpMatrix[5]  = 1.0;
    m.mpMatrix[8]  = -m.mpMatrix[2];
    m.mpMatrix[10] = m.mpMatrix[0];
    m.mpMatrix[15] = 1.0;
    return m;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a 4x4 rotation matrix around the axis.
///
///   \param val The amount to rotate.
///   \param degrees If true, val is in degrees, otherwise radians.
///
///   \return The new matrix.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix Matrix::CreateRotationZ(const double val,
                               const bool degrees)
{
    Matrix m(4,4);
    m.mpMatrix[0]  = degrees ? cos(CX_DEG2RAD(val)) : cos(val);
    m.mpMatrix[1]  = degrees ? -sin(CX_DEG2RAD(val)) : -sin(val);
    m.mpMatrix[4]  = -m.mpMatrix[1];
    m.mpMatrix[5]  = m.mpMatrix[0];
    m.mpMatrix[10] = 1.0;
    m.mpMatrix[15] = 1.0;
    return m;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a 4x4 scaling matrix.
///
///   To make scaling uniform, just make x = y = z.
///
///   \param x The amount to scale in the x axis.
///   \param y The amount to scale in the y axis.
///   \param z The amount to scale in the z axis.
///
///   \return The new scaling matrix.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix Matrix::CreateScalingMatrix(const double x,
                                   const double y,
                                   const double z)
{
    Matrix m(4,4);
    m.mpMatrix[0]  = x;
    m.mpMatrix[5]  = y;
    m.mpMatrix[10] = z;
    m.mpMatrix[15] = 1.0;
    return m;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a 4x4 translation matrix.
///
///   \param x The position change in the x axis.
///   \param y The position change in the y axis.
///   \param z The position change in the z axis.
///
///   \return The new translation matrix.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix Matrix::CreateTranslationMatrix(const double x,
                                       const double y,
                                       const double z)
{
    Matrix m(4,4);
    m.mpMatrix[0]  = 1.0;
    m.mpMatrix[3]  = x;
    m.mpMatrix[5]  = 1.0;
    m.mpMatrix[7]  = y;
    m.mpMatrix[10] = 1.0;
    m.mpMatrix[11] = z;
    m.mpMatrix[15] = 1.0;
    return m;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix& Matrix::operator=(const Matrix& m)
{
    if(this != &m) 
    {
        if(mSize != m.mSize) 
        {
            if(mpMatrix) 
            {
                delete[] mpMatrix;
                mpMatrix = NULL;
            }
        }

        mRows = m.mRows;
        mCols = m.mCols;
        mSize = m.mSize;

        if(mSize != 0) 
        {
            if(!mpMatrix) 
            {
                mpMatrix = new double[mSize];
                assert(mpMatrix);
            }
            memcpy(mpMatrix, m.mpMatrix, sizeof(double)*mSize);
        }
        else 
        {
            mpMatrix = NULL;
        }
    }

    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets all values in the matrix equal to the constant.
///
///   \param v The value to use at every location in the matrix.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix& Matrix::operator=(const double v)
{
    if(mpMatrix)
    {
        double *p = mpMatrix;
        for(unsigned int i = 0; i < mSize; i++, p++)
            *p = v;
    }
    
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds two matrices together.  <b>Both must have the exact same
///   dimensions.</b>
///
///   \param m The matrix to add.
///
///   \return The result of matrix addition.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix Matrix::operator+(const Matrix& m) const
{
    assert(mRows == m.mRows && mCols == m.mCols && "Matrix::Error - Dimensions are not equal.");
    Matrix result(mRows, mCols);
    if(mSize > 0) 
    {
        double* m1;
        double* m2;
        double *m3;
        m1 = mpMatrix;
        m2 = m.mpMatrix;
        m3 = result.mpMatrix;
        for(unsigned int i = 0; i < mSize; i++, m1++, m2++, m3++)
            *m3 = *m1 + *m2;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds two matrices together.  <b>Both must have the exact same
///   dimensions.</b>
///
///   \param m The matrix to add.
///
///   \return The result of matrix addition.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix& Matrix::operator+=(const Matrix& m)
{
    assert(mRows == m.mRows && mCols == m.mCols && "Matrix::Error - Dimensions are not equal.");
    if(mSize > 0) 
    {
        double* m1;
        double* m2;
        m1 = mpMatrix;
        m2 = m.mpMatrix;
        for(unsigned int i = 0; i < mSize; i++, m1++, m2++)
            *m1 += *m2;
    }

    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Subtracts the matrix.  <b>Both must have the exact same
///   dimensions.</b>
///
///   \param m The matrix to subtract.
///
///   \return The result of matrix subtraction.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix Matrix::operator-(const Matrix& m) const
{
    assert(mRows == m.mRows && mCols == m.mCols && "Matrix::Error - Dimensions are not equal.");
    Matrix result(mRows, mCols);
    if(mSize > 0) 
    {
        double* m1;
        double* m2;
        double *m3;
        m1 = mpMatrix;
        m2 = m.mpMatrix;
        m3 = result.mpMatrix;
        for(unsigned int i = 0; i < mSize; i++, m1++, m2++, m3++)
            *m3 = *m1 - *m2;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Subtracts the matrix.  <b>Both must have the exact same
///   dimensions.</b>
///
///   \param m The matrix to subtract.
///
///   \return The result of matrix subtraction.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix& Matrix::operator-=(const Matrix& m)
{
    assert(mRows == m.mRows && mCols == m.mCols && "Matrix::Error - Dimensions are not equal.");
    if(mSize > 0) 
    {
        double* m1;
        double* m2;
        m1 = mpMatrix;
        m2 = m.mpMatrix;
        for(unsigned int i = 0; i < mSize; i++, m1++, m2++)
            *m1 -= *m2;
    }

    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Mutliplies by the matrix.  <b>The current matrix columns must be
///   equal to the parameter matrices rows.</b>
///
///   \param m The matrix to mutliply with.
///
///   \return The result of matrix multiplication.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix Matrix::operator*(const Matrix& m) const
{
    assert(mpMatrix && m.mpMatrix && mCols == m.mRows && "Matrix::Error - M1 columns must equal M2 rows for multiplication.");
    double* m1;
    double* m2;
    double* m3;
    Matrix result(mRows, m.mCols);
    m1 = mpMatrix;
    m2 = m.mpMatrix;
    m3 = result.mpMatrix;
    for(unsigned int r = 0; r < result.mRows; r++) 
    {
        for(unsigned int c = 0; c < result.mCols; c++) 
        {
            *m3 = 0;
            for(unsigned int i = 0; i < mCols; i++) 
            {
                *m3 += m1[r*mCols + i]*m2[i*m.mCols + c];
            }
            m3++;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Divides by the matrix.  <b>The current matrix columns must be
///   equal to the parameter matrices rows.</b>
///
///   \param m The matrix to divide with (MUST BE A SQUARE MATRIX).
///
///   \return The result of matrix division.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix Matrix::operator/(const Matrix& m) const
{
    return *this * m.Inverse();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Mutliplies by the matrix.  <b>The current matrix columns must be
///   equal to the parameter matrices rows.</b>
///
///   \param m The matrix to mutliply with.
///
///   \return The result of matrix multiplication.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix& Matrix::operator*=(const Matrix& m)
{
    assert(mpMatrix && m.mpMatrix && mCols == m.mRows && "Matrix::Error - M1 columns must equal M2 rows for multiplication.");
    double* m1;
    double* m2;
    double* m3;
    Matrix result(mRows, m.mCols);
    m1 = mpMatrix;
    m2 = m.mpMatrix;
    m3 = result.mpMatrix;
    for(unsigned int r = 0; r < result.mRows; r++) 
    {
        for(unsigned int c = 0; c < result.mCols; c++) 
        {
            *m3 = 0;
            for(unsigned int i = 0; i < mCols; i++) 
            {
                *m3 += m1[r*mCols + i]*m2[i*m.mCols + c];
            }
            m3++;
        }
    }

    *this = result;

    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Divides by the matrix.  <b>The current matrix columns must be
///   equal to the parameter matrices rows.</b>
///
///   \param m The matrix to divide with (MUST BE A SQUARE MATRIX).
///
///   \return The result of matrix division.
///
////////////////////////////////////////////////////////////////////////////////////
Matrix& Matrix::operator/=(const Matrix& m)
{
    *this *= m.Inverse();

    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Using this function you can index into the object using two 
///   brackets.
///
///   Example:  Matrix m(5, 5);
///             m[0][1] = 3;
///             m[1][2] = 4;
///
///   \param i The row in the matrix to access.
///
///   \return A pointer to the row in the matrix.  This array can then be
///   indexed again to acces a column.
///
////////////////////////////////////////////////////////////////////////////////////
double* Matrix::operator[](const unsigned int i)
{
    assert(mpMatrix && i < mRows && "Matrix::Error - Invalid row/col.");
    return &(mpMatrix[i*mCols]);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Using this function you can index into the object using two 
///   brackets.
///
///   Example:  Matrix m(5, 5);
///             m[0][1] = 3;
///             m[1][2] = 4;
///
///   \param i The row in the matrix to access.
///
///   \return A pointer to the row in the matrix.  This array can then be
///   indexed again to acces a column.
///
////////////////////////////////////////////////////////////////////////////////////
double* Matrix::operator[](const unsigned int i) const
{
    assert(mpMatrix && i < mRows && "Matrix::Error - Invalid row/col.");
    return (double *)&(mpMatrix[i*mCols]);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Allows you to set the value at a specific [row,col] position.
///
///   \param row The row in the matrix [0,Rows()).
///   \param col The column in the matrix [0, Cols()).
///
///   \return The value at [row,col].
///
////////////////////////////////////////////////////////////////////////////////////
double& Matrix::operator()(const unsigned int row, const unsigned int col)
{
    assert(mpMatrix && row < mRows && col < mCols && "Matrix::Error - Invalid row/col.");
    return mpMatrix[row*mCols + col];
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Allows you to set the value at a specific [row,col] position.
///
///   \param row The row in the matrix [0,Rows()).
///   \param col The column in the matrix [0, Cols()).
///
///   \return The value at [row,col].
///
////////////////////////////////////////////////////////////////////////////////////
double Matrix::operator()(const unsigned int row, const unsigned int col) const
{
    assert(mpMatrix && row < mRows && col < mCols && "Matrix::Error - Invalid row/col.");
    return mpMatrix[row*mCols + col];
}


/*  End of File */
