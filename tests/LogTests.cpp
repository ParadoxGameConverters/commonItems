#include "../Log.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <sstream>


TEST(Log_Tests, ErrorMessagesLogged)
{
	const std::stringstream log;
	auto* const stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	Log(LogLevel::Error) << "Error message";

	std::cout.rdbuf(stdOutBuf);

	EXPECT_THAT(log.str(), testing::HasSubstr(R"([ERROR] Error message)"));
}


TEST(Log_Tests, WarningMessagesLogged)
{
	const std::stringstream log;
	auto* const stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	Log(LogLevel::Warning) << "Warning message";

	std::cout.rdbuf(stdOutBuf);

	EXPECT_THAT(log.str(), testing::HasSubstr(R"([WARNING] Warning message)"));
}


TEST(Log_Tests, InfoMessagesLogged)
{
	const std::stringstream log;
	auto* const stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	Log(LogLevel::Info) << "Info message";

	std::cout.rdbuf(stdOutBuf);

	EXPECT_THAT(log.str(), testing::HasSubstr(R"([INFO] Info message)"));
}


TEST(Log_Tests, DebugMessagesLogged)
{
	const std::stringstream log;
	auto* const stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	Log(LogLevel::Debug) << "Debug message";

	std::cout.rdbuf(stdOutBuf);

	EXPECT_THAT(log.str(), testing::HasSubstr(R"([DEBUG]     Debug message)"));
}

TEST(Log_Tests, ProgressMessagesLogged)
{
	const std::stringstream log;
	auto* const stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	Log(LogLevel::Progress) << "Progress message";

	std::cout.rdbuf(stdOutBuf);

	EXPECT_THAT(log.str(), testing::HasSubstr(R"([PROGRESS] Progress message)"));
}

TEST(Log_Tests, NoticeMessagesLogged)
{
	const std::stringstream log;
	auto* const stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	Log(LogLevel::Notice) << "Notice message";

	std::cout.rdbuf(stdOutBuf);

	EXPECT_THAT(log.str(), testing::HasSubstr(R"([NOTICE] Notice message)"));
}
