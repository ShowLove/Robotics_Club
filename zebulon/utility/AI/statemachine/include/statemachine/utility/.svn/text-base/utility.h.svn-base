////////////////////////////////////////////////////////////////////////////////////
///
/// \file utility.h
/// \brief Static class to house static inline functions.
///
/// Author(s): John Reeder<br>
/// Created: 2010<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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
#ifndef ZEBULON_AI_UTILITY_H
#define ZEBULON_AI_UTILITY_H

#include <cxutils/math/cxmath.h>
#include <display/display.h>
#include "statemachine/libstatemachinedll.h"


namespace Zebulon
{
    namespace AI
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class Utility
        ///  \brief Static class to house static utility functions.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_STATEMACHINE_UTIL_DLL  Utility
        {
        public:

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \brief Calculates angle difference with compass data.
            ///
            ///  \param[in] angleSrc source angle.
            ///  \param[in] angleDest destination angle.
            ///
            ///  \return Returns difference.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static double AngleDiff(const double angleSrc, const double angleDest)
            {
                double diff=angleDest-angleSrc;
                //assume in degrees, min diff
                while(diff>180.0)
                {
                    diff-=360.0;
                }
                while(diff<=-180.0)
                {
                    diff+=360.0;
                }
                return diff;
            };

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \brief Calculates angle sum with compass data.
            ///
            ///  \param[in] angleSrc source angle.
            ///  \param[in] angleDest destination angle.
            ///
            ///  \return Returns sum.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static double AngleSum(const double angleSrc, const double angleDest)
            {
                double diff=angleDest+angleSrc;
                //assume in degrees, min diff
                while(diff>180.0)
                {
                    diff-=360.0;
                }
                while(diff<=-180.0)
                {
                    diff+=360.0;
                }
                return diff;
            };


            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \brief Calculates a Position projected based on heading.
            ///
            ///  \param[in] srcEasting Current easting.
            ///  \param[in] srcNorthing Current northing.
            ///  \param[in] heading Current heading in degrees.
            ///  \param[in] distance Distance to project (meters).
            ///  \param[out] newEasting New easting value.
            ///  \param[out] newNorthing New northing value.
            ///
            ///  \return Returns difference.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static void CalcProject(const double srcEasting,
                                    const double srcNorthing,
                                    const double heading,
                                    const double distance,
                                    double &newEasting,
                                    double &newNorthing)
            {
                double angle=CX_DEG2RAD((90-heading));
                newEasting=srcEasting+distance*cos(angle);
                newNorthing=srcNorthing+distance*sin(angle);
                //std::cout << NewEasting << " " << NewNorthing << " " <<  angle << std::endl;
            };

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \brief Calculates the distance between GPS points.
            ///
            ///  \param[in] eastingSrc source easting
            ///  \param[in] northingSrc source northing
            ///  \param[in] eastingDest destination easting
            ///  \param[in] northingDest destination northing
            ///
            ///  \return Returns distance.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static double CalcDistance(const double eastingSrc,
                                       const double northingSrc,
                                       const double eastingDest,
                                       const double northingDest)
            {
                return sqrt(pow(eastingSrc-eastingDest,2)+pow(northingSrc-northingDest,2));
            };

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \brief Calculates the distance between GPS points.
            ///
            ///  \param[in] eastingSrc source easting
            ///  \param[in] northingSrc source northing
            ///  \param[in] eastingDest destination easting
            ///  \param[in] northingDest destination northing
            ///
            ///  \return Returns distance.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static double CalcDistanceSquared(const double eastingSrc,
                                              const double northingSrc,
                                              const double eastingDest,
                                              const double northingDest)
            {
                return pow(eastingSrc-eastingDest,2)+pow(northingSrc-northingDest,2);
            };

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \brief Calculates the angle between GPS points.
            ///
            ///  \param[in] eastingSrc source easting
            ///  \param[in] northingSrc source northing
            ///  \param[in] eastingDest destination easting
            ///  \param[in] northingDest destination northing
            ///
            ///  \return Returns angle.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static double CalcHeading(const double eastingSrc,
                                      const double northingSrc,
                                      const double eastingDest,
                                      const double northingDest)
            {
                return CX_RAD2DEG(atan2(eastingDest-eastingSrc,northingDest-northingSrc));
            };

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \brief Determines if the distance between two points is below a threshold
            ///
            ///  \param[in] eastingSrc source easting
            ///  \param[in] northingSrc source northing
            ///  \param[in] eastingDest destination easting
            ///  \param[in] northingDest destination northing
            ///  \param[in] distThreshld the desired threshold
            ///
            ///  \return True if within the threshold, False otherwise
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static bool DistanceClosed(const double eastingSrc,
                                       const double northingSrc,
                                       const double eastingDest,
                                       const double northingDest,
                                       const double distThreshld)
            {
                double distDiff = CalcDistance(eastingSrc,northingSrc,eastingDest,northingDest);
                if (distDiff < distThreshld)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            };

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \brief Determines if the difference in angle is below a threshold
            ///
            ///  \param[in] angleSrc source angle
            ///  \param[in] angleDest destination angle
            ///  \param[in] threshld
            ///
            ///  \return True if within the threshold, False otherwise
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static bool HeadingMatched(const double angleSrc,
                                       const double angleDest,
                                       const double threshld)
            {
                double angDiff = AngleDiff(angleSrc, angleDest);
                if (fabs(angDiff) < threshld)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            };

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \brief Draws a line on the screen/video image indicating desired heading change
            ///
            ///  \param[in] image the image to draw to
            ///  \param[in] currentHeading Current heading
            ///  \param[in] desiredHeading Desired heading
            ///  \param[in] R Red component of line color
            ///  \param[in] G Green component of line color
            ///  \param[in] B Blue component of Line color
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static void HeadingDisplay(IplImage* image,
                                       const double currentHeading,
                                       const double desiredHeading,
                                       const int R,
                                       const int G,
                                       const int B)
            {
                if(image == NULL) return;
                double diff=CX_DEG2RAD(AngleDiff(currentHeading, desiredHeading));
                double dispI=image->width/2 + 240*sin(diff);
                double dispJ=image->height - 240*cos(diff);
                cvLine(image, cvPoint(image->width/2,image->height), cvPoint((int)dispI, (int)dispJ), cvScalar(B,G,R), 4);
            };

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \brief Draws an indicator on the screen for current (blue) and desired (red) depth
            ///
            ///  \param[in] image the image to draw to
            ///  \param[in] currentDepth Current depth
            ///  \param[in] desiredDepth Desired depth
            ///  \param[in] surface the "depth" of the surface (usually 0)
            ///  \param[in] ground the lower bound of depth (ONR Facility = 16 ft max)
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static void DepthDisplay(IplImage* image,
                                     const double currentDepth,
                                     const double desiredDepth,
                                     const double surface,
                                     const double ground)
            {
                cvLine(image, cvPoint(10,image->height/2), cvPoint(40,image->height/2), cvScalar(0,255,0), 1);
                cvLine(image, cvPoint(10,image->height-image->height/4), cvPoint(40,image->height-image->height/4), cvScalar(0,255,0), 1);
                cvLine(image, cvPoint(15,image->height/2), cvPoint(15,image->height-image->height/4), cvScalar(0,255,0), 1);

                double curr = image->height/2 + (currentDepth-surface)*(image->height-image->height/4-image->height/2.0)/(ground-surface);
                double desi = image->height/2 + (desiredDepth-surface)*(image->height-image->height/4-image->height/2.0)/(ground-surface);

                cvLine(image, cvPoint(12, (int)curr), cvPoint(30, (int)curr), cvScalar(255,0,0), 1);
                cvLine(image, cvPoint(12, (int)desi), cvPoint(30, (int)desi), cvScalar(0,0,255), 1);
            };


            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \brief Draws an indicator on the screen for Axial and Lateral thrust, Axial
            ///         on right, Lateral on bottom.
            ///
            ///  \param[in] image the image to draw to
            ///  \param[in] desiredAxial desired axial thrust
            ///  \param[in] desiredLateral desired lateral thrust
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static void ThrustDisplay(IplImage* image,
                                      const double desiredAxial,
                                      const double desiredLateral)
            {
                double desired = (-desiredAxial/100)*image->height/4 + image->height/2;

                // Axial
                if (desiredAxial > 0)
                {
                    cvLine(image, cvPoint(image->width-10,image->height/2), cvPoint(image->width-10, (int)desired), cvScalar(255,0,0), 3);
                }
                else
                {
                    cvLine(image, cvPoint(image->width-10,image->height/2), cvPoint(image->width-10, (int)desired), cvScalar(0,0,255), 3);
                }

                desired = (desiredLateral/100)*image->width/4 + image->width/2;

                // Lateral
                if (desiredLateral > 0)
                {
                    cvLine(image, cvPoint(image->width/2,image->height-10), cvPoint( (int)desired,image->height-10), cvScalar(255,0,0), 3);
                }
                else
                {
                    cvLine(image, cvPoint(image->width/2,image->height-10), cvPoint( (int)desired,image->height-10), cvScalar(0,0,255), 3);
                }

                cvLine(image, cvPoint(image->width/2, image->height-5), cvPoint(image->width/2, image->height-15), cvScalar(0,255,0), 1);
                cvLine(image, cvPoint(image->width-5, image->height/2), cvPoint(image->width-15, image->height/2), cvScalar(0,255,0), 1);
            };

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \brief Calculates the difference between current and desired depths.
            ///
            ///  \param[in] current depth
            ///  \param[in] desired depth
            ///
            ///  \return Returns difference.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static double DepthDiff(const double currDepth,
                                    const double desiredDepth)
            {
                return fabs(currDepth-desiredDepth);
            };


            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \brief Saturates a value between max and min
            ///
            ///  \param[in] current value
            ///  \param[in] minimum value
            ///  \param[in] maximum value
            ///
            ///  \return Returns saturated value.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static double Saturate(const double value,
                                   const double min,
                                   const double max)
            {
                if(value<min)
                {
                    return min;
                }
                else if(value>max)
                {
                    return max;
                }
                else
                {
                    return value;
                }
            };
        };
    }
}

#endif // UTILITY_H
