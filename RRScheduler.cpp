#include "RRScheduler.h"
#include "Process.h"
#include "CPUCore.h"
#include "ScreenConsole.h"
#include "ConsoleManager.h"
#include "MemoryManager.h"
#include "PagingAllocator.h"
#include "ScreenConsole.h"
#include <thread>
#include <memory>
#include <fstream>
#include <sstream>

RRScheduler* RRScheduler::sharedInstance = nullptr;

RRScheduler::RRScheduler()
{

}

void RRScheduler::runRR()
{
	if (this->memory_allocator == "flat") {
		while (this->running) {
			for (std::shared_ptr<CPUCore> core : this->cores) {
				if (!core->containsProcess() && !this->readyQueue.empty()) {
					if (MemoryManager::getInstance()->isProcessInMemory(this->readyQueue.front())) { //If the process is already in memory
						core->registerProcess(this->readyQueue.front());
						this->readyQueue.pop();
						this->coresUsed++;
					}
					else if (MemoryManager::getInstance()->findMemory(this->readyQueue.front()) >= 0) { //If there is enough space in memory
						MemoryManager::getInstance()->addProcessToMemory(this->readyQueue.front(), MemoryManager::getInstance()->findMemory(this->readyQueue.front())); //Add the process first to memory before putting in CPU/core
						core->registerProcess(this->readyQueue.front());
						this->readyQueue.pop();
						this->coresUsed++;
					}
					//Process reverts back to the tail of the ready queue if there is not enough space in memory
					//TODO: Implement backing store
					else {
						auto frontProcess = this->readyQueue.front();
						this->readyQueue.pop();
						this->addProcess(frontProcess);
					}
				}
				if (core->containsProcess() && core->getIsFinished()) {
					this->finishedList.push_back(core->getProcess());
					MemoryManager::getInstance()->deallocateProcessFromMemory(core->getProcess()); //Remove process from memory when finished
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
	else {
		while (this->running) {
			for (std::shared_ptr<CPUCore> core : this->cores) {
				if (!core->containsProcess() && !this->readyQueue.empty()) {
					if (PagingAllocator::getInstance()->isProcessInMemory(this->readyQueue.front())) { //If the process is already in memory
						core->registerProcess(this->readyQueue.front());
						this->readyQueue.pop();
						this->coresUsed++;
					}
					else if (PagingAllocator::getInstance()->allocate(this->readyQueue.front())) { //If memory is allocated
						core->registerProcess(this->readyQueue.front());
						this->readyQueue.pop();
						this->coresUsed++;
					}
					//Process reverts back to the tail of the ready queue if there is not enough space in memory
					//TODO: Implement backing store
					else {
						auto frontProcess = this->readyQueue.front();
						this->readyQueue.pop();
						this->addProcess(frontProcess);
					}
				}
				if (core->containsProcess() && core->getIsFinished()) {
					this->finishedList.push_back(core->getProcess());
					PagingAllocator::getInstance()->deallocate(core->getProcess()); //Remove process from memory when finished
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

void RRScheduler::printListOfProcesses()
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

void RRScheduler::stop()
{
	this->running = false;
}

int RRScheduler::getCoresUsed() const
{
	return this->coresUsed;
}

int RRScheduler::getNoOfCores() const
{
	return this->cores.size();
}

std::unordered_map<std::string, int> RRScheduler::getRunningProcesses() const
{
	std::unordered_map<std::string, int> runningProcesses;
	for (std::shared_ptr<CPUCore> core : this->cores) {
		if (core->containsProcess()) {
			runningProcesses[core->getProcess()->getName()] = core->getProcess()->getTotalMemoryRequired();
		}
	}
	return runningProcesses;
}

int RRScheduler::getTotalIdleCPUTicks() const
{
	int totalIdleCPUTicks = 0;
	for (std::shared_ptr<CPUCore> core : this->cores) {
		totalIdleCPUTicks += core->getIdleCPUTicks();
	}
	return totalIdleCPUTicks;
}

int RRScheduler::getTotalActiveCPUTicks() const
{
	int totalActiveCPUTicks = 0;
	for (std::shared_ptr<CPUCore> core : this->cores) {
		totalActiveCPUTicks += core->getActiveCPUTicks();
	}
	return totalActiveCPUTicks;
}

void RRScheduler::putProcessToBackingStore(std::shared_ptr<Process> process)
{
	if (this->memory_allocator == "flat")
		MemoryManager::getInstance()->deallocateProcessFromMemory(process);
	else
		PagingAllocator::getInstance()->deallocate(process);
	process->setInBackingStore(true);
	ConsoleManager::getInstance()->unregisterConsole(process->getName());
	
	std::ofstream outfile("backing-store.txt");
	if (outfile.is_open()) {
		outfile << "pid " << process->getProcessID() << "\n";
		outfile << "processName " << process->getName() << "\n";
		outfile << "totalInstructions " << process->getTotalInstructions() << "\n";
		outfile << "memoryRequired " << process->getTotalMemoryRequired() << "\n";
		outfile << "memPerFrame " << process->getMemPerFrame() << "\n";
		outfile << "commandCounter " << process->getCommandCounter() << "\n";
		outfile.close();
	}
}

void RRScheduler::returnProcessFromBackingStore()
{
	int pid, totalInstructions, memoryRequired, memPerFrame, commandCounter;
	std::string processName;
	// Open the backing store text file
	std::ifstream infile("backing-store.txt");

	// Check if the file was successfully opened
	if (!infile) {
		std::cerr << "Unable to open backing store text file";
	}

	std::string line;

	// Read the file line by line
	while (std::getline(infile, line)) {
		std::istringstream iss(line);
		std::string key;

		// Extract the key (before the space) and then process based on the key
		if (line.find("pid") != std::string::npos) {
			iss >> key >> pid;
		}
		else if (line.find("processName") != std::string::npos) {
			iss >> key >> processName;
		}
		else if (line.find("totalInstructions") != std::string::npos) {
			iss >> key >> totalInstructions;
		}
		else if (line.find("memoryRequired") != std::string::npos) {
			iss >> key >> memoryRequired;
		}
		else if (line.find("memPerFrame") != std::string::npos) {
			iss >> key >> memPerFrame;
		}
		else if (line.find("commandCounter") != std::string::npos) {
			iss >> key >> commandCounter;
		}
	}
	// Close the file
	infile.close();
	const std::shared_ptr<ScreenConsole> screenConsole = std::make_shared<ScreenConsole>(pid, processName, totalInstructions, memoryRequired, memPerFrame, commandCounter);
	ConsoleManager::getInstance()->registerConsoleForSchedulerTest(screenConsole);
	if (this->memory_allocator == "flat")
		MemoryManager::getInstance()->addProcessToMemory(screenConsole->getProcess(), MemoryManager::getInstance()->findMemory(screenConsole->getProcess()));
	else
		PagingAllocator::getInstance()->allocate(screenConsole->getProcess());
}

void RRScheduler::removeProcessFromRQ(std::shared_ptr<Process> processToRemove)
{
	// Find and remove the process from the vector
	auto it = std::find(processes.begin(), processes.end(), processToRemove);
	if (it != processes.end()) {
		processes.erase(it);
		std::cout << "Process removed from vector." << std::endl;
	}
	else {
		std::cerr << "Process not found in vector!" << std::endl;
	}
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