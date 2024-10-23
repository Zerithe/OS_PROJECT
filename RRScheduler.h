#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include <queue>
#include <iostream>
#include "Process.h"
#include "CPUCore.h"
class RRScheduler
{
public:
	typedef std::queue<std::shared_ptr<Process>> ReadyQueue;
	typedef std::vector<std::shared_ptr<CPUCore>> coreList;
	typedef std::vector< std::shared_ptr<Process>> FinishedList;
	void runRR();
	void addCPUCore(std::shared_ptr<CPUCore> cpuCore);
	void addProcess(std::shared_ptr<Process> process);
	void showListOfProcesses();
	void printListOfProcesses();
	void stop();
	static RRScheduler* getInstance();
	static void initialize();
	static void destroy();
private:
	RRScheduler();
	~RRScheduler() = default;
	RRScheduler(RRScheduler const&) {}; //copy operator
	RRScheduler& operator=(RRScheduler const&) {}; //assignment operator
	static RRScheduler* sharedInstance;

	ReadyQueue readyQueue;
	coreList cores;
	FinishedList finishedList;
	bool running = true;
	int coresUsed = 0;
};

