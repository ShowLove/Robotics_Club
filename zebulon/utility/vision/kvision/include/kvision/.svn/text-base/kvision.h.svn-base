#ifndef __ZEBULON_AI_UTILITY_KVISION_
#define __ZEBULON_AI_UTILITY_KVISION_

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "gvision/target.h"
#include "gvision/return.h"
#include "gvision/segment.h"
#include "gvision/threshhold.h"
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "opencv/cxcore.h"

namespace Zebulon
{
    namespace Vision
    {
        class KVision
        {
        public:
            KVision();
            ~KVision();

            bool Kmeans(cv::Mat input, int k, bool colorOnlyFlag, cv::Mat &clusterLabels, cv::Mat &clusterCenters);
            cv::Mat ConditionMat(cv::Mat input);
            cv::Mat ReverseMat(cv::Mat input, cv::Mat clusterLabels, cv::Mat clusterCenters);
            cv::Mat ReverseMat(cv::Mat input);
            void Setup(int k, int rows, int cols, int desiredX, int desiredY, int desiredRed, int desiredGreen, int desiredBlue, int threshold, int maxIterations, bool removeBWFlag);
            cv::Mat RemovePoints(cv::Mat input, bool colorOnlyFlag, int posI, int posJ, int r, int g, int b, double threshold);
            cv::Mat KMeansSeg(cv::Mat input, cv::Mat &labels, cv::Mat &clusters, bool colorFlag);
            cv::Mat ColorCompare(cv::Mat input, int b, int g, int r);
            cv::Mat ThreeChannelCompare(cv::Mat input, int b, int g, int r);
            cv::Vec3f AverageColor(cv::Mat input);
            cv::Vec3f StandDev(cv::Mat input);
            cv::Mat StdDevThreshold(cv::Mat input, cv::Vec3f avgBGR, cv::Vec3f sdBGR);
            cv::Mat BinaryInvert(cv::Mat input);

            bool PeakValleySegmentation(cv::Mat input);

            cv::Mat FindSegColor(cv::Mat rawImg, Segment *seg);
            void FindAvgAndSD(std::vector <cv::Mat> rawImgs, std::vector <cv::Mat> grayImgs, std::vector <int> segNums, std::vector <cv::Vec3f> &avg, std::vector <cv::Vec3f> &sd);

        protected:

            void AverageFilter(cv::Mat &input);

            cv::Mat mCondMat;
            cv::Mat mClusterCenters;
            cv::Mat mClusterLabels;

            double ColorDistance(float r1, float g1, float b1, float r2, float g2, float b2);
            double PosDistance(float x1, float y1, float x2, float y2, float r1, float g1, float b1, float r2, float g2, float b2);
            cv::Vec3f ManDistance(float r1, float g1, float b1, float r2, float g2, float b2);
            Threshhold mSDThreshold;
            Segment *mSDSegment;
            int mRows;
            int mCols;
            int mDesiredX;
            int mDesiredY;
            int mDesiredRed;
            int mDesiredGreen;
            int mDesiredBlue;
            int mThreshold;
            int mIterations;
            bool mBWFlag;
            int mK;
        };
    }
}

#endif
