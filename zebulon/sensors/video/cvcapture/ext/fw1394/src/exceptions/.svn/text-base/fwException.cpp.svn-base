/****************************************************************************
 *
 *****************************************************************************/


/* \file fwException.cpp
   \brief error that can be emited by the fw class and its derivates
 */

#include "fw/fwException.h"


/* ------------------------------------------------------------------------- */
/* --- CONSTRUCTORS -------------------------------------------------------- */
/* ------------------------------------------------------------------------- */


fwException::fwException (int _code)
    :
    code (_code),
    message ()

{
    return ;
}


fwException::fwException (int _code,
	     const std::string & _msg)
    :
    code (_code),
    message (_msg)

{
    return ;
}


fwException::fwException (int _code,
	     const char * _msg)
    :
    code (_code),
    message (_msg)
{
    return ;
}

/* ------------------------------------------------------------------------ */
/* --- DESTRUCTORS -------------------------------------------------------- */
/* ------------------------------------------------------------------------ */

/* Destructeur par default suffisant. */
// fwException::
// ~fwException (void)
// {
// }

/* ------------------------------------------------------------------------ */
/* --- ACCESSORS ---------------------------------------------------------- */
/* ------------------------------------------------------------------------ */


const char *fwException::getMessage (void)
{
    return (this->message) .c_str();
}

const std::string &fwException::getStringMessage (void)
{
    return this->message;
}

int
fwException::getCode (void)
{
    return this->code;
}


/* ------------------------------------------------------------------------- */
/* --- MODIFIORS ----------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* --- OP << --------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

std::ostream &
operator << (std::ostream & os,
	     const fwException & error)
{
  os << "Error [" << error.code << "]:\t" << error.message << std::endl;

    return os;
}


/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
