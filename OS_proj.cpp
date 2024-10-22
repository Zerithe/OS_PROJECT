#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <string>
#include <map>
#include <functional>
#include <thread>
#include <vector>

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
    int numCpu;
    string scheduler;
    int quantumCycles;
    int batchProcessFreq;
    int minInstructions;
    int maxInstructions;
    int delayPerExecution;

    // Vector to hold shared pointers to CPUCore objects
    std::vector<std::shared_ptr<CPUCore>> cpuCores;

    // Vector to hold CPUCore threads
    std::vector<std::thread> coreThreads;

    headerPrint();

    while (input != "initialize")
    {
        cout << "enter a command: ";
        cin >> input;
        if (input != "initialize")
            cout << "Unknown command.\n";
    }
    system("cls");

    // Open the config text file
    std::ifstream infile("config.txt");

    // Check if the file was successfully opened
    if (!infile) {
        std::cerr << "Unable to open config text file";
        return 1; // Return with an error code
    }
 
    std::string line;

    // Read the file line by line
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string key;

        // Extract the key (before the space) and then process based on the key
        if (line.find("num-cpu") != std::string::npos) {
            iss >> key >> numCpu;
        }
        else if (line.find("scheduler") != std::string::npos) {
            iss >> key >> scheduler;
            scheduler = scheduler.substr(1, scheduler.size() - 2); // Remove the quotes around the string
        }
        else if (line.find("quantum-cycles") != std::string::npos) {
            iss >> key >> quantumCycles;
        }
        else if (line.find("batch-process-freq") != std::string::npos) {
            iss >> key >> batchProcessFreq;
        }
        else if (line.find("min-ins") != std::string::npos) {
            iss >> key >> minInstructions;
        }
        else if (line.find("max-ins") != std::string::npos) {
            iss >> key >> maxInstructions;
        }
        else if (line.find("delay-per-exec") != std::string::npos) {
            iss >> key >> delayPerExecution;
        }
    }

    // Close the file
    infile.close();

    //map<string, functionHolder> functionMap;
    //functionMap["initialize"] = functionHolder{ initialize };
    //functionMap["screen"] = functionHolder{ screen };
    //functionMap["scheduler-test"] = functionHolder{ schedulerTest };
    //functionMap["scheduler-stop"] = functionHolder{ schedulerStop };
    //functionMap["report-util"] = functionHolder{ reportUtil };
    //functionMap["clear"] = functionHolder{ clear };
    
    ConsoleManager::initialize();
    ConsoleManager::getInstance()->setNumRangeOfInstructions(minInstructions, maxInstructions);
    FCFSScheduler::initialize();

    // Create CPU cores dynamically based on numCPU and start threads
    for (int i = 0; i < numCpu; ++i) {
        // Create a new CPUCore and store it in the vector
        std::shared_ptr<CPUCore> core = std::make_shared<CPUCore>(i);
        cpuCores.push_back(core);

        // Create a new thread for each CPUCore::runCPU and store it in the vector
        coreThreads.emplace_back(&CPUCore::runCPU, core);
    }

    for (const auto& core : cpuCores) {
        FCFSScheduler::getInstance()->addCPUCore(core);
    }

    std::thread fcfsThread(&FCFSScheduler::runFCFS, FCFSScheduler::getInstance());
    
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

    FCFSScheduler::getInstance()->stop();

    for (const auto& core : cpuCores) {
        core->stop();
    }

    fcfsThread.join();
    for (std::thread& t : coreThreads) {
        if (t.joinable()) {
            t.join();
        }
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