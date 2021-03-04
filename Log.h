#ifndef LOG_H_
#define LOG_H_



#include <sstream>
#include <string>



namespace commonItems
{

enum class LogLevel
{
	Error,
	Warning,
	Info,
	Debug,
	Progress
};



class Log
{
	public:
		explicit Log(LogLevel);
		~Log();
		Log(const Log&) = delete;
		Log operator=(const Log&) = delete;
		Log(Log&&) = delete;
		Log operator=(Log&&) = delete;

		template<class T>	Log& operator<<(T t)
		{
			logMessageStream << t;
			return *this;
		}

	private:
		static void WriteToFile(LogLevel, const std::string& logMessage);
		static void WriteTheTime(std::ostream& logFile);

		LogLevel logLevel;
		std::ostringstream logMessageStream;
};

} // namespace commonItems


[[deprecated("Use commonItems::Log")]] typedef commonItems::Log Log;
[[deprecated("Use commonItems::Log")]] typedef commonItems::Log LOG;
[[deprecated("Use commonItems::LogLevel")]] typedef commonItems::LogLevel LogLevel;



#endif // LOG_H_