#include "ScreenConsole.h"
#include <iostream>
#include <Windows.h>
#include "KeyboardHandler.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <random>
using namespace std;

ScreenConsole::ScreenConsole(String name) : AConsole(name)
{
    this->name = name;
    std::random_device rd;  // Obtain a random seed from hardware
    std::mt19937 gen(rd()); // Seed the generator

    // Define the range of random numbers (e.g., 1 to 100)
    std::uniform_int_distribution<> distr(1, 100);

    // Generate a random integer
    int random_number = distr(gen);
    this->linkedProcess = std::make_shared<Process>(random_number, name);
    initializeCreationTime();
}

void ScreenConsole::onEnabled()
{
    if (enabled == false)
    {
        //cout << "Screen Layout" << endl; temporarily commented to make display similar after using cls
        printProcessData();
    }
    enabled = true;

}

void ScreenConsole::offEnabled()
{
    this->enabled = false;
}

void ScreenConsole::display()
{
    exited = false;
    string command, option, name;
    onEnabled();
    cout << "root:\> ";
    getline(cin, command);
    if (command == "exit") {
        offEnabled();
        exited = true;
    }
    else if (command == "clear" || command == "cls")
    {
        cout << "\033[2J\033[1;1H";
        printProcessData();
    }
    else if (command == "process-smi")
    {
        printProcessData();
    }
    else
    {
		cout << "Invalid command" << endl;
    }
}

void ScreenConsole::process()
{
    KeyboardHandler keyboardHandler;
    if (enabled) {
        if (_kbhit())
        {
            char key = _getch();
            char input[256];

            if (GetAsyncKeyState(key) & 0x8000)
            {
                keyboardHandler.OnKeyDown(key);
            }
            else
            {
                keyboardHandler.OnKeyUp(key);
            }

        }
    }
}


bool ScreenConsole::hasExited()
{
    return exited;
}

std::shared_ptr<Process> ScreenConsole::getProcess()
{
    return this->linkedProcess;
}

void ScreenConsole::printProcessData() const
{
    cout << "Process Name: " << this->name << endl;
    cout << this->linkedProcess->getCommandCounter() << "/" << this->linkedProcess->getTotalInstructions() << endl;
    cout << "Creation time: " << this->creationTime << endl;
}

void ScreenConsole::initializeCreationTime()
{
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);

    // Convert to local time and format it
    std::tm local_tm;

    localtime_s(&local_tm, &now_c);

    // Use a string stream to format the date and time
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%m/%d/%Y, %I:%M:%S %p");

    // Store formatted time in creationTime
    creationTime = oss.str();
}
