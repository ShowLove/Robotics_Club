/////////////////////////////////////////////////////////////////////////////
///
///  \file globalinfo.cpp
///  \brief Class for getting information about the global state and
///         sensors of a robot.
///
///  Author(s): Gary Stein, Daniel Barber<br>
///  Created: 2008<br>
///  Copyright (c) 2010<br>
///  Robotics Laboratory and Club<br>
///  University of Central Florida (UCF) <br>
///  Email: gstein@mail.ucf.edu, dbarber@ist.ucf.edu<br>
///  Web: http://robotics.ucf.edu <br>
///  All Rights Reserved <br>
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
#include "statemachine/globalinfo.h"
#include <jaus/mobility/sensors/queryglobalpose.h>
#include <jaus/mobility/sensors/reportglobalpose.h>
#include <jaus/extras/video/visualsensor.h>
#include <jaus/extras/rangesensor/rangesensor.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace Zebulon;
using namespace AI;

// Converts image data to IplImage.
inline bool CvConvertToIplImage(const CxUtils::Image& in, IplImage** out)
{
    if(in.mpImage != NULL)
    {
        if(*out == NULL ||
           (*out)->width != (int)in.mWidth ||
           (*out)->height != (int)in.mHeight ||
           (*out)->nChannels != (int)in.mChannels)
        {
            if(*out)
            {
                cvReleaseImage(out);
            }
            *out = cvCreateImage(cvSize((int)in.mWidth, (int)in.mHeight), IPL_DEPTH_8U, 3);
        }

        // Copy the data.
        memcpy((*out)->imageData, in.mpImage, (*out)->imageSize);
        return true;
    }

    return false;
}

// How to initialize a static const string.
//const std::string GlobalInfo::Latitude = "Latitude";

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalInfo::GlobalInfo() : JAUS::SubsystemCommand(JAUS::Service::ID("GlobalInfo"))
{
    mLoggingEnabledFlag = false;
    mIsLoggingFlag = false;
    mLogDataDelayMs = mImageLogDelayMs = mLaserLogDelayMs = 250;
    mLogStream = NULL;
    mRangeSensorStream = NULL;
    // Create logging directory in case it doesn't exist.
    CxUtils::FileIO::CreateDir("logs");
    mDataThread.CreateThread(GlobalInfo::DataThread, this);

    //mEastingFilter = new Utility::AverageFilter(8);
    //mNorthingFilter = new Utility::AverageFilter(8);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalInfo::~GlobalInfo()
{
    mDataThread.StopThread();
    mPointDataThread.StopThread();
    JAUS::Mutex::ScopedLock lock(&mImageMutex);
    std::map<std::string, IplImage*>::iterator img;
    for(img = mImageInfo.begin(); img != mImageInfo.end(); img++)
    {
        // Destroy windows made.
        cvDestroyWindow(img->first.c_str());
    }

    //delete mEastingFilter;
    //delete mNorthingFilter;
}


/** Method to register your callback to Global Info. */
void GlobalInfo::RegisterCallback(Callback *cb)
{
    CxUtils::Mutex::ScopedLock lock(&mGlobalInfoCallbacksMutex);
    mGlobalInfoCallbacks.insert(cb);
}


/** Method to register your callback to Global Info. */
void GlobalInfo::RegisterCallback(const std::string &name, Callback *cb)
{
    CxUtils::Mutex::ScopedLock lock(&mGlobalInfoCallbacksMutex);
    mKeyGlobalInfoCallbacks[name] = cb;
}


/** Method to register your callback to Global Info. */
void GlobalInfo::RegisterPointCallback(Callback *cb)
{
    CxUtils::Mutex::ScopedLock lock(&mGlobalInfoCallbacksMutex);
    mGlobalInfoPointCallbacks.insert(cb);
}



/** Optional method to load settings from XML. */
bool GlobalInfo::LoadSettings(const std::string& xmlFilename)
{
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Prints select data from this class to console.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::PrintStatus() const
{
    std::cout << "\nGlobal Info ==============================================================\n";
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Displays any image data to the screen using OpenCV HighGUI.
///
///  \param[in] sleepDelayMs How long of a cvWaitKey to perform (required for
///                          drawing to screen).
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::DisplayImages(const unsigned int sleepDelayMs) const
{
    JAUS::Mutex::ScopedLock lock(&mImageMutex);
    std::map<std::string, IplImage*>::const_iterator img;
    for(img = mImageInfo.begin(); img != mImageInfo.end(); img++)
    {
        Utility::Display::DisplayImage(img->first,img->second);
    }
    cvWaitKey(sleepDelayMs);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Enables or disables data logging.
///
///  Once called, logging will begin.
///
///  \param[in] enable If true enable logging, if false disable logging.
///  \param[in] dataIntervalMs Time between logging standard data (ms).
///  \param[in] imageIntervalMs Time between logging image data (ms).
///  \param[in] laserIntervalMs Time between logging laser data (ms).
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::EnableLogging(const bool enable,
                               const CxUtils::Time::Stamp dataIntervalMs,
                               const CxUtils::Time::Stamp imageIntervalMs,
                               const CxUtils::Time::Stamp laserIntervalMs)
{
    CxUtils::Mutex::ScopedLock lock(&mRunningMutex);
    mLoggingEnabledFlag = enable;
    this->mLogDataDelayMs = dataIntervalMs;
    this->mImageLogDelayMs = imageIntervalMs;
    this->mLaserLogDelayMs = laserIntervalMs;
}


// Method to disable logging of specific images added to GlobalInfo
void GlobalInfo::AddImageNotToLog(const std::string& imageName)
{
    mImageMutex.Lock();
    mImagesNotToLog.insert(imageName);
    mImageMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets how long to wait inbetween logging for all data.
///
///  \param[in] logDelayMs How long to wait in ms between log file update.
///
///  \return True if set, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::SetLoggingInterval(const unsigned int logDelayMs)
{
    CxUtils::Mutex::ScopedLock lock(&mRunningMutex);
    if(logDelayMs >= 1)
    {
        mLogDataDelayMs = logDelayMs;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if logging, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::IsLogging() const
{
    CxUtils::Mutex::ScopedLock lock(&mRunningMutex);
    if(mIsLoggingFlag && mLoggingEnabledFlag)
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data.
///  \param[out] value The data value retrieved (if set).
///
///  \return True if the data exits, false if it does not.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::GetInfo(const std::string&  name, double &value) const
{
    // Create temporary value
    double tempVal = 0.0;
    bool tempReturn = false;

    mDoubleMutex.Lock();

    // Create iterator
    std::map<std::string,double>::const_iterator it;
    // Check if variable exists in map
    it = mDoubleInfo.find(name);
    // If value doesn't exits
    if(it == mDoubleInfo.end())
    {
        std::cout << "GlobalInfo::Error - Variable (" << name << ") does not exist" << std::endl;
    }
    else
    {
        tempVal = it->second;
        tempReturn = true;
    }

    mDoubleMutex.Unlock();

    value = tempVal;
    return tempReturn;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data to set.
///  \param[out] value The data value to save/set.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::SetInfo(const std::string&  name, const double value)
{
    mDoubleMutex.Lock();
    mDoubleInfo[name] = value;
    mDoubleMutex.Unlock();
    mDataUpdateTimeMutex.Lock();
    mDataUpdateTime[name] = CxUtils::Time::GetUtcTimeMs();
    mDataUpdateTimeMutex.Unlock();

    if( mOnChangeLogData.find(name) != mOnChangeLogData.end())
    {
        LogData();
    }
    {
        Callback::Set::iterator cb;
        Callback::Map::iterator kcb;
        CxUtils::Mutex::ScopedLock lock(&mGlobalInfoCallbacksMutex);
        for(cb = mGlobalInfoCallbacks.begin();
            cb != mGlobalInfoCallbacks.end();
            cb++)
        {
            (*cb)->ProcessData(name, value);
        }
        if(mKeyGlobalInfoCallbacks.size() > 0 &&
                (kcb = mKeyGlobalInfoCallbacks.find(name)) != mKeyGlobalInfoCallbacks.end())
        {
            kcb->second->ProcessData(name, value);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data to set.
///  \param[in] value The data value to save/set.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::SetInfo(const std::string&  name, const CxUtils::Utm& value)
{
    SetInfo(name + std::string("UTM Northing"), value.mNorthing);
    SetInfo(name + std::string("UTM Easting"), value.mEasting);
    SetInfo(name + std::string("UTM Elevation"), value.mElevation);
    SetInfo(name + std::string("UTM Zone Number"), value.mZoneNumber);
    SetInfo(name + std::string("UTM Zone Letter"), value.mZoneLetter);
    if( mOnChangeLogData.find(name) != mOnChangeLogData.end())
    {
        LogData();
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data to set.
///  \param[out] value The data value to get.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::GetInfo(const std::string&  name, CxUtils::Utm& value) const
{
    bool result = true;
    result &= GetInfo(name + std::string("UTM Northing"), value.mNorthing);
    result &= GetInfo(name + std::string("UTM Easting"), value.mEasting);
    result &= GetInfo(name + std::string("UTM Elevation"), value.mElevation);
    result &= GetInfo(name + std::string("UTM Zone Number"), value.mZoneNumber);
    result &= GetInfo(name + std::string("UTM Zone Letter"), value.mZoneLetter);
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data to set.
///  \param[out] value The data value to get.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::GetInfo(const std::string&  name, CxUtils::Wgs& value) const
{
    CxUtils::Utm pos;
    if(GetInfo(name, pos))
    {
        value << pos;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data.
///  \param[out] value The data value retrieved (if set).
///
///  \return True if the data exits, false if it does not.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::GetInfo(const std::string&  name, int &value) const
{
    // Create temporary value
    int tempVal = 0;
    bool tempReturn = false;

    mIntMutex.Lock();

    // Create iterator
    std::map<std::string,int>::const_iterator it;
    // Check if variable exists in map
    it = mIntInfo.find(name);

    if(it == mIntInfo.end())
    {
        std::cout << "GlobalInfo::Error - Variable (" << name << ") does not exist" << std::endl;
    }
    else
    {
        tempVal = it->second;
        tempReturn = true;
    }

    mIntMutex.Unlock();

    value = tempVal;
    return tempReturn;


}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data to set.
///  \param[out] value The data value to save/set.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::SetInfo(const std::string&  name, const int value)
{
    mIntMutex.Lock();
    mIntInfo[name] = value;
    mIntMutex.Unlock();
    mDataUpdateTimeMutex.Lock();
    mDataUpdateTime[name] = CxUtils::Time::GetUtcTimeMs();
    mDataUpdateTimeMutex.Unlock();

    if( mOnChangeLogData.find(name) != mOnChangeLogData.end())
    {
        LogData();
    }

    {
        Callback::Set::iterator cb;
        Callback::Map::iterator kcb;
        CxUtils::Mutex::ScopedLock lock(&mGlobalInfoCallbacksMutex);
        for(cb = mGlobalInfoCallbacks.begin();
            cb != mGlobalInfoCallbacks.end();
            cb++)
        {
            (*cb)->ProcessData(name, value);
        }
        if(mKeyGlobalInfoCallbacks.size() > 0 &&
                (kcb = mKeyGlobalInfoCallbacks.find(name)) != mKeyGlobalInfoCallbacks.end())
        {
            kcb->second->ProcessData(name, value);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data.
///  \param[out] value The data value retrieved (if set).
///
///  \return True if the data exits, false if it does not.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::GetInfo(const std::string&  name, bool &value) const
{
    // Create temporary value
    bool tempVal = false;
    bool tempReturn = false;

    mBoolMutex.Lock();

    // Create iterator
    std::map<std::string,bool>::const_iterator it;
    // Check if variable exists in map
    it = mBoolInfo.find(name);

    if(it == mBoolInfo.end())
    {
        std::cout << "GlobalInfo::Error - Variable (" << name << ") does not exist" << std::endl;
    }
    else
    {
        tempVal = it->second;
        tempReturn = true;
    }

    mBoolMutex.Unlock();

    value = tempVal;
    return tempReturn;


}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data to set.
///  \param[out] value The data value to save/set.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::SetInfo(const std::string&  name, const bool value)
{
    mBoolMutex.Lock();
    mBoolInfo[name] = value;
    mBoolMutex.Unlock();
    mDataUpdateTimeMutex.Lock();
    mDataUpdateTime[name] = CxUtils::Time::GetUtcTimeMs();
    mDataUpdateTimeMutex.Unlock();

    if( mOnChangeLogData.find(name) != mOnChangeLogData.end())
    {
        LogData();
    }

    {
        Callback::Set::iterator cb;
        Callback::Map::iterator kcb;
        CxUtils::Mutex::ScopedLock lock(&mGlobalInfoCallbacksMutex);
        for(cb = mGlobalInfoCallbacks.begin();
            cb != mGlobalInfoCallbacks.end();
            cb++)
        {
            (*cb)->ProcessData(name, value);
        }
        if(mKeyGlobalInfoCallbacks.size() > 0 &&
                (kcb = mKeyGlobalInfoCallbacks.find(name)) != mKeyGlobalInfoCallbacks.end())
        {
            kcb->second->ProcessData(name, value);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data.
///  \param[out] value The data value retrieved (if set).
///
///  \return True if the data exits, false if it does not.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::GetInfo(const std::string&  name, IplImage *&value) const
{
    // Create temporary value
    bool tempReturn = false;

    CxUtils::Mutex::ScopedLock lock(&mImageMutex);

    // Create iterator
    std::map<std::string,IplImage *>::const_iterator it;
    // Check if variable exists in map
    it = mImageInfo.find(name);

    if(it == mImageInfo.end())
    {
        std::cout << "GlobalInfo::Error - Variable (" << name << ") does not exist" << std::endl;
    }
    else
    {
        // Make a copy of the data.
        if(value == NULL ||
           value->width != it->second->width ||
           value->height != it->second->height ||
           value->nChannels != it->second->nChannels)
        {
            if(value)
            {
                cvReleaseImage(&value);
                value = NULL;
            }
            value = (IplImage*)cvCloneImage(it->second);
        }
        else
        {
            cvCopy(it->second, value, NULL);
        }
        tempReturn = true;
    }

    return tempReturn;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data to set.
///  \param[out] value Pointer to an IplImage to copy.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::SetInfo(const std::string&  name, const IplImage *value)
{
    if(value == NULL)
        return;

    int frameNumber = 0;
    mImageMutex.Lock();
    std::map<std::string, IplImage*>::iterator img;
    img = mImageInfo.find(name);
    if(img == mImageInfo.end())
    {
        mImageInfo[name] = cvCloneImage(value);
        frameNumber = mImageNumbers[name] = 0;
    }
    else
    {
        cvCopy(value, img->second, NULL);
        mImageNumbers[name]++;
        frameNumber = mImageNumbers[name];
    }

    mImageMutex.Unlock();


    mDataUpdateTimeMutex.Lock();
    mDataUpdateTime[name] = CxUtils::Time::GetUtcTimeMs();
    mDataUpdateTimeMutex.Unlock();

    if(mImageLogDelayMs == 0 && mLoggingEnabledFlag && mLogDirectory.size() > 0)
    {
        if(mImagesNotToLog.find(name) == mImagesNotToLog.end())
        {
            std::stringstream fname;                    // Output filename for image
            fname << mLogDirectory << name << "/";
            CxUtils::FileIO::CreateDir(fname.str());    // Create output directory.
            fname << std::setfill('0') << std::setw(8) << frameNumber << ".png";
            cvSaveImage(fname.str().c_str(), value);

            mLastFrameNumberMutex.Lock();
            mImageNumbersLogged[name] = frameNumber;
            mLastFrameNumberMutex.Unlock();
        }
    }



    {
        Callback::Set::iterator cb;
        Callback::Map::iterator kcb;
        CxUtils::Mutex::ScopedLock lock(&mGlobalInfoCallbacksMutex);
        for(cb = mGlobalInfoCallbacks.begin();
            cb != mGlobalInfoCallbacks.end();
            cb++)
        {
            (*cb)->ProcessData(name, value);
        }
        if(mKeyGlobalInfoCallbacks.size() > 0 &&
                (kcb = mKeyGlobalInfoCallbacks.find(name)) != mKeyGlobalInfoCallbacks.end())
        {
            kcb->second->ProcessData(name, value);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data.
///  \param[out] points The data value retrieved (if set).
///
///  \return True if the data exits, false if it does not.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::GetInfo(const std::string&  name, CxUtils::Point3D::List& points) const
{
    // Create temporary value
    bool tempVal = false;
    bool tempReturn = false;

    points.clear();

    mPointMutex.Lock();

    // Create iterator
    std::map<std::string, CxUtils::Point3D::List>::const_iterator it;
    // Check if variable exists in map
    it = mPointInfo.find(name);

    if(it == mPointInfo.end())
    {
        std::cout << "GlobalInfo::Error - Variable (" << name << ") does not exist" << std::endl;
    }
    else
    {
        points = it->second;
        tempReturn = true;
    }

    mPointMutex.Unlock();

    return tempReturn;


}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data to set.
///  \param[out] points The data value to save/set.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::SetInfo(const std::string&  name, const CxUtils::Point3D::List& points)
{
    if(points.size() == 0)
        return;

    mPointMutex.Lock();
    mPointInfo[name] = points;
    mPointMutex.Unlock();
    mDataUpdateTimeMutex.Lock();
    mDataUpdateTime[name] = CxUtils::Time::GetUtcTimeMs();
    mDataUpdateTimeMutex.Unlock();

    {
        Callback::Set::iterator cb;
        Callback::Map::iterator kcb;
        CxUtils::Mutex::ScopedLock lock(&mGlobalInfoCallbacksMutex);
        for(cb = mGlobalInfoCallbacks.begin();
            cb != mGlobalInfoCallbacks.end();
            cb++)
        {
            (*cb)->ProcessData(name, points);
        }
        for(cb = mGlobalInfoPointCallbacks.begin();
            cb != mGlobalInfoPointCallbacks.end();
            cb++)
        {
            (*cb)->ProcessData(name, points);
        }
        if(mKeyGlobalInfoCallbacks.size() > 0 &&
                (kcb = mKeyGlobalInfoCallbacks.find(name)) != mKeyGlobalInfoCallbacks.end())
        {
            kcb->second->ProcessData(name, points);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the value of data by a unique string identifer label.
///
///  \param[in] name The name of the element/data.
///  \param[out] timeMs The timestamp of the image data.
///
///  \return True if the data exits, false if it does not.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::GetInfoUpdateTimeMs(const std::string&  name, CxUtils::Time::Stamp& timeMs) const
{
    // Create temporary value
    bool tempReturn = false;
    timeMs = 0;

    CxUtils::Mutex::ScopedLock lock(&mDataUpdateTimeMutex);

    // Create iterator
    std::map<std::string, CxUtils::Time::Stamp>::const_iterator it;
    // Check if variable exists in map
    it = mDataUpdateTime.find(name);

    if(it == mDataUpdateTime.end())
    {
        std::cout << "GlobalInfo::Error - Variable (" << name << ") does not exist" << std::endl;
    }
    else
    {
        timeMs = it->second;
        tempReturn = true;
    }

    return tempReturn;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Method to check if a value has been set (based on string name).
///
///  \param[in] name The name of the element/data to check for.
///
///  \return True if a value has been set, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::IsValueSet(const std::string& name) const
{
    bool result = false;

    std::map<std::string, CxUtils::Time::Stamp>::const_iterator d;
    mDataUpdateTimeMutex.Lock();
    d = mDataUpdateTime.find(name);
    if(d != mDataUpdateTime.end())
    {
        result = true;
    }
    mDataUpdateTimeMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Method to create subscriptions to any video cameras on this
///         subsystem.
///
///  \param[in] id ID of component to subscribe to.  If blank (0.0.0) then
///                method looks up a component with services required.
///
///  \return True on success, false on failure.  A return of true can happen
///          if there are no cameras available to connect to.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::CreateVideoSubscriptions(const JAUS::Address& id)
{
    bool result = false;

    if(GetComponent() == NULL)
    {
        return result;
    }

    JAUS::VideoSubscriber* video = (JAUS::VideoSubscriber*)GetComponent()->GetService(JAUS::VideoSubscriber::Name);

    if(video)
    {
       video->RegisterCallback(this, true, true);
       // Find any available Visual Sensor services.
       JAUS::Address::List components = GetComponent()->DiscoveryService()->GetSubsystem(GetComponentID().mSubsystem)->GetComponentsWithService(JAUS::VisualSensor::Name);
       JAUS::Address::List::iterator c;

       // If no visual sensors, no problem.
       if(id.IsValid() == false && components.size() == 0)
       {
           result = true;
       }

       if(id.IsValid())
       {
           components.clear();
           components.push_back(id);
       }

       for(c = components.begin(); c != components.end(); c++)
       {
           unsigned int numConnections = 0;
           std::vector<JAUS::Byte> cameras;
           // See what cameras are available, and make subscriptions to all.
           if(video->GetCameraList(*c, cameras, 1000))
           {
               for(unsigned int camera = 0; camera < (unsigned int)cameras.size(); camera++)
               {
                   for(unsigned int i = 0; i < 5; i++)
                   {
                       if(video->HaveVideoSubscription(*c, cameras[camera]) == false)
                       {
                           if(video->CreateVideoSubscription(*c, cameras[camera]))
                           {
                            numConnections++;
                            break;
                           }
                       }
                       else
                       {
                           numConnections++;
                           break;
                       }
                       CxUtils::SleepMs(50);
                   }
               }
               if(numConnections == (unsigned int)cameras.size())
               {
                    result = true;
               }
               else
               {
                   std::cout << "GlobalInfo::Error - " << c->ToString() << " Camera Connections: " << numConnections << " Cameras Discovered: " << cameras.size() << std::endl;
                   result = false;
                   break;
               }
           }
           else
           {
               std::cout << "GlobalInfo::Error - No Cameras Found on Component: " << c->ToString() << "\n";
               result = false;
               break;
           }
       }
    }

   return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Method to map a JAUS camera ID to a local string name so that
///         GlobalInfo can save the image data to internal data members
///         automatically.
///
///  \param[in] cameraID Camera enumeration ID (e.g. 0, 1, 2).
///  \param[in] name The camera name [e.g. "Front", "Back").
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::SetCameraName(const unsigned int cameraID, const std::string& name)
{
    mImageMutex.Lock();
    mCameraNames[cameraID] = name;
    mImageMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Method to create subscriptions to any range sensors on this
///         subsystem.
///
///  \param[in] id ID of component to subscribe to.  If blank (0.0.0) then
///                method looks up a component with services required.
///  \param[in] updateRateHz If greater than 0, periodic event subscriptions
///                          are made.  If 0, every change update is made.
///
///  \return True on success, false on failure.  A return of true can happen
///          if there are no range sensors available to connect to.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::CreateRangeSensorSubscriptions(const JAUS::Address& id,
                                                const double updateRateHz)
{
    bool result = false;

    if(GetComponent() == NULL)
    {
        return result;
    }

    JAUS::RangeSubscriber* range = (JAUS::RangeSubscriber*)GetComponent()->GetService(JAUS::RangeSubscriber::Name);

    if(range)
    {
       range->RegisterCallback(this, true);
       // Find any available Visual Sensor services.
       JAUS::Address::List components = GetComponent()->DiscoveryService()->GetSubsystem(GetComponentID().mSubsystem)->GetComponentsWithService(JAUS::RangeSensor::Name);
       JAUS::Address::List::iterator c;

       // If no range sensors, no problem.
       if(id.IsValid() == false && components.size() == 0)
       {
           result = true;
       }

       if(id.IsValid())
       {
           components.clear();
           components.push_back(id);
       }

       for(c = components.begin(); c != components.end(); c++)
       {
           unsigned int numConnections = 0;
           JAUS::RangeSensorConfig::List sensors;
           // See what cameras are available, and make subscriptions to all.
           if(range->GetRangeSensorInfo(*c, sensors, 5000))
           {
               for(unsigned int sensor = 0; sensor < (unsigned int)sensors.size(); sensor++)
               {
                   for(unsigned int i = 0; i < 3; i++)
                   {
                        if(range->HaveRangeSubscription(*c, sensors[sensor].mID) == false)
                        {
                           if(updateRateHz <= 0.0001)
                           {

                               if(range->CreateRangeSubscription(*c, sensors[sensor].mID))
                               {
                                    numConnections++;
                                    break;
                               }
                           }
                           else
                           {
                               JAUS::QueryRangeSensorConfiguration queryConfig(*c, GetComponentID());
                               Send(&queryConfig);
                               JAUS::QueryLocalRangeScan queryEventMessage;
                               queryEventMessage.SetSensorID(sensors[sensor].mID);
                               if(GetComponent()->EventsService()->RequestPeriodicEvent(*c,
                                                                                        &queryEventMessage,
                                                                                        updateRateHz,
                                                                                        updateRateHz - 0.1))
                               {
                                    numConnections++;
                                    break;
                               }
                           }
                        }

                        else
                        {
                            numConnections++;
                            break;
                        }
                   }
               }
               if(numConnections == (unsigned int)sensors.size())
               {
                    result = true;
               }
               else
               {
                   std::cout << "GlobalInfo::Error - " << c->ToString() << " Range Sensor Connections: " << numConnections << " Range Sensors Discovered: " << sensors.size() << std::endl;
                   result = false;
                   break;
               }
           }
           else
           {
               std::cout << "GlobalInfo::Error - No Range Sensors Found on Component: " << c->ToString() << "\n";
               result = false;
               break;
           }
       }
    }

   return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Method to map a JAUS sensor ID to a local string name so that
///         GlobalInfo can save the range data to internal data members
///         automatically.
///
///  \param[in] deviceID Sensor enumeration ID (e.g. 0, 1, 2).
///  \param[in] name The sensor name [e.g. "Hokuyo", "Front").
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::SetRangeSensorName(const unsigned int deviceID, const std::string& name)
{
    mPointMutex.Lock();
    mRangeSensorNames[deviceID] = name;
    mPointMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Callback method from VideoSubscriber interface.  This method
///         is called if we have registered for video data and new frame
///         is received.
///
///  This method using the camera id to name mapping from the SetCameraName
///  method to convert JAUS images to IplImages and save them to internal
///  data members.
///
///  \param[in] cameraID Camera enumeration ID (e.g. 0, 1, 2).
///  \param[in] name The camera name [e.g. "Front", "Back").
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::ProcessRawImage(const JAUS::Address& source,
                                 const JAUS::Byte cameraID,
                                 const JAUS::Image& image,
                                 const unsigned int frameNumber)
{
    //std::cout << "Image!\n";
    std::map<unsigned int, std::string>::iterator name;
    bool success = false;
    std::string cname;

    //  Convert to an IplImage.
    std::map<std::string, IplImage *>::iterator ipl;

    int fnumber = -1;
    mImageMutex.Lock();
    name = mCameraNames.find(cameraID);
    if(name != mCameraNames.end())
    {

        ipl = mImageInfo.find(name->second);
        if(ipl == mImageInfo.end())
        {
            mImageInfo[name->second] = NULL;
            ipl = mImageInfo.find(name->second);
            mImageNumbers[name->second] = 0;
        }
        else
        {
            mImageNumbers[name->second]++;
        }
        cname = name->second;
        if(CvConvertToIplImage(image, &ipl->second))
        {
            success = true;
        }

        fnumber = mImageNumbers[name->second];
    }
    else
    {
        return;
    }


    bool logged = false;
    if(mImageLogDelayMs == 0 && mLoggingEnabledFlag && mLogDirectory.size() > 0)
    {
        if(mImagesNotToLog.find(cname) == mImagesNotToLog.end())
        {
            std::stringstream fname;                    // Output filename for image
            fname << mLogDirectory << cname << "/";
            CxUtils::FileIO::CreateDir(fname.str());    // Create output directory.
            fname << std::setfill('0') << std::setw(8) << fnumber << ".png";
            cvSaveImage(fname.str().c_str(), ipl->second);
            logged = true;
        }
    }

    {
        Callback::Set::iterator cb;
        Callback::Map::iterator kcb;
        CxUtils::Mutex::ScopedLock lock(&mGlobalInfoCallbacksMutex);
        for(cb = mGlobalInfoCallbacks.begin();
            cb != mGlobalInfoCallbacks.end();
            cb++)
        {
            (*cb)->ProcessData(cname, ipl->second);
        }
        if(mKeyGlobalInfoCallbacks.size() > 0 &&
                (kcb = mKeyGlobalInfoCallbacks.find(cname)) != mKeyGlobalInfoCallbacks.end())
        {
            kcb->second->ProcessData(cname, ipl->second);
        }
    }

    mImageMutex.Unlock();

    if(logged)
    {
        mLastFrameNumberMutex.Lock();
        mImageNumbersLogged[cname] = fnumber;
        mLastFrameNumberMutex.Unlock();
    }

    if(success)
    {
        mDataUpdateTimeMutex.Lock();
        mDataUpdateTime[name->second] = CxUtils::Time::GetUtcTimeMs();
        mDataUpdateTimeMutex.Unlock();
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Callback method performed whenever range sensor data is
///         received by GlobalInfo.
///
///  This method using the device id to name mapping from the SetRangeSensorName.
///
///  \param[in] scan Point scan data from range sensor.
///  \param[in] sourceID JAUS component data came from.
///  \param[in] deviceID Sensor device number.
///  \param[in] timestamp Time the data was generated.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::ProcessLocalRangeScan(const CxUtils::Point3D::List& scan,
                                       const JAUS::Address& sourceID,
                                       const JAUS::Byte deviceID,
                                       const JAUS::Time& timestamp)
{
    std::map<unsigned int, std::string>::iterator name;
    bool success = false;
    std::string sname;
    mPointMutex.Lock();
    name = mRangeSensorNames.find(deviceID);
    if(name != mRangeSensorNames.end())
    {
        sname = name->second;
        success = true;
    }
    mPointMutex.Unlock();
    if(success)
    {
        this->SetInfo(sname, scan);
    }
}


/* EXAMPLE
void GlobalInfo::Receive(const JAUS::Message* jausMessage)
{
    // Call parent class first to be nice.
    switch(jausMessage->GetMessageCode())
    {
    case JAUS::REPORT_GLOBAL_POSE:
        // Do stuff with data
        {
            const JAUS::ReportGlobalPose* report = dynamic_cast<const JAUS::ReportGlobalPose*>(jausMessage);
            if(report)
            {
                // Consume, set values, etc.
            }
        }
        break;
    default:
        // If you don't know what the message is, you don't have to do anything.
        break;
    }
}
*/

/*  EXAMPLE
bool GlobalInfo::CreateSubscriptions()
{
    bool result = true;

    // Addresses are [subsystem number. node number (e.g. computer) . unique ID (e.g. 1-20)]
    JAUS::Address baselineComponent(153, 1, 1);

    // Subscribe to global pose data.
    if(EventsService()->HaveSubscription(baselineComponent, JAUS::REPORT_GLOBAL_POSE) == false)
    {
        // Create subscription
        JAUS::QueryGlobalPose queryGlobalPose;
        queryGlobalPose.SetPresenceVector(0xFFFF); // Request all fields
        if(EventsService()->RequestEveryChangeEvent(baselineComponent, &queryGlobalPose) == false)
        {
            result = false;
            return result;
        }
    }

    // Subscribe to other data...

    // Return true if you were able to successfully subscribe to all the data you want/need.
    // From this point on, the data you are subscribing to, will arrive through the
    // Receive method.

    return result;
}
*/

/* EXAMPLE
void GlobalInfo::CreateMessage(const JAUS::UShort jausMessageCode)
{
    switch(jausMessageCode)
    {
    case JAUS::REPORT_GLOBAL_POSE:
        return new JAUS::ReportGlobalPose();
        break;
    default:
        JAUS::SubsystemCommand::CreateMessage(jausMessageCode);
        break;
    }
}
*/


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Helper method for processing JAUS Report Global Pose message.
///
///  This method uses the field names from names.h to set the appropriate data
///  internally to GlobalInfo.
///
///  \param[in] message JAUS pose message to save.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::ProcessGlobalPose(const JAUS::ReportGlobalPose* message)
{
    JAUS::Mutex::ScopedLock lock(&mDoubleMutex);
    JAUS::Mutex::ScopedLock lock2(&mDataUpdateTimeMutex);
    JAUS::Time::Stamp updateTimeMs = JAUS::Time::GetUtcTimeMs();

    if(message->IsFieldPresent(JAUS::ReportGlobalPose::PresenceVector::Latitude))
    {
        mDoubleInfo[Names::Latitude] = message->GetLatitude();
        mDataUpdateTime[Names::Latitude] = updateTimeMs;
    }
    if(message->IsFieldPresent(JAUS::ReportGlobalPose::PresenceVector::Longitude))
    {
        mDoubleInfo[Names::Longitude] = message->GetLongitude();
        mDataUpdateTime[Names::Longitude] = updateTimeMs;
    }
    if(message->IsFieldPresent(JAUS::ReportGlobalPose::PresenceVector::Altitude))
    {
        mDoubleInfo[Names::Elevation] = message->GetAltitude();
        mDataUpdateTime[Names::Elevation] = updateTimeMs;
    }
    if(message->AreFieldsPresent(JAUS::ReportGlobalPose::PresenceVector::Latitude |
                                 JAUS::ReportGlobalPose::PresenceVector::Longitude |
                                 JAUS::ReportGlobalPose::PresenceVector::Altitude))
    {
        CxUtils::Wgs wgs(message->GetLatitude(),
                         message->GetLongitude(),
                         message->GetAltitude());
        CxUtils::Utm utm(wgs);

        //double easting, northing;
        //mEastingFilter->Add(utm.mEasting);
        //mNorthingFilter->Add(utm.mNorthing);
        /*
        //Check if the filter is valid or not.
        if(mEastingFilter->Output(easting))
        {
            mDoubleInfo[Names::Easting] = easting;
        }
        else
        {
            mDoubleInfo[Names::Easting] = utm.mEasting;
        }
        */
        mDoubleInfo[Names::Easting] = utm.mEasting;
        /*
        if(mNorthingFilter->Output(northing))
        {
            mDoubleInfo[Names::Northing] = northing;
        }
        else
        {
            mDoubleInfo[Names::Northing] = utm.mNorthing;
        }
        */
        mDoubleInfo[Names::Northing] = utm.mNorthing;

        mIntInfo[Names::ZoneNumber] = utm.mZoneNumber;
        mIntInfo[Names::ZoneLetter] = utm.mZoneLetter;

        mDataUpdateTime[Names::Northing] = updateTimeMs;
        mDataUpdateTime[Names::Easting] = updateTimeMs;
        mDataUpdateTime[Names::ZoneNumber] = updateTimeMs;
        mDataUpdateTime[Names::ZoneLetter] = updateTimeMs;
    }
    if(message->IsFieldPresent(JAUS::ReportGlobalPose::PresenceVector::Yaw))
    {
        mDoubleInfo[Names::Yaw] = message->GetYaw();
        mDoubleInfo[Names::YawDegrees] = CxUtils::CxToDegrees(message->GetYaw());
        mDataUpdateTime[Names::Yaw] = updateTimeMs;
        mDataUpdateTime[Names::YawDegrees] = updateTimeMs;
    }
    if(message->IsFieldPresent(JAUS::ReportGlobalPose::PresenceVector::Pitch))
    {
        mDoubleInfo[Names::Pitch] = message->GetPitch();
        mDoubleInfo[Names::PitchDegrees] = CxUtils::CxToDegrees(message->GetPitch());
        mDataUpdateTime[Names::Pitch] = updateTimeMs;
        mDataUpdateTime[Names::PitchDegrees] = updateTimeMs;
    }
    if(message->IsFieldPresent(JAUS::ReportGlobalPose::PresenceVector::Roll))
    {
        mDoubleInfo[Names::Roll] = message->GetRoll();
        mDoubleInfo[Names::RollDegrees] = CxUtils::CxToDegrees(message->GetRoll());
        mDataUpdateTime[Names::Roll] = updateTimeMs;
        mDataUpdateTime[Names::RollDegrees] = updateTimeMs;
    }
    if(message->IsFieldPresent(JAUS::ReportGlobalPose::PresenceVector::TimeStamp))
    {
        mDoubleInfo[Names::PoseTimeSeconds] = message->GetTimeStamp().ToSeconds();
        mDataUpdateTime[Names::PoseTimeSeconds] = updateTimeMs;
    }
    else
    {
        mDoubleInfo[Names::PoseTimeSeconds] = JAUS::Time::GetUtcTime().ToSeconds();
        mDataUpdateTime[Names::PoseTimeSeconds] = updateTimeMs;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Helper method for processing JAUS Report Velocity State message.
///
///  This method uses the field names from names.h to set the appropriate data
///  internally to GlobalInfo.
///
///  \param[in] message JAUS pose message to save.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::ProcessVelocityState(const JAUS::ReportVelocityState* message)
{
    JAUS::Mutex::ScopedLock lock(&mDoubleMutex);
    JAUS::Mutex::ScopedLock lock2(&mDataUpdateTimeMutex);
    JAUS::Time::Stamp updateTimeMs = JAUS::Time::GetUtcTimeMs();
    //linear velocities
    if(message->IsFieldPresent(JAUS::ReportVelocityState::PresenceVector::VelocityX))
    {
        mDoubleInfo[Names::VelocityX] = message->GetVelocityX();
        mDataUpdateTime[Names::VelocityX] = updateTimeMs;
    }
    if(message->IsFieldPresent(JAUS::ReportVelocityState::PresenceVector::VelocityY))
    {
        mDoubleInfo[Names::VelocityY] = message->GetVelocityY();
        mDataUpdateTime[Names::VelocityY] = updateTimeMs;
    }
    if(message->IsFieldPresent(JAUS::ReportVelocityState::PresenceVector::VelocityZ))
    {
        mDoubleInfo[Names::VelocityZ] = message->GetVelocityZ();
        mDataUpdateTime[Names::VelocityZ] = updateTimeMs;
    }
    if(message->IsFieldPresent(JAUS::ReportVelocityState::PresenceVector::VelocityRMS))
    {
        mDoubleInfo[Names::VelocityRMS] = message->GetVelocityRMS();
        mDataUpdateTime[Names::VelocityRMS] = updateTimeMs;
    }
    // Rotational rates
    if(message->IsFieldPresent(JAUS::ReportVelocityState::PresenceVector::RollRate))
    {
        mDoubleInfo[Names::RollRate] = message->GetRollRate();
        mDataUpdateTime[Names::RollRate] = updateTimeMs;

        mDoubleInfo[Names::RollRateDegrees] = CxUtils::CxToDegrees(message->GetRollRate());
        mDataUpdateTime[Names::RollRateDegrees] = updateTimeMs;
    }
    if(message->IsFieldPresent(JAUS::ReportVelocityState::PresenceVector::PitchRate))
    {
        mDoubleInfo[Names::PitchRate] = message->GetPitchRate();
        mDataUpdateTime[Names::PitchRate] = updateTimeMs;

        mDoubleInfo[Names::PitchRateDegrees] = CxUtils::CxToDegrees(message->GetPitchRate());
        mDataUpdateTime[Names::PitchRateDegrees] = updateTimeMs;
    }
    if(message->IsFieldPresent(JAUS::ReportVelocityState::PresenceVector::YawRate))
    {
        mDoubleInfo[Names::YawRate] = message->GetYawRate();
        mDataUpdateTime[Names::YawRate] = updateTimeMs;

        mDoubleInfo[Names::YawRateDegrees] = CxUtils::CxToDegrees(message->GetYawRate());
        mDataUpdateTime[Names::YawRateDegrees] = updateTimeMs;
    }
    if(message->IsFieldPresent(JAUS::ReportVelocityState::PresenceVector::RateRMS))
    {
        mDoubleInfo[Names::RateRMS] = message->GetRateRMS();
        mDataUpdateTime[Names::RateRMS] = updateTimeMs;
    }
    //  Timestamp
    if(message->IsFieldPresent(JAUS::ReportVelocityState::PresenceVector::TimeStamp))
    {
        mDoubleInfo[Names::VelocityTimeSeconds] = message->GetTimeStamp().ToSeconds();
        mDataUpdateTime[Names::VelocityTimeSeconds] = updateTimeMs;
    }
    else
    {
        mDoubleInfo[Names::VelocityTimeSeconds] = JAUS::Time::GetUtcTime().ToSeconds();
        mDataUpdateTime[Names::VelocityTimeSeconds] = updateTimeMs;
    }
}


/** Updates contents of data log for primitives and current frame number
    for playback alignment. */
void GlobalInfo::LogData()
{
    if(mLogStream == NULL) return;

    CxUtils::Mutex::ScopedLock lockStream(&mLogStreamMutex);

    // Log current image associated with this data point
    std::map<std::string, int> frameNumbers;
    mLastFrameNumberMutex.Lock();
    frameNumbers = mImageNumbersLogged;
    mLastFrameNumberMutex.Unlock();

    std::map<std::string, double> doubles;
    std::map<std::string, int> integers;
    std::map<std::string, bool> booleans;

    CxUtils::Mutex::ScopedLock lock(&mDataLogMutex);
    // Write common data.
    fprintf(mLogStream, "Log Time (UTC), %s,",
            CxUtils::Time::GetUtcTime().ToString().c_str());

    // Floating point data.
    std::map<std::string, double>::const_iterator d;
    mDoubleMutex.Lock();
    doubles = mDoubleInfo;
    mDoubleMutex.Unlock();
    for(d = doubles.begin(); d != doubles.end(); d++)
    {
        fprintf(mLogStream, "%s, [Double], %0.5lf,", d->first.c_str(), d->second);
    }
    // Integer data
    std::map<std::string, int>::const_iterator i;
    mIntMutex.Lock();
    integers = mIntInfo;
    mIntMutex.Unlock();
    for(i = integers.begin(); i != integers.end(); i++)
    {
        fprintf(mLogStream, "%s, [Int], %d,", i->first.c_str(), i->second);
    }


    // Boolean data
    std::map<std::string, bool>::const_iterator b;
    mBoolMutex.Lock();
    booleans = mBoolInfo;
    mBoolMutex.Unlock();
    for(b = booleans.begin(); b != booleans.end(); b++)
    {
        fprintf(mLogStream, "%s, [Bool], %d,", b->first.c_str(), b->second);
    }

    // Log the image frame numbers for this moment in time for
    // synchronized playback
    std::map<std::string, int>::const_iterator fnum;

    for(fnum = frameNumbers.begin(); fnum != frameNumbers.end(); fnum++)
    {
        if(mImagesNotToLog.find(fnum->first) == mImagesNotToLog.end())
        {
            fprintf(mLogStream, "%s, [Frame Number], %d,", fnum->first.c_str(), fnum->second);
        }
    }

    // Write additional data.
    std::vector<std::string> additional = GetAdditionalLogData();
    std::vector<std::string>::iterator add;
    for(add = additional.begin(); add != additional.end(); add++)
    {
        fprintf(mLogStream, "%s,", add->c_str());
    }
    // End of line.
    fprintf(mLogStream, "\n");
    // Force write to disk.
    fflush(mLogStream);
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Data processing thread for logging and verification of subscriptions.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::DataThread(void* args)
{
    GlobalInfo* ginfo = (GlobalInfo*)args;
    std::string logDirectory;
    CxUtils::Time::Stamp lastDataLogTimeMs = 0;
    CxUtils::Time::Stamp lastImageLogTimeMs = 0;
    CxUtils::Time::Stamp lastLaserLogTimeMs = 0;

    std::map<std::string, CxUtils::Time::Stamp> prevUpdateTimes, curUpdateTimes;

    while(ginfo && ginfo->mDataThread.QuitThreadFlag() == false)
    {
        if(ginfo->mLoggingEnabledFlag)
        {
            std::map<std::string, CxUtils::Time::Stamp>::const_iterator stamp;
            // If this is the first time we started data logging, then
            // create the log file, and directories, and intialize column headers.
            if(ginfo->mIsLoggingFlag == false)
            {
                std::stringstream dname;
                // Create unique directory name.
                time_t rawtime;
                tm * ptm;

                time ( &rawtime );

                ptm = gmtime ( &rawtime );

                dname << "logs/"
                      << std::setw(4) << std::setfill('0') << ptm->tm_year + 1900 << "."
                      << std::setw(2) << std::setfill('0') << ptm->tm_mon + 1 << "."
                      << CxUtils::Time::GetUtcTime().ToString(true, true, true, true, false) << "/";

                ginfo->mDataThreadMutex.Lock();
                logDirectory = ginfo->mLogDirectory = dname.str();
                ginfo->mDataThreadMutex.Unlock();

                // Create the directory.
                CxUtils::FileIO::CreateDir(logDirectory);
                // Create file.
                if(ginfo->mLogStream)
                {
                    fclose(ginfo->mLogStream);
                    ginfo->mLogStream = NULL;
                }
                std::stringstream fname;
                /*
                fname << dname.str() << "global_info_lidar_v2.csv";
                ginfo->mRangeSensorStream = fopen(fname.str().c_str(), "wt");
                */

                fname.clear();
                fname.str(std::string());
                fname << dname.str() << "global_info_log_v2.csv";
                ginfo->mLogStream = fopen(fname.str().c_str(), "wt");

                ginfo->mPointDataThread.CreateThread(GlobalInfo::PointDataThread, ginfo);

                ginfo->mIsLoggingFlag = true;
            }

            // Don't log all the time, only periodically based on the delay between updates.
            if(ginfo->mIsLoggingFlag)
            {        
                if(CxUtils::Time::GetUtcTimeMs() - lastDataLogTimeMs >= ginfo->mLogDataDelayMs)
                {
                    ginfo->LogData();

                    lastDataLogTimeMs = CxUtils::Time::GetUtcTimeMs();
                }

                if(ginfo->mImageLogDelayMs > 0 &&
                                   CxUtils::Time::GetUtcTimeMs() - lastImageLogTimeMs >= ginfo->mImageLogDelayMs)
                {
                    std::map<std::string, int> frameNumbersLogged;
                    // Log image data differently then other information.
                    std::map<std::string, IplImage*>::const_iterator img;
                    ginfo->mImageMutex.Lock();
                    for(img = ginfo->mImageInfo.begin(); img != ginfo->mImageInfo.end(); img++)
                    {
                        if(ginfo->mImagesNotToLog.find(img->first) == ginfo->mImagesNotToLog.end())
                        {
                            std::stringstream fname; // Output filename for image
                            fname << logDirectory << img->first << "/";
                            CxUtils::FileIO::CreateDir(fname.str());    // Create output directory.
                            fname << std::setfill('0') << std::setw(8) << ginfo->mImageNumbers[img->first] << ".png";

                            cvSaveImage(fname.str().c_str(), img->second);
                        }
                    }
                    frameNumbersLogged = ginfo->mImageNumbers;
                    ginfo->mImageMutex.Unlock();
                    lastImageLogTimeMs = CxUtils::Time::GetUtcTimeMs();

                    ginfo->mLastFrameNumberMutex.Lock();
                    ginfo->mImageNumbersLogged = frameNumbersLogged;
                    ginfo->mLastFrameNumberMutex.Unlock();
                }

                /*
                if(ginfo->mLaserLogDelayMs > 0 &&
                   CxUtils::Time::GetUtcTimeMs() - lastLaserLogTimeMs >= ginfo->mLaserLogDelayMs)
                {
                    CxUtils::Mutex::ScopedLock lock(&ginfo->mLidarLogMutex);
                    // Write LIDAR Data
                    // Write common data.
                    fprintf(ginfo->mRangeSensorStream, "Log Time (UTC), %s,",
                            CxUtils::Time::GetUtcTime().ToString().c_str());

                    std::map<std::string, CxUtils::Point3D::List> pointData;
                    ginfo->mPointMutex.Lock();
                    pointData = ginfo->mPointInfo;
                    ginfo->mPointMutex.Unlock();

                    // Write number of lasers.
                    fprintf(ginfo->mRangeSensorStream, "%d,", (int)(pointData.size()));
                    std::map<std::string, CxUtils::Point3D::List>::iterator lidar;
                    for(lidar = pointData.begin();
                        lidar != pointData.end();
                        lidar++)
                    {
                        // Save LIDAR Name
                        fprintf(ginfo->mRangeSensorStream, "%s, %d, ", lidar->first.c_str(), (int)lidar->second.size());
                        CxUtils::Point3D::List::iterator point;
                        for(point = lidar->second.begin();
                            point != lidar->second.end();
                            point++)
                        {
                            fprintf(ginfo->mRangeSensorStream, "%0.4lf, %0.4lf, %0.4lf,", point->mX, point->mY, point->mZ);
                        }
                    }
                    fprintf(ginfo->mRangeSensorStream, "\n");

                    // Only this thread access the stream, so flush
                    // outside of the mutex.
                    fflush(ginfo->mRangeSensorStream);

                    lastLaserLogTimeMs = CxUtils::Time::GetUtcTimeMs();
                }
                */
            }

        }
        else
        {
            // Close any open files, etc.
            if(ginfo->mLogStream)
            {
                fclose(ginfo->mLogStream);
                ginfo->mLogStream = NULL;
            }

            ginfo->mDataThreadMutex.Lock();
            ginfo->mLogDirectory.clear();
            ginfo->mDataThreadMutex.Unlock();
        }

        CxUtils::SleepMs(1);
    }

    if(ginfo->mLogStream)
    {
        fclose(ginfo->mLogStream);
        ginfo->mLogStream = NULL;
    }

    ginfo->mPointDataThread.StopThread();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Data processing thread for logging and verification of subscriptions.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalInfo::PointDataThread(void* args)
{
    GlobalInfo* ginfo = (GlobalInfo*)args;
    std::string logDirectory;
    CxUtils::Time::Stamp lastLaserLogTimeMs = 0;

    std::map<std::string, CxUtils::Time::Stamp> prevUpdateTimes, curUpdateTimes;

    while(ginfo && ginfo->mPointDataThread.QuitThreadFlag() == false)
    {
        if(ginfo->mLoggingEnabledFlag)
        {
            std::map<std::string, CxUtils::Time::Stamp>::const_iterator stamp;
            // If this is the first time we started data logging, then
            // create the log file, and directories, and intialize column headers.
            if(ginfo->mIsLoggingFlag == false)
            {
                ginfo->mDataThreadMutex.Lock();
                logDirectory = ginfo->mLogDirectory;
                ginfo->mDataThreadMutex.Unlock();

                // Create the directory.
                CxUtils::FileIO::CreateDir(logDirectory);
                // Create file.
                std::stringstream fname;
                fname << logDirectory << "global_info_lidar_v2.csv";
                ginfo->mRangeSensorStream = fopen(fname.str().c_str(), "wt");
            }

            // Don't log all the time, only periodically based on the delay between updates.
            if(ginfo->mIsLoggingFlag)
            {
                bool logLaserFlag = false;

                std::map<std::string, CxUtils::Point3D::List> pointData;
                ginfo->mPointMutex.Lock();
                pointData = ginfo->mPointInfo;
                ginfo->mPointMutex.Unlock();

                std::map<std::string, CxUtils::Point3D::List>::iterator lidar;

                if(ginfo->mLaserLogDelayMs == 0)
                {
                    ginfo->mDataUpdateTimeMutex.Lock();
                    curUpdateTimes = ginfo->mDataUpdateTime;
                    ginfo->mDataUpdateTimeMutex.Unlock();

                    std::map<std::string, CxUtils::Point3D::List> pointData;
                    ginfo->mPointMutex.Lock();
                    pointData = ginfo->mPointInfo;
                    ginfo->mPointMutex.Unlock();

                    std::map<std::string, CxUtils::Point3D::List>::iterator lidar;
                    bool flushFlag = false;
                    for(lidar = pointData.begin();
                        lidar != pointData.end();
                        lidar++)
                    {
                        if(prevUpdateTimes[lidar->first] != curUpdateTimes[lidar->first])
                        {
                            // Write LIDAR Data
                            // Write common data.
                            fprintf(ginfo->mRangeSensorStream, "Log Time (UTC), %s,",
                                    CxUtils::Time::GetUtcTime().ToString().c_str());

                            // Write number of lasers.
                            fprintf(ginfo->mRangeSensorStream, "%d,", (int)(1));
                            // Write laser name and points
                            fprintf(ginfo->mRangeSensorStream, "%s, %d, ", lidar->first.c_str(), (int)lidar->second.size());
                            CxUtils::Point3D::List::iterator point;
                            for(point = lidar->second.begin();
                                point != lidar->second.end();
                                point++)
                            {
                                fprintf(ginfo->mRangeSensorStream, "%0.4lf, %0.4lf, %0.4lf,", point->mX, point->mY, point->mZ);
                            }

                            fprintf(ginfo->mRangeSensorStream, "\n");
                            flushFlag = true;
                        }
                    }

                    if(flushFlag)
                    {
                        // Only this thread access the stream, so flush
                        // outside of the mutex.
                        fflush(ginfo->mRangeSensorStream);
                    }

                    prevUpdateTimes = curUpdateTimes;
                }
                if(ginfo->mLaserLogDelayMs > 0 && CxUtils::Time::GetUtcTimeMs() - lastLaserLogTimeMs >= ginfo->mLaserLogDelayMs)
                {
                    CxUtils::Mutex::ScopedLock lock(&ginfo->mLidarLogMutex);

                    std::map<std::string, CxUtils::Point3D::List> pointData;
                    ginfo->mPointMutex.Lock();
                    pointData = ginfo->mPointInfo;
                    ginfo->mPointMutex.Unlock();

                    std::map<std::string, CxUtils::Point3D::List>::iterator lidar;

                    // Write LIDAR Data
                    // Write common data.
                    fprintf(ginfo->mRangeSensorStream, "Log Time (UTC), %s,",
                            CxUtils::Time::GetUtcTime().ToString().c_str());

                    // Write number of lasers.
                    fprintf(ginfo->mRangeSensorStream, "%d,", (int)(pointData.size()));

                    for(lidar = pointData.begin();
                        lidar != pointData.end();
                        lidar++)
                    {
                        // Save LIDAR Name
                        fprintf(ginfo->mRangeSensorStream, "%s, %d, ", lidar->first.c_str(), (int)lidar->second.size());
                        CxUtils::Point3D::List::iterator point;
                        for(point = lidar->second.begin();
                            point != lidar->second.end();
                            point++)
                        {
                            fprintf(ginfo->mRangeSensorStream, "%0.4lf, %0.4lf, %0.4lf,", point->mX, point->mY, point->mZ);
                        }
                    }
                    fprintf(ginfo->mRangeSensorStream, "\n");

                    // Only this thread access the stream, so flush
                    // outside of the mutex.
                    fflush(ginfo->mRangeSensorStream);

                    lastLaserLogTimeMs = CxUtils::Time::GetUtcTimeMs();
                }
            }

        }
        else
        {
            // Close any open files, etc.
            if(ginfo->mRangeSensorStream)
            {
                fclose(ginfo->mRangeSensorStream);
                ginfo->mRangeSensorStream = NULL;
            }
        }

        CxUtils::SleepMs(1);
    }

    if(ginfo->mRangeSensorStream)
    {
        fclose(ginfo->mRangeSensorStream);
        ginfo->mRangeSensorStream = NULL;
    }
}


/* End of File */

