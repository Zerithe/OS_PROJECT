#pragma once
#include <memory>
#include <Windows.h>
#include <iostream>
#include "ScreenConsole.h"
#include "Process.h"

struct ProcessWithMemory {
	std::string processName;
	int memoryStart;
	int memoryEnd;
};
class MemoryManager
{
public:
	typedef std::vector<ProcessWithMemory> ProcessInMemoryList;

	static MemoryManager* getInstance();
	static void initialize();
	static void destroy();

	void setMaxMemory(int max_mem);
	void setMemoryPerFrame(int mem_per_frame);
	int findMemory(std::shared_ptr<Process> process);
	void addProcessToMemory(std::shared_ptr<Process> process, int startMemoryAddress);
	bool isProcessInMemory(std::shared_ptr<Process> process);
	void deallocateProcessFromMemory(std::shared_ptr<Process> process);
	int getTotalExternalFragmentation() const;
	void printMemory(int cpuNo, int quantumCycle);
	std::string getDateNow();

private:
	MemoryManager();
	~MemoryManager() = default;
	MemoryManager(MemoryManager const&) {}; //copy operator
	MemoryManager& operator=(MemoryManager const&) {}; //assignment operator
	static MemoryManager* sharedInstance;

	ProcessInMemoryList processInMemoryList;
	int max_mem;
	int mem_per_frame;
};

