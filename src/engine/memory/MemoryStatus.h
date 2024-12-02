#pragma once

#include <Windows.h>
#include <iostream>

class MemoryStatus {
public:
	static MemoryStatus& get_instance() {
		static MemoryStatus instance;
		return instance;
	}

	MemoryStatus();
	~MemoryStatus();

	void print_memory_status();
	std::tuple<float, float> get_available_memory();
};
