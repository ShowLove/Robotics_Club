//////////////////////////////////////////////////////////////////////////////////////
///
/// \file xmlconfigurator.h
/// \brief This file contains definition for the XmlConfigurator class..
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
#ifndef __ZEBULON_XMLCONFIGURATOR__H
#define __ZEBULON_XMLCONFIGURATOR__H

#include <tinyxml/tinyxml.h>
#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

// DLL Export Compiler flag for MSVC
#ifndef ZEB_XMLCONFIG_DLL
    #ifdef WIN32
        #define ZEB_XMLCONFIG_DLL __declspec(dllimport)
    #else
        #define ZEB_XMLCONFIG_DLL
    #endif
#endif

namespace Zebulon
{
    namespace Configuration
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class XmlConfigurator
        ///   \brief Interface to cache contents of an XML file in memory for
        ///          quick look up using helper methods.
        ///
        ///   Helper methods (GetVar) allow you to lookup attributes within nested
        ///   XML nodes quickly.  Configurator also checks for file update changes
        ///   and will reload.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_XMLCONFIG_DLL XmlConfigurator
        {
        public:
            XmlConfigurator();
            ~XmlConfigurator();
            std::string GetName() const;
            std::string GetPath() const;
            bool SetPath(const std::string& xmlPath);
            bool Load(const std::string& xmlPath, const std::string& inName);
            bool GetVar(const std::string& elemPath, bool& returnBool, const bool optional = false);
            bool GetVar(const std::string& elemPath, int& returnInt, const bool optional = false);
            bool GetVar(const std::string& elemPath, unsigned int& returnUInt, const bool optional = false);
            bool GetVar(const std::string& elemPath, double& returnDouble, const bool optional = false);
            bool GetVar(const std::string& elemPath, std::string& returnString, const bool optional = false);
            TiXmlDocument* GetTinyXML() { return &mXmlDoc; }
            const TiXmlDocument* GetTinyXML() const { return &mXmlDoc; }
            static void Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = ".");
        private:
            bool GetVar(const std::string& elemPath, std::string& returnString, const bool optional, int depth);
            bool Reload();
            bool IsFileUpdated();
            std::string GetTimeString();
            bool CheckXmlPointers(const std::string& elemPath, std::string& returnString);

            std::string mXmlDocPath;                                ///<  Document path.
            std::map<std::string, std::string> mVariableCache;      ///<  Variable cache.
            std::map<std::string, XmlConfigurator> mXmlPointers;    ///<  Map of pointers to different configurators from nested files.
            time_t mTimeLastModified;                               ///<  Time of last file modification.
            bool mCacheInvalidFlag;                                 ///<  Flag for invalid cache due to file change.
            struct stat mFileInfo;                                  ///<  File information for tim stamps.
            TiXmlDocument mXmlDoc;                                  ///<  XML Document handle.
            std::string mConfiguratorName;                          ///<  Name of configurator or file loaded.
        };
    }
}

#endif

/*  End of File */
