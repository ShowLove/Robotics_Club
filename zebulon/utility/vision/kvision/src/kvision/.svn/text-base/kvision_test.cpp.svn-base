#include <iostream>
#include <stdio.h>
#include "kvision/kvision.h"

using namespace Zebulon;
using namespace Vision;

int main()
{
    KVision ktest;

    //cv::Mat input = cv::imread("000134.png", 1);

    int fileNum = 0;
    char fileTrainingRaw[256];
//    char fileTrainingRaw2[256];

//    std::vector <cv::Mat> rawImgSet;
//    std::vector <cv::Mat> grayImgSet;
//    std::vector <cv::Vec3f> avg, sd;
//    std::vector <int> segList;

    while(1)
    {
        sprintf(fileTrainingRaw, "vision_prac/sub/current_camera/competition/2011.07.15.17.10.42_fri_prac_1030_1/ForwardCamera/%06d.png", fileNum);
        //sprintf(fileTrainingRaw,"000134.png");
        cv::Mat input = cv::imread(fileTrainingRaw, 1);

//        for(int a = 0; a < 2; a++)
//        {
//             sprintf(fileTrainingRaw, "test%d.bmp", 0);
//             cv::Mat input = cv::imread(fileTrainingRaw, 1);
//             rawImgSet.push_back(input);
//        }

//        for(int b = 3; b < 5; b++)
//        {
//            sprintf(fileTrainingRaw2, "test%d.bmp", 1);
//            cv::Mat input2 = cv::imread(fileTrainingRaw2, 0);
//            grayImgSet.push_back(input2);
//        }

//        segList.push_back(0);
//        segList.push_back(255);

        cv::Mat output(input.rows, input.cols, CV_8UC3);
//        cv::Mat output2(input.rows, input.cols, CV_8UC3);
//        cv::Mat clusters, labels, segOut;

//        ktest.Setup(4, input.rows, input.cols, 50, 50, 255, 0, 0, 200, 8, true);

        //ktest.FindAvgAndSD(rawImgSet, grayImgSet, segList, avg, sd);

//        for(int c = 0; c < avg.size(); c++)
//        {
//            std::cout << "Red Average: " << avg[c][2] << std::endl;
//            std::cout << "Green Average: " << avg[c][1] << std::endl;
//            std::cout << "Blue Average: " << avg[c][0] << std::endl;

//            std::cout << std::endl;
//        }

        //output2 = ktest.ThreeChannelCompare(input, 0, 0, 255);

        //cv::Mat tempCond = ktest.ConditionMat(input);
    /*
        cv::Mat tempRemoved = ktest.RemovePoints(tempCond, true, 0, 0, 255, 255, 255, 200);
        ktest.Kmeans(tempRemoved, 5, true, labels, clusters);
        tempRemoved = ktest.RemovePoints(tempRemoved, true, 0, 0, 0, 0, 255, 200);
        ktest.Kmeans(tempRemoved, 5, true, labels, clusters);*/
        //std::cout << clusters << std::endl; //We want: row 2 (1) and columns 1-3 (0-2)

        //segOut = ktest.KMeansSeg(tempCond, labels, clusters, true);
        //std::cout << clusters << std::endl;

        //cv::Mat tempRemoved = ktest.RemovePoints(tempCond, true, 0, 0, 0, 255, 255, 250);
        //ktest.Kmeans(tempRemoved, 3, true, labels, clusters);

        //output = ktest.ReverseMat(segOut, labels, clusters);

        cv::Vec3f avg, sd;

        avg[0] = 90.1211;
        avg[1] = 89.9184;
        avg[2] = 30.2735;

        sd[0] = 26.6017;
        sd[1] = 25.8759;
        sd[2] = 14.5289;

        output = ktest.StdDevThreshold(input, avg, sd*3);

//        output2 = ktest.BinaryInvert(output);

//        cv::Vec3f averageColor = ktest.AverageColor(input);
//        cv::Vec3f standDev = ktest.StandDev(input);

//        std::cout << "Average Red: " <<  averageColor[2] << std::endl;
//        std::cout << "Average Green: " <<  averageColor[1] << std::endl;
//        std::cout << "Average Blue: " <<  averageColor[0] << std::endl;

//        std::cout << std::endl;

//        std::cout << "SD Red: " << standDev[2] << std::endl;
//        std::cout << "SD Green: " << standDev[1] << std::endl;
//        std::cout << "SD Blue: " << standDev[0] << std::endl;

        cv::imshow("Original", input);
        cv::imshow("After", output);
        cv::waitKey(300);

        //std::cout << "File Number: " << fileNum << std::endl;

        fileNum++;
        if(fileNum >= 422)
        {
            fileNum = 0;
        }
    }

    return 0;
}
