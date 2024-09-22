#include "ConsoleManager.h"
#include "MainConsole.h"

ConsoleManager* ConsoleManager::sharedInstance = nullptr;
AConsole::String input;

ConsoleManager::ConsoleManager()
{
	this->running = true;

	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>("MAIN_CONSOLE");

	this->consoleTable[MAIN_CONSOLE] = mainConsole;

	this->switchConsole(MAIN_CONSOLE);

	std::cout << "constructed" << std::endl;
}


ConsoleManager* ConsoleManager::getInstance()
{
	if(sharedInstance == nullptr)
	{
		sharedInstance = new ConsoleManager();
	}

	return sharedInstance;
}

void ConsoleManager::initialize()
{
	getInstance();
}

void ConsoleManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

void ConsoleManager::drawConsole() const
{
	this->currentConsole->display();
	
}

void ConsoleManager::process() const
{
	this->currentConsole->process();
}

void ConsoleManager::switchConsole(AConsole::String consoleName)
{
	this->currentConsole = consoleTable[consoleName];
}

void ConsoleManager::returnToPreviousConsole()
{
}

void ConsoleManager::exitApplication()
{
}

bool ConsoleManager::isRunning() const
{
	return running;
}

HANDLE ConsoleManager::getConsole() const
{
	return consoleHandle;
}

void ConsoleManager::setCursorPosition(int posX, int posY) const
{
}