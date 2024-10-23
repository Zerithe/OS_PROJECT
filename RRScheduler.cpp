#include "RRScheduler.h"
#include "Process.h"
#include "CPUCore.h"
#include "ScreenConsole.h"
#include "ConsoleManager.h"
#include <thread>
#include <memory>

RRScheduler* RRScheduler::sharedInstance = nullptr;

RRScheduler::RRScheduler()
{

}

void RRScheduler::runRR()
{
	while (this->running) {
		for (std::shared_ptr<CPUCore> core : this->cores) {
			if (!core->containsProcess() && !this->readyQueue.empty()) {
				core->registerProcess(this->readyQueue.front());
				this->readyQueue.pop();
				this->coresUsed++;
			}
			if (core->containsProcess() && core->getIsFinished()) {
				this->finishedList.push_back(core->getProcess());
				core->deallocateCPU();
				this->coresUsed--;
			}
			if (core->containsProcess() && core->getIsPreEmpted()) {
				this->addProcess(core->getProcess());
				core->deallocateCPU();
				this->coresUsed--;
			}
		}
	}
}

void RRScheduler::addCPUCore(std::shared_ptr<CPUCore> cpuCore)
{
	this->cores.push_back(cpuCore);
}

void RRScheduler::addProcess(std::shared_ptr<Process> process)
{
	this->readyQueue.push(process);
}


void RRScheduler::showListOfProcesses()
{
	int noOfCores = this->cores.size();
	float cpuUtilization = (this->coresUsed / static_cast<float>(noOfCores)) * 100;
	std::cout << "CPU utilization: " << cpuUtilization << "%" << std::endl;
	std::cout << "Cores used: " << this->coresUsed << std::endl;
	std::cout << "Cores available: " << noOfCores - this->coresUsed << std::endl;
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
	for (int i = 0; i < 15; ++i) {
		std::cout << "-";
	}
	std::cout << std::endl;

}

void RRScheduler::stop()
{
	this->running = false;
}

RRScheduler* RRScheduler::getInstance()
{
	if (sharedInstance == nullptr)
	{
		sharedInstance = new RRScheduler();
	}

	return sharedInstance;
}

void RRScheduler::initialize()
{
	getInstance();
}

void RRScheduler::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}