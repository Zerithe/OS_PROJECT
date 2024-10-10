#pragma once
#include "Process.h"
#include "CPUCore.h"
#include <memory>
class CPUCore
{
public:
	CPUCore(int id);
	~CPUCore() = default;

	void runCPU();
	void registerProcess(std::shared_ptr<Process> process);
	bool containsProcess() const;
	std::shared_ptr<Process> getProcess();
	int getId();
	bool getIsFinished();
	void deallocateCPU();
private:
	int id;
	std::shared_ptr<Process> process = nullptr;
	bool finishedProcess = false;
};

