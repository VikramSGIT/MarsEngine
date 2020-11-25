#pragma once

#include "MarsHeader.h"

#include <mutex>
#include <chrono>
#include <iostream>
#include <vector>

static std::unique_ptr<std::vector<const char*>> CallStack = std::make_unique<std::vector<const char*>>();
static std::unique_ptr<std::vector<long long>> CallStackTimer = std::make_unique<std::vector<long long>>();

#define ME_PROFILE_TRACE_CALL()CallStack->push_back(__FUNCTION__)
#define ME_PROFILE_PRINTSTACK()\
for (auto it = CallStack->begin(); it != CallStack->end(); it++)\
    ME_CORE_INFO(*it);

class InstrumentorTimer
{
public:
	InstrumentorTimer()
	{
		Start();
	}

	~InstrumentorTimer()
	{

		End();
	}

	void Start()
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
	}

	void End()
	{
		auto endTime = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

		auto duration = end - start;
		CallStackTimer->push_back(duration);
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
};
