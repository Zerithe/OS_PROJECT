#include <iostream>
#include <windows.h>
#include <string>
#include <map>
#include <functional>
#include <thread>

#include "ConsoleManager.h"
#include "FCFSScheduler.h"
#include "CPUCore.h"


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
    string input;
    //map<string, functionHolder> functionMap;
    //functionMap["initialize"] = functionHolder{ initialize };
    //functionMap["screen"] = functionHolder{ screen };
    //functionMap["scheduler-test"] = functionHolder{ schedulerTest };
    //functionMap["scheduler-stop"] = functionHolder{ schedulerStop };
    //functionMap["report-util"] = functionHolder{ reportUtil };
    //functionMap["clear"] = functionHolder{ clear };

    ConsoleManager::initialize();
    FCFSScheduler::initialize();

    std::shared_ptr<CPUCore> core1 = std::make_shared<CPUCore>(0);
    std::shared_ptr<CPUCore> core2 = std::make_shared<CPUCore>(1);
    std::shared_ptr<CPUCore> core3 = std::make_shared<CPUCore>(2);
    std::shared_ptr<CPUCore> core4 = std::make_shared<CPUCore>(3);

    FCFSScheduler::getInstance()->addCPUCore(core1);
    FCFSScheduler::getInstance()->addCPUCore(core2);
    FCFSScheduler::getInstance()->addCPUCore(core3);
    FCFSScheduler::getInstance()->addCPUCore(core4);

    std::thread fcfsThread(&FCFSScheduler::runFCFS, FCFSScheduler::getInstance());
    std::thread core1Thread(&CPUCore::runCPU, core1);
    std::thread core2Thread(&CPUCore::runCPU, core2);
    std::thread core3Thread(&CPUCore::runCPU, core3);
    std::thread core4Thread(&CPUCore::runCPU, core4);

    fcfsThread.detach();
    core1Thread.detach();
    core2Thread.detach();
    core3Thread.detach();
    core4Thread.detach();
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
        if (ConsoleManager::getInstance()->getCreatedProcess() != nullptr)
            FCFSScheduler::getInstance()->addProcess(ConsoleManager::getInstance()->getCreatedProcess());
        if (ConsoleManager::getInstance()->getShowListOfProcesses())
            FCFSScheduler::getInstance()->showListOfProcesses();
        running = ConsoleManager::getInstance()->isRunning();
    }

    ConsoleManager::destroy();
    FCFSScheduler::destroy();
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