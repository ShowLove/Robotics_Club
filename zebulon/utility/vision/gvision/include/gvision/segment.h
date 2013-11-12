#include<opencv/cv.h>
#include<stdio.h>
#include<string.h>
#include "gvision/return.h"
#include "gvision/target.h"

#ifndef _SEGMENT
#define _SEGMENT

//2^16
//#define MAXSEGMENTS 65536 

//640x480
#define MAXSEGMENTS 307200

#define P1 9973
#define P2 99991
#define P3 999983

#define DEFAULTMINAREA 50
//zero is don't combine
#define DEFAULTCOMBODIST 0

#define CENDIST
//#define BBOX

#define DRAWTOP 1

namespace Zebulon
{
    namespace Vision
    {

        struct pixelData
        {
            int posJ;
            int posI;
            double redColor;
            double greenColor;
            double blueColor;
        };

        //might max this a class and smarter
        struct SegNode
        {
            int id;
            int valid;
            
            double CentroidI;
            double CentroidJ;
            
            int Area;
            
            int MinI;
            int MinJ;
            int MaxI;
            int MaxJ;
            
            //extra items for some math, simplify later with above
            //from data
            int N;
            double SumI;
            double SumJ;
            double SumIJ;
            double SSumI;
            double SSumJ;
            
            //calculated
            double MeanI;
            double MeanJ;
            double DI;
            double DJ;
            double NIJ;
            double Angle;

            std::vector <pixelData> fullData;
            cv::Vec3f avgColor;

        };

        class Segment
        {
            public:
                Segment();
                ~Segment();
            
            void Setup(int Red,int Green,int Blue,int MinArea=DEFAULTMINAREA,int ComboDist=DEFAULTCOMBODIST, int DrawNum=DRAWTOP);
            void Setup(Vision::Target target,int MinArea=DEFAULTMINAREA,int ComboDist=DEFAULTCOMBODIST, int DrawNum=DRAWTOP);
            void Process(const IplImage *Input,IplImage *&Output);
            void Process(const cv::Mat rawImg, const cv::Mat &Input, cv::Mat &Output);
            
            
            //should mutex later
            //int frame, in image coordinates
            void GetMaxPosition(double &Jpos,double &Ipos,int &Area,int &Valid,double &Angle,int Place=0);
            void GetMaxPosition(Return &Ret,int Place=0);
            void GetMaxPosition(double &Jpos,double &Ipos,int &Area,int &Valid,double &Angle,std::vector<pixelData> &FullData,cv::Vec3f &AvgColor,int Place=0);
            
            unsigned short *GetSegMap();
            int GetNumSeg();

            private:
                IplImage *output;

            bool first;
            
            int numsegments;
            int countsegments;
            SegNode allsegments[MAXSEGMENTS];
            unsigned short *segmap;
            unsigned short *cleansegmap;
            
            SegNode tmpsegments[MAXSEGMENTS];
            int tmpnumsegments;
            
            int numpoints;
            int pointlistx[MAXSEGMENTS];
            int pointlisty[MAXSEGMENTS];
            
            
            int maxarea;
            int maxsegment;
            int combodist;
            int drawnum;
                
            int minarea;
                
            int rr,rg,rb;

            int numvalid;
            SegNode validsegments[MAXSEGMENTS];

            int imagewidth;
            int imageheight;

            
            void CombineSegments(SegNode &Master,SegNode &Old);
            
            
        };
    }
}
#endif
