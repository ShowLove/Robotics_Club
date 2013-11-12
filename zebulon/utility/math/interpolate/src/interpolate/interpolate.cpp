#include "interpolate/interpolate.h"

using namespace Zebulon;
using namespace Utility;

Interpolate::Interpolate(int NumInputs,int NumOutputs)
{
    mNumInputs=NumInputs;
    mNumOutputs=NumOutputs;
}
Interpolate::~Interpolate()
{
    
}

int Interpolate::Setup(std::string Filename)
{
    char templine[1024];
    mapper tmp;
    double val;
    char *pos;
    FILE *instream=fopen(Filename.c_str(),"r");
    if(instream==NULL)
    {
        printf("Interpolate: Could not find file %s\n",Filename.c_str());
        return false;
    }
    while(fgets(templine,1024,instream)!=NULL)
    {
        //csv
        //Load inputs
        pos=templine;
        tmp.input.clear();
        for(int i=0;i<mNumInputs;i++)
        {
            val=atof(pos);
            tmp.input.push_back(val);
            pos=strstr(pos,",")+1;
        }
        
        //Load outputs
        tmp.output.clear();
        for(int i=0;i<mNumOutputs;i++)
        {
            val=atof(pos);
            tmp.output.push_back(val);
            pos=strstr(pos,",")+1;
        }
        
        mInterMap.push_back(tmp);
    }
    //printf("Interpolate: Read %d\n",mInterMap.size());
    fclose(instream);
    
    //initialze the child class
    Init();
    return true;
}


std::vector<double> Interpolate::GetOutput(std::vector<double> Input)
{
    if(Input.size()!=mNumInputs)
    {
        printf("Interpolate: Input size does not match %d vs %d\n",(int)Input.size(),mNumInputs);
    }
    
    //extra call for fixing things later, stats?
    std::vector<double> output;
    mapper ret=MapIt(Input);
    output=ret.output;
    if(output.size()!=mNumOutputs)
    {
         printf("Interpolate: Output size does not match %d vs %d\n",(int)output.size(),mNumOutputs);
    }    
    return output;
}

void Interpolate::CheckRMSE()
{
    std::vector<double> rmse;
    rmse.reserve(mNumOutputs);
    rmse.resize(mNumOutputs);
    
    for(int i=0;i<mInterMap.size();i++)
    {
        if(i%10==0)
        {
            printf("%lf\r",100.0*i/mInterMap.size());
            fflush(stdout);
        }
        std::vector<double> output=GetOutput(mInterMap[i].input);
        for(int j=0;j<output.size();j++)
        {
            rmse[j]+=pow(output[j]-mInterMap[i].output[j],2);
        }
    }
    for(int i=0;i<mNumOutputs;i++)
    {
        rmse[i]=sqrt(rmse[i]/mInterMap.size());
        printf("%lf ",rmse[i]);
    }
    printf("\n");
}

CaseBasedInter::CaseBasedInter(int NumInputs,int NumOutputs,int KeepSize) : Interpolate(NumInputs,NumOutputs)
{
    mKeepSize=KeepSize;
}

CaseBasedInter::~CaseBasedInter()
{
    
}

Interpolate::mapper CaseBasedInter::MapIt(std::vector<double> Input)
{
    //closest N
    int listsize;
        
    std::priority_queue<mapper> NewMap;
    //later iterator?
    for(int i=0;i<(int)mInterMap.size();i++)
    {
        //straight carteasian distance (input space)
        double ssum=0.0;
        for(int j=0;j<mNumInputs;j++)
        {
            ssum+=pow(mInterMap[i].input[j]-Input[j],2.0);
        }
        mInterMap[i].dist=sqrt(ssum);
        
        //if far away, ignore?
        //settable
        /*if(mInterMap[i].dist>800)
        {
            continue;
        }*/
        NewMap.push(mInterMap[i]);
    }
    //How many points actually added
    listsize=NewMap.size();

    std::vector<mapper> KeepMap;
    if(mKeepSize<listsize)
    {
        listsize=mKeepSize;
    }
    if(listsize==0)
    {
        printf("Invalid\n");
    }
    
    //cases listsize = 0 (outside range?)
    //printf("List %d\n",listsize);
    
    //find the max distance
    double maxdist=0.0;
    for(int i=0;i<listsize;i++)
    {
        mapper tmp=NewMap.top();
        NewMap.pop();
        KeepMap.push_back(tmp);
        if(tmp.dist>maxdist)
        {
            maxdist=tmp.dist;
        }
        /*printf("Matches\n");
        printf("%d: ",i);
        printf("Input:");
        for(int j=0;j<KeepMap[i].input.size();j++)
        {
            printf(" %lf",KeepMap[i].input[j]);
        }
        printf(" Output:");
        for(int j=0;j<KeepMap[i].output.size();j++)
        {
            printf(" %lf",KeepMap[i].output[j]);
        }
        printf("\n");*/
    }
    //printf("Maxdist: %lf\n",maxdist);
    
    
    //find the centroid?
    double sum=0.0;
    //intitialize
    mapper ret;
    for(int i=0;i<mNumInputs;i++)
    {
        ret.input.push_back(0.0);
    }
    for(int i=0;i<mNumOutputs;i++)
    {
        ret.output.push_back(0.0);
    }

    //centroid of remaining (scaling fairly)
    for(int i=0;i<KeepMap.size();i++)
    {
        //1 over mapping
        //double dist=1.0/(1.0+KeepMap[i].dist);
        //double dist=1.0/KeepMap[i].dist;
        //Weighted distance (ignores worst point of N)
        double dist=maxdist-KeepMap[i].dist;
        //double dist=pow(maxdist-KeepMap[i].dist,2);
        //Normal average
        //double dist=1.0;
        
        sum+=dist;
        //map together output
        for(int j=0;j<mNumOutputs;j++)
        {
            ret.output[j]+=KeepMap[i].output[j]*dist;
        }
        
        //map input for curiousity
        for(int j=0;j<mNumInputs;j++)
        {
            ret.input[j]+=KeepMap[i].input[j]*dist;
        }
    }
    if(sum>0)
    {
        //avg output
        for(int j=0;j<mNumOutputs;j++)
        {
            ret.output[j]/=sum;
        }
        //avg input
        for(int j=0;j<mNumInputs;j++)
        {
            ret.input[j]/=sum;
        }
    }
    

    //if only one point, keep
    if(listsize==1)
    {
        ret=KeepMap[0];
    }
    
    return ret;
}

LinearRegInter::LinearRegInter(int NumInputs,int NumOutputs) : Interpolate(NumInputs,NumOutputs)
{
    mConstants.reserve(NumOutputs);
    for(int i=0;i<NumOutputs;i++)
    {
        mConstants[i].reserve(NumInputs+1);
    }
}
LinearRegInter::~LinearRegInter()
{
    
}

void LinearRegInter::Init()
{
    //Only matrix library I have
    
    //Copy from the vector to a matrix for math
    //add the constant offset also
    CxUtils::Matrix XMatrix(mInterMap.size(),mNumInputs+1);
    for(int i=0;i<mInterMap.size();i++)
    {
        for(int j=0;j<mNumInputs;j++)
        {
            XMatrix[i][j]=mInterMap[i].input[j];
        }
        XMatrix[i][mNumInputs]=1.0;
    }
    
    //calculate the pseudo inverse of the input
    CxUtils::Matrix XPinv=((XMatrix.Transpose()*XMatrix).Inverse())*XMatrix.Transpose();
    
    //copy the output to a matrix for math
    CxUtils::Matrix YMatrix(mInterMap.size(),1);
    for(int i=0;i<mNumOutputs;i++)
    {
        for(int j=0;j<mInterMap.size();j++)
        {
            YMatrix[j][0]=mInterMap[j].output[i];
        }
        
        //calculate constants for linear fit
        CxUtils::Matrix BConstants;
        BConstants=XPinv*YMatrix;
        
        //copy back to a different data structure
        for(int j=0;j<mNumInputs+1;j++)
        {
            mConstants[i][j]=BConstants[j][0];
        }
    }
}

Interpolate::mapper LinearRegInter::MapIt(std::vector<double> Input)
{
    mapper ret;
    
    //copy input (not needed)
    ret.input=Input;
    
    for(int i=0;i<mNumOutputs;i++)
    {
        //get DC offset
        ret.output.push_back(mConstants[i][mNumInputs]);
        for(int j=0;j<mNumInputs;j++)
        {
            //sum out linear output
            ret.output[i]+=Input[j]*mConstants[i][j];
        }
    }
    return ret;
}

CaseJacobInter::CaseJacobInter(int NumInputs,int NumOutputs,int KeepSize) : Interpolate(NumInputs,NumOutputs)
{
    mKeepSize=KeepSize;
}

CaseJacobInter::~CaseJacobInter()
{
    
}

Interpolate::mapper CaseJacobInter::MapIt(std::vector<double> Input)
{
    //closest N
    int listsize;
        
    std::priority_queue<mapper> NewMap;
    //later iterator?
    for(int i=0;i<(int)mInterMap.size();i++)
    {
        //straight carteasian distance (input space)
        double ssum=0.0;
        for(int j=0;j<mNumInputs;j++)
        {
            ssum+=pow(mInterMap[i].input[j]-Input[j],2.0);
        }
        mInterMap[i].dist=sqrt(ssum);
        
        //if far away, ignore?
        //settable
        /*if(mInterMap[i].dist>800)
        {
            continue;
        }*/
        NewMap.push(mInterMap[i]);
    }
    //How many points actually added
    listsize=NewMap.size();

    std::vector<mapper> KeepMap;
    if(mKeepSize<listsize)
    {
        listsize=mKeepSize;
    }
    if(listsize==0)
    {
        printf("Invalid\n");
    }
    
    //cases listsize = 0 (outside range?)
    //printf("List %d\n",listsize);
    
    //find the max distance
    double maxdist=0.0;
    for(int i=0;i<listsize;i++)
    {
        mapper tmp=NewMap.top();
        NewMap.pop();
        KeepMap.push_back(tmp);
        if(tmp.dist>maxdist)
        {
            maxdist=tmp.dist;
        }
    }
    //printf("Maxdist: %lf\n",maxdist);
    
    
    //find the centroid?
    double sum=0.0;
    //intitialize
    mapper ret;
    for(int i=0;i<mNumInputs;i++)
    {
        ret.input.push_back(0.0);
    }
    for(int i=0;i<mNumOutputs;i++)
    {
        ret.output.push_back(0.0);
    }
    
    //project a point for all linearlly
    for(int i=0;i<KeepMap.size()-1;i++)
    {
        for(int j=i+1;j<KeepMap.size();j++)
        {
            //Do a jacobian type thing on input output combination
            for(int y=0;y<mNumOutputs;y++)
            {
                ret.output[y]=0.0;
                for(int x=0;x<mNumInputs;x++)
                {
                    //protect against divide by zero?
                    double dy=KeepMap[j].output[y]-KeepMap[i].output[y];
                    double dx=KeepMap[j].input[x]-KeepMap[i].input[x];
                    //move point on slope
                    double outy;
                    //small epsilon
                    if(fabs(dx)<0.00001)
                    {
                        printf("almost divide by zero\n");
                        //skip
                        outy=(Input[x]-KeepMap[i].input[x]);
                    }
                    else
                    {
                        outy=(Input[x]-KeepMap[i].input[x])*dy/dx;
                    }
                    //shift up for offset
                    ret.output[y]+=KeepMap[i].output[y];
                }
            }
        }
    }
    
    int n=(KeepMap.size()*(KeepMap.size()-1)/2);
    for(int y=0;y<mNumOutputs;y++)
    {
        ret.output[y]/n;
    }

    //if only one point, keep
    if(listsize==1)
    {
        ret=KeepMap[0];
    }
    
    return ret;
}