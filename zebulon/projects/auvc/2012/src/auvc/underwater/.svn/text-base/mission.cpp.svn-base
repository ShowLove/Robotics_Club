/////////////////////////////////////////////////////////////////////////////
///
/// \file mission.cpp
/// \brief Base class for sub missions.
///
/// Author(s): John Reeder <br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
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

#include "auvc/underwater/mission.h"

using namespace Zebulon;
using namespace Underwater;

GlobalInfo* Mission::mGlobalInfo = NULL;
GlobalCommand* Mission::mGlobalCommand = NULL;

Mission::Mission()
{
    //default to david's method
    mPathMethod = 1;
}

Mission::~Mission()
{
    
}


void Mission::SetGlobalCommand(AI::GlobalCommand* globalCommand)
{
    mGlobalCommand = (Underwater::GlobalCommand*)globalCommand;
    return;
}

void Mission::SetGlobalInfo(AI::GlobalInfo* globalInfo)
{
    mGlobalInfo = (Underwater::GlobalInfo*)globalInfo;
    return;
}

void Mission::InitPaths(bool method1, bool method2, bool method3)
{
    if(method1)
    {
        mPathMethod=0;
    }
    else if(method2)
    {
        mPathMethod=1;
    }
    else if(method3)
    {
        mPathMethod=2;
    }
}

bool Mission::GetPaths(std::vector<Path> &paths,double currYaw, IplImage *dwFrame, IplImage *dwProcFrame)
{
//    if(mPathMethod==0)
//    {
//        std::cout << "Basic Threshold and Segmentation" << std::endl;
//        std::cout << std::endl;
//        paths = mPathFinder.GetPaths(currYaw, dwFrame, dwProcFrame);
//        return mPathDebounce.Bounce(paths.size() > 0);
//    }
//    else if(mPathMethod==1)
//    {
        std::cout << "David's CV Functions" << std::endl;
        std::cout << std::endl;
        paths = mPathFinder.GetPathsDavid(currYaw, dwFrame, dwProcFrame);
        return mPathDebounce.Bounce(paths.size() > 0);
//    }
//    else if(mPathMethod==2)
//    {
//        std::cout << "Nothing is implemented here!" << std::endl;
//        std::cout << std::endl;
//        return 0;
//    }
//    else
//    {
//        std::cout << "Vision Algo DNE!" << std::endl;
//        std::cout << std::endl;
//        return 0;
//    }
}

/*
void Mission::SetSearchType(bool m, bool sweep)
{
    mSearchMethod
}

void Mission::InitSearch()
{
    if(strafe)
    {
        mStrafeSearch.InitSearch(mBuoySearchStrafeTime, mBuoySearchStrafeSpeed, mBuoyStrafeForwardTime, mBuoyStrafeForwardSpeed, mBuoyStrafeSmoothSpeed, mBuoyStrafeInitDirection);
    }
    else if(sweep)
    {
        mSweepSearch.InitSearch(mFixedYaw, mBuoySearchSweepDegrees, mBuoySearchSweepSpeed, mBuoySweepForwardTime, mBuoySweepForwardSpeed, mBuoySweepInitDirection, mBuoySweepSmoothSpeed);
    }
    else
    {
        std::cout << "NO SEARCH TYPE EXISTS TO INITIALIZE!" << std::endl;
    }
}

void Mission::SearchStep()
{

}
*/
