#include "MainConsole.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "KeyboardHandler.h"
#include <iomanip>

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

void MainConsole::offEnabled()
{
    this->enabled = false;
}

void MainConsole::display()
{
    this->stringToRead.clear();
    this->stringToRegister.clear();
    string command, option, name;
    onEnabled();
    cout << "enter a command: ";
    getline(cin, command);
    if (command == "clear" || command == "cls") {
        clear();
    }
    else if (command == "exit") {
        exited = true;
    }
    else if (command == "nvidia-smi") {
		displaySmi();
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

int MainConsole::getConsoleWidth()
{
	HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

	if (GetConsoleScreenBufferInfo(hconsole, &consoleInfo))
	{
		return consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
	}
	else
	{
		return 80;
	}
}

void MainConsole::displaySmi()
{
	int consoleWidth = getConsoleWidth();
    int halfConsoleWidth = consoleWidth / 1.5;

    //header
    AConsole::String dateTime = getCurrentDateTime();
    int headerWidth = halfConsoleWidth - 2;
    AConsole::String nvidiaSmiHeader = " NVIDIA-SMI 511.69       Driver Version: 511.69       CUDA Version: 11.6 ";
    int colWidth = headerWidth / 3;

    std::cout << dateTime << "\n";
    std::cout << "+" << AConsole::String(headerWidth, '-') << "--+\n";
    std::cout << "|" << std::setw((headerWidth + nvidiaSmiHeader.size()) / 2) << nvidiaSmiHeader << std::setw((headerWidth - nvidiaSmiHeader.size()) / 2) << "     |\n";
    std::cout << "+" << std::string(colWidth, '-') << "+" << std::string(colWidth, '-') << "+" << std::string(colWidth, '-') << "+\n";
    std::cout << "| " << adjustStringToWidth("GPU  Name   TCC/WDDM", colWidth - 2)
        << " | " << adjustStringToWidth("Bus-Id        Disp.A", colWidth - 2)
        << " | " << adjustStringToWidth("Volatile Uncorr. ECC", colWidth - 2) << " |\n";
    std::cout << "| " << adjustStringToWidth("Fan  Temp  Perf  Pwr:Usage/Cap", colWidth - 2)
        << " | " << adjustStringToWidth("Memory-Usage", colWidth - 2)
        << " | " << adjustStringToWidth("GPU-Util  Compute M.", colWidth - 2) << " |\n";
    std::cout << "| " << adjustStringToWidth("", colWidth - 2)
        << " | " << adjustStringToWidth("", colWidth - 2)
        << " | " << adjustStringToWidth("MIG M.", colWidth - 2) << " |\n";
    std::cout << "+" << std::string(colWidth, '=') << "+" << std::string(colWidth, '=') << "+" << std::string(colWidth, '=') << "+\n";


    //GPU info
    std::cout << "| " << adjustStringToWidth("0  NVIDIA GeForce GTX 1080", colWidth - 2)
        << " | " << adjustStringToWidth("00000000:01:00.0  Off", colWidth - 2)
        << " | " << adjustStringToWidth("N/A", colWidth - 2) << " |\n";
    std::cout << "| " << adjustStringToWidth("N/A   52C    P8    3W / N/A", colWidth - 2)
        << " | " << adjustStringToWidth("0MiB / 2048MiB", colWidth - 2)
        << " | " << adjustStringToWidth("0%       Default", colWidth - 2) << " |\n";
    std::cout << "+" << std::string(colWidth, '-') << "+" << std::string(colWidth, '-') << "+" << std::string(colWidth, '-') << "+\n";
    std::cout << endl;
    std::cout << endl;

    //process
    const int fixedGPUColWidth = 6;
    const int fixedPIDColWidth = 6;
    const int fixedTypeColWidth = 5;
    const int fixedMemUsageColWidth = 8;
    AConsole::String processes[5] =
    {
        "C:\\User\\Chrome\\Binaries\\Win64\\Chrome.exe",
        "F:\\Applications\\Discord\\Bin\\Discord.exe",
        "C:\\chess.exe",
        "D:\\reallyrealllyveryyylongprocessname.exe.bin.cpp.java.py.sys",
        "J:\\Games\\.minecraft\\minecraft.exe"
    };
    int pids[5] = {1234, 2345, 3456, 4567, 5678};
    AConsole::String memoryUsage[5] = {"N/A", "N/A", "N/A", "200MiB", "512MiB"};
	int dynamicColWidth = halfConsoleWidth - fixedGPUColWidth - fixedPIDColWidth - fixedTypeColWidth - fixedMemUsageColWidth - 17;
    if (dynamicColWidth < 0)
    {
		dynamicColWidth = 0;
    }

    std::cout << "+" << std::string(halfConsoleWidth - 2, '-') << "--+\n";
	std::cout << "|" << adjustStringToWidth("Processes:", halfConsoleWidth - 2) << "  |\n";
    std::cout << "+" << std::string(halfConsoleWidth - 2, '-') << "--+\n";
    std::cout << "|" << adjustStringToWidth(" GPU    GI     CI    PID   Type Process Name                         GPU Memory", halfConsoleWidth) <<"|\n";
    std::cout << "|" << adjustStringToWidth("        ID     ID                                                    Usage", halfConsoleWidth) <<"|\n";
    std::cout << "+" << std::string(halfConsoleWidth - 2, '=') << "==+\n";


    for (int i = 0; i < 5; i++)
    {
		AConsole::String truncatedProcessName = adjustStringToWidth(processes[i], dynamicColWidth);
        std::cout << "| " << " 0 "
            << " " << std::setw(fixedGPUColWidth) << "N/A"
            << " " << std::setw(fixedGPUColWidth) << "N/A"
            << " " << std::setw(fixedPIDColWidth) << pids[i]
            << " " << std::setw(fixedTypeColWidth) << "C+G"
            << " " << truncateString(processes[i], dynamicColWidth)
            << " " << std::setw(fixedMemUsageColWidth) << memoryUsage[i]
            << " |\n";

    }
    std::cout << "+" << std::string(halfConsoleWidth - 2, '-') << "--+\n";
}

AConsole::String MainConsole::getCurrentDateTime()
{
    time_t currTime = time(0);
    char buffer[80];
    struct tm timeinfo;
    localtime_s(&timeinfo, &currTime);
    strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", &timeinfo);
    return AConsole::String(buffer);

}

AConsole::String MainConsole::truncateString(const AConsole::String& str, size_t width)
{
    if (str.size() > width) 
    {
        return "..." + str.substr(str.size() - (width - 3));
    }
    return str + std::string(width - str.size(), ' ');
}

AConsole::String MainConsole::adjustStringToWidth(const AConsole::String& str, size_t width)
{
    if (str.size() > width) {
        return str.substr(0, width - 3) + "...";  
    }
    return str + std::string(width - str.size(), ' ');
}

bool MainConsole::hasExited()
{
    return exited;
}

