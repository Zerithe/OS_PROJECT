#pragma once
#include <memory>
#include <string>
#include "ICommand.h"
#include <vector>
class Process
{
public:
	Process(int pid, std::string name, int totalInstructions, int memoryRequired);
	~Process() = default;

	enum ProcessState
	{
		READY,
		RUNNING,
		WAITING,
		FINISHED
	};

	void addCommand(ICommand::CommandType command);
	void executeCurrentCommand();
	void moveToNextLine();
	void setCoreId(int coreID);
	bool isFinished() const;
	std::string getDateNow();
	std::string getTimeFinished();
	std::string getTimeStarted();
	int getProcessID() const;
	void setTimeStarted();
	int getCommandCounter() const;
	int getTotalInstructions() const;
	int getTotalMemoryRequired() const;

	std::string getName();
private:
	std::string name;
	int pid;
	typedef std::vector<std::shared_ptr<ICommand>> CommandList;
	CommandList commandList;
	int commandCounter = 0;
	int totalInstructions;
	int cpuCoreId = -1;
	std::string timeFinished;
	std::string timeStarted;
	ProcessState currentState = ProcessState::READY;
	int memoryRequired;
};

