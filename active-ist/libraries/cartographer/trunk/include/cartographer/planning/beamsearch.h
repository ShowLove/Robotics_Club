////////////////////////////////////////////////////////////////////////////////////
///
///  \file beamsearch.h
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
#ifndef LIB__CARTOGRAPHER_BEAMSEARCH__H
#define LIB__CARTOGRAPHER_BEAMSEARCH__H

#include "cartographer/planning/navigator.h"
namespace Cartographer
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class BeamSearch
    ///   \brief Path finding algorithm that uses depth first search to create potential
    ///          paths, then a A* type scoring algorithm to score each path.  
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CARTOGRAPHER_DLL BeamSearch : public Navigator
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Parameters
        ///   \brief Names representing various member variables which are configurable by
        ///          the user.  Used by calling Set() and Get() rather than a seperate set
        ///          and get for each member variable.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        enum ParameterLabels
        {
            // AngleScannedRads and AngleScannedDegrees are now ScanAngleRads and ScanAngleDegrees
            // ScansPerSweep -> AngleBetweenBranchesRads or AngleBetweenBranchesDegrees
            // SegmentLengthMeters -> BranchLengthMeters

            ScanAngleRads = 0,          ///< Defines the scan search area (e.g. 180 degrees = -PI to PI search).
            ScanAngleDegrees,           ///< Defines the scan search area (e.g. 180 degrees = -PI to PI search).
            AngleBetweenBranchesRads,   ///< Defines the angle between branches in the scanned area (radians).
            AngleBetweenBranchesDegrees,///< Defines the angle between branches in the scanned area (degrees).
            BranchLengthMeters,         ///< Maximum branch length in the search tree (meters)
            SearchDepth,                ///< Maximum depth to search
            BranchesPerDepth,           ///< Maximum number of branch points at each depth level to explore.
            PathWidthMeters,            ///< Path width (meters).
            EndpointThresholdMeters,    ///< How close to the destination endpoint must we be.

            NumOfParameters             ///< Total number of parameters
        };

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Settings
        ///   \brief Settings encapsulates all configuration variables for a particular
        ///          navigator.
        ///
        ///          See Navigator::Settings.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CARTOGRAPHER_DLL Settings : public Navigator::Settings
        {
        public:
            Settings() {}
            Settings(unsigned int numOfParameters, std::string configName = "") 
                : Navigator::Settings(numOfParameters, configName) { }

            virtual bool Set(unsigned int parameterID, double parameterValue, bool positiveValuesOnlyFlag = false);
        };
        BeamSearch();
        BeamSearch(BeamSearch::Settings* settings) { mpSettings = settings; }
        ~BeamSearch();
        virtual int GetPath(const Pose& start, 
                            const Pose& end, 
                            Path& pathFound, 
                            Path::List * allPaths = NULL, 
                            const std::string& mapName = "Default");
        virtual double CalculateFitness(const Path& path, const Navigator* navinfo = NULL);
        std::string ToString() const;
    protected:
        bool CreatePaths(const Path& seed, 
                         Path::List& pathList,
                         const int depth,
                         const std::string& mapName);

        Pose mDestination;                  ///< Desired destination waypoint.
    };
}

#endif //LIB__CARTOGRAPHER_BEAMSEARCH__H

/*End of File*/
