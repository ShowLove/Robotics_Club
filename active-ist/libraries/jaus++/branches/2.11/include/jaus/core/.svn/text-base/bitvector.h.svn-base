////////////////////////////////////////////////////////////////////////////////////
///
///  \file bitvector.h
///  \brief This file contains the BitVector class which is used for manipulation
///         of presence vector data in JAUS++.
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
#ifndef __JAUS_BIT_PRESENCE_VECTOR_H
#define __JAUS_BIT_PRESENCE_VECTOR_H

#include "jaus/core/types.h"

#ifdef __cplusplus

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class BitVector
    ///   \brief Methods for manipulation of bit vectors (such as presence vectors)
    ///          in JAUS.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL BitVector
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Sets the value of a bit at a specific position to 0 or 1.
        ///
        ///   \param[out] pv The bit vector variable to modify.
        ///   \param[in] bit The bit number to edit [0, 7]. Where 0 is the least 
        ///              significant bit.
        ///   \param[in] val The value to set the bit to (true = 1, false = 0).
        ///
        ///   \return OK if set, otherwise FAILURE.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static int SetBit(Byte& pv, const unsigned int bit, const bool val = true)
        {
            if (bit < BYTE_BITS)
            {
                 if (val)
                 {
                     pv |= (Byte)(0x01 << bit);
                 }
                 else
                 {
                     pv &= (Byte)(~(0x01 << bit));
                 }
                 return OK;
            }
            return FAILURE;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Sets the value of a bit at a specific position to 0 or 1.
        ///
        ///   \param[out] pv The bit vector variable to modify.
        ///   \param[in] bit The bit number to edit [0, 15]. Where 0 is the least 
        ///              significant bit.
        ///   \param[in] val The value to set the bit to (true = 1, false = 0).
        ///
        ///   \return OK if set, otherwise FAILURE.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static int SetBit(UShort& pv, const unsigned int bit, const bool val = true)
        {
            if (bit < USHORT_BITS)
            {
                 if (val)
                 {
                     pv |= (UShort)(0x01 << bit);
                 }
                 else
                 {
                     pv &= (UShort)(~(0x01 << bit));
                 }
                 return OK;
            }
            return FAILURE;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Sets the value of a bit at a specific position to 0 or 1.
        ///
        ///   \param[out] pv The bit vector variable to modify.
        ///   \param[in] bit The bit number to edit [0, 31]. Where 0 is the least 
        ///              significant bit.
        ///   \param[in] val The value to set the bit to (true = 1, false = 0).
        ///
        ///   \return OK if set, otherwise FAILURE.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static int SetBit(UInt& pv, const unsigned int bit, const bool val = true)
        {
            if (bit < INT_BITS)
            {
                 if (val)
                 {
                     pv |= (UInt)(0x01 << bit);
                 }
                 else
                 {
                     pv &= (UInt)(~(0x01 << bit));
                 }
                 return OK;
            }
            return FAILURE;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Sets the value of a bit at a specific position to 0 or 1.
        ///
        ///   \param[out] pv The bit vector variable to modify.
        ///   \param[in] bit The bit number to edit [0, 63]. Where 0 is the least 
        ///              significant bit.
        ///   \param[in] val The value to set the bit to (true = 1, false = 0).
        ///
        ///   \return OK if set, otherwise FAILURE.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static int SetBit(ULong& pv, const unsigned int bit, const bool val = true)
        {
            if (bit < LONG_BITS)
            {
                 if (val)
                 {
                     pv |= (ULong)(0x01) << bit;
                 }
                 else
                 {
                     pv &= (ULong)(~((ULong)(0x01) << bit));
                 }
                 return OK;
            }
            return FAILURE;
        }

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Sets the value of a bit at a specific position to 0.
        ///
        ///   \param[out] pv The bit vector variable to modify.
        ///   \param[in] bit The bit number to edit [0, 7]. Where 0 is the least 
        ///              significant bit.
        ///
        ///   \return OK if set, otherwise FAILURE.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static int ClearBit(Byte& pv, const unsigned int bit)
        {
            if (bit < BYTE_BITS)
            {
                 pv &= (Byte)(~(0x01 << bit));

                 return OK;
            }
            return FAILURE;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Sets the value of a bit at a specific position to 0.
        ///
        ///   \param[out] pv The bit vector variable to modify.
        ///   \param[in] bit The bit number to edit [0, 15]. Where 0 is the least 
        ///              significant bit.
        ///
        ///   \return OK if set, otherwise FAILURE.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static int ClearBit(UShort& pv, const unsigned int bit)
        {
            if (bit < USHORT_BITS)
            {
                 pv &= (UShort)(~(0x01 << bit));

                 return OK;
            }
            return FAILURE;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Sets the value of a bit at a specific position to 0.
        ///
        ///   \param[out] pv The bit vector variable to modify.
        ///   \param[in] bit The bit number to edit [0, 31]. Where 0 is the least 
        ///              significant bit.
        ///
        ///   \return OK if set, otherwise FAILURE.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static int ClearBit(UInt& pv, const unsigned int bit)
        {
            if (bit < INT_BITS)
            {
                 pv &= (UInt)(~(0x01 << bit));
                 
                 return OK;
            }
            return FAILURE;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Sets the value of a bit at a specific position to 0.
        ///
        ///   \param[out] pv The bit vector variable to modify.
        ///   \param[in] bit The bit number to edit [0, 63]. Where 0 is the least 
        ///              significant bit.
        ///
        ///   \return OK if set, otherwise FAILURE.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static int ClearBit(ULong& pv, const unsigned int bit)
        {
            if (bit < LONG_BITS)
            {
                 pv &= (ULong)(~(0x01 << bit));

                 return OK;
            }
            return FAILURE;
        }

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Gets the value of a bit (checks if value is 1).
        ///
        ///   \param[out] pv The bit vector to check if a bit is set for.
        ///   \param[in] bit The bit number to edit [0, 7]. Where 0 is the least 
        ///
        ///   \return True if the bit is 1, otherwise false.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static bool IsBitSet(const Byte pv, const unsigned int bit)
        {
            return (pv & (0x01 << bit)) > 0 ? true : false;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Gets the value of a bit (checks if value is 1).
        ///
        ///   \param[out] pv The bit vector to check if a bit is set for.
        ///   \param[in] bit The bit number to edit [0, 15]. Where 0 is the least 
        ///
        ///   \return True if the bit is 1, otherwise false.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static bool IsBitSet(const UShort pv, const unsigned int bit)
        {
            return (pv & (0x01 << bit)) > 0 ? true : false;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Gets the value of a bit (checks if value is 1).
        ///
        ///   \param[out] pv The bit vector to check if a bit is set for.
        ///   \param[in] bit The bit number to edit [0, 31]. Where 0 is the least 
        ///
        ///   \return True if the bit is 1, otherwise false.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static bool IsBitSet(const UInt pv, const unsigned int bit)
        {
            return (pv & (UInt)(0x01 << bit)) > 0 ? true : false;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Gets the value of a bit (checks if value is 1).
        ///
        ///   \param[out] pv The bit vector to check if a bit is set for.
        ///   \param[in] bit The bit number to edit [0, 64]. Where 0 is the least 
        ///
        ///   \return True if the bit is 1, otherwise false.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static bool IsBitSet(const ULong pv, const unsigned int bit)
        {
            return (pv & ((ULong)(0x01) << bit)) > 0 ? true : false;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Gets the value of a bit (checks if value is 1).
        ///
        ///   \param[out] pv The bit vector to check if a bit is set for.
        ///   \param[in] bit The bit number to edit [0, 7]. Where 0 is the least 
        ///
        ///   \return True if the bit is 1, otherwise false.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static bool GetBit(const Byte pv, const unsigned int bit)
        {
            return (pv & (Byte)(0x01 << bit)) > 0 ? true : false;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Gets the value of a bit (checks if value is 1).
        ///
        ///   \param[out] pv The bit vector to check if a bit is set for.
        ///   \param[in] bit The bit number to edit [0, 15]. Where 0 is the least 
        ///
        ///   \return True if the bit is 1, otherwise false.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static bool GetBit(const UShort pv, const unsigned int bit)
        {
            return (pv & (0x01 << bit)) > 0 ? true : false;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Gets the value of a bit (checks if value is 1).
        ///
        ///   \param[out] pv The bit vector to check if a bit is set for.
        ///   \param[in] bit The bit number to edit [0, 31]. Where 0 is the least 
        ///
        ///   \return True if the bit is 1, otherwise false.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static bool GetBit(const UInt pv, const unsigned int bit)
        {
            return (pv & (0x01 << bit)) > 0 ? true : false;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Gets the value of a bit (checks if value is 1).
        ///
        ///   \param[in] pv The bit vector to check if a bit is set for.
        ///   \param[in] bit The bit number to edit [0, 64]. Where 0 is the least 
        ///
        ///   \return True if the bit is 1, otherwise false.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static bool GetBit(const ULong pv, const unsigned int bit)
        {
            return (pv & ((ULong)(0x01) << bit)) > 0 ? true : false;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Use this method to see if the bit vector (pv2) is a subset of
        ///          the first presence vector (pv1).
        ///
        ///   Use this method to check if a specific set of bits is included in a 
        ///   larger bit vector.
        ///
        ///   \param[in] pv1 The bit vector to check for subset in.
        ///   \param[in] pv2 The bit vector subset that you want to check for in pv1.
        ///
        ///   \return True if pv2 is a subset (supported by) pv1.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static bool Supports(const Byte pv1, const Byte pv2)
        {
            return ((pv1 & pv2) == pv2) ? true : false;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Use this method to see if the bit vector (pv2) is a subset of
        ///          the first presence vector (pv1).
        ///
        ///   Use this method to check if a specific set of bits is included in a 
        ///   larger bit vector.
        ///
        ///   \param[in] pv1 The bit vector to check for subset in.
        ///   \param[in] pv2 The bit vector subset that you want to check for in pv1.
        ///
        ///   \return True if pv2 is a subset (supported by) pv1.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static bool Supports(const UShort pv1, const UShort pv2)
        {
            return ((pv1 & pv2) == pv2) ? true : false;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Use this method to see if the bit vector (pv2) is a subset of
        ///          the first presence vector (pv1).
        ///
        ///   Use this method to check if a specific set of bits is included in a 
        ///   larger bit vector.
        ///
        ///   \param[in] pv1 The bit vector to check for subset in.
        ///   \param[in] pv2 The bit vector subset that you want to check for in pv1.
        ///
        ///   \return True if pv2 is a subset (supported by) pv1.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static bool Supports(const UInt pv1, const UInt pv2)
        {
            return ((pv1 & pv2) == pv2) ? true : false;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Use this method to see if the bit vector (pv2) is a subset of
        ///          the first presence vector (pv1).
        ///
        ///   Use this method to check if a specific set of bits is included in a 
        ///   larger bit vector.
        ///
        ///   \param[in] pv1 The bit vector to check for subset in.
        ///   \param[in] pv2 The bit vector subset that you want to check for in pv1.
        ///
        ///   \return True if pv2 is a subset (supported by) pv1.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static bool Supports(const ULong pv1, const ULong pv2)
        {
            return ((pv1 & pv2) == pv2) ? true : false;
        }

        inline static void Clear(Byte& pv) { pv = 0; }
        inline static void Clear(UShort& pv) { pv = 0; }
        inline static void Clear(UInt& pv) { pv = 0; } 
        inline static void Clear(ULong& pv) { pv = 0; }
    };

} //  End of JAUS namespace

#endif

#endif
/*  End of File */
