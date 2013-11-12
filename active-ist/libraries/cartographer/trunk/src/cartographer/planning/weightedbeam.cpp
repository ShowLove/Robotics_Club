////////////////////////////////////////////////////////////////////////////////////
///
/// \file weightedbeam.cpp
/// \brief Class extends Cartographer::Navigator, generates paths using a
///         potential fields algorithm
///
///  <br>Author(s): Michael Scherer
///  <br>Created: 2012
///  <br>Copyright (c) 2012
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: mscherer@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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

#include "cartographer/planning/weightedbeam.h"

using namespace Cartographer;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
WeightedBeam::WeightedBeam()
{
    mLaneHeadingSet = false;

    mpSettings = new Cartographer::Navigator::Settings;
    mpSettings->Set(WeightedBeam::PathWidthMeters, 0.5);
    mpSettings->Set(WeightedBeam::AngleIncrementDegrees, 10);
    mpSettings->Set(WeightedBeam::SweepAngleStartDegrees, -90);
    mpSettings->Set(WeightedBeam::SweepAngleEndDegrees, 90);

    mpSettings->Set(WeightedBeam::SegmentLengthMeters, 10);

    mpSettings->Set(WeightedBeam::HistoryDepth, 50);
    mpSettings->Set(WeightedBeam::SearchDepth, 3);

    // Weights
    mpSettings->Set(WeightedBeam::StraightnessWeight, 1000.);
    mpSettings->Set(WeightedBeam::DistanceWeight, 1000.);
    mpSettings->Set(WeightedBeam::SimplicityWeight, 500.);
    mpSettings->Set(WeightedBeam::AngleWeight, 5.);
    mpSettings->Set(WeightedBeam::HeadingWeight, 0.);
    mpSettings->Set(WeightedBeam::LinePotentialWeight, 0.);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///	  \param[in] settings the settings to be used
///
////////////////////////////////////////////////////////////////////////////////////
WeightedBeam::WeightedBeam(Cartographer::Navigator::Settings* settings)
{
    mpSettings = settings;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Function to overload when creating a path finding algorithm.
///   
///   \param[in] start Starting pose in the map to plan from.
///   \param[in] end The desired end position (and potentially pose) to find
///                  a path to.
///   \param[out] path The path solution found which attempts to go from
///                    start to end.  Solution is in map coordinates.
///   \param[out] allPaths Optional parameter.  If used, then method
///                        returns solutions potential solution paths.
///   \param[in] mapName Optional name defaults to "Default".  This is the
///                      map to perform search on.
///
///   \return -1 if no map exists, 0 if no path exists, 1 if path found.
///
////////////////////////////////////////////////////////////////////////////////////
int WeightedBeam::GetPath(const Pose& start,
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

    mWidth = mpSettings->Get(WeightedBeam::PathWidthMeters);
    mLength = mpSettings->Get(WeightedBeam::SegmentLengthMeters);

    mStartAngle = mpSettings->Get(WeightedBeam::SweepAngleStartDegrees);
    mEndAngle = mpSettings->Get(WeightedBeam::SweepAngleEndDegrees);
    mIncrement = mpSettings->Get(WeightedBeam::AngleIncrementDegrees);

    mStraightnessWeight = mpSettings->Get(WeightedBeam::StraightnessWeight);
    mDistanceWeight = mpSettings->Get(WeightedBeam::DistanceWeight);
    mSimplicityWeight = mpSettings->Get(WeightedBeam::SimplicityWeight);
    mAngleWeight = mpSettings->Get(WeightedBeam::AngleWeight);
    mHeadingWeight = mpSettings->Get(WeightedBeam::HeadingWeight);
    mLinePotentialWeight = mpSettings->Get(WeightedBeam::LinePotentialWeight);
    /*
    double width = mpSettings->Get(WeightedBeam::PathWidthMeters);
    double length = mpSettings->Get(WeightedBeam::SegmentLengthMeters);

    double startAngle = mpSettings->Get(WeightedBeam::SweepAngleStartDegrees);
    double endAngle = mpSettings->Get(WeightedBeam::SweepAngleEndDegrees);
    double increment = mpSettings->Get(WeightedBeam::AngleIncrementDegrees);

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
    mSearchDepth = (int)mpSettings->Get(WeightedBeam::SearchDepth);
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

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Scores a path based on some criteria, including distance to final dest.
///
///   \param[in] path Path to score.
///   \param[in] navInfo Pointer to the navigator interface the fitness function
///              might need.
///
///   \return A relative score, higher is better, 1 is ideal.
///
////////////////////////////////////////////////////////////////////////////////////
double WeightedBeam::CalculateFitness(const Path& path,
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
        int historylen = (int)mpSettings->Get(WeightedBeam::HistoryDepth);

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

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all path history
///
////////////////////////////////////////////////////////////////////////////////////
void WeightedBeam::ResetHistory()
{
    mMutex.Lock();
    mDriveHistory.Clear();
    mMutex.Unlock();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Recusively creates a number of paths to a certain depth.
///
///   \param[in] path Path to start.
///   \param[out] pathList List of valid paths. Paths will be appended.
///   \param[in] depth Depth of paths to search.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool WeightedBeam::CreatePaths(const Path& seed,
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
