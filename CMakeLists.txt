cmake_minimum_required(VERSION 3.10)
project(CommonItemsTests)

# Include directories
include_directories(.)
include_directories(/usr/include)
link_directories(/usr/lib)

# Set the compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -O0")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# Create OBJECT_DIR variable
set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir)
message("-- Object files will be output to: ${OBJECT_DIR}")

include_directories(../external/googletest/googletest/include/gtest)
include_directories(../external/googletest/googlemock/include/gmock)
add_subdirectory("external/googletest" "googletest")

################################################################################
# Source groups
################################################################################
set(commonItems
    "Color.cpp"
    "Color.h"
    "CommonFunctions.cpp"
    "CommonFunctions.h"
    "CommonRegexes.h"
    "ConvenientParser.cpp"
    "ConvenientParser.h"
    "ConverterVersion.cpp"
    "ConverterVersion.h"
    "Date.cpp"
    "Date.h"
    "GameVersion.cpp"
    "GameVersion.h"
    "iconvlite.cpp"
    "iconvlite.h"
    "LinuxUtils.cpp"
    "Log.cpp"
    "Log.h"
	"Localization/LocalizationBlock.cpp"
	"Localization/LocalizationBlock.h"
	"Localization/LocalizationDatabase.cpp"
	"Localization/LocalizationDatabase.h"
	"Localization/LocalizationLambdas.h"
	"ModLoader/Mod.h"
	"ModLoader/ModFilesystem.cpp"
    "ModLoader/ModFilesystem.h"
    "ModLoader/ModLoader.cpp"
    "ModLoader/ModLoader.h"
    "ModLoader/ModParser.cpp"
    "ModLoader/ModParser.h"
    "OSCommonLayer.cpp"
    "OSCompatibilityLayer.h"
    "Parser.cpp"
    "Parser.h"
    "ParserHelpers.cpp"
    "ParserHelpers.h"
    "StringUtils.cpp"
    "StringUtils.h"
    "external/zip/src/zip.c"
)
source_group("commonItems" FILES ${commonItems})

set(tests
	"tests/test_main.cpp"
    "tests/ColorTests.cpp"
    "tests/CommonFunctionsTests.cpp"
    "tests/CommonRegexesTests.cpp"
    "tests/ConvenientParserTests.cpp"
    "tests/ConverterVersionTests.cpp"
    "tests/DateTests.cpp"
    "tests/GameVersionTests.cpp"
    "tests/iconvliteTests.cpp"
	"tests/Localization/LocalizationBlockTests.cpp"
	"tests/Localization/LocalizationDatabaseTests.cpp"
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
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/Localization/LocalizationBlock.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir/Localization
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/Localization/LocalizationDatabase.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir/Localization
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/ModLoader/ModFilesystem.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir/ModLoader
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/ModLoader/ModLoader.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir/ModLoader
	COMMAND gcov -b ${CMAKE_SOURCE_DIR}/ModLoader/ModParser.cpp -o ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir/ModLoader
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
	COMMAND rm -rf CMakeFiles/CommonItemsTests.dir/tests
	COMMAND echo "=================== LCOV ===================="
	COMMAND echo "-- Passing lcov tool under code coverage"
	COMMAND lcov --c --d CMakeFiles --o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/11/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/11/bits/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/11/ext/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/x86_64-linux-gnu/c++/11/bits/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/12/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/12/bits/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/12/ext/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "commonItems/external/googletest/googlemock/include/gmock/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "commonItems/external/googletest/googlemock/include/gmock/internal/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "commonItems/external/googletest/googletest/include/gtest/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "commonItems/external/googletest/googletest/include/gtest/internal/*" -o lcoverage/coverage.info
	COMMAND lcov --remove lcoverage/coverage.info "/usr/include/x86_64-linux-gnu/c++/12/bits/*" -o lcoverage/coverage.info
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
target_link_libraries(${PROJECT_NAME} gtest gtest_main gmock )
add_test( runUnitTests ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} )

################################
# Clang-tidy
################################

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}")
add_library(run_clang_tidy
	${ALL_FILES}
)
target_link_libraries(run_clang_tidy pthread)
set_target_properties(run_clang_tidy PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
