#pragma once
#include <memory>
#include "AConsole.h"
#include <Windows.h>
#include <unordered_map>
#include <iostream>
#include "ScreenConsole.h"
#include "Process.h"


const AConsole::String MAIN_CONSOLE = "MAIN_CONSOLE";
const AConsole::String SCHEDULING_CONSOLE = "SCHEDULING_CONSOLE";
const AConsole::String TEST_CONSOLE = "TEST_CONSOLE";
const AConsole::String SCREEN_CONSOLE = "SCREEN_CONSOLE";

class ConsoleManager
{
public:
	typedef std::unordered_map<AConsole::String, std::shared_ptr<AConsole>> ConsoleTable;

	static ConsoleManager* getInstance();
	static void initialize();
	static void destroy();

	void drawConsole();
	void process() const;
	void switchConsole(AConsole::String consoleName);
	void returnToPreviousConsole();
	void exitApplication();
	void registerConsole(std::shared_ptr<ScreenConsole> screenRef);
	void setNumRangeOfInstructions(int minInstructions, int maxInstructions);
	bool isRunning() const;
	std::shared_ptr<Process> getCreatedProcess();
	bool getShowListOfProcesses();

	HANDLE getConsole() const;

	void setCursorPosition(int posX, int posY) const;

private:
	ConsoleManager();
	~ConsoleManager() = default;
	ConsoleManager(ConsoleManager const&) {}; //copy operator
	ConsoleManager& operator=(ConsoleManager const&) {}; //assignment operator
	static ConsoleManager* sharedInstance;

	ConsoleTable consoleTable;
	std::shared_ptr<AConsole> currentConsole;
	std::shared_ptr<AConsole> previousConsole;

	HANDLE consoleHandle;
	bool running = true;
	std::shared_ptr<Process> createdProcess = nullptr;
	bool showListOfProcesses = false;
	int minInstructions;
	int maxInstructions;
};

