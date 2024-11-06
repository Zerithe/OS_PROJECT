#include "MemoryManager.h"
#include <fstream>
#include <chrono>
#include <algorithm>

MemoryManager* MemoryManager::sharedInstance = nullptr;

MemoryManager::MemoryManager()
{

}

MemoryManager* MemoryManager::getInstance()
{
	if (sharedInstance == nullptr)
	{
		sharedInstance = new MemoryManager();
	}

	return sharedInstance;
}

void MemoryManager::initialize()
{
	getInstance();
}

void MemoryManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

void MemoryManager::setMaxMemory(int max_mem)
{
	this->max_mem = max_mem;
}

void MemoryManager::setMemoryPerFrame(int mem_per_frame)
{
	this->mem_per_frame = mem_per_frame;
}

int MemoryManager::findMemory(std::shared_ptr<Process> process)
{
	if (processInMemoryList.empty()) {
		if (this->max_mem >= process->getTotalMemoryRequired())
			return 0;
		else
			return -1;
	}
	else {
		if (processInMemoryList[0].memoryStart >= process->getTotalMemoryRequired())
			return 0;
		for (size_t i = 0; i < processInMemoryList.size(); ++i) {
			if (i + 1 == processInMemoryList.size()) {
				if (this->max_mem - processInMemoryList[i].memoryEnd >= process->getTotalMemoryRequired())
					return processInMemoryList[i].memoryEnd;
				else
					return -1;
			}
			if (processInMemoryList[i + 1].memoryStart - processInMemoryList[i].memoryEnd >= process->getTotalMemoryRequired())
				return processInMemoryList[i].memoryEnd;
		}
	}
}

void MemoryManager::addProcessToMemory(std::shared_ptr<Process> process, int startMemoryAddress)
{
	const int endMemoryAddress = startMemoryAddress + process->getTotalMemoryRequired();
	this->processInMemoryList.push_back({ process->getName(), startMemoryAddress, endMemoryAddress });
	std::sort(processInMemoryList.begin(), processInMemoryList.end(), [](const ProcessWithMemory& a, const ProcessWithMemory& b) {
		return a.memoryStart < b.memoryStart;
		});
}

bool MemoryManager::isProcessInMemory(std::shared_ptr<Process> process)
{
	
	// Use std::find_if with a lambda function to search by name
	auto it = std::find_if(processInMemoryList.begin(), processInMemoryList.end(), [&process](const ProcessWithMemory& processWithMemory) {
		return processWithMemory.processName == process->getName();
		});

	// Return true if found, otherwise false
	return it != processInMemoryList.end();
}

void MemoryManager::deallocateProcessFromMemory(std::shared_ptr<Process> process)
{
	processInMemoryList.erase(std::remove_if(processInMemoryList.begin(), processInMemoryList.end(),
		[&process](const ProcessWithMemory& processWithMemory) {
			return processWithMemory.processName == process->getName();
		}),
		processInMemoryList.end());
}

int MemoryManager::getTotalExternalFragmentation() const
{
	if (processInMemoryList.empty()) {
		return this->max_mem;
	}
	else {
		int totalFragmentation = processInMemoryList[0].memoryStart;
		for (size_t i = 0; i < processInMemoryList.size(); ++i) {
			if (i + 1 == processInMemoryList.size()) {
				totalFragmentation += (this->max_mem - processInMemoryList[i].memoryEnd);
				return totalFragmentation;
			}
			totalFragmentation += (processInMemoryList[i + 1].memoryStart - processInMemoryList[i].memoryEnd);
		}
	}
}

void MemoryManager::printMemory(int cpuNo, int quantumCycle)
{
	const std::string filename = "cpu" + std::to_string(cpuNo) + "_memory_stamp_" + std::to_string(quantumCycle) + ".txt";
	std::ofstream outfile(filename);
	if (outfile.is_open()) {
		outfile << "Timestamp: (" << this->getDateNow() << ")" << "\n";
		outfile << "Number of processes in memory: " << this->processInMemoryList.size() << "\n";
		outfile << "Total external fragmentation in KB: " << this->getTotalExternalFragmentation() << "\n";
		outfile << "\n";
		outfile << "----end---- = " << this->max_mem << "\n";
		outfile << "\n";
		for (int i = this->processInMemoryList.size() - 1; i >= 0; --i) {
			outfile << this->processInMemoryList[i].memoryEnd << "\n";
			outfile << this->processInMemoryList[i].processName << "\n";
			outfile << this->processInMemoryList[i].memoryStart << "\n";
			outfile << "\n";
		}
		outfile << "----start---- = 0" << "\n";
		outfile.close();
	}
}

std::string MemoryManager::getDateNow()
{
	// Get current time
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now);

	// Convert to local time and format it
	std::tm local_tm;

	localtime_s(&local_tm, &now_c);

	// Use a string stream to format the date and time
	std::ostringstream oss;
	oss << std::put_time(&local_tm, "%m/%d/%Y, %I:%M:%S %p");

	// Store formatted time in creationTime
	return oss.str();
}
