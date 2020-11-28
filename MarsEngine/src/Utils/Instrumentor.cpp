#include "Intrumentor.h"

std::vector<std::string> callstack, calltimer;

std::string PrintFooter()
{
	return  "</Table>\n</Worksheet>\n</Workbook>\n";
}

std::string PrintHeader()
{
	return  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<?mso-application progid=\"Excel.Sheet\"?>\n"
		"<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\" xmlns:x=\"urn:schemas-microsoft-com:office:excel\" xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\" xmlns:html=\"https://www.w3.org/TR/html401/\">\n"
		"<Worksheet ss:Name=\"Mars Engine Profile\">\n"
		"<Table>\n";
}

void WriteFile(const std::string& filepath)
{
	std::ofstream file;
	file.open(filepath);
	if (file.is_open())
	{
		file << PrintHeader() <<
			"<Column ss:Index=\"1\" ss:AutoFitWidth=\"0\" ss:Width=\"400\"/>\n"
			"<Row>\n"
			"<Cell><Data ss:Type=\"String\">Call Stack</Data></Cell>\n"
			"<Cell><Data ss:Type=\"String\">Time(ms)</Data></Cell>\n"
			"</Row>\n";
		auto itc = callstack.begin();
		auto itt = calltimer.begin();
		while (itc != callstack.end())
		{
			file << "<Row>\n"
					"<Cell><Data ss:Type=\"String\">" << *itc << "</Data></Cell>\n" <<
					"<Cell><Data ss:Type=\"Number\">" << *itt << "</Data></Cell>\n"
					"</Row>\n";
			itc++;
			itt++;
		}
		file << PrintFooter();
		file.close();
	}
	else
		std::cout << "Error Creating the output file" << std::endl;
}



CallTracer::CallTracer(const char* name)
	:m_Name(name)
{
	Start();
}
CallTracer::~CallTracer()
{
	End();
}
void CallTracer::Start()
{
	m_StartTime = std::chrono::high_resolution_clock::now();
}
void CallTracer::End()
{
	auto endTime = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

	auto duration = (end - start) * 0.001;
#ifdef ME_PROFILE_TRACE_CALL_CONSOLE
	auto now = std::time(0);
	auto m_Time = localtime(&now);
	std::cout << "MARS ENGINE CALLPROFILE: " << "(" << duration << "ms) " << m_Name << std::endl;
#endif
	callstack.push_back(m_Name);
	calltimer.push_back(std::to_string(duration));
}

InstrumentorTimer::InstrumentorTimer(double& out)
	:m_Out(out)
{
	Start();
}
InstrumentorTimer::~InstrumentorTimer()
{
	End();
}
void InstrumentorTimer::Start()
{
	m_StartTime = std::chrono::high_resolution_clock::now();
}
void InstrumentorTimer::End()
{
	auto endTime = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

	m_Out = (end - start) * 0.001;
}