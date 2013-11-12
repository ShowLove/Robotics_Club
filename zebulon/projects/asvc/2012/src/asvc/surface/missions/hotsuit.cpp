//////////////////////////////////////////////////////////////////////////////////////
///
/// \file hotsuit.cpp
/// \brief Mission identifies and and reports a 'hot' target (a card suit in this case)
///
/// Author(s): Andrew Watson<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:dbarber@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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
//////////////////////////////////////////////////////////////////////////////////////
#include "asvc/surface/missions/hotsuit.h"

using namespace Zebulon;
using namespace ASVC;

const std::string Hotsuit::Name = "Hotsuit";
const std::string Hotsuit::SPADE = "SPADE";
const std::string Hotsuit::HEART = "HEART";
const std::string Hotsuit::DIAMOND = "DIAMOND";
const std::string Hotsuit::CLUB = "CLUB";

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
Hotsuit::Hotsuit()
{
    ID = Name;
    mNextMission = "";
    mCurrentState = ApproachTarget;
    mPortLeftCourse = "5555";
    mPortRightCourse = "5556";
    mReportPort = 5555;
    mReportIP = "10.11.12.208";
    mDesiredHeadingDeg = 0.0;
    mTcp;

}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Hotsuit::~Hotsuit()
{
    ShutdownCom();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the mission.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int Hotsuit::Init()
{
    Mission::Init();
    ID = Name;
    InitializeMaps(Mission::mpGlobalInfo,
                   mXmlConf,
                   Name);
    Hotsuit::LoadSettings();
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Error Check method, does any required error checking.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int Hotsuit::ErrorCheck()
{
    int result;

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Fetches data required for execution.
///
///  Gets start button state, and checks to see if GPS data is available.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int Hotsuit::FetchProcess()
{
    int result = 0;

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief If there is an early exit condition, this method will handle it.
///
///  \return Mission::KeepRunning.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Hotsuit::ExitEarly()
{
    return Mission::KeepRunning;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Executes the mission data.
///
///  \return Mission::KeepRunning.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Hotsuit::Execute()
{
    switch(mCurrentState)
    {
        case ApproachTarget:
            std::stringstream stream;
            if(GetZoneNumber() == 0) //Coarse B (Left Course)
            {
                stream << mPortLeftCourse;
                stream >> mReportPort;
            }
            else
            {
                stream << mPortRightCourse;
                stream >> mReportPort;
            }

            /* Do approach here*/

            InitializeCom(mReportPort, mReportIP);
            if (SendPing())
            {
                std::cout << "PONG Course A" << std::endl;
            }
            break;

    }
    return Mission::KeepRunning;
}
/* End of File */


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Load Settings from XML
///
///
////////////////////////////////////////////////////////////////////////////////////
void Hotsuit::LoadSettings()
{
    std::string name = Name;
    std::string baseName = std::string("Surface.") + name + ".";
    mXmlConf.GetVar(baseName + "NextMission@name", mNextMission);
    mXmlConf.GetVar(baseName + "IPAddress@IP", mReportIP);
    mXmlConf.GetVar(baseName + "CoursePort@LeftCoursePort", mPortLeftCourse);
    mXmlConf.GetVar(baseName + "CoursePort@RightCoursePort", mPortRightCourse);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes communication.
///
///  \param[in] port Port to connect on.
///  \param[in] serverIP Server IP address.
///
////////////////////////////////////////////////////////////////////////////////////
int Hotsuit::InitializeCom(const unsigned short mPort,
               const CxUtils::IP4Address& mReportIP)
{
    ShutdownCom();
    return mTcp.InitializeSocket(mReportIP, mPort);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Shutdown communication.
///
////////////////////////////////////////////////////////////////////////////////////
void Hotsuit::ShutdownCom()
{
    this->mTcp.Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends PING message.
///
///  \return True of PONG received = success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Hotsuit::SendPing() const
{
    std::string message = "PING\n";
    CxUtils::Packet response;
    if(mTcp.Send(message.c_str(), (unsigned int)message.size()))
    {
        response.Reserve(512);
        if(mTcp.Recv(response, 4, 100))
        {
            if(strstr((const char *)response.Ptr(), "PONG") != NULL)
            {
                return true;
            }
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends the location of what we believe the "hot" target is.
///
///  \param[in] typeName Type of targe (see name labels in class header).
///  \param[in] position GPS position of target.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Hotsuit::SendLocation(const std::string& typeName,
                            CxUtils::Wgs& position) const
{
    std::stringstream str;
    position = mpGlobalInfo->GetPosition(true);
    str << "REPORT;UCF:" << typeName
        << "@" << position.mLatitude << "," << position.mLongitude << "\n";
    CxUtils::Packet response;
    if(mTcp.Send(str.str().c_str(), (unsigned int)str.str().size()))
    {
        response.Reserve(512);
        if(mTcp.Recv(response, 256, 100))
        {
            if(strstr((const char *)response.Ptr(), "SUCCESS") != NULL)
            {
                return true;
            }
            else
            {
                std::cout << "COM Received: " << response.Ptr();
            }
        }
    }
    return false;
}
