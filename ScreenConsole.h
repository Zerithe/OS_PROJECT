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
	void printProcessData() const;

	bool enabled = false;
	bool exited = false;
	String creationTime;       // String to hold the creation date and time

	void initializeCreationTime();
};

