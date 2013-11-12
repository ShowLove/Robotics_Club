/////////////////////////////////////////////////////////////////////////////
///
/// \file main.cpp
/// \brief Example for the dc filter stuff, use with the following in octave
///         for a pretty graph:
///     data = load("output"); n = size(data,2); subplot(2,1,1);
///     plot(1:n, data(1,:), 1:n, data(2,:)); subplot(2,1,2);
///     plot(1:n, data(3,:), 1:n, data(4,:));
///
/// Author(s): Micheal Scherer<br>
/// Created: 5/18/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org <br>
/// Web: http://robotics.ucf.edu <br>
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
/////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <filter/dcfilter.h>
#include <cxutils/math/cxmath.h>

using namespace Zebulon;
using namespace Utility;

const double DT = .1;
const double RC = 1;

int numvals = 90;

int main(int argc, char **argv)
{
    FILE* file = fopen("output", "w");
    printf("Running Low Pass Filter for DT = %lf, RC = %lf\n", DT, RC);
    
    LowPassFilter* filter = new LowPassFilter(DT, RC);
    
    double val;
    double values[numvals];
    for (int i=0; i<numvals; i++)
    {
        values[i] = cos(i * CxUtils::CX_TWO_PI/10) + .5;
            //(rand() % 100 - 50) / 50.0;
        printf("%lf ", values[i]);
        fprintf(file, "%lf ", values[i]);
    }
    
    fprintf(file, "\n");
    printf("\n");
    
//    double filters[15];
    for (int i=0; i<numvals; i++)
    {
        filter->Add(values[i]);
        filter->Output(val);
        printf("%lf ", val);
        fprintf(file, "%lf ", val);
    }
    delete filter;
    printf("\n\n");
    fprintf(file,"\n");
    
    HighPassFilter* hfilter = new HighPassFilter(DT, RC);
    for (int i=0; i<numvals; i++)
    {
        //values[i] = cos(i * CxUtils::CX_TWO_PI/10) + .5;
            //(rand() % 100 - 50) / 50.0;
        printf("%lf ", values[i]);
        fprintf(file, "%lf ", values[i]);
    }
    
    fprintf(file, "\n");
    printf("\n");
    
//    double filters[15];
    for (int i=0; i<numvals; i++)
    {
        hfilter->Add(values[i]);
        hfilter->Output(val);
        printf("%lf ", val);
        fprintf(file, "%lf ", val);
    }
    printf("\n");
    
    fclose(file);
    
    return 0;
}
