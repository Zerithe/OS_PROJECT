#pragma once
#include "AConsole.h"

class MainConsole : public AConsole
{
public:
	MainConsole(String name);

	void onEnabled() override;
	void display() override;
	void process() override;

	void clear();

	
private:
	void headerPrint();
	bool enabled = false;
};

