#pragma once
#include <iostream>
#include <string>
#include "ICommand.h"
#include <vector>

class Process
{
public:
	enum ProcessState {
		READY,
		RUNNING,
		WAITING,
		FINISHED
	};

	Process(int pid, std::string name);
	void addCommand(ICommand::CommandType commandType);
	void executeCommand() const;
	void moveToNextLine();

	bool isFinished() const;
	int getCurrentTime() const;
	int getLinesofCode() const;
	int getCommandCounter() const;
	int getPid() const;
	std::string getName() const;
	
	void test_generateRandomCommands(int limit);
private:
	int pid;
	std::string name;
	typedef std::vector<std::shared_ptr<ICommand>> CommandList;
	CommandList commandList;

	ProcessState state;
	int commandCounter;
	int cpuCoreID = -1;
	ProcessState currentState;
};

