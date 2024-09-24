#pragma once
#include "AConsole.h"

class MainConsole : public AConsole
{
public:
	MainConsole(String name);

	void onEnabled() override;
	void display() override;
	void process() override;
	bool isRunning() override;

	void clear();

	void headerPrint();
private:
	bool enabled = false;
	bool running = true;
};

