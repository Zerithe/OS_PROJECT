#pragma once
#include <fstream>
class ICommand
{
public:
	enum CommandType
	{
		IO,
		PRINT
	};

	ICommand(int pid, CommandType commandType);
	CommandType getCommandType() const;
	void execute(std::ofstream& outfile, int coreID, std::string processName);
	std::string getDateNow();

protected:
	int pid;
	CommandType commandType;
};


