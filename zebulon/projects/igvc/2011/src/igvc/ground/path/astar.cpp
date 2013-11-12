////////////////////////////////////////////////////////////////////////////////////
///
/// \file astar.cpp
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
#include "ground/path/astar.h"
#include <stdio.h>
#include <iostream>
#include <cstdlib>

using namespace Zebulon;
using namespace IGVC;
using namespace Cartographer;
using namespace std;

#define point_t AStar::point_t
#define node_t AStar::node_t

// NEXT: Make sorted list of open and closed nodes

AStar::AStar()
{
    mpSettings = new Cartographer::Navigator::Settings(3);
    mpSettings->Set(AStar::ObstacleWeight, 0.01);
    mpSettings->Set(AStar::LineWeight, 0.1);
    mpSettings->Set(AStar::SinkWeight, 1.0);

    mClearance = .5;
}

AStar::AStar(Cartographer::Navigator::Settings* settings)
{
    mpSettings = settings;
}

int AStar::GetPath(const Pose& start,
                             const Pose& end,
                             Path& pathFound,
                             Path::List * allPaths,
                             const std::string& mapName)
{
    // Get map
    Map* map = GetMap(mapName);

    // check end position is in bounds
    if(!map->InBounds(end.mPosition))
        return 0;

    // Clear the pathFound
    pathFound.Clear();

    // Initialize list of nodes
    node_t** nodeList = (node_t**)malloc(sizeof(node_t*)); // dynamic array of node_t*
    int nodesUsed = 0;

    // Initializes the finishing point
    Point3D finish;
    finish.mX = end.mPosition.mX;
    finish.mY = end.mPosition.mY;

    // Initializes the beginning node
    node_t* begin = (node_t*)malloc(sizeof(node_t));
    begin->parent = NULL;
    begin->coordinate.mX = start.mPosition.mX;
    begin->coordinate.mY = start.mPosition.mY;
    begin->status = 'o';
    begin->f = CalcCost(begin->coordinate,finish);
    nodeList[nodesUsed] = begin;
    nodesUsed++;

    // Initializes the path
    Path solution(start);
    solution.SetPathWidth(mClearance);

    // If the start is the same as the end, return
    if (fabs(begin->coordinate.mX - end.mPosition.mX) <= .5 && fabs(begin->coordinate.mY - end.mPosition.mY) <= .5) {
        pathFound = solution;
        solution.Push(end);
        for (int i = 0; i < nodesUsed; i++)
            free(nodeList[i]);
        free(nodeList);
        return 1;
    }

    while(CloseNode(nodeList[ClosestNode(nodeList,
                                         nodesUsed)],
                    finish,
                    &nodeList,
                    &nodesUsed,map) == 0);

    ReturnList(nodeList, &solution, map);
    solution.Push(end);

    pathFound = solution;
    for (int i = 0; i < nodesUsed; i++)
        free(nodeList[i]);
    free(nodeList);
    return 1;

}

int AStar::CloseNode (node_t* myNode, Point3D finish, node_t** nodeList[], int* nodesUsed, Map* map)
{    
    int i, j;

    // NOTE: Stopped here. Change check exist to map and add collision circle
    // Find radius of next hit
    int distance = checkRadius;
    while (map->IsCollisionInXY(myNode->coordinate,checkRadius))
    {
        checkRadius = checkRadius/2;
        distance = checkRadius;
    }
    while (!map->IsCollisionInXY(myNode->coordinate,checkRadius))
    {
        checkRadius += checkRadius/2;
        distance = checkRadius;
    }
    while (distance > mClearance)
    {
        distance /= 2;
        if (map->IsCollisionInXY(myNode->coordinate,checkRadius))
            checkRadius -= distance;
        else if (!map->IsCollisionInXY(myNode->coordinate,checkRadius))
            checkRadius += distance;
    }

    for (i = -1; i <= 1; i++)
        for (j = -1; j <= 1; j++)
        {
            if (i == 0 && j== 0)
                continue;

            int addX;
            int addY;
            if (i == 0)
            {
                addY = j*checkRadius;
            }
            else if (j == 0)
            {
                addX = i*checkRadius;
            }
            else
            {
                addX = i*0.707*checkRadius;
                addY = j*0.707*checkRadius;
            }

            if (!(CheckExist(myNode->coordinate.mX + addX, myNode->coordinate.mY + addY, *nodeList, *nodesUsed)))
                // check if path is blocked
                if (!(CheckBlocked(myNode,addX,addY,map)))
                {
                    node_t* newNode = (node_t*)malloc(sizeof(node_t));
                    newNode->parent = myNode;
                    newNode->coordinate.mX = myNode->coordinate.mX + addX;
                    newNode->coordinate.mY = myNode->coordinate.mY + addY;
                    newNode->f = CalcCost(newNode->coordinate, finish) + CalcCost((*nodeList)[0]->coordinate,newNode->coordinate);
                    newNode->status = 'o';

                    *nodeList = (node_t**)realloc(*nodeList,((*nodesUsed)+1)*sizeof(node_t*));
                    (*nodeList)[*nodesUsed] = newNode;
                    (*nodesUsed)++;

                    if (fabs(newNode->coordinate.mX - finish.mX) <= .5 && fabs(newNode->coordinate.mY - finish.mY) <= .5)
                    {
                        newNode->status = 'f';
                        myNode->status = 'c';
                        return 1;
                    }
                }
        }

    myNode->status = 'c';
    return 0;
}

double AStar::CalcCost (Point3D point1, Point3D point2)
{
    double deltaX = fabs(point1.mX - point2.mX);
    double deltaY = fabs(point1.mY - point2.mY);
    double distance;

    if (deltaX > deltaY)
        distance = (deltaY) * 14 + (deltaX - deltaY) * 10;
    else
        distance = (deltaX) * 14 + (deltaY - deltaX) * 10;

    return distance;
}

int AStar::ClosestNode(node_t* nodeList[], int nodesUsed)
{
    int i;
    int lowest = -1;
    int value;

    for(i = 0; i < nodesUsed; i++)
    {
        if (nodeList[i]->status == 'o')
        {
            if (lowest == -1)
            {
                lowest = i;
                value = nodeList[i]->f;
            }
            else if (lowest != -1 && nodeList[i]->f < value)
            {
                lowest = i;
                value = nodeList[i]->f;
            }
        }
    }

    return lowest; // If lowest == -1, then there is no solution path
}

int AStar::CheckExist(double x, double y, node_t* nodeList[], int nodesUsed)
{
    /*
      p1.Distance(p2) <= .01
      */
    for (int i = 0; i < nodesUsed; i++)
        if (fabs(nodeList[i]->coordinate.mX - x) <= .5 && fabs(nodeList[i]->coordinate.mY - y) <= .5)
            return 1;

    return 0;
}

bool AStar::CheckBlocked (node_t* currNode, double addx, double addy, Map* map)
{
    Segment3D seg;
    seg.mPoint1.mX = currNode->coordinate.mX;
    seg.mPoint1.mY = currNode->coordinate.mY;
    seg.mPoint2.mX = currNode->coordinate.mX + addx;
    seg.mPoint2.mY = currNode->coordinate.mY + addy;
    seg.mWidth = mClearance;

    return map->IsCollisionInXY(seg);
}

void AStar::ReturnList(node_t* nodeList[], Path* solution, Map* map)
{
    int i = 0;
    node_t* current;
    node_t* end;
    int count = 0;


    while (1) {
        if (nodeList[i]->status == 'f') {
            end = nodeList[i];
            break;
        }
        i++;
    }

    current = end;

    // Sets straighter path
    /*node_t* currStraight = end;
    while (currStraight->parent != NULL)
    {
        if (!CheckBlocked(currStraight,nodeList[0]->coordinate.mX - currStraight->coordinate.mX,nodeList[0]->coordinate.mY - currStraight->coordinate.mY,map))
            currStraight->parent = nodeList[0];
        currStraight = (node_t*)currStraight->parent;
    }*/

    node_t* currNode = end;
    node_t* newParent = (node_t*)currNode->parent;
    while (newParent != NULL)
    {
        if (!CheckBlocked(currNode,newParent->coordinate.mX - currNode->coordinate.mX,newParent->coordinate.mY - currNode->coordinate.mY,map))
        {
            currNode->parent = newParent;
            newParent = (node_t*)newParent->parent;
        }
        else
        {
            currNode = (node_t*)currNode->parent;
        }
    }

    while (current->parent != NULL) {
        count++;
        current = (node_t*) current->parent;
    }

    node_t* route[count+1];

    current = end;

    for (i = count; i >= 0; i--) {
        route[i] = current;
        current = (node_t*) current->parent;
    }

    for (i = 1; i <= count; i++) {
        solution->Push(route[i]->coordinate);
    }
}
