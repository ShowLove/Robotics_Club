#include<opencv/cv.h>
#include<stdio.h>
#include<string.h>

#include <cxutils/math/cxmath.h>
#include <cxutils/math/matrix.h>

#ifndef _VISION_PROJ
#define _VISION_PROJ

namespace Zebulon
{
    namespace Vision
    {
        class VisionProj
        {
            public:
                VisionProj();
                ~VisionProj();
                
                //meters in (x,y,z)
                //pixels in (j,i,don't care)
                void AddPoints(CxUtils::Point3D meter,CxUtils::Point3D pixel);
                void Calculate();
                
                CxUtils::Point3D MetersToPixels(CxUtils::Point3D meter);
                CxUtils::Point3D PixelsToMeters(CxUtils::Point3D pixel);
                
            protected:
            
                CxUtils::Point3D::List mMeters;
                CxUtils::Point3D::List mPixels;
                
                //linear projection
                double mMtPConst[6];
                double mPtMConst[6];
        };
    }
}
#endif