////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_images.cpp
///  \brief Simple test/example program for the image compression/decompression
///  methods of CxUtils.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 4 April 2009
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
#include <iostream>
#include <string.h>
#include "cxutils/images/image.h"
#include "cxutils/timer.h"

using namespace std;
using namespace CxUtils;

int main(int argc, char **argv) 
{
    Image image;
    
    if(image.Load("calculon640.jpg"))
    {
        /*
        for(unsigned int i = 0; i < image.mDataSize; i+= 3)
        {
            image.mpImage[i+1] = 0;
            image.mpImage[i+2] = 0;
        }
        */
        /*
        unsigned char* buffer = new unsigned char[6000];
        unsigned int len = 6000;
        unsigned int clen = 6000;
        double startTimeS = CxUtils::Timer::GetTimeSeconds();
        for(unsigned int i = 0; i < 100; i++)
        {
            image.Compress(&buffer, &len, &clen, Image::JPEG);
        }
        double finishTimeS = CxUtils::Timer::GetTimeSeconds();
        std::cout << "FPS: " << 100/(finishTimeS - startTimeS) << std::endl;
        */
        //unsigned char* buffer = NULL;
        //unsigned int bufferSize = 0;
        //unsigned int compressSize = 0;
        //image.Compress(&buffer, &bufferSize, &compressSize, Image::JPEG);
        //image.Decompress(buffer, compressSize, Image::JPEG);
        if(image.Save("calculon640_copy.jpg") && image.Save("calculon640_copy.png"))
        {
            cout << "Saved a copy of the image as a JPEG and PNG!\n";
        }

        Image tile;
        tile.CreateImage(image.mWidth, image.mHeight, image.mChannels, image.mpImage, 320, 240, true, true);
        image.ApplyTile(tile);
        image.Save("tile.jpg");
    }

    return 0;
}

/*  End of File */
