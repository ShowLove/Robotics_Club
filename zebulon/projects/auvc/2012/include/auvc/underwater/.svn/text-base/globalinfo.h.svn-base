/////////////////////////////////////////////////////////////////////////////
///
/// \file globalinfo.h
/// \brief GlobalInfo class for the the underwater vehicle.
///
/// Author(s): Cassondra Puklavage<br>
/// Created: 04/08/2010<br>
/// Copyright (c) 2008<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
/// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
/// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
/// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
/// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
/// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
/// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
/// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
/// OF SUCH DAMAGE.
///
/////////////////////////////////////////////////////////////////////////////
#ifndef __ZEBULON_AUVC_GLOBAL_INFO__H
#define __ZEBULON_AUVC_GLOBAL_INFO__H

//New JAUS
/*#include <jaus/core/component.h>
#include <cxutils/cxutils.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>*/
////#include <jaus/mobility/sensors/queryglobalpose.h>
////#include <jaus/mobility/sensors/reportglobalpose.h>

//#include <jaus/extras/video/videosubscriber.h>
#include <jaus/mobility/sensors/globalposesensor.h>
#include <jaus/mobility/sensors/velocitystatesensor.h>
//#include <jaus/extras/extrascodes.h>
//#include <jaus/extras/mcu/querymicrocontrollerstate.h>
//#include <jaus/extras/mcu/reportmicrocontrollerstate.h>
#include <jaus/extras/mcu/microcontroller.h>
//#include <jaus/core/events/events.h>
//#include <jaus/core/component.h>
#include "statemachine/globalinfo.h"
//#include "filter/filter.h"
//#include "filter/dcfilter.h"

#include <fw/fw1394Camera.h>
#include <cvcapture/cvimagecapture.h>


#define CAMERALOG

namespace Zebulon
{
    namespace Underwater
    {
        typedef CxUtils::Point3D Orientation;   ///<  Robot orientation data.
        
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class GlobalInfo
        ///  \brief Interface class for getting information from GlobalPoseSensor.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class GlobalInfo : public AI::GlobalInfo
        {
            public:
                /** Callback interface for sensors to get data. */
                class VideoCallback : public Zebulon::Video::CvImageCapture::Callback
                {
                    GlobalInfo * mpGlobalInfo;
                    IplImage * mpConvertedFrame;
                    IplImage * mpResizedImg;
                public:
                    VideoCallback(GlobalInfo * info)
                    {
                        mpGlobalInfo = info;
                        //Assumes both cameras the same size?
                        mpConvertedFrame=NULL;
                        mpResizedImg=NULL;
                    }

                    void SetReSizeInfo(bool resizeflag, int width, int height)
                    {
                        printf("Resize: %d %d %d\n",(int)resizeflag,width,height);

                        mReSizeFlag = resizeflag;
                        mReSizeWidth = width;
                        mReSizeHeight = height;
                    }

                    virtual void ProcessCameraData(Zebulon::Video::CvImageCapture::Image* dest)
                    {
                        Video::CvConvertToIplImage(*dest, &mpConvertedFrame);

                        if(dest->mID==0)
                        {
                            //hack to mask out crap in downward frame
                            cvDrawRect(mpConvertedFrame,cvPoint(0,0),
                                       cvPoint(mpConvertedFrame->width,50),
                                       CV_RGB(0,0,0),-1);

                        }
                        if(mReSizeFlag)
                        {
                            if(mpResizedImg==NULL)
                            {
                                mpResizedImg = cvCreateImage(cvSize(mReSizeWidth,mReSizeHeight), mpConvertedFrame->depth, mpConvertedFrame->nChannels);
                            }
                            cvResize(mpConvertedFrame, mpResizedImg);
                        }

                        if(dest->mID==1)
                        {
                            if(mReSizeFlag)
                            {
                                mpGlobalInfo->SetInfo(ForwardCamera, mpResizedImg);
                            }
                            else
                            {
                                mpGlobalInfo->SetInfo(ForwardCamera, mpConvertedFrame);
                            }
                        }
                        if(dest->mID==0)
                        {
                            if(mReSizeFlag)
                            {
                                mpGlobalInfo->SetInfo(DownwardCamera, mpResizedImg);
                            }
                            else
                            {
                                mpGlobalInfo->SetInfo(DownwardCamera, mpConvertedFrame);
                            }
                        }
                    }
                    bool mReSizeFlag;
                    int mReSizeWidth;
                    int mReSizeHeight;


                };

                //static const std::string VehicleWidth;  ///<  Label name for vehicle width.
                //static const std::string VehicleLength; ///<  Label name for vehicle length.
                static const std::string ForwardCamera;     ///<  Label name for the forward camera's frame
                static const std::string DownwardCamera;    ///<  Label name for the downward camera's frame
                static const std::string Laser;             ///<  Label name for the LIDAR data
                static const std::string DepthSensor;       ///<  Label name for the depth sensor information
                
                static const std::string StartButton;       ///<  Label name for hte start button state
                static const std::string Started;           ///<  Label name for the info dictating whether the start button has been pressed
                static const std::string StartYaw;          ///<  Label name for the initial yaw of the vehicle
                static const std::string FixedYaw;          ///<  Label name for the current yaw for the buoy mission
                static const std::string PathYaw;           ///<  Label name for the last path yaw recorded
                static const std::string LastFixedYaw;
                static const std::string PingerDriveTime;

                static const std::string ExitEarlyToFinalMission;   ///< Label name for the flag dictating whether to skip to the final mission
                
                static const std::string PingerYaw;         ///<  Label name for the heading of the pinger
                static const std::string PingerUpdateTime;  ///<  Label name for the time of the last ping heard
                static const std::string PingerDistance;    ///<  Label name for distance from the pinger
                static const std::string PingerSelected;    ///<  Label name for the current selected pinger
                static const std::string PingerGain;        ///<  Label name for the gain of he hydrophones (used for distance)
                
                static const std::string EStop;             ///<  Label name for the estop state
                
                GlobalInfo();
                virtual ~GlobalInfo();
                // Optional method to load settings from an XML file.
                virtual bool LoadSettings(const std::string& xmlFilename="");
                // Method called to verify that a minimum number of capabilities have been
                // initialized.
                virtual bool VerifyInitialization();
                // This method must be overloaded to add support for new message types you are subscribing to!
                virtual JAUS::Message* CreateMessage(const JAUS::UShort jausMessageCode);
                // This method must be overloaded to add support for processing new messages you want.
                virtual void Receive(const JAUS::Message* jausMessage);             
                // Overload this method, and insert code to create subscriptions for the data you need
                virtual bool CreateSubscriptions();
                // This method is called periodically, overload to perform periodic checks, events, etc.
                virtual void CheckServiceStatus(const unsigned int timeSinceLastUpdateMs);// { CreateSubscriptions(); }
                // Gets the orientation of robot.
                Orientation GetOrientation(const bool degrees = false) const;
                // Gets the rotation rate of the robot
                //double GetRotationRateZ(const bool degrees = false) const;


                //Probably good
                //void PrintStatus() const;
                //std::string GetLogData() const;
                int mDepthFlag;
                int mHaveStartYaw;

                VideoCallback *mCameraCallback;

                fw1394Camera *mpForwardCameraRaw;
                fw1394Camera *mpDownwardCameraRaw;
                //Utility::Filter* mDepthFilter;
                //Utility::Filter* mRawDepthFilter;
        };
    }
}
#endif
