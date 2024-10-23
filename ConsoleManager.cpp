#include "ConsoleManager.h"
#include "MainConsole.h"
#include "ScreenConsole.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"
#include <memory>
#include <thread>

ConsoleManager* ConsoleManager::sharedInstance = nullptr;
AConsole::String input;

ConsoleManager::ConsoleManager()
{
	this->running = true;

	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>("MAIN_CONSOLE");

	this->consoleTable[MAIN_CONSOLE] = mainConsole;

	this->switchConsole(MAIN_CONSOLE);

	std::cout << "constructed" << std::endl;
}


ConsoleManager* ConsoleManager::getInstance()
{
	if(sharedInstance == nullptr)
	{
		sharedInstance = new ConsoleManager();
	}

	return sharedInstance;
}

void ConsoleManager::initialize()
{
	getInstance();
}

void ConsoleManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

void ConsoleManager::drawConsole()
{
	this->startSchedulerTest = false;

	this->currentConsole->display();
	if (currentConsole == consoleTable.at(MAIN_CONSOLE)) {
		std::shared_ptr<MainConsole> mainConsole = std::dynamic_pointer_cast<MainConsole>(currentConsole);
		if (mainConsole->hasExited()) {
			this->exitApplication();
		}
		if (!mainConsole->getStringToRegister().empty()) {
			const std::shared_ptr<ScreenConsole> screenConsole = std::make_shared<ScreenConsole>(mainConsole->getStringToRegister(), this->minInstructions, this->maxInstructions);
			this->registerConsole(screenConsole);
		}
		if (!mainConsole->getStringToRead().empty()) {
			this->switchConsole(mainConsole->getStringToRead());
		}
		if (mainConsole->getShowListOfProcesses()) {
			if (this->scheduler == "fcfs")
				FCFSScheduler::getInstance()->showListOfProcesses();
			else 
				RRScheduler::getInstance()->showListOfProcesses();
		}
		if (mainConsole->getStartSchedulerTest()) {
			this->startSchedulerTest = true;
		}
		if (mainConsole->getStopSchedulerTest()) {
			this->endSchedulerTest();
		}
	}
	else {
		if (this->currentConsole->hasExited()) {
			this->returnToPreviousConsole();
		}
	}
}

void ConsoleManager::process() const
{
	this->currentConsole->process();
}

void ConsoleManager::switchConsole(AConsole::String consoleName)
{
	if(this->consoleTable.contains(consoleName))
	{
		std::cout << "\033[2J\033[1;1H";
		this->previousConsole = this->currentConsole;
		if (this->previousConsole != nullptr)
		{
			this->previousConsole->offEnabled();
		}
		this->currentConsole = this->consoleTable[consoleName];
	}
	else
	{
		std::cout << "Console not found" << std::endl;
	}
}

void ConsoleManager::returnToPreviousConsole()
{
	if (this->previousConsole != nullptr)
	{
		std::cout << "\033[2J\033[1;1H";
		this->currentConsole = this->previousConsole;
		this->previousConsole = nullptr;
	}
	else
	{
		std::cout << "No previous console" << std::endl;
	}
}

void ConsoleManager::exitApplication()
{
	this->running = false;
}

void ConsoleManager::registerConsole(std::shared_ptr<ScreenConsole> screenRef)
{
	if (this->consoleTable.contains(screenRef->getName()))
	{
		std::cerr << "Screen name " << screenRef->getName() << " already exists. Please use a different name." << std::endl;
		return;
	}
	this->consoleTable[screenRef->getName()] = screenRef;
	this->switchConsole(screenRef->getName());
	if (this->scheduler == "fcfs")
		FCFSScheduler::getInstance()->addProcess(screenRef->getProcess());
	else 
		RRScheduler::getInstance()->addProcess(screenRef->getProcess());
}

void ConsoleManager::registerConsoleForSchedulerTest(std::shared_ptr<ScreenConsole> screenRef)
{
	if (this->consoleTable.contains(screenRef->getName()))
	{
		std::cerr << "Screen name " << screenRef->getName() << " already exists. Please use a different name." << std::endl;
		return;
	}
	this->consoleTable[screenRef->getName()] = screenRef;
	if (this->scheduler == "fcfs")
		FCFSScheduler::getInstance()->addProcess(screenRef->getProcess());
	else
		RRScheduler::getInstance()->addProcess(screenRef->getProcess());
}

void ConsoleManager::setNumRangeOfInstructions(int minInstructions, int maxInstructions)
{
	this->minInstructions = minInstructions;
	this->maxInstructions = maxInstructions;
}

void ConsoleManager::setBatchProcessFrequency(int batchProcessFreq)
{
	this->batchProcessFreq = batchProcessFreq;
}

void ConsoleManager::setScheduler(std::string scheduler)
{
	this->scheduler = scheduler;
}

void ConsoleManager::runSchedulerTest()
{
	this->schedulerTest = true;
	int processNameCounter = 1;
	int cpuCycle = 1;
	while (this->schedulerTest) {
		if (cpuCycle % batchProcessFreq == 0) {
			const std::string processName = "p" + std::to_string(processNameCounter);
			const std::shared_ptr<ScreenConsole> screenConsole = std::make_shared<ScreenConsole>(processName, this->minInstructions, this->maxInstructions);
			this->registerConsoleForSchedulerTest(screenConsole);
			processNameCounter++;
		}
		cpuCycle++;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void ConsoleManager::endSchedulerTest()
{
	this->schedulerTest = false;
}

bool ConsoleManager::isRunning() const
{
	return running;
}


bool ConsoleManager::getStartSchedulerTest()
{
	return this->startSchedulerTest;
}


HANDLE ConsoleManager::getConsole() const
{
	return consoleHandle;
}

void ConsoleManager::setCursorPosition(int posX, int posY) const
{
}