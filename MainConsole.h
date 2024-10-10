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

	
private:
	void headerPrint();
	int getConsoleWidth();
	void displaySmi();
	AConsole::String getCurrentDateTime();
	AConsole::String truncateString(const AConsole::String& str, size_t width);
	AConsole::String adjustStringToWidth(const AConsole::String& str, size_t width);

	bool enabled = false;
	bool exited = false;
	AConsole::String stringToRegister = "";
	AConsole::String stringToRead = "";
};

