////////////////////////////////////////////////////////////////////////////////////
///
///  \file types.cpp
///  \brief This file contains definitions of and about basic data types
///  used throughout the JAUS++ library.
///
///  Author(s): Daniel Barber
///  Created: 26 September 2009
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
#include "jaus/core/types.h"
#include <iostream>
#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;
using namespace JAUS;

const char* Exception::what() const throw()
{
    return std::string(std::string("[JAUS ++ Exception] - ") + mWhatString).c_str();
}

void Exception::Print() const { std::cout << what() << std::endl; }

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.  Sets default type of Byte, with value of 0.
///
////////////////////////////////////////////////////////////////////////////////////
VarType::VarType() : mpData(0),
                     mType(BYTE),
                     mLength(1)
{
    mpData = malloc(8);
    memset(mpData, 0, 8);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets the type variable, and copies the value.
///
///   \param[in] v The type of variable to be, and value.
///
////////////////////////////////////////////////////////////////////////////////////
VarType::VarType(const Byte v) : mpData(0),
                                 mType(BYTE),
                                 mLength(1)
{
    mpData = malloc(8);
    memset(mpData, 0, 8);
    memcpy(mpData, &v, 1);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets the type variable, and copies the value.
///
///   \param[in] v The type of variable to be, and value.
///
////////////////////////////////////////////////////////////////////////////////////
VarType::VarType(const Short v) : mpData(0),
                                  mType(SHORT),
                                  mLength(2)
{
    mpData = malloc(8);
    memset(mpData, 0, 8);
    memcpy(mpData, &v, mLength);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets the type variable, and copies the value.
///
///   \param[in] v The type of variable to be, and value.
///
////////////////////////////////////////////////////////////////////////////////////
VarType::VarType(const Int v) : mpData(0),
                                mType(INT),
                                mLength(4)
{
    mpData = malloc(8);
    memset(mpData, 0, 8);
    memcpy(mpData, &v, mLength);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets the type variable, and copies the value.
///
///   \param[in] v The type of variable to be, and value.
///
////////////////////////////////////////////////////////////////////////////////////
VarType::VarType(const Long v) : mpData(0),
                                 mType(LONG),
                                 mLength(8)
{
    mpData = malloc(8);
    memset(mpData, 0, 8);
    memcpy(mpData, &v, mLength);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets the type variable, and copies the value.
///
///   \param[in] v The type of variable to be, and value.
///
////////////////////////////////////////////////////////////////////////////////////
VarType::VarType(const UShort v) : mpData(0),
                                   mType(USHORT),
                                   mLength(2)
{
    mpData = malloc(8);
    memset(mpData, 0, 8);
    memcpy(mpData, &v, mLength);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets the type variable, and copies the value.
///
///   \param[in] v The type of variable to be, and value.
///
////////////////////////////////////////////////////////////////////////////////////
VarType::VarType(const UInt v) : mpData(0),
                                 mType(UINT),
                                 mLength(4)
{
    mpData = malloc(8);
    memset(mpData, 0, 8);
    memcpy(mpData, &v, mLength);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets the type variable, and copies the value.
///
///   \param[in] v The type of variable to be, and value.
///
////////////////////////////////////////////////////////////////////////////////////
VarType::VarType(const ULong v) : mpData(0),
                                  mType(ULONG),
                                  mLength(8)
{
    mpData = malloc(8);
    memset(mpData, 0, 8);
    memcpy(mpData, &v, mLength);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets the type variable, and copies the value.
///
///   \param[in] v The type of variable to be, and value.
///
////////////////////////////////////////////////////////////////////////////////////
VarType::VarType(const Float v) : mpData(0),
                                  mType(FLOAT),
                                  mLength(4)
{
    mpData = malloc(8);
    memset(mpData, 0, 8);
    memcpy(mpData, &v, mLength);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets the type variable, and copies the value.
///
///   \param[in] v The type of variable to be, and value.
///
////////////////////////////////////////////////////////////////////////////////////
VarType::VarType(const LongFloat v) : mpData(0),
                                      mType(LONG_FLOAT),
                                      mLength(8)
{
    mpData = malloc(8);
    memset(mpData, 0, 8);
    memcpy(mpData, &v, 8);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets the type variable, and copies the value.
///
///   \param[in] r Red color channel value.
///   \param[in] g Green color channel value.
///   \param[in] b Blue color channel value.
///
////////////////////////////////////////////////////////////////////////////////////
VarType::VarType(const Byte r, const Byte g, const Byte b) : mpData(0),
                                                             mType(RGB),
                                                             mLength(3)
{
    mpData = malloc(8);
    Byte * temp = ( (Byte *)(mpData) );
    memset(mpData, 0, 8);
    if(temp)
    {
        temp[0] = r;
        temp[1] = g;
        temp[2] = b;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets the type variable, and copies the value.
///
///   \param[in] vt The variable type data to copy.
///
////////////////////////////////////////////////////////////////////////////////////
VarType::VarType(const VarType& vt) : mpData(0),
                                      mType(BYTE),
                                      mLength(1)
{
    mpData = malloc(8);
    memset(mpData, 0, 8);
    *this = vt;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.  Deletes any memory.
///
////////////////////////////////////////////////////////////////////////////////////
VarType::~VarType()
{
    free(mpData);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Type of variable stored in VarType.  Values are BYTE, SHORT,
///           INT, LONG, USHORT, UINT, JAUS_ULONG,
///           FLOAT, LONG_FLOAT, and JAUS_RGB.
///
////////////////////////////////////////////////////////////////////////////////////
Byte VarType::Type() const
{
    return mType;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The size of the variable data in bytes.
///
////////////////////////////////////////////////////////////////////////////////////
Byte VarType::Size() const
{
    return (Byte)(mLength);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the variable type data to the type specified and returns the
///          value.
///
///   Make sure you check what type of data is stored using GetType before
///   calling this method, otherwie your data may be invalid.
///
///   \return The variable type converted to the desired type.
///
////////////////////////////////////////////////////////////////////////////////////
Byte VarType::ToByte() const
{
    return *(Byte *)(mpData);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the variable type data to the type specified and returns the
///          value.
///
///   Make sure you check what type of data is stored using GetType before
///   calling this method, otherwie your data may be invalid.
///
///   \return The variable type converted to the desired type.
///
////////////////////////////////////////////////////////////////////////////////////
Short VarType::ToShort() const
{
    return *(Short *)(mpData);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the variable type data to the type specified and returns the
///          value.
///
///   Make sure you check what type of data is stored using GetType before
///   calling this method, otherwie your data may be invalid.
///
///   \return The variable type converted to the desired type.
///
////////////////////////////////////////////////////////////////////////////////////
Int VarType::ToInt() const
{
    return *(Int *)(mpData);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the variable type data to the type specified and returns the
///          value.
///
///   Make sure you check what type of data is stored using GetType before
///   calling this method, otherwie your data may be invalid.
///
///   \return The variable type converted to the desired type.
///
////////////////////////////////////////////////////////////////////////////////////
Long VarType::ToLong() const
{
    return *(Long *)(mpData);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the variable type data to the type specified and returns the
///          value.
///
///   Make sure you check what type of data is stored using GetType before
///   calling this method, otherwie your data may be invalid.
///
///   \return The variable type converted to the desired type.
///
////////////////////////////////////////////////////////////////////////////////////
UShort VarType::ToUShort() const
{
    return *(UShort *)(mpData);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the variable type data to the type specified and returns the
///          value.
///
///   Make sure you check what type of data is stored using GetType before
///   calling this method, otherwie your data may be invalid.
///
///   \return The variable type converted to the desired type.
///
////////////////////////////////////////////////////////////////////////////////////
UInt VarType::ToUInt() const
{
    return *(UInt *)(mpData);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the variable type data to the type specified and returns the
///          value.
///
///   Make sure you check what type of data is stored using GetType before
///   calling this method, otherwie your data may be invalid.
///
///   \return The variable type converted to the desired type.
///
////////////////////////////////////////////////////////////////////////////////////
ULong VarType::ToULong() const
{
    return *(ULong *)(mpData);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the variable type data to the type specified and returns the
///          value.
///
///   Make sure you check what type of data is stored using GetType before
///   calling this method, otherwie your data may be invalid.
///
///   \return The variable type converted to the desired type.
///
////////////////////////////////////////////////////////////////////////////////////
Float VarType::ToFloat() const
{
    return *(Float *)(mpData);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the variable type data to the type specified and returns the
///          value.
///
///   Make sure you check what type of data is stored using GetType before
///   calling this method, otherwie your data may be invalid.
///
///   \return The variable type converted to the desired type.
///
////////////////////////////////////////////////////////////////////////////////////
LongFloat VarType::ToLongFloat() const
{
    return *(LongFloat *)(mpData);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the variable type data to the type specified and returns the
///          value.
///
///   Make sure you check what type of data is stored using GetType before
///   calling this method, otherwie your data may be invalid.
///
///   \return The variable type converted to the desired type.  In this case
///           a pointer to a 3 byte RGB array.
///
////////////////////////////////////////////////////////////////////////////////////
Byte* VarType::ToRGB() const
{
    return (Byte *)(mpData);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints the value to the console.
///
////////////////////////////////////////////////////////////////////////////////////
void VarType::Print() const
{
    cout << ToString() << endl;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The variable data converted to a string.
///
////////////////////////////////////////////////////////////////////////////////////
std::string VarType::ToString() const
{
    std::string str;
    char buff[128];
    memset(buff, 0, 128);

    switch(mType)
    {
    case BYTE:
        sprintf(buff, "%d", ToByte());
        break;
    case SHORT:
        sprintf(buff, "%d", ToShort());
        break;
    case INT:
        sprintf(buff, "%d", ToInt());
        break;
    case LONG:
        sprintf(buff, "%ld", (long)ToLong());
        break;
    case USHORT:
        sprintf(buff, "%u", ToUShort());
        break;
    case UINT:
        sprintf(buff, "%u", ToUInt());
        break;
    case ULONG:
        sprintf(buff, "%lu", (unsigned long)ToULong());
        break;
    case FLOAT:
        sprintf(buff, "%f", ToFloat());
        break;
    case LONG_FLOAT:
        sprintf(buff, "%lf", ToLongFloat());
        break;
    case RGB:
        sprintf(buff,
                "[%u,%u,%u]",
                ((Byte*)(mpData))[0],
                ((Byte*)(mpData))[1],
                ((Byte*)(mpData))[2]);
        break;
    default:
        str = "Invalid.";
        break;
    }

    str = buff;

    return str;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the values to see if they are equal.
///
///   In the event that the variable types are both floating point
///   values, then the ferror parameter is used.  This parameter is used to
///   set the minimum allowed difference between to floating pointer numbers
///   for them to be considered the same using the following equation:
///
///   equal == fabs( float1 - float2) < ferror.
///
///   \param[in] vt The variable type to compare with.
///   \param[in] ferror If both values are floating point, then this is the
///                     minimum differance allowed for them to be considered
///                     equal.
///
///   \return True if equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::Equals(const VarType& vt, const double ferror) const
{
    bool result = false;

    if( mType == vt.mType )
    {
        if( mType == FLOAT)
        {
            if( fabs( ToFloat() - vt.ToFloat() ) < (float)ferror )
            {
                result = true;
            }
        }
        else if(mType == LONG_FLOAT)
        {
            if( fabs( ToLongFloat() - vt.ToLongFloat() ) < ferror )
            {
                result = true;
            }
        }
        else
        {
            result = memcmp(mpData, vt.mpData, mLength) == 0 ? true : false;
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a test case to validate that the class works correctly.
///
///   \return JAUS_OK on success, JAUS_FAILURE otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
int VarType::RunTestCase()
{
    int result = 0;

    VarType v1, v2;

    v1 = 3.1415;
    v2 = 3.1415;
    // Compare to Long Float types.
    if( v1 == v2 )
    {
        v1 = (Byte)(250);
        // Verify it changed the type and value correctly.
        if( v1.Size() == 1 &&
            v1.Type() == BYTE &&
            v1.ToByte() == 250)
        {
            // Verify != operator
            v2 = (Byte)(255);
            if( v1 != v2 )
            {
                // Verify == works correctly.
                v2 = (Int)(250);
                if( v1 != v2 )
                {
                    v1 = (UInt)(555);
                    if( v1.ToString() == "555" &&
                        v1 == (UInt)(555))
                    {
                        result = 1;
                    }
                }
            }
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if values are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator==(const VarType& vt) const
{
    bool result = false;

    if( mType == vt.mType )
    {
        if( mType == FLOAT)
        {
            if( fabs( ToFloat() - vt.ToFloat() ) < .000001 )
            {
                result = true;
            }
        }
        else if(mType == LONG_FLOAT)
        {
            if( fabs( ToLongFloat() - vt.ToLongFloat() ) < .000001 )
            {
                result = true;
            }
        }
        else
        {
            result = memcmp(mpData, vt.mpData, mLength) == 0 ? true : false;
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if values are not equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator!=(const VarType& vt) const
{
    bool result = true;

    if( mType == vt.mType )
    {
        if( mType == FLOAT)
        {
            if( fabs( ToFloat() - vt.ToFloat() ) < .000001 )
            {
                result = false;
            }
        }
        else if(mType == LONG_FLOAT)
        {
            if( fabs( ToLongFloat() - vt.ToLongFloat() ) < .000001 )
            {
                result = false;
            }
        }
        else
        {
            result = memcmp(mpData, vt.mpData, mLength) == 0 ? false : true;
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator==(const Byte v) const
{
    return *this == VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator==(const Short v) const
{
    return *this == VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator==(const Int v) const
{
    return *this == VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator==(const Long v) const
{
    return *this == VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator==(const UShort v) const
{
    return *this == VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator==(const UInt v) const
{
    return *this == VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator==(const ULong v) const
{
    return *this == VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator==(const Float v) const
{
    return *this == VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator==(const LongFloat v) const
{
    return *this == VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator!=(const Byte v) const
{
    return *this != VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator!=(const Short v) const
{
    return *this != VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator!=(const Int v) const
{
    return *this != VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator!=(const Long v) const
{
    return *this != VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator!=(const UShort v) const
{
    return *this != VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator!=(const UInt v) const
{
    return *this != VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator!=(const ULong v) const
{
    return *this != VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator!=(const Float v) const
{
    return *this != VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares the variable type and value for equality.
///
///   \param[in] v The variable to compare type and value with.
///
///   \return True if type and value are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VarType::operator!=(const LongFloat v) const
{
    return *this != VarType(v);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to the variable type and value.
///
///   \param[in] v The variable type and value the VarType should be.
///
///   \return Updated version of VarType.
///
////////////////////////////////////////////////////////////////////////////////////
VarType& VarType::operator=(const Byte v)
{
    memset(mpData, 0, 8);
    mType = BYTE;
    mLength = 1;
    memcpy(mpData, &v, mLength);
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to the variable type and value.
///
///   \param[in] v The variable type and value the VarType should be.
///
///   \return Updated version of VarType.
///
////////////////////////////////////////////////////////////////////////////////////
VarType& VarType::operator=(const Short v)
{
    memset(mpData, 0, 8);
    mType = SHORT;
    mLength = 2;
    memcpy(mpData, &v, mLength);
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to the variable type and value.
///
///   \param[in] v The variable type and value the VarType should be.
///
///   \return Updated version of VarType.
///
////////////////////////////////////////////////////////////////////////////////////
VarType& VarType::operator=(const Int v)
{
    memset(mpData, 0, 8);
    mType = INT;
    mLength = 4;
    memcpy(mpData, &v, mLength);
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to the variable type and value.
///
///   \param[in] v The variable type and value the VarType should be.
///
///   \return Updated version of VarType.
///
////////////////////////////////////////////////////////////////////////////////////
VarType& VarType::operator=(const Long v)
{
    memset(mpData, 0, 8);
    mType = LONG;
    mLength = 8;
    memcpy(mpData, &v, mLength);
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to the variable type and value.
///
///   \param[in] v The variable type and value the VarType should be.
///
///   \return Updated version of VarType.
///
////////////////////////////////////////////////////////////////////////////////////
VarType& VarType::operator=(const UShort v)
{
    memset(mpData, 0, 8);
    mType = USHORT;
    mLength = 2;
    memcpy(mpData, &v, mLength);
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to the variable type and value.
///
///   \param[in] v The variable type and value the VarType should be.
///
///   \return Updated version of VarType.
///
////////////////////////////////////////////////////////////////////////////////////
VarType& VarType::operator=(const UInt v)
{
    memset(mpData, 0, 8);
    mType = UINT;
    mLength = 4;
    memcpy(mpData, &v, mLength);
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to the variable type and value.
///
///   \param[in] v The variable type and value the VarType should be.
///
///   \return Updated version of VarType.
///
////////////////////////////////////////////////////////////////////////////////////
VarType& VarType::operator=(const ULong v)
{
    memset(mpData, 0, 8);
    mType = ULONG;
    mLength = 8;
    memcpy(mpData, &v, mLength);
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to the variable type and value.
///
///   \param[in] v The variable type and value the VarType should be.
///
///   \return Updated version of VarType.
///
////////////////////////////////////////////////////////////////////////////////////
VarType& VarType::operator=(const Float v)
{
    memset(mpData, 0, 8);
    mType = FLOAT;
    mLength = 4;
    memcpy(mpData, &v, mLength);
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to the variable type and value.
///
///   \param[in] v The variable type and value the VarType should be.
///
///   \return Updated version of VarType.
///
////////////////////////////////////////////////////////////////////////////////////
VarType& VarType::operator=(const LongFloat v)
{
    memset(mpData, 0, 8);
    mType = LONG_FLOAT;
    mLength = 8;
    memcpy(mpData, &v, mLength);
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
VarType& VarType::operator=(const VarType& vt)
{
    if(this != &vt)
    {
        memcpy(mpData, vt.mpData, 8);
        mLength = vt.mLength;
        mType = vt.mType;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the VarType to a JAUS_RGB, and assigns the values.
///
///   \param[in] r Red component of RGB triplet.
///   \param[in] g Green component of RGB triplet.
///   \param[in] b Blue component of RGB triplet.
///
///   \return Updated version of VarType.
///
////////////////////////////////////////////////////////////////////////////////////
VarType& VarType::operator()(const Byte r, const Byte g, const Byte b)
{
    memset(mpData, 0, 8);
    mType = RGB;
    mLength = 3;
    ((Byte *)(mpData))[0] = r;
    ((Byte *)(mpData))[1] = g;
    ((Byte *)(mpData))[2] = b;
    return *this;
}

/*  End of File */
