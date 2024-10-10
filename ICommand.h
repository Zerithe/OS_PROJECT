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
	virtual void execute();
protected:
	int pid;
	CommandType commandType;
};

inline ICommand::CommandType ICommand::getCommandType()
{
	return this->commandType;
}