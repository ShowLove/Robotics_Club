/****************************************************************************
 *
 *****************************************************************************/



#ifndef fwFrameGrabber_hh
#define fwFrameGrabber_hh


#include <fw/fwConfig.h>

/*!
  \file fwFrameGrabber.h
  \brief Base class for all video devices. It is
         designed to provide a generic front end to video sources.
*/

/*!
  \class fwFrameGrabber
  \brief Base class for all video devices. It is
         designed to provide a generic front end to video sources.

  \author Eric Marchand (Eric.Marchand@irisa.fr), Irisa / Inria Rennes

  This class should provide a virtual function that allows the acquisition
  of an image.

*/
class FW_EXPORT fwFrameGrabber
{
public :
  bool   init ;  //!< bit 1 if the frame grabber has been initialized
protected:
  unsigned int height ;  //!< number of rows in the image
  unsigned int width ;  //!< number of columns in the image


public:
  //! return the number of rows in the image
  inline  unsigned int getHeight() { return height ; }
  //! return the number of columns in the image
  inline  unsigned int getWidth() { return width ; }

public:
  virtual ~fwFrameGrabber() { ; }


  /*!
    This virtual function is used to de-allocate
    the memory used by a specific frame grabber
  */
  virtual void close() =0 ;

} ;

#endif
