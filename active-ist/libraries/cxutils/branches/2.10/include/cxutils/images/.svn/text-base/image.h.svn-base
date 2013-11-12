////////////////////////////////////////////////////////////////////////////////////
///
///  \file image.h
///  \brief Simple image data structure for used with compression/decompression
///  routines.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 5 April 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
#ifndef __CXUTILS_IMAGES_IMAGE__H
#define __CXUTILS_IMAGES_IMAGE__H

#include <string>
#include <iostream>
#include "cxutils/images/jpeg/jpeg.h"
#include "cxutils/images/png/png.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///  \class Image
    ///  \brief Simple image data structure for storing RAW image data and
    ///         for easy conversion to/from a few compression formats.
    ///
    ///  The format for image data in this library must be BGR, not RGB, otherwise
    ///  your images will look weird!  BGR is used because this is the format that
    ///  most used format from cameras and other capture devices.
    ///
    ///  Image compression and decompression software and image data storage.
    ///  formats supported:
    ///      + JPG  - A modified version of libjpeg is used to convert channel order
    ///               to BGR from RGB.
    ///      + PNG  - libpng is used
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Image
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \brief Enumeration of possible image formats supported by this library.
        ///         However, not all are supported.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        enum Format
        {
            Unused = 0,
            Invalid = 0,
            MPEG2,
            MPEG4,
            MJPEG, // Supported
            NTSC,
            PAL,
            TIFF,
            JPEG,  // Supported
            GIF,
            H263,
            H264,
            PNG,   // Supported
            BMP,
            RAW,
            PPM,
            PGM,
            PNM
        };
        Image();
        Image(const Image& img);
        ~Image();
        int Create(const unsigned short width,
                   const unsigned short height,
                   const unsigned char channels,
                   const unsigned char* rawImage = NULL,
                   const bool verticalFlip = false,
                   const bool horizontalFlip = false);
        int Create(const unsigned short width,
                   const unsigned short height,
                   const unsigned char channels,
                   const unsigned char* rawImage,
                   const double scale,
                   const bool verticalFlip = false,
                   const bool horizontalFlip = false);
        int Create(const unsigned short width,
                   const unsigned short height,
                   const unsigned char channels,
                   const unsigned char* rawImage,
                   const unsigned short maxWidth,
                   const unsigned short maxHeight,
                   const bool verticalFlip);
        int CreateImage(const unsigned short width,
                   const unsigned short height,
                   const unsigned char channels,
                   const unsigned char* rawImage,
                   const unsigned short maxWidth,
                   const unsigned short maxHeight,
                   const bool verticalFlip = false,
                   const bool horizontalFlip = false);

        int Destroy();
        int Decompress(const unsigned char* compressed,
                       const unsigned int len,
                       const Format format);
        int Compress(unsigned char** buffer,
                     unsigned int* len,
                     unsigned int* clen,
                     const Format format,
                     void* args = NULL) const;
        static int Compress(unsigned char* image,
                            unsigned int width,
                            unsigned int height,
                            unsigned char channels,
                            unsigned char** buffer,
                            unsigned int* len,
                            unsigned int* clen,
                            const Format format,
                            void* args = NULL);
        void ApplyTile(const Image& tile);
        int Load(const std::string& file);
        int Save(const std::string& file) const;
        static int SaveCompressedImage(const std::string& name,
                                       const unsigned char* data,
                                       const unsigned int size,
                                       const Format format);
        static Format GetFormat(const std::string& file);
        inline void FlipChannels()
        {
            if(mpImage && mChannels == 3)
            {
                FlipColorChannels(mpImage, mWidth, mHeight);
            }
        }
        inline static void FlipColorChannels(unsigned char* colorImage, unsigned int width, unsigned int height)
        {
            // Flip color channels to RGB->BGR or BGR-RGB Depending on Input.
            unsigned char* ptr = colorImage;
            unsigned char temp;
            unsigned int size = width*height*3;
            for(unsigned int i = 0; i < size; i+= 3)
            {
                temp = *ptr;
                *ptr = *(ptr + 2);
                *(ptr + 2) = temp;
                ptr += 3;
            }
        }
        Image& operator=(const Image& img);

        unsigned char mChannels;        ///<  Number of color channels 1, or 3.
        unsigned short mWidth;          ///<  Horizontal resolution of image in pixels.
        unsigned short mHeight;         ///<  Vertical resolution of image in pixels.
        unsigned int mDataSize;         ///<  Size of RAW data in bytes.
        unsigned char* mpImage;         ///<  Raw uncompressed image data.
    };
}

#endif
/* End of File */
