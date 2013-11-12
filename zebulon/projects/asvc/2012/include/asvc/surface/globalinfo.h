////////////////////////////////////////////////////////////////////////////////////
///
/// \file globalinfo.h
/// \brief Class for getting all sensor data required for use
///        by GlobalCommand interface.
///
/// Author(s): Daniel Barber, David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu<br>
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
#ifndef __ZEBULON_SURFACE_GLOBAL_INFO__H
#define __ZEBULON_SURFACE_GLOBAL_INFO__H

#include <cxutils/math/coordinates.h>
#include <statemachine/globalinfo.h>
#include <statemachine/utility/objectscan.h>
#include <display/laserdisplay.h>
#include <cartographer/pose.h>
#include <cartographer/map.h>
#include <cvcapture/cvimagecapture.h>
#include <cvcapture/imagecapturepylon.h>
#include "asvc/baseline/microcontroller.h"

namespace Zebulon
{
    namespace ASVC
    {
        typedef CxUtils::Utm Utm;               ///<  Type definition for UTM data.
        typedef CxUtils::Wgs Wgs;               ///<  Type defintion for WGS data.
        typedef CxUtils::Point3D Orientation;   ///<  Robot orientation data.
        class DepthMap;                         ///<  Forward declaration.
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class GlobalInfo
        ///  \brief Interface class for collecting all information about the platform
        ///         that the Ground Vehicle AI will use for navigation and decision
        ///            making.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class GlobalInfo : public AI::GlobalInfo,
                           public Zebulon::Video::CvImageCapture::Callback
        {
        public:
            class SimplePose
            {
            public:
                //global coordinates
                SimplePose()
                {
                }

                SimplePose(const Utm& position, double yawrads)
                {
                    mPosition = position;
                    mYawRads = yawrads;
                }
                Cartographer::Segment3D Segment(double distMeters= 1)
                {
                    Cartographer::Segment3D seg;
                    seg.mWidth = .1;
                    seg.mPoint1.mX = mPosition.mEasting;
                    seg.mPoint1.mY = mPosition.mNorthing;
                    seg.mPoint2 = seg.mPoint1;
                    seg.mPoint2.mX +=distMeters;
                    seg.mPoint2 = seg.mPoint2.Rotate(seg.mPoint1,
                                                     CxUtils::Orientation::AddToAngle(mYawRads,-CxUtils::CX_HALF_PI),Cartographer::Point3D::Z);
                    return seg;
                }
                SimplePose Project(double distMeters = 1)
                {
                    //Cartographer::Segment3D seg = Segment(distMeters);
                    double angle=CxUtils::Orientation::AddToAngle(CxUtils::CX_HALF_PI,-mYawRads);
                    CxUtils::Point3D point(distMeters,0,0);
                    point = point.Rotate(CxUtils::Point3D(),angle,CxUtils::Point3D::Z);
                    SimplePose result(mPosition,mYawRads);
                    result.mPosition.mEasting += point.mX;
                    result.mPosition.mNorthing += point.mY;
                    return result;
                }

                SimplePose Reverse() const{return SimplePose(mPosition,CxUtils::Orientation::AddToAngle(mYawRads,CxUtils::CX_PI));}
                Utm mPosition;
                double mYawRads;

            };
            class SimpleTrail
            {
            public:
                SimpleTrail(const SimplePose& sp)
                {
                    mDistTraveledM = 0;
                    Append(sp);
                }
                SimpleTrail()
                {
                    mDistTraveledM = 0;
                }

                void Append(const SimplePose& sp, double minDistToPrevM=2)
                {
                    if(mTrail.size()>0)
                    {
                        double dist = CxUtils::Utm::Distance2D(mTrail.back().mPosition,sp.mPosition);
                        //waypoints too close together
                        if(dist < minDistToPrevM)
                        {
                            return;
                        }
                        mDistTraveledM += dist;
                    }
                    mTrail.push_back(sp);
                    mReverseTrail.insert(mReverseTrail.begin(),sp.Reverse());
                }
                //if 0 will default to dist between prev 2 points which make projection
                //else will project distM meters out.
                bool GuessNextPose(SimplePose& newpose, double distM = 0)
                {
                    if(mTrail.size()<2) return false;
//                    CxUtils::Utm projection;
//                    AI::Utility::CalcProject(mStartPosition.mEasting,
//                                             mStartPosition.mNorthing,
//                                             mStartHeading,mWaypointDistance*x,
//                                             projection.mEasting, projection.mNorthing);
//                    mWaypoints.push_back(projection);
                    SimplePose p1 = mTrail[mTrail.size()-2];
                    SimplePose p2 = mTrail[mTrail.size()-1];
                    //double aveheading = CxUtils::Orientation::AddToAngle(p1.mYawRads,p2.mYawRads)/2.0;
                    CxUtils::Point3D point1(p1.mPosition.mEasting,p1.mPosition.mNorthing);
                    CxUtils::Point3D point2(p2.mPosition.mEasting,p2.mPosition.mNorthing);

                    double distance = CxUtils::Utm::Distance2D(p1.mPosition,p2.mPosition);
                    if(distM > 0)
                    {
                        distance = distM;
                    }
                    CxUtils::Segment3D seg(point1,point2);

                    double angle = seg.GetAngleXY();
//                    double angle = (p2.mYawRads+p1.mYawRads)/2.0;
//                    angle = CxUtils::Orientation::AddToAngle(angle,-CxUtils::CX_HALF_PI);
//                    angle += seg.GetAngleXY();
//                    angle /= 2.0;

                    CxUtils::Point3D newpoint = point2;
                    newpoint.mX += distance*cos(angle);
                    newpoint.mY += distance*sin(angle);

                    newpose.mPosition.mEasting = newpoint.mX;
                    newpose.mPosition.mNorthing = newpoint.mY;
                    newpose.mPosition.mElevation = 0;
                    newpose.mYawRads = angle;
                    return true;
                }

                double mDistTraveledM;
                std::vector<SimplePose> mTrail;
                std::vector<SimplePose> mReverseTrail;
            };


            typedef Microcontroller::PortNumber PinNumber; ///<  Refernce to pins on microcontroller.
            /**  Stores constants for pin names for microcontroller */
            static const std::string VehicleWidth;  ///<  Label name for vehicle width.
            static const std::string VehicleLength; ///<  Label name for vehicle length.
            static const std::string MainCamera;    ///<  Label name for camera sensor.
            static const std::string BackupCamera;  ///<  Label name for additional secondary camera.
            static const std::string DepthDataMainCamera;     ///<  Label name for depth image from main camera.
            static const std::string DepthImageMainCamera;    ///<  Label name for depth image from main camera.
            static const std::string TiltCamera;              ///<  Label name for tilt camera.
            static const std::string Laser;                   ///<  Label name for laser sensor.
            static const std::string StartPosition;           ///<  Label name for starting position of mission (when start button pressed).
            static const std::string ChannelStartPosition;    ///<  Label name for position where channel starts.
            static const std::string ChannelStartHeading;     ///<  Label name for heading when we started channel (radians).
            static const std::string ChannelEndPosition;      ///<  Label name for position where buoys/channel ends.
            static const std::string ChannelEndHeading;       ///<  Heading we were facing when we left the buoys/channel (radians).
            static const unsigned int LaserMapID = 0;         ///<  ID for laser data when adding to maps.
            static const unsigned int CameraMapID = 1;        ///<  ID for camera data when adding to maps.

            GlobalInfo();
            virtual ~GlobalInfo();
            // Optional method to load settings from an XML file.
            virtual bool LoadSettings(const std::string& xmlFilename="");
            // Method called to verify that a minimum number of capabilities have been
            // initialized.
            virtual bool VerifyInitialization();
            // This method must be overloaded to add support for new message types you are subscribing to!
            virtual JAUS::Message* CreateMessage(const JAUS::UShort jausMessageCode);
            // Processes messages created by the sychronize service.
            virtual void Receive(const JAUS::Message* jausMessage);
            // Process any video data as it arrives.
            virtual void ProcessCameraData(Zebulon::Video::CvImageCapture::Image* dest);
            // Creates a subscription to the video subscriber service.
            virtual bool CreateSubscriptions();
            // This method is called periodically, overload to perform periodic checks, events, etc.
            virtual void CheckServiceStatus(const unsigned int timeSinceLastUpdateMs);
            // Gets UTM position of robot.
            Utm GetPosition(const bool setElevationToZero = true) const;
            // Gets world position (lat/long) values.
            Wgs GetPositionWgs(const bool setElevationToZero = true) const;
            // Gets the orientation of robot.
            Orientation GetOrientation(const bool degrees = false) const;
            // Gets the velocity of the robot forward/reverse
            double GetVelocityX() const;
            // Gets the velocity of the robot laterally
            double GetVelocityY() const;
            // Gets the rotation rate of the robot
            double GetRotationRateZ(const bool degrees = false) const;
            // Gets the current pose.
            Cartographer::Pose GetPose(const Cartographer::Map* map, const bool noVelocities = false) const;
            // Method to push a bread crumb trail of points the vehicle has travelled to
            // store.  Data is not logged.
            void PushCurrentPositionToTrail();
            Utm::List GetPositionTrail(const bool reversed = false) const;
            void AddToSimpleTrail(const std::string& trailname, double minDistToPrevM = 2);
            bool GetSimpleTrail(const std::string& trailname,SimpleTrail& trail);
            //bool GetFrame(cv::Mat &value) const;
            //bool GetFrame(IplImage *&value) const;
            virtual bool GetMicroInfo(const Microcontroller::PortNumber port, double& result) const;
            virtual bool GetMicroInfo(const Microcontroller::PortNumber port, int& result) const;
            virtual bool GetMicroInfo(const Microcontroller::PortNumber port, bool& result) const;
            // Sets the pointer to the pylon interface for Basler camera (if enabled).
            void SetPylonCamera(Video::ImageCapturePylon* camera) { mpPylonCamera = camera; }
            Video::ImageCapturePylon* GetPylonCamera() { return mpPylonCamera; }
            void SetDepthMap(DepthMap* dmap) { mpDepthMap = dmap; }
            DepthMap* GetDepthMap() { return mpDepthMap; }
        protected:
            void AddToSimpleTrail(std::string trailname, const Utm& utm, const double yawrads, double minDistToPrevM = 2);
            bool mDebugLaserFlag;                ///<  Debug laser data flag.
            Utility::LaserDisplay mLaserDisplay; ///<  Used to debug laser output.
            Utility::LaserDisplay mObjectDisplay;///<  Used to debug object scan output.
            AI::ObjectScan  mObjectScan;         ///<  Object scan (debugging).
            CxUtils::Mutex mTrailMutex;          ///<  Mutex for thread protection of position trail.
            Utm::List mPositionTrail;            ///<  Trail of waypoints we've visited.
            Video::ImageCapturePylon* mpPylonCamera;    ///<  Pointer to pylon camera interface.
            IplImage* mpConvertedFrame;                 ///<  Converted image frame.
            std::map<std::string,SimpleTrail> mSimpleTrails;
            DepthMap* mpDepthMap;                       ///<  Pointer to depth map.
        };

    }
}

 #endif ///__ZEBULON_SURFACE_GLOBAL_INFO__H
