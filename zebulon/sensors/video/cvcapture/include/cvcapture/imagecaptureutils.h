
//#include <opencv/cv.h>
#include <cvcapture/cvimagecapture.h>

namespace Zebulon
{
    namespace Video
    {
        ///
        /// \brief Halves the size of a Bayer BG 8-bit image and puts it into an RGB
        ///         image.
        /// \param[in] source The source image, encoded Bayer BG 8-bit
        /// \param[in] dest The destination image, encoded RGB (8 bits per pixel, 3 channels)
        ///             with .5 the dimensions of the source image
        ///
        static void HalfBG2RGB(const CvImageCapture::Image* source, CvImageCapture::Image* dest, bool flipX=false, bool flipY=false)
        {
            //M.data + M.step*i + j*M.elemSize()
            unsigned short rd=0, cd=0;
            unsigned char R=0, G=0, B=0;

            for (unsigned short r=0; r<source->mHeight; r+=2)
            {
                rd = flipY? (source->mHeight-r-1)/2 : r/2;
                for (unsigned short c=0; c<source->mWidth; c+=2)
                {
                    cd = flipX? (source->mWidth-c-1)/2 : c/2;

                    R = source->Get(r,c,0);

                    if (r > 0 && r < source->mHeight-1 && c > 0 && c < source->mHeight-1)
                    {
                        G = (unsigned char)(((int)source->Get(r-1,c,0) + (int)source->Get(r+1,c,0)
                                           + (int)source->Get(r,c-1,0) + (int)source->Get(r,c+1,0)) / 4);
                        B = (unsigned char)(((int)source->Get(r-1,c-1,0) + (int)source->Get(r+1,c+1,0)
                                           + (int)source->Get(r+1,c-1,0) + (int)source->Get(r-1,c+1,0)) / 4);
                    }
                    else if (r > 0 && r < source->mHeight-1 && c > 0)
                    {
                        G = (unsigned char)(((int)source->Get(r-1,c,0) + (int)source->Get(r,c+1,0)
                                           + (int)source->Get(r,c-1,0)) / 3);
                        B = (unsigned char)(((int)source->Get(r-1,c-1,0) + (int)source->Get(r+1,c-1,0)) / 2);
                    }
                    else if (r > 0 && r < source->mHeight-1 && c < source->mHeight-1)
                    {
                        G = (unsigned char)(((int)source->Get(r+1,c,0) + (int)source->Get(r,c+1,0)
                                           + (int)source->Get(r-1,c,0)) / 3);
                        B = (unsigned char)(((int)source->Get(r-1,c+1,0) + (int)source->Get(r+1,c+1,0)) / 2);
                    }
                    else if (r > 0 && c > 0 && c < source->mHeight-1)
                    {
                        G = (unsigned char)(((int)source->Get(r-1,c,0)
                                           + (int)source->Get(r,c-1,0) + (int)source->Get(r+1,c,0)) / 3);
                        B = (unsigned char)(((int)source->Get(r-1,c-1,0) + (int)source->Get(r-1,c+1,0)) / 2);
                    }
                    else if (r < source->mHeight-1 && c > 0 && c < source->mHeight-1)
                    {
                        G = (unsigned char)(((int)source->Get(r+1,c,0)
                                           + (int)source->Get(r,c-1,0) + (int)source->Get(r,c+1,0)) / 3);
                        B = (unsigned char)(((int)source->Get(r+1,c+1,0) + (int)source->Get(r+1,c-1,0)) / 2);
                    }
                    else if (r == 0 && c == 0)
                    {
                        G = (unsigned char)(((int)source->Get(r+1,c,0) + (int)source->Get(r,c+1,0)) / 2);
                        B = (unsigned char)((int)source->Get(r+1,c+1,0));
                    }
                    else // This shouldn't happen
                    {
                        G=0;
                        B=0;
                    }

                    dest->Set(rd, cd, 0, R);
                    dest->Set(rd, cd, 1, G);
                    dest->Set(rd, cd, 2, B);
                }
            }
        }
    }
}
