//Cheap-o Hydrophone simulator for mapping
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define SpeedOfSound 1497.0
#define Frequency 23000.0
#define HydroRadius (1.5/100.0)
#define WaveLength (SpeedOfSound/Frequency)

#define NUMHYDRO 5

//#define NUMHYDRO 3

//X Forward, Y right
struct HydroPhone
{
    double x;
    double y;
    double z;
    
    double dist;
    double phase;
};
struct HydroPhone HydroPhones[NUMHYDRO];

struct DiffPhone
{
    double PhaseDiff;
    double RawDistDiff;
    double DistDiff;   //fixed for max value
    double Angle;
    
    double IAngle;
};
struct DiffPhone DiffPhones[NUMHYDRO-1];

void PlaceHydro()
{
    HydroPhones[0].x=HydroRadius;
    HydroPhones[0].y=0;
    HydroPhones[0].z=0;
    
    HydroPhones[1].x=0;
    HydroPhones[1].y=HydroRadius;
    HydroPhones[1].z=0;
    
    HydroPhones[2].x=-HydroRadius;
    HydroPhones[2].y=0;
    HydroPhones[2].z=0;
    
    HydroPhones[3].x=0;
    HydroPhones[3].y=-HydroRadius;
    HydroPhones[3].z=0;
    
    HydroPhones[4].x=0;
    HydroPhones[4].y=0;
    HydroPhones[4].z=0;
}

void CalcPhase(double x,double y,double z)
{
    int i;
    for(i=0;i<NUMHYDRO;i++)
    {
        HydroPhones[i].dist=sqrt(pow(HydroPhones[i].x-x,2)+pow(HydroPhones[i].y-y,2)+pow(HydroPhones[i].z-z,2));
        HydroPhones[i].phase=fmod(HydroPhones[i].dist,WaveLength)/WaveLength*2*M_PI;
        if(HydroPhones[i].phase>M_PI)
        {
            HydroPhones[i].phase-=2*M_PI;
        }
        
    }
}

void CalcDiff()
{
    int i;
    for(i=0;i<NUMHYDRO-1;i++)
    {
        DiffPhones[i].PhaseDiff=HydroPhones[i].phase-HydroPhones[NUMHYDRO-1].phase;
        //DiffPhones[i].PhaseDiff=HydroPhones[i].phase-HydroPhones[(i+1)%NUMHYDRO].phase;
        /*if(i<2)
        {
            DiffPhones[i].PhaseDiff=HydroPhones[i].phase-HydroPhones[3].phase;
        }
        else
        {
            DiffPhones[i].PhaseDiff=HydroPhones[i+1].phase-HydroPhones[3].phase;
        }*/
        //force -pi to pi
        while(DiffPhones[i].PhaseDiff <= -M_PI)
        {
            DiffPhones[i].PhaseDiff += 2*M_PI;
        }
        while(DiffPhones[i].PhaseDiff > M_PI)
        {
            DiffPhones[i].PhaseDiff -= 2*M_PI;
        }
        
        DiffPhones[i].RawDistDiff = WaveLength*DiffPhones[i].PhaseDiff/(2*M_PI);
        DiffPhones[i].DistDiff = DiffPhones[i].RawDistDiff;
        //fudge to fix for math
        if(DiffPhones[i].DistDiff>HydroRadius)
        {
            DiffPhones[i].DistDiff=HydroRadius;
        }
        if(DiffPhones[i].DistDiff<-HydroRadius)
        {
            DiffPhones[i].DistDiff=-HydroRadius;
        }
        
        
        
        DiffPhones[i].Angle=asin(DiffPhones[i].DistDiff/HydroRadius);
        
        
        
        /*DiffPhones[i].Angle=acos(DiffPhones[i].DistDiff/HydroRadius);
        if(HydroPhones[NUMHYDRO-1].dist==0.0)
        {
            DiffPhones[i].IAngle=0.0;
        }
        else
        {
            DiffPhones[i].IAngle=M_PI-acos((pow(HydroPhones[NUMHYDRO-1].dist,2)+pow(HydroRadius,2)-pow(HydroPhones[i].dist,2))/(2*HydroPhones[NUMHYDRO-1].dist*HydroRadius));
        }*/
        
        //printf("%lf vs %lf = %lf\n",DiffPhones[i].Angle,DiffPhones[i].IAngle,fabs(DiffPhones[i].Angle-DiffPhones[i].IAngle)); 
    }
}

struct DiffPhone MeasPhones[NUMHYDRO-1];

void LoopPinger()
{
    double x,y,z;
    double a;
    int i;
    double step=0.25;
    double dist;
    double vangle;
    //Loop through posistion
    for(z=4.5;z<=4.5;z+=step)
    {
        for(y=-15.0;y<=15.0;y+=step)
        {
            for(x=-15.0;x<=15.0;x+=step)
            {
                a=atan2(y,x);
                dist=sqrt(x*x+y*y);
                vangle=atan2(z,dist);
                CalcPhase(x,y,z);
                CalcDiff();
                
                //output of hydrodiff
                for(i=0;i<NUMHYDRO-1;i++)
                {
                    //printf("%lf,",DiffPhones[i].PhaseDiff);
		    if(i!=2)
                    {
                        printf("%lf,",DiffPhones[i].DistDiff);
                    }
                    else
                    {
                        printf("0,");
                    }
                    //printf("%lf,",DiffPhones[i].Angle);
                }
                //placement of pinger
                printf("%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",x,y,z,a,sin(a),cos(a),dist,vangle);
                printf("\n");
            }
        }
    }
}
void LinearPinger()
{
    double x,y,z;
    double a;
    int i;
    double step=0.25;
    double dist;
    double vangle;
    //Loop through posistion
    for(z=4.5;z<=4.5;z+=step)
    {
        for(y=-15.0;y<=15.0;y+=step)
        {
            for(x=-15.0;x<=15.0;x+=step)
            {
                a=atan2(y,x);
                dist=sqrt(x*x+y*y);
                vangle=atan2(z,dist);
                CalcPhase(x,y,z);
                CalcDiff();
                
                printf("%lf,%lf",a,dist);
                //output of hydrodiff
                for(i=0;i<NUMHYDRO-1;i++)
                {
                    //printf("%lf,",DiffPhones[i].PhaseDiff);
                    //printf("%lf,",DiffPhones[i].DistDiff);
                    printf(",%lf",DiffPhones[i].Angle);
                }
                //placement of pinger
                //printf("%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",x,y,z,a,sin(a),cos(a),dist,vangle);
                printf("\n");
            }
        }
    }
}

double ba;
double bdist;
void FindPinger()
{
    double x,y,z;
    double a;
    int i;
    double step=0.25;
    double dist;
    double vangle;
    ba=0;
    bdist=1e6;
    z=5;
    //Loop through posistion
    //for(z=5;z<=5;z+=step)
    //for(z=0.0;z<=0.0;z+=step)
    int tryc=10000;
    while(tryc>0)
    {
        tryc--;
        
        x=rand()%30-15;
        y=rand()%30-15;
        //for(y=-15.0;y<=15.0;y+=step)
        {
            //for(x=-15.0;x<=15.0;x+=step)
            {
                a=atan2(y,x);
                dist=sqrt(x*x+y*y);
                vangle=atan2(z,dist);
                CalcPhase(x,y,z);
                CalcDiff();
                double dist=0;
                //output of hydrodiff
                for(i=0;i<NUMHYDRO-1;i++)
                {
                    //printf("%lf,",DiffPhones[i].PhaseDiff);
                    //printf("%lf,",DiffPhones[i].DistDiff);
                    dist+=sqrt(pow(sin(DiffPhones[i].PhaseDiff)-sin(MeasPhones[i].PhaseDiff),2)+pow(cos(DiffPhones[i].PhaseDiff)-cos(MeasPhones[i].PhaseDiff),2));
                    //dist+=fabs(DiffPhones[i].DistDiff-MeasPhones[i].DistDiff);
                    //dist+=sqrt(pow(sin(DiffPhones[i].Angle)-sin(MeasPhones[i].Angle),2)+pow(cos(DiffPhones[i].Angle)-cos(MeasPhones[i].Angle),2));
                    //printf("%lf,",DiffPhones[i].Angle);
                }
                if(dist<bdist)
                {
                    bdist=dist;
                    ba=a;
                }
                //placement of pinger
                //printf("%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",x,y,z,a,sin(a),cos(a),dist,vangle);
                //printf("\n");
            }
        }
    }
}

void LoadFile(char *filename)
{
    double score=0;
    int i;
    FILE *instream=fopen(filename,"r");
    char tmpline[1024];
    int count=0;
    int order[NUMHYDRO];
    for(i=0;i<NUMHYDRO;i++)
    {
        order[i]=i;
    }
    /*for(i=0;i<NUMHYDRO;i++)
    {
        int r=rand()%NUMHYDRO;
        int t;
        t=order[r];
        order[r]=order[i];
        order[i]=t;
        
    }*/
    
    while(fgets(tmpline,1024,instream)!=NULL)
    {
        double yaw,heading;
        sscanf(tmpline,"%lf %lf %lf %lf %lf %lf %lf",&HydroPhones[0].phase,&HydroPhones[1].phase,&HydroPhones[2].phase,&HydroPhones[3].phase,&HydroPhones[4].phase
                                                      ,&yaw,&heading);
        //printf("%lf %lf %lf %lf %lf\n",HydroPhones[0].phase,HydroPhones[1].phase,HydroPhones[2].phase,HydroPhones[3].phase,HydroPhones[4].phase);
        //flip around the front one

        CalcDiff();
        //output of hydrodiff
        for(i=0;i<NUMHYDRO-1;i++)
        {
            //printf("%lf ",DiffPhones[i].PhaseDiff);
            MeasPhones[i]=DiffPhones[i];
            //printf("%lf,",DiffPhones[i].DistDiff);
            //printf("%lf,",DiffPhones[i].Angle);
        }
  
        FindPinger();
        double g=heading+ba*180/M_PI;
        while(g>180.0)
        {
            g-=360;
        }
        while(g<-180.0)
        {
            g+=360;
        }
        //throw out data
        if(bdist>.2 && bdist<0.9 && fabs(fmod(ba,M_PI/2.0))>0.01)
        {
            printf("Ba %lf: %lf %lf %lf %lf\n",bdist,ba*180/M_PI,yaw,heading,g);
            fflush(stdout);
            score+=g*g;
            count++;
        }
        fflush(stdout);
        //printf("\n");
    }
    score/=count;
    score=sqrt(score);
    fprintf(stderr,"Score: %lf\n",score);
    
    fclose(instream);
}


int main()
{
    srand(time(NULL));
    PlaceHydro();
    LoopPinger();
    //LinearPinger();
    //LoadFile("phase.log");
    return 0;
}
