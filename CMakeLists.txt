cmake_minimum_required(VERSION 3.10)
project(CommonItemsTests)

# Include diretories
include_directories(/usr/include)
include_directories("../ZipLib")
link_directories(/usr/lib)

# Set the compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -O0")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# Create OBJECT_DIR variable
set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir)
message("-- Object files will be output to: ${OBJECT_DIR}")

include_directories(googletest/googletest/include/gtest)
include_directories(googletest/googlemock/include/gmock)
add_subdirectory("tests/googletest" "googletest")
add_subdirectory(ZipLib [binary_dir])

################################################################################
# Source groups
################################################################################
set(commonItems
    "tests/test_main.cpp"
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
	"../Localization/LocalizationBlock.cpp"
	"../Localization/LocalizationBlock.h"
	"../Localization/LocalizationDatabase.cpp"
	"../Localization/LocalizationDatabase.h"
	"../Localization/LocalizationLambdas.h"
	"../ModLoader/Mod.h"
	"../ModLoader/ModFilesystem.cpp"
    "../ModLoader/ModFilesystem.h"
    "../ModLoader/ModLoader.cpp"
    "../ModLoader/ModLoader.h"
    "../ModLoader/ModParser.cpp"
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
    "tests/ColorTests.cpp"
    "tests/CommonFunctionsTests.cpp"
    "tests/CommonRegexesTests.cpp"
    "tests/ConvenientParserTests.cpp"
    "tests/ConverterVersionTests.cpp"
    "tests/DateTests.cpp"
    "tests/GameVersionTests.cpp"
    "tests/iconvliteTests.cpp"
    "tests/LogTests.cpp"
	"tests/ModFilesystemTests.cpp"
    "tests/ModLoaderTests.cpp"
    "tests/ModParserTests.cpp"
    "tests/OSCompatibilityLayerTests.cpp"
    "tests/ParserHelperTests.cpp"
    "tests/ParserTests.cpp"
    "tests/StringUtilsTests.cpp"
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
# Test Files
################################################################################
file(COPY "tests/TestFiles/" DESTINATION "./")


################################
# Coverage
################################
add_custom_target(gcov
	COMMAND mkdir -p gcoverage
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_custom_command(TARGET gcov
	COMMAND echo "=================== GCOV ===================="
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/ModLoader/ModFilesystem.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/ModLoader/ModLoader.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/ModLoader/ModParser.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	#COMMAND gcov -b ${CMAKE_SOURCE_DIR}/Localization/LocalizationBlock.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	#COMMAND gcov -b ${CMAKE_SOURCE_DIR}/Localization/LocalizationDatabase.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	#COMMAND gcov -b ${CMAKE_SOURCE_DIR}/CardinalToOrdinal.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/Color.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/CommonFunctions.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/ConvenientParser.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/ConverterVersion.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/Date.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/GameVersion.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/iconvlite.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/LinuxUtils.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/Log.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/OSCommonLayer.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/Parser.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/ParserHelpers.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/StringUtils.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir
	COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/gcoverage"
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gcoverage
)

add_custom_target(lcov
	COMMAND mkdir -p lcoverage
)
add_custom_command(TARGET lcov
	COMMAND echo "=================== LCOV ===================="
	COMMAND echo "-- Passing lcov tool under code coverage"
	COMMAND lcov --capture --directory CMakeFiles --output-file lcoverage/coverage.info
	COMMAND echo "-- Generating HTML output files"
	COMMAND genhtml lcoverage/coverage.info --output-directory lcoverage
)
add_dependencies(gcov ${PROJECT_NAME})

# Make sure to clean up the coverage folder
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES gcoverage)

# Create the gcov-clean target. This cleans the build as well as generated 
# .gcda and .gcno files.
add_custom_target(init
	COMMAND ${CMAKE_MAKE_PROGRAM} clean
	COMMAND rm -f ${TEST_OUTPUT_DIRECTORY}/*.gcno
	COMMAND rm -f ${TEST_OUTPUT_DIRECTORY}/*.gcda
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

################################
# Unit Tests
################################
enable_testing()
# Link test executable against gtest & gtest_main
target_link_libraries(${PROJECT_NAME} gtest gtest_main gmock ZIPLIB )
add_test( runUnitTests ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} )
