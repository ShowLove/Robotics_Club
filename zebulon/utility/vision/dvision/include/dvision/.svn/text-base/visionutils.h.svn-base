/////////////////////////////////////////////////////////////////////////////
///
/// \file visualtarget.h
/// \brief Target for dropper bins
///
/// Author(s): Michael Scherer, David Adams<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
/// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
/// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
/// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
/// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
/// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
/// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
/// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
/// OF SUCH DAMAGE.
///
/////////////////////////////////////////////////////////////////////////////

#ifndef __ZEBULON_AI_UTILITY__VISION_UTILS__H
#define __ZEBULON_AI_UTILITY__VISION_UTILS__H

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <cxutils/cxutils.h>
#include <list>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "gvision/return.h"

namespace Zebulon
{
    namespace Vision
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class VisionUtils
        ///   \brief Class with useful computer vision functions
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class VisionUtils
        {
            private:
                VisionUtils();
                ~VisionUtils();
                static bool mWindowCreated;
            public:
                static void ExtractColor(const IplImage * img, IplImage * dest, const CvScalar sampleColor, const int threshold = 10, const int dilate = 0);
                static void ExtractColor(const IplImage * img,  IplImage * dest, const CvScalar sampleStart, const CvScalar sampleEnd, const int threshold = 10, const int dilate = 0);
                static IplImage * BackProject(const IplImage * img, const IplImage * sampleHistImg, const int threshold = 10, const int dilate = 0);
                static IplImage* Rotate90(const IplImage* img, const int angle = 90, const int orientation = CV_CLOCKWISE);
                static double GetAngle(const CvPoint* pt1, const CvPoint* pt2, const CvPoint* pt0 );
                static IplImage*  Crop(const IplImage* img, const CvBox2D cropArea);
                static std::vector<IplImage*> GetImageCells(const IplImage* img, const int numRows, const int numCols);
                static IplImage* StitchCells(const std::vector<IplImage*> cells, const int numRows, const int numCols, const bool drawBorders = false);
                static std::vector<CvBox2D> GetCells(const IplImage* image, const int numRows, const int numCols);
                static CxUtils::Point3D GetCellOffset(int index,
                                               const int numRows, const int numCols, const IplImage * cellImg);
                static void GetContourSimilarity(const IplImage* toMatch, const IplImage* tmplt, double &similarity);

                static void GetSimpleTemplateSimilarity(const IplImage* toMatch, const IplImage* tmplt, double &similarity, const bool useBestRatio = true);
                static void GetTemplateSimilarity(const IplImage* toMatch, const IplImage* tmplt, double &similarity);

                //static CvSeq* FindSquares(const IplImage* img, CvMemStorage* storage,int cannyThreshold = 50);
                static std::list<CvBox2D> FindSquares(const IplImage* img, IplImage *dest, const double minsize = .05, const double maxsize = .75, const int cannyThreshold = 50, const bool angle3d = false);
                //static double SquareTemplateMatch(CvSeq* squares, IplImage * temp, IplImage * source, IplImage * output)
                static void DrawSquare( IplImage* img, const CvSeq* squares, CvScalar color= cvScalar(255,255,255), int thickness =3);
                static void DrawSquare(IplImage* img, const CvBox2D square, const CvScalar color = cvScalar(255,255,255), const int thickness = 3, const bool crosslines = false);
                static std::list<CvBox2D> GetBlobBoxes(const IplImage * img, const double minsize = .001, const double maxsize = .95, const bool allowRotation = true);
                static double ExtractBiggestBlob(const IplImage* img, IplImage* dest, CvBox2D &biggestBox, const double minsize = .01, const double maxsize = .95);
                static void SquareDiff(const IplImage * img1,const IplImage * img2, IplImage * dest);
                static void SquareDiffS(const IplImage * img1, IplImage * dest,const CvScalar val);
                static unsigned char GetPixel(const IplImage*img, int row, int col, int channel );
                static bool SetPixel(const IplImage*img, int row, int col, int channel, uchar val);
                static IplImage* RGBToCustom(const IplImage * img);
                static void CvtBRG2HSL(const IplImage* src, IplImage* dest);
                static void CvtBRG2HSL(const CvScalar& src, CvScalar& dest);
                static double PointLineDistance(const CvScalar& p1, const CvScalar& p2, const CvScalar& p3,const int dimensions = 3);
                static double Dist(const CvScalar& p1, const CvScalar& p2, int dimensions = 3);
                static double Dist(const CvPoint& p1, const CvPoint& p2);
                static bool Calc3DSquareAngle(CvSeq*& polySquareApprox, float& angle, IplImage* dest = NULL);
                static void BoxPoints( CvBox2D box, CvPoint2D32f pt[4] );
                static CvScalar AveColorUnderLine(const IplImage* image, const CvPoint pt1, const CvPoint pt2 );
                static bool IsGrayScale(const IplImage* image);
                static bool CombineMasks(const IplImage * mask1, const IplImage * mask2, IplImage * dest, const double weight1 = 1, const double weight2 = 1);
                static CvHistogram *  CreateHSVHist(const IplImage * imgIn);
                static bool ConvertAndGetSingleColorChannel(const IplImage * imgIn, IplImage * imgOut, const std::string& cvColorSpaceNameAsString, const uchar channel);
                static bool Invert(const IplImage * imgIn, IplImage * imgOut);
                static double GetAngle(const CvBox2D& box);
                static Vision::Return VisionReturn(const CvBox2D& box);
                static bool ClearEdges(IplImage* imgOut,int borderWidthPixels = 20, CvScalar color = CV_RGB(0,0,0));
                static bool ContourLengthFilter(const IplImage* imgIn, IplImage* maskOut, int minLengthPixels = 100, int maxLengthPixels = 100000000000000, bool invert = false);
                static void MakeSquare(CvRect& rect);
                static CvRect ToCvRect(const CvBox2D& rect);
        };
    }
}

#endif // __ZEBULON_AI_UTILITY__VISION_UTILS__H
/* End of file */
