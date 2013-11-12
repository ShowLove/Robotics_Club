#include <mixer/mixer.h>

using namespace Zebulon;
using namespace Utility;

Mixer::Mixer()
{
    mA=0.0;
    mB=0.0;
    mX=0.0;
    mY=0.0;
    mMin=-100.0;
    mMax=100.0;
    mType=Halved;
}

Mixer::~Mixer()
{
    
}

void Mixer::Setup(MixingType Type,double Min,double Max)
{
    mType=Type;
    mMin=Min;
    mMax=Max;
}
                    
int Mixer::SetA(double A)
{
    mutex.Lock();
    //mA=Saturate(A);
    mA=A;
    mutex.Unlock();
    return 0;
}
int Mixer::SetB(double B)
{
    mutex.Lock();
    //mB=Saturate(B);
    mB=B;
    mutex.Unlock();
    return 0;
}

double Mixer::GetX()
{
    double T;
    Mix();
    mutex.Lock();
    T=mX;
    mutex.Unlock();
    return T;
}

double Mixer::GetY()
{
    double T;
    Mix();
    mutex.Lock();
    T=mY;
    mutex.Unlock();
    return T;
}

void Mixer::Mix()
{
    //Simple Mixing
    mutex.Lock();
    if(mType == Halved)
    {
        mX=mA/2.0+mB/2.0;
        mY=mA/2.0-mB/2.0;
    }
    else if(mType == Saturated)
    {
        mX=mA+mB;
        mY=mA-mB;
    }
    
    mX=Saturate(mX);
    mY=Saturate(mY);
    
    mutex.Unlock();
}

double Mixer::Saturate(double Val)
{
    if(Val>mMax)
    {
        Val=mMax;
    }
    else if(Val<mMin)
    {
        Val=mMin;
    }
    return Val;
}
