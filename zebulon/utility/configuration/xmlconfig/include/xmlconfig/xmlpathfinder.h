//////////////////////////////////////////////////////////////////////////////////////
///
/// \file xmlpathfinder.h
/// \brief This file contains a TinyXML Node Visitor class to support
///        XmlConfigurator.
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
#ifndef _ZEUBLON_XMLPATHFINDER__H
#define _ZEUBLON_XMLPATHFINDER__H

#include <tinyxml/tinyxml.h>
#include <map>

namespace Zebulon
{
    namespace Configuration
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class XmlPathFinder
        ///   \brief TiXmlVisitor implementation to help find data within XML.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_XMLCONFIG_DLL XmlPathFinder : public TiXmlVisitor
        {
            public:
                XmlPathFinder()
                {
                    mXmlAttribute = "xml";
                }
                ~XmlPathFinder() {}
                std::map<std::string, std::string> GetXmlMap() const
                {
                    return mXmlMap;
                }
                virtual bool VisitEnter ( const TiXmlElement &element, const TiXmlAttribute *firstAttribute )
                {
                    mParentElements.push_back ( element.Value() );
                    if ( firstAttribute && mXmlAttribute.compare ( firstAttribute->Name() ) == 0 )
                    {
                        mXmlMap[GetElemPath ( mParentElements ) ] = firstAttribute->Value();
                    }
                    return true;
                }
                virtual bool VisitExit ( const TiXmlElement &element )
                {
                    mParentElements.pop_back();
                    return true;
                }
            private:
                std::string GetElemPath (const std::vector<std::string>& elements )
                {
                    std::vector<std::string>::const_iterator it;
                    std::string outstring = "";
                    it = elements.begin();
                    it++;
                    for (; it < elements.end(); it++ )
                    {
                        outstring += ( *it );
                        outstring += ".";
                    }
                    return outstring;
                }
                std::map<std::string, std::string> mXmlMap; ///<  Map of XML Data
                std::vector<std::string> mParentElements;   ///<  Names of parent elements.
                std::string mXmlAttribute;                  ///<  XML Attribute string.
        };
    }
}

#endif 

/*  End of File */
