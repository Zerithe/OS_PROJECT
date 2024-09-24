#include "MainConsole.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "KeyboardHandler.h"

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
    string command, option, name;
    onEnabled();
    cout << "enter a command: ";
    getline(cin, command);
    if (command == "clear") {
        clear();
    }
    else if (command == "exit") {
        exited = true;
    }
    else if (command.substr(0, 9) == "screen -r") {
        name = command.substr(10);

        if (!name.empty())
        {
            cout << "Screen name: " << name << endl;
        }
        else
        {
            cout << "Invalid command" << endl;
        }
    }
    else if (command.substr(0, 9) == "screen -s") {
        name = command.substr(10);

        if (!name.empty())
        {
            cout << "Screen name: " << name << endl;
        }
        else
        {
            cout << "Invalid command" << endl;
        }
    }
    else {
        cout << "Command not found" << endl;
    }

}

void MainConsole::process()
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

bool MainConsole::hasExited()
{
    return exited;
}
