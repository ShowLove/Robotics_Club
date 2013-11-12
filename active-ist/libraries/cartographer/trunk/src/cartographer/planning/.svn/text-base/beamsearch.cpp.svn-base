////////////////////////////////////////////////////////////////////////////////////
///
///  \file beamsearch.cpp
///  \brief Beamsearch implementation of pathfinding using the navigator interface.
///
///  <br>Author(s): David Adams
///  <br>Created: 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dadams@ist.ucf.edu
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
#include "cartographer/planning/beamsearch.h"
#include "cartographer/maps/terrainmap.h"
#include <cstdio>

using namespace Cartographer;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets a parameter
///     
///   \param[in] parameterID ID of parameter
///   \param[in] parameterValue value of parameter
///   \param[in] positiveValuesOnlyFlag if set, ensures this value is positive
///
///   \return True if success, false otherwise
///
////////////////////////////////////////////////////////////////////////////////////
bool BeamSearch::Settings::Set(unsigned int parameterID, double parameterValue, bool positiveValuesOnlyFlag)
{
    if(positiveValuesOnlyFlag && (parameterValue < 0)) return false;
    if(parameterID <= NumOfParameters)
    {
        mParameters[parameterID] = parameterValue;
    
        if(parameterID == (unsigned int)BeamSearch::ScanAngleRads)
        {
            mParameters[BeamSearch::ScanAngleDegrees] = CxUtils::CxToDegrees(mParameters[BeamSearch::ScanAngleRads]);
        }
        if (parameterID == (unsigned int)BeamSearch::ScanAngleDegrees)
        {
            mParameters[BeamSearch::ScanAngleRads] = CxUtils::CxToRadians(mParameters[BeamSearch::ScanAngleDegrees]);
        }
        if(parameterID == (unsigned int)BeamSearch::AngleBetweenBranchesRads)
        {
            mParameters[BeamSearch::AngleBetweenBranchesDegrees] = CxUtils::CxToDegrees(mParameters[BeamSearch::AngleBetweenBranchesRads]);
        }
        if (parameterID == (unsigned int)BeamSearch::AngleBetweenBranchesDegrees)
        {
            mParameters[BeamSearch::AngleBetweenBranchesRads] = CxUtils::CxToRadians(mParameters[BeamSearch::AngleBetweenBranchesDegrees]);
        } 

        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
BeamSearch::BeamSearch()
{
    mpSettings = new Navigator::Settings(BeamSearch::NumOfParameters, "Default BeamSearch");
    mpSettings->Set(BeamSearch::ScanAngleRads, CxUtils::CX_PI);
    mpSettings->Set(BeamSearch::BranchesPerDepth, 3);
    mpSettings->Set(BeamSearch::EndpointThresholdMeters, 3);
    mpSettings->Set(BeamSearch::PathWidthMeters, 4);
    mpSettings->Set(BeamSearch::BranchesPerDepth, 10);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
BeamSearch::~BeamSearch()
{

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
int BeamSearch::GetPath(const Pose& start, 
                        const Pose& end, 
                        Path& pathFound, 
                        Path::List * allPaths, 
                        const std::string& mapName)
{
    if(GetMap(mapName) == NULL) 
    {
        return -1;
    }

    mDestination = end;

    pathFound.Clear();

    Path::List solutions;
    Path::List newPaths;

    // Create the first solution in the set which is the starting point.
    solutions.push_back(start);
    TerrainMap* terrainMap = dynamic_cast<TerrainMap*>(GetMap(mapName));
    double terrainElev = 0.0;
    if(terrainMap)
    {
        terrainElev = terrainMap->GetElevation(start.mPosition.mX, start.mPosition.mY);
        if(start.mPosition.mZ < terrainElev)
        {
            solutions.begin()->First().mPosition.mZ = terrainElev;
        }
    }

    solutions.begin()->SetPathWidth(mpSettings->Get(BeamSearch::PathWidthMeters));

    // Search up to maximum depth.
    for(unsigned int i = 0; i < mpSettings->Get(BeamSearch::SearchDepth); i++)
    {
        Path::PriorityQueue sortedPaths;
        for(unsigned int j = 0; j < solutions.size(); j++)
        {
            //  populates solutions with possible paths that extend from the
            //  current end point.
            CreatePaths(solutions[j], newPaths, 1, mapName);
        }
        solutions.clear();
        //  We only want to investigate the mBranchesPerDepth number of best branches
        //  every time we search one more level deep
        if(mpSettings->Get(BeamSearch::BranchesPerDepth) > 0)
        {
            Path::List::iterator p;
            for(p = newPaths.begin(); p != newPaths.end(); p++)
            {
                p->SetFitness(CalculateFitness(*p));
                sortedPaths.push(*p);
            }
            newPaths.clear();
            for(int j = 0; j < pow((float)mpSettings->Get(BeamSearch::BranchesPerDepth),(float)(i+1)) && !sortedPaths.empty(); j++)
            {
                solutions.push_back(sortedPaths.top());
                sortedPaths.pop();
            }
        }
        //  If branches chosen per depth is 0 or less, assume we want to
        //  go investigate solutions branches.
        else
        {
            Path::List::iterator p;
            for(p = newPaths.begin(); p != newPaths.end(); p++)
            {
                p->SetFitness(CalculateFitness(*p));
                solutions.push_back(*p);
            }
            newPaths.clear();
        }
    }
    //  If there are no paths, return 0.
    if(solutions.empty()) 
    {
        return 0;
    }
    
    double bestFitness = 0;
    Path::List::iterator p;
    for(p = solutions.begin(); p != solutions.end(); p++)
    {
        p->SetFitness(CalculateFitness(*p));
        if(p->GetFitness() > bestFitness)
        {
            bestFitness = p->GetFitness();
            pathFound = *p;
        }
    }
    // If size is 1,then we only have the current position.
    if(pathFound.Size() == 1)
    {
        return 0;
    }

    //  Set the output parameter to all possible paths if requested.
    if(allPaths)
    {
        (*allPaths) = solutions;
    }

    return 1;
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
double BeamSearch::CalculateFitness(const Path& path, const Navigator* navinfo)
{
    //if an external fitness function has been set, do this.
    if(mFitnessFunction)
    {
        return mFitnessFunction->CalculateFitness(path, this);
    }

    if(path.Size()<=0) return -1;
     //calculating f = g + h standard a* scoring
    //the g part of the equations uses distance to get to this point
    double pathLength = path.GetDistance();

    //the h part of the equation just calculates straight distance to goal from last point
    double distToEnd = path.Last().mPosition.Distance(mDestination.mPosition);
    //printf("dist %lf\n", 10*(distToEnd+pathLength));
    //if we are at goal just return g
    if(path.Intersects(mDestination, mpSettings->Get(BeamSearch::EndpointThresholdMeters)))
    {
        return 1./(pathLength);
    }
    //else return g+h
    return 1./((distToEnd+pathLength));
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Recusively creates a number of paths to a certain depth.
///
///   \param[in] path Path to start.
///   \param[out] pathList List of valid paths. Paths will be appended.
///   \param[in] depth Depth of paths to search.
///   \param[in] mapName Name of map to use.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool BeamSearch::CreatePaths(const Path& path, 
                             Path::List& pathList, 
                             const int depth,
                             const std::string& mapName)
{
    bool result = false;

    if(path.Size() < 1) 
    {
        return false;
    }

    Path extendedPath;
    Pose tempPose;
    Segment3D tempSegment;
    //if our depth is 0 just add the current path to list
    if(depth == 0)
    {
        pathList.push_back(path);
        return result;
    }
    if(path.Intersects(mDestination, mpSettings->Get(BeamSearch::EndpointThresholdMeters)))
    {
        pathList.push_back(path);
        return result;
    }
    double leftLimit = -(mpSettings->Get(BeamSearch::ScanAngleRads)) / 2.0;
    double rightLimit = (mpSettings->Get(BeamSearch::ScanAngleRads)) / 2.0;
    double angleIncrement = mpSettings->Get(BeamSearch::AngleBetweenBranchesRads);
    double searchAngle = 0.0;
    double sign = 1;
    int count = 0;
    int branchesFound = 0;
    while(searchAngle >= leftLimit && searchAngle <= rightLimit)
    {
        searchAngle = searchAngle + angleIncrement*count*sign;

        extendedPath = path;
        extendedPath.SetPathWidth(mpSettings->Get(BeamSearch::PathWidthMeters));
        //calculates next step in path
        //tempPose = path.Last();
        tempPose = Path::CalculateNextPoint(path.Last(), mpSettings->Get(BeamSearch::BranchLengthMeters), searchAngle);
        tempSegment.mPoint1 = path.Last().mPosition;
        tempSegment.mPoint2 = tempPose.mPosition;
        tempSegment.mWidth = mpSettings->Get(BeamSearch::PathWidthMeters);
        //make sure next step is in bounds of map, and doenst intersect any objects
        if(GetMap(mapName)->InBounds(tempPose.mPosition)
            && !GetMap(mapName)->IsCollisionInXY(tempSegment))
        {
            //branchesFound++;
            //  Add this to the path, and keep searching if we need to look deeper
            extendedPath.Push(tempPose);
            if(extendedPath.Intersects(mDestination, mpSettings->Get(BeamSearch::EndpointThresholdMeters)))
            {
                extendedPath = path;
                extendedPath.Push(Path::CalculateNextPoint(path.Last(), path.Last().mPosition.Distance(mDestination.mPosition), searchAngle));
                CreatePaths(extendedPath, pathList, 0, mapName);
            }
            else
            {
                CreatePaths(extendedPath, pathList, (depth-1), mapName);
            }
            result = true;
        }

        count++;
        sign *= -1;
    }
    /*
    //else we search 1 level deep for new extensions to path
    for(double i = leftLimit;
        i < rightLimit;
        i += angleIncrement)
    {
        extendedPath = path;
        extendedPath.SetPathWidth(mpSettings->Get(BeamSearch::PathWidthMeters));
        //calculates next step in path
        //tempPose = path.Last();
        tempPose = Path::CalculateNextPoint(path.Last(), mpSettings->Get(BeamSearch::BranchLengthMeters), i);
        tempSegment.mPoint1 = path.Last().mPosition;
        tempSegment.mPoint2 = tempPose.mPosition;
        tempSegment.mWidth = mpSettings->Get(BeamSearch::PathWidthMeters);
        //make sure next step is in bounds of map, and doenst intersect any objects
        if(GetMap(mapName)->InBounds(tempPose.mPosition)
            && !GetMap(mapName)->IsCollisionInXY(tempSegment))
        {
                //add this to the path, and keep searching if we need to look deeper
                extendedPath.Push(tempPose);
                if(extendedPath.Intersects(mDestination, mpSettings->Get(BeamSearch::EndpointThresholdMeters)))
                {
                    extendedPath = path;
                    extendedPath.Push(Path::CalculateNextPoint(path.Last(), path.Last().mPosition.Distance(mDestination.mPosition), i));
                    CreatePaths(extendedPath, pathList, 0, mapName);
                }
                else
                {
                    CreatePaths(extendedPath, pathList, (depth-1), mapName);
                }
                result = true;
        }
    }
    */
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Puts various parameters into a string.
///
///   \return Parameters in string form for printing/debugging, etc.
///
////////////////////////////////////////////////////////////////////////////////////
std::string BeamSearch::ToString() const
{
    char buff[512];
    sprintf(buff,
            "Scan Angle %.2lf Angle Between Branches %.2lf Branches Per Depth %.1lf\nSearch Depth %.1lf Segment Length %.2lf SegmentWidth %.2lf",
            CxUtils::CxToDegrees(mpSettings->Get(BeamSearch::ScanAngleRads)),
            CxUtils::CxToDegrees(mpSettings->Get(BeamSearch::AngleBetweenBranchesRads)),
            mpSettings->Get(BeamSearch::BranchesPerDepth),
            mpSettings->Get(BeamSearch::SearchDepth),
            mpSettings->Get(BeamSearch::BranchLengthMeters),
            mpSettings->Get(BeamSearch::PathWidthMeters));

    return buff;
}


/*End of File*/
