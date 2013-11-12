#include<ieee/communication/gameserver.h>
using namespace std;
using namespace cv;

GameServer::GameServer()
{
    mBackupImage=imread("failwhale.jpg");
}

GameServer::~GameServer()
{
    if(mImageReceivers.size()>0)
    {
        for(int i=0;i<mImageReceivers.size();i++)
        {
            delete mImageReceivers[i];
        }
    }
    mImageReceivers.clear();
    if(mDataReceivers.size()>0)
    {
        for(int i=0;i<mDataReceivers.size();i++)
        {
            delete mDataReceivers[i];
        }
    }
    mDataReceivers.clear();
}

void GameServer::Initialize(int NumReceivers)
{
    mNumReceivers=NumReceivers;
    cout << "Starting " << mNumReceivers << " Receivers.." << endl;
    for(int i=0;i<mNumReceivers;i++)
    {
        ImageReceiver *tmpI=new ImageReceiver();
        tmpI->Initialize(5000+i);
        mImageReceivers.push_back(tmpI);

        DataReceiver *tmpD=new DataReceiver();
        tmpD->Initialize(4000+i);
        mDataReceivers.push_back(tmpD);
    }

}

Mat GameServer::GetImage(int Receiver)
{
    Mat out;
    if(Receiver>=0 && Receiver<mImageReceivers.size())
    {
        if(!mImageReceivers[Receiver]->GetImage(out))
        {
            cout << "Receiver " << Receiver << ": does not have images yet" << endl;
            out=mBackupImage;
        }
    }
    else
    {
        out=mBackupImage;
    }
    return out;
}


ProcessedData GameServer::GetData(int Receiver)
{
    if(Receiver>=0 && Receiver<mDataReceivers.size())
    {
        ProcessedData pd = mDataReceivers[Receiver]->GetData();
        return pd;
    }
    else
    {
        ProcessedData tmp;
        tmp.mCorrect=0;
        tmp.mIncorrect=0;
        tmp.mScore=0;
        tmp.mTeamID=-1;
        tmp.mTimeMs=0;
        tmp.mTrail.clear();
        return tmp;
    }
}

bool GameServer::GetValid(int Receiver)
{
    if(Receiver>=0 && Receiver<mDataReceivers.size())
    {
        //return (mDataReceivers[Receiver]->GetValid() || mImageReceivers[Receiver]->GetValid() );
        return (mDataReceivers[Receiver]->GetValid());
    }
    return false;
}
