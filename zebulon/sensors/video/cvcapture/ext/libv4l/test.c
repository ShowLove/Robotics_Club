/*
* Copyright (C) 2007-2008 Gilles Gigan (gilles.gigan@gmail.com)
* eResearch Centre, James Cook University (eresearch.jcu.edu.au)
*
* This program was developed as part of the ARCHER project
* (Australian Research Enabling Environment) funded by a
* Systemic Infrastructure Initiative (SII) grant and supported by the Australian
* Department of Innovation, Industry, Science and Research
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public  License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/


// This program tries capturing frames from the video device given in argument
// for CAPTURE_LENGTH seconds and prints the resulting fps
// Uncomment line 144 to write the captured frames to raw files


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "libv4l.h"
#include "palettes.h"

#define CAPTURE_LENGTH  	10 // in seconds

/* convert from 4:2:2 YUYV interlaced to RGB24 */
/* based on ccvt_yuyv_bgr32() from camstream */
#define SAT(c) \
        if (c & (~255)) { if (c < 0) c = 0; else c = 255; }
static void 
yuyv_to_rgb24 (int width, int height, unsigned char *src, unsigned char *dst)
{
   unsigned char *s;
   unsigned char *d;
   int l, c;
   int r, g, b, cr, cg, cb, y1, y2;
   
   l = height;
   s = src;
   d = dst;
   while (l--) {
      c = width >> 1;
      while (c--) {
         y1 = *s++;
         cb = ((*s - 128) * 454) >> 8;
         cg = (*s++ - 128) * 88;
         y2 = *s++;
         cr = ((*s - 128) * 359) >> 8;
         cg = (cg + (*s++ - 128) * 183) >> 8;

         r = y1 + cr;
         b = y1 + cb;
         g = y1 - cg;
         SAT(r);
         SAT(g);
         SAT(b);

	 *d++ = b;
	 *d++ = g;
	 *d++ = r;

         r = y2 + cr;
         b = y2 + cb;
         g = y2 - cg;
         SAT(r);
         SAT(g);
         SAT(b);

	 *d++ = b;
	 *d++ = g;
	 *d++ = r;
      }
   }
}


void write_frame(void *d, int size) {
	int outfile, len = 0;
	char filename[50];
	struct timeval tv;


	//Construct the filename
	gettimeofday(&tv, NULL);
	sprintf(filename,"raw_frame-%d-%d.raw", (int) tv.tv_sec, (int) tv.tv_usec);


	//open file
	if ((outfile = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644)) < 0) {
		printf( "FILE: can't open %s\n", filename);
		return;
	}

	//printf( "FILE: writing %d bytes to file\n", size);
	while((len+=write(outfile, (d+len), (size-len)))<size);

	close(outfile);
}

int main(int argc, char** argv) {
	struct capture_device *c;
	struct video_device *v;
	void *d;
	struct timeval start, now;
	int size, count=0, std=0, channel=0, width=0, height=0, cap_length = 0;
    IplImage *testimage;
	if(argc!=2 && argc!= 3 && argc!=5 && argc!=7) {
		printf("Usage: %s <video_device_file> [ capture_length [standard channel [ width height ] ] ]\n", argv[0]);
		printf("This program requires the path to the video device file to be tested.\n");
		printf("The optional second argument is the length (in seconds) of the capture (use 0 to capture a single frame), default is %d\n", CAPTURE_LENGTH);
		printf("The optional third and fourth arguments are a video standard and channel.\n");
		printf("Video standards: webcam:0 - PAL:1 - SECAM:2 - NTSC:3\n");
		printf("Arguments must be in the specified order !!!\n");
		return -1;
	}
    cvNamedWindow("normal", CV_WINDOW_AUTOSIZE);

	if(argc >= 3)
		cap_length = atoi(argv[2]);
	else
		cap_length = CAPTURE_LENGTH;

	printf("This program will capture frames from %s for %d seconds\n", argv[1], cap_length);

	if (argc>=5){
		std = atoi(argv[3]);
		channel = atoi(argv[4]);
		printf("Using standard %d, channel %d\n",std, channel);
	}

	if (argc==7) {
		width = atoi(argv[5]);
		height = atoi(argv[6]);
		printf("Requested resolution: %dx%d\n", width, height);
	}

	printf("Make sure your video source is connected, and press <Enter>, or Ctrl-C to abort now.");
	getchar();

	v = open_device(argv[1]);
	if(v==NULL){
		printf("Error opening device %s", argv[1]);
		return -1;
    }
	
    width=720;
    height=480;
    channel=0;
    std=3;
    c = init_capture_device(v, width, height ,channel, std,1);
    testimage = cvCreateImage(cvSize(width,height), 8, 3 );

	if(c==NULL) {
		printf("Error initialising device.\n");
		close_device(v);
		return -1;
	}

	if((*c->actions->set_cap_param)(v, NULL, 0)){
		free_capture_device(v);
		close_device(v);
		printf("Cant set capture parameters\n");
		return -1;
	}

	printf("Capturing from %s at %dx%d.\n", argv[1], c->width,c->height);
	printf("Image format %s, size: %d\n", libv4l_palettes[c->palette].name, c->imagesize);

	if((*c->actions->init_capture)(v)<0){
		free_capture_device(v);
		close_device(v);
		printf("Cant initialise capture ");
		return -1;
	}

	if((*c->actions->start_capture)(v)<0){
		(*c->actions->free_capture)(v);
		free_capture_device(v);
		close_device(v);
		printf("Cant start capture");
		return -1;
	}

	gettimeofday(&start, NULL);
	gettimeofday(&now, NULL);
	while(now.tv_sec<=start.tv_sec+cap_length) {

		//get frame from v4l2
		if((d = (*c->actions->dequeue_buffer)(v, &size)) != NULL) {
            
			//uncomment the following line to output raw captured frame
			//to a file
			//write_frame(d, size);
            //ConvYYUV422(d,testimage->imageData,width,height);
            yuyv_to_rgb24(testimage->width,testimage->height,d,testimage->imageData);
            cvShowImage("normal",testimage);
            cvWaitKey(10);
			count++;
			//Put frame
			(*c->actions->enqueue_buffer)(v);
		} else {
			printf("Cant get buffer ");
			break;
		}
		gettimeofday(&now, NULL);
	}
	printf("fps: %.1f\n", (count/((now.tv_sec - start.tv_sec) + ((float) (now.tv_usec - start.tv_usec)/1000000))));

	if((*c->actions->stop_capture)(v)<0)
		fprintf(stderr, "Error stopping capture\n");

	(*c->actions->free_capture)(v);
	free_capture_device(v);
	close_device(v);

	return 0;
}
