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

//#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <boost/math/constants/constants.hpp>
#include <cxutils/cxutils.h>
#include <pcl/impl/point_types.hpp>

// Baseline

// Dynamixel
#include <dynamixel/dynamixel.h>

// Threading Dependencies
#include <boost/thread.hpp>

// Laser Dependencies
#include <math.h>
#include <laser/laser.h>
#include <laser/laserfactory.h>

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
#include <pcl/registration/elch.h>
#include <pcl/registration/correspondence_estimation.h>

// Visualization Dependencies
//#include <boost/thread/thread.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>

// DEFINITIONS
#define CLOUD_EXPIRE_TIME        3   // If the cloud has NOT been updated within this time period (in seconds) the cloud has gone bad.
#define DYNAMIXEL_ANGLE_OFFSET 126   // Offset of the Dynamixel
#define DYNAMIXEL_SERVO_ID       1   // The Dynamixel's ID
#define DYNAMIXEL_UPDATE_RATE   10   // The update rate (Hz) of the Dynamixel
#define MAX_LASER_RANGE         10   // Maximum distance (m) of a point that should be input into mCloud.
#define MAX_ICP_ITERATIONS      50   // Number of maximum internal iterations undergone by the ICP. No idea really.
#define MAX_DYNAMIXEL_ANGLE    108   // The max angle the Dynamixel should be set to. Any further and damage may occur.
#define MIN_DYNAMIXEL_ANGLE    150   // The min angle the Dynamixel should be set to. Any further and damage may occur.
#define REGISTRATION_SLEEPTIME 100   // The amount of time (ms) for which the Registration thread should sleep.
#define SEARCH_TOLERANCE       1.1   // Tolerance for the search radius of CollideLine.
#define VISUALIZE_SLEEPTIME    200   // The amount of time (ms) for which the Visualize thread should sleep.

namespace Zebulon
{
    namespace IGVC
    {
        namespace PCL
        {
            class PclMap : public Zebulon::Laser::Callback, public Zebulon::Motor::Callback
            {
            public:

                // Default constructor
                PclMap();

                // Initialize: Initializes variables.
                void Initialize();

                // Initializes: Same as Initialize(), but will initilize the lidar given a laser pointer.
                void Initialize(Zebulon::Laser::GenericLaser* laser);

                void Initialize(Zebulon::Laser::GenericLaser* laser, Zebulon::Motor::Dynamixel* dynamixel);

                // Addpoint: Adds a PointXYZRGB point to the cloud.
                void AddPoint(pcl::PointXYZRGB point);

                // AddPoint: Adds a point to mCloud given an x,y,z
                void AddPoint(float x,
                              float y,
                              float z,
                              float rgb = 0);

                // CloudStruct: Will hold a cloud and it's other attributes
                struct CloudStruct
                {
                    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud;   // The Cloud.
                    double angle;                                   // Angle in degrees of the Dynamixel when the cloud data was taken.
                    time_t lastUpdated;                             // Time this cloud was last updated.
                    unsigned short sweep;
                };

                // LoadCloud: Loads a .pcd file into mCloud.
                bool LoadMcloud(std::string fileName);

                // LoadmCloud: Loads a PointCloud into mCloud.
                bool LoadMcloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud);

                // LoadCloudNew: Loads .pcd file into mIncomingLaserData.
                bool LoadIncomingCloudData(std::string fileName);

                // MakeOscillateDynamixelThread:
                void MakeOscillateDynamixelThread();

                // MakeRegistrationThread: Creates the thread used for Registration.
                void MakeRegistrationThread();

                // MakeVisualCloudThread: Creates the thread for VisualizeMcloud.
                void MakeVisualizeMcloudThread();

                void MakeVisualizeRawDataThread();

                // GetLaserAngle: Returns the current angle held by mCurrentAngle.
                double GetLaserAngle();

                // GetCloud: Returns mCloud by default. Returns mIncomingLaser data if option != 0.
                pcl::PointCloud<pcl::PointXYZRGB>::Ptr GetCloud(int option = 0);

                void ClearCloudVector();

                // Returns the number of planes within the CloudVector.
                unsigned int getCloudVectorSize();

                // CollideSphere: Checks to see if there are any points within mCloud that are within the given radius
                //                from the given center of a sphere.
                virtual int CollideSphere(pcl::PointXYZ center,
                                          double radius);

                // CollideLine: Given the two end-points and the width of a given line, detemines if any points within
                //              mCloud are within the given width.
                virtual bool CollideLine(pcl::PointXYZ point1,
                                         pcl::PointXYZ point2,
                                         double width);

                // Merges all cloud data to form a 3D cloud.
                void MergeRaw(bool clear = true);

                // ProcessLaserData: Processes the laser data from lidar.
                void ProcessLaserData(const CxUtils::Point3D::List& scan,
                                      const CxUtils::Time& timestamp,
                                      const unsigned int id = 0);

                void ProcessServoData(const double pos,
                                      const unsigned int id = 0);

                // SaveCloud: Saves mCloud as the given fileName.
                bool SaveMcloud(std::string fileName);

                // SetLaserAngle: Sets mCurrentAngle to angle.
                void SetLaserAngle(double angle);

                // ShutDown:
                bool ShutDown();

                // Slam:
                void Slam();

                struct TestLine
                {
                    pcl::PointXYZ point1;
                    pcl::PointXYZ point2;
                };

                // TriangulateCloud: Triangulates cloud data. Creates surfaces by drawing triangles between points.
                bool TriangulateCloud();

                void VisualizeCloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cPtr);

                // VisualMesh: Can only be called after TriangulateCloud. Visualizes a mesh (points with surfaces).
                bool VisualizeMesh();

                Zebulon::Motor::Dynamixel* mDynamixelController;

            protected:
                // Assigns the mCloudStructPtr to the appropriate CloudStruct based upon the incoming cloud's angle.
                void AssignPointers();

                // mCloud: The accumulated clouds.
                pcl::PointCloud<pcl::PointXYZRGB>::Ptr mCloud;
                pcl::PointCloud<pcl::PointXYZRGB>::Ptr mCloudOld;
                pcl::PointCloud<pcl::PointXYZRGB>::Ptr mRawData;
                pcl::PointCloud<pcl::PointXYZRGB>::Ptr mIncomingData;

                // mCloudPtr: The current cloud pointer.
                CloudStruct* mCloudStructPtr;

                // Vector to store the clouds of different angles.
                std::vector<CloudStruct> mCloudVector;

                // mMockAngle: Mock angle to be used to set hokuyo angle.
                double mCurrentAngle;

                // mCurrentPosition: The robot's estimated current position within the cloud.
                //pcl::PointXYZ mCurrentPosition;

                // mMesh: Stores the mesh, the point-cloud with surfaces.
                pcl::PolygonMesh mMesh;

                // mMutex1: A mutex.
                boost::shared_mutex mMutex1;

                // mNewLaserDataFlag: Flag to indicate there is new laser data to be processed.
                bool mNewLaserDataFlag;

                TestLine mTestLine;

                // mOscillateThread
                CxUtils::Thread mThread_Oscillate;

                // mRegistrationThread: The thread for Registraion.
                CxUtils::Thread mThread_Registration;

                // mVisualThread: The thread for Visualize.
                CxUtils::Thread mThread_Visualize;

                // mVisualThread: The thread for Visualize.
                CxUtils::Thread mThread_RawVisualize;

                static void OscillateDynamixelThread(void * ptr);

                // Registration: Compares two clouds and determines if they fit together. If the clouds converges, merges them.
                static void RegistrationThread(void *ptr);

                uint32_t SetIncomingPointColor(double distance);

                // VisualizeCloud: Creates a window to visualize mCloud.
                static void VisualizeMcloudThread(void * ptr);

                static void VisualizeRawDataThread(void * ptr);

            }; // PclMap
        } // PclMap
    } // IGVC
} // Zebulon

#endif

/*End of File*/
