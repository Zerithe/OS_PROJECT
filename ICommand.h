#pragma once


class ICommand
{
public:
	enum CommandType 
	{
		IO,
		PRINT
	};

	ICommand(int pid, CommandType type);
	CommandType getCommandType();
	virtual void execute() = 0;
protected:
	int pid;
	CommandType commandType;
};

inline ICommand::ICommand(int pid, CommandType type)
{
	this->pid = pid;
	this->commandType = type;
}

inline ICommand::CommandType ICommand::getCommandType()
{
	return this->commandType;
}