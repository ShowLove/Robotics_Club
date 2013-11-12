////////////////////////////////////////////////////////////////////////////////////
///
///  \file scaledinteger.h
///  \brief This file contains the ScaledInteger class which is used for the
///         conversion to/from scaled integers and real numbers as defined in the
///         JAUS Standard.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 27 September 2009
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
#ifndef __JAUS_SCALED_INTEGERS_H
#define __JAUS_SCALED_INTEGERS_H

#include "jaus/core/types.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ScaledInteger
    ///   \brief Methods for conversion to/from scaled integers and real numbers.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL ScaledInteger
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \enum Type
        ///   \brief Enumerations of the different types of scaled integers.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        enum Type 
        {
            Byte = 10,
            Short,
            UShort,
            Int,
            UInt,
            Long,
            ULong
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Convert a floating point value to a scaled integer and write to
        ///          the packet.
        ///
        ///   \param[out] packet Packet to write to.
        ///   \param[in] real Floating point number to write.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///   \param[in] type Type of scaled integer to write.
        ///   \param[in] pos The byte position within the stream to write at.  If pos is
        ///              equal to UINT_MAX, then the internal write position is used,
        ///              which is the default behavior.
        ///
        ///   \return Number of bytes written, FAILURE on error.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static int Write(Packet& packet, 
                                const LongFloat real, 
                                const LongFloat upper,
                                const LongFloat lower, 
                                const Type type,
                                const unsigned int pos = UINT_MAX)
        {
            //  Determine what type of scaled integer to
            //  convert to, do so, then write the data into
            //  the packet.
            switch( type )
            {
                case ScaledInteger::Byte:
                    {
                        return packet.Write(ScaledInteger::ToScaledByte(real, upper, lower), pos);
                    }
                    break;
                case ScaledInteger::UShort:
                    {
                        return packet.Write(ScaledInteger::ToScaledUShort(real, upper, lower), pos);
                    }
                    break;
                case ScaledInteger::Short:
                    {
                        return packet.Write(ScaledInteger::ToScaledShort(real, upper, lower), pos);
                    }
                    break;
                case ScaledInteger::Int:
                    {
                        return packet.Write(ScaledInteger::ToScaledInt(real, upper, lower), pos);
                    }
                    break;
                case ScaledInteger::UInt:
                    {
                        return packet.Write(ScaledInteger::ToScaledUInt(real, upper, lower), pos);
                    }
                    break;
                case ScaledInteger::Long:
                    {
                        return packet.Write(ScaledInteger::ToScaledLong(real, upper, lower), pos);
                    }
                    break;
                case ScaledInteger::ULong:
                    {
                        return packet.Write(ScaledInteger::ToScaledULong(real, upper, lower), pos);
                    }
                    break;
                default:
                    return FAILURE;
                    break;
            }
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Read a real number at the index in the packet that is a scaled int.
        ///
        ///   \param[in] packet The packet to read from.
        ///   \param[out] val The value read.
        ///   \param[in] upper The upper limit of real.
        ///   \param[in] lower The lower limit of real.
        ///   \param[in] type What type of scaled integer to convert from (ScaledInteger::Int,
        ///               ScaledInteger::UInt, ScaledInteger::Short, ScaledInteger::UShort, or
        ///               ScaledInteger::Byte).
        ///   \param[in] pos The byte position within the stream to read at.  If pos is
        ///              equal to UINT_MAX, then the internal read position is used,
        ///              which is the default behavior.
        ///
        ///   \return The number of bytes read, 0 if nothing read.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static int Read(const Packet& packet,
                               LongFloat& val,
                               const LongFloat upper,
                               const LongFloat lower,
                               const ScaledInteger::Type type,
                               const unsigned int pos = UINT_MAX)
        {
            int read = 0;
            switch(type)
            {
            case ScaledInteger::Byte:
                {
                    JAUS::Byte scaled;
                    read = packet.Read(scaled, pos);
                    if(read > 0)
                    {
                        val = ScaledInteger::ToDouble(scaled, upper, lower);
                    }
                }
                break;
            case ScaledInteger::Short:
                {
                    JAUS::Short scaled;
                    read = packet.Read(scaled, pos);
                    if(read > 0)
                    {
                        val = ScaledInteger::ToDouble(scaled, upper, lower);
                    }
                }
                break;
            case ScaledInteger::UShort:
                {
                    JAUS::UShort scaled;
                    read = packet.Read(scaled, pos);
                    if(read > 0)
                    {
                        val = ScaledInteger::ToDouble(scaled, upper, lower);
                    }
                }
                break;
            case ScaledInteger::Int:
                {
                    JAUS::Int scaled;
                    read = packet.Read(scaled, pos);
                    if(read > 0)
                    {
                        val = ScaledInteger::ToDouble(scaled, upper, lower);
                    }
                }
                break;
            case ScaledInteger::UInt:
                {
                    JAUS::UInt scaled;
                    read = packet.Read(scaled, pos);
                    if(read > 0)
                    {
                        val = ScaledInteger::ToDouble(scaled, upper, lower);
                    }
                }
                break;
            case ScaledInteger::Long:
                {
                    JAUS::Long scaled;
                    read = packet.Read(scaled, pos);
                    if(read > 0)
                    {
                        val = ScaledInteger::ToDouble(scaled, upper, lower);
                    }
                }
                break;
            case ScaledInteger::ULong:
                {
                    JAUS::ULong scaled;
                    read = packet.Read(scaled, pos);
                    if(read > 0)
                    {
                        val = ScaledInteger::ToDouble(scaled, upper, lower);
                    }
                }
                break;
            default:
                read = 0;
                break;
            };

            return read;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static double ToDouble(const JAUS::Byte scaled, const double upper, const double lower)
        {
            return scaled * (upper - lower) / ByteRange + lower;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static float ToFloat(const JAUS::Byte scaled, const double upper, const double lower)
        {
            return (float)(scaled * (upper - lower) / ByteRange + lower);
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::Byte ToScaledByte(const double real, const double upper, const double lower)
        {
            return (JAUS::Byte)((real - lower) / ((upper - lower) / ByteRange));
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::Byte ToScaledByte(const float real, const float upper, const float lower)
        {
            return (JAUS::Byte)((real - lower) / ((upper - lower) / ByteRange));
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static double ToDouble(const JAUS::Short scaled, const double upper, const double lower)
        {
            return scaled * ((upper - lower) / Int16Range) + (upper + lower) / 2.0;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static float ToFloat(const JAUS::Short scaled, const double upper, const double lower)
        {
            return (float)(scaled * ((upper - lower) / Int16Range) + (upper + lower) / 2.0);
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::Short ToScaledShort(const double real, const double upper, const double lower)
        {
            return (JAUS::Short)((real - (upper + lower) / 2.0) * (Int16Range / (upper - lower)));
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::Short ToScaledShort(const float real, const float upper, const float lower)
        {
            return (JAUS::Short)((real - (upper + lower) / 2.0) * (Int16Range / (upper - lower)));
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static double ToDouble(const JAUS::UShort scaled, const double upper, const double lower)
        {
            return scaled * (upper - lower) / UInt16Range + lower;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static float ToFloat(const JAUS::UShort scaled, const double upper, const double lower)
        {
            return (float)(scaled * (upper - lower) / UInt16Range + lower);
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::UShort ToScaledUShort(const double real, const double upper, const double lower)
        {
            return (JAUS::UShort)((real - lower) / ((upper - lower) / UInt16Range));
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::UShort ToScaledUShort(const float real, const float upper, const float lower)
        {
            return (JAUS::UShort)((real - lower) / ((upper - lower) / UInt16Range));
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static double ToDouble(const JAUS::Int scaled, double upper, double lower)
        {
            return scaled * ((upper - lower) / Int32Range) + (upper + lower) / 2.0;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static float ToFloat(const JAUS::Int scaled, const float upper, const float lower)
        {
            return (float)(scaled * ((upper - lower) / Int32Range) + (upper + lower) / 2.0);
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::Int ToScaledInt(const double real, double upper, double lower)
        {
            return (JAUS::Int)((real - (upper + lower) / 2.0) * (Int32Range / (upper - lower)));
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::Int ToScaledInt(const float real, const float upper, const float lower)
        {
            return (JAUS::Int)((real - (upper + lower) / 2.0) * (Int32Range / (upper - lower)));
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static double ToDouble(const JAUS::UInt scaled, double upper, double lower)
        {
            return scaled * (upper - lower) / UInt32Range + lower;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static float ToFloat(const JAUS::UInt scaled, const float upper, const float lower)
        {
            return (float)(scaled * (upper - lower) / UInt32Range + lower);
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::UInt ToScaledUInt(const double real, double upper, double lower)
        {
            return (JAUS::UInt)((real - lower) / ((upper - lower) / UInt32Range));
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::UInt ToScaledUInt(const float real, const float upper, const float lower)
        {
            return (JAUS::UInt)((real - lower) / ((upper - lower) / UInt32Range));
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static double ToDouble(const JAUS::Long scaled, double upper, double lower)
        {
            return scaled / 2.0 * ((upper - lower) / Int64HalfRange) + (upper + lower) / 2.0;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static float ToFloat(const JAUS::Long scaled, const float upper, const float lower)
        {
            return (float)(scaled / 2.0 * ((upper - lower) / Int64HalfRange) + (upper + lower) / 2.0);
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::Long ToScaledLong(const double real, double upper, double lower)
        {
            return (JAUS::Long)((real - (upper + lower) / 2.0) * 2 * (Int64HalfRange / (upper - lower)));
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::Long ToScaledLong(const float real, const float upper, const float lower)
        {
            return (JAUS::Long)((real - (upper + lower) / 2.0) * 2 * (Int64HalfRange / (upper - lower)));
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static double ToDouble(const JAUS::ULong scaled, double upper, double lower)
        {
            return scaled * (upper - lower) / UInt64HalfRange + lower;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts a scaled integer to a real number based on the upper and
        ///          lower limits of the real number.
        ///
        ///   \param[in] scaled Scaled integer to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Real number conversion of the scaled integer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static float ToFloat(const JAUS::ULong scaled, const float upper, const float lower)
        {
            return (float)(scaled * (upper - lower) / UInt64HalfRange + lower);
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::ULong ToScaledULong(const double real, double upper, double lower)
        {
            return (JAUS::ULong)((real - lower) / ((upper - lower) / UInt64HalfRange));
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Converts from a real number to a Scaled Integer value.
        ///
        ///   \param[in] real Real number to convert.
        ///   \param[in] upper Upper limit of real number.
        ///   \param[in] lower Lower limit of real number.
        ///
        ///   \return Scaled integer representation of the real number.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static JAUS::ULong ToScaledULong(const float real, const float upper, const float lower)
        {
            return (JAUS::ULong)((real - lower) / ((upper - lower) / UInt64HalfRange));
        }
    private:
        static double ByteRange;        ///<  Range of values for a Byte.
        static double UInt64HalfRange;  ///<  Half the range of values for 64 bit unsigned int.
        static double Int64HalfRange;   ///<  Half the range of values for 64 bit integer.
        static double UInt32Range;      ///<  Range of values for a 32 bit unsigned int.
        static double Int32Range;       ///<  Range of values for a 32 bit int.
        static double UInt16Range;      ///<  Range of values for 16 bit unsigned int.
        static double Int16Range;       ///<  Range of values for 16 bit int.
        static double Epsilon;          ///<  Small number to prevent divide by zero errors.
    };
}


#endif
/*  End of File */
