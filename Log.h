#ifndef LOG_H_
#define LOG_H_



// A class to log information during conversion.Everything is logged to log.txt in the directory used to run the program.No configuration or setup is required,
// just include the header and build the.cpp file, then start logging.
//
// The logger uses C++ stream semantics:
//  Log(LogLevel::Info) << "Message: " << variable;
//
// When logging decimals, standard cout modifiers apply:
//  Log(LogLevel::Info) << std::fixed << std::setprecision(2) << doubleVariable;
//
// Log level specifies a message at the beginning of the logged line, and can be set to any of the following :
//  *LogLevel::Error
//  * LogLevel::Warning
//  * LogLevel::Info
//  * LogLevel::Debug
//  * LogLevel::Progress - this is used to set the progress bar in the Fronter by passing an integer specifying the percentage.
//
// Note that Log will clear log.txt file not on program startup but on first use within a program. To prevent confusion, clear log.txt manually in main.cpp or
// just export converter version or a similar log to clear it.



#include <sstream>
#include <string>



enum class LogLevel
{
	Error,
	Warning,
	Info,
	Debug,
	Progress,
	Notice
};



class Log
{
  public:
	explicit Log(LogLevel level);
	~Log();
	Log(const Log&) = delete;
	Log operator=(const Log&) = delete;
	Log(Log&&) = delete;
	Log operator=(Log&&) = delete;

	template <class T> Log& operator<<(T t)
	{
		logMessageStream << t;
		return *this;
	}

  private:
	static void WriteToFile(LogLevel level, const std::string& logMessage);
	static void WriteTheTime(std::ostream& logFile);

	LogLevel logLevel;
	std::ostringstream logMessageStream;
};


using LOG = Log;



#endif // LOG_H_