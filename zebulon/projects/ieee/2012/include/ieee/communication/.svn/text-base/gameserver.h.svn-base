//Coordinates the receiving of data for the gaming

//2 threads per client

#include <ieee/communication/imagereceiver.h>
#include <ieee/communication/datareceiver.h>
#include <vector>
#include<opencv2/core/core.hpp>

#ifndef GAMESERVER_H
#define GAMESERVER_H

class GameServer
{
    public:
        GameServer();
        ~GameServer();

        void Initialize(int NumReceivers);

        cv::Mat GetImage(int Receiver);

        ProcessedData GetData(int Receiver);

        bool GetValid(int Receiver);

    private:
        int mNumReceivers;
        std::vector<ImageReceiver *> mImageReceivers;
        std::vector<DataReceiver *> mDataReceivers;

        cv::Mat mBackupImage;
};


#endif // GAMESERVER_H
