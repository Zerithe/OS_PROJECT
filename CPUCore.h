#pragma once
#include "Process.h"
#include "CPUCore.h"
#include <memory>
class CPUCore
{
public:
	CPUCore(int id, int delayPerExecution);
	~CPUCore() = default;

	void runCPU();
	void registerProcess(std::shared_ptr<Process> process);
	bool containsProcess() const;
	std::shared_ptr<Process> getProcess();
	int getId();
	bool getIsFinished();
	void deallocateCPU();
	void stop();
private:
	int id;
	std::shared_ptr<Process> process = nullptr;
	bool finishedProcess = false;
	bool running = true;
	int cpuCycle = 0;
	int delayPerExecution;
};

