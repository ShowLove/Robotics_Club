//Simple test to see if everything works with the Hokuyo
#define HOKUYO_LOW_LEVEL_DEBUG
#include <laser/laserfactory.h>
//#include <sick/sick.h>
//#include <hokuyo/hokuyo.h>

#define DRAW

#ifdef DRAW
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <display/laserdisplay.h>
#endif

#include <cxutils/cxutils.h>
#include <string.h>

// Example implementation of a GenericLaser::Callback
class MessageCallback : public Zebulon::Laser::Callback
{
public:
    MessageCallback()
    {
    }
    ~MessageCallback()
    {
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Event called when new data is available, stored in a vector in 
    ///          polar coordinates.
    ///
    ///   The coordinate frame for a laser is as follows: X positive in front, Y
    ///   positive to the right, Z positive down.  A positive Z rotation is a
    ///   rotation to the right, a positive Y rotation is a tilt up.
    ///   A Z value of 0.0 is the front of the laser, 90 degrees the right, etc.
    ///
    ///   \param[in] scan Laser scan data in polar coordinates relative to the
    ///                   laser.  Values are as follows: X = range/distance in
    ///                   meters, Y = pitch [-pi/2, pi/2], Z = yaw [-PI,PI].
    ///                   values are in order from left to right.
    ///   \param[in] timestamp Time when the data was captured (UTC).
    ///   \param[in] id Laser sensor ID.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual void ProcessLaserData(const CxUtils::Point3D::List& scan,
                                  const CxUtils::Time& timestamp,
                                  const unsigned int id)
    {
        //std::cout << "Scan Received at: " << timestamp.ToString() << std::endl;
        mMutex.Lock();
        mScan = scan;
        mMutex.Unlock();
    }
    CxUtils::Mutex mMutex;            ///<  For thread safe OpenCV drawing!
    CxUtils::Point3D::List mScan;     ///<  Scan data.
};

int main()
{
    Zebulon::Laser::GenericLaser::Parameters params;
    
    
    //#define USE_SICK
    
    params.mTries = 1;
    
    //#ifdef USE_SICK
    //params.mSerialPort = "/dev/ttyUSB1";
    //params.mBaudRate = 38400;
    //Zebulon::Laser::GenericLaser* laser = Zebulon::Laser::LaserFactory::CreateLaser("SICK");
    //#else
    params.mSerialPort = "/dev/ttyACM0";
    params.mBaudRate = 115200;
    params.mTries = 3;
    //Zebulon::Laser::GenericLaser* laser = Zebulon::Laser::LaserFactory::CreateLaser("Hokuyo");
    Zebulon::Laser::GenericLaser* laser = Zebulon::Laser::LaserFactory::CreateLaser("HokuyoURG");
    //#endif
    
    
    MessageCallback callback;
    laser->RegisterCallback(&callback);
    
    // Initialize the Laser.
    if(laser->Initialize(&params) == false)
    {
        std::cout << "Failed to initialize laser!\n";
        return 0;
    }
    
    // Creae an image to display scan data.
    #ifdef DRAW
    Zebulon::Utility::LaserDisplay laserDisplay;
    laserDisplay.Setup("Laser Display", 4, 640);
    #endif
    
    while(1)
    {
        #ifdef DRAW
        CxUtils::Point3D::List scan;
        callback.mMutex.Lock();
        scan = callback.mScan;
        callback.mMutex.Unlock();
        
        laserDisplay.Clear();
        laserDisplay.AddScan(scan);
        laserDisplay.Display();
        
        int c = cvWaitKey(33);
        if(c>0)
        {
            printf("%d\n",c);
            /*if(c == 1048681)
            {
                pixelsPerMeter*=1.25;
            }
            if(c == 1048687)
            {
                pixelsPerMeter/=1.25;
            }
            printf("PPM: %lf\n",pixelsPerMeter);
             */
        }
        
        /*if(c == 27)
        {
            // Exit
            break;
        }*/
        
        if ( (cvWaitKey(33) & 255) == 27 ) break;
        #endif              
    }
    
    laser->Shutdown();
    delete laser;
    return 0;
}

/* End of File */
