#include <cvcapture/cvimagecapture.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <cxutils/mutex.h>
#include <cxutils/math/cxmath.h>

#include <statemachine/objectscan.h>
#include <math.h>

#ifdef WIN32
#include "vld.h"
#endif


#include <stdio.h>
#include <vector>
#include <string>
#include <time.h>

using namespace std;
using namespace Zebulon;

#include "gvision/visionutil.h"

#include "gvision/visionproj.h"
#include <laser/laserfactory.h>

IplImage *displayImage=NULL;

CxUtils::Mutex displayMutex;

#define InchToMeter(x) ((x)/39.37)
#define MeterToInch(x) (round((x)*39.37))

int distanceThreshold;
int derivativeThreshold;
int maxDistance;
int minAngle;
int maxAngle;


//need to fix, points in forward right down (XYZ) rules

int main(int argc, char *argv[])
{
    
    Vision::VisionProj proj;
    
    AI::ObjectScan os;
    
    proj.AddPoints(CxUtils::Point3D(0,15,0),CxUtils::Point3D(480,360,0));
    proj.AddPoints(CxUtils::Point3D(12,15,0),CxUtils::Point3D(480,720,0));
    proj.AddPoints(CxUtils::Point3D(88,195,0),CxUtils::Point3D(0,720,0));
    proj.AddPoints(CxUtils::Point3D(0,195,0),CxUtils::Point3D(0,360,0));
    proj.Calculate();
    
    //return 0;
    int usecamera=0;

    cvNamedWindow("adj", CV_WINDOW_AUTOSIZE);

    cvCreateTrackbar("Distance (in)","adj",&distanceThreshold,500,NULL);
    cvCreateTrackbar("Derivative (in)","adj",&derivativeThreshold,500,NULL);
    cvCreateTrackbar("Max (in)","adj",&maxDistance,500,NULL);
    cvCreateTrackbar("Min Angle (deg)","adj",&minAngle,135,NULL);
    cvCreateTrackbar("Max Angle (deg)","adj",&maxAngle,135,NULL);

    cvSetTrackbarPos("Distance (in)","adj",MeterToInch(0.33));
    cvSetTrackbarPos("Derivative (in)","adj",MeterToInch(0.5));
    cvSetTrackbarPos("Max (in)","adj",MeterToInch(10.0));
    cvSetTrackbarPos("Min Angle (deg)","adj",135);
    cvSetTrackbarPos("Max Angle (deg)","adj",135);

    Video::CvImageCapture capture;
    Video::CvImageCapture::Image src;
    std::vector<std::string> sources;
    
    Laser::GenericLaser *laser;
    
    //laser=Laser::LaserFactory::CreateLaser("Hokuyo");
    //laser->Initialize("/dev/ttyACM");
    
    Zebulon::Laser::GenericLaser::Parameters params;
    laser=Laser::LaserFactory::CreateLaser("SICK");
    params.mSerialPort = "/dev/ttyUSB0";
    params.mBaudRate = 38400;

    // Connect to and initialize the laser.
    if(laser)
    {
        if(laser->Initialize(&params))
        {
            std::cout << "Laser Initialized!\n";
        }
        else
        {
            std::cout << "could not be initialized.\n";
            return false;
        }
    }
    else
    {
        std::cout << "Could not connect to laser!\n";
        return false;
    }
    
    //14in height
    //6 in in front
    CxUtils::Point3D LaserPosition( 0.1524, 0, 0.3556);
    //CxUtils::Point3D LaserPosition(0, 0, 0);
    CxUtils::Point3D LaserOrientation(0, 0, 0);

    bool valid;
    
    //Crappy way of skipping JAUS
    double MaxRange = laser->GetMaximumDistance();
    double MinRange = laser->GetMinimumDistance();
    double AngleIncrement = laser->GetAngularResolution();
    double ScanAngle = laser->GetScanAngle();
    //double UnitType = JAUS::RangeSensorConfig::CM;
    /*while(1)
    {
        printf("Metric: %lf %lf %lf\n",InchToMeter(distanceThreshold),InchToMeter(derivativeThreshold),InchToMeter(maxDistance));        
        int c=cvWaitKey(10);
    }*/
    cvNamedWindow("normal", CV_WINDOW_AUTOSIZE);

    printf("Press <enter> key to exit. Press the <c> key to save picture\n");
    printf("Initializing Image Capture...\n");
    if(argc > 1)
    {
        if(argc > 2)
        {
            capture.Start(argv[1], 0, 0, false, false);
        }
        else
        {
            capture.Start(argv[1], 0, 0, false, true);
        }
        usecamera=1;
    }
    else
    {
        printf("Needs to have a camera\n");
    }
    if(usecamera)
    {
        if(!capture.IsReady()) {

            printf("Unable to start capture\n");
            return 0;
        }
        else {
            printf("Success!\n");
        }
    }
    if(usecamera==0)
    {
        displayImage=cvCreateImage(cvSize(720,480),IPL_DEPTH_8U, 3);
    }


    while(1)
    {
        if(usecamera)
        {
            capture.GetFrame(&src);
            CvConvertToIplImage(src, &displayImage);
        }
        else
        {
            memset(displayImage->imageData,0,displayImage->imageSize);
        }

            //Stolen from Range DATA
            // Convert the data using the configuration info.
                        //ReportLocalRangeScan::Scan::const_iterator v;
                        //const ReportLocalRangeScan::Scan* ptr = report->GetScan();
                        
                        
                        CxUtils::Point3D::List lscan=laser->GetData();
                        
                        //printf("%d\n",lscan.size());
                        CxUtils::Point3D::List scanPolar;
                        CxUtils::Point3D::List scan;
                        
                        //right to left scan, correct? different than JAUS
                        
                        
                        //negative for hokuyo
                        //double angle = ScanAngle/2.0;
                        //double incr = -AngleIncrement;
                        //SICK
                        double angle = -ScanAngle/2.0;
                        double incr = AngleIncrement;
                        
                        //already in meters
                        double divider=1.0;

                        CxUtils::Quaternion rotX, rotY, rotZ, final;
                        rotX.SetRotationX(LaserOrientation.mX);
                        rotY.SetRotationY(LaserOrientation.mY);
                        rotZ.SetRotationZ(LaserOrientation.mZ);

                        final = rotX*rotY*rotZ;

                        bool first = true;
                        double pitch = 0.0;

                        for(int i=0;i<lscan.size();i++)
                        {
                            CxUtils::Point3D point((lscan[i])/divider);  // Convert to meters.
                            CxUtils::Point3D polar(point.mX);
                            
                            // Convert to cartesian coordinates, relative to
                            // the sensor.
                            CxUtils::Quaternion rotation;
                            rotation.SetRotationZ(angle);
                            point = rotation.Rotate(point);
                            
                            // Now translate and rotate relative to the
                            // platform frame.
                            point = final.Rotate(point);
                            point += LaserPosition;   
                            
                            // Now add to final scan.
                            scan.push_back(point);
                            // If first
                            if(first)
                            {
                                pitch = atan2(point.mZ, 1.0);
                                first = false;
                            }
                            polar.mY = pitch;
                            polar.mZ = angle;
                            scanPolar.push_back(polar);

                            angle += incr;
                        }
                        
                        
                        //cvCircle(displayImage, cvPoint(360,240), 1, cvScalar(255,0,0), 2);
                        for(int i=0;i<scan.size();i++)
                        {
                            double scale=100.0;
                            cvCircle(displayImage, cvPoint(360+scale*scan[i].mY,240-scale*scan[i].mX), 1, cvScalar(0,255,0), 1);
                        }
                        //os.Setup(LaserPosition,.33,.5,10.0);
                        printf("Metric: %lf %lf %lf\n",InchToMeter(distanceThreshold),InchToMeter(derivativeThreshold),InchToMeter(maxDistance));        
                        
                        
                        os.Setup(InchToMeter(distanceThreshold),InchToMeter(derivativeThreshold),InchToMeter(maxDistance));
            
                        //os.CalculateCartesian(scan);
                        printf("%d %d\n",-minAngle,maxAngle);
                        os.CalculateCartesian(scan,180,-minAngle,maxAngle);
                        
                        CxUtils::Point3D::List objects=os.GetObjects(valid,AI::ObjectScan::CLOSEST);
                        
                        if(valid)
                        {
                            printf("Front: %lf Left: %lf Size: %lf\n",objects[0].mX,objects[0].mY,objects[0].mZ);
                        }
                        
                        for(int i=0;i<objects.size();i++)
                        {
                            double scale=100.0;
                            //Hokuyo
                            //double circleScale=1.0/25.0;
                            //SICK
                            double circleScale=3.0/25.0;
                            if(i==0)
                            {
                                cvCircle(displayImage, cvPoint(360+scale*objects[i].mY,240-scale*objects[i].mX), objects[i].mZ*circleScale, cvScalar(255,0,0), objects[i].mZ*circleScale);
                            }
                            else
                            {
                                cvCircle(displayImage, cvPoint(360+scale*objects[i].mY,240-scale*objects[i].mX), objects[i].mZ*circleScale, cvScalar(0,0,255), objects[i].mZ*circleScale);
                            }
                        }
                        
                        /*CxUtils::Point3D tmp=scan[scan.size()/2]*39.37;
                        tmp.Print();
                        CxUtils::Point3D pix=proj.MetersToPixels(tmp);
                        //pix.Print();
                        CxUtils::Point3D tmp2=proj.PixelsToMeters(pix);
                        tmp2.Print();
                        
                        printf("\n");*/
                        /*
                        for(int i=0;i<scan.size();i++)
                        {
                            CxUtils::Point3D tmp=scan[i]*39.37;
                            CxUtils::Point3D pix=proj.MetersToPixels(tmp);

                            cvCircle(displayImage, cvPoint(pix.mY,pix.mX), 2, cvScalar(255,0,0), 2);
                        }
                        */
                        
                        //cvCircle(displayImage, cvPoint(100,100), 10, cvScalar(0,255,0), 2);
                        cvShowImage("normal",displayImage);
        
        
        
        int c=cvWaitKey(10);
        
        if(c>0)
        {
            switch(c)
            {

            }
            printf("Key: %d\n",c);
        }
        
    }

    cvDestroyAllWindows();

    return 0;
}
