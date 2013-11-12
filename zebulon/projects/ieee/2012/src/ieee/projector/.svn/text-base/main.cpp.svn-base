
#include "ieee/cassy/ieeevision.h"

#include<ieee/communication/imagesender.h>
#include<ieee/communication/datasender.h>

#include<ieee/communication/gameserver.h>
#include <ieee/communication/gamepacket.h>

#include <cxutils/cxutils.h>
#include <xmlconfig/xmlconfigurator.h>

using namespace Zebulon;
using namespace IEEE;

#define HOSTNAME "localhost"
#define NUM_SENDERS 4

#define ORIG_RES cv::Size(640, 480)

// Raw Display Info
#define RAW_SCALE_WIDTH 592
#define RAW_SCALE_HEIGHT 444
#define RAW_POS_X 4
#define RAW_POS_Y 4

#define RAW_POS cv::Point(4, 4)
#define RAW_RES cv::Size(592, 444)

#define RAW_DISPLAY_TIME 5 // in seconds

// Trail Display Info
#define TRAIL_RES cv::Size(196, 147)
#define TRAIL_INIT_POS cv::Point(600, 4)
#define TRAIL_SCALE_POS_OFFSET_Y 148
#define TRAIL_BG_COLOR cv::Vec3b(50,50,50) // 30,30,30
#define TRAIL_HL_COLOR cv::Scalar(04, 201, 255) // 25,217,217 // 0,150,0

#define TRAIL_VEHICLE_RADIUS 10

#define TRAIL_COLOR_1_RED 255
#define TRAIL_COLOR_1_GREEN 0
#define TRAIL_COLOR_1_BLUE 0

#define TRAIL_COLOR_2_RED 0
#define TRAIL_COLOR_2_GREEN 255
#define TRAIL_COLOR_2_BLUE 0

#define TRAIL_COLOR_3_RED 0
#define TRAIL_COLOR_3_GREEN 0
#define TRAIL_COLOR_3_BLUE 255

#define TRAIL_COLOR_4_RED 255
#define TRAIL_COLOR_4_GREEN 255
#define TRAIL_COLOR_4_BLUE 0

// Text Display Info
#define LARGE_TEXT_SIZE 16
#define MEDIUM_TEXT_SIZE 12
#define SMALL_TEXT_SIZE 8

#define TEXT_OFFSET 25

#define TEXT_POS_TL_X 10 // was 4
#define TEXT_POS_BR_X 595 // was 595
#define TEXT_POS_TL_Y 452 // was 452
#define TEXT_POS_BR_Y 596 // was 596

#define UNIVERSITY_POS cv::Point(TEXT_POS_TL_X, TEXT_POS_TL_Y + LARGE_TEXT_SIZE + 3)
//#define TEAM_POS cv::Point(((TEXT_POS_BR_X - TEXT_POS_TL_X) / 2.0), TEXT_POS_TL_Y + LARGE_TEXT_SIZE)
#define TEAM_POS cv::Point(TEXT_POS_TL_X, TEXT_POS_TL_Y + LARGE_TEXT_SIZE + TEXT_OFFSET + 1 + 5)
#define COMP_TYPE_POS cv::Point(TEXT_POS_TL_X + 0, TEXT_POS_TL_Y + (TEXT_POS_BR_Y - TEXT_POS_TL_Y) / 3.0 + MEDIUM_TEXT_SIZE + 1 + 5) // 450 to right align

#define CURR_SCORE_POS cv::Point(4 + ((TEXT_POS_BR_X - 4) / 4.0) - 10, TEXT_POS_BR_Y - 0)
#define PERCENTAGE_POS cv::Point(4 + ((TEXT_POS_BR_X - 4) / 2.0) + 20, TEXT_POS_BR_Y - 0)
#define LAPS_POS cv::Point(4 + ((TEXT_POS_BR_X - 4)*3.0 / 4.0) + 50, TEXT_POS_BR_Y - 0)

#define CORRECT_POS cv::Point(5, 59 + LARGE_TEXT_SIZE)
#define INCORRECT_POS cv::Point(5, 5 + LARGE_TEXT_SIZE)

//#define HIGH_SCORE_POS cv::Point(5, 5)
//#define RAW_TIME_POS cv::Point(TEXT_POS_TL_X, TEXT_POS_TL_Y + (TEXT_POS_BR_Y - TEXT_POS_TL_Y)/2.0 + LARGE_TEXT_SIZE + 13)
#define RAW_TIME_POS cv::Point(596 - 130, 448 - 5) // bottom left of raw image
//#define RAW_TIME_POS cv::Point(100, 100)

#define TRAIL_L_X 600
#define TRAIL_R_X 796
#define TRAIL_T_Y 4
#define TRAIL_B_Y 151

#define INIT_TRAIL_SCORE_POS cv::Point(TRAIL_L_X + 2, TRAIL_T_Y + MEDIUM_TEXT_SIZE)
#define INIT_TRAIL_PERCENT_POS cv::Point(TRAIL_L_X + (TRAIL_R_X - TRAIL_L_X)/2.0 - 15, TRAIL_T_Y + MEDIUM_TEXT_SIZE)
#define INIT_TRAIL_LAPS_POS cv::Point(TRAIL_R_X - 30, TRAIL_T_Y + MEDIUM_TEXT_SIZE)

#define INIT_TRAIL_TIME_POS cv::Point(TRAIL_R_X - 85, TRAIL_B_Y - 3)


// Course stuff
#define TASK_POS_INIT 592/2.0 - 100
#define TASK1_POS cv::Point(4 + TASK_POS_INIT, 4 + 30)
#define TASK2_POS cv::Point(4 + TASK_POS_INIT + 200*(1.0/3.0), 4 + 30)
#define TASK3_POS cv::Point(4 + TASK_POS_INIT + 200*(2.0/3.0), 4 + 30)
#define TASK4_POS cv::Point(4 + TASK_POS_INIT + 200, 4 + 30)

//(196, 147)
#define TASK_TRAIL_POS_INIT 196/2.0 - 50
#define TASK1_TRAIL_POS cv::Point(600 + TASK_TRAIL_POS_INIT, 4 + 147/2.0)
#define TASK2_TRAIL_POS cv::Point(600 + TASK_TRAIL_POS_INIT + 100*(1.0/3.0), 4 + 147/2.0)
#define TASK3_TRAIL_POS cv::Point(600 + TASK_TRAIL_POS_INIT + 100*(2.0/3.0), 4 + 147/2.0)
#define TASK4_TRAIL_POS cv::Point(600 + TASK_TRAIL_POS_INIT + 100, 4 + 147/2.0)

//#define TEXT_COLOR_RED 217
//#define TEXT_COLOR_GREEN 217
//#define TEXT_COLOR_BLUE 25



TeamTextInfo Load(const std::string& filename) // add string, string, vec3b?
{
    // TODO:: FILL IN XML STUFF BASED ON NEW STRUCTURE
    Configuration::XmlConfigurator* config = new Configuration::XmlConfigurator();
    config->Load(filename,"TeamInfo");

    std::string tmpString;
    int tmp;
    TeamTextInfo tmpTeamTextInfo;

    config->GetVar("Names@University", tmpString); tmpTeamTextInfo.University = tmpString;
    //std::cout << "University: " << tmpTeamTextInfo.University << std::endl;
    config->GetVar("Names@Name", tmpString); tmpTeamTextInfo.Team = tmpString;
    //std::cout << "Team Name: " << tmpTeamTextInfo.Team << std::endl;

    config->GetVar("Competition@Type", tmpString); tmpTeamTextInfo.CompType = tmpString;
    //std::cout << "Comp Type: " << tmpTeamTextInfo.CompType << std::endl;

    config->GetVar("Colors@TextColorRed", tmp); tmpTeamTextInfo.TextColor[0] = tmp;
    //std::cout << "Text Colors - Red: " << (int)tmpTeamTextInfo.TextColor[0] << std::endl;
    config->GetVar("Colors@TextColorGreen", tmp); tmpTeamTextInfo.TextColor[1] = tmp;
    //std::cout << "Text Colors - Green: " <<  (int)tmpTeamTextInfo.TextColor[1] << std::endl;
    config->GetVar("Colors@TextColorBlue", tmp); tmpTeamTextInfo.TextColor[2] = tmp;
    //std::cout << "Text Colors - Blue: " <<  (int)tmpTeamTextInfo.TextColor[2] << std::endl;

    config->GetVar("Colors@BGColorRed", tmp); tmpTeamTextInfo.BGColor[0] = tmp;
    //std::cout << "BG Colors - Red: " << (int)tmpTeamTextInfo.BGColor[0] << std::endl;
    config->GetVar("Colors@BGColorGreen", tmp); tmpTeamTextInfo.BGColor[1] = tmp;
    //std::cout << "BG Colors - Green: " << (int)tmpTeamTextInfo.BGColor[1] << std::endl;
    config->GetVar("Colors@BGColorBlue", tmp); tmpTeamTextInfo.BGColor[2] = tmp;
    //std::cout << "BG Colors - Blue: " << (int)tmpTeamTextInfo.BGColor[2] << std::endl;

    delete config;

    return tmpTeamTextInfo;

}

int main(int argc, char *argv[])
{
    // Initialize communication
    GameServer server;
    server.Initialize(NUM_SENDERS);

    // Initialize dispaly functions
    IEEEVisionCassy ieeeVision;

    // Create space for final display
    cv::Mat finalDisplay(600, 800, CV_8UC3);
    // Create space for additional information
    cv::Mat senderRawImg(480, 640, CV_8UC3);

    // Info storage about senders
    std::vector< bool > currValid;
    currValid.assign(NUM_SENDERS, false);
    std::vector< int > displayCount;
    displayCount.assign(NUM_SENDERS, 0);
    //bool initChoiceFlag = false;
    bool anyValidFlag = false;
    int senderToDisplay = 0;
    int currSenderToDisplay = 0;

    std::vector< ProcessedData > currProcessedPacket;
    ProcessedData tmpProcessedPacket;
    tmpProcessedPacket.mCorrect = 0;
    tmpProcessedPacket.mIncorrect = 0;
    tmpProcessedPacket.mLaps = 0;
    tmpProcessedPacket.mScore = 0;
    tmpProcessedPacket.mTeamID = -1;
    tmpProcessedPacket.mTimeMs = 0;
    //tmpGamePacket.mTrail
    currProcessedPacket.assign(NUM_SENDERS, tmpProcessedPacket);

    std::vector< cv::Point > taskPositions;
    taskPositions.push_back(TASK1_POS);
    taskPositions.push_back(TASK2_POS);
    taskPositions.push_back(TASK3_POS);
    taskPositions.push_back(TASK4_POS);

    std::vector< cv::Point > taskTrailPositions;
    taskTrailPositions.push_back(TASK1_TRAIL_POS);
    taskTrailPositions.push_back(TASK2_TRAIL_POS);
    taskTrailPositions.push_back(TASK3_TRAIL_POS);
    taskTrailPositions.push_back(TASK4_TRAIL_POS);

    bool initStartDisplay = true;
    double startTime = CxUtils::GetTimeSeconds();
    double displayTime = 0;

    TextPos textPositions;
    textPositions.LargeText = LARGE_TEXT_SIZE;
    textPositions.MediumText = MEDIUM_TEXT_SIZE;
    textPositions.SmallText = SMALL_TEXT_SIZE;

    textPositions.University = UNIVERSITY_POS;
    textPositions.Team = TEAM_POS;
    textPositions.CompType = COMP_TYPE_POS;
    textPositions.RawTime = RAW_TIME_POS;
    textPositions.CurrScore = CURR_SCORE_POS;
    textPositions.Percentage = PERCENTAGE_POS;
    textPositions.Laps = LAPS_POS;

    textPositions.InitTrailScore = INIT_TRAIL_SCORE_POS;
    textPositions.InitTrailPercent = INIT_TRAIL_PERCENT_POS;
    textPositions.InitTrailLaps = INIT_TRAIL_LAPS_POS;
    textPositions.InitTrailTime = INIT_TRAIL_TIME_POS;
    textPositions.TrailOffset = TRAIL_SCALE_POS_OFFSET_Y;

    // Resize robotics logo
    cv::Mat roboticsLogo = cv::imread("robotics_logo.jpg");
    cv::Mat roboticsLogoLarge, roboticsLogoSmall;
    cv::resize(roboticsLogo, roboticsLogoLarge, RAW_RES);
    cv::resize(roboticsLogo, roboticsLogoSmall, TRAIL_RES);

    // Set window to be full screen
    cv::namedWindow("Final Display", CV_WINDOW_NORMAL);
    cv::setWindowProperty("Final Display", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    // Run until program is killed
    while(1)
    {
        //ieeeVision.DrawWhiteLines(finalDisplay);
        // Clear Display
        ieeeVision.ConditionFinalImage(finalDisplay);

        // Check what data is available
        anyValidFlag = false;
        for(int i = 0; i < currValid.size(); i++)
        {
            if(server.GetValid(i) == true)
            {
                anyValidFlag = true;
                currValid[i] = true;
                // Load Game Packet
                currProcessedPacket[i] = server.GetData(i);
            }
            else
            {
                currValid[i] = false;
            }
        }
        // Display who is valid
        /*std::cout << "////////////////////" << std::endl;
        for(int i = 0; i < currValid.size(); i++)
        {
            std::cout << "CurrValid: " << i << " is " << currValid[i] << std::endl;
            std::cout << "TeamID: " << i << " is " << currProcessedPacket[i].mTeamID << std::endl;
        }*/

        /*int tempID = 0;

        char tmpFilename[64];
        sprintf(tmpFilename, "settings/teams/%d.xml", tempID);
        std::cout << "tmpFilename: " << tmpFilename << std::endl;

        TeamTextInfo tmpTeamTextInfo = Load(tmpFilename);*/

        // Insert raw and trail images and data
        if(anyValidFlag == true)
        {
            // Check which sender to choose next
            int leastTime = 1000;
            for(int i = 0; i < currValid.size(); i++)
            {
                if(currValid[i] == true && displayCount[i] < leastTime)
                {
                    leastTime = displayCount[i];
                    senderToDisplay = i;
                }
            }

            // Check if raw feed should be switched
            displayTime = CxUtils::GetTimeSeconds() - startTime;
            //std::cout << "displayTime: " << displayTime << std::endl;
            if(initStartDisplay == true)
            {
                startTime = CxUtils::GetTimeSeconds();
                currSenderToDisplay = senderToDisplay;
                displayCount[senderToDisplay]++;
                initStartDisplay = false;
            }
            else
            {
                if(displayTime > RAW_DISPLAY_TIME)
                {
                    startTime = CxUtils::GetTimeSeconds();
                    currSenderToDisplay = senderToDisplay;
                    displayCount[senderToDisplay]++;
                }
            }

            // Grab raw feed
            senderRawImg = server.GetImage(currSenderToDisplay);
            // Condition raw data and add to finalDisplay
            ieeeVision.AddRawImage(senderRawImg, RAW_RES, RAW_POS, finalDisplay);
            // Add text for raw sender
            char tmpFilename[64];
            sprintf(tmpFilename, "settings/teams/%d.xml", currProcessedPacket[currSenderToDisplay].mTeamID);
            TeamTextInfo tmpTeamTextInfo = Load(tmpFilename);

            ieeeVision.DrawRawAnswers(finalDisplay, currProcessedPacket[currSenderToDisplay].mCourseMoves, taskPositions);

            ieeeVision.AddRawText(tmpTeamTextInfo, currProcessedPacket[currSenderToDisplay], textPositions, finalDisplay);





            // Condition image for trail images
            ieeeVision.ConditionTrailSpace(TRAIL_INIT_POS, TRAIL_SCALE_POS_OFFSET_Y, TRAIL_RES, TRAIL_BG_COLOR, finalDisplay);
            // Highlight trail that is being displayed
            ieeeVision.HighlightCurrentTrail(currSenderToDisplay, TRAIL_INIT_POS, TRAIL_SCALE_POS_OFFSET_Y, TRAIL_RES, TRAIL_HL_COLOR, finalDisplay);
            // Add the rest of the information
            for(int i = 0; i < currValid.size(); i++)
            {
                if(currValid[i] == true)
                {
                    // Add Trail
                    //int offset, ProcessedData processedData, cv::Point initPos, cv::Size origRes, cv::Size trailRes, cv::Mat &output
                    ieeeVision.AddTrail(i, TRAIL_SCALE_POS_OFFSET_Y, currProcessedPacket[i], TRAIL_INIT_POS, ORIG_RES, TRAIL_RES, finalDisplay);

                    ieeeVision.DrawTrialAnswers(finalDisplay, i, TRAIL_SCALE_POS_OFFSET_Y, currProcessedPacket[i].mCourseMoves, taskTrailPositions);

                    // Add Text
                    ieeeVision.AddTrailText(i, currProcessedPacket[i], textPositions, finalDisplay);
                }
                else
                {
                    // UCF picture?
                    for(int j = 0; j < roboticsLogoSmall.rows; j++)
                    {
                        for(int k = 0; k < roboticsLogoSmall.cols; k++)
                        {
                            cv::Vec3b tempVec = roboticsLogoSmall.at< cv::Vec3b >(j, k);
                            finalDisplay.at< cv::Vec3b >(j + TRAIL_INIT_POS.y + i*TRAIL_SCALE_POS_OFFSET_Y, k + TRAIL_INIT_POS.x) = tempVec;
                        }
                    }
                }
            }


        }
        else
        {
            // Display Robotics Club logo?
            for(int j = 0; j < roboticsLogoLarge.rows; j++)
            {
                for(int i = 0; i < roboticsLogoLarge.cols; i++)
                {
                    cv::Vec3b tempVec = roboticsLogoLarge.at< cv::Vec3b >(j, i);
                    finalDisplay.at< cv::Vec3b >(j + RAW_POS.y, i + RAW_POS.x) = tempVec;
                }
            }

            CvFont displayTextSmall = cv::fontQt("Times", 10, cv::Scalar(255, 201, 04));
            cv::addText(finalDisplay, "presents", cv::Point(104, 400), displayTextSmall);

            CvFont displayTextLarge = cv::fontQt("Times", 20, cv::Scalar(255, 201, 04));
            cv::addText(finalDisplay, "The IEEE SoutheastCon 2012", cv::Point(104, 482), displayTextLarge);
            cv::addText(finalDisplay, "Hardware Competition", cv::Point(104, 536), displayTextLarge);

            //ieeeVision.DrawX(cv::Point(50,50), 20, 8, cv::Scalar(255,0,0), finalDisplay);
            //ieeeVision.DrawCheck(cv::Point(70,50), 20, 4, cv::Scalar(255,0,0), finalDisplay);
        }

        cv::imshow("Final Display", finalDisplay);

        if ( (cvWaitKey(10) & 255) == 27 ) break;
    }


    return 0;
}

/* End of File */

