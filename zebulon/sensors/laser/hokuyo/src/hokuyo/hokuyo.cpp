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
#include "hokuyo/hokuyo.h"

#ifndef WIN32
#include "Hokuyo/HokuyoReader.hh"
#endif

using namespace CxUtils;
using namespace Zebulon;


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///////////////////////////////////////////////////////////////////////////////
Laser::Hokuyo::Hokuyo() : Laser::GenericLaser("Hokuyo")
{
#ifndef WIN32
    //Hard coded defaults for now
    mScanName = "range";
    mScanStart = 0;
    mScanEnd = 1080;
    mScanSkip = 1;
    mHokuyoReader = new HokuyoReader();
    mData = new unsigned int[HOKUYO_MAX_NUM_POINTS];
    memset(mData, 0, HOKUYO_MAX_NUM_POINTS*sizeof(unsigned int));
    mEncoding = HOKUYO_3DIGITS;
    mValidDataFlag = false;
#endif
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
///////////////////////////////////////////////////////////////////////////////
Laser::Hokuyo::~Hokuyo()
{
#ifndef WIN32
    Shutdown();
    if(mData)
    {
        delete[] mData;
    }
    mData = NULL;
    if(mHokuyoReader)
    {
        delete mHokuyoReader;
    }
    mHokuyoReader = NULL;
#endif
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
bool Laser::Hokuyo::Initialize(const Laser::GenericLaser::Parameters* params)
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
bool Laser::Hokuyo::Initialize(const std::string& port, const int baud, const int tries)
{
#ifndef WIN32
    int portNumber=0;
    bool connected=false;
    std::string newport;


    //If more than 10, then problems
    for(portNumber=-1;portNumber<10;portNumber++)
    {
        std::stringstream tmpstr;
        if(portNumber != -1)
        {
            tmpstr << port << portNumber;
        }
        else
        {
            tmpstr << port;
        }
        newport=tmpstr.str();
        std::cout << "Hokuyo: Trying port: " << newport << std::endl;
        if(mHokuyoReader->Connect(newport,baud))
        {
            std::cout << "Hokuyo: Could not connect to the sensor." << std::endl;
        }
        else
        {
            connected=true;
            break;
        }
    }
    if(connected == false)
    {
        std::cout << "Hokuyo: All Ports Failed" << std::endl;
        return false;
    }

    mDistanceMin = mHokuyoReader->GetDistMin();
    mDistanceMax = mHokuyoReader->GetDistMax();
    mDistanceMode = mHokuyoReader->GetDistRes();
    mScanningAngle = mHokuyoReader->GetAngleMax()-mHokuyoReader->GetAngleMin();
    mAngularResolution = mHokuyoReader->GetAngleRes();

    mSensorType = mHokuyoReader->_type;
    int newSkip;

    //get the special skip value (if needed) and scan type, depending on the mScanName and sensor type
    if(mHokuyoReader->GetScanTypeAndSkipFromName(mSensorType, (char *)mScanName.c_str(), &newSkip, &mScanType))
    {
        std::cerr << "Hokuyo: getScanTypeAndSkipFromName: Error getting the scan parameters."<< std::endl;
        return false;
    }

    if(newSkip != 1)
    {
        mScanSkip = newSkip;
    }

    //set the scan parameters
    if(mHokuyoReader->SetScanParams((char *)mScanName.c_str(),mScanStart, mScanEnd, mScanSkip, mEncoding, mScanType))
    {
        std::cerr << "Hokuyo: setScanSettings: Error setting the scan parameters." << std::endl;
        return false;
    }

    int numOutputArgs = mHokuyoReader->GetNumOutputArgs(mSensorType,(char *)mScanName.c_str());
    if(numOutputArgs < 0)
    {
        std::cerr << "Hokuyo: bad number of output args." << std::endl;
        return false;
    }

    //std::cout << "Number of output arguments = " << numOutputArgs <<std::endl;

    if(!mThread.CreateThread(&ThreadFunction, this))
    {
        return false;
    }
#endif
    return true;
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Shutsdown the interface.
///
///////////////////////////////////////////////////////////////////////////////
void Laser::Hokuyo::Shutdown()
{
    mThread.StopThread();
#ifndef WIN32
    mHokuyoReader->Disconnect();
#endif
}


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Continuously receives data from the device, and generates
///         callbacks.
///
///////////////////////////////////////////////////////////////////////////////
void Laser::Hokuyo::ThreadFunction(void *args)
{
#ifndef WIN32
    //Pass in as a void *, then cast, then virtuals should work
    Hokuyo* laser= (Hokuyo*)args;
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
void Laser::Hokuyo::ProcessingFunction()
{
#ifndef WIN32
    unsigned int count = 0;

    while(!mThread.QuitThreadFlag())
    {
        //Self blocking, until data arrives.
        mMutex.Lock();
        if(mHokuyoReader->GetScan(mData, mNumData) == 1)
        {
            //std::cerr<<"Hokuyo: scan failed"<<std::endl;
            mValidDataFlag=false;
        }
        else
        {
            mValidDataFlag=true;
            //std::cerr<<"Hokuyo: scan succeeded! got " <<mNumData<<" points"<<std::endl;
        }
        mMutex.Unlock();

        if(mValidDataFlag)
        {
            CxUtils::Point3D::List scan;
            CxUtils::Time timestamp(true);

            double angle = mScanningAngle/2.0;
            /*
            for(int i = mNumData-1; i >=0 ; i--)
            {
                CxUtils::Point3D point(mData[i]*mDistanceMode, 0.0, angle);
                // Put in reverse order so that the angles are correct.
                //scan[mNumData - i - 1] = point;
                scan.push_back(point);
                angle -= mAngularResolution;
            }
            */
            for(int i = 0; i < mNumData; i++)
            {
                CxUtils::Point3D point(mData[i]*mDistanceMode, 0.0, angle);
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
            mLaserScan=scan;
            mLaserMutex.Unlock();
            
            // Trigger callbacks.
            Callback::Set::iterator callback;
            mCallbacksMutex.Lock();
            for(callback = mCallbacks.begin();
                    callback != mCallbacks.end();
                    callback++)
            {
                (*callback)->ProcessLaserData(mLaserScan, timestamp, mID);
                /* Substitute later.
                Sick::Callback* sickCallback = dynamic_cast<Sick::Callback*>(*callback);
                if(sickCallback)
                {
                    sickCallback->ProcessMessage(finalMessage);
                }
                */
            }
            mCallbacksMutex.Unlock();
        }
        CxUtils::SleepMs(1);
    }
#endif
}

/*  End of File */
