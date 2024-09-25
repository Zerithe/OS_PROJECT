#pragma once
#include "AConsole.h"

class MainConsole : public AConsole
{
public:
	MainConsole(String name);

	void onEnabled() override;
	void display() override;
	void process() override;
	bool hasExited() override;

	void clear();
	AConsole::String getStringToRegister();
	AConsole::String getStringToRead();

	
private:
	void headerPrint();
	bool enabled = false;
	bool exited = false;
	AConsole::String stringToRegister = "";
	AConsole::String stringToRead = "";
};

