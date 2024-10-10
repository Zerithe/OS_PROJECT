#include "ICommand.h"
#include <chrono>
#include <iostream>

ICommand::ICommand(int pid, CommandType commandType)
{
    this->pid = pid;
    this->commandType = commandType;
}

ICommand::CommandType ICommand::getCommandType() const
{
    return this->commandType;
}

void ICommand::execute(std::ofstream& outfile, int coreID, std::string processName)
{
    std::string dateNow = getDateNow();
    if (outfile.is_open()) {
        outfile << "(" << dateNow << ")" << " Core:" << coreID << " Hello world from " << processName << "\n";
    }
    else {
        std::cerr << "File is not open for writing.\n";
    }
}

std::string ICommand::getDateNow()
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
    return oss.str();
}
