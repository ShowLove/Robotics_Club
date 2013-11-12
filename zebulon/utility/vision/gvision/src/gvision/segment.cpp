#include "gvision/segment.h"
#include <cxutils/math/cxmath.h>

using namespace Zebulon;
using namespace Vision;

#ifdef WIN32
    #define M_PI CxUtils::CX_PI
#endif

Segment::Segment()
{
    output=NULL;
    rr=255;
    rg=255;
    rb=255;

    first = true;
}

Segment::~Segment()
{
    if(output!=NULL)
    {
        cvReleaseImage(&output);
        delete [] segmap;
        delete [] cleansegmap;
    }
    minarea=DEFAULTMINAREA;
}

void Segment::Setup(int Red,int Green,int Blue,int MinArea,int ComboDist, int DrawNum)
{
    rr=Red;
    rg=Green;
    rb=Blue;
    minarea=MinArea;
    combodist=ComboDist;
    drawnum=DrawNum;
}

void Segment::Setup(Vision::Target target,int MinArea,int ComboDist, int DrawNum)
{
    Setup(target.mRed,target.mGreen,target.mBlue,MinArea,ComboDist,DrawNum);
}
void Segment::Process(const IplImage *Input,IplImage *&Output)
{
    if(Input==NULL)
    {
        printf("threshhold image corrupt\n");
        return;
    }

    if(output==NULL)
    {
        //create "grey" image of threshhold
        //printf("Input %dx%d\n",Input->width,Input->height);
        output=cvCreateImage(cvSize(Input->width,Input->height),IPL_DEPTH_8U,3);
        segmap=new unsigned short[output->width*output->height];
        cleansegmap = new unsigned short[output->width*output->height];
        imagewidth=output->width;
        imageheight=output->height;
    }

    //clear the threshhold
    //memset(output->imageData,0,3*(output->width*output->height));
    memcpy(output->imageData,Input->imageData,output->imageSize);
    //clear the segmap
    memset(segmap,0,sizeof(unsigned short)*(output->width*output->height));

    //reset segment, memory dirty, make sure to set everything
    int i,j;
    int pos;
    int segpos;
    int top,toppos;
    int id,nid;

    unsigned char *imageData=(unsigned char *)Input->imageData;

    //zero never happens
    numsegments=1;
    countsegments=0;

    maxarea=0;
    maxsegment=-1;

    //pass two not finished
    //#define RLETWO
#ifdef RLETWO
    //printf("%dx%d -> %d\n",output->width,output->height,output->widthStep);
    tmpnumsegments=0;
    //first pass is the columns
    for(j=0;j<output->height;j++)
    {
        for(i=0;i<output->width;i++)
        {
            pos=j*output->widthStep+3*i;
            //printf("%d: %d\n",pos,(int)Input->imageData[pos]);

            //do run length
            //assume not a segment

            if(imageData[pos]==255)
            {
                //printf("%d: ",numsegments);
                tmpsegments[tmpnumsegments].valid=1;
                tmpsegments[tmpnumsegments].id=tmpnumsegments;
                tmpsegments[tmpnumsegments].MaxJ=j;

                tmpsegments[tmpnumsegments].MinI=i;
                tmpsegments[tmpnumsegments].Area=0;

                //loop until segment is over
                while(i<output->width && imageData[pos]==255)
                {
                    i++;
                }
                tmpsegments[tmpnumsegments].MaxI=i-1;
                tmpnumsegments++;
            }
        }
    }

    printf("tmp: %d\n",tmpnumsegments);

    int ei=0,ej=1;

    j=0;
    i=0;

    //skip to next line
    for(;tmpsegments[ei].MaxJ==ei;ei++);







#endif

#define RLE
#ifdef RLE
    //printf("%dx%d -> %d\n",output->width,output->height,output->widthStep);

    //first pass is the columns
    for(j=0;j<output->height;j++)
    {
        for(i=0;i<output->width;i++)
        {
            pos=j*output->widthStep+3*i;
            //printf("%d: %d\n",pos,(int)Input->imageData[pos]);

            //do run length
            //assume not a segment

            if(imageData[pos]==255)
            {
                //printf("%d: ",numsegments);
                //not valid till size is big enough
                allsegments[numsegments].valid=0;
                allsegments[numsegments].id=numsegments;
                allsegments[numsegments].MinJ=j;
                allsegments[numsegments].MaxJ=j;

                allsegments[numsegments].MinI=i;
                allsegments[numsegments].MaxI=i;
                allsegments[numsegments].Area=0;

                //New Code for line finding and stats
                allsegments[numsegments].N=0;
                allsegments[numsegments].SumI=0;
                allsegments[numsegments].SumJ=0;
                allsegments[numsegments].SumIJ=0;
                allsegments[numsegments].SSumI=0;
                allsegments[numsegments].SSumJ=0;

                top=0;
                id=0;
                nid=0;

                toppos=(j-1)*output->width+i;
                segpos=j*output->width+i;

                //loop until segment is over
                while(i<output->width && imageData[pos]==255)
                {
                    allsegments[numsegments].MaxI=i;
                    allsegments[numsegments].Area++;

                    //new code
                    allsegments[numsegments].N++;
                    allsegments[numsegments].SumI+=i;
                    allsegments[numsegments].SumJ+=j;
                    allsegments[numsegments].SumIJ+=i*j;
                    allsegments[numsegments].SSumI+=i*i;
                    allsegments[numsegments].SSumJ+=j*j;

                    segmap[segpos]=allsegments[numsegments].id;
                    if(j>0)
                    {
                        top=segmap[toppos];
                        //printf("%d %d\n",toppos,top);
                        //printf("%dx%d %dx%d\n",j-1,i,j,i);

                        //segment set, analyze
                        if(top>0)
                        {
                            int count=0;
                            while(allsegments[top].id!=top)
                            {
                                count++;
                                top=allsegments[top].id;
                                if(count>100)
                                {
                                    printf("Fail: %d %d\n",top,allsegments[top].id);
                                    fflush(stdout);
                                    break;
                                }
                            }
                            nid=allsegments[top].id;
                            if(nid>0)
                            {
                                if(id==0)
                                {
                                    id=nid;
                                }
                                if(id!=nid)
                                {
                                    if(id>nid)
                                    {
                                        CombineSegments(allsegments[nid],allsegments[id]);
                                        id=nid;
                                    }
                                    else
                                    {
                                        CombineSegments(allsegments[id],allsegments[nid]);
                                    }
                                    if(allsegments[id].Area>maxarea)
                                    {
                                        maxarea=allsegments[id].Area;
                                        maxsegment=allsegments[id].id;
                                    }
                                    if(allsegments[id].Area>=minarea)
                                    {
                                        allsegments[id].valid=1;
                                    }

                                    countsegments--;
                                }
                            }
                        }
                    }
                    i++;
                    pos=j*output->widthStep+3*i;
                    toppos=(j-1)*output->width+i;
                    segpos=j*output->width+i;
                }
                //Centriod is center for a single line
                allsegments[numsegments].CentroidJ=j;
                allsegments[numsegments].CentroidI=(allsegments[numsegments].MaxI+allsegments[numsegments].MinI)/2.0;
                if(id!=0)
                {
                    CombineSegments(allsegments[id],allsegments[numsegments]);
                }
                else
                {
                    id=numsegments;
                    countsegments++;
                }

                if(allsegments[id].Area>maxarea)
                {
                    maxarea=allsegments[id].Area;
                    maxsegment=allsegments[id].id;
                }
                if(allsegments[id].Area>=minarea)
                {
                    allsegments[id].valid=1;
                }

                //printf(" %d %d %d %d",allsegments[numsegments].MaxJ,allsegments[numsegments].MinI,allsegments[numsegments].MaxI,allsegments[numsegments].Area);

                //printf("\n");
                numsegments++;
            }
        }
    }
#endif
//    #define FLOOD
#ifdef FLOOD
    int x,y;
    int currpoint;

    //pointlistx
    //non-recursive flood fill
    for(j=0;j<output->height;j++)
    {
        for(i=0;i<output->width;i++)
        {
            pos=j*output->widthStep+3*i;
            segpos=j*output->width+i;
            //found an unmarked point
            if(imageData[pos]==255 && segmap[segpos]==0)
            {
                numpoints=0;
                //use the stack
                pointlistx[numpoints]=i;
                pointlisty[numpoints]=j;
                allsegments[numsegments].Area=0;
                allsegments[numsegments].CentroidI=0.0;
                allsegments[numsegments].CentroidJ=0.0;
                allsegments[numsegments].MinI=i;
                allsegments[numsegments].MaxI=i;
                allsegments[numsegments].MinJ=j;
                allsegments[numsegments].MaxJ=j;

                allsegments[numsegments].id=numsegments;
                allsegments[numsegments].valid=0;
                numpoints++;
                currpoint=0;
                //while still points in the list
                while(currpoint<numpoints)
                {
                    //if the point is good, find connected
                    pos=pointlisty[currpoint]*output->widthStep+3*pointlistx[currpoint];
                    segpos=pointlisty[currpoint]*output->width+pointlistx[currpoint];
                    if(imageData[pos]==255 && segmap[segpos]==0)
                    {
                        //value set and counted
                        segmap[segpos]=numsegments;
                        allsegments[numsegments].Area++;
                        allsegments[numsegments].CentroidI+=pointlistx[currpoint];
                        allsegments[numsegments].CentroidJ+=pointlisty[currpoint];
                        if(pointlistx[currpoint]<allsegments[numsegments].MinI)
                        {
                            allsegments[numsegments].MinI=pointlistx[currpoint];
                        }
                        if(pointlisty[currpoint]<allsegments[numsegments].MinJ)
                        {
                            allsegments[numsegments].MinJ=pointlisty[currpoint];
                        }
                        if(pointlistx[currpoint]>allsegments[numsegments].MaxI)
                        {
                            allsegments[numsegments].MaxI=pointlistx[currpoint];
                        }
                        if(pointlisty[currpoint]>allsegments[numsegments].MaxJ)
                        {
                            allsegments[numsegments].MaxJ=pointlisty[currpoint];
                        }

                        //unrolled, for speed?
                        x=pointlistx[currpoint];
                        y=pointlisty[currpoint]-1;
                        if(y>=0)
                        {
                            pos=y*output->widthStep+3*x;
                            segpos=y*output->width+x;
                            if(imageData[pos]==255 && segmap[segpos]==0)
                            {
                                pointlistx[numpoints]=x;
                                pointlisty[numpoints]=y;
                                numpoints++;
                            }
                        }

                        x=pointlistx[currpoint];
                        y=pointlisty[currpoint]+1;
                        if(y<output->height)
                        {
                            pos=y*output->widthStep+3*x;
                            segpos=y*output->width+x;
                            if(imageData[pos]==255 && segmap[segpos]==0)
                            {
                                pointlistx[numpoints]=x;
                                pointlisty[numpoints]=y;
                                numpoints++;
                            }
                        }

                        x=pointlistx[currpoint]-1;
                        y=pointlisty[currpoint];
                        if(x>=0)
                        {
                            pos=y*output->widthStep+3*x;
                            segpos=y*output->width+x;
                            if(imageData[pos]==255 && segmap[segpos]==0)
                            {
                                pointlistx[numpoints]=x;
                                pointlisty[numpoints]=y;
                                numpoints++;
                            }
                        }


                        x=pointlistx[currpoint]+1;
                        y=pointlisty[currpoint];
                        if(x<output->width)
                        {
                            pos=y*output->widthStep+3*x;
                            segpos=y*output->width+x;
                            if(imageData[pos]==255 && segmap[segpos]==0)
                            {
                                pointlistx[numpoints]=x;
                                pointlisty[numpoints]=y;
                                numpoints++;
                            }
                        }


                    }
                    currpoint++;
                }
                //printf("numpoints: %d\n",numpoints);
                allsegments[numsegments].CentroidI/=allsegments[numsegments].Area;
                allsegments[numsegments].CentroidJ/=allsegments[numsegments].Area;
                if(allsegments[numsegments].Area>maxarea)
                {
                    maxarea=allsegments[numsegments].Area;
                    maxsegment=numsegments;
                }
                if(allsegments[numsegments].Area>minarea)
                {
                    allsegments[numsegments].valid=1;
                }

                numsegments++;
            }
        }
    }
#endif

    //combine together - if set

    if(combodist>0)
    {

#ifdef CENDIST
        //1. distance of centroids to combine
        for(i=1;i<numsegments-1;i++)
        {
            if(allsegments[i].valid==0)
            {
                continue;
            }
            for(j=i+1;j<numsegments;j++)
            {
                if(allsegments[j].valid==0)
                {
                    continue;
                }
                //1. distance of centroids to combine
                double dist=fabs(allsegments[i].CentroidI-allsegments[j].CentroidI)+fabs(allsegments[i].CentroidJ-allsegments[j].CentroidJ);
                //printf("%d %d: %lf\n",i,j,dist);
                if(dist<combodist)
                {
                    CombineSegments(allsegments[i],allsegments[j]);
                }
            }
        }
#endif


#ifdef BBOX
        //2. bounding boxes
        for(i=1;i<numsegments-1;i++)
        {
            if(allsegments[i].valid==0)
            {
                continue;
            }
            for(j=i+1;j<numsegments;j++)
            {
                if(allsegments[j].valid==0)
                {
                    continue;
                }

                //check bounding boxes j in i
                if(
                (allsegments[i].MinI <= allsegments[j].MinI && allsegments[j].MinI <= allsegments[i].MaxI
                && allsegments[i].MinJ <= allsegments[j].MinJ && allsegments[j].MinJ <= allsegments[i].MaxJ)
                ||
                (allsegments[i].MinI <= allsegments[j].MaxI && allsegments[j].MaxI <= allsegments[i].MaxI
                && allsegments[i].MinJ <= allsegments[j].MaxJ && allsegments[j].MaxJ <= allsegments[i].MaxJ)
                )
                {
                    CombineSegments(allsegments[i],allsegments[j]);
                }
                //check if i in j
                else if(
                (allsegments[j].MinI <= allsegments[i].MinI && allsegments[i].MinI <= allsegments[j].MaxI
                && allsegments[j].MinJ <= allsegments[i].MinJ && allsegments[i].MinJ <= allsegments[j].MaxJ)
                ||
                (allsegments[j].MinI <= allsegments[i].MaxI && allsegments[i].MaxI <= allsegments[j].MaxI
                && allsegments[j].MinJ <= allsegments[i].MaxJ && allsegments[i].MaxJ <= allsegments[j].MaxJ)
                )
                {
                    CombineSegments(allsegments[i],allsegments[j]);
                }
            }
        }
#endif
    }

    //assume no valid
    numvalid=0;
    //copy in valid members
    for(i=1;i<numsegments;i++)
    {
        if(allsegments[i].valid==1)
        {
            validsegments[numvalid]=allsegments[i];

            //Do new math at the end
            validsegments[numvalid].MeanI=validsegments[numvalid].SumI/validsegments[numvalid].N;
            validsegments[numvalid].MeanJ=validsegments[numvalid].SumJ/validsegments[numvalid].N;


            validsegments[numvalid].DI=validsegments[numvalid].SSumI-2*validsegments[numvalid].MeanI*validsegments[numvalid].SumI+validsegments[numvalid].N*validsegments[numvalid].MeanI*validsegments[numvalid].MeanI;
            validsegments[numvalid].DJ=validsegments[numvalid].SSumJ-2*validsegments[numvalid].MeanJ*validsegments[numvalid].SumJ+validsegments[numvalid].N*validsegments[numvalid].MeanJ*validsegments[numvalid].MeanJ;

            validsegments[numvalid].NIJ=validsegments[numvalid].SumIJ-validsegments[numvalid].MeanJ*validsegments[numvalid].SumI-validsegments[numvalid].MeanI*validsegments[numvalid].SumJ+validsegments[numvalid].N*validsegments[numvalid].MeanI*validsegments[numvalid].MeanJ;

            if(validsegments[numvalid].DI>validsegments[numvalid].DJ)
            {
                //validsegments[numvalid].Angle=180.0/M_PI*atan2(validsegments[numvalid].NIJ,validsegments[numvalid].DI);
                validsegments[numvalid].Angle=-180.0/M_PI*atan2(validsegments[numvalid].DI,validsegments[numvalid].NIJ);
            }
            else
            {
                //validsegments[numvalid].Angle=180.0/M_PI*atan2(validsegments[numvalid].DJ,validsegments[numvalid].NIJ);
                validsegments[numvalid].Angle=-180.0/M_PI*atan2(validsegments[numvalid].NIJ,validsegments[numvalid].DJ);
            }

            if(validsegments[numvalid].Angle>90)
            {
                validsegments[numvalid].Angle-=180;
            }
            if(validsegments[numvalid].Angle<-90)
            {
                validsegments[numvalid].Angle+=180;
            }

            //printf("%lf %lf vs %lf %lf\n", validsegments[numvalid].CentroidJ,validsegments[numvalid].CentroidI,validsegments[numvalid].MeanJ,validsegments[numvalid].MeanI);


            numvalid++;
        }
    }
    //printf("A%d V%d\n",numsegments,numvalid);

    SegNode tmpNode;
    int invert=1;
    //bubble sort the valid?
    for(j=0;j<numvalid-1 && invert==1;j++)
    {
        invert=0;
        for(i=numvalid-1;i>j;i--)
        {
            if(validsegments[i-1].Area<validsegments[i].Area)
            {
                invert=1;
                tmpNode=validsegments[i-1];
                validsegments[i-1]=validsegments[i];
                validsegments[i]=tmpNode;
            }
        }
    }

#define DRAWOLD
#ifdef DRAWOLD
    int n;

    int numdraw=numvalid;
    if(numdraw>=drawnum)
    {
        numdraw=drawnum;
    }
    for(n=0;n<numdraw;n++)
    {
        for(j=0;j<output->height;j++)
        {
            for(i=0;i<output->width;i++)
            {
                top=segmap[j*output->width+i];


                if(top!=0)
                {
                    int count=0;
                    while(allsegments[top].id!=top)
                    {
                        count++;
                        top=allsegments[top].id;
                        if(count>100)
                        {
                            printf("Fail: %d %d\n",top,allsegments[top].id);
                            fflush(stdout);
                            break;
                        }
                    }


                    if(allsegments[top].id==validsegments[n].id)
                    {
                        //printf("%d  %d %d\n",allsegments[n].id,top,allsegments[segmap[top]]);
                        //cvRectangle(output,cvPoint(i,j),cvPoint(i,j),CV_RGB(0,0,255));

                        pos=j*output->widthStep+3*i;
                        output->imageData[pos+0]=rb;
                        output->imageData[pos+1]=rg;
                        output->imageData[pos+2]=rr;
                    }
                }
            }

        }

#define DRAWEXTRA
#ifdef DRAWEXTRA
        cvRectangle(output,cvPoint(validsegments[n].MinI,validsegments[n].MinJ),cvPoint(validsegments[n].MaxI,validsegments[n].MaxJ),CV_RGB(rr,rg,rb));
        //printf("%lf %lf\n",allsegments[n].CentroidI,allsegments[n].CentroidJ);
        //cvRectangle(output,cvPoint(validsegments[n].CentroidI-10,validsegments[n].CentroidJ-10),cvPoint(validsegments[n].CentroidI+10,validsegments[n].CentroidJ+10),CV_RGB(255,255,255),CV_FILLED);
        cvLine(output,cvPoint(validsegments[n].CentroidI-10,validsegments[n].CentroidJ-10),cvPoint(validsegments[n].CentroidI+10,validsegments[n].CentroidJ+10),CV_RGB(255-rr,255-rg,255-rb),2);
        cvLine(output,cvPoint(validsegments[n].CentroidI+10,validsegments[n].CentroidJ-10),cvPoint(validsegments[n].CentroidI-10,validsegments[n].CentroidJ+10),CV_RGB(255-rr,255-rg,255-rb),2);
        //printf("%d %d\n",validsegments[n].id,validsegments[n].Area);

        //draw the angle
        double drawangle=M_PI/2.0+validsegments[n].Angle*M_PI/180.0;
        cvLine(output,cvPoint(validsegments[n].MeanI-100*cos(drawangle),validsegments[n].MeanJ-100*sin(drawangle)),cvPoint(validsegments[n].MeanI+100*cos(drawangle),validsegments[n].MeanJ+100*sin(drawangle)),CV_RGB(255-rr,255-rg,255-rb),2);
#endif

    }
    //printf("Num segments: %d %d %d\n",numsegments-1,countsegments,numgood);
#endif
        Output=output;
}

void Segment::CombineSegments(SegNode &Master,SegNode &Old)
{

    //printf("Combine: %d %d\n",Master.id,Old.id);
    //immediate invalidate old
    Old.valid=0;
    //master should always be less
    Old.id=Master.id;

    //find extents
    if(Old.MinI<Master.MinI)
    {
        Master.MinI=Old.MinI;
    }
    if(Old.MinJ<Master.MinJ)
    {
        Master.MinJ=Old.MinJ;
    }

    if(Old.MaxI>Master.MaxI)
    {
        Master.MaxI=Old.MaxI;
    }
    if(Old.MaxJ>Master.MaxJ)
    {
        Master.MaxJ=Old.MaxJ;
    }

    //pretty Sure this works
    Master.CentroidI=(Old.CentroidI*Old.Area+Master.CentroidI*Master.Area)/(Old.Area+Master.Area);
    Master.CentroidJ=(Old.CentroidJ*Old.Area+Master.CentroidJ*Master.Area)/(Old.Area+Master.Area);

    Master.avgColor[0] = (Old.avgColor[0]*Old.Area+Master.avgColor[0]*Master.Area)/(Old.Area+Master.Area);
    Master.avgColor[1] = (Old.avgColor[1]*Old.Area+Master.avgColor[1]*Master.Area)/(Old.Area+Master.Area);
    Master.avgColor[2] = (Old.avgColor[2]*Old.Area+Master.avgColor[2]*Master.Area)/(Old.Area+Master.Area);

    Master.Area+=Old.Area;

    Master.fullData.insert(Master.fullData.end(), Old.fullData.begin(), Old.fullData.end());

    //New code
    Master.N+=Old.N;
    Master.SumI+=Old.SumI;
    Master.SumJ+=Old.SumJ;
    Master.SumIJ+=Old.SumIJ;
    Master.SSumI+=Old.SSumI;
    Master.SSumJ+=Old.SSumJ;
}

void Segment::GetMaxPosition(double &Jpos,double &Ipos,int &Area,int &Valid,double &Angle,int Place)
{
    if(numvalid>Place)
    {
        Valid=1;
        //Xpos=(allsegments[Place].CentroidI-output->width/2.0)/(output->width/2.0);
        //Ypos=(allsegments[Place].CentroidJ-output->height/2.0)/(output->height/2.0);
        Ipos=(validsegments[Place].CentroidI);
        Jpos=(validsegments[Place].CentroidJ);
        Area=validsegments[Place].Area;
        Angle=validsegments[Place].Angle;
    }
    else
    {
        Valid=0;
        Ipos=0.0;
        Jpos=0.0;
        Area=0;
        Angle=0.0;
    }
}

void Segment::GetMaxPosition(Return &Ret,int Place)
{
    GetMaxPosition(Ret.mCenterJ,Ret.mCenterI,Ret.mArea,Ret.mValid,Ret.mAngle,Place);
    if(Place<numvalid)
    {
        Ret.mMinI = validsegments[Place].MinI;
        Ret.mMinJ = validsegments[Place].MinJ;
        Ret.mMaxI = validsegments[Place].MaxI;
        Ret.mMaxJ = validsegments[Place].MaxJ;
    }
    else
    {
        Ret.mMinI = 0;
        Ret.mMinJ = 0;
        Ret.mMaxI = 0;
        Ret.mMaxJ = 0;
    }
}

void Segment::GetMaxPosition(double &Jpos,double &Ipos,int &Area,int &Valid,double &Angle,std::vector<pixelData> &FullData,cv::Vec3f &AvgColor,int Place)
{
    if(numvalid>Place)
    {
        Valid=1;
        //Xpos=(allsegments[Place].CentroidI-output->width/2.0)/(output->width/2.0);
        //Ypos=(allsegments[Place].CentroidJ-output->height/2.0)/(output->height/2.0);
        Ipos=(validsegments[Place].CentroidI);
        Jpos=(validsegments[Place].CentroidJ);
        Area=validsegments[Place].Area;
        Angle=validsegments[Place].Angle;
        FullData=validsegments[Place].fullData;
        AvgColor=validsegments[Place].avgColor;
    }
    else
    {
        std::vector <pixelData> empty;
        Valid=0;
        Ipos=0.0;
        Jpos=0.0;
        Area=0;
        Angle=0.0;
        FullData=empty;
        AvgColor[0]=0.0;
        AvgColor[1]=0.0;
        AvgColor[2]=0.0;
    }
}

unsigned short *Segment::GetSegMap()
{
    memset(cleansegmap,0,sizeof(unsigned short)*(imagewidth*imageheight));
    
    //int numdraw=numvalid;
    
    for(int n=0;n<numvalid;n++)
    {
        for(int j=0;j<imageheight;j++)
        {
            for(int i=0;i<imagewidth;i++)
            {
                int top=segmap[j*imagewidth+i];
                
                if(top!=0)
                {
                    int count=0;
                    while(allsegments[top].id!=top)
                    {
                        count++;
                        top=allsegments[top].id;
                        if(count>100)
                        {
                            printf("Fail: %d %d\n",top,allsegments[top].id);
                            fflush(stdout);
                            break;
                        }
                    }


                    if(allsegments[top].id==validsegments[n].id)
                    {
                        //printf("%d  %d %d\n",allsegments[n].id,top,allsegments[segmap[top]]);
                        //cvRectangle(output,cvPoint(i,j),cvPoint(i,j),CV_RGB(0,0,255));
                        
                        cleansegmap[j*imagewidth+i] = n+1;
                    }
                }
            }
        }
    }
    
    return cleansegmap;
}

int Segment::GetNumSeg()
{
    return numvalid;
}

void Segment::Process(const cv::Mat rawImg, const cv::Mat &Input, cv::Mat &Output)
{

    if(Input.empty())
    {
        printf("threshhold image corrupt\n");
        return;
    }

    if(rawImg.empty())
    {
        printf("No raw image\n");
        return;
    }

    if(Output.empty())
    {
        Output.create(Input.rows, Input.cols, CV_8UC3);
    }

    if(first)
    {
        segmap=new unsigned short[Output.rows*Output.cols];
        cleansegmap = new unsigned short[Output.rows*Output.cols];
        first = false;
        imagewidth=Output.cols;
        imageheight=Output.rows;
    }

    for(int a = 0; a < MAXSEGMENTS; a++)
    {
        allsegments[a].fullData.clear();
    }

    //clear the threshhold
    Input.copyTo(Output);
    //clear the segmap
    memset(segmap,0,sizeof(unsigned short)*(Output.rows*Output.cols));

    //reset segment, memory dirty, make sure to set everything
    int i,j;
    int pos;
    int segpos;
    int top,toppos;
    int id,nid;

    //unsigned char *imageData=(unsigned char *)Input->imageData;

    //zero never happens
    numsegments=1;
    countsegments=0;

    maxarea=0;
    maxsegment=-1;

    //printf("%dx%d -> %d\n",output->width,output->height,output->widthStep);

    cv::Vec3b tempVec;
    //first pass is the columns
    for(j=0;j<Output.rows;j++)
    {
        for(i=0;i<Output.cols;i++)
        {
            //pos=j*output->widthStep+3*i;
            //printf("%d: %d\n",pos,(int)Input->imageData[pos]);

            //do run length
            //assume not a segment

            tempVec = Input.at< cv::Vec3b >(j, i);
            //std::cout << "tempVec[]: " << tempVec[0] << ", " << tempVec[1] <<  ", " << tempVec[2] << std::endl;

            if(tempVec[0]==255)
            {
                //printf("%d: ",numsegments);
                //not valid till size is big enough
                allsegments[numsegments].valid=0;
                allsegments[numsegments].id=numsegments;
                allsegments[numsegments].MinJ=j;
                allsegments[numsegments].MaxJ=j;

                allsegments[numsegments].MinI=i;
                allsegments[numsegments].MaxI=i;
                allsegments[numsegments].Area=0;

                //Initializing average colors
                allsegments[numsegments].avgColor[0] = 0.0;
                allsegments[numsegments].avgColor[1] = 0.0;
                allsegments[numsegments].avgColor[2] = 0.0;

                //New Code for line finding and stats
                allsegments[numsegments].N=0;
                allsegments[numsegments].SumI=0;
                allsegments[numsegments].SumJ=0;
                allsegments[numsegments].SumIJ=0;
                allsegments[numsegments].SSumI=0;
                allsegments[numsegments].SSumJ=0;
                allsegments[numsegments].fullData.clear();

                top=0;
                id=0;
                nid=0;

                toppos=(j-1)*Output.cols+i;
                segpos=j*Output.cols+i;

                //loop until segment is over
                while(i<Output.cols && tempVec[0]==255)
                {
                    allsegments[numsegments].MaxI=i;
                    allsegments[numsegments].Area++;

                    //Passing in info to fullData
                    pixelData tempPD;
                    cv::Vec3b tempVec2;

                    tempVec2 = rawImg.at<cv::Vec3b>(j,i);

                    tempPD.posJ = j;
                    tempPD.posI = i;
                    tempPD.redColor = tempVec2[2];
                    tempPD.greenColor = tempVec2[1];
                    tempPD.blueColor = tempVec2[0];
                    allsegments[numsegments].fullData.push_back(tempPD);

                    //Summing colors for average color
                    allsegments[numsegments].avgColor[0] += tempVec2[0];
                    allsegments[numsegments].avgColor[1] += tempVec2[1];
                    allsegments[numsegments].avgColor[2] += tempVec2[2];

                    //new code
                    allsegments[numsegments].N++;
                    allsegments[numsegments].SumI+=i;
                    allsegments[numsegments].SumJ+=j;
                    allsegments[numsegments].SumIJ+=i*j;
                    allsegments[numsegments].SSumI+=i*i;
                    allsegments[numsegments].SSumJ+=j*j;

                    segmap[segpos]=allsegments[numsegments].id;
                    if(j>0)
                    {
                        top=segmap[toppos];
                        //printf("%d %d\n",toppos,top);
                        //printf("%dx%d %dx%d\n",j-1,i,j,i);

                        //segment set, analyze
                        if(top>0)
                        {
                            int count=0;
                            while(allsegments[top].id!=top)
                            {
                                count++;
                                top=allsegments[top].id;
                                if(count>100)
                                {
                                    printf("Fail: %d %d\n",top,allsegments[top].id);
                                    fflush(stdout);
                                    break;
                                }
                            }
                            nid=allsegments[top].id;
                            if(nid>0)
                            {
                                if(id==0)
                                {
                                    id=nid;
                                }
                                if(id!=nid)
                                {
                                    if(id>nid)
                                    {
                                        CombineSegments(allsegments[nid],allsegments[id]);
                                        id=nid;
                                    }
                                    else
                                    {
                                        CombineSegments(allsegments[id],allsegments[nid]);
                                    }
                                    if(allsegments[id].Area>maxarea)
                                    {
                                        maxarea=allsegments[id].Area;
                                        maxsegment=allsegments[id].id;
                                    }
                                    if(allsegments[id].Area>=minarea)
                                    {
                                        allsegments[id].valid=1;
                                    }

                                    countsegments--;
                                }
                            }
                        }
                    }
                    i++;
                    //pos=j*Output.cols+3*i;
                    tempVec = Input.at< cv::Vec3b >(j, i);
                    toppos=(j-1)*Output.cols+i;
                    segpos=j*Output.cols+i;
                }

                //Final value of average color
                allsegments[numsegments].avgColor[0] = allsegments[numsegments].avgColor[0]/allsegments[numsegments].Area;
                allsegments[numsegments].avgColor[1] = allsegments[numsegments].avgColor[1]/allsegments[numsegments].Area;
                allsegments[numsegments].avgColor[2] = allsegments[numsegments].avgColor[2]/allsegments[numsegments].Area;


                //Centriod is center for a single line
                allsegments[numsegments].CentroidJ=j;
                allsegments[numsegments].CentroidI=(allsegments[numsegments].MaxI+allsegments[numsegments].MinI)/2.0;
                if(id!=0)
                {
                    CombineSegments(allsegments[id],allsegments[numsegments]);
                }
                else
                {
                    id=numsegments;
                    countsegments++;
                }

                if(allsegments[id].Area>maxarea)
                {
                    maxarea=allsegments[id].Area;
                    maxsegment=allsegments[id].id;
                }
                if(allsegments[id].Area>=minarea)
                {
                    allsegments[id].valid=1;
                }

                //printf(" %d %d %d %d",allsegments[numsegments].MaxJ,allsegments[numsegments].MinI,allsegments[numsegments].MaxI,allsegments[numsegments].Area);

                //printf("\n");
                numsegments++;
            }
        }
    }


    //combine together - if set

    if(combodist>0)
    {

#ifdef CENDIST
        //1. distance of centroids to combine
        for(i=1;i<numsegments-1;i++)
        {
            if(allsegments[i].valid==0)
            {
                continue;
            }
            for(j=i+1;j<numsegments;j++)
            {
                if(allsegments[j].valid==0)
                {
                    continue;
                }
                //1. distance of centroids to combine
                double dist=fabs(allsegments[i].CentroidI-allsegments[j].CentroidI)+fabs(allsegments[i].CentroidJ-allsegments[j].CentroidJ);
                //printf("%d %d: %lf\n",i,j,dist);
                if(dist<combodist)
                {
                    CombineSegments(allsegments[i],allsegments[j]);
                }
            }
        }
#endif


#ifdef BBOX
        //2. bounding boxes
        for(i=1;i<numsegments-1;i++)
        {
            if(allsegments[i].valid==0)
            {
                continue;
            }
            for(j=i+1;j<numsegments;j++)
            {
                if(allsegments[j].valid==0)
                {
                    continue;
                }

                //check bounding boxes j in i
                if(
                (allsegments[i].MinI <= allsegments[j].MinI && allsegments[j].MinI <= allsegments[i].MaxI
                && allsegments[i].MinJ <= allsegments[j].MinJ && allsegments[j].MinJ <= allsegments[i].MaxJ)
                ||
                (allsegments[i].MinI <= allsegments[j].MaxI && allsegments[j].MaxI <= allsegments[i].MaxI
                && allsegments[i].MinJ <= allsegments[j].MaxJ && allsegments[j].MaxJ <= allsegments[i].MaxJ)
                )
                {
                    CombineSegments(allsegments[i],allsegments[j]);
                }
                //check if i in j
                else if(
                (allsegments[j].MinI <= allsegments[i].MinI && allsegments[i].MinI <= allsegments[j].MaxI
                && allsegments[j].MinJ <= allsegments[i].MinJ && allsegments[i].MinJ <= allsegments[j].MaxJ)
                ||
                (allsegments[j].MinI <= allsegments[i].MaxI && allsegments[i].MaxI <= allsegments[j].MaxI
                && allsegments[j].MinJ <= allsegments[i].MaxJ && allsegments[i].MaxJ <= allsegments[j].MaxJ)
                )
                {
                    CombineSegments(allsegments[i],allsegments[j]);
                }
            }
        }
#endif
    }

    //assume no valid
    numvalid=0;
    //copy in valid members
    for(i=1;i<numsegments;i++)
    {
        if(allsegments[i].valid==1)
        {
            validsegments[numvalid]=allsegments[i];

            //Do new math at the end
            validsegments[numvalid].MeanI=validsegments[numvalid].SumI/validsegments[numvalid].N;
            validsegments[numvalid].MeanJ=validsegments[numvalid].SumJ/validsegments[numvalid].N;


            validsegments[numvalid].DI=validsegments[numvalid].SSumI-2*validsegments[numvalid].MeanI*validsegments[numvalid].SumI+validsegments[numvalid].N*validsegments[numvalid].MeanI*validsegments[numvalid].MeanI;
            validsegments[numvalid].DJ=validsegments[numvalid].SSumJ-2*validsegments[numvalid].MeanJ*validsegments[numvalid].SumJ+validsegments[numvalid].N*validsegments[numvalid].MeanJ*validsegments[numvalid].MeanJ;

            validsegments[numvalid].NIJ=validsegments[numvalid].SumIJ-validsegments[numvalid].MeanJ*validsegments[numvalid].SumI-validsegments[numvalid].MeanI*validsegments[numvalid].SumJ+validsegments[numvalid].N*validsegments[numvalid].MeanI*validsegments[numvalid].MeanJ;

            if(validsegments[numvalid].DI>validsegments[numvalid].DJ)
            {
                //validsegments[numvalid].Angle=180.0/M_PI*atan2(validsegments[numvalid].NIJ,validsegments[numvalid].DI);
                validsegments[numvalid].Angle=-180.0/M_PI*atan2(validsegments[numvalid].DI,validsegments[numvalid].NIJ);
            }
            else
            {
                //validsegments[numvalid].Angle=180.0/M_PI*atan2(validsegments[numvalid].DJ,validsegments[numvalid].NIJ);
                validsegments[numvalid].Angle=-180.0/M_PI*atan2(validsegments[numvalid].NIJ,validsegments[numvalid].DJ);
            }

            if(validsegments[numvalid].Angle>90)
            {
                validsegments[numvalid].Angle-=180;
            }
            if(validsegments[numvalid].Angle<-90)
            {
                validsegments[numvalid].Angle+=180;
            }

            //printf("%lf %lf vs %lf %lf\n", validsegments[numvalid].CentroidJ,validsegments[numvalid].CentroidI,validsegments[numvalid].MeanJ,validsegments[numvalid].MeanI);


            numvalid++;
        }
    }
    //printf("A%d V%d\n",numsegments,numvalid);

    SegNode tmpNode;
    int invert=1;
    //bubble sort the valid?
    for(j=0;j<numvalid-1 && invert==1;j++)
    {
        invert=0;
        for(i=numvalid-1;i>j;i--)
        {
            if(validsegments[i-1].Area<validsegments[i].Area)
            {
                invert=1;
                tmpNode=validsegments[i-1];
                validsegments[i-1]=validsegments[i];
                validsegments[i]=tmpNode;
            }
        }
    }

//#define DRAW
#ifdef DRAW
    int n;

    int numdraw=numvalid;
    if(numdraw>=drawnum)
    {
        numdraw=drawnum;
    }
    for(n=0;n<numdraw;n++)
    {
        for(j=0;j<Output.rows;j++)
        {
            for(i=0;i<Output.cols;i++)
            {
                top=segmap[j*Output.cols+i];


                if(top!=0)
                {
                    int count=0;
                    while(allsegments[top].id!=top)
                    {
                        count++;
                        top=allsegments[top].id;
                        if(count>100)
                        {
                            printf("Fail: %d %d\n",top,allsegments[top].id);
                            fflush(stdout);
                            break;
                        }
                    }


                    if(allsegments[top].id==validsegments[n].id)
                    {
                        //printf("%d  %d %d\n",allsegments[n].id,top,allsegments[segmap[top]]);
                        //cvRectangle(output,cvPoint(i,j),cvPoint(i,j),CV_RGB(0,0,255));

                        //pos=j*output->widthStep+3*i;
                        tempVec[0] = rb;
                        tempVec[1] = rg;
                        tempVec[2] = rr;
                        Output.at< cv::Vec3b >(j, i) = tempVec;
                        /*output->imageData[pos+0]=rb;
                        output->imageData[pos+1]=rg;
                        output->imageData[pos+2]=rr;*/
                    }
                }
            }

        }
#endif

#define DRAW2
#ifdef DRAW2

        int n, count2 = 0;

        int numdraw=numvalid;
        if(numdraw>=drawnum)
        {
            numdraw=drawnum;
        }
        for(n=0;n<numdraw;n++)
        {
            count2 = 0;

            for(j=0;j<Output.rows;j++)
            {
                for(i=0;i<Output.cols;i++)
                {
                    top=segmap[j*Output.cols+i];


                    if(top!=0)
                    {
                        int count=0;
                        while(allsegments[top].id!=top)
                        {
                            count++;
                            top=allsegments[top].id;
                            if(count>100)
                            {
                                printf("Fail: %d %d\n",top,allsegments[top].id);
                                fflush(stdout);
                                break;
                            }
                        }


                        if(allsegments[top].id==validsegments[n].id)
                        {

//#define avgImg
#ifdef avgImg

                            Output.at<cv::Vec3b>(j,i) = validsegments[n].avgColor;
#endif

#define rawImg
#ifdef rawImg
                            //std::cout << validsegments[n].fullData.size() << std::endl;
                            tempVec[0] = validsegments[n].fullData[count2].blueColor;
                            tempVec[1] = validsegments[n].fullData[count2].greenColor;
                            tempVec[2] = validsegments[n].fullData[count2].redColor;

                            Output.at<cv::Vec3b>(validsegments[n].fullData[count2].posJ, validsegments[n].fullData[count2].posI) = tempVec;
#endif
                            count2++;

                         }
                     }

                 }
             }
#endif


//#define DRAWEXTRA
#ifdef DRAWEXTRA

        cv::rectangle(Output, cvPoint(validsegments[n].MinI,validsegments[n].MinJ),cvPoint(validsegments[n].MaxI,validsegments[n].MaxJ), cv::Scalar(rr, rg, rb));

        cv::line(Output, cvPoint(validsegments[n].CentroidI-10,validsegments[n].CentroidJ-10), cvPoint(validsegments[n].CentroidI+10,validsegments[n].CentroidJ+10), cv::Scalar(255-rr,255-rg,255-rb),2);
        cv::line(Output, cvPoint(validsegments[n].CentroidI+10,validsegments[n].CentroidJ-10), cvPoint(validsegments[n].CentroidI-10,validsegments[n].CentroidJ+10), cv::Scalar(255-rr,255-rg,255-rb),2);

        double drawangle=M_PI/2.0+validsegments[n].Angle*M_PI/180.0;
        cv::line(Output, cvPoint(validsegments[n].MeanI-100*cos(drawangle), validsegments[n].MeanJ-100*sin(drawangle)), cvPoint(validsegments[n].MeanI+100*cos(drawangle),validsegments[n].MeanJ+100*sin(drawangle)), cv::Scalar(255-rr,255-rg,255-rb),2);


        //cvRectangle(output,cvPoint(validsegments[n].MinI,validsegments[n].MinJ),cvPoint(validsegments[n].MaxI,validsegments[n].MaxJ),CV_RGB(rr,rg,rb));
        //printf("%lf %lf\n",allsegments[n].CentroidI,allsegments[n].CentroidJ);
        //cvRectangle(output,cvPoint(validsegments[n].CentroidI-10,validsegments[n].CentroidJ-10),cvPoint(validsegments[n].CentroidI+10,validsegments[n].CentroidJ+10),CV_RGB(255,255,255),CV_FILLED);
        //cvLine(output,cvPoint(validsegments[n].CentroidI-10,validsegments[n].CentroidJ-10),cvPoint(validsegments[n].CentroidI+10,validsegments[n].CentroidJ+10),CV_RGB(255-rr,255-rg,255-rb),2);
        //cvLine(output,cvPoint(validsegments[n].CentroidI+10,validsegments[n].CentroidJ-10),cvPoint(validsegments[n].CentroidI-10,validsegments[n].CentroidJ+10),CV_RGB(255-rr,255-rg,255-rb),2);
        //printf("%d %d\n",validsegments[n].id,validsegments[n].Area);

        //draw the angle
        //double drawangle=M_PI/2.0+validsegments[n].Angle*M_PI/180.0;
        //cvLine(output,cvPoint(validsegments[n].MeanI-100*cos(drawangle),validsegments[n].MeanJ-100*sin(drawangle)),cvPoint(validsegments[n].MeanI+100*cos(drawangle),validsegments[n].MeanJ+100*sin(drawangle)),CV_RGB(255-rr,255-rg,255-rb),2);
#endif

//#define DRAWEXTRA2
#ifdef DRAWEXTRA2

        cv::rectangle(Output, cvPoint(validsegments[n].MinI,validsegments[n].MinJ),cvPoint(validsegments[n].MaxI,validsegments[n].MaxJ), cv::Scalar(rr, rg, rb));

        cv::line(Output, cvPoint(validsegments[n].CentroidI-5,validsegments[n].CentroidJ-5), cvPoint(validsegments[n].CentroidI+5,validsegments[n].CentroidJ+5), cv::Scalar(255-rr,255-rg,255-rb),2);
        cv::line(Output, cvPoint(validsegments[n].CentroidI+5,validsegments[n].CentroidJ-5), cvPoint(validsegments[n].CentroidI-5,validsegments[n].CentroidJ+5), cv::Scalar(255-rr,255-rg,255-rb),2);

        double drawangle=M_PI/2.0+validsegments[n].Angle*M_PI/180.0;
        cv::line(Output, cvPoint(validsegments[n].MeanI-35*cos(drawangle), validsegments[n].MeanJ-35*sin(drawangle)), cvPoint(validsegments[n].MeanI+35*cos(drawangle),validsegments[n].MeanJ+35*sin(drawangle)), cv::Scalar(255-rr,255-rg,255-rb),2);

        cv::Scalar color(255, 255, 255);
        cv::Scalar colorB(0, 0, 0);

        std::string JPos = static_cast<std::ostringstream*>( &(std::ostringstream() << (std::floor(validsegments[n].CentroidJ*100.0)/100.0)) )-> str();
        std::string IPos = static_cast<std::ostringstream*>( &(std::ostringstream() << (std::floor(validsegments[n].CentroidI*100.0)/100.0)) )-> str();
        std::string area = static_cast<std::ostringstream*>( &(std::ostringstream() << validsegments[n].Area) )-> str();
        std::string angle = static_cast<std::ostringstream*>( &(std::ostringstream() << (std::floor(validsegments[n].Angle*100.0)/100.0)) )-> str();
        std::string avgRed = static_cast<std::ostringstream*>( &(std::ostringstream() << (std::floor(validsegments[n].avgColor[2]*100.0)/100.0)) )-> str();
        std::string avgGreen = static_cast<std::ostringstream*>( &(std::ostringstream() << (std::floor(validsegments[n].avgColor[1]*100.0)/100.0)) )-> str();
        std::string avgBlue = static_cast<std::ostringstream*>( &(std::ostringstream() << (std::floor(validsegments[n].avgColor[0]*100.0)/100.0)) )-> str();

        cv::putText(Output, "(" + IPos + ", " + JPos + ")",cvPoint(validsegments[n].MaxI,validsegments[n].MinJ-50), cv::FONT_HERSHEY_PLAIN, 0.85, colorB, 2, 8, false);
        cv::putText(Output, "A: "+ area,cvPoint(validsegments[n].MaxI,validsegments[n].MinJ-35), cv::FONT_HERSHEY_PLAIN, 0.85, colorB, 2, 8, false);
        cv::putText(Output, "A<: "+ angle, cvPoint(validsegments[n].MaxI,validsegments[n].MinJ-20), cv::FONT_HERSHEY_PLAIN, 0.85, colorB, 2, 8, false);
        cv::putText(Output, "RGB: ("+ avgRed + ", "+ avgGreen + ", "+ avgBlue + ")",cvPoint(validsegments[n].MaxI,validsegments[n].MinJ-5), cv::FONT_HERSHEY_PLAIN, 0.85, colorB, 2, 8, false);

        cv::putText(Output, "(" + IPos + ", " + JPos + ")",cvPoint(validsegments[n].MaxI,validsegments[n].MinJ-51), cv::FONT_HERSHEY_PLAIN, 0.85, color, 1, 8, false);
        cv::putText(Output, "A: "+ area,cvPoint(validsegments[n].MaxI,validsegments[n].MinJ-36), cv::FONT_HERSHEY_PLAIN, 0.85, color, 1, 8, false);
        cv::putText(Output, "A<: "+ angle, cvPoint(validsegments[n].MaxI, validsegments[n].MinJ-21), cv::FONT_HERSHEY_PLAIN, 0.85, color, 1, 8, false);
        cv::putText(Output, "RGB: ("+ avgRed + ", "+ avgGreen + ", "+ avgBlue + ")",cvPoint(validsegments[n].MaxI,validsegments[n].MinJ-6), cv::FONT_HERSHEY_PLAIN, 0.85, color, 1, 8, false);

//        double disY = (validsegments[n].MaxJ+validsegments[n].MinJ)/2;
//        double disX = (validsegments[n].MaxI+validsegments[n].MinI)/2;

//        if(fabs(validsegments[n].Angle) >= 45)
//        {
//            if(validsegments[n].Angle < 0)
//                cv::line(Output, cvPoint(validsegments[n].MinI, disY-drawangle), cvPoint(validsegments[n].MaxI, disY+drawangle), cv::Scalar(255-rr,255-rg,255-rb),2);
//            else
//                cv::line(Output, cvPoint(validsegments[n].MinI, disY+drawangle), cvPoint(validsegments[n].MaxI, disY-drawangle), cv::Scalar(255-rr,255-rg,255-rb),2);
//        }
//        else if(fabs(validsegments[n].Angle) < 45)
//        {
//            if(validsegments[n].Angle < 0)
//                cv::line(Output, cvPoint(disX-drawangle, validsegments[n].MinJ), cvPoint(disX+drawangle, validsegments[n].MaxJ), cv::Scalar(255-rr,255-rg,255-rb),2);
//            else
//                cv::line(Output, cvPoint(disX+drawangle, validsegments[n].MinJ), cvPoint(disX-drawangle, validsegments[n].MaxJ), cv::Scalar(255-rr,255-rg,255-rb),2);
//        }

        //cvRectangle(output,cvPoint(validsegments[n].MinI,validsegments[n].MinJ),cvPoint(validsegments[n].MaxI,validsegments[n].MaxJ),CV_RGB(rr,rg,rb));
        //printf("%lf %lf\n",allsegments[n].CentroidI,allsegments[n].CentroidJ);
        //cvRectangle(output,cvPoint(validsegments[n].CentroidI-10,validsegments[n].CentroidJ-10),cvPoint(validsegments[n].CentroidI+10,validsegments[n].CentroidJ+10),CV_RGB(255,255,255),CV_FILLED);
        //cvLine(output,cvPoint(validsegments[n].CentroidI-10,validsegments[n].CentroidJ-10),cvPoint(validsegments[n].CentroidI+10,validsegments[n].CentroidJ+10),CV_RGB(255-rr,255-rg,255-rb),2);
        //cvLine(output,cvPoint(validsegments[n].CentroidI+10,validsegments[n].CentroidJ-10),cvPoint(validsegments[n].CentroidI-10,validsegments[n].CentroidJ+10),CV_RGB(255-rr,255-rg,255-rb),2);
        //printf("%d %d\n",validsegments[n].id,validsegments[n].Area);

        //draw the angle
        //double drawangle=M_PI/2.0+validsegments[n].Angle*M_PI/180.0;
        //cvLine(output,cvPoint(validsegments[n].MeanI-100*cos(drawangle),validsegments[n].MeanJ-100*sin(drawangle)),cvPoint(validsegments[n].MeanI+100*cos(drawangle),validsegments[n].MeanJ+100*sin(drawangle)),CV_RGB(255-rr,255-rg,255-rb),2);
#endif


    }

    //printf("Num segments: %d %d %d\n",numsegments-1,countsegments,numgood);
//#endif
        //Output=output;
}
