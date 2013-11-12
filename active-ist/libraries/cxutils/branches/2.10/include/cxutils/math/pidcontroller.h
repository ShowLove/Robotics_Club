////////////////////////////////////////////////////////////////////////////////////
///
///  \file pidcontroller.h
///  \brief Proportional, Integral, and Differential Controller class used to
///  create closed loop operations.
///   
///  <br>Author(s): Daniel Barber
///  <br>Created: 9 May 2007
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef _CXUTILS_MATH_PIDCONTROLLER_H
#define _CXUTILS_MATH_PIDCONTROLLER_H

#include "cxutils/cxbase.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///  \class PIDController
    ///  \brief A generic PID controller.  This can be used for any
    ///  application that needs to handle oscillations using a PID
    ///  controller.  Specific examples using a PID controller are
    ///  maintain velocity, heading, or position.
    ///
    ///  This controller does not calculate the error for your specific
    ///  application, instead it takes the error as an argument to the
    ///  UpdatePID function, which returns the new solution to use in your
    ///  closed loop process.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL PIDController
    {
    public:
        PIDController();
        ~PIDController();
        // Sets the value of DT (rate of change for PID).
        void SetRate(const double dt = 1.0);
        // Update the PID based on error, return next value to use.
        double UpdatePID(const double error);
        //  Add an offset to the solution to taylor the
        //  results more to your specific application.  For example
        //  if your system inputs range from 50 - 150, then you would have
        //  an offset of 50 to offset the [-x, 0, x] range and
        //  have max an min bounds of 50 - 150
        void SetOffset(const double offset);
        //  Setting the bounds prevents the sum of error
        //  from getting out of control.  This should be set
        //  to the max and min values your system can use.  This
        //  basically prevents you from giving 150% if your max
        //  value is 100%.
        void SetBounds(const double min, const double max);
        void SetGains(const double p, const double i, const double d);
        void ClearError();
        void Reset();
        void Reset(const double lower, 
                   const double upper, 
                   const double offset, 
                   const double p, 
                   const double i, 
                   const double d);
        double GetProportionalGain()const { return mP; }
        double GetIntegralGrain() const { return mI; }
        double GetDifferentialGain() const { return mD; }
        double GetSumOfError()const { return mSumOfError; }
        double GetOffset()const { return mOffset; }
        double GetLowerBound()const { return mLowerBound; }
        double GetUpperBound()const { return mUpperBound; }
    protected:
        double mPreviousError;      ///<  Previous error of system
        double mPreviousSolution;   ///<  Previous feedback result (used with upper and lower bounds to prevent winding)
        double mP;              ///<  Proportional Gain
        double mI;              ///<  Integral Gain
        double mD;              ///<  Differential Gain
        double mSumOfError;     ///<  Sum of error
        double mOffset;         ///<  Offset to add onto solution (default 0)
        double mUpperBound;     ///<  Upper bound on solution (prevents winding/large sum of errors) default is INFINITE
        double mLowerBound;     ///<  Lower bound on solution (prevents winding/large sum of erros)  default is -INFINITE
        double mDT;             ///<  DT term.
    };
}

#endif
/*  End of File */
