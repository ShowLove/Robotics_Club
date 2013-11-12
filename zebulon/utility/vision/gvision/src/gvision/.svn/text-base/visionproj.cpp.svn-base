#include "gvision/visionproj.h"

using namespace Zebulon;
using namespace Vision;
VisionProj::VisionProj()
{
    
    
}
VisionProj::~VisionProj()
{
    
}

void VisionProj::AddPoints(CxUtils::Point3D meter,CxUtils::Point3D pixel)
{
    mMeters.push_back(meter);
    mPixels.push_back(pixel);
    
}
void VisionProj::Calculate()
{
    CxUtils::Matrix metersMatrix(mMeters.size(),3);
    for(int i=0;i<mMeters.size();i++)
    {
        metersMatrix[i][0]=mMeters[i].mX;
        metersMatrix[i][1]=mMeters[i].mY;
		metersMatrix[i][2]=1.0;
    }
    
    CxUtils::Matrix pixelsMatrix(mPixels.size(),3);
    for(int i=0;i<mPixels.size();i++)
    {
        pixelsMatrix[i][0]=mPixels[i].mX;
        pixelsMatrix[i][1]=mPixels[i].mY;
        pixelsMatrix[i][2]=1.0;
    }
    
    CxUtils::Matrix Output(mMeters.size(),1);
    CxUtils::Matrix Constants;
    
    CxUtils::Matrix metersPinv=((metersMatrix.Transpose()*metersMatrix).Inverse())*metersMatrix.Transpose();
    CxUtils::Matrix pixelsPinv=((pixelsMatrix.Transpose()*pixelsMatrix).Inverse())*pixelsMatrix.Transpose();
    
    //Do J
    for(int i=0;i<mPixels.size();i++)
    {
        Output[i][0]=mPixels[i].mX;
    }
    Constants=metersPinv*Output;
    mMtPConst[0]=Constants[0][0];mMtPConst[1]=Constants[1][0];mMtPConst[2]=Constants[2][0];
    
    //Do I
    for(int i=0;i<mPixels.size();i++)
    {
        Output[i][0]=mPixels[i].mY;
    }
    Constants=metersPinv*Output;
    mMtPConst[3]=Constants[0][0];mMtPConst[4]=Constants[1][0];mMtPConst[5]=Constants[2][0];
    
    for(int i=0;i<6;i++)
    {
        printf("%lf ",mMtPConst[i]);
    }
    printf("\n");
    
    //Do X
    for(int i=0;i<mMeters.size();i++)
    {
        Output[i][0]=mMeters[i].mX;
    }
    Constants=pixelsPinv*Output;
    mPtMConst[0]=Constants[0][0];mPtMConst[1]=Constants[1][0];mPtMConst[2]=Constants[2][0];
    
    //Do Y
    for(int i=0;i<mMeters.size();i++)
    {
        Output[i][0]=mMeters[i].mY;
    }
    Constants=pixelsPinv*Output;
    mPtMConst[3]=Constants[0][0];mPtMConst[4]=Constants[1][0];mPtMConst[5]=Constants[2][0];
    
    for(int i=0;i<6;i++)
    {
        printf("%lf ",mPtMConst[i]);
    }
    printf("\n");
}

CxUtils::Point3D VisionProj::MetersToPixels(CxUtils::Point3D meter)
{
    CxUtils::Point3D tmp;
    tmp.mX=mMtPConst[0]*meter.mX+mMtPConst[1]*meter.mY+mMtPConst[2];
    tmp.mY=mMtPConst[3]*meter.mX+mMtPConst[4]*meter.mY+mMtPConst[5];
    tmp.mZ=0.0;
    return tmp;
}
CxUtils::Point3D VisionProj::PixelsToMeters(CxUtils::Point3D pixel)
{
    CxUtils::Point3D tmp;
    tmp.mX=mPtMConst[0]*pixel.mX+mPtMConst[1]*pixel.mY+mPtMConst[2];
    tmp.mY=mPtMConst[3]*pixel.mX+mPtMConst[4]*pixel.mY+mPtMConst[5];
    tmp.mZ=0.0;
    return tmp;
}