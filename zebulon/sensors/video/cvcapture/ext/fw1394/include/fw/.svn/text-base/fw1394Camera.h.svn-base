/****************************************************************************
 *
 *****************************************************************************/

/*!
  \file fw1394Camera.h
  \brief class for firewire cameras video capture using libdc1394-2.x api

  \warning This class needs libdc1394-2 and
  libraw1394 (version greater than 1.1.0). These libraries are available from
  http://sourceforge.net/projects/libdc1394 and
  http://sourceforge.net/projects/libraw1394 .

  fw1394Camera was tested with libdc1394-2.0.0 (svn version) and
  with the following cameras:
  - Marlin F033C
  - Marlin F131B
  - APPLE iSight extern webcam

  \ingroup libdevice
*/

#ifndef fw1394Camera_h
#define fw1394Camera_h

#include <fw/fwConfig.h>

#if defined(FW_HAVE_DC1394_2)

/*!
  \class fw1394Camera
  \brief class for firewire ieee1394 video devices using libdc1394-2.x api.

  Needs libraw1394-1.2.0 and libdc1394-2.0.0 or more recent versions
  available on http://sourceforge.net.

  This class was tested with Marlin F033C and F131B cameras and with
  Point Grey Dragonfly 2 camera.

  \ingroup libdevice

  This grabber allows single or multi camera acquisition. An example
  of a single camera acquisition is given in fw1394Camera(). An
  example of multi camera acquisition is available in setCamera().

  - Here an example of single capture from the first camera found on the bus::
  \code
  fwImage<unsigned char> I;
  fw1394Camera g;
  g.setVideoMode(fw1394Camera::fwVIDEO_MODE_640x480_MONO8);
  g.setFramerate(fw1394Camera::fwFRAMERATE_60);
  while(1)
    g.acquire(I);
  \endcode

  - Here an example of multi camera capture:
  \code
  unsigned int ncameras; // Number of cameras on the bus
  fw1394Camera g;
  g.getNumCameras(ncameras);
  fwImage<unsigned char> *I = new fwImage<unsigned char> [ncameras];

  // If the first camera supports fwVIDEO_MODE_640x480_YUV422 video mode
  g.setCamera(0);
  g.setVideoMode(fw1394Camera::fwVIDEO_MODE_640x480_YUV422);

  // If the second camera support 30 fps acquisition
  g.setCamera(1);
  g.setFramerate(fw1394Camera::fwFRAMERATE_30);

  while(1) {
    for (unsigned int camera=0; camera < ncameras; camera ++) {
      // Acquire alternatively images from camera 0 and from camera 1
      g.setCamera(camera);
      g.acquire(I[camera]);
    }
  }
  delete [] I;
  \endcode


  \author  Fabien Spindler (Fabien.Spindler@irisa.fr), Irisa / Inria Rennes

*/

/*
 * Interface with libdc1394 2.x
 */

#include <dc1394/control.h>
#include <dc1394/utils.h>

//#include <fw/fwImage.h>
#include <fw/fwCamera.h>
#include <fw/fwList.h>

#include <pthread.h>

#define CLAMPCONV(x) (unsigned char) ( (x<0)? 0: ( (x>255)? 255: x))

class FW_EXPORT fw1394Camera : public fwCamera
{

 public:
  static const int NUM_BUFFERS; // Number of buffers
  static const char * strVideoMode[DC1394_VIDEO_MODE_NUM];
  static const char * strFramerate[DC1394_FRAMERATE_NUM];
  static const char * strColorCoding[DC1394_COLOR_CODING_NUM];
  static pthread_mutex_t         mutex;

  /*!
    Enumeration of video modes. See libdc1394 2.x header file
    dc1394/control.h
  */
  typedef enum {
    fwVIDEO_MODE_160x120_YUV444   = DC1394_VIDEO_MODE_160x120_YUV444,
    fwVIDEO_MODE_320x240_YUV422   = DC1394_VIDEO_MODE_320x240_YUV422,
    fwVIDEO_MODE_640x480_YUV411   = DC1394_VIDEO_MODE_640x480_YUV411,
    fwVIDEO_MODE_640x480_YUV422   = DC1394_VIDEO_MODE_640x480_YUV422,
    fwVIDEO_MODE_640x480_RGB8     = DC1394_VIDEO_MODE_640x480_RGB8,
    fwVIDEO_MODE_640x480_MONO8    = DC1394_VIDEO_MODE_640x480_MONO8,
    fwVIDEO_MODE_640x480_MONO16   = DC1394_VIDEO_MODE_640x480_MONO16,
    fwVIDEO_MODE_800x600_YUV422   = DC1394_VIDEO_MODE_800x600_YUV422,
    fwVIDEO_MODE_800x600_RGB8     = DC1394_VIDEO_MODE_800x600_RGB8,
    fwVIDEO_MODE_800x600_MONO8    = DC1394_VIDEO_MODE_800x600_MONO8,
    fwVIDEO_MODE_1024x768_YUV422  = DC1394_VIDEO_MODE_1024x768_YUV422,
    fwVIDEO_MODE_1024x768_RGB8    = DC1394_VIDEO_MODE_1024x768_RGB8,
    fwVIDEO_MODE_1024x768_MONO8   = DC1394_VIDEO_MODE_1024x768_MONO8,
    fwVIDEO_MODE_800x600_MONO16   = DC1394_VIDEO_MODE_800x600_MONO16,
    fwVIDEO_MODE_1024x768_MONO16  = DC1394_VIDEO_MODE_1024x768_MONO16,
    fwVIDEO_MODE_1280x960_YUV422  = DC1394_VIDEO_MODE_1280x960_YUV422,
    fwVIDEO_MODE_1280x960_RGB8    = DC1394_VIDEO_MODE_1280x960_RGB8,
    fwVIDEO_MODE_1280x960_MONO8   = DC1394_VIDEO_MODE_1280x960_MONO8,
    fwVIDEO_MODE_1600x1200_YUV422 = DC1394_VIDEO_MODE_1600x1200_YUV422,
    fwVIDEO_MODE_1600x1200_RGB8   = DC1394_VIDEO_MODE_1600x1200_RGB8,
    fwVIDEO_MODE_1600x1200_MONO8  = DC1394_VIDEO_MODE_1600x1200_MONO8,
    fwVIDEO_MODE_1280x960_MONO16  = DC1394_VIDEO_MODE_1280x960_MONO16,
    fwVIDEO_MODE_1600x1200_MONO16 = DC1394_VIDEO_MODE_1600x1200_MONO16,
    fwVIDEO_MODE_EXIF      = DC1394_VIDEO_MODE_EXIF,
    fwVIDEO_MODE_FORMAT7_0 = DC1394_VIDEO_MODE_FORMAT7_0,
    fwVIDEO_MODE_FORMAT7_1 = DC1394_VIDEO_MODE_FORMAT7_1,
    fwVIDEO_MODE_FORMAT7_2 = DC1394_VIDEO_MODE_FORMAT7_2,
    fwVIDEO_MODE_FORMAT7_3 = DC1394_VIDEO_MODE_FORMAT7_3,
    fwVIDEO_MODE_FORMAT7_4 = DC1394_VIDEO_MODE_FORMAT7_4,
    fwVIDEO_MODE_FORMAT7_5 = DC1394_VIDEO_MODE_FORMAT7_5,
    fwVIDEO_MODE_FORMAT7_6 = DC1394_VIDEO_MODE_FORMAT7_6,
    fwVIDEO_MODE_FORMAT7_7 = DC1394_VIDEO_MODE_FORMAT7_7
  } fw1394TwoVideoModeType;

  /*!
    Enumeration of framerates. See libdc1394 2.x header file
    dc1394/control.h
  */
  typedef enum {
    fwFRAMERATE_1_875 = DC1394_FRAMERATE_1_875,
    fwFRAMERATE_3_75  = DC1394_FRAMERATE_3_75,
    fwFRAMERATE_7_5   = DC1394_FRAMERATE_7_5,
    fwFRAMERATE_15    = DC1394_FRAMERATE_15,
    fwFRAMERATE_30    = DC1394_FRAMERATE_30,
    fwFRAMERATE_60    = DC1394_FRAMERATE_60,
    fwFRAMERATE_120   = DC1394_FRAMERATE_120,
    fwFRAMERATE_240   = DC1394_FRAMERATE_240
  } fw1394TwoFramerateType;

  /*!
    Enumeration of color codings. See libdc1394 2.x header file
    dc1394/control.h
  */
  typedef enum {
    fwCOLOR_CODING_MONO8   = DC1394_COLOR_CODING_MONO8,
    fwCOLOR_CODING_YUV411  = DC1394_COLOR_CODING_YUV411,
    fwCOLOR_CODING_YUV422  = DC1394_COLOR_CODING_YUV422,
    fwCOLOR_CODING_YUV444  = DC1394_COLOR_CODING_YUV444,
    fwCOLOR_CODING_RGB8    = DC1394_COLOR_CODING_RGB8,
    fwCOLOR_CODING_MONO16  = DC1394_COLOR_CODING_MONO16,
    fwCOLOR_CODING_RGB16   = DC1394_COLOR_CODING_RGB16,
    fwCOLOR_CODING_MONO16S = DC1394_COLOR_CODING_MONO16S,
    fwCOLOR_CODING_RGB16S  = DC1394_COLOR_CODING_RGB16S,
    fwCOLOR_CODING_RAW8    = DC1394_COLOR_CODING_RAW8,
    fwCOLOR_CODING_RAW16   = DC1394_COLOR_CODING_RAW16
  } fw1394TwoColorCodingType;


 public:
  fw1394Camera(unsigned int camera_id);
  virtual ~fw1394Camera();

  void setCamera(unsigned int camera);
  void getCamera(unsigned int &camera);
  void getNumCameras(unsigned int &ncameras);

  void setVideoMode(fw1394TwoVideoModeType videomode);
  void getVideoMode(fw1394TwoVideoModeType & videomode);
  int  getVideoModeSupported(fwList<fw1394TwoVideoModeType> & videomodes);
  bool isVideoModeFormat7(fw1394TwoVideoModeType videomode);
  bool isColor();

  void setFramerate(fw1394TwoFramerateType fps);
  void getFramerate(fw1394TwoFramerateType & fps);
  int  getFramerateSupported(fw1394TwoVideoModeType videomode,
			     fwList<fw1394TwoFramerateType> & fps);

  void setColorCoding(fw1394TwoColorCodingType coding);
  void getColorCoding(fw1394TwoColorCodingType & coding);
  int  getColorCodingSupported(fw1394TwoVideoModeType videomode,
			       fwList<fw1394TwoColorCodingType> & codings);

  void setFormat7ROI(unsigned int left=0, unsigned int top=0,
		     unsigned int width=0, unsigned int height=0);

  void printCameraInfo();

  dc1394video_frame_t *dequeue();
  void enqueue(dc1394video_frame_t *frame);

  void acquire(unsigned char *I);

  void setFeature(unsigned int gain,dc1394feature_t feature);
  void getFeature(unsigned int &min_gain,
	       unsigned int &gain,
	       unsigned int &max_gain,
           dc1394feature_t feature);

  void getWidth(unsigned int &width);
  void getHeight(unsigned int &height);
  
  void enableAutoWhite(bool white);
  void doAutoWhiteBalance(unsigned char *image,int size);
  void SetWhiteBalance(int red,int blue);
  void GetWhiteBalance(int &red,int &blue);

  void enableAutoShutter(bool shutter);
  void doAutoShutter(unsigned char *image,int size);
  void SetShutter(int shutter);
  void GetShutter(int &shutter);
  void SetDesiredAvg(int avg);
  
  void doingYUV(bool val);
  
  void debugDisplay(bool display);
  
  void setPTerm(double p);
  

  void close();

public:
  static std::string videoMode2string(fw1394TwoVideoModeType videomode);
  static std::string framerate2string(fw1394TwoFramerateType fps);
  static std::string colorCoding2string(fw1394TwoColorCodingType colorcoding);
  static fw1394TwoVideoModeType string2videoMode(std::string videomode);
  static fw1394TwoFramerateType string2framerate(std::string fps);
  static fw1394TwoColorCodingType string2colorCoding(std::string colorcoding);

 private:
  void open();
  void setCapture(dc1394switch_t _switch);
  void setTransmission(dc1394switch_t _switch);

 private:
  dc1394camera_t *camera;
  unsigned int num_cameras;
  unsigned int camera_id;

private:
  bool verbose;
  bool *camInUse;
  
  static bool initFlag;
  static dc1394_t * d;
  dc1394camera_list_t * list;
  unsigned char *internalImage;

  bool balanceFlag;
  bool shutterFlag;
  int Shutter;
  int Blue;
  int Red;
  int Avg;
  double PTerm;
  bool blackFlag;
  int blackCount;
  int frameCount;
  
  int manualShutter;
  int manualWhite;
  
  bool displayFlag;
  bool yuvFlag;
  int cameraId;
  //void MakeTable();
  //unsigned char YUVTableR[256][256][256];
  //unsigned char YUVTableG[256][256][256];
  //unsigned char YUVTableB[256][256][256];
};


#endif
#endif

