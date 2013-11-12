#include "virtualvelocity/gpsvelocity.h"

using namespace Zebulon;
using namespace Utility;

GPSVelocity::GPSVelocity()
{
    mAxialVelocity=0;
    mLateralVelocity=0;
    mReady=0;
    mCount=0;
    mFoundOld=0;
    
    mListPos=0;
    
    mFilterX=new AverageFilter(10);
    mFilterY=new AverageFilter(10);

    //compass good, no filtering?
    mFilterH=new AverageFilter(1);
    mFilterH->Setup(-M_PI,M_PI);

    mFilterC=new AverageFilter(5);
    mFilterC->Setup(-M_PI,M_PI);
    
    mFilterA=new AverageFilter(5);
    mFilterL=new AverageFilter(5);
  
}

GPSVelocity::~GPSVelocity()
{
    delete mFilterX;
    delete mFilterY;
    delete mFilterH;
    delete mFilterC;
    delete mFilterA;
    delete mFilterL;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Every time the GPS updates, this function should be called to update
///         the current location with this function
///   \param[in] time the current time
///   \param[in] easting gps easting value
///   \param[in] northing gps northing value
///   \param[in] current heading (in radians)
///   
////////////////////////////////////////////////////////////////////////////////////
void GPSVelocity::SetValues(double time,double easting,double northing,double heading)
{
    double tmp;
    mFilterX->Add(easting);
    mFilterY->Add(northing);
    mFilterH->Add(heading);
    mTime=time;
    
    if(mFilterX->Output(tmp)==1)
    {
        if(Calc()==1)
        {
            mReady=1;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the forward velocity
///   \return returns the current axial (x) velocity
///   
////////////////////////////////////////////////////////////////////////////////////
double GPSVelocity::GetAxialVelocity()
{
    return mAxialVelocity;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the lateral velocity
///   \return returns the current lateral (y) velocity
///   
////////////////////////////////////////////////////////////////////////////////////
double GPSVelocity::GetLateralVelocity()
{
    return mLateralVelocity;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the easting value
///   \return returns the easting value
///   
////////////////////////////////////////////////////////////////////////////////////
double GPSVelocity::GetEasting()
{
    return mEasting;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the northing value
///   \return returns the northing value
///   
////////////////////////////////////////////////////////////////////////////////////
double GPSVelocity::GetNorthing()
{
    return mNorthing;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Does all the heavy lifting, calculates and updates the velocity
///   \return returns 0 on failure, 1 on success
///   
////////////////////////////////////////////////////////////////////////////////////
int GPSVelocity::Calc()
{
    mFilterX->Output(mEasting);
    mFilterY->Output(mNorthing);
    mFilterH->Output(mHeading);

    //check for same time stamp
    if(mListPos>0 && mTimeList[(mListPos-1)%MaxList]>=mTime)
    {
        return 0;
    }
    mEastingList[mListPos%MaxList]=mEasting;
    mNorthingList[mListPos%MaxList]=mNorthing;
    mTimeList[mListPos%MaxList]=mTime;
    mListPos++;
        
    /*if(mOldReady==0)
    {
        mOldEasting=mEasting;
        mOldNorthing=mNorthing;
        mOldTime=mTime;
        mOldReady=1;
        return 0;
    }*/
    
    //search through looking for first point MinDistance behind
    int i;
    for(i=0;i<MaxList;i++)
    {
        int pos=mListPos-1-i;
        if(pos<0)
        {
            mFoundOld=0;
            break; 
        }
        double dist=CalcDistance(mEastingList[pos%MaxList],mNorthingList[pos%MaxList],mEasting,mNorthing);
        if(dist>MinDistance)
        {
            mOldEasting=mEastingList[pos%MaxList];
            mOldNorthing=mNorthingList[pos%MaxList];
            mOldTime=mTimeList[pos%MaxList];
            mFoundOld=1;
            break;
        }
    }
    if(i==MaxList)
    {
        mFoundOld=0;
    }

    double a,l;
    //if didn't find a point outside of min, must have not moved
    if(mFoundOld==0)
    {
        a=0.0;
        l=0.0;
        //fprintf(stderr,"Did not find old: %lf\n",mTime);
    }
    else
    {
        //fix time instead of distance
        
        
        double dist=CalcDistance(mOldEasting,mOldNorthing,mEasting,mNorthing);
        double VelMag=dist/(mTime-mOldTime);
        double VelAngle=CalcAngle(mOldEasting,mOldNorthing,mEasting,mNorthing);
        
        /*mFilterC->Add(VelAngle);
        if(mFilterC->Output(VelAngle)==0)
        {
            return 0;
        }*/
        
        if(mTime<=mOldTime)
        {
           //fprintf(stderr,"VelProb: Same Time\n");
        }
        //printf("DEBUG: %lf %lf %lf\n",dist,VelMag,VelAngle);
        //printf("DEBUG: %lf %lf %lf %lf\n",mOldEasting,mOldNorthing,mEasting,mNorthing);
        
        a=VelMag*(cos(mHeading)*cos(VelAngle)+sin(mHeading)*sin(VelAngle));
        l=VelMag*(-sin(mHeading)*cos(VelAngle)+cos(mHeading)*sin(VelAngle));
        /*if(mCount%10==0)
        {
            printf("H %lf C %lf - %lf %lf\n",mHeading*180.0/M_PI,VelAngle*180.0/M_PI,a,l);
        }*/
        
        mCount++;
    }

    mFilterA->Add(a);
    mFilterL->Add(l);
    
    if(mFilterA->Output(a)==0)
    {
        return 0;
    }
    mFilterA->Output(mAxialVelocity);
    mFilterL->Output(mLateralVelocity);
    
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks if the object is ready
///   \return returns 0 if not ready, 1 otherwise
///   
////////////////////////////////////////////////////////////////////////////////////
int GPSVelocity::IsReady()
{
    return mReady;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates the distance between two points
///   \param[in] e1 first easting value
///   \param[in] n1 first northing value
///   \param[in] e2 second easting value
///   \param[in] n2 second northing value
///   \return returns the distance as a double
///   
////////////////////////////////////////////////////////////////////////////////////
double GPSVelocity::CalcDistance(double e1,double n1,double e2,double n2)
{
    return sqrt((e2-e1)*(e2-e1)+(n2-n1)*(n2-n1));
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates the angle between two points
///   \param[in] e1 first easting value
///   \param[in] n1 first northing value
///   \param[in] e2 second easting value
///   \param[in] n2 second northing value
///   \return returns the angle as a double in radians
///   
////////////////////////////////////////////////////////////////////////////////////
double GPSVelocity::CalcAngle(double e1,double n1,double e2,double n2)
{
    return atan2((e2-e1),(n2-n1));
}