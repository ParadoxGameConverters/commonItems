cmake_minimum_required(VERSION 3.10)
project(CommonItemsTests)

# Include diretories
include_directories(/usr/include)
link_directories(/usr/lib)

# Set the compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# Create OBJECT_DIR variable
set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir)
message("-- Object files will be output to: ${OBJECT_DIR}")

# Find required packages
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

################################################################################
# Source groups
################################################################################
set(commonItems
    "test_main.cpp"
    "../Color.cpp"
    "../Color.h"
    "../CommonFunctions.cpp"
    "../CommonFunctions.h"
    "../CommonRegexes.h"
    "../ConvenientParser.cpp"
    "../ConvenientParser.h"
    "../ConverterVersion.cpp"
    "../ConverterVersion.h"
    "../Date.cpp"
    "../Date.h"
    "../GameVersion.cpp"
    "../GameVersion.h"
    "../iconvlite.cpp"
    "../iconvlite.h"
    "../Log.cpp"
    "../Log.h"
    "../OSCommonLayer.cpp"
    "../OSCompatibilityLayer.h"
    "../Parser.cpp"
    "../Parser.h"
    "../ParserHelpers.cpp"
    "../ParserHelpers.h"
    "../StringUtils.cpp"
    "../StringUtils.h"
    "../WinUtils.cpp"
)
source_group("commonItems" FILES ${commonItems})

set(gtest
    "googletest/googletest/src/gtest-all.cc"
    "googletest/googletest/src/gtest_main.cc"
)
source_group("gtest" FILES ${gtest})

set(tests
    "ColorTests.cpp"
    "CommonFunctionsTests.cpp"
    "ConvenientParserTests.cpp"
    "ConverterVersionTests.cpp"
    "DateTests.cpp"
    "GameVersionTests.cpp"
    "iconvliteTests.cpp"
    "LogTests.cpp"
    "OSCompatibilityLayerTests.cpp"
    "ParserHelperTests.cpp"
    "ParserTests.cpp"
    "StringUtilsTests.cpp"
)
source_group("tests" FILES ${tests})

set(ALL_FILES
    ${commonItems}
    ${gtest}
    ${tests}
)


################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})
target_link_libraries(${PROJECT_NAME} gtest)
target_link_libraries(${PROJECT_NAME} gmock)

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../compile-time-regular-expressions/single-header"
)


################################
# Unit Tests
################################
enable_testing()
# Link test executable against gtest & gtest_main
target_link_libraries(${PROJECT_NAME} gtest gtest_main)
add_test( runUnitTests ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} )