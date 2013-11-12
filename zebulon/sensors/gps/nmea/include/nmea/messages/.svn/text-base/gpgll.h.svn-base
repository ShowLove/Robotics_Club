/////////////////////////////////////////////////////////////////////////////
///
/// \file gpgll.h
/// \brief GPGLL NMEA message structure.
///
/// Author(s): Daniel Barber<br>
/// Created: 2/23/2008<br>
/// Last Modified: 2/23/2008<br>
/// Copyright (c) 2007<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
///
/// This library is free software; you can redistribute it and/or
/// modify it under the terms of the GNU Lesser General Public
/// License as published by the Free Software Foundation; either
/// version 2.1 of the License, or (at your option) any later version.
/// 
/// This library is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
/// Lesser General Public License for more details.
/// 
/// You should have received a copy of the GNU Lesser General Public
/// License along with this library; if not, write to the Free Software
/// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
/// MA 02110-1301 USA
///
/////////////////////////////////////////////////////////////////////////////
#ifndef _ZEBULON_NMEA_GPGLL__H
#define _ZEBULON_NMEA_GPGLL__H

#include "nmea/message.h"

namespace Zebulon
{
    namespace GPS
    {
        namespace NMEA
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class GPGLL
            ///   \brief Structure for storing, reading/writing GPGLL NMEA messages.
            ///          GPGLL - Geographic Position - Latitude/Longitude
            ///
            ///   $GPGLL,3723.2475,N,12158.3416,W,161229.487,A*2C
            ///   
            ///   Where:<br>
            ///        GPGLL          Geographic position, Latitude and Longitude<br>
            ///        3723.2475,N    Latitude ddmm.mmmm (37 deg. 23.2475 min), North<br>
            ///        12158.3416,W   Longitude dddmm.mmmm (121 deg 58.3416 min). West<br>
            ///        161229.487     hhmmss.sss Fix taken at 16:12:29.487 UTC<br>
            ///        A              A= data valid or V (not valid)<br>
            ///        *31            checksum data<br>
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_NMEA_DLL GPGLL : public Message
            {
            public:
                GPGLL();
                GPGLL(const GPGLL &arg);
                ~GPGLL();
                virtual int GetPosition(CxUtils::Wgs& pos) const;
                virtual int GetUTC(double& utc) const;
                virtual unsigned int Write(CxUtils::Packet& msg) const;
                virtual unsigned int Read(const CxUtils::Packet& msg);
                virtual Message *Clone() const { return new GPGLL(*this); }
                static int RunTestCase();
                GPGLL &operator=(const GPGLL &arg);
                char mDataActive;        ///< Data active (A or V(void)).    
                char mIndicatorNS;       ///<  N/S Indicator.    
                char mIndicatorEW;       ///<  E/W Indicator.    
                double mLatitude;        ///<  Latitude (dddmm.mmmmm).    
                double mLongitude;       ///<  Longitude (dddmm.mmmmm).    
                double mUTC;             ///<  Time of fixed position (hhmmss.ss).   
            };
        }
    }
}

#endif
/*  End of File */
