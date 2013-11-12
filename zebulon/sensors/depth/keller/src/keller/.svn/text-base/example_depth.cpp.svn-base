#include<stdio.h>
#include<cxutils/cxutils.h>

#include "keller/keller.h"
#include <grapher/grapher.h>

using namespace Zebulon;
using namespace Depth;


Utility::Grapher graph;
class TestCallback : public Callback
{
    public:
        TestCallback() {}
        virtual ~TestCallback() {}

        virtual void ProcessDepthData(const double depth)
        {
            //printf("Depth: %lf\n",depth);
            graph.AddValue(0,depth);
        }
};     

int main()
{
    
    Keller k;
    graph.Setup("Depth",1,1000,1000,400);
    graph.Range(0,-6,12);
    
    k.Initialize("/dev/ttyUSB3",50,9600);
    k.RegisterCallback(new TestCallback());
    
    while(1)
    {
        //do nothing
        printf("Req: %lf Rec: %lf\n",k.ReqPerSec,k.RecPerSec);
        graph.Graph();
        CxUtils::SleepMs(100);
    }
    
}