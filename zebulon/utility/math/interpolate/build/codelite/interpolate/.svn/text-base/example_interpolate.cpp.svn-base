#include<stdio.h>
#include<vector>
#include "interpolate/interpolate.h"


int main(int argc,char *argv[])
{
    //Zebulon::Utility::Interpolate *inter=new Zebulon::Utility::CaseBasedInter(1,1,5);
    //Zebulon::Utility::Interpolate *inter=new Zebulon::Utility::LinearRegInter(1,1);
    
    //Zebulon::Utility::Interpolate *inter=new Zebulon::Utility::CaseBasedInter(4,6,5);
    Zebulon::Utility::Interpolate *inter=new Zebulon::Utility::LinearRegInter(4,6);
    inter->Setup("../../../../projects/underwater/auvc/2010/doc/hydrosimp.log");
    
    
    inter->CheckRMSE();
    return 0;
    
    std::vector<double> input;
    std::vector<double> output;
    
    input.reserve(4);
    input.resize(4);
    
    for(input[0]=-M_PI;input[0]<=M_PI;input[0]+=0.1)
    {
        for(input[1]=-M_PI;input[1]<=M_PI;input[1]+=0.1)
        {
            for(input[2]=-M_PI;input[2]<=M_PI;input[2]+=0.1)
            {
                for(input[3]=-M_PI;input[3]<=M_PI;input[3]+=0.1)
                {
                    for(int i=0;i<input.size();i++)
                    {
                        printf("%lf,",input[i]);
                    }
                    output=inter->GetOutput(input);
                    for(int i=0;i<output.size();i++)
                    {
                        printf("%lf,",output[i]);
                    }
                    printf("\n");
                    //printf("%lf,%lf\n",input[0],output[0]);
                }
            }
        }
    }
    delete inter;
    return 0;
}