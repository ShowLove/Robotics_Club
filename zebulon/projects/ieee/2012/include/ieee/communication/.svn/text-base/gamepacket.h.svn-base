//Data packet to be sent around

#ifndef GAMEPACKET_H
#define GAMEPACKET_H

struct CourseMoves
{
    int mTrue[4];
    int mMeasured[4];
};

#define MAXPOINTS (16)
struct GamePacket
{
    int mScore;
    int mCorrect;
    int mIncorrect;
    int mTimeMs;
    int mTeamID;
    int mLaps;

    CourseMoves mCourseMoves;
    int mNumPoints;
    //for a single update of points (not trail)
    float mXList[MAXPOINTS];
    float mYList[MAXPOINTS];
};

struct ProcessedData
{
    int mScore;
    int mCorrect;
    int mIncorrect;
    int mTimeMs;
    int mTeamID;
    int mLaps;

    CourseMoves mCourseMoves;
    std::vector<cv::Point2f> mTrail;
};

#endif // GAMEPACKET_H
