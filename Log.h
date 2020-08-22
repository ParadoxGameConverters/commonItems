#ifndef LOG_H_
#define LOG_H_



#include <sstream>
#include <string>



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


using LOG = Log;



#endif // LOG_H_