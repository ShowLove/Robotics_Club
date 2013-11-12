/****************************************************************************
 *
 *****************************************************************************/


/* \file fwException.h
   \brief error that can be emited by the fw class and its derivates
 */

#ifndef __fwException_H
#define __fwException_H


/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */


/* Classes standards. */
#include <iostream>                /* Classe std::ostream.    */
#include <string>                  /* Classe string.     */

#include <fw/fwConfig.h>



/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */


/* \class fwException
   \brief error that can be emited by the fw class and its derivates
 */
class FW_EXPORT fwException //: public std::exception
{

private:

  //! Contains the error code, see the errorCodeEnum table for details.
  int code;

  //! Contains an error message (can be empty)
  std::string message;

private:

  //!  forbid the empty constructor (private)
  fwException();

public:

  enum generalExceptionEnum
    {
      memoryAllocationError,
      memoryFreeError,
      functionNotImplementedError,
      ioError,
      cannotUseConstructorError,
      notImplementedError,
      divideByZeroError,
      dimensionError,
      fatalError
    } ;

  fwException (const int code, const char * msg);
  fwException (const int code, const std::string & msg);
  fwException (const int code);

  //!  send the object code
  int getCode (void);

  //! send a reference (constant) related the error message (can be empty)
  const std::string &getStringMessage (void);
  //! send a pointer on the array of  \e char related to the error string.
  //!Cannot be  \e NULL.
  const char *getMessage (void);

  //! print the error structure
  friend FW_EXPORT std::ostream & operator << (std::ostream & os,
				const fwException & art);

};





#endif /* #ifndef __fwException_H */


/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
