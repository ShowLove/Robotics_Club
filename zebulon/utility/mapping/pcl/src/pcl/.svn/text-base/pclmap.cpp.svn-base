//////////////////////////////////////////////////////////////////////////////////////
///
/// \file pclmap.cpp
/// \brief Interface for PCL.
///
/// Author(s): Tuyen Do<br>
/// Created: 5/08/2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: nintwindo@knights.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the Robotics Laboratory, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
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
//////////////////////////////////////////////////////////////////////////////////////

#include "pcl/pclmap.h"

using namespace Zebulon;
using namespace IGVC;
using namespace PCL;

PclMap::PclMap() { } // default constructor

void PclMap::Initialize()
{
    // Initialize Pointers
    mCloud =  pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
    mRawData = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
    mIncomingData = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
    mCloudStructPtr = 0;

    // Initiailizes new laser data flag
    mNewLaserDataFlag = false;

    // Initializes the current angle.
    mCurrentAngle = 0;

    // Initializes the robots current position within mCloud.
    //mCurrentPosition.x = 0;
    //mCurrentPosition.y = 0;
    //mCurrentPosition.z = 0;
} // Initilize w/o Laser

void PclMap::Initialize(Zebulon::Laser::GenericLaser* laser)
{
    Initialize();

    laser->RegisterCallback(this);
} // Initilize

void PclMap::Initialize(Zebulon::Laser::GenericLaser* laser, Zebulon::Motor::Dynamixel* dynamixel)
{
    Initialize(laser);

    dynamixel->RegisterCallback(this);
    mDynamixelController = dynamixel;
} // Initilize

void PclMap::AddPoint(pcl::PointXYZRGB point)
{
    mMutex1.lock();
    mCloud->push_back(point);
    mMutex1.unlock();
} // addPoint

void PclMap::AddPoint(float x, float y, float z, float rgb)
{
    pcl::PointXYZRGB p;
    p.x = x;
    p.y = y;
    p.z = z;
    p.rgb = rgb;

    mMutex1.lock();
    mCloud->push_back(p);
    mMutex1.unlock();
} // addPoint

void PclMap::AssignPointers()
{
    int i = 0;
    time_t now = time(&now);

    // Checks if the incoming cloud data has the same angle as a cloud within mCloudVector.
    // If it finds a cloud with the same angle, assigns the StructPtr.
    for (i = 0; i < mCloudVector.size(); i++)
    {
        if (mCloudVector[i].angle == mCurrentAngle)
        {
            // Clears old clouds
            if (difftime(now, mCloudVector[i].lastUpdated) > CLOUD_EXPIRE_TIME)
                mCloudVector[i].cloud->clear();
            mCloudStructPtr = &mCloudVector[i];
            break;
        } // if
    } // for

    // If a no cloud with the same angle is found, creates a new CloudStruct, inserts it into
    // mCloudVector and assigns the StructPtr to the new cloud.
    if (i >= mCloudVector.size())
    {
        // Initializes a new CloudStruct to insert into the mCloudVector
        CloudStruct cs;
        cs.cloud = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
        mCloudVector.push_back(cs);
        mCloudStructPtr = &mCloudVector.back();
    } // if

} // assignPointers method

void PclMap::ClearCloudVector()
{
    mMutex1.lock();
    mCloudVector.clear();
    mMutex1.unlock();
} // ClearStructVector method

bool PclMap::CollideLine(pcl::PointXYZ point1, pcl::PointXYZ point2, double width)
{
    mTestLine.point1 = point1;
    mTestLine.point2 = point2;

    // The difference between the X and Y coordinates.
    float deltaX = (point2.x - point1.x);
    float deltaY = (point2.y - point1.y);
    // The length of the line segment between the point1 and point2.
    float length = sqrt(deltaX * deltaX + deltaY * deltaY);

    pcl::PointXYZRGB centerPoint; // the center of the rectangle
    centerPoint.x = ((point2.x + point1.x) / 2);
    centerPoint.y = ((point2.y + point1.y) / 2);

    std::vector<int> indices;     // Stores the returned indices from radiusSearch below.
    std::vector<float> distances; // Stores the returned squared distances from radiusSearch.

    // Half of the hypotenuse of the triangle create by opposite points of the rectangle.
    float searchRadius = (sqrt(length * length + width * width)) * (SEARCH_TOLERANCE / 2);

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr pc = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>);
    mMutex1.lock();
    *pc = *mCloud;
    mMutex1.unlock();

    pcl::search::KdTree<pcl::PointXYZRGB> searcher;
    searcher.setInputCloud(pc);
    searcher.radiusSearch(centerPoint, searchRadius, indices ,distances,0);

    for (int i = 0; i < indices.size(); i++)
    {
        // Calculates the perpendicular distance from the line segment to the given point found.
        if (abs((pc->points[indices[i]].x - point1.x) * (point2.y - point1.y) -
                (pc->points[indices[i]].y - point1.y) * (point2.x - point1.x) ) / length <= (width / 2) )
        {
            pcl::PointXYZ point1Perp; // first point for the perpendicular bisector of the original line.
            pcl::PointXYZ point2Perp; // second point for the perpendicular bisector of the original line.

            // slope in the slope-line equations y = mx + b, where the m is the negitive reciprocle for perpendicular points
            double perpB = (deltaX / deltaY) * centerPoint.x + centerPoint.y;

            // If deltaY != 0, then the point is not horizontal
            if (deltaY)
            {
                point1Perp.x = point1.x;
                point1Perp.y = (-deltaX / deltaY) * point1Perp.x + perpB;
                point2Perp.x = point2.x;
                point2Perp.y = (-deltaX / deltaY) * point2Perp.x + perpB;
            } // if
            // Else deltaY is equal to zero, the perpendicular bisector is a verticle line.
            else
            {
                point1Perp.x = centerPoint.x;
                point1Perp.y = (centerPoint.y + (width / 2) );
                point2Perp.x = centerPoint.x;
                point2Perp.y = (centerPoint.y - (width / 2) );
            } // else

            // // Calculates the perpendicular distance from the perpendicular bisector to the given point found.
            if (abs((pc->points[indices[i]].x - point1Perp.x) * (point2Perp.y - point1Perp.y) -
                    (pc->points[indices[i]].y - point1Perp.y) * (point2Perp.x - point1Perp.x) ) / length <= (length / 2) )
                return true;
        } // if
    } // for loop

    return false;
} // CollideLine

// bool PclMap::CollideSphere
    // Returns: number of points that collided.
int PclMap::CollideSphere(pcl::PointXYZ center, double radius)
{
    pcl::PointXYZRGB p; // temporary point used to convert a PointXYZ into PointXYZRGB
    p.x = center.x;
    p.y = center.y;
    p.z = center.z;

    std::vector<int> indices;     // stores the returned indices from radiusSearch below.
    std::vector<float> distances; // stores the returned squared distances from radiusSearch.

    pcl::search::KdTree<pcl::PointXYZRGB> searcher; // KdTree search object.

    mMutex1.lock();
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr pc = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
    *pc = *mCloud;
    mMutex1.unlock();

    searcher.setInputCloud(pc);
    // Returns the number of points within the given sphere.
    // NOTE: radius is multiplied by 1.001, otherwise the search will miss points on the line of the sphere.
    return (searcher.radiusSearch(p, radius * 1.001, indices, distances, 0));
} // CollideSphere

//unsigned int PclMap::getCloudVectorSize()
//{
//    mMutex1.lock();
//    int size = mCloudVector.size();
//    mMutex1.unlock();

//    return size;
//} // getCloudVectorSize method

double PclMap::GetLaserAngle()
{
    mMutex1.lock();
    double temp = mCurrentAngle;
    mMutex1.unlock();

    return temp;
} // getLaserAngle method

bool PclMap::LoadMcloud(std::string fileName)
{
    bool returnValue = true;

    mMutex1.lock();
    mCloud->clear();
    if (pcl::io::loadPCDFile<pcl::PointXYZRGB> (fileName, *mCloud) == -1)
    {
        std::cout << "Error: Cound not read file: " << fileName << std::endl;
        returnValue = false;
    } // if

    std::cout << "Loaded " << mCloud->width * mCloud->height
                << " data points from " << fileName << std::endl;
    mMutex1.unlock();

    return returnValue;
} // LoadMap method

bool PclMap::LoadMcloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud)
{
    mMutex1.lock();
    *mCloud = *cloud;
    mMutex1.unlock();
} // LoadCloud method

void PclMap::MakeOscillateDynamixelThread()
{
    mThread_Oscillate.CreateThread(PclMap::OscillateDynamixelThread, this);
} // MakeOscillateDynamixelThread method

void PclMap::MakeRegistrationThread()
{
    mThread_Registration.CreateThread(PclMap::RegistrationThread, this);
} // MakeRegistrationThread

/** VisualizeCloud
 * Red Axis = X Axis
 * Green Axis = Y Axis
 * Blue Axis = Z Axis
 */
void PclMap::MakeVisualizeMcloudThread()
{
    mThread_Visualize.CreateThread(PclMap::VisualizeMcloudThread, this);
} // MakeVisualizeCloudThread

void PclMap::MergeRaw(bool clear)
{
    time_t now = time(&now);
    std::vector<CloudStruct>::iterator iter;

    mMutex1.lock();
    if (clear) mRawData->clear(); // if clear flag is set, clears the cloud.

    for(iter = mCloudVector.begin(); iter != mCloudVector.end(); iter++)
    {
        // Gets rid of old data that has NOT been updated.
        if (difftime(now, iter->lastUpdated) > CLOUD_EXPIRE_TIME)
        {
            mCloudVector.erase(iter);
            iter--; // since I have changed the structure of the vector, I want to have it repeat this iteration.
        } // if
        else
            if (iter->cloud->width > 0) *mRawData += *iter->cloud;
    } // for

    mMutex1.unlock();
} // mergeClouds method

void PclMap::OscillateDynamixelThread(void * ptr)
{
    PclMap * map = (PclMap *)ptr;

    map->mMutex1.lock();
    float angle = map->mCurrentAngle;
    map->mMutex1.unlock();

    while (!map->mThread_Oscillate.QuitThreadFlag())
    {
        for (angle = MIN_DYNAMIXEL_ANGLE; angle > MAX_DYNAMIXEL_ANGLE; angle--)
        {
            map->mDynamixelController->SetServoDegrees(DYNAMIXEL_SERVO_ID, angle);
            boost::this_thread::sleep(boost::posix_time::milliseconds(20));
        } // for
        for (; angle < MIN_DYNAMIXEL_ANGLE; angle++)
        {
            map->mDynamixelController->SetServoDegrees(DYNAMIXEL_SERVO_ID, angle);
            boost::this_thread::sleep(boost::posix_time::milliseconds(20));
        } // for
    } // while
} // OscillateDynamixel method

void PclMap::ProcessLaserData(const CxUtils::Point3D::List& scan, const CxUtils::Time& timestamp, const unsigned int id)
{
    // Initializes the incoming cloud data.
    pcl::PointXYZRGB point;

    mIncomingData->clear();  // Clears previous data.

    CxUtils::Point3D::List::const_iterator iter;
    for(iter=scan.begin(); iter!=scan.end(); iter++)
    {
        if (iter->mX < MAX_LASER_RANGE)
        {
            point.x = iter->mX * cos(iter->mZ) * cos(iter->mY); //+ mCurrentPosition.x;
            point.y = iter->mX * sin(iter->mZ) * cos(iter->mY); //+ mCurrentPosition.y;
            //point.z = iter->mX * sin(iter->mY);

            mMutex1.lock();
            point.z = tan(CxUtils::CxToRadians(mCurrentAngle)) * iter->mX;
            mMutex1.unlock();

            uint32_t rgb = SetIncomingPointColor(iter->mX);
            point.rgb = *reinterpret_cast<float*>(&rgb);

            mIncomingData->push_back(point);
        } // if
    } // for

    mMutex1.lock();
    AssignPointers(); // assigns the global cloud pointer.
    if (mCloudStructPtr != 0)
    {
        mCloudStructPtr->cloud = mIncomingData;
        mCloudStructPtr->angle = mCurrentAngle;
        mCloudStructPtr->sweep = (mCloudVector.size()) % 2;
        time(&mCloudStructPtr->lastUpdated); // time stamp
    } // if

    mNewLaserDataFlag = true;   // Flags that new laser data has arrived.
    mMutex1.unlock();
} // ProcessLaserData method

void PclMap::ProcessServoData(const double pos, const unsigned int id)
{
    mMutex1.lock();
    mCurrentAngle = ((pos + 100) * 1.5) - DYNAMIXEL_ANGLE_OFFSET; // Offset of the Dynamixel
    mMutex1.unlock();
} // ProcessServoData method

void PclMap::RegistrationThread(void * ptr)
{
    PclMap * map = (PclMap *) ptr;

    bool flag = false; // Flag used to signal when new laser data needs to be processed.
    // Copy of the incoming laser data.
    pcl::PointCloud<pcl::PointXYZRGB> alignedCloud;// = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr rawData1 = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr rawData2 = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
    std::vector<CloudStruct>::iterator iter;

    while (!map->mThread_Registration.QuitThreadFlag())
    {
        map->mMutex1.lock();
        flag = map->mNewLaserDataFlag;
        map->mMutex1.unlock();

        if (flag)
        {
            map->mMutex1.lock();
            for (iter = map->mCloudVector.begin(); iter != map->mCloudVector.end(); iter++)
            {
                if (iter->sweep) *rawData1 += *iter->cloud;
                else *rawData2 += *iter->cloud;
            } // for
            map->mMutex1.unlock();

            pcl::IterativeClosestPoint<pcl::PointXYZRGB, pcl::PointXYZRGB> icp;
            // The new scan data.
            icp.setInputCloud(rawData1);
            // What the aligned cloud should align to.
            icp.setInputTarget(rawData2);
            // Set the max correspondence distance in cm (.05 = 5 cm) (e.g., correspondences with higher distances will be ignored)
            icp.setMaxCorrespondenceDistance (0.05);
            // Set the maximum number of iterations (criterion 1)
            icp.setMaximumIterations(MAX_ICP_ITERATIONS);
            // Set the transformation epsilon (criterion 2)
            icp.setTransformationEpsilon (1e-8);
            // Set the euclidean distance difference epsilon (criterion 3)
            icp.setEuclideanFitnessEpsilon (1);

            alignedCloud.clear();
            icp.align(alignedCloud);

            // If the clouds have converged, combines mIncomingLaserData with the current mCloud.

            if (icp.hasConverged())
            {
                map->mMutex1.lock();
                *map->mCloud = alignedCloud;
                map->mMutex1.unlock();
            } // if
            else
            {
                std::cout << "Did not converge" << std::endl;
                // Flush data or something
            } // else
        } // if (flag)

        map->mMutex1.lock();
        map->mNewLaserDataFlag = false;
        map->mMutex1.unlock();

        boost::this_thread::sleep(boost::posix_time::milliseconds(REGISTRATION_SLEEPTIME));

    } // while

} // Registration method

//void PclMap::RegistrationThread(void * ptr)
//{
//    PclMap * map = (PclMap *) ptr;
//    // Copy of the incoming laser data.
//    boost::shared_ptr<CloudStruct> cloudPtr = boost::shared_ptr<CloudStruct>(new CloudStruct);;
//    pcl::PointCloud<pcl::PointXYZRGB>::Ptr IncomingData = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
//    pcl::PointCloud<pcl::PointXYZRGB>::Ptr alignedCloud = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
//    // Flag used to signal when new laser data needs to be processed.
//    bool flag = false;

//    while (!map->mThread_Registration.QuitThreadFlag())
//    {
//        map->mMutex1.lock();
//        flag = map->mNewLaserDataFlag;
//        map->mMutex1.unlock();

//        if (flag)
//        {
//            map->mMutex1.lock();
//            *IncomingData = *map->mIncomingLaserData.cloud;
//            cloudPtr = map->mCloudStructPtr;
//            unsigned int size = map->mCloudStructPtr->cloud->size();
//            map->mMutex1.unlock();

//            // If mCloud has no points or cloud has NOT converged for a while, then mCloud is just mIncomingLaserData.
//            if (size == 0)
//            {
//                *cloudPtr->cloud = *IncomingData;
//                time(&cloudPtr->lastUpdated);
//            } // if
//            else
//            {
//                pcl::IterativeClosestPoint<pcl::PointXYZRGB, pcl::PointXYZRGB> icp;

//                // The new scan data.
//                icp.setInputCloud(IncomingData);
//                // What the aligned cloud should align to.
//                icp.setInputTarget(cloudPtr->cloud);
//                // Set the max correspondence distance in cm (.05 = 5 cm) (e.g., correspondences with higher distances will be ignored)
//                icp.setMaxCorrespondenceDistance (0.05);
//                // Set the maximum number of iterations (criterion 1)
//                icp.setMaximumIterations(MAX_ICP_ITERATIONS);
//                // Set the transformation epsilon (criterion 2)
//                icp.setTransformationEpsilon (1e-8);
//                // Set the euclidean distance difference epsilon (criterion 3)
//                icp.setEuclideanFitnessEpsilon (1);

//                alignedCloud->clear();
//                icp.align(*alignedCloud);
////                const Eigen::Matrix4f icpTransformation = icp.getFinalTransformation(); // Retrieves the transformation.

//                //int intialSize = map->mCloudPtr->width;
//                //pcl::registration::CorrespondenceEstimation corEst;
//                //corEst.setInputCloud();
//                //corEst.setInputTarget();
//                //corEst.determineCorrespondences();

//                // If the clouds have converged, combines mIncomingLaserData with the current mCloud.
//                if (icp.hasConverged())
//                {
//                    *cloudPtr->cloud = *alignedCloud;
//                    time(&cloudPtr->lastUpdated);

//                  // Updates the current position using Matrices

////                    Eigen::Matrix<float, 4, 1> currentPostionMatrix;
////                    currentPostionMatrix << map->mCurrentPosition.x, map->mCurrentPosition.y, map->mCurrentPosition.z, 1; // Adds values into matrix

////                    currentPostionMatrix = icpTransformation * currentPostionMatrix;
////                    map->mCurrentPosition.x = currentPostionMatrix[0,1];
////                    map->mCurrentPosition.y = currentPostionMatrix[1,1];
////                  // If Delta Y > SOME_VALUE erase CloudVector except for this one.
////                  std::cout << "X = " << map->mCurrentPosition.x * 10 << ", Y = " << map->mCurrentPosition.y * 10 << std::endl;
//                } // if
//                // If the data does NOT converge throw the incoming data into the cloud
//                else
//                {
//                    *cloudPtr->cloud = *IncomingData;
//                    time(&cloudPtr->lastUpdated);
//                } // else
//            } // else

//            map->mMutex1.lock();
//            map->mNewLaserDataFlag = false;
//            map->mMutex1.unlock();
//        } // if (flag)

//        boost::this_thread::sleep(boost::posix_time::milliseconds(REGISTRATION_SLEEPTIME));

//    } // while

//} // Registration method

bool PclMap::SaveMcloud(std::string fileName)
{
    bool returnValue = true;

    mMutex1.lock();
    if (pcl::io::savePCDFileASCII (fileName, *mCloud) == -1)
    {
        std::cerr << "Error: Could not save file: " << fileName << std::endl;
        returnValue = false;
    } // if

    std::cout << "Saved " << mCloud->points.size () << " data points to" << fileName << std::endl;
    mMutex1.unlock();

    return returnValue;
} // SaveMap

void PclMap::SetLaserAngle(double angle)
{
    mMutex1.lock();
    mCurrentAngle = angle;
    mMutex1.unlock();
} // SetLaserAngle


uint32_t PclMap::SetIncomingPointColor(double distance)
{
    // RGB Colors:
        // Orange = 255, 165, 0; Yellow = 255, 255, 0; Green = 0, 128, 0; Red = 255, 0, 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    r = (distance > (MAX_LASER_RANGE * .80 )) ? 0 : 255;

    if (distance > (MAX_LASER_RANGE * .80 ))
        g = 128;
    else if (distance > (MAX_LASER_RANGE * .60 ))
        g = 255;
    else if (distance > (MAX_LASER_RANGE * .40 ))
        g = 165;

    return ((uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b);
} // setIncomingPointColor method

bool PclMap::ShutDown()
{
    mThread_Oscillate.StopThread(400);
    mThread_Visualize.StopThread(400);
    mThread_Registration.StopThread(400);
    mThread_RawVisualize.StopThread(400);
} // ShutDown method

//void PclMap::Slam()
//{
//    // Doesn't work
//    pcl::registration::ELCH<pcl::PointXYZRGB> elch;

//    for (int i = 1; i < mCloudVector.size(); i++)
//        elch.addPointCloud(mCloudVector[i].cloud);

//    elch.setLoopStart(0);                   // Sets begining of loop. FIX THIS.
//    elch.setLoopEnd(mCloudVector.size());   // Sets the end of the loop. FIX THIS.
//    elch.compute();

//    typedef boost::adjacency_list<
//      boost::listS, boost::vecS, boost::undirectedS,
//      pcl::registration::ELCH<pcl::PointXYZRGB>::Vertex,
//      boost::no_property>
//    LoopGraph;

//    typedef boost::shared_ptr< LoopGraph > LoopGraphPtr;

//    LoopGraphPtr loopGraphPtr;

//    loopGraphPtr = elch.getLoopGraph();
//} // SLAM method

// Don't ask me how this works
bool PclMap::TriangulateCloud()
{
    int const MAX_NEIGHBORS = 100; // how many neighbors are searched.
    // specifies the maximum acceptable distance for a point to be considered,
    // relative to the distance of the nearest point (in order to adjust to changing densities).
    int const MU = 3;
    int const K_SEARCH = 3; // no idea.
    int const SEARCH_RADIUS = 2; // the maximum radius to search.

    /* Load input file into a PointCloud<T> with an appropriate type
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
    sensor_msgs::PointCloud2 cloud_blob;
    pcl::io::loadPCDFile (READ_FILE, cloud_blob);       // loads cloud into cloud_blob
    pcl::fromROSMsg (cloud_blob, *cloud);
    * the data should be available in cloud */

    // Normal estimation*
    pcl::NormalEstimation<pcl::PointXYZRGB, pcl::Normal> n;
    pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal>);
    pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZRGB>);

    mMutex1.lock();
    tree->setInputCloud (mCloud);
    n.setInputCloud (mCloud);

    pcl::PointCloud<pcl::PointXYZRGB>::iterator iter; // An iterator to traverse through the mCloud.

    // Copys an PointXYZRGB into an PointXYZ cloud.
    pcl::PointCloud<pcl::PointXYZ> pc; // Copies cloud
    pc.height = mCloud->height;
    pc.width = mCloud->width;
    pc.is_dense = mCloud->is_dense;

    // Copys over the RGB cloud to a XYZ cloud. Needed for concatenateFields.
    for(iter = mCloud->begin(); iter != mCloud->end(); iter++)
    {
        pcl::PointXYZ p(iter->x, iter->y, iter->z);
        pc.push_back(p);
    } // for iter

    n.setSearchMethod (tree);
    n.setKSearch (K_SEARCH);
    n.compute (*normals);
    //* normals should not contain the point normals + surface curvatures
    mMutex1.unlock();

    // Concatenate the XYZ and normal fields*
    pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::PointNormal>);
    pcl::concatenateFields(pc, *normals, *cloud_with_normals);
    //* cloud_with_normals = cloud + normals

    // Create search tree*
    pcl::search::KdTree<pcl::PointNormal>::Ptr tree2 (new pcl::search::KdTree<pcl::PointNormal>);
    tree2->setInputCloud (cloud_with_normals);

    // Initialize objects
    pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
    //pcl::PolygonMesh triangles;

    gp3.setSearchRadius (SEARCH_RADIUS);

    // Set typical values for the parameters
    gp3.setMu (MU);
    gp3.setMaximumNearestNeighbors (MAX_NEIGHBORS);
    gp3.setMaximumSurfaceAngle(M_PI / 4);                 // 45 degrees
    gp3.setMinimumAngle(M_PI / 18);                       // 10 degrees
    gp3.setMaximumAngle(2 * M_PI / 3);                    // 120 degrees
    gp3.setNormalConsistency(false);

    // Get result
    gp3.setInputCloud (cloud_with_normals);
    gp3.setSearchMethod (tree2);
    gp3.reconstruct(mMesh);

    // Additional vertex information
    // std::vector<int> parts = gp3.getPartIDs();
    // std::vector<int> states = gp3.getPointStates();

    //pcl::io::saveVTKFile("mesh.vtk",triangles);        // saves triangles to file

    return true;
} // triangulateCloud

void PclMap::VisualizeCloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cPtr)
{
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer ("3D Viewer"));
    viewer->setBackgroundColor(0, 0, 0);
    //pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZRGB> single_color(cPtr, 255, 255, 255);
    viewer->addPointCloud<pcl::PointXYZRGB>(cPtr, "Cloud", 0);
    viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "Cloud");
    viewer->addCoordinateSystem(1.0);
    viewer->initCameraParameters();

    while (!viewer->wasStopped ())
    {
        viewer->updatePointCloud(cPtr, "Cloud");;
        viewer->spinOnce(100);
        boost::this_thread::sleep (boost::posix_time::milliseconds (VISUALIZE_SLEEPTIME));
    } // while

} // VisualizeCloud method

void PclMap::VisualizeMcloudThread(void * ptr)
{
    PclMap * map = (PclMap *)ptr;

    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer ("3D Viewer"));
    viewer->setBackgroundColor(0, 0, 0);
    map->mMutex1.lock();
    //pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZRGB> single_color(map->mCloud, 255, 255, 255);
    //viewer->addPointCloud<pcl::PointXYZRGB>(map->mCloud, single_color, cloudName, 0);
    viewer->addPointCloud<pcl::PointXYZRGB>(map->mCloud, "3D Cloud", 0);
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "3D Cloud");
    map->mMutex1.unlock();

    viewer->addCoordinateSystem(1.0);
    viewer->initCameraParameters();

//    boost::shared_ptr<pcl::visualization::PCLVisualizer> rawViewer(new pcl::visualization::PCLVisualizer ("Raw"));
//    rawViewer->setBackgroundColor(0, 0, 0);
//    map->mMutex1.lock();
//    //pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZRGB> single_color(map->mCloud, 255, 255, 255);
//    //viewer->addPointCloud<pcl::PointXYZRGB>(map->mCloud, single_color, cloudName, 0);
//    rawViewer->addPointCloud<pcl::PointXYZRGB>(map->mRawData, "Raw Data", 2);
//    rawViewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "Raw Data");
//    map->mMutex1.unlock();

//    rawViewer->addCoordinateSystem(1.0);
//    rawViewer->initCameraParameters();

    while (!viewer->wasStopped () && !map->mThread_Visualize.QuitThreadFlag())
    {
        //map->MergeRaw();
        map->mMutex1.lock();
        viewer->updatePointCloud(map->mCloud, "3D Cloud");
        //rawViewer->updatePointCloud(map->mRawData, "Raw Data");
        //viewer->addLine(map->mTestLine.point1, map->mTestLine.point2, "Test Line", 1);
        map->mMutex1.unlock();
        viewer->spinOnce(100);
        boost::this_thread::sleep (boost::posix_time::milliseconds (VISUALIZE_SLEEPTIME));
    } // while

} // VisualizemCloud method

bool PclMap::VisualizeMesh()
{
    //pcl::PolygonMesh mesh = meshToDraw;
    //pcl::io::loadPolygonFileVTK(fileName,meshToDraw); // loads mesh as .vtk

    pcl::visualization::PCLVisualizer visualizer ("mesh visualizer", true); // creates new visualizer "mesh visualizer" with create interactor = true
    visualizer.addPolygonMesh(mMesh, "polygon_mesh", 0); // adds a mesh-cloud to visualizer.

    while (!visualizer.wasStopped())
    {
        visualizer.spin(); // updates the render. see spinOnce() for one time only rendering
    } // while
} // VisualizeMesh

/*End of File*/
