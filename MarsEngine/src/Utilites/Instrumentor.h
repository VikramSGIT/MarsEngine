#ifndef ME_INSTRUMENTOR
#define ME_INTSTUMENTOR

#include <mutex>
#include <chrono>
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

class CallTracer
{
public:
	CallTracer(const char* name);
	~CallTracer();

	void Start();
	void End();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
	const char* m_Name;
};

class InstrumentorTimer
{
public:
	InstrumentorTimer(double& out);
	~InstrumentorTimer();

	void Start();
	void End();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
	double& m_Out;
};

std::string PrintFooter();
std::string PrintHeader();
void WriteFile(const std::string& filepath);

#ifdef ME_PROFILE_TRACE_CALLFUNC
#define ME_PROFILE_TRACE_CALL()\
CallTracer tracer##__LINE__(__FUNCTION__)
#else
#define ME_PROFILE_TRACE_CALL()
#define ME_PROFILE_PRINT_CALLSTACK(X)
#endif

#endif