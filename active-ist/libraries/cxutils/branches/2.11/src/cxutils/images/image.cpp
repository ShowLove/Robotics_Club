////////////////////////////////////////////////////////////////////////////////////
///
///  \file image.cpp
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
#include "cxutils/images/image.h"
#include "cxutils/fileio.h"
#include <assert.h>
#include <string.h>
#include <cstdio>

using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Image::Image()
{
    mpImage = NULL;
    mDataSize = 0;
    mWidth = 0;
    mHeight = 0;
    mChannels = 0;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Image::Image(const Image& img)
{
    mpImage = NULL;
    mDataSize = 0;
    mWidth = 0;
    mHeight = 0;
    mChannels = 0;
    *this = img;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.  Destroys the Image object and closes the temporary file
///  produced by the mVirtual file
///
////////////////////////////////////////////////////////////////////////////////////
Image::~Image()
{
    Destroy();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief This initializes the Image
///
///  \param[in] width The width of the image
///  \param[in] height The height of the image
///  \param[in] channels Number of channels used (1, 3, or 4)
///  \param[in] rawImage The image data to be copied to the image, if
///                      the pointer is NULL (default), only memory allocation
///                      takes place an no copying.
///  \param[in] verticalFlip If true, image is flipped vertically when copied.
///  \param[in] horizontalFlip If true, image is flipped horizontally when copied.
///
///  \return 1 on success, 0 on failure
///
////////////////////////////////////////////////////////////////////////////////////
int Image::Create(const unsigned short width,
                  const unsigned short height,
                  const unsigned char channels,
                  const unsigned char* rawImage,
                  const bool verticalFlip,
                  const bool horizontalFlip)
{
    if(width == 0 || height == 0 || !(channels == 1 || channels == 3 || channels == 4) )
    {
        return 0;
    }

    if (mHeight != height || mWidth != width || mChannels != channels)
    {
        Destroy();
        mHeight = height;
        mWidth = width;
        mChannels = channels;
        mDataSize = mHeight*mWidth*mChannels*sizeof(unsigned char);
        mpImage = new unsigned char[mDataSize + 100];
        assert(mpImage);
    }

    if (rawImage != NULL)
    {
        //if(verticalFlip)
        //{
        //    unsigned int widthStep = width*channels;
        //    for(unsigned int row1 = 0, row2 = height - 1;
        //        row1 < height;
        //        row1++, row2--)
        //    {
        //        memcpy(&mpImage[row1*widthStep], &rawImage[row2*widthStep], widthStep);
        //    }
        //}
        if(verticalFlip || horizontalFlip)
        {
            unsigned int widthStep = width*channels;
            for(unsigned int y = 0, y2 = height-1; y < height; y++, y2--)
            {
                if(verticalFlip && !horizontalFlip)
                {
                    memcpy(&mpImage[y*widthStep], &rawImage[y2*widthStep], widthStep);
                    continue;
                }
                for(unsigned int x = 0, x2 = width -1; x < width; x++, x2--)
                {
                    if(horizontalFlip && !verticalFlip)
                    {
                        memcpy(&mpImage[y*widthStep + x*channels], &rawImage[y*widthStep + x2*channels], channels);
                    }
                    if(verticalFlip && horizontalFlip)
                    {
                        memcpy(&mpImage[y*widthStep + x*channels], &rawImage[y2*widthStep + x2*channels], channels);
                    }
                }
            }
        }
        else
        {
            memcpy(mpImage, rawImage, mDataSize);
        }

    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief This initializes the Image
///
///  \param[in] width The width of the image
///  \param[in] height The height of the image
///  \param[in] channels Number of channels used (1, 3, or 4)
///  \param[in] rawImage The image data to be copied to the image, if
///                      the pointer is NULL (default), only memory allocation
///                      takes place an no copying.
///  \param[in] scale How much to scale the image data.
///  \param[in] verticalFlip If true, image is flipped vertically when copied.
///  \param[in] horizontalFlip If true, image is flipped horizontally when copied.
///
///  \return 1 on success, 0 on failure
///
////////////////////////////////////////////////////////////////////////////////////
int Image::Create(const unsigned short width,
                  const unsigned short height,
                  const unsigned char channels,
                  const unsigned char* rawImage,
                  const double scale,
                  const bool verticalFlip,
                  const bool horizontalFlip)
{
    // Must have image data.
    if(rawImage == NULL)
    {
        return 0;
    }

    if(scale >=  0.99 && scale <= 1.01)
    {
        return Create(width, height, channels, rawImage, verticalFlip);
    }

    unsigned short newHeight, newWidth;
    double magnitude;
    newHeight = (unsigned short)(height*scale);
    newWidth = (unsigned short)(width*scale);

    if(newHeight == 0 || newWidth == 0) { return 0; }

    if(scale < 1.0)
    {
        magnitude = (double)(height)/newHeight;
    }
    else
    {
        magnitude = (double)(newHeight)/height;
    }

    // Allocate memory.
    Create(newWidth, newHeight, channels, NULL, false);
    unsigned short x, y;
    if(scale < 1) // Sub sample image
    {
        for(unsigned short i = 0; i < newHeight; i++)
        {
            if(verticalFlip)
            {
                y = (unsigned short)((newHeight - i - 1)*magnitude);
            }
            else
            {
                y = (unsigned short)(i*magnitude);
            }
            for(unsigned short j = 0; j < newWidth; j++)
            {
                if(horizontalFlip)
                {
                    x = (unsigned short) ((newWidth - j - 1) * magnitude);
                }
                else
                {
                    x = (unsigned short)(j*magnitude);
                }
                if(channels == 1)
                {
                    mpImage[i*newWidth + j] = rawImage[y*width + x];
                }
                else if(channels == 3)
                {
                    //x = (unsigned short)(j*magnitude);
                    mpImage[i*newWidth*3 + j*3] = rawImage[y*width*3 + x*3];
                    mpImage[i*newWidth*3 + j*3 + 1] = rawImage[y*width*3 + x*3 + 1];
                    mpImage[i*newWidth*3 + j*3 + 2] = rawImage[y*width*3 + x*3 + 2];
                }
                else
                {
                    //x = (unsigned short)(j*magnitude);
                    for(unsigned short k = 0; k < channels; k++)
                    {
                        mpImage[i*newWidth*channels + j*channels + k] = rawImage[y*width*channels + x*channels + k];
                    }
                }
            }
        }
    }
    else          // Enlarge image.
    {
        for(unsigned short i = 0; i < height; i++)
        {
            if(verticalFlip)
            {
                y = (height - i - 1);
            }
            else
            {
                y = i;
            }
            for(unsigned short j = 0; j < width; j++)
            {
                if(horizontalFlip)
                {
                    x = (width - j - 1);
                }
                else
                {
                    x = j;
                }
                // Copy the pixel multiple times based on how much larger
                    // it needs to be.
                for(unsigned short m = (unsigned short)(i*magnitude); m < (unsigned short)((i+1)*magnitude); m++)
                {
                    for(unsigned short n = (unsigned short)(j*magnitude); n < (unsigned short)((j+1)*magnitude); n++)
                    {
                        if(channels == 1)
                        {
                            mpImage[m*newWidth + n] = rawImage[y*width + x];
                        }
                        else if(channels == 3)
                        {
                            mpImage[m*newWidth*3 + n*3] = rawImage[y*width*3 + x*3];
                            mpImage[m*newWidth*3 + n*3 + 1] = rawImage[y*width*3 + x*3 + 1];
                            mpImage[m*newWidth*3 + n*3 + 2] = rawImage[y*width*3 + x*3 + 2];
                        }
                        else
                        {
                            for(unsigned short k = 0; k < channels; k++)
                            {
                                mpImage[m*newWidth*channels + n*channels + k] = rawImage[y*width*channels + x*channels + k];
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief This initializes the Image. Added because of ambiguous function 
///         overload conflicts. Use if horizontal flip is needed.
///
///  \param[in] width The width of the image
///  \param[in] height The height of the image
///  \param[in] channels Number of channels used (1, 3, or 4)
///  \param[in] rawImage The image data to be copied to the image, if
///                      the pointer is NULL (default), only memory allocation
///                      takes place an no copying.
///  \param[in] maxWidth Maximum width to store image as (will force scaling).
///  \param[in] maxHeight Maximum height to store image as (will force scaling).
///  \param[in] verticalFlip If true, image is flipped vertically when copied.
///  \param[in] horizontalFlip If true, image is flipped horizontally when copied.
///
///  \return 1 on success, 0 on failure
///
////////////////////////////////////////////////////////////////////////////////////
int Image::CreateImage(const unsigned short width,
           const unsigned short height,
           const unsigned char channels,
           const unsigned char* rawImage,
           const unsigned short maxWidth,
           const unsigned short maxHeight,
           const bool verticalFlip,
           const bool horizontalFlip)
{
    Image temp;
    // Find out how much to scaled image to fit scaled Width or Height.
    double scaleFactor1 = 1.0, scaleFactor2 = 1.0;

    if(maxWidth == 0 || maxHeight == 0)
    {
        return Create(width, height, channels, rawImage, verticalFlip, horizontalFlip);
    }
    /*
    if(maxWidth > width && maxHeight > height)
    {
        return 0;
    }
    */
    scaleFactor1 = (double)(maxWidth)/width;
    scaleFactor2 = (double)(maxHeight)/height;
    if(scaleFactor2 < scaleFactor1)
    {
        scaleFactor1 = scaleFactor2;
    }

    if(temp.Create(width, height, channels, rawImage, scaleFactor1, verticalFlip, horizontalFlip) == 0)
    {
        return 0;
    }

    // Now copy the data internally, but make it fit in the center.
    Create(maxWidth, maxHeight, channels, NULL, false);
    memset(mpImage, 0, maxWidth*maxHeight*channels*sizeof(unsigned char));
    unsigned char* ptr1, *ptr2;
    int startRow, startCol;
    ptr1 = mpImage;
    ptr2 = temp.mpImage;

    // Calculate where to start copying the scaled image into the
    // width/height contrained image.
    startRow = ((int)(maxHeight) - 1)/2 - ((int)(temp.mHeight) - 1)/2;
    startCol = ((int)(maxWidth) - 1)/2 - ((int)(temp.mWidth) - 1)/2;
    if(startRow < 0)
        startRow = 0;
    if(startCol < 0)
        startCol = 0;

    for(int i = startRow, m = 0; i < maxHeight && m < temp.mHeight; i++, m++)
    {
        for(int j = startCol, n = 0; j < maxWidth && n < temp.mWidth; j++, n++)
        {
            if(channels == 1)
            {
                mpImage[i*maxWidth*channels + j*channels] = temp.mpImage[m*temp.mWidth*channels + n*channels];
            }
            else if(channels == 3)
            {
                mpImage[i*maxWidth*channels + j*channels] = temp.mpImage[m*temp.mWidth*channels + n*channels];
                mpImage[i*maxWidth*channels + j*channels + 1] = temp.mpImage[m*temp.mWidth*channels + n*channels + 1];
                mpImage[i*maxWidth*channels + j*channels + 2] = temp.mpImage[m*temp.mWidth*channels + n*channels + 2];
            }
            else
            {
                for(int k = 0; k < channels; k++)
                {
                    mpImage[i*maxWidth*channels + j*channels + k] = temp.mpImage[m*temp.mWidth*channels + n*channels + k];
                }
            }
        }
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief This initializes the Image
///
///  \param[in] width The width of the image
///  \param[in] height The height of the image
///  \param[in] channels Number of channels used (1, 3, or 4)
///  \param[in] rawImage The image data to be copied to the image, if
///                      the pointer is NULL (default), only memory allocation
///                      takes place an no copying.
///  \param[in] maxWidth Maximum width to store image as (will force scaling).
///  \param[in] maxHeight Maximum height to store image as (will force scaling).
///  \param[in] verticalFlip If true, image is flipped vertically when copied.
///
///  \return 1 on success, 0 on failure
///
////////////////////////////////////////////////////////////////////////////////////
int Image::Create(const unsigned short width,
                  const unsigned short height,
                  const unsigned char channels,
                  const unsigned char* rawImage,
                  const unsigned short maxWidth,
                  const unsigned short maxHeight,
                  const bool verticalFlip)
{
    return CreateImage(width, height, channels, rawImage, maxWidth, maxHeight, verticalFlip);
}



////////////////////////////////////////////////////////////////////////////////////
///
///  \brief The method compresses the image to buffer.
///
///  If necessary, this method will re-size the buffer to fit the compressed
///  image.
///
///  \param[out] buffer The pointer to the output buffer
///  \param[in] len Specify the lenght of the buffer
///  \param[out] clen Is the length of the compressed image in the output buffer.
///  \param[in] format The image format type to compress to. See jimageformats.h.
///  \param[in] args Pointer to additional compression arguments (like quality)
///                  which is converted the appropriate format for the compression
///                  routine based on data format. (by default is NULL and does
///                  nothing).
///
///  \return 0 on success, 1 on failure
///
////////////////////////////////////////////////////////////////////////////////////
int Image::Compress(unsigned char** buffer,
                    unsigned int* len,
                    unsigned int* clen,
                    const Format format,
                    void* args) const
{
    return Compress(mpImage, mWidth, mHeight, mChannels, buffer, len, clen, format, args);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief The method compresses the image to buffer.
///
///  If necessary, this method will re-size the buffer to fit the compressed
///  image.
///
///  \param[in] image Image data to compress.
///  \param[in] width Image width in pixels.
///  \param[in] height Image height in pixels.
///  \param[in] channels Number of color channels in image.
///  \param[out] buffer The pointer to the output buffer
///  \param[in] len Specify the lenght of the buffer
///  \param[out] clen Is the length of the compressed image in the output buffer.
///  \param[in] format The image format type to compress to. See jimageformats.h.
///  \param[in] args Pointer to additional compression arguments (like quality)
///                  which is converted the appropriate format for the compression
///                  routine based on data format. (by default is NULL and does
///                  nothing).
///
///  \return 0 on success, 1 on failure
///
////////////////////////////////////////////////////////////////////////////////////
int Image::Compress(unsigned char* image,
                    unsigned int width,
                    unsigned int height,
                    unsigned char channels,
                    unsigned char** buffer,
                    unsigned int* len,
                    unsigned int* clen,
                    const Format format,
                    void* args)
{
    int result = 0;

    switch (format)
    {
    case PNG:
        result = PNG::CompressImage(width, height, channels, image, buffer, len, clen);
        break;
    case JPEG:
        {
            int quality = -1;
            if(args)
            {
                quality = *((int *)(args));
            }
            result = JPEG::CompressImage(width, height, channels, image, buffer, len, clen, quality);
        }
        break;
    case MJPEG:
        {
            int quality = -1;
            if(args)
            {
                quality = *((int *)(args));
            }
            result = JPEG::CompressImage(width, height, channels, image, buffer, len, clen, quality);
        }
        break;
    default:
        result = 0;
        break;
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Takes a compressed image in the buffer, and uses one of the
///   supported image decompression algorithms to convert to RAW image data.
///
///   \param[in] compressed The buffer with data
///   \param[in] len The size of the buffer
///   \param[in] format The format of the data in the compressed buffer
///
///   \return 1 on success, 0 on failure
///
////////////////////////////////////////////////////////////////////////////////////
int Image::Decompress(const unsigned char* compressed,
                      const unsigned int len,
                      const Format format)
{
    int result = 0;

    switch (format)
    {
    case PNG:
        result = PNG::DecompressImage(compressed, len, &mpImage, &mWidth, &mHeight, &mChannels);
        break;
    case JPEG:
        result = JPEG::DecompressImage(compressed, len, &mpImage, &mWidth, &mHeight, &mChannels);
        break;
    case MJPEG:
        result = JPEG::DecompressImage(compressed, len, &mpImage, &mWidth, &mHeight, &mChannels);
        break;
    default:
        result = 0;
        break;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destroys the Image object data.
///
///  \return Always returns 1.
///
////////////////////////////////////////////////////////////////////////////////////
int Image::Destroy()
{
    if( mpImage )
    {
        delete[] mpImage;
        mpImage = NULL;
    }
    mDataSize = 0;
    mWidth = 0;
    mHeight = 0;
    mChannels = 0;
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief The function saves a frame to file. The format will be compressed based
///   on the extension of the file
///
///   \param[in] file The name of the file
///
///   \return 1 on success, 0 on failure
///
////////////////////////////////////////////////////////////////////////////////////
int Image::Save(const std::string& file) const
{
    int result = 0;
    FILE *fp = NULL;
    Format format = GetFormat(file);
    unsigned char *buffer = NULL;
    unsigned len = 0;
    unsigned int clen = 0;

    if( format == Invalid )
    {
        return 0;
    }
    int quality = 100;
    if( Compress(&buffer, &len, &clen, format, &quality ) )
    {
        fp = fopen( file.c_str() , "wb");
        if( fp && fwrite(buffer, clen, 1, fp) )
        {
            result = 1;
        }
    }

    if(fp)
    {
        fclose(fp);
        if(result == 0)
        {
            FileIO::DeleteFiles(file);
        }
    }
    if( buffer )
        delete[] buffer;

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief The funtion saves data to disk.
///
///   \param[in] name The name of the file (without extension).
///   \param[in] data Compressed data.
///   \param[in] size Size of the image.
///   \param[in] format Image format (for extension lookup).
///
///   \return 1 on success, 0 on failure
///
////////////////////////////////////////////////////////////////////////////////////
int Image::SaveCompressedImage(const std::string& name,
                               const unsigned char* data,
                               const unsigned int size,
                               const Format format)
{
    int result = 0;
    FILE *fp = NULL;
    
    std::string file = name;
    switch(format)
    {
    case JPEG:
        file += ".jpg";
        break;
    case PNG:
        file += ".png";
        break;
    case MJPEG:
        file += ".jpg";
        break;
    case BMP:
        file += ".bmp";
        break;
    case TIFF:
        file += ".tiff";
        break;
    case GIF:
        file += ".gif";
        break;
    case PPM:
        file += ".ppm";
        break;
    case PGM:
        file += ".pgm";
        break;
    default:
        file += ".jpg";
        break;
    };

    if( data && size > 0 )
    {
        fp = fopen( file.c_str() , "w+b");
        if( fp && fwrite(data, size, 1, fp) )
        {
            result = 1;
        }
    }

    if(fp)
    {
        fclose(fp);
        if(result == 0)
        {
            FileIO::DeleteFiles(file);
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief  Applies a tile repeatedly accross an image.
///
///   \param tile Tile image to stamp accross.
///
////////////////////////////////////////////////////////////////////////////////////
void Image::ApplyTile(const Image& tile)
{
    if(tile.mChannels == mChannels && mWidth > 0 && mHeight > 0 && tile.mWidth > 0 && tile.mHeight > 0)
    {
        double w, h;
        w = mWidth*1.0/tile.mWidth;
        h = mHeight*1.0/tile.mHeight;
        for(double r = 0; r < h; r++)
        {
            for(double c = 0; c < w; c++)
            {
                unsigned char* d = tile.mpImage;
                for(unsigned short int i = 0; i < tile.mHeight; i++)
                {
                    for(unsigned short int j = 0; j < tile.mWidth; j++)
                    {
                        unsigned short int xOffset = (unsigned short int)(c*tile.mWidth);
                        unsigned short int yOffset = (unsigned short int)(r*tile.mHeight);
        
                        for(unsigned char ch = 0; ch < mChannels; ch++)
                        {                
                            unsigned int oPosition = (yOffset + i)*mWidth*mChannels + (xOffset + j)*mChannels + ch; 
                            if(oPosition < mDataSize)
                            {
                                mpImage[oPosition] = d[i*tile.mWidth*tile.mChannels + j*tile.mChannels + ch];
                            }
                        }
                        
                    }
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief The function loads image to internal structure. The image will be
///   decompressed based on the extension of the file
///
///   \param file The name of the file
///
///   \return 1 on success, 0 on failure
///
////////////////////////////////////////////////////////////////////////////////////
int Image::Load(const std::string& file)
{
    Format format = GetFormat(file);
    int result = 0;
    unsigned int len = 0;
    unsigned char* buffer = NULL;

    //  Must handle video sequences differently.
    if( format == Invalid)
    {
        //  Add video code in future... maybe.
        return 0;
    }

    FILE *fp = fopen(file.c_str(), "r+b");
    if( fp )
    {
        //  Add up number of bytes in file so
        //  we can read it all in for decompression.
        while( !feof(fp))
        {
            fgetc(fp);
            len++;
        }
        --len;
        if( len > 0 )
        {
            buffer = new unsigned char[len + 1];
            rewind(fp);
            if( fread( buffer, len, 1, fp) )
            {
                result = Decompress(buffer, len, format);
                if(result > 0)
                {
                    mDataSize = mWidth*mHeight*mChannels;
                }
            }
        }
    }
    if(fp)
    {
        fclose(fp);
    }
    if( buffer )
    {
        delete[] buffer;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief The function returns the format based on the extension
///   of the file
///
///   \param[in] file The name of the file
///
///   \return The image format as devined in the Image class enumeration.
///
////////////////////////////////////////////////////////////////////////////////////
Image::Format Image::GetFormat(const std::string& file)
{
    if ( strstr(file.c_str(), ".png") || strstr(file.c_str(), ".PNG")  )
    {
        return PNG;
    }
    else if ( strstr(file.c_str(), ".jpeg") || strstr(file.c_str(),".JPEG") ||
              strstr(file.c_str(), ".JPG")  || strstr(file.c_str(), ".jpg") )
    {
        return JPEG;
    }

    return Invalid;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copies contents of image.
///
////////////////////////////////////////////////////////////////////////////////////
Image& Image::operator=(const Image& img)
{
    if(this != &img)
    {
        if (mHeight != img.mHeight || mWidth != img.mWidth || mChannels != img.mChannels)
        {
            Destroy();
        }
        if (img.mpImage != NULL)
        {
            if(mpImage == NULL)
            {
                Create(img.mWidth, img.mHeight, img.mChannels, img.mpImage);
            }
            else
            {
                memcpy(mpImage, img.mpImage, mDataSize);
            }
        }
    }
    return *this;
}


/*  End of File */
