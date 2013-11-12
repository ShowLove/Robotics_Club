/****************************************************************************
 *
 *****************************************************************************/


/*!
  \file fw1394Camera.cpp
  \brief member functions for firewire cameras
  \ingroup libdevice
*/
#include <iostream>

#include <fw/fwConfig.h>

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

/*
 * Interface with libdc1394 2.x
 */
#if defined(FW_HAVE_DC1394_2)

#include <fw/fw1394Camera.h>
#include <fw/fwCameraException.h>
#include <fw/fwTime.h>


dc1394_t * fw1394Camera::d = NULL;
bool fw1394Camera::initFlag = false;

const int fw1394Camera::NUM_BUFFERS = 1; /*!< Number of buffers */
pthread_mutex_t         fw1394Camera::mutex = PTHREAD_MUTEX_INITIALIZER;

const char * fw1394Camera::strVideoMode[DC1394_VIDEO_MODE_NUM]= {
  "MODE_160x120_YUV444",
  "MODE_320x240_YUV422",
  "MODE_640x480_YUV411",
  "MODE_640x480_YUV422",
  "MODE_640x480_RGB8",
  "MODE_640x480_MONO8",
  "MODE_640x480_MONO16",
  "MODE_800x600_YUV422",
  "MODE_800x600_RGB8",
  "MODE_800x600_MONO8",
  "MODE_1024x768_YUV422",
  "MODE_1024x768_RGB8",
  "MODE_1024x768_MONO8",
  "MODE_800x600_MONO16",
  "MODE_1024x768_MONO16",
  "MODE_1280x960_YUV422",
  "MODE_1280x960_RGB8",
  "MODE_1280x960_MONO8",
  "MODE_1600x1200_YUV422",
  "MODE_1600x1200_RGB8",
  "MODE_1600x1200_MONO8",
  "MODE_1280x960_MONO16",
  "MODE_1600x1200_MONO16",
  "MODE_EXIF",
  "MODE_FORMAT7_0",
  "MODE_FORMAT7_1",
  "MODE_FORMAT7_2",
  "MODE_FORMAT7_3",
  "MODE_FORMAT7_4",
  "MODE_FORMAT7_5",
  "MODE_FORMAT7_6",
  "MODE_FORMAT7_7"
};

const char * fw1394Camera::strFramerate[DC1394_FRAMERATE_NUM]= {
  "FRAMERATE_1_875",
  "FRAMERATE_3_75",
  "FRAMERATE_7_5",
  "FRAMERATE_15",
  "FRAMERATE_30",
  "FRAMERATE_60",
  "FRAMERATE_120",
  "FRAMERATE_240"
};

const char * fw1394Camera::strColorCoding[DC1394_COLOR_CODING_NUM]= {
  "COLOR_CODING_MONO8",
  "COLOR_CODING_YUV411",
  "COLOR_CODING_YUV422",
  "COLOR_CODING_YUV444",
  "COLOR_CODING_RGB8",
  "COLOR_CODING_MONO16",
  "COLOR_CODING_RGB16",
  "COLOR_CODING_MONO16S",
  "COLOR_CODING_RGB16S",
  "COLOR_CODING_RAW8",
  "COLOR_CODING_RAW16",
};



/*!
  Default constructor.

  By default:
  - the camera is the first found on the bus.

  Current camera settings can be changed using setCamera() to select the active
  camera on the bus and than setVideoMode() or setFramerate() to fix the active
  camera settings. The list of supported video modes and framerates is
  available using respectively getVideoModeSupported() and
  getFramerateSupported().

  \code
  fwImage<unsigned char> I;
  fw1394Camera g;
  g.setVideoMode(fw1394Camera::fwVIDEO_MODE_640x480_MONO8);
  g.setFramerate(fw1394Camera::fwFRAMERATE_15);
  while(1)
    g.acquire(I);
  \endcode

  \sa setCamera(), setVideoMode(), setFramerate()

*/

fw1394Camera::fw1394Camera(unsigned int camera_id)
{
  // protected members
  width = height = 0;
  cameraId=camera_id;
  // private members
  camera = NULL;
  verbose = true;
  camInUse = NULL;
  internalImage=NULL;
  
  list = NULL;
  init = false;
  
  PTerm=0.125;
  
  //Magic factory color values -gary
  Blue=95;
  Red=87;
  Shutter=2000;
  frameCount=0;
  Avg=128;
  
  displayFlag=false;
  
  manualShutter=0;
  manualWhite=0;

  blackFlag=false;
  blackCount=0;

  // static class members
  //dc1394_t * fw1394Camera::d = NULL;
  balanceFlag=false;
  yuvFlag=false;
  shutterFlag=false;

  // one time only
  if (fw1394Camera::d == NULL)
{
    //system call hack
    //system("dc1394_reset_bus");
    //stop it from displaying to the screen
    //system("dc1394_reset_all > /dev/null 2> /dev/null");
    
    usleep(1000000);
    fw1394Camera::d = dc1394_new ();
    initFlag=true;
}
else
{
    while(initFlag==false)
    {
        printf("Waiting for init...\n");
        usleep(1000000);
    }
}
    printf("Camera %d Ready\n",camera_id);

  if (dc1394_camera_enumerate (fw1394Camera::d, &list) != DC1394_SUCCESS) {
    dc1394_camera_free_list (list);
    close();
    fwERROR_TRACE("Failed to enumerate cameras\n");
    throw (fwCameraException(fwCameraException::initializationError,
            "Failed to enumerate cameras") );
  }

  if (list->num == 0) {
    dc1394_camera_free_list (list);
    close();
    fwERROR_TRACE("No cameras found");
    throw (fwCameraException(fwCameraException::initializationError,
            "No cameras found") );
  }
  
  //reset everyone

std::cout << "camera_id " << camera_id << std::endl;
std::cout << "list->num " << list->num << std::endl;

  if (camera_id >= list->num) {
    dc1394_camera_free_list (list);
    close();
    fwERROR_TRACE("No cameras found");
    throw (fwCameraException(fwCameraException::initializationError,
            "Bad camera id") );
  }

  //printf("GUID: %d UNIT: %d\n",list->ids[camera_id].guid,list->ids[camera_id].unit);
  camera = dc1394_camera_new (fw1394Camera::d, list->ids[camera_id].guid);
  if (!camera) {
    fwERROR_TRACE ("Failed to initialize camera with guid \"%ld\"\n",
         list->ids[camera_id].guid);
    throw (fwCameraException(fwCameraException::initializationError,
            "No camera") );
  }
  
  if (verbose) {
    std::cout << "------ Bus information ------" << std::endl;
    std::cout << "Number of camera(s) on the bus : " << list->num <<std::endl;
    std::cout << "-----------------------------" << std::endl;
  }

  if (list != NULL)
    dc1394_camera_free_list (list);
  list = NULL;

  //Can mess up using multiple cameras
  //dc1394_reset_bus(camera);

  dc1394switch_t status = DC1394_OFF;

  dc1394_video_get_transmission(camera, &status);
  if (status != DC1394_OFF){
    if (dc1394_video_set_transmission(camera,DC1394_OFF)!=DC1394_SUCCESS)
      fwTRACE("Could not stop ISO transmission");
    else {
      fwTime::wait(500);
      if (dc1394_video_get_transmission(camera, &status)!=DC1394_SUCCESS)
          fwTRACE("Could get ISO status");
      else {
          if (status==DC1394_ON) {
            fwTRACE("ISO transmission refuses to stop");
          }
      }
    }
  }

  //SETUP FOR 1394B
  dc1394_video_set_operation_mode(camera,DC1394_OPERATION_MODE_1394B);
  dc1394_video_set_iso_speed(camera, DC1394_ISO_SPEED_800);
  //SETUP for old firewire
  //dc1394_video_set_iso_speed(camera, DC1394_ISO_SPEED_400);
  setCapture(DC1394_ON);
  setTransmission(DC1394_ON);

  //Auto features completly different
  dc1394_feature_set_mode(camera,DC1394_FEATURE_BRIGHTNESS,DC1394_FEATURE_MODE_AUTO);
  
  //dc1394_feature_set_mode(camera,DC1394_FEATURE_EXPOSURE,DC1394_FEATURE_MODE_MANUAL);
  dc1394_feature_set_mode(camera,DC1394_FEATURE_EXPOSURE,DC1394_FEATURE_MODE_AUTO);
  
  //Originally assume Camera Auto White Balance
  dc1394_feature_set_mode(camera,DC1394_FEATURE_WHITE_BALANCE,DC1394_FEATURE_MODE_AUTO);
  
  
  //testing
  dc1394_feature_set_mode(camera,DC1394_FEATURE_SHUTTER,DC1394_FEATURE_MODE_AUTO);
  //enableAutoWhite(true);
  //dc1394_feature_set_mode(camera,DC1394_FEATURE_SHUTTER,DC1394_FEATURE_MODE_MANUAL);

  //dc1394_feature_whitebalance_set_value(camera,45,150); //50-150
  //No longer modify?
  //dc1394_feature_set_value(camera,DC1394_FEATURE_EXPOSURE,256);
  //enableAutoShutter(true);

  init = true;
}

/*!

  Destructor.

  Close the firewire grabber.

  \sa close()

*/
fw1394Camera::~fw1394Camera()
{
  close();
}


/*!

  Return the number of cameras connected on the bus.

  \param ncameras : The number of cameras found on the bus.


*/
void
fw1394Camera::getNumCameras(unsigned int &ncameras)
{
  ncameras = num_cameras;
}

/*!

  Set the camera video capture mode. Image size is than updated with respect to
  the new video capture mode.

  The iso transmission (setTransmission()) and the dma capture (see
  setCapture()) are first stopped. Then, the camera video capture mode is
  set. Finaly, the dma capture and the iso transmission are re-started.

  \param videomode : The camera video capture mode. The current camera mode is
  given by getVideoMode(). The camera supported modes are given by
  getVideoModeSupported().

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \exception fwCameraException::settingError : If we can't set
  the video mode.

  \sa getVideoMode(), getVideoModeSupported(), setCamera()

*/
void
fw1394Camera::setVideoMode(fw1394TwoVideoModeType videomode)
{
   // Stop dma capture if started
  setTransmission(DC1394_OFF);
  setCapture(DC1394_OFF);

  if (dc1394_video_set_mode(camera, (dc1394video_mode_t) videomode) != DC1394_SUCCESS) {

    close();
    fwERROR_TRACE("Can't set video mode");
    throw (fwCameraException(fwCameraException::settingError,
                   "Can't set video mode") );
    }

  setCapture(DC1394_ON);
  setTransmission(DC1394_ON);

  // Updates image size from new video mode
  if (dc1394_get_image_size_from_video_mode(camera,
                        (dc1394video_mode_t) videomode,
                        &this->width, &this->height)
      != DC1394_SUCCESS) {

    close();
    fwERROR_TRACE("Can't set video mode");
    throw (fwCameraException(fwCameraException::settingError,
                   "Can't get image size") );
  }

}

/*!

  Query the actual capture video mode of the active camera. All
  the active camera supported modes are given by getVideoModeSupported().

  \param videomode : The camera capture video mode.

  \exception fwCameraException::initializationError : If the
  required camera is not present.

  \exception fwCameraException::settingError : If we can't get
  the camera actual video mode.

  \sa setVideoMode(), getVideoModeSupported(), setCamera()

*/
void
fw1394Camera::getVideoMode(fw1394TwoVideoModeType & videomode)
{
  dc1394video_mode_t _videomode;
  if (dc1394_video_get_mode(camera, &_videomode) != DC1394_SUCCESS) {

    close();
    fwERROR_TRACE("Can't get current video mode");
    throw (fwCameraException(fwCameraException::settingError,
                   "Can't get current video mode") );
  }
  videomode = (fw1394TwoVideoModeType) _videomode;

}



/*!

  Query the available active camera video modes.


  \param videomodes : The list of supported camera video modes.

  \return The number of supported camera modes, 0 if an error occurs.

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \exception fwCameraException::settingError : If we can't get
  video modes.

  \sa setVideoMode(), getVideoMode(), getCamera()
*/
int
fw1394Camera::getVideoModeSupported(fwList<fw1394TwoVideoModeType> & videomodes)
{
  // Refresh the list of supported modes
  videomodes.kill();

  dc1394video_modes_t _videomodes;

  // get video modes:
  if (dc1394_video_get_supported_modes(camera, &_videomodes)!=DC1394_SUCCESS) {

    close();
    fwERROR_TRACE("Can't get video modes");
    throw (fwCameraException(fwCameraException::settingError,
                   "Can't get video modes") );
  }

  // parse the video modes to add in the list
  for (unsigned i=0; i < _videomodes.num; i++) {
    fw1394TwoVideoModeType _mode = (fw1394TwoVideoModeType) _videomodes.modes[i];
    videomodes.addRight( _mode );
  }

  // return the number of available video modes
  return _videomodes.num;
}

/*!

  Indicates if the video mode is format 7.

  \return true : If the video mode is scalable (Format 7).
  \return false : If the video mode is not Format 7 like.

  \sa setVideoMode(), getVideoModeSupported(), setCamera()

*/
bool
fw1394Camera::isVideoModeFormat7(fw1394TwoVideoModeType  videomode)
{

  if (dc1394_is_video_mode_scalable((dc1394video_mode_t) videomode))
    return true;

  return false;
}

/*!

  Indicates if the active camera is grabbing color or grey images.

  We consider color images if the color coding is either YUV (411, 422, 444) or
  RGB (8, 16, 16S).  We consider grey images if the color coding is MONO (8,
  16, 16S) or RAW (8, 16). fw1394TwoColorCodingType gives the supported color
  codings.

  \return true : If color images are acquired.
  \return false : If grey images are acquired.

  \sa getColorCoding(), setCamera()

*/
bool
fw1394Camera::isColor()
{
  fw1394TwoColorCodingType coding;
  getColorCoding(coding);

  switch(coding) {
  case fwCOLOR_CODING_MONO8:
  case fwCOLOR_CODING_MONO16:
  case fwCOLOR_CODING_MONO16S:
  case fwCOLOR_CODING_RAW8:
  case fwCOLOR_CODING_RAW16:
    return false;
  case fwCOLOR_CODING_YUV411:
  case fwCOLOR_CODING_YUV422:
  case fwCOLOR_CODING_YUV444:
  case fwCOLOR_CODING_RGB8:
  case fwCOLOR_CODING_RGB16:
  case fwCOLOR_CODING_RGB16S:
    return true;
  }
  return false;
}

/*!

  Set the active camera framerate for non scalable video modes.

  The iso transmission (setTransmission()) and the dma capture (see
  setCapture()) are first stopped. Then, the camera framerate capture mode is
  set. Finaly, the dma capture and the iso transmission are re-started.

  If the current video mode is scalable (Format 7), this function is without
  effect.

  \param fps : The camera framerate. The current framerate of the camera is
  given by getFramerate(). The camera supported framerates are given by
  getFramerateSupported().

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \exception fwCameraException::settingError : If we can't set
  the framerate.

  \sa getFramerate(), getFramerateSupported() , setCamera()

*/
void
fw1394Camera::setFramerate(fw1394TwoFramerateType fps)
{
  fw1394TwoVideoModeType cur_videomode;
  getVideoMode(cur_videomode);
  if (isVideoModeFormat7(cur_videomode))
    return;

  // Stop dma capture if started
  setTransmission(DC1394_OFF);
  setCapture(DC1394_OFF);

  if (dc1394_video_set_framerate(camera, (dc1394framerate_t) fps) != DC1394_SUCCESS) {

    close();
    fwERROR_TRACE("Can't set framerate");
    throw (fwCameraException(fwCameraException::settingError,
                   "Can't set framerate") );
  }

  setCapture(DC1394_ON);
  setTransmission(DC1394_ON);
}

/*!

  Query the actual camera framerate of the active camera. The camera supported
  framerates are given by getFramerateSupported().

  \param fps : The camera capture framerate.

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \exception fwCameraException::settingError : If we can't get
  the framerate.

  \sa setFramerate(), getFramerateSupported(), setCamera()

*/
void
fw1394Camera::getFramerate(fw1394TwoFramerateType & fps)
{
  dc1394framerate_t _fps;
  if (dc1394_video_get_framerate(camera, &_fps) != DC1394_SUCCESS) {

    close();
    fwERROR_TRACE("Can't get current framerate");
    throw (fwCameraException(fwCameraException::settingError,
                   "Can't get current framerate") );
  }
  fps = (fw1394TwoFramerateType) _fps;

}

/*!

  Query the available framerates for the given camera video mode (see
  file dc1394/control.h). No framerate is associated to the following
  camera modes :

  - fw1394Camera::fwVIDEO_MODE_EXIF (format 6),
  - fw1394Camera::fwVIDEO_MODE_FORMAT7_0 (format 7):
  - fw1394Camera::fwVIDEO_MODE_FORMAT7_1 (format 7)
  - fw1394Camera::fwVIDEO_MODE_FORMAT7_2 (format 7)
  - fw1394Camera::fwVIDEO_MODE_FORMAT7_3 (format 7)
  - fw1394Camera::fwVIDEO_MODE_FORMAT7_4 (format 7)
  - fw1394Camera::fwVIDEO_MODE_FORMAT7_5 (format 7)
  - fw1394Camera::fwVIDEO_MODE_FORMAT7_6 (format 7)
  - fw1394Camera::fwVIDEO_MODE_FORMAT7_7 (format 7)

  \param mode : Camera video mode.

  \param fps : The list of supported camera framerates for the given camera
  video mode.

  \return The number of supported framerates, 0 if no framerate is available.

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \exception fwCameraException::settingError : If we can't get
  the supported framerates.

  \sa setFramerate(), getFramerate(), setCamera()
*/
int
fw1394Camera::getFramerateSupported(fw1394TwoVideoModeType mode,
                    fwList<fw1394TwoFramerateType> & fps)
{
  // Refresh the list of supported framerates
  fps.kill();

  switch(mode) {
    // Framerate not avalaible for:
    //  - fwVIDEO_MODE_EXIF ie Format_6
    //  - fwVIDEO_MODE_FORMAT7... ie the Format_7
  case fwVIDEO_MODE_EXIF:
  case fwVIDEO_MODE_FORMAT7_0:
  case fwVIDEO_MODE_FORMAT7_1:
  case fwVIDEO_MODE_FORMAT7_2:
  case fwVIDEO_MODE_FORMAT7_3:
  case fwVIDEO_MODE_FORMAT7_4:
  case fwVIDEO_MODE_FORMAT7_5:
  case fwVIDEO_MODE_FORMAT7_6:
  case fwVIDEO_MODE_FORMAT7_7:
    return 0;
    break;
  default:
    {
      dc1394framerates_t _fps;
      if (dc1394_video_get_supported_framerates(camera,
                        (dc1394video_mode_t)mode,
                        &_fps) != DC1394_SUCCESS) {
    close();
    fwERROR_TRACE("Could not query supported frametates for mode %d\n",
              mode);
    throw (fwCameraException(fwCameraException::settingError,
                       "Could not query supported framerates") );
      }
      if (_fps.num == 0)
    return 0;

      for (unsigned int i = 0; i < _fps.num; i ++)
    fps.addRight((fw1394TwoFramerateType)_fps.framerates[i]);

      return _fps.num;
    }
    break;
  }
}

/*!

  Set the active camera Format 7 color coding.

  The iso transmission (setTransmission()) and the dma capture (see
  setCapture()) are first stopped. Then, the active camera Format 7 is
  set. Finaly, the dma capture and the iso transmission are re-started.

  \warning Setting color coding for non format 7 video mode will be
  without effect.

  \param coding : The camera color coding for Format 7 video mode. The
  current color coding of the camera is given by getColorCoding(). The
  camera supported color codings are given by
  getColorCodingSupported().

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \exception fwCameraException::settingError : If we can't set
  the color coding for Format 7 video mode.

  \sa getColorCoding(), getColorCodingSupported() , setCamera()

*/
void
fw1394Camera::setColorCoding(fw1394TwoColorCodingType coding)
{
  dc1394video_mode_t _videomode;
  if (dc1394_video_get_mode(camera, &_videomode) != DC1394_SUCCESS) {

    close();
    fwERROR_TRACE("Can't get current video mode");
    throw (fwCameraException(fwCameraException::settingError,
                   "Can't get current video mode") );
  }

  // Format 7 video mode
  if (dc1394_is_video_mode_scalable(_videomode)) {
    setTransmission(DC1394_OFF);
    setCapture(DC1394_OFF);

    if (dc1394_format7_set_color_coding(camera, _videomode,
                      (dc1394color_coding_t) coding)
    != DC1394_SUCCESS) {

      close();
      fwERROR_TRACE("Can't set color coding");
      throw (fwCameraException(fwCameraException::settingError,
                     "Can't set color coding") );
    }

    setCapture(DC1394_ON);
    setTransmission(DC1394_ON);
  }
}

/*!

  Query the actual color coding of the active camera. The camera supported
  color codings are given by getColorCodingSupported().

  \param coding : The camera capture color coding.

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \exception fwCameraException::settingError : If we can't get
  the actual color coding. Occurs if current video mode is
  fw1394Camera::fwVIDEO_MODE_EXIF (format 6).

  \sa setColorCoding(), getColorCodingSupported(), setCamera()

*/
void
fw1394Camera::getColorCoding(fw1394TwoColorCodingType & coding)
{
  dc1394video_mode_t _videomode;
  while (dc1394_video_get_mode(camera, &_videomode) != DC1394_SUCCESS) {

    //close();
    printf("Camera Failure: %d\n",cameraId);
    fwERROR_TRACE("Can't get current video mode");
    //throw (fwCameraException(fwCameraException::settingError,
    //               "Can't get current video mode") );
    usleep(1000);
  }

  dc1394color_coding_t _coding;
  if (dc1394_is_video_mode_scalable(_videomode)) {
    // Format 7 video mode
    if (dc1394_format7_get_color_coding(camera, _videomode, &_coding)
    != DC1394_SUCCESS) {

      close();
      fwERROR_TRACE("Can't get current color coding");
      throw (fwCameraException(fwCameraException::settingError,
                     "Can't query current color coding") );
    }
  }
  else if (dc1394_is_video_mode_still_image((dc1394video_mode_t)_videomode)) {
      throw (fwCameraException(fwCameraException::settingError,
                     "No color coding for format 6 video mode"));
  }
  else {
    // Not Format 7 and not Format 6 video modes
    if (dc1394_get_color_coding_from_video_mode(camera,
                         (dc1394video_mode_t)_videomode,
                         &_coding) != DC1394_SUCCESS) {
      close();
      fwERROR_TRACE("Could not query supported color coding for mode %d\n",
            _videomode);
      throw (fwCameraException(fwCameraException::settingError,
                     "Can't query current color coding"));
    }
  }
  coding = (fw1394TwoColorCodingType) _coding;
}

/*!

  Query the available color codings for the given camera video mode (see
  file dc1394/control.h).

  \param mode : Camera video mode.

  \param codings : The list of supported color codings for the given camera
  video mode.

  \return The number of supported color codings, 0 if no color codings
  is available.

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \exception fwCameraException::settingError : If we can't get
  the color codingss.

  \sa setColorCoding(), getColorCoding(), setCamera()
*/
int
fw1394Camera::getColorCodingSupported(fw1394TwoVideoModeType mode,
                      fwList<fw1394TwoColorCodingType> & codings)
{
  // Refresh the list of supported framerates
  codings.kill();

  if (dc1394_is_video_mode_scalable((dc1394video_mode_t)mode)) {
    // Format 7 video mode
    dc1394color_codings_t _codings;
    if (dc1394_format7_get_color_codings(camera,
                     (dc1394video_mode_t)mode,
                     &_codings) != DC1394_SUCCESS) {
      close();
      fwERROR_TRACE("Could not query supported color codings for mode %d\n",
            mode);
      throw (fwCameraException(fwCameraException::settingError,
                     "Could not query supported color codings") );
    }
    if (_codings.num == 0)
      return 0;

    for (unsigned int i = 0; i < _codings.num; i ++)
      codings.addRight((fw1394TwoColorCodingType)_codings.codings[i]);

    return _codings.num;
  }
  else if (dc1394_is_video_mode_still_image((dc1394video_mode_t)mode)) {
    // Format 6 video mode
    return 0;
  }
  else  {
    // Not Format 7 and not Format 6 video modes
    dc1394color_coding_t _coding;
    if (dc1394_get_color_coding_from_video_mode(camera,
                        (dc1394video_mode_t)mode,
                        &_coding) != DC1394_SUCCESS) {
      close();
      fwERROR_TRACE("Could not query supported color coding for mode %d\n",
            mode);
      throw (fwCameraException(fwCameraException::settingError,
                     "Could not query supported color coding") );
    }
    codings.addRight((fw1394TwoColorCodingType)_coding);
    return 1;
  }
}


/*!

  Set the grabbed region of interest ie roi position and size for format 7
  video mode.

  The iso transmission (setTransmission()) and the dma capture (see
  setCapture()) are first stopped. Then, the format 7 roi is
  set. Finaly, the dma capture and the iso transmission are re-started.

  \warning Setting format 7 roi takes only effect if video mode is
  format 7 like.

  \param left : Position of the upper left roi corner.

  \param top : Position of the upper left roi corner.

  \param width : Roi width. If width is set to 0, uses the maximum
  allowed image width.

  \param height : Roi height. If width is set to 0, uses the maximum
  allowed image height.


  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \exception fwCameraException::settingError : If we can't set
  roi.

  \sa isVideoModeFormat7()
*/
void
fw1394Camera::setFormat7ROI(unsigned int left, unsigned int top,
                unsigned int width, unsigned int height)
{
  dc1394video_mode_t _videomode;
  if (dc1394_video_get_mode(camera, &_videomode) != DC1394_SUCCESS) {

    close();
    fwERROR_TRACE("Can't get current video mode");
    throw (fwCameraException(fwCameraException::settingError,
                   "Can't get current video mode") );
  }
  if (dc1394_is_video_mode_scalable(_videomode)) {
    // Stop dma capture if started
    setTransmission(DC1394_OFF);
    setCapture(DC1394_OFF);
    // Format 7 video mode
    unsigned int max_width, max_height;
    if (dc1394_format7_get_max_image_size(camera, _videomode,
                      &max_width, &max_height)
    != DC1394_SUCCESS) {

      close();
      fwERROR_TRACE("Can't get format7 max image size");
      throw (fwCameraException(fwCameraException::settingError,
                     "Can't get format7 max image size") );
    }
#if 1
    fwTRACE("left: %d top: %d width: %d height: %d", left, top,
        width == 0 ? DC1394_USE_MAX_AVAIL: width,
        height == 0 ? DC1394_USE_MAX_AVAIL : height);
    fwTRACE("max_width: %d max_height: %d", max_width, max_height);
#endif

    if (left > max_width) {
      fwERROR_TRACE("Can't set format7 ROI");
      throw (fwCameraException(fwCameraException::settingError,
                     "Can't set format7 ROI") );
    }
    if (top > max_height) {
      fwERROR_TRACE("Can't set format7 ROI");
      throw (fwCameraException(fwCameraException::settingError,
                     "Can't set format7 ROI") );
    }

    int roi_width;
    int roi_height;

    if (width != 0) {
      // Check if roi width is acceptable (ie roi is contained in the image)
      if (width > (max_width - left))
    width = (max_width - left);
      roi_width = width;
    }
    else {
      roi_width = DC1394_USE_MAX_AVAIL;
    }

    if (height != 0) {
      // Check if roi height is acceptable (ie roi is contained in the image)
      if (height > (max_height - top))
    height = (max_height - top);
      roi_height = height;
    }
    else {
      roi_height = DC1394_USE_MAX_AVAIL;
    }


    if (dc1394_format7_set_roi(camera, _videomode,
                   (dc1394color_coding_t) DC1394_QUERY_FROM_CAMERA, // color_coding
                   /*DC1394_USE_RECOMMENDED*/DC1394_USE_MAX_AVAIL/*DC1394_QUERY_FROM_CAMERA*/, // bytes_per_packet
                   left, // left
                   top, // top
                   roi_width,
                   roi_height)
    != DC1394_SUCCESS) {
      close();
      fwERROR_TRACE("Can't set format7 roi");
      throw (fwCameraException(fwCameraException::settingError,
                     "Can't get current video mode") );
    }
    // Update the image size
    if (dc1394_format7_get_image_size(camera, _videomode,
                      &this->width,
                      &this->height)
    != DC1394_SUCCESS) {
      close();
      fwERROR_TRACE("Can't get format7 image size");
      throw (fwCameraException(fwCameraException::settingError,
                     "Can't get format7 image size") );
    }

    setCapture(DC1394_ON);
    setTransmission(DC1394_ON);
  }
}

  
/*!

  Close the firewire grabber.

  Stops the capture and the iso transmission of the active cameras and than
  release the cameras.

*/
void
fw1394Camera::close()
{
  if (init) {
    setTransmission(DC1394_OFF);
    setCapture(DC1394_OFF);
    dc1394_camera_free(camera);
  }

  init = false;
}

/*!

  Setup camera capture using dma.

  \param _switch : Camera capture switch:
  - DC1394_ON to start dma capture,
  - DC1394_OFF to stop camera capture.

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \exception fwCameraException::settingError : If we can't set
  dma capture.

  \sa setVideoMode(), setFramerate()
*/
void
fw1394Camera::setCapture(dc1394switch_t _switch)
{
  if (_switch == DC1394_ON) {
    //if (dc1394_capture_setup(camera, NUM_BUFFERS) != DC1394_SUCCESS) {
    // To be compatible with libdc1394 svn 382 version
    /*if (dc1394_capture_setup(camera, NUM_BUFFERS,
                 DC1394_CAPTURE_FLAGS_DEFAULT) != DC1394_SUCCESS) {*/
    if (dc1394_capture_setup(camera, NUM_BUFFERS,
                 DC1394_CAPTURE_FLAGS_AUTO_ISO) != DC1394_SUCCESS) {
      fwERROR_TRACE("Unable to setup camera capture-\n"
            "make sure that the video mode and framerate are "
            "supported by your camera.\n");
      close();
      throw (fwCameraException(fwCameraException::settingError,
            "Could not setup dma capture") );
    }
  }
  else { // _switch == DC1394_OFF
    dc1394error_t code = dc1394_capture_stop(camera);

    if (code != DC1394_SUCCESS && code != DC1394_CAPTURE_IS_NOT_SET) {
      fwERROR_TRACE("Unable to stop camera capture\n");
      close();
      throw (fwCameraException(fwCameraException::settingError,
                     "Could not setup dma capture") );
    }
  }
}


/*!

  Setup camera transmission.

  \param _switch : Transmission switch:
  - DC1394_ON to start iso transmission,
  - DC1394_OFF to stop iso transmission.

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \exception fwCameraException::settingError : If we can't set
  the video mode.

  \sa getTransmission()
*/
void
fw1394Camera::setTransmission(dc1394switch_t _switch)
{
  // Start dma capture if halted
  if (dc1394_video_set_transmission(camera, _switch) != DC1394_SUCCESS) {
    fwERROR_TRACE("Unable to setup camera capture-\n"
                  "make sure that the video mode and framerate are "
                  "supported by your camera.\n");
    close();
    throw (fwCameraException(fwCameraException::settingError,
                  "Could not setup dma capture") );
  }

  if (_switch == DC1394_ON) {
    dc1394switch_t status = DC1394_OFF;

    int i = 0;
    while( status == DC1394_OFF && i++ < 5 ) {
      usleep(50000);
      if (dc1394_video_get_transmission(camera, &status)!=DC1394_SUCCESS) {
        fwERROR_TRACE("Unable to get transmision status");
        close();
        throw (fwCameraException(fwCameraException::settingError,
                "Could not setup dma capture") );
      }
    }
  }
}


/*!

  Get an image from the active camera frame buffer. This buffer neads to be
  released by enqueue().

  \return Pointer to the libdc1394-2.x image data structure.

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \code
  fw1394Camera g;
  dc1394video_frame_t *frame;
  g.setVideoMode(fw1394Camera::fwVIDEO_MODE_640x480_MONO8);
  g.setFramerate(fw1394Camera::fwFRAMERATE_15);
  while(1) {
    frame = g.dequeue();
    // Current image is now in frame structure
    g.enqueue(frame);
  }

  \endcode

  \sa enqueue()
*/
dc1394video_frame_t *
fw1394Camera::dequeue()
{
  dc1394video_frame_t *frame = NULL;

  if (dc1394_capture_dequeue(camera, DC1394_CAPTURE_POLICY_WAIT, &frame)
      !=DC1394_SUCCESS) {
    fwERROR_TRACE ("Error: Failed to capture from camera %d\n", camera_id);
  }


  return frame;
}

/*!
  Release the frame buffer used by the active camera.

  \param frame : Pointer to the libdc1394-2.x image data structure.

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \sa dequeue()
*/
void
fw1394Camera::enqueue(dc1394video_frame_t *frame)
{
  if (frame)
    dc1394_capture_enqueue(camera, frame);

}

/*!

  Get the image width. It depends on the camera video mode setVideoMode(). The
  image size is only available after a call to open() or acquire().

  \param width : The image width, zero if the required camera is not avalaible.

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \warning Has to be called after open() or acquire() to be sure that camera
  settings are send to the camera.

  \sa getHeight(), open(), acquire()

*/
void fw1394Camera::getWidth(unsigned int &width)
{
  width = this->width;
}

/*!

  Get the image height. It depends on the camera vide mode
  setVideoMode(). The image size is only available after a call to
  open() or acquire().

  \param height : The image width.

  \exception fwCameraException::initializationError : If no
  camera found on the bus.

  \warning Has to be called after open() or acquire() to be sure that camera
  settings are send to the camera.

  \sa getWidth()

*/
void fw1394Camera::getHeight(unsigned int &height)
{
  height = this->height;
}

/*!
  Display camera information for the active camera.

  \sa setCamera()
*/
void
fw1394Camera::printCameraInfo()
{
  std::cout << "----------------------------------------------------------"
       << std::endl
       << "-----            Information for camera " << camera_id
       << "            -----" << std::endl
       << "----------------------------------------------------------" << std::endl;

  dc1394_camera_print_info( camera, stdout);

  dc1394featureset_t features;
  if(dc1394_feature_get_all(camera, &features) != DC1394_SUCCESS) {
    close();
    fwERROR_TRACE("unable to get feature set for camera %d\n", camera_id);
    throw (fwCameraException(fwCameraException::initializationError,
                   "Cannot get camera features") );

  } else {
    dc1394_feature_print_all(&features, stdout);
  }
  std::cout << "----------------------------------------------------------" << std::endl;
}

/*!

  Converts the video mode identifier into a string containing the description
  of the mode.

  \param videomode : The camera capture video mode.

  \return A string describing the mode, an empty string if the mode is not
  supported.

  \sa string2videoMode()
*/
std::string fw1394Camera::videoMode2string(fw1394TwoVideoModeType videomode)
{
  std::string _str = "";
  dc1394video_mode_t _videomode = (dc1394video_mode_t) videomode;

  if ((_videomode >= DC1394_VIDEO_MODE_MIN)
      && (_videomode <= DC1394_VIDEO_MODE_MAX)) {
    _str = strVideoMode[_videomode - DC1394_VIDEO_MODE_MIN];
   }
  else {
    fwCERROR << "The video mode " << videomode
     << " is not supported by the camera" << std::endl;
  }

  return _str;
}

/*!

  Converts the framerate identifier into a string containing the description
  of the framerate.

  \param fps : The camera capture framerate.

  \return A string describing the framerate, an empty string if the framerate
  is not supported.

  \sa string2framerate()
*/
std::string fw1394Camera::framerate2string(fw1394TwoFramerateType fps)
{
  std::string _str = "";
  dc1394framerate_t _fps = (dc1394framerate_t) fps;

  if ((_fps >= DC1394_FRAMERATE_MIN)
      && (_fps <= DC1394_FRAMERATE_MAX)) {
    _str = strFramerate[_fps - DC1394_FRAMERATE_MIN];
  }
  else {
    fwCERROR << "The framerate " << fps
         << " is not supported by the camera" << std::endl;
  }

  return _str;
}

/*!

  Converts the color coding identifier into a string containing the description
  of the color coding.

  \param colorcoding : The color coding format.

  \return A string describing the color coding, an empty string if the
  color coding is not supported.

  \sa string2colorCoding()
*/
std::string fw1394Camera::colorCoding2string(fw1394TwoColorCodingType colorcoding)
{
  std::string _str = "";
  dc1394color_coding_t _coding = (dc1394color_coding_t) colorcoding;

  if ((_coding >= DC1394_COLOR_CODING_MIN)
      && (_coding <= DC1394_COLOR_CODING_MAX)) {
    _str = strColorCoding[_coding - DC1394_COLOR_CODING_MIN];

  }
  else {
    fwCERROR << "The color coding " << colorcoding
         << " is not supported by the camera" << std::endl;
  }

  return _str;
}

/*!

  Converts the string containing the description of the vide mode into
  the video mode identifier.

  \param videomode : The string describing the video mode.

  \return The camera capture video mode identifier.

  \exception fwCameraException::settingError : If the required videomode
  is not valid.

  This method returns 0 if the string does not match to a video mode string.

  \sa videoMode2string()

*/
fw1394Camera::fw1394TwoVideoModeType
fw1394Camera::string2videoMode(std::string videomode)
{
  fw1394TwoVideoModeType _id;

  for (int i = DC1394_VIDEO_MODE_MIN; i <= DC1394_VIDEO_MODE_MAX; i ++) {
    _id = (fw1394TwoVideoModeType) i;
    if (videomode.compare(videoMode2string(_id)) == 0)
      return _id;
  };

  throw (fwCameraException(fwCameraException::settingError,
                 "The required videomode is not valid") );

  return (fw1394TwoVideoModeType) 0;
}


/*!

  Converts the string containing the description of the framerate into the
  framerate identifier.

  \param framerate : The string describing the framerate.

  \return The camera capture framerate identifier.

  \exception fwCameraException::settingError : If the required framerate
  is not valid.

  This method returns 0 if the string does not match to a framerate string.

  \sa framerate2string()

*/
fw1394Camera::fw1394TwoFramerateType
fw1394Camera::string2framerate(std::string framerate)
{
  fw1394TwoFramerateType _id;

  for (int i = DC1394_FRAMERATE_MIN; i <= DC1394_FRAMERATE_MAX; i ++) {
    _id = (fw1394TwoFramerateType) i;
    if (framerate.compare(framerate2string(_id)) == 0)
      return _id;
  };

  throw (fwCameraException(fwCameraException::settingError,
                 "The required framerate is not valid") );

  return (fw1394TwoFramerateType) 0;
}

/*!

  Converts the string containing the description of the color coding into the
  color coding identifier.

  \param colorcoding : The string describing the color coding format.

  \return The camera capture color coding identifier.

  \exception fwCameraException::settingError : If the required
  color coding is not valid.

  This method returns 0 if the string does not match to a color coding string.

  \sa colorCoding2string()

*/
fw1394Camera::fw1394TwoColorCodingType
fw1394Camera::string2colorCoding(std::string colorcoding)
{
  fw1394TwoColorCodingType _id;

  for (int i = DC1394_COLOR_CODING_MIN; i <= DC1394_COLOR_CODING_MAX; i ++) {
    _id = (fw1394TwoColorCodingType) i;
    if (colorcoding.compare(colorCoding2string(_id)) == 0)
      return _id;
  };

  throw (fwCameraException(fwCameraException::settingError,
                 "The required color coding is not valid") );

  return (fw1394TwoColorCodingType) 0;
}

/*!
  Acquire a grey level image from the active camera.

  \param I : Image data structure (8 bits image).

  \exception fwCameraException::initializationError : If no
  camera found on the bus or if can't get camera settings.

  \exception fwCameraException::otherError : If format
  conversion to return a 8 bits image is not implemented.

  \sa setCamera(), setVideoMode(), setFramerate(), dequeue(), enqueue()
*/
void
fw1394Camera::acquire(unsigned char *I)
{
  dc1394video_frame_t *frame;

  pthread_mutex_lock(&mutex);
  frame = dequeue();
  
  //sometimes returns null
  if(frame==NULL)
  {
      printf("fw1394: Dequeueing Error\n");
      fflush(stdout);
  }
  else
  {
      this->width  = frame->size[0];
      this->height = frame->size[1];
      unsigned int size;
      size = this->width * this->height;
      if(internalImage==NULL)
      {
          internalImage=new unsigned char[3*size];
      }

      //color copy more bits -gary, wrong for 422 etc
      if(isColor())
      {
          //printf("Size: %d %d %d\n",this->width,this->height,size);
          if(yuvFlag==false)
          {
            //memcpy(I, (unsigned char *) frame->image, 3*size*sizeof(unsigned char));
            int i;
            unsigned char *InData=(unsigned char *) frame->image;
            for(i=0;i<size;i++)
            {
                internalImage[3*i+0]=InData[3*i+2];
                internalImage[3*i+1]=InData[3*i+1];
                internalImage[3*i+2]=InData[3*i+0];
            }
          }
          else
          {
            int i,j;
            int pos;
            unsigned char *InData=(unsigned char *) frame->image;
            for(j=0;j<this->height;j++)
            {
                for(i=0;i<this->width;i++)
                {
                    //height backward, BGR
                    
                    int y=InData[2*(j*this->width+i)+1];
                    int u=InData[4*((j/2)*this->width+i/2)+2];
                    int v=InData[4*((j/2)*this->width+i/2)+0];
                    pos=3*(j*this->width+i);
                    //Lookup tables
                    //red
                    internalImage[pos+2]=CLAMPCONV(1.164*(y-16)+1.4022*(u-128));
                    //I[pos+2]=YUVTableR[y][u][v];
                    //green
                    internalImage[pos+1]=CLAMPCONV(1.164*(y-16)-0.7145*(u-128)-0.3456*(v-128));
                    //I[pos+1]=YUVTableG[y][u][v];
                    //blue
                    internalImage[pos+0]=CLAMPCONV(1.164*(y-16)+1.7771*(v-128));
                    //I[pos+0]=YUVTableB[y][u][v];
                }
            }
          }
          if(balanceFlag)
          {
            doAutoWhiteBalance(internalImage, size);
          }
          if(shutterFlag)
          {
            doAutoShutter(internalImage,size);
          }

          if(displayFlag)
          {
             printf("Camera %d: Black %d %d\n",cameraId,(int)blackFlag,blackCount);
          }
          memcpy(I,internalImage,3*size);
          
          //printf("%d Manual Shutter: %d White: %d\n",cameraId,manualShutter,manualWhite);
          if(manualWhite==1)
          {
            dc1394_feature_set_mode(camera,DC1394_FEATURE_WHITE_BALANCE,DC1394_FEATURE_MODE_MANUAL);
            dc1394_feature_whitebalance_set_value(camera,Blue,Red);
          }
          if(manualShutter==1)
          {
            dc1394_feature_set_mode(camera,DC1394_FEATURE_SHUTTER,DC1394_FEATURE_MODE_MANUAL);
            dc1394_feature_set_value(camera,DC1394_FEATURE_SHUTTER,Shutter);
          }
      }
      else
      {
        memcpy(I, (unsigned char *) frame->image, size*sizeof(unsigned char));
      }
      frameCount++;
  }
  enqueue(frame);
  pthread_mutex_unlock(&mutex);
}

/*!

  Set the gain for a given camera.

  \warning Before setting the gain a handle must
  be created by calling open(), and a camera must be connected.

  \param gain The gain value to apply to the camera.

  \exception fwCameraException::settingError : If the required camera is not present or if an error occurs.

  \sa getGain(), getNumCameras(), setCamera()
*/
void
fw1394Camera::setFeature(unsigned int gain, dc1394feature_t feature)
{
  if (dc1394_feature_set_value(camera, feature , gain)!= DC1394_SUCCESS) {
    fwERROR_TRACE("Unable to set gain");
    throw (fwCameraException(fwCameraException::settingError,
                   "Cannot set gain") );
  }
}

/*!

  Query the actual gain value of a given camera and the bounded gain
  values.

  \warning Before requerying the gain a handle must
  be created by calling open(), and a camera must be connected.

  \param min_gain : Minimal autorized gain value.

  \param gain : The current camera gain value. This value is comprised
  between \e min_gain and \e max_gain.

  \param max_gain : Maximal autorized gain value.

  \exception fwCameraException::settingError :If the required camera is
  not present or if an error occurs.

  \sa setGain(), open(), getNumCameras(), setCamera()

*/
void
fw1394Camera::getFeature(unsigned int &min_gain,
               unsigned int &gain,
               unsigned int &max_gain,
               dc1394feature_t feature)
{
  gain = 0;
  min_gain = 0;
  max_gain = 0;

  if(dc1394_feature_get_value(camera,
             feature,
             &gain) !=DC1394_SUCCESS) {
    close();
    fwERROR_TRACE("Unable to get gain value");
    throw (fwCameraException(fwCameraException::settingError,
                   "Unable to get gain value") );

  }

  if(dc1394_feature_get_boundaries(camera,
              feature,
              &min_gain, &max_gain) !=DC1394_SUCCESS) {
    close();
    fwERROR_TRACE("Unable to get min/max gain value");
    throw (fwCameraException(fwCameraException::settingError,
                   "Unable to get min/max gain value") );
  }

}

void fw1394Camera::doAutoWhiteBalance(unsigned char *image,int size)
{
    //do some auto white balance
    unsigned int sumr=0;
    unsigned int sumg=0;
    unsigned int sumb=0;
    for(int i=0;i<size;i++)
    {
      //red green blue
      sumr+=image[3*i+2];
      sumg+=image[3*i+1];
      sumb+=image[3*i+0];
    }
    sumr/=size;
    sumg/=size;
    sumb/=size;
    
    
    //try to get colors even (white or at least grey)
    /*if(sumr<sumg)
    {
      Red+=1;
    }
    else
    {
      Red-=1;
    }
    
    if(sumb<sumg)
    {
      Blue+=1;
    }
    else
    {
      Blue-=1;
    }
     */
    double controlr=((int)sumg-(int)sumr);
    Red+=round(controlr*0.125);
    double controlb=((int)sumg-(int)sumb);
    Blue+=round(controlb*0.125);
     
     
     
    Blue=(Blue<1)?1:((Blue>568)?568:Blue);
    Red=(Red<1)?1:((Red>568)?568:Red);
    
    if(displayFlag)
    {
        printf("White %d %d: %d %d %d\n",Red,Blue,sumr,sumg,sumb);

    }
    dc1394_feature_whitebalance_set_value(camera,Blue,Red);
}

void fw1394Camera::doAutoShutter(unsigned char *image,int size)
{
    //do some auto white balance
    unsigned int sum=0;
    for(int i=0;i<3*size;i++)
    {
      //red green blue
      sum+=image[i];
    }
    sum/=3*size;

    
    //try to make an average color
    /*if(sum>160)
    {
        Shutter-=10;
    }
    else
    {
        Shutter+=10;
    }*/
    //double control=(160-(int)sum);
    double control=(Avg-(int)sum);
    /*int limit=1;
    if(frameCount<500)
    {
        limit=10;
    }*/
    //control=(control<-limit)?-limit:((control>limit)?limit:control);
    if(sum<1)
    {
        blackCount++;
        blackFlag=true;
    }
    else
    {
        blackFlag=false;
    }
    Shutter+=round(control*PTerm);
    
    
    Shutter=(Shutter<1)?1:((Shutter>4095)?4095:Shutter);
    /*static int startfail=120;
    Shutter=startfail;

    startfail++;
    if(startfail>150)
    {
        startfail=120;
    }*/
    
    //uint32_t tmp;
    //dc1394_feature_get_value(camera,DC1394_FEATURE_SHUTTER,&tmp);
    //Shutter=tmp;
    if(displayFlag)
    {
        printf("Shutter %d: %d\n",Shutter,sum);
    }
}

void fw1394Camera::SetWhiteBalance(int red,int blue)
{
  //if set was called, want to no longer use auto white balance of camera
  Red=red;
  Blue=blue;
  manualWhite=1;
}

void fw1394Camera::GetWhiteBalance(int &red,int &blue)
{
  red=Red;
  blue=Blue;
}

void fw1394Camera::SetShutter(int shutter)
{
    Shutter=shutter;
    manualShutter=1;

}
void fw1394Camera::GetShutter(int &shutter)
{
    shutter=Shutter;
}


void fw1394Camera::enableAutoWhite(bool white)
{
    //if doing auto, also manual mode
    balanceFlag=white;
    manualWhite=1;
}
void fw1394Camera::enableAutoShutter(bool shutter)
{
    //if doing auto, also manual mode
    shutterFlag=shutter;
    manualShutter=1;
}

void fw1394Camera::SetDesiredAvg(int avg)
{
    Avg=avg;
}

void fw1394Camera::doingYUV(bool val)
{
    yuvFlag=val;
/*    if(val)
    {
        MakeTable();
    }*/
}
/*void fw1394Camera::MakeTable()
{
    for(int y=0;y<256;y++)
    {
        for(int u=0;u<256;u++)
        {
            for(int v=0;v<256;v++)
            {
                YUVTableR[y][u][v]=CLAMPCONV(1.164*(y-16)+1.4022*(u-128));
                YUVTableG[y][u][v]=CLAMPCONV(1.164*(y-16)-0.7145*(u-128)-0.3456*(v-128));
                YUVTableB[y][u][v]=CLAMPCONV(1.164*(y-16)+1.7771*(v-128));
            }
        }
    }
}*/


void fw1394Camera::debugDisplay(bool display)
{
    displayFlag=display;
}

void fw1394Camera::setPTerm(double p)
{
    PTerm=p;
}

#endif
