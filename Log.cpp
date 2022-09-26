#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>



Log::Log(LogLevel level): logLevel(level)
{
	if (!log_file_created)
	{
		std::ofstream logFile("log.txt", std::ofstream::trunc);
		log_file_created = true;
		logFile.close();
	}
}


Log::~Log()
{
	logMessageStream << std::endl;
	const auto logMessage = logMessageStream.str();
	WriteToFile(logLevel, logMessage);
}


static const std::map<LogLevel, std::string> logLevelStrings = {{LogLevel::Error, "   [ERROR] "},
	 {LogLevel::Warning, " [WARNING] "},
	 {LogLevel::Info, "    [INFO] "},
	 {LogLevel::Debug, "   [DEBUG]     "},
	 {LogLevel::Progress, "[PROGRESS] "},
	 {LogLevel::Notice, "  [NOTICE] "}};

void Log::WriteToFile(LogLevel level, const std::string& logMessage)
{
	std::ofstream logFile("log.txt", std::ofstream::app);
	WriteTheTime(logFile);
	logFile << logLevelStrings.find(level)->second;
	logFile << logMessage;
	std::cout << logLevelStrings.find(level)->second;
	std::cout << logMessage; // write to the console for the sake of tests
}


void Log::WriteTheTime(std::ostream& logFile)
{
	time_t rawTime;
	time(&rawTime);

	tm timeInfo{};
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
	localtime_s(&timeInfo, &rawTime);
#else
	localtime_r(&rawTime, &timeInfo); // POSIX
#endif


	if (char timeBuffer[64]; strftime(timeBuffer, sizeof timeBuffer, "%Y-%m-%d %H:%M:%S ", &timeInfo) != 0)
	{
		logFile << timeBuffer;
	}
}
