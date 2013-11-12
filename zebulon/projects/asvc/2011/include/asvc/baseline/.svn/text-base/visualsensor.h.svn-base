////////////////////////////////////////////////////////////////////////////////////
///
///  \file visualsensor.h
///  \brief Contains the Visual Sensor Service implementation.
///
///  <br>Author(s): Brian Valentino
///  <br>Created: 7 March 2011
///  <br>Copyright (c) 2011
///  <br>Robotics Club at UCF
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: bmvalentino@gmail.com
///  <br>Web:  http://robotics.ucf.edu
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
#ifndef __SURFACE_VISUAL_SENSOR_H
#define __SURFACE_VISUAL_SENSOR_H

#include <jaus/extras/video/visualsensor.h>
#include <cxutils/cxutils.h>
#include <cvcapture/cvimagecapture.h>

namespace Zebulon
{
    namespace ASVC
    {
        ///////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class VisualSensor
        ///   \brief This class creates a visual sensor, which inherits off of the JAUS
        ///          Visual Sensor and CvImageCapture callback to share
        ///          video data.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class VisualSensor    : public JAUS::VisualSensor,
                                public Video::CvImageCapture::Callback
        {
            public:
                // Constructor.
                VisualSensor();
                // Destructor.
                virtual ~VisualSensor();
                // Callback function when image data is available.
                virtual void ProcessCameraData(Video::CvImageCapture::Image* dest);
        };
    }
}

#endif