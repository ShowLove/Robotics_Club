////////////////////////////////////////////////////////////////////////////////////
///
/// \file astar.h
/// \brief Class extends Cartographer::Navigator, generates paths using a
///         potential fields algorithm
///
/// Author(s): Robin Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: robin.adams@knights.ucf.edu<br>
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
#ifndef __ZEBULON_GROUND_ASTAR_H__
#define __ZEBULON_GROUND_ASTAR_H__

#include <cartographer/planning/navigator.h>

namespace Zebulon
{
    namespace IGVC
    {
        class AStar : public Cartographer::Navigator
        {
        public:
            enum ParameterLabels
            {
                ObstacleWeight = 0,
                LineWeight,
                SinkWeight
            };

            typedef struct {
                void* parent; // Points to the parent node, needs a cast
                Cartographer::Point3D coordinate;
                char status;
                double f;
            } node_t;

            AStar();
            AStar(Cartographer::Navigator::Settings* settings);
            ~AStar(){}

            virtual int GetPath(const Cartographer::Pose& start,
                                const Cartographer::Pose& end,
                                Cartographer::Path& pathFound,
                                Cartographer::Path::List * allPaths = NULL,
                                const std::string& mapName = "Default");

            virtual double CalculateFitness(const Cartographer::Path& path, const Cartographer::Navigator* nav) { return 0; }

        protected:

            double CalcCost (Cartographer::Point3D point1, Cartographer::Point3D point2);
            int CloseNode (node_t* myNode, Cartographer::Point3D finish, node_t** nodeList[], int* nodesUsed, Cartographer::Map* map);
            int ClosestNode(node_t* nodeList[], int nodesUsed);
            int CheckExist(double x, double y, node_t* nodeList[], int nodesUsed);
            bool CheckBlocked (node_t* currNode, double addx, double addy, Cartographer::Map* map);
            void ReturnList(node_t* nodeList[], Cartographer::Path* solution, Cartographer::Map* map);

            double mClearance; ///< Will be slower to return the path with a greater clearance
            double checkRadius;
        };
    }
}

#endif // __ZEBULON_GROUND_ASTAR_H__
