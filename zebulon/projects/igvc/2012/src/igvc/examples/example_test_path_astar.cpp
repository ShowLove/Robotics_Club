///////////////////////////////////////////////////////////////////////////////////////
/////
///// \file example_test_path.h
///// \brief A program to test cartographer maps and navigators that were created for
/////           IGVC, such as the QuadTreeMap.
/////
///// Author(s): Michael Scherer<br>
///// Created: 2011<br>
///// Copyright (c) 2010<br>
///// Robotics Laboratory and Club<br>
///// University of Central Florida (UCF) <br>
///// Email: michael.scherer@knights.ucf.edu <br>
///// Web: http://robotics.ucf.edu <br>
/////
/////  Redistribution and use in source and binary forms, with or without
/////  modification, are permitted provided that the following conditions are met:
/////      * Redistributions of source code must retain the above copyright
/////        notice, this list of conditions and the following disclaimer.
/////      * Redistributions in binary form must reproduce the above copyright
/////        notice, this list of conditions and the following disclaimer in the
/////        documentation and/or other materials provided with the distribution.
/////      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
/////        names of its contributors may be used to endorse or promote products
/////        derived from this software without specific prior written permission.
/////
/////  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
/////  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/////  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/////  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
/////  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
/////  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
/////  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
/////  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
/////  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/////  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/////
//////////////////////////////////////////////////////////////////////////////////////
#include "opencv/cv.h"

#include "cartographer/maps/worldmap.h"
#include "cartographer/objects/sphere.h"
#include "ground/path/quadtreemap.h"
#include "cartographer/planning/beamsearch.h"
#include "igvc/ground/path/astar.h"

#include "cxutils/keyboard.h"

using namespace Zebulon;
using namespace IGVC;

int DisplayImage(const std::string& windowName, IplImage *image);

int main(int argc, char** argv)
{
    //QuadTreeMap* map = new QuadTreeMap();
    Cartographer::WorldMap* map = new Cartographer::WorldMap();
    //Cartographer::WorldMap* map = new Cartographer::WorldMap();
    map->Create(1000);
    map->SetWorldOrigin(CxUtils::Wgs(0,0));

    AStar nav;
    nav.AddMap(map, "Default");
    nav.SetMapImageSizeMeters(100);
    nav.SetDrawResolution("Default", 8);
    nav.SetDrawPoseSize(.75, .25);

    /*
    double mPathWidth = .5;
    double mSearchDepth = 2;
    double mSegmentLengthMeters = 15;
    double mScansPerSweep = 15;
    double mAngleScannedRads = CxUtils::CX_TWO_PI;
    double mBranchesPerDepth = 5;
    double mAngleSweepIncrementRads = mAngleScannedRads/mScansPerSweep;
    double mEndpointThreshold = 0.5;

    Cartographer::Navigator::Settings *settings = new Cartographer::Navigator::Settings;
    settings->Set(Cartographer::BeamSearch::PathWidthMeters, mPathWidth);
    settings->Set(Cartographer::BeamSearch::SearchDepth, mSearchDepth);
    settings->Set(Cartographer::BeamSearch::BranchLengthMeters, mSegmentLengthMeters);
    settings->Set(Cartographer::BeamSearch::ScanAngleRads, mAngleScannedRads);
    settings->Set(Cartographer::BeamSearch::BranchesPerDepth, mBranchesPerDepth);
    settings->Set(Cartographer::BeamSearch::AngleBetweenBranchesRads, mAngleSweepIncrementRads);
    settings->Set(Cartographer::BeamSearch::EndpointThresholdMeters, mEndpointThreshold);
    nav.SetSettings(settings);
    */

    printf("Finished configuring, beginning object add sequence...\n");

    IplImage* img = cvCreateImage(cvSize(800, 800), 8, 3);

    CvFont font;
    double hScale=.5;
    double vScale=.5;
    int    lineWidth=1;
    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX, hScale,vScale,0,lineWidth);

    char buf[255];

    Cartographer::Pose myPose = map->CreatePose(CxUtils::Utm(CxUtils::Wgs(0, 0)), CxUtils::Point3D());
    Cartographer::Pose destPose = map->CreatePose(CxUtils::Utm(CxUtils::Wgs(0.0004, 0.0002)), CxUtils::Point3D());

    while (true)
    {
        for (int i=0; i<540; i++)
        {
            double x, y;
            //x = ((rand()%2)? -1 : 1)*(rand()%40+5);
            //y = ((rand()%2)? -1 : 1)*(rand()%40+5);
            if (rand()%2 == 1)
            {
                x = cos(i*CxUtils::PI/400.0)*20;
                y = sin(i*CxUtils::PI/400.0)*20;
            }
            else
            {
                x = cos(-i*CxUtils::PI/400.0)*40;
                y = sin(-i*CxUtils::PI/400.0)*40;
            }
            Cartographer::Object* obj = new Cartographer::Objects::Sphere(CxUtils::Point3D(x, y, 0), .25);
            obj->SetTimeToLive(10000);
            //printf("Object x:%lf\ty:%lf\n", obj->GetOrigin().mX, obj->GetOrigin().mY);
            map->AddObject(obj);
        }

        map->Update();

        Cartographer::Path path;
        if (!nav.GetPath(myPose, destPose, path))
        {
            myPose = map->CreatePose(CxUtils::Utm(CxUtils::Wgs(0, 0)), CxUtils::Point3D());
        }

        Cartographer::Path waypoints;
        waypoints.Push(destPose);

        nav.DrawLocalMap("Default", 100, NULL, &path, &myPose, &waypoints);
        //nav.DrawBlankMap("Default");
        DisplayImage("Default", nav.GetLocalMapImage());

        if (path.Size() > 1)
        {
            myPose = *(++path.Begin());
        }
        else
            destPose = Cartographer::Pose(CxUtils::Point3D(rand()%80-40, rand()%80-40, 0));
            //destPose = map->CreatePose(CxUtils::Utm(CxUtils::Wgs((rand()%80)*0.00001, (rand()%80)*0.00001)), CxUtils::Point3D());
            //myPose = map->CreatePose(CxUtils::Utm(CxUtils::Wgs(0,0)), CxUtils::Point3D());


        //map->Draw(img, 4, CxUtils::Point3D(-50,-50,0));

        Cartographer::Object::Collisions* collisions = NULL;//new Cartographer::Object::Collisions(10);
        if (map->ScanRadiusInXY(CxUtils::Point3D(), 10, collisions))
        {
            cvCircle(img, cvPoint(200, 200), 40, cvScalar(0,0,255));
        }
        else
        {
            cvCircle(img, cvPoint(200, 200), 40, cvScalar(0,255,0));
        }

        //cvPutText (img,map->ToString().c_str(),cvPoint(0,370), &font, cvScalar(0,255,255));
        if (collisions != NULL)
        {
            sprintf(buf, "Collisions: %d", (int)collisions->size());
            cvPutText (img, buf,cvPoint(0,390), &font, cvScalar(0,255,255));
        }


        //DisplayImage("Map", img);

        //printf(" [] Map ToString == %s\n", map->ToString().c_str());
        CxUtils::SleepMs(10);
    }

    //cvReleaseImage(img);
}

int DisplayImage(const std::string& windowName, IplImage *image)
{
    if(image==NULL)
    {
        return 0;
    }

    cvNamedWindow( windowName.c_str() , CV_WINDOW_AUTOSIZE);

    IplImage *smallImage = cvCreateImage(cvSize((int)(image->width),(int)(image->height)),
                                         image->depth, image->nChannels);
    cvResize(image,smallImage);
    cvShowImage(windowName.c_str(), smallImage);
    cvWaitKey(10);
    cvReleaseImage(&smallImage);
    return 1;
}
