#include "kvision/kvision.h"

using namespace Zebulon;
using namespace Vision;

KVision::KVision()
{
    mSDSegment = new Segment();
}

KVision::~KVision()
{

}

bool KVision::Kmeans(cv::Mat input, int k, bool colorOnlyFlag, cv::Mat &clusterLabels, cv::Mat &clusterCenters)
{
    cv::TermCriteria tempCriteria;
    tempCriteria.epsilon = 5;
    cv::Vec3f tempVec;

    if(input.rows < k)
    {
        std::cout << "KMeans: Input rows is less than k" << std::endl;
        return false;
    }

    else if(colorOnlyFlag == true)
    {
        cv::Mat noPosMat(input.rows, 3, input.type());

        for(int j = 0; j < input.rows; j++)
        {
            for(int i = 2; i < 5; i++)
            {
                tempVec[i-2] = input.at<float>(j,i);
            }
            noPosMat.at<float>(j, 0) = tempVec[0];
            noPosMat.at<float>(j, 1) = tempVec[1];
            noPosMat.at<float>(j, 2) = tempVec[2];
        }

        cv::kmeans(noPosMat, k, clusterLabels, tempCriteria, 10, cv::KMEANS_RANDOM_CENTERS, clusterCenters);
    }

    else
    {
        cv::kmeans(input, k, clusterLabels, tempCriteria, 10, cv::KMEANS_RANDOM_CENTERS, clusterCenters);
    }

    std::cout << "Kmeans Clusters: " << clusterCenters << std::endl;

    return true;
}

cv::Mat KVision::ConditionMat(cv::Mat input)
{
    int tempRows = mRows*mCols;
    int tempWidth = 5;
    int rowCount = 0;
    cv::Mat convertInput, condMat;

    condMat.create(tempRows, tempWidth, CV_32FC1);
    input.convertTo(convertInput, CV_32FC3);

    for(int i = 0; i < mCols; i++){
        for(int j = 0; j < mRows; j++){

            condMat.at<float>(rowCount, 0) = j;
            condMat.at<float>(rowCount, 1) = i;

            cv::Vec3f tempVec = convertInput.at<cv::Vec3f>(j, i);
            condMat.at<float>(rowCount, 2) = tempVec[0];
            condMat.at<float>(rowCount, 3) = tempVec[1];
            condMat.at<float>(rowCount, 4) = tempVec[2];

            rowCount++;
        }
    }

    return condMat;
}

void KVision::Setup(int k, int rows, int cols, int desiredX, int desiredY, int desiredRed, int desiredGreen, int desiredBlue, int threshold, int maxIterations, bool removeBWFlag)
{
    mRows = rows;
    mCols = cols;
    mDesiredX = desiredX;
    mDesiredY = desiredY;
    mDesiredRed = desiredRed;
    mDesiredGreen = desiredGreen;
    mDesiredBlue = desiredBlue;
    mThreshold = threshold;
    mIterations = maxIterations;
    mBWFlag =  removeBWFlag;
    mK = k;
}

cv::Mat KVision::ReverseMat(cv::Mat input, cv::Mat clusterLabels, cv::Mat clusterCenters)
{
    cv::Mat output(mRows, mCols, CV_8UC3);
    float posI, posJ;

    cv::Vec3f tempVec;
    tempVec[0] = 0;
    tempVec[1] = 0;
    tempVec[2] = 0;

    for(int b = 0; b < mRows; b++)
    {
        for(int c = 0; c < mCols; c++)
        {
            output.at<cv::Vec3b>(b, c) = tempVec;
        }
    }

    std::cout << clusterCenters << std::endl;

    for(int a = 0; a < input.rows; a++){

        posJ = input.at<float>(a, 0);
        posI = input.at<float>(a, 1);

        //std::cout << "Labels" << clusterLabels << std::endl;

        cv::Vec3f tempVec;
        tempVec[0] = clusterCenters.at<float>(clusterLabels.at<int>(a, 0), 0);
        tempVec[1] = clusterCenters.at<float>(clusterLabels.at<int>(a, 0), 1);
        tempVec[2] = clusterCenters.at<float>(clusterLabels.at<int>(a, 0), 2);

//        tempVec[0] = input.at<float>(a, 2);
//        tempVec[1] = input.at<float>(a, 3);
//        tempVec[2] = input.at<float>(a, 4);


        output.at<cv::Vec3b>(posJ, posI) = tempVec;
    }

    return output;
}

cv::Mat KVision::ReverseMat(cv::Mat input)
{
    cv::Mat output(mRows, mCols, CV_8UC3);
    float posI, posJ;

    cv::Vec3f tempVec;
    tempVec[0] = 0;
    tempVec[1] = 0;
    tempVec[2] = 0;

    for(int b = 0; b < mRows; b++)
    {
        for(int c = 0; c < mCols; c++)
        {
            output.at<cv::Vec3b>(b, c) = tempVec;
        }
    }

    for(int a = 0; a < input.rows; a++){

        posJ = input.at<float>(a, 0);
        posI = input.at<float>(a, 1);

        cv::Vec3f tempVec;
        tempVec[0] = input.at<float>(a, 2);
        tempVec[1] = input.at<float>(a, 3);
        tempVec[2] = input.at<float>(a, 4);

        output.at<cv::Vec3b>(posJ, posI) = tempVec;
    }

    return output;
}

cv::Mat KVision::RemovePoints(cv::Mat input, bool colorOnlyFlag, int posI, int posJ, int r, int g, int b, double threshold)
{
    double dist;
    cv::Mat output;
    cv::Mat tempInput(1, 5, input.type());

    for(int j = 0; j < input.rows; j++)
    {
        for(int i = 0; i < input.cols; i++)
        {
            tempInput.at<float>(0, i) = input.at<float>(j, i);
        }

        if(colorOnlyFlag == true)
        {
            dist = ColorDistance(tempInput.at<float>(0, 4), tempInput.at<float>(0, 3), tempInput.at<float>(0, 2), r, g, b);
        }
        else
        {
            dist = PosDistance(tempInput.at<float>(0, 0), tempInput.at<float>(0, 1), posI, posJ, tempInput.at<float>(0, 4), tempInput.at<float>(0, 3), tempInput.at<float>(0, 2), r, g, b);
        }

        if(dist > threshold)
        {
            output.push_back(tempInput);
        }
    }

    //std::cout << output.rows << std::endl;

    return output;
}

bool KVision::PeakValleySegmentation(cv::Mat input)
{
    std::vector<cv::Mat> rgbPlanes;
    cv::split( input, rgbPlanes );

    int histSize = 255;

    float range[] = { 0, 255 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    cv::Mat r_hist, g_hist, b_hist;

    /// Compute the histograms:
    cv::calcHist( &rgbPlanes[0], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    cv::calcHist( &rgbPlanes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    cv::calcHist( &rgbPlanes[2], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );

    //std::cout << "r_hist: " << r_hist << std::endl;

    // Gaussian the output of the histograms, to smooth
    std::cout << "r_hist: " << r_hist << std::endl;

    cv::Mat smoothHistRed, smoothHistGreen, smoothHistBlue;
    cv::GaussianBlur(r_hist, smoothHistRed, cv::Size(50, 1), 5, 5);
    cv::GaussianBlur(g_hist, smoothHistGreen, cv::Size(50, 1), 5, 5);
    cv::GaussianBlur(b_hist, smoothHistBlue, cv::Size(50, 1), 5, 5);

    std::cout << "smoothHistRed: " << smoothHistRed << std::endl;

    // Draw the histograms for R, G and B
     int hist_w = 400; int hist_h = 400;
     int bin_w = cvRound( (double) hist_w/histSize );

     cv::Mat histImage( hist_w, hist_h, CV_8UC3, cv::Scalar( 0,0,0) );


     /// Normalize the result to [ 0, histImage.rows ]
     cv::normalize(smoothHistRed, smoothHistRed, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
     cv::normalize(smoothHistGreen, smoothHistGreen, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
     cv::normalize(smoothHistBlue, smoothHistBlue, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );

     /// Draw for each channel
     for(int i = 1; i < histSize; i++)
       {
         cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(smoothHistRed.at<float>(i-1)) ) ,
                          cv::Point( bin_w*(i), hist_h - cvRound(smoothHistRed.at<float>(i)) ),
                          cv::Scalar( 0, 0, 255), 2, 8, 0  );
         cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(smoothHistGreen.at<float>(i-1)) ) ,
                          cv::Point( bin_w*(i), hist_h - cvRound(smoothHistGreen.at<float>(i)) ),
                          cv::Scalar( 0, 255, 0), 2, 8, 0  );
         cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(smoothHistBlue.at<float>(i-1)) ) ,
                          cv::Point( bin_w*(i), hist_h - cvRound(smoothHistBlue.at<float>(i)) ),
                          cv::Scalar( 255, 0, 0), 2, 8, 0  );
        }

     /// Display
     cv::namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
     cv::imshow("calcHist Demo", histImage );

     cv::waitKey(0);

    return true;
}

void KVision::AverageFilter(cv::Mat &input)
{
    int avgWindow = 5; // must be odd

    cv::Mat tempOutput;
    input.copyTo(tempOutput);

    for(int i = 0; i < input.rows; i++)
    {


        /*int tempAvgWindow = 0;
        for(int n = 0; n < avgWindow; n++)
        {
            if(i-n < 0)
        }*/
    }


}

double KVision::ColorDistance(float r1, float g1, float b1, float r2, float g2, float b2)
{
    return sqrt(pow((r1 - r2), 2) + pow((g1 - g2), 2) + pow((b1 - b2), 2));
}

double KVision::PosDistance(float x1, float y1, float x2, float y2, float r1, float g1, float b1, float r2, float g2, float b2)
{
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2) + pow((r1 - r2), 2) + pow((g1 - g2), 2) + pow((b1 - b2), 2));
}

cv::Vec3f KVision::ManDistance(float r1, float g1, float b1, float r2, float g2, float b2)
{
    cv::Vec3f output;

    output[0] = fabs(b1-b2);
    output[1] = fabs(g1-g2);
    output[2] = fabs(r1-r2);

    return output;
}

cv::Mat KVision::KMeansSeg(cv::Mat input, cv::Mat &labels, cv::Mat &clusters, bool colorFlag)
{

    cv::Mat output, worstCenter(1,3, input.type());
    cv::Mat output2(input.rows, input.cols, input.type());
    bool foundFlag = false;
    int currIter = 0;
    float distFrom;
    input.copyTo(output);
    double worstDist = 0;
    int threshold = 75;

    if(mBWFlag == true)
    {
        output = RemovePoints(output, true, 0, 0, 0, 0, 0, 50);
        output = RemovePoints(output, true, 0, 0, 255, 255, 255, 100);
    }

    //output = RemovePoints(output, true, 0, 0, 0, 0, 255, 150);
    //output = RemovePoints(output, true, 0, 0, 0, 255, 0, 150);

    while(foundFlag == false && currIter < mIterations)
    {
        double sizeBefore = 0, sizeAfter = 0, sizeDiff;

        Kmeans(output, mK, true, labels, clusters);

        for(int i = 0; i < clusters.rows; i++)
        {

            if(colorFlag == true)
            {
                distFrom = ColorDistance(clusters.at<float>(i, 2), clusters.at<float>(i, 1), clusters.at<float>(i, 0), mDesiredRed, mDesiredGreen, mDesiredBlue);
                std::cout << "Color Distance: " << distFrom << std::endl;
            }
            else
            {
                distFrom = PosDistance(mDesiredX, mDesiredY, output.at<float>(i, 1), output.at<float>(i, 0), mDesiredRed, mDesiredGreen, mDesiredBlue, clusters.at<float>(i, 2),clusters.at<float>(i, 1), clusters.at<float>(i, 0));
                std::cout << "Position Distance: " << distFrom << std::endl;
            }

            if(distFrom < 100)
            {
                foundFlag = true;
                std::cout << "Found flag is set to true." << std::endl;
                std::cout << "Closest Color: " << clusters.row(i) << std::endl;
                break;
            }
            else
            {
                if(worstDist < distFrom)
                {
                    worstDist = distFrom;
                    worstCenter = clusters.row(i)*1.0;
                }
            }


        }

        sizeBefore = output.rows;

        output = RemovePoints(output, true, 0, 0, worstCenter.at<float>(0, 2), worstCenter.at<float>(0, 1), worstCenter.at<float>(0, 0), threshold);

        sizeAfter = output.rows;

        sizeDiff = sizeAfter - sizeBefore;

//        if(sizeDiff == 0 || sizeDiff < -30)
//        {
//            threshold += 5;
//        }

        std::cout << "Size Difference: " << sizeDiff << std::endl;
        std::cout << "Worst Distance: " << worstDist << std::endl;
        std::cout << "Worst Center: " << worstCenter << std::endl;
        std::cout << "Iterations passed: " << currIter+1 << std::endl;

            output2 = ReverseMat(output, labels, clusters);

            cv::imshow("win1", output2);
            cv::waitKey(50);

        currIter++;
    }

    return output;
}

cv::Mat KVision::ColorCompare(cv::Mat input, int b, int g, int r)
{
    cv::Mat output(input.rows, input.cols, CV_32FC1), convertInput;
    cv::Vec3f tempVec;
    double biggestDist = 0, colorDist;
    input.convertTo(convertInput, CV_32FC3);

    for(int j = 0; j < input.rows; j++)
    {
        for(int i = 0; i < input.cols; i++)
        {
             tempVec = convertInput.at<cv::Vec3f>(j, i);
             colorDist = ColorDistance(tempVec[2], tempVec[1], tempVec[0], r, g, b);

             output.at<float>(j, i) = colorDist;

             if(biggestDist < colorDist)
                 biggestDist = colorDist;
        }
    }

    output = output/biggestDist;

    return output;
}

cv::Mat KVision::ThreeChannelCompare(cv::Mat input, int b, int g, int r)
{
    cv::Mat output(input.rows, input.cols, CV_8UC3), convertInput;
    cv::Vec3f tempVec, colorDist;
    cv::Vec3b tempVec2;
    input.convertTo(convertInput, CV_32FC3);

    for(int j = 0; j < input.rows; j++)
    {
        for(int i = 0; i < input.cols; i++)
        {
             tempVec = convertInput.at<cv::Vec3f>(j, i);
             colorDist = ManDistance(tempVec[2], tempVec[1], tempVec[0], r, g, b);

             tempVec2[0] = round(colorDist[0]);
             tempVec2[1] = round(colorDist[1]);
             tempVec2[2] = round(colorDist[2]);

             //std::cout << colorDist[0] << "," << colorDist[1] << "," << colorDist[2] << std::endl;

             output.at<cv::Vec3b>(j, i) = tempVec2;
        }
    }

    return output;
}

cv::Vec3f KVision::AverageColor(cv::Mat input)
{
    double redSum = 0.0, greenSum = 0.0, blueSum = 0.0;
    cv::Vec3f colorAverage;

    for(int j = 0; j < input.rows; j++)
    {
        for(int i = 0; i < input.cols; i++)
        {
            cv::Vec3b tempVec = input.at<cv::Vec3b>(j,i);

            redSum += tempVec[2];
            greenSum += tempVec[1];
            blueSum += tempVec[0];
        }
    }

    colorAverage[2] = redSum/input.total();
    colorAverage[1] = greenSum/input.total();
    colorAverage[0] = blueSum/input.total();

    return colorAverage;
}

cv::Vec3f KVision::StandDev(cv::Mat input)
{
    cv::Vec3f deviation;
    cv::Vec3f averageColor;
    double redSum = 0.0, greenSum = 0.0, blueSum = 0.0;

    averageColor = AverageColor(input);

    for(int j = 0; j < input.rows; j++)
    {
        for(int i = 0; i < input.cols; i++)
        {
            cv::Vec3f tempVec = input.at<cv::Vec3b>(j, i);

            redSum += pow((tempVec[2] - averageColor[2]), 2);
            greenSum += pow((tempVec[1] - averageColor[1]), 2);
            blueSum += pow((tempVec[0] - averageColor[0]), 2);
        }
    }

    deviation[2] = sqrt((redSum/input.total()));
    deviation[1] = sqrt((greenSum/input.total()));
    deviation[0] = sqrt((blueSum/input.total()));

    return deviation;
}

cv::Mat KVision::StdDevThreshold(cv::Mat rawImg, cv::Vec3f avgBGR, cv::Vec3f sdBGR)
{
//    Segment *seg = NULL;
//    seg = new Segment();

    double j, i, angle;
    int area, valid;
    std::vector<pixelData> data;
    cv::Vec3f avgColor;

    cv::Mat output(rawImg.rows, rawImg.cols, rawImg.type()), output2;

    mSDThreshold.Setup(avgBGR[2], avgBGR[1], avgBGR[0], sdBGR[2], sdBGR[1], sdBGR[0], 0);
    mSDThreshold.Process(rawImg, output);

    output = BinaryInvert(output);

    //IplImage iplInput = output;

    mSDSegment->Setup(avgBGR[2], avgBGR[1], avgBGR[0], 50, 0, 3);
    //seg->Process(&iplInput, segImg);
    mSDSegment->Process(rawImg, output, output2);

    mSDSegment->GetMaxPosition(j, i, area, valid, angle, data, avgColor, 0);
//    std::cout << "Position J (Row): " << j << std::endl;
//    std::cout << "Position I (Column): " << i << std::endl;
//    std::cout << "Area: " << area << std::endl;
//    std::cout << "Angle: " << angle << std::endl;
//    std::cout << "Average Colors (Red): " << avgColor[2] << std::endl;
//    std::cout << "Average Colors (Green): " << avgColor[1] << std::endl;
//    std::cout << "Average Colors (Blue): " << avgColor[0] << std::endl;
//    std::cout << std::endl;

//    for(int a = 0; a < data.size(); a++)
//    {
//        cv::Vec3b tempVec;

//        tempVec[0] = data[a].blueColor;
//        tempVec[1] = data[a].greenColor;
//        tempVec[2] = data[a].redColor;

//        output2.at<cv::Vec3b>(data[a].posJ, data[a].posI) = tempVec;
//    }

    //cv::Mat output3 = FindSegColor(rawImg, seg);

    return output2;
}


cv::Mat KVision::BinaryInvert(cv::Mat input)
{
    cv::Mat output(input.rows, input.cols, input.type());

    for(int j = 0; j < input.rows; j++)
    {
        for(int i = 0; i < input.cols; i++)
        {
            cv::Vec3b tempVec = input.at<cv::Vec3b>(j,i);

            if(tempVec[0] == 255)
            {
                tempVec[0] = 0;
                tempVec[1] = 0;
                tempVec[2] = 0;

                output.at<cv::Vec3b>(j,i) = tempVec;
            }
            else
            {
                tempVec[0] = 255;
                tempVec[1] = 255;
                tempVec[2] = 255;

                output.at<cv::Vec3b>(j,i) = tempVec;
            }
        }
    }

    return output;
}


cv::Mat KVision::FindSegColor(cv::Mat rawImg, Segment *seg)
{
    cv::Mat aveImg(rawImg.size(), rawImg.type());

    unsigned short *segmentMap = seg->GetSegMap();
    int segAmount = seg->GetNumSeg();
    std::cout << "SegAmount: " << segAmount << std::endl;

    //if(segAmount > 0)
    int bestSeg = 0;
    int currBestSegColorDiff = 1e6;
    int bestSegColorDiff = 1e6;

    //unsigned char *rawImageData = (unsigned char *)rawImg->imageData;
    //unsigned char *aveImageData = (unsigned char *)aveImg->imageData;
    //memset(aveImageData,0,aveImg->imageSize);

    for(int s = 1; s <= segAmount; s++)
    {
        double aveColorRed = 0;
        double aveColorGreen = 0;
        double aveColorBlue = 0;
        double count = 0;
        std::vector<int> redColor;
        std::vector<int> greenColor;
        std::vector<int> blueColor;

        int mapIndex = 0;

        for(int n = 0; n < rawImg.rows; n++)
        {
            for(int m = 0; m < rawImg.cols; m++)
            {
                if(segmentMap[mapIndex] == s)
                {
                    cv::Vec3f tempVec = rawImg.at<cv::Vec3b>(n,m);

                    //rawColors.push_back(tempVec);

                    redColor.push_back(tempVec[2]);
                    greenColor.push_back(tempVec[1]);
                    blueColor.push_back(tempVec[0]);

//                    if(!(tempVec[0] > 230 && tempVec[1] > 230 && tempVec[2] > 230) &&
//                        !(tempVec[0] < 30 && tempVec[1] < 30 && tempVec[2] < 30))
//                    {
                        aveColorRed += tempVec[2];
                        aveColorGreen += tempVec[1];
                        aveColorBlue += tempVec[0];
                        count++;
//                    }
                }

                mapIndex++;
            }
        }

        if(count != 0)
        {
            aveColorRed /= count;
            aveColorGreen /= count;
            aveColorBlue /= count;
        }

        // sort for median
        std::sort(redColor.begin(), redColor.end());
        std::sort(greenColor.begin(), greenColor.end());
        std::sort(blueColor.begin(), blueColor.end());
        //greenColor.push_back(rawImageData[3*n+1]);
        //blueColor.push_back(rawImageData[3*n+0]);

        mapIndex = 0;
        for(int n = 0; n < rawImg.rows; n++)
        {
            for(int m = 0; m < rawImg.cols; m++)
            {
                if(segmentMap[mapIndex] == s)
                {
                    cv::Vec3f tempVec2;

                    // median
                    int index = count/2;
                    tempVec2[2] = redColor[index];
                    tempVec2[1] = greenColor[index];
                    tempVec2[0] = blueColor[index];

                    aveImg.at<cv::Vec3b>(n,m) = tempVec2;

                    // ave
                    /*aveImageData[3*n+2] = aveColorRed;
                    aveImageData[3*n+1] = aveColorGreen;
                    aveImageData[3*n+0] = aveColorBlue;*/
                }

                mapIndex++;
            }
        }

        currBestSegColorDiff = sqrt(pow(255 - aveColorRed, 2) + pow(0 - aveColorGreen, 2) + pow(0 - aveColorBlue, 2));
        if(currBestSegColorDiff < bestSegColorDiff)
        {
            bestSegColorDiff = currBestSegColorDiff;
            bestSeg = s;
        }

    }

    return aveImg;
}

void KVision::FindAvgAndSD(std::vector <cv::Mat> rawImgs, std::vector <cv::Mat> grayImgs, std::vector <int> segNums, std::vector <cv::Vec3f> &avg, std::vector <cv::Vec3f> &sd)
{

    for(int a = 0; a < rawImgs.size(); a++) //Number of rawImgs = number of grayImgs
    {
        std::map <int, cv::Vec3f> tempAvgs;
        std::map <unsigned char, int> avgCounter;

        for(int b = 0; b < segNums.size(); b++)
        {
            cv::Vec3f tempVec;

            tempVec[0] = 0, tempVec[1] = 0, tempVec[2] = 0;

            tempAvgs[segNums[b]] = tempVec;
            avgCounter[segNums[b]] = 0;
        }

//        std::cout << rawImgs[a].rows << std::endl;
//        std::cout << rawImgs[a].cols << std::endl;
//        std::cout << rawImgs[a].channels() << std::endl;

//        std::cout << grayImgs[a].rows << std::endl;
//        std::cout << grayImgs[a].cols << std::endl;
//        std::cout << grayImgs[a].channels() << std::endl;

        for(int j = 0; j < rawImgs[a].rows; j++)
        {
            for(int i = 0; i < rawImgs[a].cols; i++)
            {
                cv::Vec3b tempVec2 = rawImgs[a].at<cv::Vec3b>(j,i);

                tempAvgs[grayImgs[a].at<unsigned char>(j,i)][0] += tempVec2[0];
                tempAvgs[grayImgs[a].at<unsigned char>(j,i)][1] += tempVec2[1];
                tempAvgs[grayImgs[a].at<unsigned char>(j,i)][2] += tempVec2[2];

                avgCounter[grayImgs[a].at<unsigned char>(j,i)]++;
            }
        }

        for(int temp = 0; temp < segNums.size(); temp++)
        {
            std::cout << "TempAvgs 0: " << tempAvgs[segNums[temp]][0] << std::endl;
            std::cout << "TempAvgs 1: " << tempAvgs[segNums[temp]][1] << std::endl;
            std::cout << "TempAvgs 2: " << tempAvgs[segNums[temp]][2] << std::endl;
        }

        for(int c = 0; c < segNums.size(); c++)
        {
            cv::Vec3f tempAvgVec;

            if(avgCounter[segNums[c]] != 0)
            {
                tempAvgVec[2] = tempAvgs[segNums[c]][2]/avgCounter[segNums[c]];
                tempAvgVec[1] = tempAvgs[segNums[c]][1]/avgCounter[segNums[c]];
                tempAvgVec[0] = tempAvgs[segNums[c]][0]/avgCounter[segNums[c]];
            }

            avg.push_back(tempAvgVec);
        }

//        for(int j = 0; j < rawImgs.rows; j++)
//        {
//            for(int i = 0; i < rawImgs.cols; i++)
//            {
//                for(int d = 0; d < numSegs; d++)
//                {
//                    cv::Vec3b tempVec3 = rawImgs.at<cv::Vec3b>(j,i);

//                    redSum += pow((tempVec3[2] - tempAvgs[d][2]), 2);
//                    greenSum += pow((tempVec3[1] - tempAvgs[d][1]), 2);
//                    blueSum += pow((tempVec3[0] - tempAvgs[d][0]), 2);
//                }
//            }
//        }

//        sd[2] = sqrt((redSum/avgCounter[c]));
//        sd[1] = sqrt((greenSum/avgCounter[c]));
//        sd[0] = sqrt((blueSum/avgCounter[c]));
    }
}
