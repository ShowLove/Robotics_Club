////////////////////////////////////////////////////////////////////////////////////
///
///  \file path.cpp
///  \brief Array of poses that make up a path.
///
///  <br>Author(s): David Adams, Jonathan Mohlenhoff
///  <br>Created: 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dadams@ist.ucf.edu, jmohlenh@ist.ucf
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

#include "cartographer/path.h"
#include "cartographer/object.h"
#include <cstdio>
using namespace Cartographer;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////////
Path::Path()
{
    Clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor
///
///   \param[in] pose starting the path.
///
////////////////////////////////////////////////////////////////////////////////////
Path::Path(const Pose& pose)
{
    Clear();
    Push(pose);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
Path::~Path()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Push a pose onto the end of the path, updates path distance, segments,
///          etc.
///
///   \param[in] point to put into the path.
///
////////////////////////////////////////////////////////////////////////////////////
void Path::Push(const Pose& point)
{
    if(mPath.size() >= 1)
    {
        mDistance += mPath.back().mPosition.Distance(point.mPosition);
        mSegments.push_back(Segment3D(mPath.back().mPosition,point.mPosition,mPathWidth));
    }
    UpdateLinearRegressionAdd(point);
    mPath.push_back(point);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Pops a point from the end of the path, and updates internal metrics.
///
///   \param[out] point Point popped. Null if no point exists.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Path::Pop(Pose* point)
{
    point = NULL;
    if(mPath.empty()) return false;
    point = &(mPath[mPath.size()-1]);
    mPath.erase(mPath.begin()+mPath.size()-1);
    UpdateLinearRegressionRemove(*point);
    mDistance -= mSegments.back().GetMagnitude();
    mSegments.erase(mSegments.begin()+mSegments.size()-1);
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Pops a point from the front of the path, and updates internal metrics.
///
///   \param[out] point Point popped. Null if no point exists.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Path::PopFront(Pose* point)
{
    //point = NULL;
    if(mPath.empty()) return false;
    Pose copy;
    if(point != NULL)
    {
        *point = (mPath[0]);
    }
    copy = mPath[0];
    mPath.erase(mPath.begin());
    UpdateLinearRegressionRemove(copy);
    if(!mSegments.empty())
    {
        mDistance -= mSegments.front().GetMagnitude();
        mSegments.erase(mSegments.begin());
    }
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the direction of the overall path direction using points with
///          linear regression technique, only looks at XY plane the yaw of the
///          path.
///
///   \param[out] dir Direction of overall path in radians.  Values are
///                   in map headings with 0 being north, positive values 
///                   rotating east, etc.
///
///   \return True on success, false on failure (check number of points in path).
///
////////////////////////////////////////////////////////////////////////////////////
bool Path::GetDirection(double& dir) const
{
    /*
    if(mPath.size() < 2)
    {
        return false;
    }
    double sumX = 0;
    double sumY = 0;
    double sumXY = 0;
    double avgX = 0;
    double avgY = 0;
    Pose::List::const_iterator point;
    for(point = mPath.begin();
        point != mPath.end();
        point++)
    {
        avgX += point->mPosition.mX;
        avgY += point->mPosition.mY;
    }
    avgX /= mPath.size();
    avgY /= mPath.size();
    for(point = mPath.begin();
        point != mPath.end();
        point++)
    {
        sumX += (point->mPosition.mX - avgX)*(point->mPosition.mX - avgX);
        sumY += (point->mPosition.mY - avgY)*(point->mPosition.mY - avgY);
        sumXY += (point->mPosition.mX - avgX)*(point->mPosition.mY - avgY);
    }

    double slope = atan2(sumX, sumXY);
    dir = slope;

    //dir = CxUtils::Orientation::AddToAngle(dir, CxUtils::CX_HALF_PI);
    */
    double yaw;
    double theta;
    double slopeXY;
    if(mPath.size() < 2)
        return false;

    double numerator, denominator;
    numerator = ((double)mPath.size()*mSumXY - mSumPoints.mX*mSumPoints.mY);
    denominator = ((double)mPath.size()*mSumPoints2.mX - mSumPoints.mX*mSumPoints.mX);
    if(denominator <  CxUtils::CX_EPSILON)
    {
        slopeXY = 1.0/CxUtils::CX_EPSILON;
    }
    else
    {
        slopeXY = numerator/denominator;
    }
    theta = atan(slopeXY);

    if(theta >= 0.0)
    {
        yaw = CxUtils::CX_HALF_PI - theta;
    }
    else
    {
        yaw = -CxUtils::CX_HALF_PI - theta;
    }

    if(mSumPointsDiff.mY < 0.0) //Traveling South, opposite direction than how yaw was calculated
    {
        if(yaw >= 0.0)
        {
            yaw += -CxUtils::CX_PI;
        }
        else
        {
            yaw += CxUtils::CX_PI;
        }
    }
    if(mSumPointsDiff.mX < 0 && fabs(slopeXY) < .001)
    {
        yaw *= -1;
    }

    dir = yaw*-1;
    // Convert to 0 being north, with positive values
    // to the right (global direction).
    //dir = CxUtils::Orientation::AddToAngle(dir, CxUtils::CX_HALF_PI);

    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the direction of the overall path direction using points with
///          linear regression technique.
///
///   \return Direction of overall path in radians.
///
////////////////////////////////////////////////////////////////////////////////////
double Path::GetDirection() const
{
    double yaw;
    if(GetDirection(yaw))
    {
        return yaw;
    }
    else
    {
        return 0.0;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns distance from start point to end point(not following path);
///
///   \return Distance in meters.
///
////////////////////////////////////////////////////////////////////////////////////
double Path::GetDistanceStartToEnd() const
{
    return mPath.front().mPosition.Distance(mPath.back().mPosition);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets point at index. 0 is begining, mPath.size() -1 is end.
///
///   \param[in] index in path list of point you want to retrieve.
///   \param[out] point that will be updated if found.
///
///   \return true if point exists, false if not.
///
////////////////////////////////////////////////////////////////////////////////////
bool Path::Get(const unsigned int index, Pose& point)
{
    if(index >= mPath.size()) return false;
    point = mPath[index];
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets line segment at index. 0 is begining, mPath.size()-2 is end.
///
///   \param[in] index of segment, 0 is first segment(must have at least 2 points)
///              mPath.size()-2 is last segment.
///   \param[out] segment that will be updated if found.
///
///   \return true if segment exists, false if not.
///
////////////////////////////////////////////////////////////////////////////////////
bool Path::Get(const unsigned int index, Segment3D& segment)
{
    if(index >= mSegments.size()) return false;
    segment = mSegments[index];
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Set width of path segments and updates metrics if needed.
///
///   \param[in] width Width of path in meters. Value must be >= 0.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Path::SetPathWidth(const double width)
{
    if(width < 0) return false;
    mPathWidth = width;
    for(Segment3D::List::iterator it = mSegments.begin();
        it != mSegments.end(); it++)
    {
        it->mWidth = width;
    }
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a point [distance] meters from the end of the path
///         (follows last points heading).
///
///   \param[in] pose The position to be relative to.
///   \param[in] distance Distance from last point to the next point.
///   \param[in] angleOffset Optional parameter to offset current angle in radians.
///
///   \return Pose where the projected line would end.
///
////////////////////////////////////////////////////////////////////////////////////
Pose Path::CalculateNextPoint(const Pose pose, const double distance, const double angleOffset)
{
    Point3D point;
    CxUtils::Quaternion quat;
    Pose projection = pose;
    projection.mRotation.mZ += angleOffset;
    if(projection.mpPoseRotation != NULL)
    {
        projection.mpPoseRotation->mZ += angleOffset;
        quat = *projection.mpPoseRotation;
    }
    else
    {
        quat.SetRotationZ(-projection.Yaw());
    }
    point.mY = distance;
    point = quat.Rotate(point);
    projection.mPosition += point;
    return projection;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears the path.
///
////////////////////////////////////////////////////////////////////////////////////
void Path::Clear()
{
    mPath.clear();
    mSegments.clear();
    mDistance = 0;
    mPathWidth = 0;
    mFitness = 0;
    mSumPoints = 0;
    mSumPoints2 = 0;
    mSumXY = 0;
    mSumPointsDiff = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Get number of points in the path.
///
///   \return number of poses in the path list.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Path::Size() const
{
    return (unsigned int)mPath.size();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks if any segment along the path intersects the point, takes into
///          account path width.
///
///   \param[in] point to check for intersection.
///   \param[in] threshold Intersect threshold (meters). Determins how close
///              to desitnation we must be.
///
///   \return true if point intersects, false if not.
///
////////////////////////////////////////////////////////////////////////////////////
bool Path::Intersects(const Pose& point, const double threshold) const
{
    if(mPath.size() <= 0) 
    {
        return false;
    }
    if(mPath[0].mPosition.Distance(point.mPosition) <= threshold) 
    {
        return true;
    }
    if(mSegments.size() <= 0) 
    {
        return false;
    }

    for(Segment3D::List::const_iterator it = mSegments.begin();
        it != mSegments.end(); it++)
    {
        if((*it).GetDistanceToPoint(point.mPosition) <= threshold)
        {
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief 
///
///   \param[in] pose
///
///   \return path object in local coordinates
///
////////////////////////////////////////////////////////////////////////////////////
Path Path::ConvertToLocalCoordinates(const Pose& pose) const
{
    // Create copy of current path
    Path path(*this);

    // For each pose in the path, convert to local based on 'pose' parameter
    Pose::List::iterator curPath;
    for(curPath = path.mPath.begin(); curPath != path.mPath.end(); curPath++)
    {
        Pose::ConvertFromMapToLocalCoordinates(pose,(*curPath).mPosition);
    }
    
    return path;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief 
///
///   \param[in] pose
///
///   \return path object in map coordinates
///
////////////////////////////////////////////////////////////////////////////////////
Path Path::ConvertToMapCoordinates(const Pose& pose) const
{
    // Create copy of current path
    Path path(*this);

    // For each pose in the path, convert to local based on 'pose' parameter
    Pose::List::iterator curPath;
    for(curPath = path.mPath.begin(); curPath != path.mPath.end(); curPath++)
    {
        Pose::ConvertFromLocalToMapCoordinates(pose,(*curPath).mPosition);
    }

    return path;
}

#ifdef USE_OPENCV
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws a path to the image.
///
///   \param[in] image Opencv image to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
///   \param[in] color to set background to.
///
///   \return If the object was able to be drawn or not.
///
////////////////////////////////////////////////////////////////////////////////////
bool Path::Draw(IplImage* image,
                const double pixelsPerMeter,
                const Point3D& mapBottomCorner,
                CvScalar color,
                bool drawText,
                bool drawNodes) const
{

    CvFont font;
    char charBuffer[64];

    cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0);
    if(mPath.empty()) return false;
    if(mPathWidth > 0)
    {
        Cartographer::Draw(image,pixelsPerMeter,mapBottomCorner,mPath[0].mPosition,mPathWidth/2.,color);
        for(unsigned int i = 0; i < mSegments.size(); i++)
        {
            Cartographer::Draw(image,pixelsPerMeter,mapBottomCorner,mSegments[i],color);
        }
        if(drawNodes)
        {
            for(unsigned int i = 0; i < mSegments.size(); i++)
            {
                Cartographer::Draw(image,pixelsPerMeter,mapBottomCorner,mSegments[i].mPoint2,mPathWidth,CV_RGB(255-color.val[0],255-color.val[1],255-color.val[2]));
            }
        }        

    
    //    for(unsigned int i = 0; i < mPath.size(); i++)
    //    {
    //        mPath[i].Draw(image,pixelsPerMeter,mapBottomCorner,mPathWidth*1.5,mPathWidth,CV_RGB(0,0,255));
    //        if(drawText)
    //        {
    //            sprintf(charBuffer,"%d",i);
    //            cvPutText(image, charBuffer, ConvertToImagePoint(image->height,
    //                        pixelsPerMeter,
    //                        mPath[i].mPosition - Point3D(mPathWidth/2.,mPathWidth/2.,0),
    //                        mapBottomCorner),
    //                        &font, CV_RGB(255,255,0));
    //        }
    //    }
    }
    else
    {
        for(unsigned int i = 0; i < mPath.size(); i++)
        {
            Cartographer::Draw(image,pixelsPerMeter,mapBottomCorner,mPath[i].mPosition,.5,color);
            if(drawText)
            {
                sprintf(charBuffer,"%d",i);
                cvPutText(image, charBuffer, ConvertToImagePoint(image->height,
                            pixelsPerMeter,
                            mPath[i].mPosition - Point3D(mPathWidth/2.,mPathWidth/2.,0),
                            mapBottomCorner),
                            &font, CV_RGB(255,255,0));
            }
        }
    }
    if(drawText)
    {
        sprintf(charBuffer,"%.2lf M %.3lf Fit",mDistance,mFitness);
        cvPutText(image, charBuffer, ConvertToImagePoint(image->height,
                                    pixelsPerMeter,
                                    mPath.back().mPosition + Point3D(mPathWidth/2.,mPathWidth/2.,0),
                                    mapBottomCorner),
                                    &font, CV_RGB(255,255,0));
    }
    return true;
}
#endif


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Updates the member variables that are used to calculate linear
///          regression of the path used to get an overall path direction. Call
///          this function when before a new pose is to be added to the path list.
///          BE SURE to call this function prior to adding the pose to the list as
///          this funtion uses the back point in the list to calculate a difference
///          to the point being added.
///
///   \param[in] point New pose to be added to the list, only uses position to
///                    update linear regression variables, DOES NOT add pose to
///                    the mPath list.
///
///   \return True on success.
///
////////////////////////////////////////////////////////////////////////////////////
bool Path::UpdateLinearRegressionAdd(const Cartographer::Pose &point)
{
    mSumPoints += point.mPosition;
    mSumPoints2.mX += point.mPosition.mX*point.mPosition.mX;
    mSumPoints2.mY += point.mPosition.mY*point.mPosition.mY;
    mSumPoints2.mZ += point.mPosition.mZ*point.mPosition.mZ;
    mSumXY += point.mPosition.mX * point.mPosition.mY;

    if(mPath.size() > 0)
    {
        mSumPointsDiff += point.mPosition - mPath.back().mPosition;
    }
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Updates the member variables that are used to calculate linear
///          regression of the path used to get an overall path direction. Call
///          this function after the back pose is to removed from the path list.
///          BE SURE to call this function only after the the pose has been deleted
///          from the list as this funtion uses the back point in the list to
///          recalculate a difference between the last to points of the list.
///
///   \param[in] point Old pose removed from the list, only uses position to
///                    update linear regression variables, DOES NOT remove pose to
///                    the mPath list.
///
///   \return True on success.
///
////////////////////////////////////////////////////////////////////////////////////
bool Path::UpdateLinearRegressionRemove(const Cartographer::Pose &point)
{
    mSumPoints -= point.mPosition;
    mSumPoints2.mX -= point.mPosition.mX*point.mPosition.mX;
    mSumPoints2.mY -= point.mPosition.mY*point.mPosition.mY;
    mSumPoints2.mZ -= point.mPosition.mZ*point.mPosition.mZ;
    mSumXY -= point.mPosition.mX * point.mPosition.mY;

    if(mPath.size() > 0)
    {
        mSumPointsDiff -= point.mPosition - mPath.back().mPosition;
    }
    return true;
}

/*End of File*/
