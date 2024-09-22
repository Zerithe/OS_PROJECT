#include "MainConsole.h"
#include <iostream>
#include <Windows.h>



MainConsole::MainConsole(String name) : AConsole(name)
{
	this->name = name;
}

void MainConsole::onEnabled()
{
    if (enabled == false)
    {
        headerPrint();
    }
    enabled = true;

}

void MainConsole::display()
{
    onEnabled();
}

void MainConsole::process()
{
    
}

void MainConsole::clear()
{
    std::cout << "\033[2J\033[1;1H";
    headerPrint();
}

void MainConsole::headerPrint()
{
    std::cout << "   ___________ ____  ____  _____________  __\n";
    std::cout << "  / ____/ ___// __ \\/ __ \\/ ____/ ___/\\ \\/ /\n";
    std::cout << " / /    \\__ \\/ / / / /_/ / __/  \\__ \\  \\  /\n";
    std::cout << "/ /___ ___/ / /_/ / ____/ /___ ___/ /  / /\n";
    std::cout << "\\____//____/\\____/_/   /_____//____/  /_/\n";
    std::cout << "\n";

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    std::cout << "Hello, Welcome to CSOPESY commandline!\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    std::cout << "Type 'exit' to quit, 'clear' to clear the screen \n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
