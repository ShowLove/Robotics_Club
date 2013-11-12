#include<ieee/communication/datasender.h>

using namespace CxUtils;
using namespace std;
using namespace cv;

DataSender::DataSender()
{

}

DataSender::~DataSender()
{

}

void DataSender::Initialize(std::string ServerName,int Port)
{
    mPort=Port;
    mServerName=ServerName;
    if(!mClient.InitializeSocket(mServerName, mPort))
    {
        cout << "Unable to connect to host " << mServerName << " on Port " << mPort<< endl;
        return;
    }
    cout << "Initialized UDP Client Socket on port " << mPort << endl;

}


void DataSender::SendData(GamePacket Data)
{
    mClient.Send((const char *)&Data, sizeof(struct GamePacket));
}
