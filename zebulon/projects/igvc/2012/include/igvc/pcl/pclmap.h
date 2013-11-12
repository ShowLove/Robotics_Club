//////////////////////////////////////////////////////////////////////////////////////
///
/// \file pclmap.h
/// \brief Header for PCL.
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

#ifndef PCLMAP_H
#define PCLMAP_H

#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <boost/math/constants/constants.hpp>
#include <cxutils/cxutils.h>

// Dynamixel


// Threading Dependencies
#include <boost/thread.hpp>

// Laser Dependencies
#include <math.h>
#include <laser/laser.h>

// Save/Load Dependencies
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
//#include <pcl/io/vtk_io.h>
//#include <pcl/io/vtk_lib_io.h>

// Triangulate Dependencies
//#include <pcl/point_types.h>
//#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
//#include <pcl/io/vtk_io.h>

// Search Dependencies
#include <pcl/search/search.h>
#include <pcl/search/pcl_search.h>

// Registration Dependencies
#include <pcl/registration/icp.h>

// Visualization Dependencies
//#include <boost/thread/thread.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>


// DEFINITIONS
#define INITIAL_POINT_LIFE 100 // Determines how long before a point is removed from the cloud without being refreshed. NOTE: DO NOT PUT IT LESS THAN 2
#define SEARCH_TOLERANCE 1.1 // Tolerance for the search radius of CollideLine.
#define MAX_LASER_RANGE 30 // Maximum distance (m) of a point that should be input into mCloud.
#define MAX_ICP_ITERATIONS 100 // Number of maximum internal iterations undergone by the ICP. No idea really.
#define VISUALIZE_SLEEPTIME 400 // The amount of time (ms) for which the Visualize thread should sleep.
#define REGISTRATION_SLEEPTIME 100 // The amount of time (ms) for which the Registration thread should sleep.

namespace Zebulon
{
    namespace IGVC
    {
        namespace PCL
        {
            class PclMap : public Zebulon::Laser::Callback
            {
            public:
                // Default constructor
                PclMap();

                // Initialize: Initializes variables.
                void Initialize();

                // Initializes: Same as Initialize(), but will initilize the lidar given a laser pointer.
                void Initialize(Zebulon::Laser::GenericLaser* laser);

                // MakeVisualCloudThread: Creates the thread using CxUtils for VisualizeCloud.
                void MakeVisualizeCloudThread();

                // VisualizeCloud: Creates a window to visualize mCloud.
                static void VisualizeCloud(void * ptr);

                // Addpoint: Adds a PointXYZRGB point to the cloud.
                void AddPoint(pcl::PointXYZRGB point);

                // AddPoint: Adds a point to mCloud given an x,y,z
                void AddPoint(float x,
                              float y,
                              float z);

                // TriangulateCloud: Triangulates cloud data. Creates surfaces by drawing triangles between points.
                bool TriangulateCloud();

                // VisualMesh: Can only be called after TriangulateCloud. Visualizes a mesh (points with surfaces).
                bool VisualizeMesh();

                // MakeRegistrationThread: Creates the thread used for Registration.
                void MakeRegistrationThread();

                // Registration: Compares two clouds and determines if they fit together. If the clouds converges, merges them.
                static void Registration(void *ptr);

                // SaveCloud: Saves mCloud as the given fileName.
                bool SaveCloud(std::string fileName);

                // LoadCloud: Loads a .pcd file into mCloud.
                bool LoadCloud(std::string fileName);

                // LoadCloudNew: Loads .pcd file into mIncomingLaserData.
                bool LoadCloudNew(std::string fileName);

                // GetCloud: Returns mCloud by default. Returns mIncomingLaser data if option != 0.
                pcl::PointCloud<pcl::PointXYZRGB>::Ptr GetCloud(int option = 0);

                // CollideSphere: Checks to see if there are any points within mCloud that are within the given radius
                //                from the given center of a sphere.
                virtual int CollideSphere(pcl::PointXYZ center,
                                          double radius);

                // CollideLine: Given the two end-points and the width of a given line, detemines if any points within
                //              mCloud are within the given width.
                virtual bool CollideLine(pcl::PointXYZ point1,
                                         pcl::PointXYZ point2,
                                         double width);

                // ProcessLaserData: Processes the laser data from lidar.
                void ProcessLaserData(const CxUtils::Point3D::List& scan,
                                      const CxUtils::Time& timestamp,
                                      const unsigned int id = 0);

            protected:
                // mCloud: The accumulated clouds.
                pcl::PointCloud<pcl::PointXYZRGB>::Ptr mCloud;

                // mIncomingLaserData: Stores the new laser data.
                pcl::PointCloud<pcl::PointXYZRGB>::Ptr mIncomingLaserData;

                // mAlignedCloud: Stores the result of aligned mCloud with mIncomingLaserData.
                pcl::PointCloud<pcl::PointXYZRGB>::Ptr mAlignedCloud;

                // mVisualThread: The thread for Visualize.
                CxUtils::Thread mVisualizeThread;

                // mRegistrationThread: The thread for Registraion.
                CxUtils::Thread mRegistrationThread;

                // mCloudConvergence: Tracks the number of times the cloud has not been converged.
                int mCloudConvergence;

                // mCurrentPosition: The robot's estimated current position within the cloud.
                pcl::PointXYZ mCurrentPosition;

                // mMutex1: A mutex.
                boost::shared_mutex mMutex1;

                // mMutex2: A mutex.
                boost::shared_mutex mMutex2;

                // mMesh: Stores the mesh, the point-cloud with surfaces.
                pcl::PolygonMesh mMesh;

                // ViewerPsycho:
                static void ViewerPsycho(pcl::visualization::PCLVisualizer& viewer);

                // ViewOneOff:
                static void ViewerOneOff(pcl::visualization::PCLVisualizer& viewer);

            }; // PclMap

        } // PclMap
    } // IGVC
} // Zebulon

#endif

/*End of File*/
