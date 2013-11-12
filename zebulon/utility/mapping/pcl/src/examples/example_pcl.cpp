 //////////////////////////////////////////////////////////////////////////////////////
///
/// \file example_pcl.cpp
/// \brief Example for running PCL.
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
//////////////////////////////////////////////////////////////////////////////////////

#include <pcl/pclmap.h>
#include <laser/laser.h>
#include <laser/laserfactory.h>
#include <boost/thread.hpp>

using namespace Zebulon;
using namespace IGVC;
using namespace PCL;

int main()
{
//    Laser::GenericLaser *laser = Laser::LaserFactory::CreateLaser(Laser::LaserFactory::Hokuyo);

//    if(laser == NULL) return -1;

//    laser->Initialize("/dev/ttyACM",115200);
//    if(laser==NULL) return -1;
//    std::cout << "Laser initialized" << std::endl;

//    PclMap map;

//    map.Initialize(laser);
//    //map.Initialize();
//    std::cout << "Map initialized" << std::endl;

//    //map.MakeVisualizeCloudThread();
//    map.MakeRegistrationThread();

//    Zebulon::Motor::Dynamixel test;
//    if (!test.Initialize("dev/ttyUSB0",false,10.0) ) return -1;


// ------------------------------ START DYNAMIXEL TESTING ------------------------------ //

    Laser::GenericLaser *laser = Laser::LaserFactory::CreateLaser(Laser::LaserFactory::Hokuyo);
    if (laser == NULL) return -1;

    laser->Initialize("/dev/ttyACM0", 115200);
    if (laser == NULL) return -1;
    std::cout << "Laser initialized" << std::endl;
    laser->ClearCallbacks();

    Zebulon::Motor::Dynamixel* dyna = new Zebulon::Motor::Dynamixel();
    dyna->Initialize("/dev/ttyUSB0", false, 10.0);
    if (dyna == NULL) return -1;
    std::cout << "Dynamixel initialized" << std::endl;

    PclMap map;
    map.Initialize(laser, dyna);
    std::cout << "Map initialized" << std::endl;

    map.MakeOscillateDynamixelThread();
    std::cout << "Called OscillateDynamixelThread" << std::endl;
    map.MakeRegistrationThread();
    std::cout << "Called Registration" << std::endl;
    map.MakeVisualizeMcloudThread();
    std::cout << "Called Visualize" << std::endl;


    float testCoordinates[2][2] = {0};
    float width = 0;

    std::cin >> width;

//    while (testCoordinates[0][0] != 9 && testCoordinates[0][1] != 9)
//    {
//        std::cout << "Enter the first point to test." << std::endl;
//        scanf("%f %f", &testCoordinates[0][0], &testCoordinates[0][1]);

//        std::cout << "Enter the second point to test." << std::endl;
//        scanf("%f %f", &testCoordinates[1][0], &testCoordinates[1][1]);

//        std::cout << "Enter the width to test." << std::endl;
//        std::cin >> width;

//        pcl::PointXYZ temp1(testCoordinates[0][0], testCoordinates[0][1], 0);
//        pcl::PointXYZ temp2(testCoordinates[1][0], testCoordinates[1][1], 0);

//        std::cout << "CollideLine returned: " << map.CollideLine(temp1, temp2, width) << std::endl << std::endl;
//     } // while

//    while (scans < 30)
//    {
//        for (angle = MIN_DYNAMIXEL_ANGLE; angle > MAX_DYNAMIXEL_ANGLE; angle--)
//        {
//            dyna->SetServoDegrees(DYNAMIXEL_SERVO_ID, angle);
//            std::cout << "Laser Angle =" << map.GetLaserAngle() << std::endl;
//            map.MergeClouds();
//            boost::this_thread::sleep(boost::posix_time::milliseconds(20));
//        } // for
//        for (; angle < MIN_DYNAMIXEL_ANGLE; angle++)
//        {
//            dyna->SetServoDegrees(DYNAMIXEL_SERVO_ID, angle);
//            std::cout << "Laser Angle =" << map.GetLaserAngle() << std::endl;
//            map.MergeClouds();
//            boost::this_thread::sleep(boost::posix_time::milliseconds(20));
//        } // for

//        scans++;
//    } // while

    laser->Shutdown();
    dyna->Shutdown();
    map.ShutDown();

    //map.SavemCloud("mCloud1.pcd");

// ------------------------------- END DYNAMIXEL TESTING ------------------------------- */


/* -------------------------------- START MESH TESTING -------------------------------- //

    PclMap map;
    map.Initialize();
    std::cout << "Map Initialized" << std::endl;
    map.LoadmCloud("mCloud.pcd");
    std::cout << "Cloud loaded" << std::endl;

    map.TriangulateCloud();
    std::cout << "Finished Triangulating" << std::endl;
    map.VisualizeMesh();

// --------------------------------- END MESH TESTING --------------------------------- */


/* --------------------------- START MAP COLLISION TESTING  --------------------------- //

    PclMap map;
    map.Initialize();

    // Add points to cloud
    map.AddPoint(1,0,0); map.AddPoint(2,5,0);

    double testRadius;
    float testCoordinates1[2];
    float testCoordinates2[2];

    int input;

    std::cout << "1) Collide Sphere" << std::endl << "2) Collide Line" << std::endl;
    std::cin >> input;

    if (input == 1)
    {
        while (true)
        {
            std::cout << "Enter the 2D coordinates to test." << std::endl;
            scanf("%f %f",&testCoordinates1[0],&testCoordinates1[1]);

            std::cout << "Enter the radius to test." << std::endl;
            std::cin >> testRadius;

            pcl::PointXYZ temp1(testCoordinates1[0], testCoordinates1[1], 0);

            std::cout << "CollideSphere returned: " << map.CollideSphere(temp1, testRadius) << std::endl << std::endl;
        } // while
    } // if
    else
    {
        while (true)
        {
            std::cout << "Enter the first point to test." << std::endl;
            scanf("%f %f", &testCoordinates1[0], &testCoordinates1[1]);

            std::cout << "Enter the second point to test." << std::endl;
            scanf("%f %f", &testCoordinates2[0], &testCoordinates2[1]);

            std::cout << "Enter the width to test." << std::endl;
            std::cin >> testRadius;

            pcl::PointXYZ temp1(testCoordinates1[0], testCoordinates1[1], 0);
            pcl::PointXYZ temp2(testCoordinates2[0], testCoordinates2[1], 0);

            std::cout << "CollideLine returned: " << map.CollideLine(temp1,temp2,testRadius) << std::endl << std::endl;
        } // while
    } // else

// -------------------------- END MAP COLLISION TESTING -------------------------- */

//    double deletethis = 0;

//    while(angle != 45)
//    {
//        scanf("%lf", &deletethis);
//        map.SetLaserAngle(angle);
//        angle += 9;

//        std::cout << "Laser Angle = " << map.getLaserAngle() << std::cout;
//        boost::this_thread::sleep(boost::posix_time::milliseconds(50));
//    } // while

//    map.mergeClouds();
//    map.VisualizeCloud(map.mCloud);

    return 0;

} // int main
