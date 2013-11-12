#include "ground/path/potentialfields.h"

using namespace Zebulon;
using namespace IGVC;

using namespace Cartographer;

PotentialFields::PotentialFields()
{
    mpSettings = new Cartographer::Navigator::Settings(3);
    mpSettings->Set(PotentialFields::ObstacleWeight, 0.01);
    mpSettings->Set(PotentialFields::LineWeight, 0.1);
    mpSettings->Set(PotentialFields::SinkWeight, 1.0);
}

PotentialFields::PotentialFields(Cartographer::Navigator::Settings* settings)
{
    mpSettings = settings;
}

int PotentialFields::GetPath(const Pose& start,
                             const Pose& end,
                             Path& pathFound,
                             Path::List * allPaths,
                             const std::string& mapName)
{
    const ObjectMap* map = static_cast<const ObjectMap*>(GetMap(mapName));
    //mpSettings->Get(PotentialFields::ObstacleWeight)

    // check end position is in bounds
    if(!GetMap(mapName)->InBounds(end.mPosition))
        return 0;

    Path solution(start);

    const Object::Set* objects = map->GetObjects();
    Object::Set::iterator obj;
    for (obj = objects->begin(); obj != objects->end(); obj++)
    {

    }

    pathFound = solution;

    return 0;
}
