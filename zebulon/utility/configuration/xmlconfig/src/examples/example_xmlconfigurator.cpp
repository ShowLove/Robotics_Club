//////////////////////////////////////////////////////////////////////////////////////
///
/// \file example_xmlconfigurator.h
/// \brief Simple example of how to use XmlConfigurator.
///
/// Author(s): John Reeder<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Modified by: Daniel Barber
/// Email: dbarber@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the mConfiguratorName of the Robotics Laboratory, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
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
//////////////////////////////////////////////////////////////////////////////////////
#include "xmlconfig/xmlconfigurator.h"

using namespace std;
using namespace Zebulon::Configuration;

int main(int argc, char* argv[])
{
    cout << "starting ..." << endl;
    string xmlPath = "./main.xml";
    XmlConfigurator xmlconf;
    xmlconf.Load(xmlPath, "main");

    int flaredepth;
    int machdepth;
    int machtimeout;
    double flareangle;
    double machangle;
    string flarename = "";

    xmlconf.GetVar("missions.flare.vars@flaredepth",flaredepth);
    xmlconf.GetVar("missions.flare.vars@flareangle",flareangle);
    xmlconf.GetVar("missions.flare.vars@missionname",flarename);

    xmlconf.GetVar("missions.machinegunnest.vars@machdepth",machdepth);
    xmlconf.GetVar("missions.machinegunnest.vars@machtimeout",machtimeout);
    xmlconf.GetVar("missions.machinegunnest.vars@machangle",machangle);

    cout << "Flare Angle: " << flareangle << endl;
    cout << "Flare Depth: " << flaredepth << endl;
    cout << "Mission Name: " << flarename << endl;
    cout << "Machine Gun Depth: " << machdepth << endl;
    cout << "Machine Gun Time Out: " << machtimeout << endl;
    cout << "Machine Gun Angle: " << machangle << endl;


    cout.flush();

    return 0;
}

/*  End of File */

