#include "ground/path/historybeam.h"

using namespace Zebulon;
using namespace IGVC;

using namespace Cartographer;

HistoryBeam::HistoryBeam()
{
    mLaneHeadingSet = false;

    mpSettings = new Cartographer::Navigator::Settings;
    mpSettings->Set(HistoryBeam::PathWidthMeters, 0.5);
    mpSettings->Set(HistoryBeam::AngleIncrementDegrees, 10);
    mpSettings->Set(HistoryBeam::SweepAngleStartDegrees, -90);
    mpSettings->Set(HistoryBeam::SweepAngleEndDegrees, 90);

    mpSettings->Set(HistoryBeam::SegmentLengthMeters, 10);

    mpSettings->Set(HistoryBeam::HistoryDepth, 50);
    mpSettings->Set(HistoryBeam::SearchDepth, 3);

    // Weights
    mpSettings->Set(HistoryBeam::StraightnessWeight, 1000.);
    mpSettings->Set(HistoryBeam::DistanceWeight, 1000.);
    mpSettings->Set(HistoryBeam::SimplicityWeight, 500.);
    mpSettings->Set(HistoryBeam::AngleWeight, 5.);
    mpSettings->Set(HistoryBeam::HeadingWeight, 0.);
    mpSettings->Set(HistoryBeam::LinePotentialWeight, 0.);
}

HistoryBeam::HistoryBeam(Cartographer::Navigator::Settings* settings)
{
    mpSettings = settings;
}

int HistoryBeam::GetPath(const Pose& start,
                             const Pose& end,
                             Path& pathFound,
                             Path::List * allPaths,
                             const std::string& mapName)
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);

    mDestination = end;

    mMap = static_cast<const ObjectMap*>(GetMap(mapName));
    //mpSettings->Get(PotentialFields::ObstacleWeight)

    // check end position is in bounds
    //if(!GetMap(mapName)->InBounds(end.mPosition))
    //{
    //    mDriveHistory.Push(start);
    //    return 0;
    //}

    int result=0;
    Path::List solutions;

    mWidth = mpSettings->Get(HistoryBeam::PathWidthMeters);
    mLength = mpSettings->Get(HistoryBeam::SegmentLengthMeters);

    mStartAngle = mpSettings->Get(HistoryBeam::SweepAngleStartDegrees);
    mEndAngle = mpSettings->Get(HistoryBeam::SweepAngleEndDegrees);
    mIncrement = mpSettings->Get(HistoryBeam::AngleIncrementDegrees);

    mStraightnessWeight = mpSettings->Get(HistoryBeam::StraightnessWeight);
    mDistanceWeight = mpSettings->Get(HistoryBeam::DistanceWeight);
    mSimplicityWeight = mpSettings->Get(HistoryBeam::SimplicityWeight);
    mAngleWeight = mpSettings->Get(HistoryBeam::AngleWeight);
    mHeadingWeight = mpSettings->Get(HistoryBeam::HeadingWeight);
    mLinePotentialWeight = mpSettings->Get(HistoryBeam::LinePotentialWeight);
    /*
    double width = mpSettings->Get(HistoryBeam::PathWidthMeters);
    double length = mpSettings->Get(HistoryBeam::SegmentLengthMeters);

    double startAngle = mpSettings->Get(HistoryBeam::SweepAngleStartDegrees);
    double endAngle = mpSettings->Get(HistoryBeam::SweepAngleEndDegrees);
    double increment = mpSettings->Get(HistoryBeam::AngleIncrementDegrees);

    /*
    for (double theta=startAngle; theta<endAngle; theta+=increment)
    {
        Path solution(start);
        Pose next = solution.CalculateNextPoint(start, length, CxUtils::CxToRadians(theta));
        solution.Push(next);
        solution.SetPathWidth(width);
        //CxUtils::Point3D point(start.mPosition.mX + length*cos(CxUtils::CxToRadians(theta-start.mRotation.mZ)),
        //                       start.mPosition.mY - length*sin(CxUtils::CxToRadians(theta-start.mRotation.mZ)), 0);
        CxUtils::Segment3D seg(start.mPosition, next.mPosition, width);

        if (!mMap->ScanSegmentInXY(seg))
        {
            //solution.Push(Pose(point, Orientation(0, 0, theta-start.mRotation.mZ)));
            solutions.push_back(solution);
        }
    }*/

    mStart = start;
    mSearchDepth = mpSettings->Get(HistoryBeam::SearchDepth);
    CreatePaths(Path(start), solutions, mSearchDepth);

    result = solutions.size();
    if (result < 1)
    {
        mDriveHistory.Push(start);
        return result;
    }

    if (allPaths)
    {
        (*allPaths) = solutions;
    }

    Path bestSolution;
    double bestFitness = std::numeric_limits<double>::infinity();

    Path::List::iterator iter;
    for (iter = solutions.begin(); iter != solutions.end(); iter++)
    {
        Path solution = *iter;
        solution.SetFitness(CalculateFitness(solution));
        if (solution.GetFitness() <= bestFitness)
        {
            bestSolution = solution;
            bestFitness = solution.GetFitness();
        }
    }

    mDriveHistory.Push(start);
    pathFound = bestSolution;
    double dirRad = pathFound.GetDirection();
    double dirDeg = CxUtils::CxToDegrees(dirRad);
    return result;
}

double HistoryBeam::CalculateFitness(const Path& path,
                                     const Navigator* navinfo)
{
    if (mFitnessFunction)
    {
        return mFitnessFunction->CalculateFitness(path, this);
    }

    if (path.Size() <= 1)
    {
        return std::numeric_limits<double>::infinity();
    }

    double prevRot = path.First().mRotation.mZ;
    double totRot = CxUtils::Epsilon;
    Path::const_iterator iter;
    for (iter = path.Begin(); iter != path.End(); iter++)
    {
        totRot += fabs(CxUtils::Orientation::AngleDiff(prevRot, iter->mRotation.mZ));
        prevRot = iter->mRotation.mZ;
    }
    double straightness = mStraightnessWeight*(totRot + 1.);

    //double simplicity = 1.0/(path.GetSegments().size()*path.GetSegments().size());
    double simplicity;
    if (path.GetSegments().size() > 0)
    {
        //simplicity = mSimplicityWeight*((path.GetSegments().size()-1)*mLength + (path.GetSegments().end())->GetMagnitude());
        simplicity = mSimplicityWeight*path.GetDistance();
    }
    else
    {
        return std::numeric_limits<double>::infinity();
    }
    //simplicity *= simplicity;

    double angle;
    if (mLaneHeadingSet)
    {
        angle = mAngleWeight*fabs(CxUtils::Orientation::AngleDiff(path.GetDirection(), mLaneHeading));
    }
    else
    {
        angle = 1.0;
    }

    double distance = path.Last().mPosition.Distance(mDestination.mPosition);
    //distance = (mMap->GetMapSizeMeters() - distance)/1000.0;
    //distance *= distance;
    distance = mDistanceWeight*(distance + CxUtils::CX_EPSILON);
    //distance *= distance;
    //if (distance > 100)
    //    distance = 100;

    double history = 1;
    /*if (mDriveHistory.Size() > 0)
    {
        int historylen = (int)mpSettings->Get(HistoryBeam::HistoryDepth);

        Path::const_iterator iter;
        int i;
        for (iter = path.End(), i=1; iter != path.Begin() && i <= historylen; iter--, i++)
        {
            double dist = path.Last().mPosition.Distance(iter->mPosition);
            history += (dist*dist)/i;
        }

        //history *= history;
    }
    else
    {
        history = 1;
    }*/

    //return distance * history * angle * simplicity * straightness;
    //return distance * distance * distance * simplicity * angle * straightness;
    double vHeading = mStart.mRotation.mZ;
    double pHeading = path.GetDirection();
    double vHeadingDeg = CxUtils::CxToDegrees(vHeading);
    double pHeadingDeg = CxUtils::CxToDegrees(pHeading);
    double heading = mHeadingWeight*fabs(CxUtils::Orientation::AngleDiff(mStart.mRotation.mZ, path.GetDirection()));
    //double heading = mHeadingWeight * fabs(CxUtils::Orientation::AngleDiff(mStart.mRotation.mZ, path.GetSegments().front().GetAngleXY()));

    double linePotential = 0;
    const Object::Set* objects = mMap->GetObjects();
    Object::Set::const_iterator objIter;
    for (objIter = objects->begin(); objIter != objects->end(); objIter++)
    {
        if ((*objIter)->GetSourceID() == 1)
            linePotential += 1/(*objIter)->GetOrigin().Distance(path.GetSegments().front().mPoint2);
    }
    linePotential *= mLinePotentialWeight;

    return distance + simplicity + angle + straightness + heading + linePotential;

}

void HistoryBeam::ResetHistory()
{
    mMutex.Lock();
    mDriveHistory.Clear();
    mMutex.Unlock();
}

bool HistoryBeam::CreatePaths(const Path& seed,
                              Path::List& pathList,
                              const int depth)
{
    for (double theta=mStartAngle; theta<=mEndAngle; theta+=mIncrement)
    {
        Path solution = seed;
        Pose start = solution.Last();
        Pose next = solution.CalculateNextPoint(start, mLength, CxUtils::CxToRadians(theta));
        solution.Push(next);
        solution.SetPathWidth(mWidth);
        CxUtils::Segment3D seg(start.mPosition, next.mPosition, mWidth);

        if (!mMap->ScanSegmentInXY(seg))
        {
            //solution.Push(Pose(point, Orientation(0, 0, theta-start.mRotation.mZ)));
            if (solution.Intersects(mDestination) && (solution.GetSegments().size() >= 1))
            {
                //solution.Pop();
                //solution.Push(mDestination);
                //solution.SetPathWidth(mWidth);

                //if (solution.GetNumPoints() == 2)
                //    CxUtils::SleepMs(1);

                Path solution2 = seed;
                solution2.Push(mDestination);
                solution2.SetPathWidth(mWidth);

                pathList.push_back(solution2);
            }
            else if ((depth == 1) && (solution.GetSegments().size() >= 1))
            {
                pathList.push_back(solution);
            }
            else
            {
                CreatePaths(solution, pathList, depth-1);
            }
        }
    }

    return true;
}
