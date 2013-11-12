////////////////////////////////////////////////////////////////////////////////////
///
/// \file objectscan.cpp
/// \brief Builds large objects from collections of points within LIDAR data.
///
/// Author(s): Gary Stein<br>
/// Created: 2010<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org<br>
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
#include "statemachine/utility/objectscan.h"

using namespace Zebulon;
using namespace AI;
using namespace CxUtils;


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, sets defaults.
///
////////////////////////////////////////////////////////////////////////////////////
ObjectScan::ObjectScan()
{
    mDistanceThreshold = 1.0;
    mDerivativeThreshold = 1.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ObjectScan::~ObjectScan()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Calculates/generates values for objects based on a scan in
///         polar coordinates.
///
////////////////////////////////////////////////////////////////////////////////////
void ObjectScan::CalculatePolar(const Point3D::List& rangeScan)
{
    std::vector<int> startIndex;
    std::vector<int> endIndex;
    startIndex.push_back(0);
    for(int i =0; i < (int)rangeScan.size()-1;i++)
    {
        double derivative = rangeScan[i+1].mX - rangeScan[i].mX;
        if(fabs(derivative) > mDerivativeThreshold)
        {
            endIndex.push_back(i);
            startIndex.push_back(i+1);
        }
    }
    endIndex.push_back(rangeScan.size()-1);

    Point3D tmp;
    Point3D::List allObjects;
    for(int i=0;i < (int)startIndex.size();i++)
    {
        if(endIndex[i] - startIndex[i] > 2)
        {
            tmp =  (rangeScan[startIndex[i]] + rangeScan[endIndex[i]]) /  2;
            allObjects.push_back(tmp);
        }
    }

    mObjects.clear();
    for(int i=0;i < (int)allObjects.size();i++)
    {
        bool flag = false;
        for(int j=0; j < (int)mObjects.size();j++)
        {

            if(  ((allObjects[i].mX * allObjects[i].mX)
                 + (mObjects[j].mX * mObjects[j].mX))
                 - ((2*allObjects[i].mX * mObjects[j].mX)
                 * cos(allObjects[i].mY - mObjects[j].mY)) <  mDistanceThreshold * mDistanceThreshold)
            {
                mObjects[j]= allObjects[i] + mObjects[j] / 2;
                flag = true;
                break;
            }
        }
        if(flag == false)
        {
            mObjects.push_back(allObjects[i]);
        }
    }


}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Calculates/generates values for objects based on a scan in
///         cartesian coordinates.
///
///  \param[in] rangeScan LIDAR scan data.
///  \param[in] giveRadius If true, radius of object is saved (z component).
///  \param[in] angleRange Angle of laser (total scan range) (degrees).
///  \param[in] startAngle Angle to start calculations from (degrees).
///  \param[in] endAngle Angle to end calculations at (degrees).
///
////////////////////////////////////////////////////////////////////////////////////
void ObjectScan::CalculateCartesian(const CxUtils::Point3D::List& rangeScan,
                                    bool giveRadius,
                                    double angleRange,
                                    double startAngle,
                                    double endAngle)
{
    std::vector<int> startIndex;
    std::vector<int> endIndex;

    int startI = (int)((rangeScan.size()/2) + ((startAngle/(angleRange/2.0)) * (rangeScan.size()/2)));
    if(startI<0)
    {
        startI=0;
    }
    int endI = (int)((rangeScan.size()/2) + ((endAngle/(angleRange/2.0)) * (rangeScan.size()/2)));
    if(endI >= (int)rangeScan.size())
    {
        endI=rangeScan.size();
    }
    startIndex.push_back(startI);

    for(int i =startI;i<endI-1;i++)
    {
        if(AI::Utility::CalcDistance(rangeScan[i].mX,rangeScan[i].mY,rangeScan[i+1].mX,rangeScan[i+1].mY)>mDerivativeThreshold)
        {
            endIndex.push_back(i);
            startIndex.push_back(i+1);
        }
    }
    endIndex.push_back(endI-1);

    Point3D tmp;
    Point3D::List allObjects;
    std::vector<double> allRadii;
    for(int i=0;i< (int)startIndex.size();i++)
    {
        if(endIndex[i] - startIndex[i] > 2)
        {
            tmp =  (rangeScan[startIndex[i]] + rangeScan[endIndex[i]]) /  2;

            tmp.mZ=endIndex[i]-startIndex[i];

            double radius=(rangeScan[startIndex[i]] - rangeScan[endIndex[i]]).Magnitude()/2.0;
            allRadii.push_back(radius);
            allObjects.push_back(tmp);
        }
    }

    std::vector<double> tmpRadii;
    Point3D::List tmpObjects;
    for(int i=0;i< (int)allObjects.size();i++)
    {
        if(AI::Utility::CalcDistance(0.0,0.0,allObjects[i].mX,allObjects[i].mY) < mMinDistance)
        {
            continue;
        }
        bool flag = false;
        for(int j=0; j < (int)tmpObjects.size();j++)
        {
            double tmpDist=AI::Utility::CalcDistance(tmpObjects[j].mX,tmpObjects[j].mY,allObjects[i].mX,allObjects[i].mX);
            if( tmpDist <  mDistanceThreshold )
            {
                tmpObjects[j].mX = (allObjects[i].mX*allObjects[i].mZ + tmpObjects[j].mX*tmpObjects[j].mZ)/(tmpObjects[j].mZ+allObjects[i].mZ);
                tmpObjects[j].mY = (allObjects[i].mY*allObjects[i].mZ + tmpObjects[j].mY*tmpObjects[j].mZ)/(tmpObjects[j].mZ+allObjects[i].mZ);
                //grow radius
                if(tmpDist>tmpRadii[j])
                {
                    tmpRadii[j]=tmpDist;
                }
                tmpObjects[j].mZ+=allObjects[i].mZ;
                flag = true;
                break;
            }
        }
        if(flag == false)
        {
            allObjects[i].mZ=allObjects[i].mZ;
            tmpRadii.push_back(allRadii[i]);
            tmpObjects.push_back(allObjects[i]);
        }
    }

    mObjects.clear();
    for(int i=  (int)(tmpObjects.size()-1);i>=0;i--)
    {
        if(AI::Utility::CalcDistance(0.0,0.0,tmpObjects[i].mX,tmpObjects[i].mY) < mMaxDistance)
        {
            if(giveRadius)
            {
                tmpObjects[i].mZ=tmpRadii[i];
            }
            mObjects.push_back(tmpObjects[i]);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets up how objects are calculated.
///
///  \param[in] distanceThreshold Distance threshold between objects found for
///                               possible merging (meters).
///  \param[in] derivativeThreshold Distance threshold (meters) between points
///                                 next to each other in the ordered scan
///                                 for grouping together.
///  \param[in] maxDistance Maximum distance to check for objects.
///  \param[in] minDistance Minimum for checking for objects.
///
////////////////////////////////////////////////////////////////////////////////////
void ObjectScan::Setup(double distanceThreshold, double derivativeThreshold, double maxDistance,double minDistance)
{
    mDistanceThreshold = distanceThreshold;
    mDerivativeThreshold = derivativeThreshold;
    mMaxDistance = maxDistance;
    mMinDistance = minDistance;
}

void ObjectScan::SimpleSetup(double maxDistance,double minDistance, double angleRange, double startAngle, double endAngle)
{
    // positive
    mStartAngle = startAngle;
    // negative
    mEndAngle = endAngle;
    mAngleRange = angleRange;
    mMaxDistance = maxDistance;
    mMinDistance = minDistance;
}

double ObjectScan::SimpleDistance(const CxUtils::Point3D::List& rangeScan, bool &valid)
{
    //std::cout << "In SimpleDistance" << std::endl;
    
    std::vector<int> startIndex;
    std::vector<int> endIndex;
    
    double distance = 0;
    double pointCount = 0;

    int startI =  (int)((rangeScan.size()/2) + ((mStartAngle/(mAngleRange/2.0)) * (rangeScan.size()/2)));
    if(startI<0)
    {
        startI=0;
    }
    int endI =  (int)((rangeScan.size()/2) + ((mEndAngle/(mAngleRange/2.0)) * (rangeScan.size()/2)));
    if(endI>= (int)rangeScan.size())
    {
        endI=rangeScan.size();
    }
    //startIndex.push_back(startI);

    for(int i =startI;i<endI-1;i++)
    {
        if((AI::Utility::CalcDistance(0.0,0.0,rangeScan[i].mX,rangeScan[i].mY) > mMinDistance) && (AI::Utility::CalcDistance(0.0,0.0,rangeScan[i].mX,rangeScan[i].mY) < mMaxDistance))
        {
            //std::cout << "Within Bounds" << std::endl;
            distance += AI::Utility::CalcDistance(0.0,0.0,rangeScan[i].mX,rangeScan[i].mY);
            pointCount++;
        }
    }
    //endIndex.push_back(endI-1);
    
    if(pointCount >= 1)
    {
        distance /= pointCount;
    }
    
    
    if(distance > 0)
    {
        valid=true;
    }
    else
    {
        distance = 10000;
    }
    
    return distance;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets up how objects are calculated.
///
///  \param[in] valid If true, data is valid.
///  \param[in] type How to sort the data.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D::List ObjectScan::GetObjects(bool &valid, const GetType type)
{
    valid=false;
    if(mObjects.size()>0)
    {
        valid=true;
    }
    switch(type)
    {
        case LEFTTORIGHT:
            return mObjects;
            break;
        case CLOSEST:
        case BIGGEST:
            return Sort(type);
            break;
    }
    return Point3D::List();
}

Point3D::List ObjectScan::Sort(GetType type)
{
    Point3D tmp;
    //make a copy of the list
    Point3D::List CopyObjects=mObjects;
    int size=CopyObjects.size();
    for(int i=0;i<size-1;i++)
    {
        for(int j=i+1;j<size;j++)
        {
            if((type==CLOSEST && AI::Utility::CalcDistance(0.0,0.0,CopyObjects[i].mX,CopyObjects[i].mY) > AI::Utility::CalcDistance(0.0,0.0,CopyObjects[j].mX,CopyObjects[j].mY))
                || (type==BIGGEST && CopyObjects[i].mZ < CopyObjects[j].mZ))
            {
                tmp=CopyObjects[i];
                CopyObjects[i]=CopyObjects[j];
                CopyObjects[j]=tmp;
            }
        }
    }
    return CopyObjects;
}
