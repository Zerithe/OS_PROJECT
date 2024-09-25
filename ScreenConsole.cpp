#include "ScreenConsole.h"
#include <iostream>
#include <Windows.h>
#include "KeyboardHandler.h"
using namespace std;

ScreenConsole::ScreenConsole(String name) : AConsole(name)
{
    this->name = name;
}

void ScreenConsole::onEnabled()
{
    if (enabled == false)
    {
        cout << "Screen Layout" << endl;
    }
    enabled = true;

}

void ScreenConsole::display()
{
    printProcessData();
    string command, option, name;
    onEnabled();
    cout << "root:\>";
    getline(cin, command);
    if (command == "exit") {
        exited = true;
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

void ScreenConsole::printProcessData() const
{

}