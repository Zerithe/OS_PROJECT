#include "Process.h"
#include <memory>
#include <fstream>
#include <chrono>

Process::Process(int pid, std::string name)
{
    this->pid = pid;
    this->name = name;
    addCommand(ICommand::CommandType::PRINT);
    this->totalInstructions = this->commandList.size();
}

void Process::addCommand(ICommand::CommandType command)
{
    if (command == ICommand::CommandType::PRINT) {
        for (int i = 0; i < 100; i++) {
            this->commandList.push_back(std::make_shared<ICommand>(i, ICommand::CommandType::PRINT));
        }
    }
}

void Process::executeCurrentCommand()
{
    if (this->commandCounter == 0)
    {
        this->currentState = ProcessState::RUNNING;
        std::ofstream outfile(this->name + ".txt");
        if (outfile.is_open()) {
            outfile << "Process name: " << this->name << "\n";
            outfile << "Logs:" << "\n" << "\n";
            this->commandList.at(this->commandCounter)->execute(outfile, this->cpuCoreId, this->name);
            outfile.close();
        }
    }
    else if (this->commandCounter > 0 && this->commandCounter < this->totalInstructions) {
        std::ofstream appendfile(this->name + ".txt", std::ios::app);
        if (appendfile.is_open()) {
            this->commandList.at(this->commandCounter)->execute(appendfile, this->cpuCoreId, this->name);
            appendfile.close();
        }
    }
    else {
        this->currentState = ProcessState::FINISHED;
        this->timeFinished = getDateNow();
    }

}

void Process::moveToNextLine()
{
    this->commandCounter++;
}

void Process::setCoreId(int coreID)
{
    this->cpuCoreId = coreID;
}

bool Process::isFinished() const
{
    return (this->currentState == ProcessState::FINISHED);
}

std::string Process::getDateNow()
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

std::string Process::getTimeFinished()
{
    return this->timeFinished;
}

std::string Process::getTimeStarted()
{
    return this->timeStarted;
}

void Process::setTimeStarted()
{
    this->timeStarted = getDateNow();
}

int Process::getCommandCounter() const
{
    return this->commandCounter;
}

int Process::getTotalInstructions() const
{
    return this->totalInstructions;
}

std::string Process::getName()
{
    return this->name;
}
