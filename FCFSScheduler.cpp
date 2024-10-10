#include "FCFSScheduler.h"
#include "Process.h"
#include "CPUCore.h"
#include <thread>

FCFSScheduler* FCFSScheduler::sharedInstance = nullptr;

FCFSScheduler::FCFSScheduler()
{
	
}



void FCFSScheduler::runFCFS()
{
	while (this->running) {
		for (std::shared_ptr<CPUCore> core: this->cores) {
			if (!core->containsProcess() && !this->readyQueue.empty()) {
				core->registerProcess(this->readyQueue.front());
				this->readyQueue.pop();
			}
			if (core->containsProcess() && core->getIsFinished()) {
				this->finishedList.push_back(core->getProcess());
				core->deallocateCPU();
			}
		}
	}
}

void FCFSScheduler::addCPUCore(std::shared_ptr<CPUCore> cpuCore)
{
	this->cores.push_back(cpuCore);
}

void FCFSScheduler::addProcess(std::shared_ptr<Process> process)
{
	this->readyQueue.push(process);
}

void FCFSScheduler::showListOfProcesses()
{
	for (int i = 0; i < 15; ++i) {
		std::cout << "-";
	}
	std::cout << std::endl;
	std::cout << "Running processes:" << std::endl;
	for (std::shared_ptr<CPUCore> core : this->cores) {
		if (core->containsProcess()) {
			std::cout << core->getProcess()->getName() << " " << core->getProcess()->getTimeStarted() << " Core: " << core->getId() << " " << core->getProcess()->getCommandCounter() << " / " << core->getProcess()->getTotalInstructions() << std::endl;
		}
	}
	std::cout << "Finished processes:" << std::endl;
	for (std::shared_ptr<Process> process : this->finishedList) {
		std::cout << process->getName() << " " << process->getTimeFinished() << " Finished " << process->getTotalInstructions() << " / " << process->getTotalInstructions() << std::endl;
	}
	
}

void FCFSScheduler::stop()
{
	this->running = false;
}

FCFSScheduler* FCFSScheduler::getInstance()
{
	if (sharedInstance == nullptr)
	{
		sharedInstance = new FCFSScheduler();
	}

	return sharedInstance;
}

void FCFSScheduler::initialize()
{
	getInstance();
}

void FCFSScheduler::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}