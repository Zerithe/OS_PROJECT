#include "ScreenConsole.h"
#include <iostream>
#include <Windows.h>

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
    onEnabled();
}

void ScreenConsole::process()
{

}


bool ScreenConsole::hasExited()
{
    return exited;
}
