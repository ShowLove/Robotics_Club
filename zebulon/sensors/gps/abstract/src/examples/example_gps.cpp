/////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_gps.cpp
///  \brief This is a simple GPS example program.
///
///  <br>Author(s): Daniel Barber<br>
///  <br>Created: 12/10/2010<br>
///  <br>Copyright (c) 2010<br>
///  <br>Robotics Club at UCF
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://robotics.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
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
#include <iostream>
#include <cxutils/keyboard.h>
#include <gps/gpsfactory.h>

using namespace Zebulon;

class MyCallback : public GPS::Callback
{
public:
    MyCallback() {}
    ~MyCallback() {}
    virtual void ProcessGPSData(double latitude, double longitude, double altitude)
    {
        std::cout << latitude << " - " << longitude << std::endl;
    }
};

int main(int argc, char* argv[])
{
    //Call the abstract GPS create.
    GPS::GenericGPS* gps = NULL;
    MyCallback callback;

    gps =  GPS::GPSFactory::CreateGPS(GPS::Factory::NMEA);

    if(gps == NULL)
    {
        std::cout << "Could not create a GPS intels rface to GPS type:" << GPS::Factory::NMEA  << "!";
        return false;
    }

    //Initialize the connection to the GPS over a serial port.
    if (gps->Initialize("/dev/asv-gps", 115200) == 1)
    {
        std::cout << "Successfully connected to GPS\n";
    }
    else
    {
        std::cout << "Failure to connect to GPS!";
        return false;
    }

    //Register the GPS callback.
    gps->RegisterCallback(&callback);

    while(CxUtils::GetChar() != 27)
    {
        CxUtils::SleepMs(1);
    }

    return 0;
}

/*  End of File */

