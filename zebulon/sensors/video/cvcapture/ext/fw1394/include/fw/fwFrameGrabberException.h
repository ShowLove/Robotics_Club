/****************************************************************************
 *
 *****************************************************************************/


#ifndef __fwFrameGrabberException_H
#define __fwFrameGrabberException_H


/* ------------------------------------------------------------------------- */
/* --- INCLUDE ------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */


/* \file fwFrameGrabberException.h
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
class FW_EXPORT fwFrameGrabberException : public fwException
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
  fwFrameGrabberException (const int code, const char * msg)
    : fwException(code, msg){ ; }
  fwFrameGrabberException (const int code, const std::string & msg)
    : fwException(code, msg){ ; }
  fwFrameGrabberException (const int code)
    : fwException(code){ ; }

};





#endif /* #ifndef __fwFrameGrabberException_H */


/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
