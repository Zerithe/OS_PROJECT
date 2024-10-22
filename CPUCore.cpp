#include "CPUCore.h"
#include <memory>
#include <thread>

CPUCore::CPUCore(int id, int delayPerExecution)
{
	this->id = id;
	this->delayPerExecution = delayPerExecution;
}

void CPUCore::runCPU()
{
	while (this->running)
	{
		if (this->process != nullptr) { // && this->cpuCycle % (delayPerExecution + 1) == 0
			if (this->cpuCycle % (delayPerExecution + 1) == 0)
				this->process->executeCurrentCommand();
			if (this->process->isFinished()) {
				this->finishedProcess = true;
			}
			else if (this->cpuCycle % (delayPerExecution + 1) == 0) {
				this->process->moveToNextLine();
			}
			this->cpuCycle++;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void CPUCore::registerProcess(std::shared_ptr<Process> process)
{
	this->process = process;
	process->setCoreId(this->id);
	process->setTimeStarted();
}

bool CPUCore::containsProcess() const
{
	return (this->process != nullptr);
}

std::shared_ptr<Process> CPUCore::getProcess()
{
	return this->process;
}

int CPUCore::getId()
{
	return this->id;
}

bool CPUCore::getIsFinished()
{
	return this->finishedProcess;
}

void CPUCore::deallocateCPU()
{
	this->process = nullptr;
	this->finishedProcess = false;
	this->cpuCycle = 0;
}

void CPUCore::stop()
{
	this->running = false;
}




