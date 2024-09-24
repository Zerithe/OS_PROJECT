#pragma once
#include "AConsole.h"

class ScreenConsole : public AConsole
{
public:
	ScreenConsole(String name);

	void onEnabled() override;
	void display() override;
	void process() override;
	bool hasExited() override;

private:
	bool enabled = false;
	bool exited = false;
};

