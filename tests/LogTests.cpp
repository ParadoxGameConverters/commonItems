#include "../Log.h"
#include "gtest/gtest.h"
#include <sstream>



TEST(Log_Tests, ErrorMessagesLogged)
{
	const std::stringstream log;
	const auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	Log(LogLevel::Error) << "Error message";

	std::cout.rdbuf(stdOutBuf);

	ASSERT_EQ("   [ERROR] Error message\n", log.str());
}


TEST(Log_Tests, WarningMessagesLogged)
{
	const std::stringstream log;
	const auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	Log(LogLevel::Warning) << "Warning message";

	std::cout.rdbuf(stdOutBuf);

	ASSERT_EQ(" [WARNING] Warning message\n", log.str());
}


TEST(Log_Tests, InfoMessagesLogged)
{
	const std::stringstream log;
	const auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	Log(LogLevel::Info) << "Info message";

	std::cout.rdbuf(stdOutBuf);

	ASSERT_EQ("    [INFO] Info message\n", log.str());
}


TEST(Log_Tests, DebugMessagesLogged)
{
	const std::stringstream log;
	const auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	Log(LogLevel::Debug) << "Debug message";

	std::cout.rdbuf(stdOutBuf);

	ASSERT_EQ("   [DEBUG]     Debug message\n", log.str());
}

TEST(Log_Tests, ProgressMessagesLogged)
{
	const std::stringstream log;
	const auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	Log(LogLevel::Progress) << "Progress message";

	std::cout.rdbuf(stdOutBuf);

	ASSERT_EQ("[PROGRESS] Progress message\n", log.str());
}