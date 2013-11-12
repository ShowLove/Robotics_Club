/////////////////////////////////////////////////////////////////////////////
///
/// \file controls.h
/// \brief Wraps up all of the "controllable outputs"
///
/// Author(s): Michael Scherer<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
/// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
/// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
/// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
/// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
/// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
/// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
/// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
/// OF SUCH DAMAGE.
///
/////////////////////////////////////////////////////////////////////////////

#ifndef __ZEBULON_UNDERWATER_CONTROLS_H
#define __ZEBULON_UNDERWATER_CONTROLS_H

namespace Zebulon
{
    namespace Underwater
    {
        struct Controls
        {
        public:
            Controls();
            Controls(const Controls& controls);
            Controls& operator=(const Controls& controls);
            ~Controls();
            
            bool mAxialThrustEnable;
            double mAxialThrust;
            
            bool mLateralThrustEnable;
            double mLateralThrust;
            
            bool mVerticalThrustEnable;
            double mVerticalThrust;
            
            bool mYawEnable;
            double mYaw;
            
            bool mPitchEnable;
            double mPitch;
            
            bool mRollEnable;
            double mRoll;
            
            bool mDepthEnable;
            double mDepth;
        };
    }
}

#endif // __ZEBULON_UNDERWATER_CONTROLS_H