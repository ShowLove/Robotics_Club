////////////////////////////////////////////////////////////////////////////////////
///
///  \file pidcontroller.cpp
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
#include <limits.h>
#include "cxutils/timer.h"
#include "cxutils/math/pidcontroller.h"

using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
PIDController::PIDController()
{
    Reset();
    //Default DT if not set elsewhere
    mDT = 1.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
PIDController::~PIDController()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Set the update interval for PID constants
///
///   \param[in] dt Delta Time variable for PID calculations, default is 1.
///
////////////////////////////////////////////////////////////////////////////////////
void PIDController::SetRate(const double dt)
{
    mDT = dt;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Uses error feedback to provide new input to the system.
///
///   This function performs the calculations of the PID controller.  It checks
///   to make sure the returned value is within the bounds of the maximum and
///   minimum values your system can take.  The upper and lower bounds can be
///   set using the setBounds function.
///
///   The error provided should be the error of your system between desired
///   and actual.  This controller does not calculate error so that it can
///   be kept generic.  The results of this function are only as good as
///   the PID gain values that model your system.
///
///   \param error The feedback error of the system.
///
///   \return The output to provide your system from the PID controller.
///
////////////////////////////////////////////////////////////////////////////////////
double PIDController::UpdatePID(const double error)
{
    double solution;

    //  Prevent over winding the sum of error.  This basically
    //  means, if you are already giving the system 100%, you can't
    //  give it higher than that, so don't keep increasing
    //  the error
    if(mPreviousSolution > mLowerBound && mPreviousSolution < mUpperBound)
    {
        this->mSumOfError += error;
    }
    solution = mP*error + mI*mSumOfError*mDT + mD*(error - mPreviousError)/mDT;
    mPreviousError = error;

    //  Add any offset for the system
    solution += this->mOffset;

    //  Check upper and lower bounds of solution for
    //  the system being used
    if(solution <= mLowerBound)
        solution = mLowerBound;
    if(solution >= mUpperBound)
        solution = mUpperBound;

    this->mPreviousSolution = solution;

    return solution;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets any desired offset for calculating solution from the PID.
///
///   For example, if the inputs you want to provide your system are between
///   [-100,100], then your offset would be 0, and you would have upper and 
///   lower bounds of [-100,100].  If your system was [50, 150], you would have
///   an offset of 50, with upper and lower bounds of [50,150].
///
///   \param offset  The offset to add to the PID solutions.
///
////////////////////////////////////////////////////////////////////////////////////
void PIDController::SetOffset(const double offset)
{
    this->mOffset = offset;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets boundaries for the solutions the PID can calculate.  
///
///   For example, if the inputs you want to provide your system are between
///   [-100,100], then your offset would be 0, and you would have upper and 
///   lower bounds of [-100,100].  If your system was [50, 150], you would have
///   an offset of 50, with upper and lower bounds of [50,150].
///
///   \param min The lower bound of your system inputs
///   \param max The upper bound of your system inputs
///
////////////////////////////////////////////////////////////////////////////////////
void PIDController::SetBounds(const double min, const double max)
{
    if(max > min)
    {
        this->mLowerBound = min;
        this->mUpperBound = max;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the PID gain values of your controller.
///
///   By adjusting these values you can increase or decrease the performance
///   of the PID controller.  You should have a limited understanding of
///   how to adjust these values before doing so.
///
///   \param p The proportional gain
///   \param i The integral gain
///   \param d The derivative gain
///
////////////////////////////////////////////////////////////////////////////////////
void PIDController::SetGains(const double p, const double i, const double d)
{
    mP = p;
    mI = i;
    mD = d;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears out any summed error.
///
////////////////////////////////////////////////////////////////////////////////////
void PIDController::ClearError()
{
    mSumOfError = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets all values back to defaults.
///
////////////////////////////////////////////////////////////////////////////////////
void PIDController::Reset()
{
    mLowerBound = -LONG_MAX;
    mUpperBound = LONG_MAX;

    mP = 0;
    mI = 0;
    mD = 0;
    mSumOfError = 0;
    mPreviousSolution = 0;
    mPreviousError = 0;
    mOffset = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Resets the sum of errors, and configures the controller based on
///   the inputs.
///
///   \param lower Lower bound of system inputs
///   \param upper Upper bound of system inputs
///   \param offset Offset to add to system input solutions
///   \param p The proportional gain
///   \param i The integral gain
///   \param d The derivative gain 
///
////////////////////////////////////////////////////////////////////////////////////
void PIDController::Reset(const double lower, 
                          const double upper, 
                          const double offset, 
                          const double p, 
                          const double i, 
                          const double d)
{
    SetBounds(lower, upper);
    SetGains(p, i, d);
    SetOffset(offset);
    mSumOfError = 0;
    mPreviousSolution = mOffset;
    mPreviousError = 0;
}

/*  End of File */
