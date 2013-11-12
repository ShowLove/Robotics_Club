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

#include <igvc/pcl/pclmap.h>
#include <laser/laser.h>
#include <laser/laserfactory.h>
#include <boost/thread.hpp>

using namespace Zebulon;
using namespace IGVC;
using namespace PCL;

int main()
{
    Laser::GenericLaser *laser = Laser::LaserFactory::CreateLaser(Laser::LaserFactory::Hokuyo);

    if(laser == NULL) return -1;

    laser->Initialize("/dev/ttyACM",115200);
    if(laser==NULL) return -1;
    std::cout << "Laser initialized" << std::endl;

    PclMap map;

    map.Initialize(laser);
    //map.Initialize();
    std::cout << "Map initialized" << std::endl;

    boost::this_thread::sleep(boost::posix_time::milliseconds(50));
    map.MakeVisualizeCloudThread();
    boost::this_thread::sleep(boost::posix_time::milliseconds(50));
    map.MakeRegistrationThread();

    /* -------------------------- START MAP COLLISION TESTING -------------------------- //

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

    while(true)
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(50));
    }

    return 0;
} // int main
