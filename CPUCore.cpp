#include "CPUCore.h"
#include <memory>
#include <thread>

CPUCore::CPUCore(int id)
{
	this->id = id;
}

void CPUCore::runCPU()
{
	while (true)
	{
		if (this->process != nullptr) {
			this->process->executeCurrentCommand();
			if (this->process->isFinished()) {
				this->finishedProcess = true;
			}
			else {
				this->process->moveToNextLine();
			}
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
}




