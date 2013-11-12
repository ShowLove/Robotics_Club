/////////////////////////////////////////////////////////////////////////////
///
/// \file gpgga.h
/// \brief GPGGA NMEA message structure.
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
#ifndef _ZEBULON_NMEA_GPGGA__H
#define _ZEBULON_NMEA_GPGGA__H

#include "nmea/message.h"

namespace Zebulon
{
    namespace GPS
    {
        namespace NMEA
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class GPGGA
            ///   \brief Structure for storing, reading/writing GPGGA NMEA messages.
            ///          GPGGA - Global Positioning System Fixed Data.
            ///
            ///   $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
            ///   
            ///   Where:<br>
            ///     GPGGA        Global Positioning System Fix Data<br>
            ///     123519       Fix taken at 12:35:19 UTC<br>
            ///     4807.038,N   Latitude 48 deg 07.038' N<br>
            ///     01131.000,E  Longitude 11 deg 31.000' E<br>
            ///     1            Fix quality: 0 = invalid<br>
            ///                               1 = GPS fix (SPS)<br>
            ///                               2 = DGPS fix<br>
            ///                               3 = PPS fix<br>
            ///                                  4 = Real Time Kinematic<br>
            ///                                  5 = Float RTK<br>
            ///                               6 = estimated (dead reckoning) (2.3 feature)<br>
            ///                                  7 = Manual input mode<br>
            ///                                  8 = Simulation mode<br>
            ///     08           Number of satellites being tracked<br>
            ///     0.9          Horizontal dilution of position<br>
            ///     545.4,M      Altitude, Meters, above mean sea level<br>
            ///     46.9,M       Height of geoid (mean sea level) above WGS84<br>
            ///                      ellipsoid<br>
            ///     (empty field) time in seconds since last DGPS update<br>
            ///     (empty field) DGPS station ID number<br>
            ///     *47          the checksum data, always begins with *  <br>
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_NMEA_DLL GPGGA : public Message
            {
            public:
                GPGGA();
                GPGGA(const GPGGA &arg);
                ~GPGGA();
                virtual int GetPosition(CxUtils::Wgs& pos) const;
                virtual int GetUTC(double& utc) const;
                virtual unsigned int Write(CxUtils::Packet& msg) const;
                virtual unsigned int Read(const CxUtils::Packet& msg);
                virtual Message *Clone() const { return new GPGGA(*this); }
                static int RunTestCase();
                GPGGA &operator=(const GPGGA &arg);
                int mQuality;                       ///<  GPS quality indicator.    
                int mNumSatellites;                 ///<  Number of satellites (0-12) .   
                int mBaseId;                        ///<  Base station Id (0000-1023).    
                char mAltUnits;                     ///<  Unit for altitude.
                char mGeoidalUnits;                 ///<  Unit of geoidal separation.    
                char mIndicatorNS;                  ///<  N/S Indicator.   
                char mIndicatorEW;                  ///<  E/W Indicator. 
                double mLatitude;                   ///<  Latitude (dddmm.mmmmm).    
                double mLongitude;                  ///<  Longitude (dddmm.mmmmm).    
                double mUTC;                        ///<  Time of fixed position (hhmmss.ss).    
                double mAltitude;                   ///<  Antenna height above MSL (mean sea level).    
                double mAgeDGPS;                    ///<  Age of differential GPS data record.    
                double mGeoidalSeparation;          ///<  Geoidal separation.    
                double mHDOP;                       ///<  Horizontal DOP.  
            };
        }
    }
}

#endif
/*  End of File */
