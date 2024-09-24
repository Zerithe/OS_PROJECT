#include "ConsoleManager.h"
#include "MainConsole.h"
#include "ScreenConsole.h"

ConsoleManager* ConsoleManager::sharedInstance = nullptr;
AConsole::String input;

ConsoleManager::ConsoleManager()
{
	this->running = true;

	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>("MAIN_CONSOLE");
	const std::shared_ptr<ScreenConsole> screenConsole = std::make_shared<ScreenConsole>("SCREEN_CONSOLE");

	this->consoleTable[MAIN_CONSOLE] = mainConsole;
	this->consoleTable[SCREEN_CONSOLE] = screenConsole;

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
	if (currentConsole == consoleTable.at(MAIN_CONSOLE) && currentConsole->hasExited()) {
		const_cast<ConsoleManager*>(this)->exitApplication();
	}
}

void ConsoleManager::process() const
{
	this->currentConsole->process();
}

void ConsoleManager::switchConsole(AConsole::String consoleName)
{
	if(this->consoleTable.contains(consoleName))
	{
		std::cout << "\033[2J\033[1;1H";
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[consoleName];
		this->currentConsole->onEnabled();
	}
	else
	{
		std::cout << "Console not found" << std::endl;
	}
	this->currentConsole->onEnabled();
}

void ConsoleManager::returnToPreviousConsole()
{
	if (this->previousConsole != nullptr)
	{
		std::cout << "\033[2J\033[1;1H";
		this->currentConsole = this->previousConsole;
		this->previousConsole = nullptr;
	}
	else
	{
		std::cout << "No previous console" << std::endl;
	}
}

void ConsoleManager::exitApplication()
{
	this->running = false;
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