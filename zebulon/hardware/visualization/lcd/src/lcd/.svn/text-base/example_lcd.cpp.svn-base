#include "lcd/lcd2041.h"
#include "lcd/vfd420.h"
#include <iostream>

using namespace std;

using namespace Zebulon;
using namespace Hardware;

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        cout << "Port name not set, please set in command line" << endl;
        cin;
        return 0;
    }
    
    cout << "Initializing screen on port " << argv[1] << endl;
    VFD420 screen;
    screen.Initialize(argv[1], 9600);
    
    cout << "Connecting..." << endl;
    screen.Connect();
    screen.ClearScreen();
    
    cout << "Turning on the cursor..." << endl;
    screen.CursorOn();
    
    CxUtils::SleepMs(3000);
    cout << "Turning the backlight ON for 1 minute..." << endl;
    screen.BackLightOn(0);
    
    CxUtils::SleepMs(3000);
    cout << "Setting contrast to 75%..." << endl;
    screen.SetContrast(75);
    screen.ClearScreen();
    screen.SetText("Setting contrast to 75%...");
    
    CxUtils::SleepMs(3000);
    cout << "Displaying the message:" << endl << "\t1234567890 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz -?/|\\!@#$%^&*()" << endl;
    screen.ClearScreen();
    screen.SetText("1234567890 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz -?/|\\!@#$%^&*()");
    
    CxUtils::SleepMs(3000);
    cout << "Setting contrast to 100%..." << endl;
    screen.ClearScreen();
    screen.SetText("Setting contrast to 100%...");
    screen.SetContrast(100);
    
    CxUtils::SleepMs(3000);
    cout << "Setting contrast to 0%..." << endl;
    screen.ClearScreen();
    screen.SetText("Setting contrast to 0%...");
    screen.SetContrast(0);
    
    CxUtils::SleepMs(3000);
    cout << "Setting contrast to 75%..." << endl;
    screen.SetContrast(75);
    screen.ClearScreen();
    screen.SetText("Setting contrast to 75%...");
    
    screen.SetText("Clearing the screen...");
    CxUtils::SleepMs(3000);
    screen.ClearScreen();
    cout << "Clearing the screen..." << endl;
    screen.ClearScreen();
    
    cout << "Attempting write to second line.." << endl;
    //screen.SetCursorPosition(0x02, 0x01);
    screen.WriteAtPosition(0x01, 0x0A, "This is");
    screen.WriteAtPosition(0x02, 0x02, "pretty neat");
    screen.WriteAtPosition(0x03, 0x0A, "if you");
    screen.WriteAtPosition(0x04, 0x0F, "ask me");
    CxUtils::SleepMs(3000);
    
    cout << "Putting cursor in the middle of the screen..." << endl;
    screen.SetCursorPosition(2, 10);
    CxUtils::SleepMs(3000);
    
    cout << "Turning off the cursor..." << endl;
    screen.CursorOff();
    CxUtils::SleepMs(3000);
    
    cout << "This is the end of the program, I hope nothing broke for you." << endl;
    return 0;
}