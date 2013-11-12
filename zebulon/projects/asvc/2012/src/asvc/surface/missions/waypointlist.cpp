#include "asvc/surface/missions/waypointlist.h"


using namespace Zebulon;
using namespace ASVC;

const std::string WayPointList::Name = "WayPointList";


WayPointList::WayPointList()
{
    
}

WayPointList::~WayPointList()
{
    
}

int WayPointList::Init()
{
    mCurrentVelocity = mDesiredVelocity = 0;
    //mDesiredPositionList.push_back(Utm(CxUtils::Wgs(38.889588, -77.023333)));
    //mDesiredPositionList.push_back(Utm(CxUtils::Wgs(38.889784, -77.021493)));
    //mDesiredPositionList.push_back(Utm(CxUtils::Wgs(38.89004, -77.01458)));
    
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58521, -81.19915)));
    
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58518, -81.19906)));
    
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58507, -81.19904)));
    
    mDesiredPositionIterator = mDesiredPositionList.begin();
    return 0;
}

int WayPointList::FetchProcess()
{
    mXmlConf.GetVar("Surface.WayPointList.LIDAR@MaxDistance", mMaxDistanceThresh);

    mXmlConf.GetVar("Surface.WayPointList.Controls@DistanceThresh", mDistanceThresh);
    mXmlConf.GetVar("Surface.WayPointList.Controls@ForceWeight", mForceWeight);
    mXmlConf.GetVar("Surface.WayPointList.Controls@SinkWeight", mSinkWeight);

//mMaxVelocity
    mXmlConf.GetVar("Surface.WayPointList.Controls@MaxVelocity", mMaxVelocity);

    mCurrentPosition = mpGlobalInfo->GetPosition();
    mCurrentOrientation = mpGlobalInfo->GetOrientation();

    mCurrentVelocity = mpGlobalInfo->GetVelocityX();

    // LASER
    mpGlobalInfo->GetInfo("LIDAR", mRangeData);

    return 0;
}

std::string WayPointList::ExitEarly()
{
    return "KeepRunning";
}

std::string WayPointList::Execute()
{
    double distance;
    switch (mState)
    {
    case GoToWaypoint:
        {
            
            
            /*CxUtils::Point3D force(0, 0, 0);
            CxUtils::Point3D::List::iterator iter;
            for (iter = mRangeData.begin(); iter != mRangeData.end(); iter++)
            {
                distance = iter->SumOfSquares() + CxUtils::CX_EPSILON;
                force -= (CxUtils::Point3D(iter->mX, iter->mY, iter->mZ)) * (mForceWeight/distance);
            }

            CxUtils::Point3D globalForce = CxUtils::Point3D(mDesiredPositionIterator->mNorthing-mCurrentPosition.mNorthing,
                                                            mDesiredPositionIterator->mEasting-mCurrentPosition.mEasting, 0)
                                           * (mSinkWeight);
*/
            double northing, easting;
            int zoneNum, zoneLetter;
            mDesiredPositionIterator->Get(northing, easting, zoneNum, zoneLetter);
            Utm desiredPos(northing, easting, zoneNum, zoneLetter);

            distance = Utm::Distance(desiredPos, mCurrentPosition);
            //double angle = CxUtils::Orientation::AngleDiff(mCurrentOrientation.mZ,
            //                                               CxUtils::Orientation::GetGlobalAngle(mCurrentPosition, desiredPos));
            
            double angle = CxUtils::Orientation::GetGlobalAngle(mCurrentPosition, desiredPos);
            
            //force += globalForce.Rotate(angle, 2);
            if (distance < mDistanceThresh)
            {
                mDesiredVelocity = 0;
                mState = NextWaypoint;
            }
            else
            {
                //mDesiredOrientation = mCurrentOrientation + CxUtils::Point3D(0, 0, angle);
                mDesiredVelocity = 0;
                mDesiredOrientation = CxUtils::Point3D(0, 0, angle);
                if (fabs(CxUtils::Orientation::AngleDiff(mCurrentOrientation.mZ,angle)) < CxUtils::CX_PI/3.0)
                    mDesiredVelocity = mMaxVelocity;    //2.0 - 2.0/force.SumOfSquares();
            }

            break;
        }
    case NextWaypoint:
        {
            mDesiredVelocity = 0;
            mDesiredPositionIterator++;
            if (mDesiredPositionIterator == mDesiredPositionList.end())
            {
                mState = Finish;
            }
            else
            {
                mState = GoToWaypoint;
            }
            break;
        }
    case Finish:
        {
            mDesiredVelocity = 0;
            mDesiredPositionIterator = mDesiredPositionList.begin();
            mState = GoToWaypoint;
            break;
        }
    }

    printf("WayPointList:: Heading: %lf -> %lf \n Velocity: %lf -> %lf\n", CxUtils::CxToDegrees(mCurrentOrientation.mZ), CxUtils::CxToDegrees(mDesiredOrientation.mZ), mCurrentVelocity, mDesiredVelocity);
    printf("Distance: %lf", distance);
    
    mpGlobalCommand->SetYaw(mDesiredOrientation.mZ);
    mpGlobalCommand->SetForwardVelocity(mDesiredVelocity);
    return "KeepRunning";
}

int WayPointList::ErrorCheck()
{
    return 0;
}
