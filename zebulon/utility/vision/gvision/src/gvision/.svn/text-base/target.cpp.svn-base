#include "gvision/target.h"

using namespace Zebulon;
using namespace Vision;

Target::Target()
{
    mRed=mGreen=mBlue=127;
    mRedRange=mGreenRange=mBlueRange=32;
}

Target::~Target()
{
    
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Uses xml configurator to populate values. Requires the tag prefix
///          such as GreenBouy, will automatically find GreenBouyThresh, etc. Will
///          only load Red, Green, Blue and RedRange, GreenRange, and BlueRange vars.
///          XML File must use specific structure such as:
///
///         <Green Red="70" Green="194" Blue="141" />
///         <GreenThresh Red="10" Green="10" Blue="10" />
///
///   \param[in] xml XmlConfigurator object containing original xml file.
///   \param[in] xmlTagName XML Tag to look for, such as GreenBouy
///   \param[in] fulXMLPath Full path to tag with out . or @ at end. Such as
///              Underwater.Buoys.Default
///
///   \return True if at least one tag was found, false if not.
///
///////////////////////////////////////////////////////////////////////////////
bool Target::Setup(Configuration::XmlConfigurator& xml, const std::string& xmlTagName, const std::string& fullXMLPath)
{
    std::string fullpath = fullXMLPath + "." + xmlTagName;
    std::string threshSuffix = "Thresh";
    bool result = false;
    mXMLTagName = xmlTagName;
    if(xml.GetVar(fullpath + "@Red",mRed,true))
    {
        result = true;
    }
    if(xml.GetVar(fullpath + "@Green",mGreen,true))
    {
        result = true;
    }
    if(xml.GetVar(fullpath + "@Blue",mBlue,true))
    {
        result = true;
    }
    if(xml.GetVar(fullpath + threshSuffix + "@Red",mRedRange,true))
    {
        result = true;
    }
    if(xml.GetVar(fullpath + threshSuffix + "@Green",mGreenRange,true))
    {
        result = true;
    }
    if(xml.GetVar(fullpath + threshSuffix + "@Blue",mBlueRange,true))
    {
        result = true;
    }
    return result;
}
