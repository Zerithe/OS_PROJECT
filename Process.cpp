#include "Process.h"
#include <string>
#include "PrintCommand.h"

Process::Process(int pid, std::string name)
{
	this->pid = pid;
	this->name = name;
	this->commandCounter = 0;
	this->currentState = ProcessState::READY;
}

void Process::addCommand(ICommand::CommandType commandType)
{
	if (commandType == ICommand::CommandType::IO)
	{
	}
	else if (commandType == ICommand::CommandType::PRINT)
	{
		std::string toPrint = "Hello World!";
		const std::shared_ptr<ICommand> command = std::make_shared<PrintCommand>(this->pid, toPrint);
		this->commandList.push_back(command);
	}
}

void Process::executeCommand() const
{
	this->commandList[this->commandCounter]->execute();
}

void Process::moveToNextLine()
{
	this->commandCounter++;
}

bool Process::isFinished() const
{
	return this->commandCounter == this->commandList.size();
}

int Process::getCurrentTime() const
{
	return 0;
}

int Process::getLinesofCode() const
{
	return 0;
}

int Process::getCommandCounter() const
{
	return 0;
}

int Process::getPid() const
{
	return this->pid;
}

std::string Process::getName() const
{
	return std::string();
}

void Process::test_generateRandomCommands(int limit)
{
}
