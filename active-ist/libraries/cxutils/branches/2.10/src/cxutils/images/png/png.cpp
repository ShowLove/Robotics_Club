////////////////////////////////////////////////////////////////////////////////////
///
///  \file png.cpp
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
#include "cxutils/images/png/png.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

extern "C"
{
#include <png.h>
}

#ifdef WIN32

#define XMD_H // prevent redefinition of INT32
#undef FAR    // prevent FAR redefinition

#endif

namespace CxUtils
{
    namespace PNG
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \struct InputSource
        ///   \brief Input source for PNG decompression manager.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        struct InputSource
        {
            unsigned char* mpImage;                       ///<  Pointer to PNG image data.
            unsigned int mImageSize;                      ///<  Size of the PNG image in bytes.
            unsigned int mNumBytesRead;                   ///<  Total number of bytes decompressed/read so far.
        };

        // Function to read data for PNG library.
        void ReadData(png_structp png_ptr, png_bytep data, png_size_t length)
        {
            unsigned int bytesToRead = (unsigned int)length;
            struct InputSource* input = (struct InputSource*)png_ptr->io_ptr;

            if(bytesToRead > input->mImageSize - input->mNumBytesRead)
            {
                bytesToRead = input->mImageSize - input->mNumBytesRead;
            }
            if(bytesToRead > 0)
            {
                memcpy(data, &input->mpImage[input->mNumBytesRead], bytesToRead);
                input->mNumBytesRead += bytesToRead;
            }
        }
    }
}

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
int CxUtils::PNG::DecompressImage(const unsigned char* png,
                                  const unsigned int pngSize,
                                  unsigned char** image,
                                  unsigned short* width,
                                  unsigned short* height,
                                  unsigned char* channels)
{
    png_structp      png_ptr = NULL;
    png_infop        info_ptr = NULL;
    png_byte *ptr = (png_byte *)png;

    int is_png = !png_sig_cmp(ptr, 0, 8);

    if (is_png)
    {
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if(png_ptr)
        {
            info_ptr = png_create_info_struct(png_ptr);
        }
        else
        {
            return 0;
        }
    }
    else 
    {
        return 0;
    }

    if (info_ptr)
    {   
        // Create an input structure and
        // assign it and read method to 
        // the png decompressor structure.
        struct InputSource input;

        input.mImageSize = pngSize;
        input.mpImage = (unsigned char *)png;
        input.mNumBytesRead = 0;

        png_ptr->io_ptr = &input;
        png_ptr->read_data_fn = ReadData;

        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_BGR, NULL);

        // Calculate size of output image in bytes.
        unsigned int outputSize;
        outputSize = info_ptr->width*info_ptr->height*info_ptr->channels;

        // Allocate memory to store the decompressed image.
        if(*image != NULL)
        {
            // See if we need to re-allocate. 
            if(outputSize >= (unsigned int)(*width)*(*height)*(*channels))
            {
                delete[] *image;
                *image = new unsigned char[outputSize + 100];
            }
        }
        else
        {
            *image = new unsigned char[outputSize + 100];
        }

        *width = (unsigned short)info_ptr->width;
        *height = (unsigned short)info_ptr->height;
        *channels = (unsigned char)info_ptr->channels;

        png_bytep * row_pointers = NULL;
        row_pointers = NULL;

        int row_stride = (*width)*(*channels)*sizeof(unsigned char);
        if (info_ptr->color_type == PNG_COLOR_TYPE_RGB || info_ptr->color_type == PNG_COLOR_TYPE_GRAY)
        {
            row_pointers = png_get_rows(png_ptr, info_ptr);

            for (unsigned int i = 0; i < *height; i++)
            {
                memcpy( *image + i*row_stride, row_pointers[i], row_stride);
            }
        }

        if(png_ptr && info_ptr)
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        }
        else if(png_ptr)
        {
            png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        }

        return 1;
    }
    
    return 0;
}

namespace CxUtils
{
    namespace PNG
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \struct OutputSource
        ///   \brief Output source for PNG compression manager.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        struct OutputSource
        {
            unsigned char* mpCompressedPNG;             ///<  Pointer to PNG image data.
            unsigned int mCompressedBufferSize;         ///<  Size of the PNG image in bytes.
            unsigned int mNumBytesCompressed;           ///<  Total number of bytes decompressed/read so far.
        };

        // Function to read data for PNG library.
        void WriteData(png_structp png_ptr, png_bytep data, png_size_t length)
        {
            struct OutputSource* output = (struct OutputSource*)png_ptr->io_ptr;
            
            if(output->mNumBytesCompressed + length > output->mCompressedBufferSize)
            {
                unsigned char* temp = new unsigned char[output->mCompressedBufferSize + length];
                memcpy(temp, output->mpCompressedPNG, output->mNumBytesCompressed);
                delete[] output->mpCompressedPNG;
                output->mpCompressedPNG = temp;
                output->mCompressedBufferSize += (unsigned int)length;        
            }
            memcpy(&output->mpCompressedPNG[output->mNumBytesCompressed], data, length);
            output->mNumBytesCompressed += (unsigned int)length;
        }
    }
}

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
int CxUtils::PNG::CompressImage(const unsigned short width,
                                const unsigned short height,
                                const unsigned char channels,
                                const unsigned char* image,
                                unsigned char** png,
                                unsigned int* pngBufferSize,
                                unsigned int* pngSize)
{
    int result = 0;

    if( width == 0 || 
        height == 0 || 
        !(channels == 1 || channels == 3) ||
        image == NULL || 
        png == NULL ) 
    {
        return result;
    }

    if( channels == 3 || channels == 1 )
    {
        if(*pngBufferSize == 0 || *png == NULL)
        {
            // Delete memory if it already exists
            if( (*png) )
            {
                delete[] *png;
                *png = NULL;
            }
            *png = new unsigned char[4096];
            *pngBufferSize = 4096;
        }
        *pngSize = 0;

        png_structp     png_ptr = NULL;
        png_bytep     * row_pointers = NULL;

        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        png_infop       info_ptr = png_create_info_struct(png_ptr);

        // Initialize the output devices and function
        // for png compression
        struct OutputSource output;
        output.mCompressedBufferSize = *pngBufferSize;
        output.mNumBytesCompressed = 0;
        output.mpCompressedPNG = *png;

        png_ptr->io_ptr = &output;
        png_ptr->write_data_fn = WriteData;

        png_byte color;
        if (channels == 1)
        {
            color = PNG_COLOR_TYPE_GRAY;
        }
        else if (channels == 3)
        {
            color = PNG_COLOR_TYPE_RGB;
        }
        else
        {
            color = PNG_COLOR_TYPE_RGB;
        }


        png_set_IHDR(png_ptr, info_ptr, width, height,
                8, color, PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        row_pointers = (png_bytep*)malloc(height*sizeof(png_bytep));
        int row_stride = width*channels*sizeof(unsigned char);
        // Go and assign row pointers.
        for (int i=0 ; i< height; i++)
        {
            row_pointers[i] = (png_byte*)&image[i*row_stride];

        }

        png_set_rows(png_ptr, info_ptr, row_pointers);
        png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_BGR, NULL);

        free(row_pointers);

        *png = output.mpCompressedPNG;
        *pngBufferSize = output.mCompressedBufferSize;
        *pngSize = output.mNumBytesCompressed;

        png_destroy_write_struct(&png_ptr, &info_ptr);

        result = 1;

    }

    return result;
}


/*  End of File */
