#include <iostream>
#include <windows.h>
#include <string>
#include <map>
#include <functional>

#include "ConsoleManager.h"


using namespace std;

struct functionHolder
{
    function<void()> func;
};

void headerPrint()
{
    cout << "   ___________ ____  ____  _____________  __\n";
    cout << "  / ____/ ___// __ \\/ __ \\/ ____/ ___/\\ \\/ /\n";
    cout << " / /    \\__ \\/ / / / /_/ / __/  \\__ \\  \\  /\n";
    cout << "/ /___ ___/ / /_/ / ____/ /___ ___/ /  / /\n";
    cout << "\\____//____/\\____/_/   /_____//____/  /_/\n";
    cout << "\n";

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    cout << "Hello, Welcome to CSOPESY commandline!\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    cout << "Type 'exit' to quit, 'clear' to clear the screen \n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}


void initialize()
{
    cout << "initialize command recognized. Doing something.\n";
}

void screen()
{
    cout << "screen command recognized. Doing something.\n";
}

void schedulerTest()
{
    cout << "scheduler-test command recognized. Doing something.\n";
}

void schedulerStop()
{
    cout << "scheduler-stop command recognized. Doing something.\n";
}

void reportUtil()
{
    cout << "report-util command recognized. Doing something.\n";
}

void clear()
{
    cout << "\033[2J\033[1;1H";
    headerPrint();
}


int main()
{
    /*string input;
    map<string, functionHolder> functionMap;
    functionMap["initialize"] = functionHolder{ initialize };
    functionMap["screen"] = functionHolder{ screen };
    functionMap["scheduler-test"] = functionHolder{ schedulerTest };
    functionMap["scheduler-stop"] = functionHolder{ schedulerStop };
    functionMap["report-util"] = functionHolder{ reportUtil };
    functionMap["clear"] = functionHolder{ clear };*/

    ConsoleManager::initialize();

    //headerPrint();

    /*while (input != "exit")
    {
        cout << "enter a command: ";
        cin >> input;

        if (functionMap.find(input) != functionMap.end())
        {
            functionMap[input].func();
        }
        else if (input != "exit")
        {
            cout << "Unkown command.\n";
        }
    }*/

    bool running = true;
    while (running) 
    {
        ConsoleManager::getInstance()->process();
        ConsoleManager::getInstance()->drawConsole();

        running = ConsoleManager::getInstance()->isRunning();
    }

    ConsoleManager::destroy();

    return 0;
}

/*
   ___________ ____  ____  _____________  __
  / ____/ ___// __ \/ __ \/ ____/ ___/\ \/ /
 / /    \__ \/ / / / /_/ / __/  \__ \  \  /
/ /___ ___/ / /_/ / ____/ /___ ___/ /  / /
\____//____/\____/_/   /_____//____/  /_/

*/

/*

Color      Background    Foreground
---------------------------------------------
Black            0           0
Blue             1           1
Green            2           2
Cyan             3           3
Red              4           4
Magenta          5           5
Brown            6           6
White            7           7
Gray             -           8
Intense Blue     -           9
Intense Green    -           10
Intense Cyan     -           11
Intense Red      -           12
Intense Magenta  -           13
Yellow           -           14
Intense White    -           15



  */