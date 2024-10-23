#include "FCFSScheduler.h"
#include "Process.h"
#include "CPUCore.h"
#include "ScreenConsole.h"
#include "ConsoleManager.h"
#include <thread>
#include <memory>

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
				this->coresUsed++;
			}
			if (core->containsProcess() && core->getIsFinished()) {
				this->finishedList.push_back(core->getProcess());
				core->deallocateCPU();
				this->coresUsed--;
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

void FCFSScheduler::printListOfProcesses()
{
	int noOfCores = this->cores.size();
	float cpuUtilization = (this->coresUsed / static_cast<float>(noOfCores)) * 100;
	std::ofstream outfile("csopesy-log.txt");
	if (outfile.is_open()) {
		outfile << "CPU utilization: " << cpuUtilization << "%" << "\n";
		outfile << "Cores used: " << this->coresUsed << "\n";
		outfile << "Cores available: " << noOfCores - this->coresUsed << "\n";
		for (int i = 0; i < 15; ++i) {
			outfile << "-";
		}
		outfile << "\n";
		outfile << "Running processes:" << "\n";
		for (std::shared_ptr<CPUCore> core : this->cores) {
			if (core->containsProcess()) {
				outfile << core->getProcess()->getName() << " " << core->getProcess()->getTimeStarted() << " Core: " << core->getId() << " " << core->getProcess()->getCommandCounter() << " / " << core->getProcess()->getTotalInstructions() << "\n";
			}
		}
		outfile << "Finished processes:" << "\n";
		for (std::shared_ptr<Process> process : this->finishedList) {
			outfile << process->getName() << " " << process->getTimeFinished() << " Finished " << process->getTotalInstructions() << " / " << process->getTotalInstructions() << "\n";
		}
		for (int i = 0; i < 15; ++i) {
			outfile << "-";
		}
		outfile << "\n";
		outfile.close();
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