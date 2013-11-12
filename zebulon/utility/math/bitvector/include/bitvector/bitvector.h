////////////////////////////////////////////////////////////////////////////////////
///
///  \file bitvector.h
///  \brief This file contains the BitVector class which is used for manipulation
///         of presence vector data in JAUS++.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef __ZEBULON_BIT_VECTOR_H
#define __ZEBULON_BIT_VECTOR_H

namespace Zebulon
{
    namespace Utility
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class BitVector
        ///   \brief Methods for manipulation of bit vectors.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class BitVector
        {
        public:
            static const int BYTESIZE = 8;
            
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Sets the value of a bit at a specific position to 0 or 1.
            ///
            ///   \param[out] pv The bit vector variable to modify.
            ///   \param[in] bit The bit number to edit [0, 7]. Where 0 is the least 
            ///              significant bit.
            ///   \param[in] val The value to set the bit to (true = 1, false = 0).
            ///
            ///   \return true if set, otherwise false.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            inline static bool SetBit(unsigned char& pv, const unsigned int bit, const bool val = true)
            {
                if (bit < sizeof(unsigned char)*BYTESIZE)
                {
                     if (val)
                     {
                         pv |= (unsigned char)(0x01 << bit);
                     }
                     else
                     {
                         pv &= (unsigned char)(~(0x01 << bit));
                     }
                     return true;
                }
                return false;
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
            ///   \return true if set, otherwise false.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            inline static bool SetBit(unsigned short& pv, const unsigned int bit, const bool val = true)
            {
                if (bit < sizeof(unsigned short)*BYTESIZE)
                {
                     if (val)
                     {
                         pv |= (unsigned short)(0x01 << bit);
                     }
                     else
                     {
                         pv &= (unsigned short)(~(0x01 << bit));
                     }
                     return true;
                }
                return false;
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
            ///   \return true if set, otherwise false.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            inline static bool SetBit(unsigned int& pv, const unsigned int bit, const bool val = true)
            {
                if (bit < sizeof(unsigned int)*BYTESIZE)
                {
                     if (val)
                     {
                         pv |= (unsigned int)(0x01 << bit);
                     }
                     else
                     {
                         pv &= (unsigned int)(~(0x01 << bit));
                     }
                     return true;
                }
                return false;
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
            ///   \return true if set, otherwise false.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            inline static bool SetBit(unsigned long& pv, const unsigned int bit, const bool val = true)
            {
                if (bit < sizeof(unsigned long)*BYTESIZE)
                {
                     if (val)
                     {
                         pv |= (unsigned long)(0x01) << bit;
                     }
                     else
                     {
                         pv &= (unsigned long)(~((unsigned long)(0x01) << bit));
                     }
                     return true;
                }
                return false;
            }

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Sets the value of a bit at a specific position to 0.
            ///
            ///   \param[out] pv The bit vector variable to modify.
            ///   \param[in] bit The bit number to edit [0, 7]. Where 0 is the least 
            ///              significant bit.
            ///
            ///   \return true if set, otherwise false.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            inline static bool ClearBit(unsigned char& pv, const unsigned int bit)
            {
                if (bit < sizeof(unsigned char)*BYTESIZE)
                {
                     pv &= (unsigned char)(~(0x01 << bit));

                     return true;
                }
                return false;
            }
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Sets the value of a bit at a specific position to 0.
            ///
            ///   \param[out] pv The bit vector variable to modify.
            ///   \param[in] bit The bit number to edit [0, 15]. Where 0 is the least 
            ///              significant bit.
            ///
            ///   \return true if set, otherwise false.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            inline static bool ClearBit(unsigned short& pv, const unsigned int bit)
            {
                if (bit < sizeof(unsigned short)*BYTESIZE)
                {
                     pv &= (unsigned short)(~(0x01 << bit));

                     return true;
                }
                return false;
            }
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Sets the value of a bit at a specific position to 0.
            ///
            ///   \param[out] pv The bit vector variable to modify.
            ///   \param[in] bit The bit number to edit [0, 31]. Where 0 is the least 
            ///              significant bit.
            ///
            ///   \return true if set, otherwise false.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            inline static bool ClearBit(unsigned int& pv, const unsigned int bit)
            {
                if (bit < sizeof(unsigned int)*BYTESIZE)
                {
                     pv &= (unsigned int)(~(0x01 << bit));
                     
                     return true;
                }
                return false;
            }
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Sets the value of a bit at a specific position to 0.
            ///
            ///   \param[out] pv The bit vector variable to modify.
            ///   \param[in] bit The bit number to edit [0, 63]. Where 0 is the least 
            ///              significant bit.
            ///
            ///   \return true if set, otherwise false.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            inline static bool ClearBit(unsigned long& pv, const unsigned int bit)
            {
                if (bit < sizeof(unsigned long)*BYTESIZE)
                {
                     pv &= (unsigned long)(~(0x01 << bit));

                     return true;
                }
                return false;
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
            inline static bool IsBitSet(const unsigned char pv, const unsigned int bit)
            {
                return (pv & ((unsigned char)(0x01) << bit)) > 0 ? true : false;
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
            inline static bool IsBitSet(const unsigned short pv, const unsigned int bit)
            {
                return (pv & ((unsigned short)(0x01) << bit)) > 0 ? true : false;
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
            inline static bool IsBitSet(const unsigned int pv, const unsigned int bit)
            {
                return (pv & ((unsigned int)(0x01) << bit)) > 0 ? true : false;
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
            inline static bool IsBitSet(const unsigned long pv, const unsigned int bit)
            {
                return (pv & ((unsigned long)(0x01) << bit)) > 0 ? true : false;
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
            inline static bool GetBit(const unsigned char pv, const unsigned int bit)
            {
                return (pv & ((unsigned char)(0x01) << bit)) > 0 ? true : false;
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
            inline static bool GetBit(const unsigned short pv, const unsigned int bit)
            {
                return (pv & ((unsigned short)(0x01) << bit)) > 0 ? true : false;
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
            inline static bool GetBit(const unsigned int pv, const unsigned int bit)
            {
                return (pv & ((unsigned int)(0x01) << bit)) > 0 ? true : false;
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
            inline static bool GetBit(const unsigned long pv, const unsigned int bit)
            {
                return (pv & ((unsigned long)(0x01) << bit)) > 0 ? true : false;
            }

            inline static void Clear(unsigned char& pv) { pv = 0; }
            inline static void Clear(unsigned short& pv) { pv = 0; }
            inline static void Clear(unsigned int& pv) { pv = 0; } 
            inline static void Clear(unsigned long& pv) { pv = 0; }
        };
    }
}

#endif
/*  End of File */
