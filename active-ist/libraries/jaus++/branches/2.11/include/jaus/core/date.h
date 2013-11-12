////////////////////////////////////////////////////////////////////////////////////
///
///  \file date.h
///  \brief Contains the Date class which is used to store date information in
///  the JAUS library.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 29 December
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
#ifndef __JAUS_CORE_DATE__H
#define __JAUS_CORE_DATE__H

#include <cxutils/time.h>
#include "jaus/core/types.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Simple Date structure.  Contains methods to read/write date data
    ///          stored in JAUS messages.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Date
    {
    public:
        Date() : mYear(0), mMonth(0), mDay(0) {}
        Date(const Date& d) { *this = d; }
        ~Date() {}
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Creates a UShort of the date information in the format defined by the
        ///          JAUS Spec.
        ///
        ///   Bits 0-4: Day, range 1, 31<br>
        ///   Bits 5-8: Month, range 1, 12<br>
        ///   Bits 9-15: Year, range 2000, 2127 where 0 is 2000, 1 is 2001...<br>
        ///
        ///   \return The Date data in JAUS format.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        UShort ToUShort() const
        {
            UShort dstamp = 0;
            
            dstamp |= (mYear - 2000) << 9;
            dstamp |= mMonth << 5;
            dstamp |= mDay;

            return dstamp;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Sets the date using a UShort in JAUS Date format.
        ///
        ///   \param dstamp Bits 0-4: Day, range 1, 31
        ///                 Bits 5-8: Month, range 1, 12
        ///                 Bits 9-15: Year, range 2000, 2127 where 0 is 2000, 1 is 2001
        ///                 etc.
        ///
        ///   \return Current instance of the Date data.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline Date& FromUShort(const UShort dstamp)
        {
            mDay = (dstamp & 0x001F);
            mMonth = (dstamp & 0x01E0) >> 5;
            mYear = ((dstamp & 0xFE00) >> 9) + 2000;
            return *this;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Sets equal to.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline Date& operator=(const Date& date)
        {
            mYear = date.mYear;
            mMonth = date.mMonth;
            mDay = date.mDay;
            return *this;
        }
        UShort mYear;   ///<  Year (UTC) [2000, 2127].
        UShort mMonth;  ///<  Month data (UTC) [1,12].
        UShort mDay;    ///<  Day (UTC) [1,31].
    };
}

#endif
/*  End of File */
