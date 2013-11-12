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
    mCloud =  pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
    mIncomingLaserData = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
    mAlignedCloud =  pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
    mCloudConvergence = 0;

    mCurrentPosition.x = 0;
    mCurrentPosition.y = 0;
    mCurrentPosition.z = 0;

    //Zebulon::Motor::Dynamixel test = new Zebulon::Motor::Dynamixel();

} // Initilize w/o Laser

void PclMap::Initialize(Zebulon::Laser::GenericLaser* laser)
{
    mCloud =  pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
    mIncomingLaserData = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
    mAlignedCloud =  pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
    mCloudConvergence = 0;

    mCurrentPosition.x = 0;
    mCurrentPosition.y = 0;
    mCurrentPosition.z = 0;

    //Zebulon::Motor::Dynamixel test = new Zebulon::Motor::Dynamixel();

    laser->RegisterCallback(this);
} // Initilize

void PclMap::AddPoint(float x, float y, float z)
{
    pcl::PointXYZRGB tp;
    tp.x = x;
    tp.y = y;
    tp.z = z;
    tp.rgb = INITIAL_POINT_LIFE;
    mCloud->insert(mCloud->end(), tp);
} // addPoint

void PclMap::AddPoint(pcl::PointXYZRGB point)
{
    mCloud->insert(mCloud->end(), point);
} // addPoint

/** VisualizeCloud
  * Red Axis = X Axis
  * Green Axis = Y Axis
  * Blue Axis = Z Axis
  */
void PclMap::MakeVisualizeCloudThread()
{
    mVisualizeThread.CreateThread(PclMap::VisualizeCloud, this);
} // MakeVisualizeCloudThread

void PclMap::VisualizeCloud(void * ptr)
{
    PclMap * map = (PclMap *)ptr;
    const std::string cloudName = "Cloud";

    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
    viewer->setBackgroundColor (0, 0, 0);
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZRGB> single_color (map->mCloud, 0, 255, 0);
    map->mMutex2.lock();
    viewer->addPointCloud<pcl::PointXYZRGB> (map->mCloud, single_color, cloudName, 0);
    viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, cloudName);
    viewer->addCoordinateSystem (1.0);
    viewer->initCameraParameters ();
    map->mMutex2.unlock();

    while (!viewer->wasStopped () && !map->mVisualizeThread.QuitThreadFlag())
    {
        map->mMutex2.lock();
        viewer->updatePointCloud(map->mCloud, cloudName);
        map->mMutex2.unlock();
        viewer->spinOnce (100);
        viewer->removeShape("CurrentPosition");
        viewer->addSphere(map->mCurrentPosition, .2, .5, .5, 0, "CurrentPosition");
        boost::this_thread::sleep (boost::posix_time::milliseconds (VISUALIZE_SLEEPTIME));
    } // while

} // VisualCloud

// Don't ask me how this works
bool PclMap::TriangulateCloud()
{
    int const MAX_NEIGHBORS = 100; // how many neighbors are searched.
    int const MU = 3; // specifies the maximum acceptable distance for a point to be considered, relative to the distance of the nearest point (in order to adjust to changing densities)
    int const K_SEARCH = 3; // no idea.
    int const SEARCH_RADIUS = 1; // specifies the maximum acceptable distance for a point to be considered, relative to the distance of the nearest point (in order to adjust to changing densities)

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
    tree->setInputCloud (mCloud);
    n.setInputCloud (mCloud);
    n.setSearchMethod (tree);
    n.setKSearch (K_SEARCH);
    n.compute (*normals);
    //* normals should not contain the point normals + surface curvatures

    // Copys an PointXYZRGB into an PointXYZ cloud.
    pcl::PointCloud<pcl::PointXYZ>::Ptr tempCloud;
    tempCloud->height = mCloud->height;
    tempCloud->width = mCloud->width;
    tempCloud->is_dense = mCloud->is_dense;

    pcl::PointCloud<pcl::PointXYZRGB>::iterator iter; // An iterator to traverse through the mCloud.

    // copys over the RGB cloud to a XYZ cloud. Needed for concatenateFields.
    for(iter = mCloud->begin(); iter != mCloud->end(); iter++)
    {
        pcl::PointXYZ tempPoint(iter->x, iter->y, iter->z);
        tempCloud->insert(tempCloud->end(), tempPoint);
    } // for int i = 0

    // Concatenate the XYZ and normal fields*
    pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::PointNormal>);
    pcl::concatenateFields(*tempCloud, *normals, *cloud_with_normals);
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
    gp3.setMaximumAngle(2 * M_PI / 3);                      // 120 degrees
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

bool PclMap::VisualizeMesh()
{
    //pcl::PolygonMesh mesh = meshToDraw;
    //pcl::io::loadPolygonFileVTK(fileName,meshToDraw); // loads mesh as .vtk

    pcl::visualization::PCLVisualizer visualizer ("mesh visualizer", true); // creates new visualizer "mesh visualizer" with create interactor = true
    visualizer.addPolygonMesh(mMesh, "polygon_mesh", 0); // adds a mesh-cloud to visualizer.

    visualizer.spin(); // updates the render. see spinOnce() for one time only rendering
} // VisualizeMesh

bool PclMap::SaveCloud(std::string fileName)
{
    mMutex1.lock_shared();
    if (pcl::io::savePCDFileASCII (fileName, *mCloud) == -1)
    {
        std::cerr << "Error: Could not save file: " << fileName << std::endl;
        return false;
    } // if

    std::cout << "Saved " << mCloud->points.size () << " data points to" << fileName << std::endl;

    mMutex1.unlock_shared();
    return true;
} // SaveMap

bool PclMap::LoadCloud(std::string fileName)
{
    mMutex1.lock();
    mCloud->clear();
    if (pcl::io::loadPCDFile<pcl::PointXYZRGB> (fileName, *mCloud) == -1)
    {
      std::cerr << "Error: Cound not read file: " << fileName << std::endl;
      return false;
    }

    std::cout << "Loaded " << mCloud->width * mCloud->height
                << " data points from " << fileName << std::endl;

    mMutex1.unlock();
    return true;
} // LoadMap

bool PclMap::LoadCloudNew(std::string fileName)
{
    mMutex1.lock();
    mIncomingLaserData->clear();
    if (pcl::io::loadPCDFile<pcl::PointXYZRGB> (fileName, *mIncomingLaserData) == -1)
    {
      std::cerr << "Error: Cound not read file: " << fileName << std::endl;
      return false;
    }

    std::cout << "Loaded " << mIncomingLaserData->width * mIncomingLaserData->height
                << " data points from " << fileName << std::endl;

    mMutex1.unlock();
    return true;
} // LoadCloudNew

pcl::PointCloud<pcl::PointXYZRGB>::Ptr PclMap::GetCloud(int option)
{
    if (option)
        return mIncomingLaserData;
    return mCloud;
} // getCloud

// bool PclMap::CollideSphere
    // Returns: number of points that collided.
int PclMap::CollideSphere(pcl::PointXYZ center, double radius)
{
    pcl::PointXYZRGB tp; // temporary point used to convert a PointXYZ into PointXYZRGB
    tp.x = center.x;
    tp.y = center.y;
    tp.z = center.z;

    std::vector<int> indices;     // stores the returned indices from radiusSearch below.
    std::vector<float> distances; // stores the returned squared distances from radiusSearch.

    pcl::search::KdTree<pcl::PointXYZRGB> searcher; // KdTree search object.
    searcher.setInputCloud(mCloud);

    // Returns the number of points within the given sphere.
    // NOTE: radius is multiplied by 1.001, otherwise the search will miss points on the line of the sphere.
    return (searcher.radiusSearch(tp, radius * 1.001, indices, distances, 0));
} // CollideSphere

bool PclMap::CollideLine(pcl::PointXYZ point1, pcl::PointXYZ point2, double width)
{
    double deltaX = (point2.x - point1.x);
    double deltaY = (point2.y - point1.y);
    float length = sqrt(deltaX * deltaX + deltaY * deltaY);

    pcl::PointXYZRGB centerPoint; // the center of the rectangle
    centerPoint.x = ((point2.x + point1.x) / 2);
    centerPoint.y = ((point2.y + point1.y) / 2);

    std::vector<int> indices;     // Stores the returned indices from radiusSearch below.
    std::vector<float> distances; // Stores the returned squared distances from radiusSearch.

    // Half of the hypotenuse of the triangle create by opposite points of the rectangle.
    float searchRadius = (sqrt(length * length + width * width)) * (SEARCH_TOLERANCE / 2);

    pcl::search::KdTree<pcl::PointXYZRGB> searcher;
    searcher.setInputCloud(mCloud);
    searcher.radiusSearch(centerPoint, searchRadius, indices ,distances,0);

    for (int i = 0; i < indices.size(); i++)
    {
        // Calculates the perpendicular distance from the line segment to the given point found.
        if (abs((mCloud->points[indices[i]].x - point1.x) * (point2.y - point1.y) -
                (mCloud->points[indices[i]].y - point1.y) * (point2.x - point1.x) ) / length <= (width / 2) )
        {
            pcl::PointXYZ point1Perp; // first point for the perpendicular bisector of the original line.
            pcl::PointXYZ point2Perp; // second point for the perpendicular bisector of the original line.

            // slope in the slope-line equations y = mx + b, where the m is the negitive reciprocle for perpendicular points
            double perp_b = (deltaX / deltaY) * centerPoint.x + centerPoint.y;

            // If deltaY != 0, then the point is not horizontal
            if (deltaY)
            {
                point1Perp.x = point1.x;
                point1Perp.y = (-deltaX / deltaY) * point1Perp.x + perp_b;
                point2Perp.x = point2.x;
                point2Perp.y = (-deltaX / deltaY) * point2Perp.x + perp_b;
            } // if
            // If deltaY is equal to zero, the perpendicular bisector is a verticle line.
            else
            {
                point1Perp.x = centerPoint.x;
                point1Perp.y = (centerPoint.y + (width / 2) );
                point2Perp.x = centerPoint.x;
                point2Perp.y = (centerPoint.y - (width / 2) );
            } // else

            //
            if (abs((mCloud->points[indices[i]].x - point1Perp.x) * (point2Perp.y - point1Perp.y) -
                    (mCloud->points[indices[i]].y - point1Perp.y) * (point2Perp.x - point1Perp.x) ) / length <= (length / 2) )
                return true;
        } // if
    } // for loop

    return false;
} // CollideLine

void PclMap::ProcessLaserData(const CxUtils::Point3D::List& scan, const CxUtils::Time& timestamp, const unsigned int id)
{
    mMutex1.lock();
    mIncomingLaserData->clear();
    CxUtils::Point3D::List::const_iterator iter;
    for(iter=scan.begin(); iter!=scan.end(); iter++)
    {
        pcl::PointXYZRGB point;

        if (iter->mX < MAX_LASER_RANGE)
        {
            point.x = iter->mX * cos(iter->mZ) * cos(iter->mY);
            point.y = iter->mX * sin(iter->mZ) * cos(iter->mY);
            point.z = 0;
            //point.z = iter->mX * sin(iter->mY);
            //point.z = tan(CxUtils::CxToRadians()) * iter->mX;
            point.rgb = INITIAL_POINT_LIFE;
            mIncomingLaserData->insert(mIncomingLaserData->end(),point);
        } // if

    } // for
    mMutex1.unlock();

} // ProcessLaserData

void PclMap::MakeRegistrationThread()
{
    mRegistrationThread.CreateThread(PclMap::Registration, this);

} // MakeRegistrationThread

void PclMap::Registration(void * ptr)
{
    PclMap * map = (PclMap *) ptr;

    while (!map->mRegistrationThread.QuitThreadFlag())
    {
        // If mCloud has no points, then mCloud is just mIncomingLaserData.
        if (map->mCloud->size() == 0)
        {
            map->mMutex1.lock();
            *map->mCloud = *map->mIncomingLaserData;
            map->mMutex1.unlock();
        } // if
        else
        { std::cout << "Hi!" << std::endl;
            pcl::PointCloud<pcl::PointXYZRGB>::Ptr copy = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>());
            // Lock cloud to copy data.
            map->mMutex1.lock();
            *copy = *map->mIncomingLaserData;
            map->mMutex1.unlock();

            pcl::IterativeClosestPoint<pcl::PointXYZRGB, pcl::PointXYZRGB> icp;
            //icp.registerVisualizationCallback(boost::bind(&PclMap::update))
            icp.setInputCloud(copy);
            icp.setInputTarget(map->mCloud);
            //icp.setTransformationEpsilon(.01);
            icp.setMaximumIterations(MAX_ICP_ITERATIONS); // Maximum amount iterations the internal optimizer should run for.

            map->mAlignedCloud->clear();
            icp.align(*map->mAlignedCloud);

            // If the clouds have converged, combines mIncomingLaserData with the current mCloud.
            if (icp.hasConverged())
            {
                map->mMutex2.lock();
                *map->mCloud = *map->mAlignedCloud;
                map->mMutex2.unlock();
                map->mCloudConvergence = 0;

                if (*map->mAlignedCloud->header.stamp > *map->mCloud->header.stamp)
                        *map->mCloud->header.stamp = *map->mAlignedCloud->header.stamp;

                      size_t nr_points = map->mCloud->points.size();
                      map->mCloud->points.resize (nr_points + map->mIncomingLaserData->.points.size() );
                      for (size_t i = nr_points; i < map->mCloud->points.size(); ++i)
                        map->mCloud->points[i] = rhs.points[i - nr_points];

                      map->mCloud->width = static_cast<uint32_t>(map->mCloud->points.size() );
                      map->mCloud->height = 1;
                      if (map->mIncomingLaserData->is_dense && map->mCloud->is_dense)
                      map->mCloud->is_dense = true;
                      else
                        map->mCloud->is_dense = false;

                // Updates the current position.
                //const Eigen::Matrix4f icpTransformation = icp.getFinalTransformation(); // Retrieves the transformation.

//                std::cout << "Transformation = " << std::endl;
//                for (int i = 0; i < 4; i++)
//                {
//                    for(int j = 0; j < 4; j++)
//                        std::cout << icpTransformation(i,j) << " ";
//                std::cout << std::endl;
//                }

//                Eigen::Matrix<float, 4, 1> currentPostionMatrix;
//                currentPostionMatrix << map->mCurrentPosition.x, map->mCurrentPosition.y, map->mCurrentPosition.z, 1; // Adds values into matrix

//                std::cout << "Pre-Matrix = " << currentPostionMatrix[0] << ", " << currentPostionMatrix[1] <<
//                             ", " << currentPostionMatrix[2] << ", " << currentPostionMatrix[3]<< std::endl;
//                currentPostionMatrix = icp. * currentPostionMatrix;
//                map->mCurrentPosition.x /= currentPostionMatrix[0,3];
//                map->mCurrentPosition.y /= currentPostionMatrix[0,3];
//                map->mCurrentPosition.z /= currentPostionMatrix[0,3];
//                std::cout << "Post-Matrix = " << map->mCurrentPosition.x << ", " <<
//                             map->mCurrentPosition.y << ", " << map->mCurrentPosition.z <<
//                             ", " << currentPostionMatrix[3] << std::endl << std::endl;

            } // if
            // Else the clouds do not converage, after 5 back-to-back failures erases the cloud.
//            else
//            {
//                if (map->mCloudConvergence != 5) map->mCloudConvergence++;
//                else
//                {
//                    map->mCloud = map->mIncomingLaserData;
//                    map->mCloudConvergence = 0;
//                } // else

//            } // else

        } // else

//        pcl::PointCloud<pcl::PointXYZRGB>::iterator iter; // An iterator to traverse through the mCloud.

//        // Removes points within the cloud as they age and are never refreshed.
//        for(iter = map->mCloud->begin(); iter != map->mCloud->end(); iter++)
//        {
//            iter->rgb--; // Decrements the life of the point.

//            if (iter->rgb == 0) map->mCloud->erase(iter); // Removes point if it has NOT been refreshed.

//        } // for iter

        boost::this_thread::sleep(boost::posix_time::milliseconds(REGISTRATION_SLEEPTIME));
    } // while

} // Registration

/*End of File*/
