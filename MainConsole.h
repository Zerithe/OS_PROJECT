#pragma once
#include "AConsole.h"

class MainConsole : public AConsole
{
public:
	MainConsole(String name);

	void onEnabled() override;
	void offEnabled() override;
	void display() override;
	void process() override;
	bool hasExited() override;

	void clear();
	AConsole::String getStringToRegister();
	AConsole::String getStringToRead();
	bool getShowListOfProcesses();
	bool getPrintListOfProcesses();
	bool getStartSchedulerTest();
	bool getStopSchedulerTest();
	bool getProcessSMI();
	bool getVMStat();

	
private:
	void headerPrint();
	bool enabled = false;
	bool exited = false;
	bool showListOfProcesses = false;
	bool printListOfProcesses = false;
	bool startSchedulerTest = false;
	bool stopSchedulerTest = false;
	bool process_smi = false;
	bool vmstat = false;
	AConsole::String stringToRegister = "";
	AConsole::String stringToRead = "";
};

