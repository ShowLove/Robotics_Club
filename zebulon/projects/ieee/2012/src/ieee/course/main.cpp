
#include "ieee/course/game.h"

#include "cxutils/cxutils.h"

#include <iostream>
#include <stdio.h>

using namespace Zebulon;
using namespace IEEE;

// PortOffset:int, JudgeOverride:(1 || 0), [Camera:int, UsbPort:string]*
int main(int argc, char* argv[])
{
    int numCameras=1;
    int portOffset=0;

    bool judgeOverride=false;

    if (argc > 1)
    {
        //numCameras = atoi(argv[1]);
        portOffset = atoi(argv[1]);
    }

    if (argc > 2)
        judgeOverride = atoi(argv[2]);

    std::vector<int> firewires;
    std::vector<std::string> usbports;

    for (int k=0; argc>(4+k); k+=2)
    {
        firewires.push_back(atoi(argv[3+k]));
        usbports.push_back(argv[3+k+1]);
    }

    std::cout << "Initializing game with " << firewires.size() << " cameras" << std::endl
              << " Port offset: " << portOffset << std::endl
              << " Judge Override: " << judgeOverride << std::endl;

    std::vector<Game*> games;

    /*
    for (int i=0; i<numCameras; i++)
    {
        Game* game = new Game();

        //XML the course numbers!
        game->Initialize(i, "192.168.1.211", 4000+i+portOffset, 5000+i+portOffset, judgeOverride);

        games.push_back(game);
    }//*/

    for (int i=0; i<firewires.size(); i++)
    {
        Game* game = new Game();

        game->Initialize(firewires[i], usbports[i], "192.168.1.211", 4000+i+portOffset, 5000+i+portOffset, judgeOverride);

        games.push_back(game);
    }

    cv::Mat pic;
    std::vector<Game*>::iterator iter;
    while (true)
    {
        for (iter=games.begin(); iter!=games.end(); iter++)
        {
            (*iter)->GetImage(pic);
            //int tempSelectedID = (*iter)->GetSelectedID();
            //std::cout << "tempSelectedID: " << tempSelectedID << std::endl;
            if (pic.rows)
                cv::imshow((*iter)->GetWindowName(), pic);
        }
        //std::cout << "Stuff" << std::endl;
        if (cv::waitKey(150) == 27) break;
    }

    return 0;
}
