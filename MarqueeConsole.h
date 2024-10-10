#pragma once
#include "AConsole.h"
class MarqueeConsole : public AConsole
{
public:
	MarqueeConsole(String name);

	void onEnabled() override;
	void offEnabled() override;
	void display() override;
	void process() override;
	bool hasExited() override;
};

