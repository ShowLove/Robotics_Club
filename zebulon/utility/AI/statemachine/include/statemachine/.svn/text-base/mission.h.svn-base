////////////////////////////////////////////////////////////////////////////////////
///
/// \file mission.h
/// \brief Abstract mission interface for developing missions for state machine.
///
/// Author(s): Gary Stein<br>
/// Created: 2009<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org<br>
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
////////////////////////////////////////////////////////////////////////////////////
#ifndef _ZEBULON_AI_MISSION__H
#define _ZEBULON_AI_MISSION__H

#include <cxutils/timer.h>
#include <iostream>
#include <sstream>
#include "statemachine/libstatemachinedll.h"
#include "statemachine/globalinfo.h"
#include "statemachine/globalcommand.h"
#include "xmlconfig/xmlconfigurator.h"


#define STATEDEBUG
//Add the different enum for a mission
//easy way to access by name

//Do not have to be declared in order

//Purely to clean up loading variables, Macro function
//major assumptions on structure and existance of success, fail, and node
//and assume y string is a constant

//also assumed that constants do NOT start with a "m"

#define LoadConstant(x) ((Loadable::ReadNode(x,node,#x))?success.append(#x"\n"):failure.append(#x"\n"))

namespace Zebulon
{
    namespace AI
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Mission
        ///
        ///   Base Level Mission Planner
        ///
        ///   All other missions inhereit off of this
        ///
        ///   It contains all base things that all other missions
        ///   need, if multiple sub-missions need a function
        ///   (like a timer) push up to the mission level.
        ///   Also, sub-mission should call parent function
        ///
        ///   By default, it is the default mission, doesn't do anything
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_STATEMACHINE_UTIL_DLL  Mission
        {
        public:
            const static std::string KeepRunning;   ///<  Signals the mission should keep running.
            Mission();
            virtual ~Mission();
            /** Returns Name of the given mission, each is unqiue. */
            std::string GetName() const;

            /** Defines that function that is called for that mission
                Only for 1 Time tick, as defined by DT (probably 0.1 seconds)
                No longer inherit Run */
            std::string Run();

            // New system Requires more structure
            // These will be called within Run

            /** Call Order - 1. Checks for sensor errors that exists */
            virtual int ErrorCheck() = 0;
            /** Call Order - 2. Grabs data from GlobalInfo to internal variables
                                Should do processing like camera work */
            virtual int FetchProcess() = 0;
            /** Call Order - 3. Escape Terms outside of internal state machine. */
            virtual std::string ExitEarly() = 0;
            /** Call Order - 4. The switch-case logic of the system
                                Internal Transitions
                                External Transitions
                                Set up Actions for GlobalCommand */
            virtual std::string Execute() = 0;

            /** Defines the Initialization of the state, only called when the
                state is switched to from some other state, not when control
                is taken away. */
            virtual int Init();
            /** Defines the Exiting information for the state, when the state is over
                not when control is taken over. */
            virtual int Exit();
            /** Defines Sentinal Rules that can interrupt normal state operation
                in case of emergency, etc, higher return higher priority */
            virtual double Sentinal();
            /** Sets the global command interface to use for control */
            virtual void SetGlobalCommand(GlobalCommand *globalCommand) = 0;
            /** Sets the global info interface for getting environment knowledge. */
            virtual void SetGlobalInfo(GlobalInfo *globalInfo) = 0;

            /**  Loads file name for this mission to load from master XML file. */
            static bool XmlLoad(std::string MainFilename="");
            static Configuration::XmlConfigurator mXmlConf;  ///< XML configuration

            /** Helper function to load GPS waypoints from a file.
                \param[in] filename XML file containing waypoints.
            */
            static std::vector<CxUtils::Wgs> LoadWaypointsFromFile(const std::string& filename) { return LoadWgsWaypointsFromFile(filename); }
            /** Loads GPS waypoints from an XML file in order they are listed. */
            static std::vector<CxUtils::Wgs> LoadWgsWaypointsFromFile(const std::string& filename)
            {
                std::vector<CxUtils::Wgs> waypoints;
                TiXmlDocument doc;
                if(doc.LoadFile(filename.c_str()) == false)
                {
                    std::cout << "Mission::ERROR Unable to load waypoints from " << filename << std::endl;
                    return waypoints;
                }

                TiXmlHandle handle(&doc);
                TiXmlElement* child = handle.FirstChild().ToElement();
                while(std::string("Waypoints") != std::string(child->Value()))
                {
                    child = child->FirstChildElement();
                    if(child == NULL)
                        break;
                }
                if(child == NULL ||
                   std::string("Waypoints") != std::string(child->Value()))
                {
                    std::cout << "Mission::ERROR Unable to load waypoints from " << filename << std::endl;
                    return waypoints;
                }
                //TiXmlElement* waypoint = handle.FirstChild("Waypoints").FirstChild("Waypoint").ToElement();
                TiXmlElement* waypoint = child->FirstChildElement("Waypoint");

                while(waypoint)
                {
                    if(waypoint->Attribute("latitude") && waypoint->Attribute("longitude"))
                    {
                        CxUtils::Wgs wgs;
                        wgs.mLatitude = atof(waypoint->Attribute("latitude"));
                        wgs.mLongitude = atof(waypoint->Attribute("longitude"));
                        if(waypoint->Attribute("elevation"))
                        {
                            wgs.mElevation = atof(waypoint->Attribute("elevation"));
                        }
                        waypoints.push_back(wgs);
                    }
                    if(waypoint->Attribute("latitude_deg") &&
                       waypoint->Attribute("latitude_min") &&
                       waypoint->Attribute("latitude_sec") &&
                       waypoint->Attribute("longitude_deg") &&
                       waypoint->Attribute("longitude_min") &&
                       waypoint->Attribute("longitude_sec"))
                    {
                        CxUtils::Wgs wgs;
                        wgs.mLatitude = fabs(atof(waypoint->Attribute("latitude_deg"))) +
                                        fabs(atof(waypoint->Attribute("latitude_min"))/60.0) +
                                        fabs(atof(waypoint->Attribute("latitude_sec"))/3600.0);
                        if(waypoint->Attribute("ns"))
                        {
                            if(strstr(waypoint->Attribute("ns"), "S") || strstr(waypoint->Attribute("ns"), "s"))
                            {
                                wgs.mLatitude *= -1;
                            }
                        }
                        wgs.mLongitude = fabs(atof(waypoint->Attribute("longitude_deg"))) +
                                         fabs(atof(waypoint->Attribute("longitude_min"))/60.0) +
                                         fabs(atof(waypoint->Attribute("longitude_sec"))/3600.0);
                        if(waypoint->Attribute("ew"))
                        {
                            if(strstr(waypoint->Attribute("ew"), "W") || strstr(waypoint->Attribute("ew"), "w"))
                            {
                                wgs.mLongitude *= -1;
                            }
                        }
                        else
                        {
                            // Default to Western hemisphere.
                            wgs.mLongitude *= -1;
                        }
                        if(waypoint->Attribute("elevation"))
                        {
                            wgs.mElevation = atof(waypoint->Attribute("elevation"));
                        }
                        waypoints.push_back(wgs);
                    }
                    waypoint = waypoint->NextSiblingElement("Waypoint");
                }

                return waypoints;
            }


            /** Loads GPS waypoints (WGS format in file) from an XML file in order they are listed. Returns as UTM. */
            static std::vector<CxUtils::Utm> LoadUtmWaypointsFromFile(const std::string& filename)
            {
                std::vector<CxUtils::Wgs> wgs = LoadWgsWaypointsFromFile(filename);
                std::vector<CxUtils::Wgs>::iterator p;
                std::vector<CxUtils::Utm> utm;
                for(p = wgs.begin(); p != wgs.end(); p++)
                {
                    utm.push_back(CxUtils::Utm(*p));
                }
                return utm;
            }
        protected:
            double GetMissionTime();  ///< A function that gets time from start of mission

            std::string ID;     ///< ID/Name of the Mission
            int mTicks;         ///< A timer that keeps track of number of ticks
            int mState;         ///< State of internal state machine
            double mStartMissionTime; ///< A relative time versus a point
        private:

        };
    }
}

#endif
