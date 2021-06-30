cmake_minimum_required(VERSION 3.10)
project(CommonItemsTests)

# Include diretories
include_directories(/usr/include)
include_directories("../ZipLib")
link_directories(/usr/lib)

# Set the compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -O0 -Wall")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# Create OBJECT_DIR variable
set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir)
message("-- Object files will be output to: ${OBJECT_DIR}")

include_directories(googletest/googletest/include/gtest)
include_directories(googletest/googlemock/include/gmock)
add_subdirectory("googletest" "googletest")
add_subdirectory(../ZipLib [binary_dir])

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
    "../LinuxUtils.cpp"
    "../Log.cpp"
    "../Log.h"
    "../ModLoader/ModLoader.h"
    "../ModLoader/ModParser.h"
    "../OSCommonLayer.cpp"
    "../OSCompatibilityLayer.h"
    "../Parser.cpp"
    "../Parser.h"
    "../ParserHelpers.cpp"
    "../ParserHelpers.h"
    "../StringUtils.cpp"
    "../StringUtils.h"
)
source_group("commonItems" FILES ${commonItems})

set(tests
    "ColorTests.cpp"
    "CommonFunctionsTests.cpp"
    "ConvenientParserTests.cpp"
    "ConverterVersionTests.cpp"
    "DateTests.cpp"
    "GameVersionTests.cpp"
    "iconvliteTests.cpp"
    "LogTests.cpp"
    "ModLoaderTests.cpp"
    "ModParserTests.cpp"
    "OSCompatibilityLayerTests.cpp"
    "ParserHelperTests.cpp"
    "ParserTests.cpp"
    "StringUtilsTests.cpp"
)
source_group("tests" FILES ${tests})

set(ALL_FILES
    ${commonItems}
    ${tests}
)


################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

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
target_link_libraries(${PROJECT_NAME} gtest gtest_main gmock ZIPLIB )
add_test( runUnitTests ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} )
