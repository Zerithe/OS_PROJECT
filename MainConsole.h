#pragma once
#include "AConsole.h"

class MainConsole : public AConsole
{
public:
	MainConsole(String name);

	void onEnabled() override;
	void offEnabled() override;
	void display() override;
	void process() override;
	bool hasExited() override;

	void clear();
	AConsole::String getStringToRegister();
	AConsole::String getStringToRead();
	bool getShowListOfProcesses();

	
private:
	void headerPrint();
	bool enabled = false;
	bool exited = false;
	bool showListOfProcesses = false;
	AConsole::String stringToRegister = "";
	AConsole::String stringToRead = "";
};

