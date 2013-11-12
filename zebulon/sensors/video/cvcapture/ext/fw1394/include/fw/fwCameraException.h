/****************************************************************************
 *
 *****************************************************************************/


#ifndef __fwCameraException_H
#define __fwCameraException_H


/* ------------------------------------------------------------------------- */
/* --- INCLUDE ------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */


/* \file fwCameraException.h
   \brief error that can be emited by the fwFrameGrabber class and its derivates
 */
/* Classes standards. */
#include <iostream>                /* Classe std::ostream.    */
#include <string>                  /* Classe string.     */
#include <fw/fwConfig.h>
#include <fw/fwException.h>



/* ------------------------------------------------------------------------- */
/* --- CLASS --------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

/* \brief error that can be emited by the fwFrameGrabber class and its derivates
 */
class FW_EXPORT fwCameraException : public fwException
{
public:
  /*!
    \brief Lists the possible error than can be emmited while calling
    fwFrameGrabber member
   */
  enum errorFrameGrabberCodeEnum
    {
      settingError,
      initializationError,
      otherError
    } ;

public:
  fwCameraException (const int code, const char * msg)
    : fwException(code, msg){ ; }
  fwCameraException (const int code, const std::string & msg)
    : fwException(code, msg){ ; }
  fwCameraException (const int code)
    : fwException(code){ ; }

};





#endif /* #ifndef __fwCameraException_H */


/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
