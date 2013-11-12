/////////////////////////////////////////////////////////////////////////////////////
///
/// \file hokuyo.cpp
/// \brief LIDAR Interface.
///
/// Author(s): Gary Stein<br>
/// Created: 3/10/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
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
#include "hokuyourg/hokuyourg.h"
#include <urg/UrgCtrl.h>
#include <cxutils/math/cxmath.h>

using namespace CxUtils;
using namespace Zebulon;

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///////////////////////////////////////////////////////////////////////////////
Laser::HokuyoURG::HokuyoURG() : Laser::GenericLaser("URG")
{
    mpURG = new qrk::UrgCtrl();
    mAngularResolution = CxUtils::CxToRadians(.25);
    mScanningAngle = CxUtils::CxToRadians(275);
    mDistanceMode = .001;
    mDistanceMin = .1;
    mDistanceMax = 60;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
///////////////////////////////////////////////////////////////////////////////
Laser::HokuyoURG::~HokuyoURG()
{
    if(mpURG)
    {
        delete ((qrk::UrgCtrl*)mpURG);
    }
    mpURG = NULL;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Connects to the device and starts data acquistion.
///
///  \param[in] params Parameter data for the connection.
///
///  \return True on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
bool Laser::HokuyoURG::Initialize(const Laser::GenericLaser::Parameters* params)
{
    return Initialize(params->mSerialPort, params->mBaudRate, params->mTries);
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Connects to the device and start data acquistion.
///
///  \param[in] port The serial port to connect to.
///  \param[in] baud The baud rate at which to try to connect to first.  If
///                  this baud fails, then others will be automatically
///                  attempted.
///  \param[in] tries The number of tries to "ping" the laser and wait for a
///                   response (default is 1, try more if you need it)
///
///  \return True on success, false on failure.
///
///////////////////////////////////////////////////////////////////////////////
bool Laser::HokuyoURG::Initialize(const std::string& port, const int baud, const int tries)
{
    qrk::UrgCtrl* urg = (qrk::UrgCtrl*)mpURG;

#ifndef WIN32
#endif

    urg->disconnect();

    mValidDataFlag == false;
    bool connected = false;
    for(int i = 0; i < tries; i++)
    {
        if(urg->connect(port.c_str()))
        {
            connected = true;
            break;
        }
    }

    if(!connected)
    {
        if(urg->connect(port.c_str(), baud))
        {
            connected = true;
        }
        return false;
    }
    if(!connected)
    {
        return false;
    }

    this->mDistanceMax = urg->maxDistance()/1000.0;
    this->mDistanceMin = urg->minDistance()/1000.0;

    int maxScanLines = urg->maxScanLines();
    urg->setCaptureRange(0, maxScanLines - 1);
    std::string what = urg->what();

    double min = urg->index2rad(0);
    double max = urg->index2rad(maxScanLines - 1);
    mScanningAngle = fabs(min)*2;
    mAngularResolution = mScanningAngle/maxScanLines;
    // Set capture mode to continuous
    //urg->setCaptureMode(qrk::AutoCapture);

    CxUtils::SleepMs(1000);

    std::vector<long> data;
    long urg_timestamp;
    for(unsigned int i = 0; i < 5; i++)
    {
        if(urg->capture(data, &urg_timestamp) > 0)
        {
           mValidDataFlag = true;
           break;
        }
        CxUtils::SleepMs(10);
    }

    if(mValidDataFlag == false)
    {
        urg->reboot();
        urg->disconnect();
        CxUtils::SleepMs(3000);
        if(tries == 0)
        {
            return false;
        }
        return Initialize(port, baud, tries - 1);
    }

    if(mValidDataFlag == false)
        return false;

    if(!mThread.CreateThread(&ThreadFunction, this))
    {
        return false;
    }

    return true;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Shutsdown the interface.
///
///////////////////////////////////////////////////////////////////////////////
void Laser::HokuyoURG::Shutdown()
{
    mThread.StopThread();

    if(mpURG)
    {
        ((qrk::UrgCtrl*)mpURG)->disconnect();
    }
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Continuously receives data from the device, and generates
///         callbacks.
///
///////////////////////////////////////////////////////////////////////////////
void Laser::HokuyoURG::ThreadFunction(void *args)
{
#ifndef WIN32
    //Pass in as a void *, then cast, then virtuals should work
    HokuyoURG* laser= (HokuyoURG*)args;
    //call the appropriate real processing function per compass
    laser->ProcessingFunction();
#endif
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Continuously receives data from the device, and generates
///         callbacks.
///
///////////////////////////////////////////////////////////////////////////////
void Laser::HokuyoURG::ProcessingFunction()
{
    unsigned int count = 0;
    std::vector<long> data;
    qrk::UrgCtrl* urg = (qrk::UrgCtrl*)mpURG;
    while(!mThread.QuitThreadFlag())
    {
        long urg_timestamp = 0;
        if(urg->capture(data, &urg_timestamp) > 0)
        {
            mValidDataFlag = true;
            /* Testing code.
            long value;
            for(int i = 0; i < data.size(); i++)
            {
                value = data[i];


            }
            */
            CxUtils::Point3D::List scan;
            CxUtils::Time timestamp(true);

            double angle = mScanningAngle/2.0;
            for(std::vector<long>::iterator r = data.begin();
                r != data.end();
                r++)
            {
                CxUtils::Point3D point(*r*mDistanceMode, 0.0, angle);
                if(point.mX < mDistanceMin)
                {
                    point.mX = mDistanceMax + .01;
                }
                // Put in reverse order so that the angles are correct.
                //scan[mNumData - i - 1] = point;
                scan.push_back(point);
                angle -= mAngularResolution;
            }

            mLaserMutex.Lock();
            mLaserScan = scan;
            mLaserMutex.Unlock();

            // Trigger callbacks.
            Callback::Set::iterator callback;
            mCallbacksMutex.Lock();
            for(callback = mCallbacks.begin();
                    callback != mCallbacks.end();
                    callback++)
            {
                (*callback)->ProcessLaserData(mLaserScan, timestamp, mID);
            }
            mCallbacksMutex.Unlock();
        }

        CxUtils::SleepMs(1);
    }
}

/*  End of File */
