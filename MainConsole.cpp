#include "MainConsole.h"
#include <iostream>
#include <Windows.h>

using namespace std;

MainConsole::MainConsole(String name) : AConsole(name)
{
	this->name = name;
}

void MainConsole::onEnabled()
{
    if (enabled == false)
    {
        clear();
    }
    enabled = true;

}

void MainConsole::display()
{
    string input;
    onEnabled();
    cout << "enter a command: ";
    cin >> input;
    if (input == "clear") {
        clear();
    }
    else if (input == "exit") {
        running = false;
    }
    else {
        cout << "Command not found";
    }

}

void MainConsole::process()
{
    
}

void MainConsole::clear()
{
    cout << "\033[2J\033[1;1H";
    headerPrint();
}

void MainConsole::headerPrint()
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

bool MainConsole::isRunning()
{
    return running;
}
