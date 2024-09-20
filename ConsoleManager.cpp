#include "ConsoleManager.h"

ConsoleManager* ConsoleManager::getInstance()
{
	return nullptr;
}

void ConsoleManager::initialize()
{
	std::cout << "test" << std::endl;
}

void ConsoleManager::destroy()
{
}

void ConsoleManager::drawConsole() const
{
}

void ConsoleManager::process() const
{
}

void ConsoleManager::switchConsole(AConsole::String name)
{
}

void ConsoleManager::returnToPreviousConsole()
{
}

void ConsoleManager::exitApplication()
{
}

bool ConsoleManager::isRunning() const
{
	return false;
}

HANDLE ConsoleManager::getConsole() const
{
	return HANDLE();
}

void ConsoleManager::setCursorPosition(int posX, int posY) const
{
}

ConsoleManager::ConsoleManager()
{
	this->running = true;

	std::cout << "constructed" << std::endl;
}
