#include "igvc/ground/path/laneline.h"

using namespace Zebulon;
using namespace IGVC;

using namespace Cartographer;

const std::string LaneLine::Name = "LaneLine";
const double gLaneLineWidth = .2;

LaneLine::LaneLine(const unsigned int sourceID) : Objects::Line(sourceID)
{
    mObjectIdentification = LaneLine::Name;
    this->mLine.mWidth = gLaneLineWidth;
}

LaneLine::LaneLine(const Cartographer::Segment3D& line, const unsigned int sourceID)
    : Objects::Line(sourceID)
{
    mObjectIdentification = LaneLine::Name;
    Objects::Line::mLine = line;
    this->mLine.mWidth = gLaneLineWidth;
}

LaneLine::LaneLine(const LaneLine& line) : Objects::Line(line.GetSourceID())
{
    mObjectIdentification = LaneLine::Name;
    this->mLine.mWidth = gLaneLineWidth;
    *this = line;
}

bool LaneLine::IsSameAs(const Object* object,
                      const double allowedError) const
{
    const LaneLine* other = dynamic_cast<const LaneLine*>(object);
    if (other != NULL)
    {
        if (/*fabs(CxUtils::Orientation::AngleDiff(other->mLine.GetAngleXY(), this->mLine.GetAngleXY())) < allowedError
            && */CxUtils::Point3D::Distance(other->mLine.GetMidpoint(), this->mLine.GetMidpoint()) < allowedError)
        {
            return true;
        }
    }

    return false;
}

LaneLine& LaneLine::operator=(const LaneLine& line)
{
    if(this != &line)
    {
        *( (Objects::Line *)this) = *( (Objects::Line*)&line);
        mObjectIdentification = LaneLine::Name;
        // Add other stuff here.
    }
    return *this;
}

/*  End of File */

