/****************************************************************************
 *
 *
 * Description:
 * Debug and trace macro.
 *
 *   - TRACING:    vpTRACE and vpERROR_TRACE work like printf with carreer return at the end of the string.
 *                 vpCERROR et vpCTRACE work like the C++ output streams std::cout and std::cerr.
 *   - DEBUGING:   vpDEBUG_TRACE(niv,  and vpDERROR_TRACE(niv, work like printf, but print only if the
 *                 tracing level niv is greater than the debug level VP_DEBUG_MODE.
 *                 vpCDEBUG(niv) work like the C++ output stream std::cout.
 *                 vpDEBUG_ENABLE(niv) is equal to 1 if the debug level niv is greater than the debug mode
 *                 VP_DEBUG_MODE, 0 else.
 *   - PROG DEFENSIVE: DEFENSIF(a) is equal to a if defensive mode is active, 0 else.
 *
 * Authors:
 * Nicolas Mansard, Bruno Renier
 *
 *****************************************************************************/

#ifndef __VP_DEBUG_HH
#define __VP_DEBUG_HH

#include <stdio.h>
#include <stdarg.h>
#include <iostream>


#ifdef WIN32
#  ifndef __FUNCTION__
#    define __FUNCTION__ " "
#  endif
#endif

#ifndef VP_DEBUG_MODE
#  define VP_DEBUG_MODE 0
#endif


/*!
  \class fwTraceOutput
  \brief This class is used to display debug or error messages.
  It needs to be initialized with the file name, function name and line, of
  the place where it is created.
  It is best used by first instanciating the object and directly calling the () operator.
  This is used to mimic variadic macros (not supported in MSVC prior to version 8)
*/
class fwTraceOutput
{
private:
    const char* currentFile; //Name of the file to use in the displays
    const char* currentFunc; //Name of the function to use in the displays
    int currentLine;		 //Line to use in the displays

    //if true, output to std::cerr/stderr else std::cout/stdout
    bool err;
    //string to display before anything else
    const char* header;

public:
    /*!
      Constructor.
      \param file Should be the name of the file where this constructor was called.
      \param line Should be the line in file where this constructor was called.
      \param func Should be the name of the function where this constructor was called.
      \param error If true, writes to the error stream.
      \param s String to print before any other message (acts like a header).
      \note Call the constructor with something like fwTraceOutput(__FILE__,__LINE__, __FUNCTION__).
    */
    fwTraceOutput(const char* file, int line, const char* func, bool error=false, const char * s=NULL) :
        currentFile(file),
	currentFunc(func),
	currentLine(line),
	err(error),
	header(s)
    {}

    /*!
      Displays a string if the debug level is inferior to VP_DEBUG_MODE.
      \param niv Level of this message.
      \param format Formating string.
    */
    void operator()(int niv, const char* format, ...)
    {
	//if the niv level is inferior to VP_DEBUG_MODE
	if(VP_DEBUG_MODE >= niv)
	{
	    //gets the variable list of arguments
	    va_list args;
	    va_start(args, format);

	    std::cout << "(N" << niv << ") " ;
	    //calls display with it
	    display(format, args);

	    va_end(args);
	}
    }

    /*!
      Displays a string.
      \param format Formating string.
    */
    void operator()(const char* format, ...)
    {
	//gets the variable list of arguments
	va_list args;
	va_start(args, format);

#ifdef VP_DEBUG
	std::cout<<"(N0) ";
#endif

	//calls display with it
	display(format, args);

	va_end(args);
    }

    /*!

      Displays a message to either stdout/std::cout or stderr/std::cerr (based on error
      boolean).

      \param format Formating string.
      \param args List of arguments.

    */
    void display(const char* format, va_list args)
    {
	//if we want to write to std::cerr/stderr
	if(err)
	{
	    //first writes the header if there is one
	    if(header != NULL) std::cerr<<header;
	    //then writes the recorded namefile, function and line
	    std::cerr << "!!\t" << currentFile << ": " <<currentFunc << "(#" << currentLine << ") :" ;
	    //and finally writes the message passed to () operator.
	    vfprintf (stderr, format, args);
	    fprintf (stderr, "\n");
	    //flushes the buffer
	    fflush (stderr);
	}
	else
	{
	    //first writes the header if there is one
	    if(header != NULL) std::cout<<header;
	    //then writes the recorded namefile, function and line
	    std::cout <<currentFile << ": " << currentFunc << "(#" << currentLine << ") :" ;
	    //and finally writes the message passed to () operator.
	    vprintf (format, args);
	    printf ("\n");
	    //flushes the buffer
	    fflush (stdout);
	}
    }

};




/* -------------------------------------------------------------------------- */
/* --- fwTRACE -------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/*!
  Used to display trace messages on the standard stream (C++).
  Use like this : fwCTRACE<<"my message"<<std::endl;
*/
#define fwCTRACE std::cout << __FILE__ << ": " << __FUNCTION__ << "(#" << __LINE__ << ") :"


/*!
  Used to display error messages on the error stream (C++).
  Use like this : fwCERROR<<"my message"<<std::endl;
*/
#define fwCERROR std::cerr << "!!\t" << __FILE__ << ": " << __FUNCTION__ << "(#" << __LINE__ << ") :"

/*!
  Used to display error messages on the error stream.
  Prints the name of the file, the function name and the line where
  it was used.
  Use like this : fwERRORTRACE("my error message number %d", i);
  with any "printf" string.
*/
#define fwERROR_TRACE (fwTraceOutput( __FILE__,__LINE__, __FUNCTION__, true))

/*!
  Used to display trace messages on the standard stream.
  Prints the name of the file, the function name and the line where
  it was used.
  Use like this : fwTRACE("my debug message number %d", i);
  with any "printf" string.
*/
#define fwTRACE (fwTraceOutput( __FILE__,__LINE__, __FUNCTION__, false))


/* -------------------------------------------------------------------------- */
/* --- VP_DEBUG ------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#ifdef VP_DEBUG

/*!
  fwDERROR_TRACE works like printf, but prints only if the
  tracing level niv is greater than the debug level VP_DEBUG_MODE.
*/
#define fwDERROR_TRACE (fwTraceOutput( __FILE__,__LINE__, __FUNCTION__, true))

/*!
  fwDEBUG_TRACE works like printf, but prints only if the
  tracing level niv is greater than the debug level VP_DEBUG_MODE.
*/
#define fwDEBUG_TRACE (fwTraceOutput( __FILE__,__LINE__, __FUNCTION__, false))

/*!
  fwCDEBUG(niv) work like the C++ output stream std::cout.
*/
#define fwCDEBUG(niv) if (VP_DEBUG_MODE < niv) ; else \
		std::cout << "(N" << niv << ") "<<  __FILE__ << ": " << __FUNCTION__ << "(#" << __LINE__ << ") :"

/*!

  fwDEBUG_ENABLE(niv) is equal to 1 if the debug level niv is greater than
  the debug mode VP_DEBUG_MODE, 0 else.
*/
#define fwDEBUG_ENABLE(niv) (VP_DEBUG_MODE >= niv)

#else

inline void fwDERROR_TRACE(int /* niv */, char * /* a */, ...){};
inline void fwDEBUG_TRACE(int /* niv */, char * /* a */, ...){};
#define fwCDEBUG(niv) if (1) ; else std::cout
#define fwDEBUG_ENABLE(niv) (0)

#endif



/* -------------------------------------------------------------------------- */
/* --- fwTRACE IN/OUT FONCTION ---------------------------------------------- */
/* -------------------------------------------------------------------------- */

/*!
  Works like fwTRACE and should be used at the beginning of a function.
*/
#define fwIN_FCT (fwTraceOutput(__FILE__,__LINE__, __FUNCTION__, false, "begin "))


/*!
  Works like fwTRACE and should be used at the end of a function.
*/
#define fwOUT_FCT (fwTraceOutput(__FILE__,__LINE__, __FUNCTION__, false, "end "))


/* -------------------------------------------------------------------------- */
/* --- DEFENSIF ------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#ifdef VP_DEFENSIF
#define DEFENSIF(a)  (a)
#else
#define DEFENSIF(a)  (0)
#endif  /*#ifdef DEFENSIF*/


#endif /* #ifdef __DEBUG_HH */

/*
 * Local variables:
 * c-basic-offset: 4
 * End:
 */
