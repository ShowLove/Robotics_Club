//Dirty Class to allow for less parameters

#ifndef _TARGET
#define _TARGET

#include <xmlconfig/xmlconfigurator.h>

namespace Zebulon
{
    namespace Vision
    {
        class Target
        {
            public:
                Target();
                ~Target();
                virtual bool Setup(Configuration::XmlConfigurator& xml, const std::string& xmlTagName, const std::string& fullXMLPath="");
                //virtual void Setup(int Red,int Green,int Blue,int Rrange=32,int Grange=32,int Brange=32,int Edge=0);
                int mRed,mGreen,mBlue;
                int mRedRange,mGreenRange,mBlueRange;
                
                double mRedMean, mGreenMean, mBlueMean;
                double mRedVariance,mGreenVariance,mBlueVariance;
                
                double mRedTrue, mGreenTrue, mBlueTrue;
                double mRedLeftFalse, mRedRightFalse;
                double mGreenLeftFalse, mGreenRightFalse;
                double mBlueLeftFalse, mBlueRightFalse;
        protected:
                std::string mXMLTagName;
                
        };
    }
}
#endif
