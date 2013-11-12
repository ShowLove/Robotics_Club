////////////////////////////////////////////////////////////////////////////////////
///
///  \file types.h
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
#ifndef _JAUS_CORE_TYPES_H
#define _JAUS_CORE_TYPES_H

#include <limits.h>
#include <cxutils/packet.h>
#include <cxutils/mutex.h>
#include <cxutils/thread.h>
#include <exception>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

#include "jauscoredll.h"

#ifndef NULL
#define NULL 0
#endif

#define JAUSPP_VERSION 3

// There is a bug in Boost 1.48 where upgrade locks
// do not compile correctly on Linux/Mac
#ifdef WIN32
#define JAUS_USE_UPGRADE_LOCKS 1
#endif

//////////////////////////////////////////////////////////////////////////////////// 
///  
///  \namespace JAUS
///  \brief Name space used for the JAUS library.
///
//////////////////////////////////////////////////////////////////////////////////// 
namespace JAUS
{
    typedef CxUtils::Packet Packet;     ///<  Typedef to more easily reference Packet data.
    typedef CxUtils::Mutex  Mutex;      ///<  Typedef to more easily reference Mutex data.
    typedef CxUtils::Thread Thread;     ///<  Typedef to more easily reference Thread data.
    typedef boost::shared_lock<boost::shared_mutex> ReadLock;                       ///<  Typedef for ReadLock operation
    typedef boost::unique_lock<boost::shared_mutex> WriteLock;                      ///<  Write lock.
#if JAUS_USE_UPGRADE_LOCKS
    typedef boost::upgrade_lock<boost::shared_mutex> UpgradeLock;                   ///<  Typedef for UpgradeLock operation
    typedef boost::upgrade_to_unique_lock<boost::shared_mutex> UpgradeToUniqueLock; ///<  Typedef to upgrade to unique lock.
#endif
    typedef boost::shared_mutex SharedMutex;                                        ///<  Typedef for shared mutex.

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Exception
    ///   \brief Exception thrown for various reasons.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class Exception : public std::exception
    {
    public:
        Exception(const std::string& whatString = "NULL Ptr Access") : mWhatString(whatString) {}
        ~Exception() throw () {}
        virtual const char* what() const throw();
        virtual void Print() const;
        std::string mWhatString;    ///< The reason for the exception.
    };

    typedef unsigned char Byte;                  ///<  JAUS byte (8 bit unsigned integer).
    typedef short Short;                         ///<  JAUS short integer (16 bit signed integer).
    typedef int Int;                             ///<  JAUS integer (32 bit signed integer).
    typedef long long int Long;                  ///<  JAUS long integer (64 bit signed integer).
    typedef unsigned short UShort;               ///<  JAUS unsigned short integer (16 bit unsigned integer).
    typedef unsigned int UInt;                   ///<  JAUS unsigned integer (32 bit unsigned integer).
    typedef unsigned long long int ULong;        ///<  JAUS unsigned long integer (64 bit unsigned integer).
    typedef float Float;                         ///<  JAUS float (IEEE 32 8 bit floating point number).
    typedef double LongFloat;                    ///<  JAUS long float (IEEE 64 bit floating point number).

    const int OK                            = 1;        ///<  Shorter version of JAUS_OK.
    const int FAILURE                       = 0;        ///<  Shorter version of JAUS_FAILURE.
    //  Data type enumerations.
    const Byte BYTE                        = 0;         ///<  Type enumeration number for a byte.
    const Byte SHORT                       = 1;         ///<  Type enumeration number for a short.
    const Byte INT                         = 2;         ///<  Type enumeration number for an int.
    const Byte LONG                        = 3;         ///<  Type enumeration number for a long.
    const Byte USHORT                      = 4;         ///<  Type enumeration number for an unsigned short.
    const Byte UINT                        = 5;         ///<  Type enumeration number for an unsigned int.
    const Byte ULONG                       = 6;         ///<  Type enumeration number for an unsigned long.
    const Byte FLOAT                       = 7;         ///<  Type enumeration number for a float.
    const Byte LONG_FLOAT                  = 8;         ///<  Type enumeration number for a long float.
    const Byte RGB                         = 9;         ///<  RGB type of data.

    const Short     JAUS_SHORT_MAX      = SHRT_MAX;     ///<  Maximum value of scaled short (Short).
    const Short     JAUS_SHORT_MIN      = SHRT_MIN;     ///<  Minimum value of a scaled short (Short).
    const UShort    JAUS_USHORT_MAX     = USHRT_MAX;    ///<  Maximum value of a scaled unsigned short (UShort).
    const UShort    JAUS_USHORT_MIN     =  0;           ///<  Minimum value of scaled unsigned short (UShort).
    const Int       JAUS_INT_MIN        = INT_MIN;      ///<  Minimum value of scaled integer (Int).
    const Int       JAUS_INT_MAX        = INT_MAX;      ///<  Maximum value of scaled integer (Int).
    const UInt      JAUS_UINT_MIN       = 0;            ///<  Minimum value of scaled unsigned integer (UInt).
    const UInt      JAUS_UINT_MAX       = UINT_MAX;     ///<  Maximum value of scaled unsigned integer (UInt).
    const Long      JAUS_LONG_MIN                       ///<  Minimum value of long integer (Long).
    #ifdef _WIN32_WCE
                                        = _I64_MIN;
    #else
       #ifdef __GNUC__
            #if __GNUC__ > 3
                                        = LLONG_MIN;
            #else
                                        = LONG_LONG_MIN;
            #endif
       #else
                                        = LLONG_MIN;
       #endif
    #endif
    const Long      JAUS_LONG_MAX                       ///<  Maximum value of long integer (Long).
    #ifdef _WIN32_WCE
                                        = _I64_MAX;
    #else
        #ifdef __GNUC__
            #if __GNUC__ > 3
                                        = LLONG_MAX;
            #else
                                        = LONG_LONG_MAX;
            #endif
        #else
                                        = LLONG_MAX;    
        #endif
    #endif
    const ULong JAUS_ULONG_MIN          = 0;            ///<  Minimum value of unsigned long integer (ULong).
    const ULong JAUS_ULONG_MAX                          ///<  Maximum value of unsigned long integer (ULong).
    #ifdef _WIN32_WCE
                                        = _UI64_MAX;
    #else
        #ifdef __GNUC__
            #if __GNUC__ > 3
                                        = ULLONG_MAX;
            #else
                                        = ULONG_LONG_MAX;
            #endif
        #else
                                        = ULLONG_MAX;    
        #endif
    #endif

    const UInt BYTE_SIZE                = 1;          ///<  Size in bytes.
    const UInt SHORT_SIZE               = 2;          ///<  Size in bytes.
    const UInt USHORT_SIZE              = 2;          ///<  Size in bytes.
    const UInt INT_SIZE                 = 4;          ///<  Size in bytes.
    const UInt UINT_SIZE                = 4;          ///<  Size in bytes.
    const UInt FLOAT_SIZE               = 4;          ///<  Size in bytes.
    const UInt LONG_SIZE                = 8;          ///<  Size in bytes.
    const UInt ULONG_SIZE               = 8;          ///<  Size in bytes.
    const UInt LONG_FLOAT_SIZE          = 8;          ///<  Size in bytes.

    const UInt BYTE_BITS                = 8;          ///<  Size in bits of Byte.
    const UInt SHORT_BITS               = 16;         ///<  Size in bits of Short.
    const UInt USHORT_BITS              = 16;         ///<  Size in bits of UShort.
    const UInt INT_BITS                 = 32;         ///<  Size in bits of Int.
    const UInt UINT_BITS                = 32;         ///<  Size in bits of UInt.
    const UInt FLOAT_BITS               = 32;         ///<  Size in bits of Float.
    const UInt LONG_BITS                = 64;         ///<  Size in bits of Long.
    const UInt ULONG_BITS               = 64;         ///<  Size in bits of ULong.
    const UInt LONG_FLOAT_BITS          = 64;         ///<  Size in bits of LongFloat.
    
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class VarType
    ///   \brief VarType stands for Variable Type.  This class is used to deal with
    ///          fields in JAUS messages which vary based on what kind of data needs
    ///          to be stored.  This class encapsulates the storage of variable
    ///          type information.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL VarType
    {
    public:
        VarType();
        VarType(const Byte v);
        VarType(const Short v);
        VarType(const Int v);
        VarType(const Long v);
        VarType(const UShort v);
        VarType(const UInt v);
        VarType(const ULong v);
        VarType(const Float v);
        VarType(const LongFloat v);
        VarType(const Byte r, const Byte g, const Byte b);
        VarType(const VarType& vt);
        virtual ~VarType();
        // Get the variable type information.
        Byte Type() const;
        // Get the size of the variable type.
        Byte Size() const;
        // Return as a specific type of variable.
        Byte ToByte() const;
        // Return as a specific type of variable.
        Short ToShort() const;
        // Return as a specific type of variable.
        Int ToInt() const;
        // Return as a specific type of variable.
        Long ToLong() const;
        // Return as a specific type of variable.
        UShort ToUShort() const;
        // Return as a specific type of variable.
        UInt ToUInt() const;
        // Return as a specific type of variable.
        ULong ToULong() const;
        // Return as a specific type of variable.
        Float ToFloat() const;
        // Return as a specific type of variable.
        LongFloat ToLongFloat() const;
        // Return as a specific type of variable.
        Byte* ToRGB() const;
        // Print variable information to console.
        void Print() const;
        // Convert value to a string.
        std::string ToString() const;
        // Check fi the values are equal.
        bool Equals(const VarType& vt, const double ferror = .000001) const;
        // Run a test case to validate methods of the class.
        static int RunTestCase();
        // Compare values of VarType.
        bool operator==(const VarType& vt) const;
        // Compare values of to see if they are equal.
        bool operator==(const Byte v) const;
        // Compare values of to see if they are equal.
        bool operator==(const Short v) const;
        // Compare values of to see if they are equal.
        bool operator==(const Int v) const;
        // Compare values of to see if they are equal.
        bool operator==(const Long v) const;
        // Compare values of to see if they are equal.
        bool operator==(const UShort v) const;
        // Compare values of to see if they are equal.
        bool operator==(const UInt v) const;
        // Compare values of to see if they are equal.
        bool operator==(const ULong v) const;
        // Compare values of to see if they are equal.
        bool operator==(const Float v) const;
        // Compare values of to see if they are equal.
        bool operator==(const LongFloat v) const; 
        // Compare values of VarType
        bool operator!=(const VarType& vt) const;
        // Compare values of to see if they are not equal
        bool operator!=(const Byte v) const;
        // Compare values of to see if they are not equal
        bool operator!=(const Short v) const;
        // Compare values of to see if they are not equal
        bool operator!=(const Int v) const;
        // Compare values of to see if they are not equal
        bool operator!=(const Long v) const;
        // Compare values of to see if they are not equal
        bool operator!=(const UShort v) const;
        // Compare values of to see if they are not equal
        bool operator!=(const UInt v) const;
        // Compare values of to see if they are not equal
        bool operator!=(const ULong v) const;
        // Compare values of to see if they are not equal
        bool operator!=(const Float v) const;
        // Compare values of to see if they are not equal
        bool operator!=(const LongFloat v) const; 
        // Sets the type as Byte and copies the value.
        VarType& operator=(const Byte v);
        // Sets the type as Short and copies the value.
        VarType& operator=(const Short v);
        // Sets the type as Int and copies the value.
        VarType& operator=(const Int v);
        // Sets the type as Long and copies the value.
        VarType& operator=(const Long v);
        // Sets the type as UShort and copies the value.
        VarType& operator=(const UShort v);
        // Sets the type as UInt and copies the value.
        VarType& operator=(const UInt v);
        // Sets the type as ULong and copies the value.
        VarType& operator=(const ULong v);
        // Sets the type as Float and copies the value.
        VarType& operator=(const Float v);
        // Sets the type as LongFloat and copies the value.
        VarType& operator=(const LongFloat v); 
        // Sets equal to.
        VarType& operator=(const VarType& vt);
        // Sets the type as RGB and copies the value.
        VarType& operator()(const Byte r, const Byte g, const Byte b);
    private:
        void* mpData;           ///<  Pointer to data.
        Byte mType;             ///<  Type of data.
        unsigned int mLength;   ///<  Size of data in bytes.
    };

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Limits
    ///   \brief Contains constants for limit values of data members of classes.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Limits
    {
    public:
        Limits() {}
        virtual ~Limits() {}
    };

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class PresenceVector
    ///   \brief This class contains bit masks for bitwise operations on the
    ///          presence vector for this message.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL PresenceVector
    {
    public:
        PresenceVector() {}
        virtual ~PresenceVector() {}
    };
}

#endif
/*  End of File */
