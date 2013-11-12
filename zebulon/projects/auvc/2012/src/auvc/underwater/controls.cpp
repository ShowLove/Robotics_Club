#include <auvc/underwater/controls.h>

using namespace Zebulon;
using namespace Underwater;

Controls::Controls()
{
    mAxialThrust = 0;
    mLateralThrust = 0;
    mVerticalThrust = 0;
    
    mYaw = 0;
    mPitch = 0;
    mRoll = 0;
    
    mDepth = 0;
    
    //////////////////////
    
    mAxialThrustEnable = false;
    mLateralThrustEnable = false;
    mVerticalThrustEnable = false;
    
    mYawEnable = false;
    mPitchEnable = false;
    mRollEnable = false;
    
    mDepthEnable = false;
}


Controls::Controls(const Controls& controls)
{
    *this = controls;
}
Controls& Controls::operator=(const Controls& controls)
{
    if (this != &controls)
    {
        mAxialThrust = controls.mAxialThrust;
        mLateralThrust = controls.mLateralThrust;
        mVerticalThrust = controls.mVerticalThrust;
        
        mYaw = controls.mYaw;
        mPitch = controls.mPitch;
        mRoll = controls.mRoll;
        
        mDepth = controls.mDepth;
        
        //////////////////////
        
        mAxialThrustEnable = controls.mAxialThrustEnable;
        mLateralThrustEnable = controls.mLateralThrustEnable;
        mVerticalThrustEnable = controls.mVerticalThrustEnable;
        
        mYawEnable = controls.mYawEnable;
        mPitchEnable = controls.mPitchEnable;
        mRollEnable = controls.mRollEnable;
        
        mDepthEnable = controls.mDepthEnable;
    }
    
    return *this;
}

Controls::~Controls()
{
    
}