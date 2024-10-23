#pragma once
#include <Windows.h>
#include <vector>
#include <memory>
#include <queue>
#include <iostream>
#include "Process.h"
#include "CPUCore.h"
class FCFSScheduler
{
public:
	typedef std::queue<std::shared_ptr<Process>> ReadyQueue;
	typedef std::vector<std::shared_ptr<CPUCore>> coreList;
	typedef std::vector< std::shared_ptr<Process>> FinishedList;
	void runFCFS();
	void addCPUCore(std::shared_ptr<CPUCore> cpuCore);
	void addProcess(std::shared_ptr<Process> process);
	void showListOfProcesses();
	void printListOfProcesses();
	void stop();
	static FCFSScheduler* getInstance();
	static void initialize();
	static void destroy();
private:
	FCFSScheduler();
	~FCFSScheduler() = default;
	FCFSScheduler(FCFSScheduler const&) {}; //copy operator
	FCFSScheduler& operator=(FCFSScheduler const&) {}; //assignment operator
	static FCFSScheduler* sharedInstance;

	ReadyQueue readyQueue;
	coreList cores;
	FinishedList finishedList;
	bool running = true;
	int coresUsed = 0;
};

