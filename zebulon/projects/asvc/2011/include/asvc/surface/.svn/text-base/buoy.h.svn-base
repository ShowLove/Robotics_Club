/////////////////////////////////////////////////////////////////////////////////////
///
/// \file buoy.h
/// \brief Simple Buoy Cartographer object structure.
///
/// <br>Author(s): Daniel Barber
/// <br>Created: 05/31/2011<br>
/// <br>Copyright (c) 2011<br>
/// <br>Robotics Laboratory and Club<br>
/// <br>University of Central Florida (UCF) <br>
/// <br>Email: jmohlenh@ist.ucf.edu <br>
/// <br>Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef ZEUBLON_CARTOGRAPHER_BUOY_H
#define ZEUBLON_CARTOGRAPHER_BUOY_H

#include <cartographer/objects/sphere.h>
#include <vector>

namespace Zebulon
{
    namespace ASVC
    {
        #define BUOY_OBJECT_NAME "Buoy"
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Buoy
        ///   \brief Simple 3D Buoy object class.  Used to describe floating Buoy in water.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CARTOGRAPHER_DLL Buoy : public Cartographer::Objects::Sphere
        {
        public:
            typedef std::vector<Buoy> List; ///<  List of buoys.
            enum Color
            {
                Unknown = 0,
                Red,
                Green,
                Yellow,
                Blue
            };
            Buoy(const unsigned int sourceID = 0) : Cartographer::Objects::Sphere(sourceID)
            {
                mBuoyColor = Unknown;
                mObjectIdentification = BUOY_OBJECT_NAME;
            }
            Buoy(const Cartographer::Point3D& origin,
                   const double radius,
                   const unsigned int sourceID = 0) : Cartographer::Objects::Sphere(origin,
                                                                                    radius,
                                                                                    sourceID)
            {
                mBuoyColor = Unknown;
                mObjectIdentification = BUOY_OBJECT_NAME;
            }
            Buoy(const Buoy& buoy)
            {
                *this = buoy;
                mObjectIdentification = BUOY_OBJECT_NAME;
            }
            virtual ~Buoy()
            {

            }
            void SetBuoyColor(const Color color)
            {
                mBuoyColor = color;
            }
            Color GetBuoyColor() const { return mBuoyColor; }
            virtual Cartographer::Object* Clone() const { return new Buoy(*this); }
            Buoy& operator=(const Buoy& buoy)
            {
                if(this != &buoy)
                {
                    *( (Cartographer::Objects::Sphere*)this) = *( (Cartographer::Objects::Sphere*)&buoy);
                    mBuoyColor = buoy.mBuoyColor;
                    mObjectIdentification = BUOY_OBJECT_NAME;
                }
                return *this;
            }
            /** Method assumes each buoy is relative to the same pose. */
            static bool Compare(const Buoy& left, const Buoy& right)
            {
                return left.GetOrigin().Distance() < right.GetOrigin().Distance();
            }
#ifdef USE_OPENCV
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Gets the color of the object.
            ///
            ///   \return Color object should be in an image.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            virtual CvScalar GetDrawColor() const
            {
                CvScalar result;
                switch(mBuoyColor)
                {
                case Red:
                    result = CV_RGB(255, 0, 0);
                    break;
                case Green:
                    result = CV_RGB(0, 255, 0);
                    break;
                case Blue:
                    result = CV_RGB(0, 0, 255);
                    break;
                 case Yellow:
                    result = CV_RGB(255, 255, 0);
                    break;
                default:
                    result = CV_RGB(255, 255, 255);
                    break;
                }
                return result;
            }
#endif
        protected:
            Color mBuoyColor;
        };
    }
}

#endif // BUOY_H
