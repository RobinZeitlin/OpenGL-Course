#include "MemoryStatus.h"

#include <tuple>

MemoryStatus::MemoryStatus() { }

MemoryStatus::~MemoryStatus() { }

void MemoryStatus::print_memory_status() {
	MEMORYSTATUSEX statex; statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	std::cout << "There is " << statex.ullAvailPhys / (1024 * 1024)
		<< " MB of physical memory available. \n";
	std::cout << "There is " << statex.ullAvailVirtual / (1024 * 1024)
		<< " MB of virtual memory available.";
}

std::tuple<float, float> MemoryStatus::get_available_memory() {
	MEMORYSTATUSEX statex; statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	float physicalMemory = statex.ullAvailPhys / (1024 * 1024);
	float virtualMemory = statex.ullAvailVirtual / (1024 * 1024);

	return { physicalMemory, virtualMemory };
}