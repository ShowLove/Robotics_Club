////////////////////////////////////////////////////////////////////////////////////
///
/// \file calibration_program.h
/// \brief Finds camera calibration characteristics and saves them to re-loadable
///        XML files.
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cxutils/cxutils.h>
#include <cvcapture/cvimagecapture.h>
#include <homography/homography.h>
#include <stdio.h>
#include <stdlib.h>
using namespace Zebulon;
using namespace Vision;

// c a l i b.c p p
// Calling convention:
// calib board_w board_h number_of_views
//
// Hit ‘p’ to pause/unpause, ESC to quit
//
int n_boards = 50; //Will be set by input list
const int board_dt = 5; //Wait 20 frames per chessboard view
int board_w;
int board_h;
Homography gHomo;
Zebulon::Video::CvImageCapture gCamera;
int gBoardWidth = 5;
int gBoardHeight = 8;
std::string gDistortionSaveName = "settings/vision/distortion.xml";
std::string gIntrinsicSaveName = "settings/vision/intrinsics.xml";
std::string gCameraDir = "/dev/fw0";

bool ConnectToCamera(const std::string& camera)
{
    if(gCamera.Start(camera, 800, 600, true, false))
        return true;
    return false;
}

IplImage * CaptureFrame()
{
    //Zebulon::Video::CvConvertToIplImage();
    Zebulon::Video::CvImageCapture::Image img;
    gCamera.GetFrame(&img);
    IplImage * image = NULL;
    Zebulon::Video::CvConvertToIplImage(img,&image);
    return image;
}

int main(int argc, char* argv[])
{

    if(argc > 1)
    {
        gCameraDir = argv[1];
    }
    ConnectToCamera(gCameraDir);
    board_w  = gBoardWidth;
    board_h  = gBoardHeight;
    //n_boards = atoi(argv[3]);
    int board_n  = board_w * board_h;
    CvSize board_sz = cvSize( board_w, board_h );
    cvNamedWindow( "Calibration" );
    //ALLOCATE STORAGE
    CvMat* image_points      = cvCreateMat(n_boards*board_n,2,CV_32FC1);
    CvMat* object_points     = cvCreateMat(n_boards*board_n,3,CV_32FC1);
    CvMat* point_counts      = cvCreateMat(n_boards,1,CV_32SC1);
    CvMat* intrinsic_matrix  = cvCreateMat(3,3,CV_32FC1);
    CvMat* distortion_coeffs = cvCreateMat(5,1,CV_32FC1);
    CvPoint2D32f* corners = new CvPoint2D32f[ board_n ];
    int corner_count;
    int successes = 0;
    int step, frame = 0;
    IplImage *image = CaptureFrame();
    IplImage *gray_image = cvCreateImage(cvGetSize(image),8,1);//subpixel
    // CAPTURE CORNER VIEWS LOOP UNTIL WE’VE GOT n_boards
    // SUCCESSFUL CAPTURES (ALL CORNERS ON THE BOARD ARE FOUND)
    //
    while(successes < n_boards) {
        //Skip every board_dt frames to allow user to move chessboard
        if(frame++ % board_dt == 0) {
            //Find chessboard corners:
            int found = cvFindChessboardCorners(
                        image, board_sz, corners, &corner_count,
                        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS
                        );
            //Get Subpixel accuracy on those corners
            cvCvtColor(image, gray_image, CV_BGR2GRAY);
            cvFindCornerSubPix(gray_image, corners, corner_count,
                               cvSize(11,11),cvSize(-1,-1), cvTermCriteria(
                                   CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
            //Draw it
            cvDrawChessboardCorners(image, board_sz, corners,corner_count, found);
            cvShowImage("Calibration",image);
            // If we got a good board, add it to our data
            if( corner_count == board_n ) {
                step = successes*board_n;
                for( int i=step, j=0; j<board_n; ++i,++j ) {
                    CV_MAT_ELEM(*image_points, float,i,0) = corners[j].x;
                    CV_MAT_ELEM(*image_points, float,i,1) = corners[j].y;
                    CV_MAT_ELEM(*object_points,float,i,0) = j/board_w;
                    CV_MAT_ELEM(*object_points,float,i,1) = j%board_w;
                    CV_MAT_ELEM(*object_points,float,i,2) = 0.0f;
                }
                CV_MAT_ELEM(*point_counts, int,successes,0) = board_n;
                successes++;
            }
        } //end skip board_dt between chessboard capture
        //Handle pause/unpause and ESC
        int c = cvWaitKey(15);
        if(c == 'p'){
            c = 0;
            while(c != 'p' && c != 27){
                c = cvWaitKey(250);
            }
        }
        if(c == 27)
            return 0;
        cvReleaseImage(&image);
        image = CaptureFrame(); //Get next image
    } //END COLLECTION WHILE LOOP.
    //ALLOCATE MATRICES ACCORDING TO HOW MANY CHESSBOARDS FOUND
    CvMat* object_points2  = cvCreateMat(successes*board_n,3,CV_32FC1);
    CvMat* image_points2   = cvCreateMat(successes*board_n,2,CV_32FC1);
    CvMat* point_counts2   = cvCreateMat(successes,1,CV_32SC1);
    //TRANSFER THE POINTS INTO THE CORRECT SIZE MATRICES
    //Below, we write out the details in the next two loops. We could
    //instead have written:
    //image_points->rows = object_points->rows  = \
    //successes*board_n; point_counts->rows = successes;
    //
    for(int i = 0; i<successes*board_n; ++i) {
        CV_MAT_ELEM( *image_points2, float, i, 0) =
                CV_MAT_ELEM( *image_points, float, i, 0);
        CV_MAT_ELEM( *image_points2, float,i,1) =
                CV_MAT_ELEM( *image_points, float, i, 1);
        CV_MAT_ELEM(*object_points2, float, i, 0) =
                CV_MAT_ELEM( *object_points, float, i, 0) ;
        CV_MAT_ELEM( *object_points2, float, i, 1) =
                CV_MAT_ELEM( *object_points, float, i, 1) ;
        CV_MAT_ELEM( *object_points2, float, i, 2) =
                CV_MAT_ELEM( *object_points, float, i, 2) ;
    }
    for(int i=0; i<successes; ++i){ //These are all the same number
        CV_MAT_ELEM( *point_counts2, int, i, 0) =
                CV_MAT_ELEM( *point_counts, int, i, 0);
    }
    cvReleaseMat(&object_points);
    cvReleaseMat(&image_points);
    cvReleaseMat(&point_counts);
    // At this point we have all of the chessboard corners we need.
    // Initialize the intrinsic matrix such that the two focal
    // lengths have a ratio of 1.0
    //
    CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0f;
    CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1 ) = 1.0f;
    //CALIBRATE THE CAMERA!
    cvCalibrateCamera2(
                object_points2, image_points2,
                point_counts2,  cvGetSize( image ),
                intrinsic_matrix, distortion_coeffs,
                NULL, NULL,0  //CV_CALIB_FIX_ASPECT_RATIO
                );
    // SAVE THE INTRINSICS AND DISTORTIONS
    cvSave(gIntrinsicSaveName.c_str(),intrinsic_matrix);
    cvSave(gDistortionSaveName.c_str(),distortion_coeffs);// EXAMPLE OF LOADING THESE MATRICES BACK IN:
    CvMat *intrinsic = (CvMat*)cvLoad(gIntrinsicSaveName.c_str());
    CvMat *distortion = (CvMat*)cvLoad(gDistortionSaveName.c_str());
    // Build the undistort map that we will use for all
    // subsequent frames.
    //
    IplImage* mapx = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );
    IplImage* mapy = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );
    cvInitUndistortMap(
                intrinsic,
                distortion,
                mapx,
                mapy
                );
    // Just run the camera to the screen, now showing the raw and
    // the undistorted image.
    //
    cvNamedWindow("Undistort" );
    while(image) {
        IplImage *t = cvCloneImage(image);
        cvShowImage("Calibration", image ); // Show raw image
        cvRemap( t, image, mapx, mapy );     // Undistort image
        cvReleaseImage(&t);
        cvShowImage("Undistort", image);     // Show corrected image
        //Handle pause/unpause and ESC
        int c = cvWaitKey(15);
        if(c == 'p') {
            c = 0;
            while(c != 'p' && c != 27) {
                c = cvWaitKey(250);
            }
        }
        if(c == 27)
            break;
        cvReleaseImage(&image);
        image = CaptureFrame();
    }
    return 0;
}
