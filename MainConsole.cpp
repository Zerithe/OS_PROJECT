#include "MainConsole.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "KeyboardHandler.h"
#include <thread>

using namespace std;

std::atomic<bool> inputProcessed = false;
static constexpr int POLLING_DELAY = 5;

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

void MainConsole::offEnabled()
{
    this->enabled = false;
}

void MainConsole::display()
{
    onEnabled();
    cout << "enter a command: ";
    if (inputProcessed == true) {
        this->stringToRead.clear();
        this->stringToRegister.clear();
        string option, name;
        if (command == "clear" || command == "cls") {
            clear();
        }
        else if (command == "exit") {
            exited = true;
        }
        else if (command.substr(0, 9) == "screen -r") {
            name = command.substr(10);

            if (!name.empty())
            {
                this->stringToRead = name;
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
                this->stringToRegister = name;
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
	inputProcessed = false;

	//std::this_thread::sleep_for(std::chrono::milliseconds(POLLING_DELAY));
}

void MainConsole::process()
{
	static AConsole::String input;
    if (enabled) {
        if (_kbhit())
        {
			char ch = _getch();
            if (ch == '\r') {
				inputProcessed = true;
				command = input;
				input.clear();
			}
			else if (ch == '\b') {
				if (input.length() > 0) {
					input.pop_back();
					cout << "\b \b";
				}
			}
			else {
				input += ch;
				cout << ch;
            }
        }
    }

	//std::this_thread::sleep_for(std::chrono::milliseconds(POLLING_DELAY));
}

void MainConsole::clear()
{
    cout << "\033[2J\033[1;1H";
    headerPrint();
}

AConsole::String MainConsole::getStringToRegister()
{
    return this->stringToRegister;
}

AConsole::String MainConsole::getStringToRead()
{
    return this->stringToRead;
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
