#include "AConsole.h"
#include <string>

AConsole::AConsole(String name)
{
	this->name = name;
}

AConsole::String AConsole::getName()
{
	return this->name;
}
