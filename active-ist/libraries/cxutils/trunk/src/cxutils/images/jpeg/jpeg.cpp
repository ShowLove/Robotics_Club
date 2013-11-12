////////////////////////////////////////////////////////////////////////////////////
///
///  \file jpeg.cpp
///  \brief This file contains methods for compression and decompression
///         of JPEG data.
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
#include "cxutils/images/jpeg/jpeg.h"
#include "cxutils/packet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
//#include <iostream>


extern "C"
{
    #ifdef WIN32
    #include <jpeglib.h>
    #else
        #ifdef JPEG_TURBO_DIR
            #include JPEG_TURBO_DIR
        #else
            #include "linux/jpeglib.h"
        #endif
    #endif
}

const unsigned int gJPEGHeaderSize = 20;
unsigned char gJPEGHeader[gJPEGHeaderSize] =
{
    0xff,0xd8,                  // SOI
    0xff,0xe0,                  // APP0
    0x00,0x10,                  // APP0 Hdr size
    0x4a,0x46,0x49,0x46,0x00,   // ID string
    0x01,0x01,                  // Version
    0x00,                       // Bits per type
    0x00, 0x00,                 // X density
    0x00, 0x00,                 // Y density
    0x00,                       // X Thumbnail size
    0x00                        // Y Thumbnail size
};

const unsigned int gMJPGDHTSegSize = 420;

unsigned char gMJPGDHTSeg[gMJPGDHTSegSize] =
{
    /* JPEG DHT Segment for YCrCb omitted from MJPG data */
    0xFF,0xC4,0x01,0xA2,
    0x00,0x00,0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x01,0x00,0x03,0x01,0x01,0x01,0x01,
    0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
    0x08,0x09,0x0A,0x0B,0x10,0x00,0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,
    0x00,0x01,0x7D,0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,
    0x07,0x22,0x71,0x14,0x32,0x81,0x91,0xA1,0x08,0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,0x24,
    0x33,0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,0x25,0x26,0x27,0x28,0x29,0x2A,0x34,
    0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,0x54,0x55,0x56,
    0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,
    0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,
    0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,
    0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,
    0xDA,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,
    0xF8,0xF9,0xFA,0x11,0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,
    0x02,0x77,0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,
    0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xA1,0xB1,0xC1,0x09,0x23,0x33,0x52,0xF0,0x15,0x62,
    0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A,0x26,0x27,0x28,0x29,0x2A,
    0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,0x54,0x55,0x56,
    0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,
    0x79,0x7A,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,
    0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,
    0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,
    0xD9,0xDA,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,
    0xF9,0xFA
};

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Takes an MJPEG frame data, and converts to JPEG data.
///
///   \param[in] mjpeg Pointer to MJPEG data.
///   \param[in] mjpegSize The size in bytes of the MJPEG data.
///   \param[out] jpeg Pointer to buffer which will store the result.  If NULL
///                     or too small new memory will be allocated dynamically.
///                     Decompressed data will be in BGR format.
///   \param[out] jpegSize Pointer to output size of the jpeg buffer.  If a
///                        resize happens, this value will be modified.
///
///   \return Number of bytes written to buffer (0 on failure).
///
////////////////////////////////////////////////////////////////////////////////////
int CxUtils::JPEG::ConvertMJPEG2JPEG(const unsigned char* mjpeg,
                                     const unsigned int mjpegSize,
                                     unsigned char** jpeg,
                                     unsigned int* jpegSize)
{
    if(mjpeg == NULL || mjpegSize == 0)
    {
        return 0;
    }
    int aviHeaderSize = *(mjpeg + 4);
    aviHeaderSize <<= 8;
    aviHeaderSize += *(mjpeg + 5) + 4;
    if(aviHeaderSize >= (int)mjpegSize)
    {
        return 0;
    }
    int outputSize = mjpegSize + gMJPGDHTSegSize + gJPEGHeaderSize - aviHeaderSize;
    // Allocate memory to store the mjpeg image + jpeg header.
    if(*jpeg != NULL)
    {
        // See if we need to re-allocate. 
        if(outputSize >= (int)*jpegSize)
        {
            delete[] *jpeg;
            *jpeg = new unsigned char[outputSize + 128];
            // Reset the size of the buffer.
            *jpegSize = outputSize + 128;
        }
    }
    else
    {
        *jpeg = new unsigned char[outputSize + 128];
        // Reset the size of the buffer.
        *jpegSize = outputSize + 128;
    }
    // Now write to buffer.
    memcpy(*jpeg, gJPEGHeader, gJPEGHeaderSize);
    memcpy(*jpeg + gJPEGHeaderSize, gMJPGDHTSeg, gMJPGDHTSegSize);
    memcpy(*jpeg + gMJPGDHTSegSize + gJPEGHeaderSize, mjpeg + aviHeaderSize, mjpegSize - aviHeaderSize);

    return outputSize;
}

#include <setjmp.h>

#define JPEG_BUFFER_SIZE 4096

namespace CxUtils
{
    namespace JPEG
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \struct InputSource
        ///   \brief Input source for JPEG decompression manager.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        struct InputSource
        {
            struct
            {
                struct jpeg_error_mgr mManager; ///<  jpeg_error_mgr data.
                jmp_buf mSetJumpBuffer;         ///<  Buffer used on jump call.
            } mErrorManager;                    ///<  Error handling structure data.

            unsigned char* mpImage;                       ///<  Pointer to JPEG image data.
            unsigned int mImageSize;                      ///<  Size of the JPEG image in bytes.
            unsigned int mNumBytesRead;                   ///<  Total number of bytes decompressed/read so far.
            unsigned char mpBuffer[JPEG_BUFFER_SIZE];     ///<  Buffer storing JPEG data to decompress.
        };

        // Initializes source (does nothing).
        void InitSource(j_decompress_ptr cinfo)
        {
#ifdef JCS_EXTENSIONS
            cinfo->out_color_space = JCS_EXT_BGR;
#endif
        }


        // Reads data from the compressed image and puts it into 
        // a buffer for decompression by jpeg decompressor
        boolean FillInputBuffer(j_decompress_ptr cinfo)
        {
            struct InputSource* input = (struct InputSource*)cinfo->client_data;

            if(input->mImageSize <= 0)
            {
                // Insert fake EOI marker
                input->mpBuffer[0] = (JOCTET)0xFF;
                input->mpBuffer[1] = (JOCTET)JPEG_EOI;
                cinfo->src->bytes_in_buffer = 2;
            }
            if((unsigned int)(input->mImageSize - input->mNumBytesRead) < (unsigned int)JPEG_BUFFER_SIZE)
            {
                memcpy(input->mpBuffer, &input->mpImage[input->mNumBytesRead], input->mImageSize - input->mNumBytesRead);
                cinfo->src->bytes_in_buffer = input->mImageSize - input->mNumBytesRead;
            }
            else
            {
                memcpy(input->mpBuffer, &input->mpImage[input->mNumBytesRead], JPEG_BUFFER_SIZE);
                input->mNumBytesRead += JPEG_BUFFER_SIZE;
                cinfo->src->bytes_in_buffer = JPEG_BUFFER_SIZE;
            }
            cinfo->src->next_input_byte = input->mpBuffer;
            return TRUE;
        }

        // Handles skiped bytes.
        void SkipInputData(j_decompress_ptr cinfo, long num_bytes)
        {
            if(num_bytes > 0)
            {
                while(num_bytes > (long)cinfo->src->bytes_in_buffer)
                {
                    num_bytes -= (long)cinfo->src->bytes_in_buffer;
                    FillInputBuffer(cinfo);
                }
                cinfo->src->next_input_byte += (size_t)num_bytes;
                cinfo->src->bytes_in_buffer -= (size_t)num_bytes;
            }
        }

        // Terminates source data, does nothing for memory buffer.
        void TerminateSource(j_decompress_ptr cinfo)
        {
        }


        // Handles exit on errors.
        void ErrorExit(j_common_ptr cinfo)
        {
            struct InputSource* inputSource = (struct InputSource*)(cinfo->client_data);
            longjmp(inputSource->mErrorManager.mSetJumpBuffer, 1);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Takes JPEG data inside of a buffer and decompresses it to
///   another buffer which will store the raw image.
///
///   \param[in] jpeg Pointer to JPEG data.
///   \param[in] jpegSize The size in bytes of the JPEG data.
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
int CxUtils::JPEG::DecompressImage(const unsigned char* jpeg,
                                   const unsigned int jpegSize,
                                   unsigned char** image,
                                   unsigned short* width,
                                   unsigned short* height,
                                   unsigned char* channels)
{
    int result = 0;

    *width = *height = *channels = 0;

    // Create an input source for the JPEG source manager.
    struct InputSource inputSource;
    inputSource.mpImage = (unsigned char*)jpeg;
    inputSource.mImageSize = jpegSize;
    inputSource.mNumBytesRead = 0;

    // Create an input source manager for JPEG decompressor.
    jpeg_source_mgr mgr;
    mgr.bytes_in_buffer = 0;
    mgr.next_input_byte = NULL;
    mgr.init_source             = InitSource;
    mgr.fill_input_buffer       = FillInputBuffer;
    mgr.skip_input_data         = SkipInputData;
    mgr.resync_to_restart       = jpeg_resync_to_restart; // Default from library
    mgr.term_source             = TerminateSource;

    // Create a JPEG compression structure.
    struct jpeg_decompress_struct cinfo;

    // Initialize it.
    jpeg_create_decompress(&cinfo);

    // Set the input data for decompression.
    cinfo.client_data = &inputSource;
    // Set the error handler.
    cinfo.err = jpeg_std_error(&inputSource.mErrorManager.mManager);
    inputSource.mErrorManager.mManager.error_exit = ErrorExit;
    // Set jump state on error for decompressor, if failed
    // exit.
    if(setjmp(inputSource.mErrorManager.mSetJumpBuffer))
    {
        jpeg_destroy_decompress(&cinfo);
        return result;
    }
    
    // Assign the source manager to the decompressor.
    cinfo.src = &mgr;

    // Read the JPEG header data.
    jpeg_read_header(&cinfo, TRUE);

#ifdef JCS_EXTENSIONS
    cinfo.out_color_space = JCS_EXT_BGR;
    //std::cout << "JPEG_TURBO!\n";
#endif


    // Start decompression.
    jpeg_start_decompress(&cinfo);

    // Do we support the number of color components?
    // We only support grayscale or BGR images.
    if(cinfo.output_components != 1 && cinfo.output_components != 3)
    {
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        return result;
    }

    // Double check the color space that we support.
#ifdef JCS_EXTENSIONS
    if (cinfo.out_color_space == JCS_RGB || cinfo.out_color_space == JCS_EXT_BGR)
        *channels = (unsigned char)3;
#else
    if (cinfo.out_color_space == JCS_RGB)
        *channels = (unsigned char)3;
#endif
    else if (cinfo.out_color_space == JCS_GRAYSCALE)
        *channels = (unsigned char)1;
    else
    {
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        return result;
    }
    
    // Make a one-row high sample array that will be deleted when
    // done decompressing.  This will be used to store temp data.
    int row_stride = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer = NULL;
    buffer = (*cinfo.mem->alloc_sarray)
             ((j_common_ptr) &cinfo,
             JPOOL_IMAGE,
             row_stride,
             1);

    // Calculate size of output image in bytes.
    unsigned int outputSize;
    outputSize = cinfo.output_components*cinfo.output_width*cinfo.output_height;

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

    *width = (unsigned short)cinfo.output_width;
    *height = (unsigned short)cinfo.output_height;
    *channels = (unsigned char)cinfo.output_components;
    
    unsigned char* ptr = *image;
    int linesRead = 0;
    // Now decompress each scanline in the image.
    while (cinfo.output_scanline < cinfo.output_height) 
    {
        linesRead += (int)jpeg_read_scanlines(&cinfo, buffer, 1);
        //(void) jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(ptr, buffer[0], row_stride);
        ptr += row_stride;
    }
    
    if(linesRead != (int)cinfo.output_height)
    {
        return result;
    }

    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    
    result = 1;

    return result;
}

namespace CxUtils
{
    namespace JPEG
    {

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \struct OutputSource
        ///   \brief Output source for JPEG compression manager.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        struct OutputSource
        {
            struct jpeg_destination_mgr mDestinationManager;    ///<  Destination manager.
            unsigned char mOutputBuffer[JPEG_BUFFER_SIZE];      ///<  Output buffer to pull data for compression from.
            unsigned char* mpCompressedJPEG;                    ///<  Where to write compressed data to.
            unsigned int mCompressionBufferSize;                ///<  Total memory available for compressed data.
            unsigned int mNumBytesCompressed;                   ///<  Size of compressed JPEG inside of mpCompressedJPEG.
        };

        // Initializes destination manager.
        void InitDestination (j_compress_ptr cinfo)
        {
            struct OutputSource *dest = (struct OutputSource *) cinfo->dest;
            dest->mDestinationManager.next_output_byte = dest->mOutputBuffer;
            dest->mDestinationManager.free_in_buffer = JPEG_BUFFER_SIZE;
        }


        // Takes data from raw image and writes to temp buffer for compression.
        boolean EmptyOutputBuffer (j_compress_ptr cinfo)
        {
          struct OutputSource *dest = (struct OutputSource *) cinfo->dest;

          // See if we need to allocate more memory.
          if(dest->mpCompressedJPEG == NULL || dest->mNumBytesCompressed + JPEG_BUFFER_SIZE >= dest->mCompressionBufferSize)
          {
              unsigned char *tempOutput = new unsigned char[dest->mCompressionBufferSize + JPEG_BUFFER_SIZE*2];
              if(dest->mpCompressedJPEG)
              {
                  memcpy(tempOutput, dest->mpCompressedJPEG, dest->mCompressionBufferSize);
                  delete[] dest->mpCompressedJPEG;
              }
              dest->mpCompressedJPEG = tempOutput;
              dest->mCompressionBufferSize += JPEG_BUFFER_SIZE*2;
          }
          memcpy(&dest->mpCompressedJPEG[dest->mNumBytesCompressed], dest->mOutputBuffer, JPEG_BUFFER_SIZE);
          dest->mNumBytesCompressed += JPEG_BUFFER_SIZE;
          dest->mDestinationManager.next_output_byte = dest->mOutputBuffer;
          dest->mDestinationManager.free_in_buffer = JPEG_BUFFER_SIZE;

          return TRUE;
        }


        // Writes the last remaining bytes of the image for compression.
        void TerminateDestination(j_compress_ptr cinfo)
        {
            OutputSource *dest = (OutputSource *) cinfo->dest;
            unsigned int datacount = (unsigned int)(JPEG_BUFFER_SIZE - dest->mDestinationManager.free_in_buffer);
            // Write any remaining data.

            // See if we need to allocate more memory.
            if(dest->mpCompressedJPEG == NULL || dest->mNumBytesCompressed + datacount >= dest->mCompressionBufferSize)
            {
                unsigned char *tempOutput = new unsigned char[dest->mCompressionBufferSize + datacount];
                if(dest->mpCompressedJPEG)
                {
                    memcpy(tempOutput, dest->mpCompressedJPEG, dest->mCompressionBufferSize);
                    delete[] dest->mpCompressedJPEG;
                }
                dest->mpCompressedJPEG = tempOutput;
                dest->mCompressionBufferSize += datacount;
            }

            memcpy(&dest->mpCompressedJPEG[dest->mNumBytesCompressed], dest->mOutputBuffer, datacount);
            dest->mNumBytesCompressed += datacount;
            dest->mDestinationManager.next_output_byte = dest->mOutputBuffer;
            dest->mDestinationManager.free_in_buffer = JPEG_BUFFER_SIZE;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compresses RAW image data to JPEG format.
///
///   \param[in] image Pointer to raw image data that needs to be compressed.
///                    Data must be in BGR channel format.
///   \param[in] width The width of the image.
///   \param[in] height The height of the image.
///   \param[in] channels The number of channels in the image.
///   \param[out] jpeg Pointer to buffer which will store resulting JPEG
///                    data.  If NULL or too small this buffer will grow
///                    dynamically.
///   \param[out] jpegBufferSize The size of the buffer in bytes holding the
///                              compressed JPEG.
///   \param[out] jpegSize The size of the JPEG data inside the buffer in bytes.
///   \param[in] quality The compression quality.  Set to -1 for default, 0 worst
///                      and 100 best (but larger size).
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int CxUtils::JPEG::CompressImage(const unsigned short width,
                                 const unsigned short height,
                                 const unsigned char channels,
                                 const unsigned char* image,
                                 unsigned char** jpeg,
                                 unsigned int* jpegBufferSize,
                                 unsigned int* jpegSize,
                                 const int quality)
{
    int result = 0;

    if(width > 0 && 
        height > 0 && 
        (channels == 1 || channels == 3) &&
        image != 0 && 
        jpeg != 0)
    {
        unsigned int imageSize = width*height*channels*sizeof(unsigned char);
        // Reset the compressed JPEG size to 0.
        *jpegSize = 0;

        // If necessary, allocate some memory to store
        // the resulting compressed image.
        if(*jpeg == NULL || jpegBufferSize == 0)
        {
            if(*jpeg)
            {
                delete[] *jpeg;
                *jpeg = 0;
            }
            *jpeg = new unsigned char[imageSize];
            assert( *jpeg );
            *jpegBufferSize = imageSize;
        }

        // Create a JPEG compression structure and an error manager.
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;

        // Create a compression object.
        jpeg_create_compress(&cinfo);

        cinfo.err = jpeg_std_error(&jerr);
        // Create a compression output destination object.
        OutputSource dest;

        // Initialize output destination manager.
        dest.mpCompressedJPEG = *jpeg;
        dest.mCompressionBufferSize = *jpegBufferSize;
        dest.mNumBytesCompressed = 0;
        dest.mDestinationManager.init_destination = InitDestination;
        dest.mDestinationManager.empty_output_buffer = EmptyOutputBuffer;
        dest.mDestinationManager.term_destination = TerminateDestination;

        cinfo.dest = (struct jpeg_destination_mgr *)&dest;

        cinfo.image_width = width;    
        cinfo.image_height = height;
        cinfo.input_components = channels;
#ifdef JCS_EXTENSIONS
        cinfo.in_color_space = channels > 1 ? JCS_EXT_BGR : JCS_GRAYSCALE;
#else
        cinfo.in_color_space = channels > 1 ? JCS_RGB : JCS_GRAYSCALE;
#endif
        

        // Sets default values.
        jpeg_set_defaults(&cinfo);

        // Adjust quality if set.
        if(quality >= 0)
        {
            jpeg_set_quality(&cinfo, quality, TRUE);
        }

        // Start compression process
        jpeg_start_compress(&cinfo, TRUE);

        JSAMPROW row_pointer[1];    /// JSAMPLE row to store a single compressed row.
        int row_stride;             /// Physical row width in image buffer (width*channels).
        row_stride = width*channels;
        
        while (cinfo.next_scanline < cinfo.image_height)
        {
            // jpeg_write_scanlines expects an array of pointers to scanlines.
            // Here the array is only one element long, but you could pass
            // more than one scanline at a time if that's more convenient.
            row_pointer[0] = & ((unsigned char *)(image)) [cinfo.next_scanline * row_stride];
            (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);

        }

        // Finish compression (finalize).
        jpeg_finish_compress(&cinfo);

        // Save results
        *jpegSize = dest.mNumBytesCompressed;
        *jpegBufferSize = dest.mCompressionBufferSize;
        *jpeg = dest.mpCompressedJPEG; // Make sure pointer is saved.

        // Destroy the compression object.
        jpeg_destroy_compress(&cinfo);

        result = 1;
    }

    return result;
}


namespace CxUtils
{
    namespace JPEG
    {
        Compressor::Compressor() : mpCompressionObject(0)
        {
        }
        Compressor::~Compressor()
        {
            if(mpCompressionObject)
            {
                jpeg_destroy_compress((j_compress_ptr)(mpCompressionObject));
                delete ((jpeg_compress_struct *)(mpCompressionObject));
            }
            mpCompressionObject = 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Compresses RAW image data to JPEG format.
        ///
        ///   \param[in] image Pointer to raw image data that needs to be compressed.
        ///   \param[in] width The width of the image.
        ///   \param[in] height The height of the image.
        ///   \param[in] channels The number of channels in the image.
        ///   \param[out] jpeg Pointer to buffer which will store resulting JPEG
        ///                    data.  If NULL or too small this buffer will grow
        ///                    dynamically.
        ///   \param[out] jpegBufferSize The size of the buffer in bytes holding the
        ///                              compressed JPEG.
        ///   \param[out] jpegSize The size of the JPEG data inside the buffer in bytes.
        ///   \param[in] quality The compression quality.  Set to -1 for default, 0 worst
        ///                      and 100 best (but larger size).
        ///
        ///   \return 1 on success, 0 on failure.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        int Compressor::CompressImage(const unsigned short width,
                                      const unsigned short height,
                                      const unsigned char channels,
                                      const unsigned char* image,
                                      unsigned char** jpeg,
                                      unsigned int* jpegBufferSize,
                                      unsigned int* jpegSize,
                                      const int quality)
        {
            int result = 0;

            if(width > 0 && 
                height > 0 && 
                (channels == 1 || channels == 3) &&
                image != 0 && 
                jpeg != 0)
            {
                unsigned int imageSize = width*height*channels*sizeof(unsigned char);
                // Reset the compressed JPEG size to 0.
                *jpegSize = 0;

                // If necessary, allocate some memory to store
                // the resulting compressed image.
                if(*jpeg == NULL || jpegBufferSize == 0)
                {
                    if(*jpeg)
                    {
                        delete[] *jpeg;
                        *jpeg = 0;
                    }
                    *jpeg = new unsigned char[imageSize];
                    assert( *jpeg );
                    *jpegBufferSize = imageSize;
                }

                // Create a JPEG compression structure and an error manager.
                if(mpCompressionObject == 0)
                {
                    mpCompressionObject = (void *)(new jpeg_compress_struct);
                    // Create a compression object.
                     jpeg_create_compress((j_compress_ptr)(mpCompressionObject));
                }
                j_compress_ptr cinfo = (j_compress_ptr)(mpCompressionObject);
                struct jpeg_error_mgr jerr;

                
                cinfo->err = jpeg_std_error(&jerr);
                // Create a compression output destination object.
                OutputSource dest;

                // Initialize output destination manager.
                dest.mpCompressedJPEG = *jpeg;
                dest.mCompressionBufferSize = *jpegBufferSize;
                dest.mNumBytesCompressed = 0;
                dest.mDestinationManager.init_destination = InitDestination;
                dest.mDestinationManager.empty_output_buffer = EmptyOutputBuffer;
                dest.mDestinationManager.term_destination = TerminateDestination;

                cinfo->dest = (struct jpeg_destination_mgr *)&dest;

                cinfo->image_width = width;    
                cinfo->image_height = height;
                cinfo->input_components = channels; 
#ifdef JCS_EXTENSIONS
                cinfo->in_color_space = channels > 1 ? JCS_EXT_BGR : JCS_GRAYSCALE;
#else
                cinfo->in_color_space = channels > 1 ? JCS_RGB : JCS_GRAYSCALE;
#endif

                // Sets default values.
                jpeg_set_defaults(cinfo);

                // Adjust quality if set.
                if(quality >= 0)
                {
                    jpeg_set_quality(cinfo, quality, TRUE);
                }

                // Start compression process
                jpeg_start_compress(cinfo, TRUE);

                JSAMPROW row_pointer[1];    /// JSAMPLE row to store a single compressed row.
                int row_stride;             /// Physical row width in image buffer (width*channels).
                row_stride = width*channels;
                
                while (cinfo->next_scanline < cinfo->image_height)
                {
                    // jpeg_write_scanlines expects an array of pointers to scanlines.
                    // Here the array is only one element long, but you could pass
                    // more than one scanline at a time if that's more convenient.
                    row_pointer[0] = & ((unsigned char *)(image)) [cinfo->next_scanline * row_stride];
                    (void) jpeg_write_scanlines(cinfo, row_pointer, 1);

                }

                // Finish compression (finalize).
                jpeg_finish_compress(cinfo);

                // Save results
                *jpegSize = dest.mNumBytesCompressed;
                *jpegBufferSize = dest.mCompressionBufferSize;
                *jpeg = dest.mpCompressedJPEG; // Make sure pointer is saved.

                result = 1;
            }

            return result;
        }

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Compresses RAW image data to JPEG format.
        ///
        ///   \param[in] image Pointer to raw image data that needs to be compressed.
        ///   \param[in] width The width of the image.
        ///   \param[in] height The height of the image.
        ///   \param[in] channels The number of channels in the image.
        ///   \param[out] jpeg Pointer to buffer which will store resulting JPEG
        ///                    data.  If NULL or too small this buffer will grow
        ///                    dynamically.
        ///   \param[out] jpegBufferSize The size of the buffer in bytes holding the
        ///                              compressed JPEG.
        ///   \param[out] jpegSize The size of the JPEG data inside the buffer in bytes.
        ///   \param[in] quality The compression quality.  Set to -1 for default, 0 worst
        ///                      and 100 best (but larger size).
        ///
        ///   \return 1 on success, 0 on failure.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        int Compressor::CompressImageNoResize(const unsigned short width,
                                              const unsigned short height,
                                              const unsigned char channels,
                                              const unsigned char* image,
                                              unsigned char* jpeg,
                                              unsigned int jpegBufferSize,
                                              unsigned int* jpegSize,
                                              const int quality)
        {
            int result = 0;

            if(width > 0 && 
                height > 0 && 
                (channels == 1 || channels == 3) &&
                image != 0 && 
                jpeg != 0)
            {
                unsigned int imageSize = width*height*channels*sizeof(unsigned char);
                // Reset the compressed JPEG size to 0.
                *jpegSize = 0;

                // If necessary, allocate some memory to store
                // the resulting compressed image.
                if(jpeg == NULL || jpegBufferSize < imageSize)
                {
                    return 0;
                }

                // Create a JPEG compression structure and an error manager.
                if(mpCompressionObject == 0)
                {
                    mpCompressionObject = (void *)(new jpeg_compress_struct);
                    // Create a compression object.
                     jpeg_create_compress((j_compress_ptr)(mpCompressionObject));
                }
                j_compress_ptr cinfo = (j_compress_ptr)(mpCompressionObject);
                struct jpeg_error_mgr jerr;

                
                cinfo->err = jpeg_std_error(&jerr);
                // Create a compression output destination object.
                OutputSource dest;

                // Initialize output destination manager.
                dest.mpCompressedJPEG = jpeg;
                dest.mCompressionBufferSize = jpegBufferSize;
                dest.mNumBytesCompressed = 0;
                dest.mDestinationManager.init_destination = InitDestination;
                dest.mDestinationManager.empty_output_buffer = EmptyOutputBuffer;
                dest.mDestinationManager.term_destination = TerminateDestination;

                cinfo->dest = (struct jpeg_destination_mgr *)&dest;

                cinfo->image_width = width;    
                cinfo->image_height = height;
                cinfo->input_components = channels; 
#ifdef JCS_EXTENSIONS
                cinfo->in_color_space = channels > 1 ? JCS_EXT_BGR : JCS_GRAYSCALE;
#else
                cinfo->in_color_space = channels > 1 ? JCS_RGB : JCS_GRAYSCALE;
#endif

                // Sets default values.
                jpeg_set_defaults(cinfo);

                // Adjust quality if set.
                if(quality >= 0)
                {
                    jpeg_set_quality(cinfo, quality, TRUE);
                }

                // Start compression process
                jpeg_start_compress(cinfo, TRUE);

                JSAMPROW row_pointer[1];    /// JSAMPLE row to store a single compressed row.
                int row_stride;             /// Physical row width in image buffer (width*channels).
                row_stride = width*channels;
                
                while (cinfo->next_scanline < cinfo->image_height)
                {
                    // jpeg_write_scanlines expects an array of pointers to scanlines.
                    // Here the array is only one element long, but you could pass
                    // more than one scanline at a time if that's more convenient.
                    row_pointer[0] = & ((unsigned char *)(image)) [cinfo->next_scanline * row_stride];
                    (void) jpeg_write_scanlines(cinfo, row_pointer, 1);

                }

                // Finish compression (finalize).
                jpeg_finish_compress(cinfo);

                // Save results
                *jpegSize = dest.mNumBytesCompressed;
                
                result = 1;
            }

            return result;
        }        
    }
}


/* End of File */
