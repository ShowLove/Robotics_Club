////////////////////////////////////////////////////////////////////////////////////
///
///  \file png.h
///  \brief This file contains methods for compression and decompression
///         of PNG data.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 21 November 2008
///  <br>Copyright (c) 2008
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
#ifndef __CXUTILS_IMAGES_PNG_HANDLER__H
#define __CXUTILS_IMAGES_PNG_HANDLER__H

#include "cxutils/cxbase.h"

namespace CxUtils
{
    namespace PNG
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Takes PNG data inside of a buffer and decompresses it to
        ///   another buffer which will store the raw image.
        ///
        ///   \param[in] png Pointer to PNG data.
        ///   \param[in] pngSize The size in bytes of the PNG data.
        ///   \param[out] image Pointer to buffer which will store the result.  If NULL
        ///                     or too small new memory will be allocated dynamically.
        ///                     Decompressed data will be in BGR format.
        ///   \param[out] width The width of the image decompressed.
        ///   \param[out] height The height of the image decompressed.
        ///   \param[out] channels The number of channels in the decompressed image.
        ///
        ///   \return 1 on success, 0 on failure.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        CX_UTILS_DLL int DecompressImage(const unsigned char* png,
                                         const unsigned int pngSize,
                                         unsigned char** image,
                                         unsigned short* width,
                                         unsigned short* height,
                                         unsigned char* channels);
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Compresses RAW image data to PNG format.
        ///
        ///   \param[in] image Pointer to raw image data that needs to be compressed.
        ///                    Data must be in BGR channel format.
        ///   \param[in] width The width of the image.
        ///   \param[in] height The height of the image.
        ///   \param[in] channels The number of channels in the image.
        ///   \param[out] png Pointer to buffer which will store resulting PNG
        ///                    data.  If NULL or too small this buffer will grow
        ///                    dynamically.
        ///   \param[out] pngBufferSize The size of the buffer in bytes holding the
        ///                              compressed PNG.
        ///   \param[out] pngSize The size of the PNG data inside the buffer in bytes.
        ///
        ///   \return 1 on success, 0 on failure.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        CX_UTILS_DLL int CompressImage(const unsigned short width,
                                       const unsigned short height,
                                       const unsigned char channels,
                                       const unsigned char* image,
                                       unsigned char** png,
                                       unsigned int* pngBufferSize,
                                       unsigned int* pngSize);
    }
}

#endif
/*  End of File */
