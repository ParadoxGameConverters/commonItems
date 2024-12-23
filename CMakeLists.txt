cmake_minimum_required(VERSION 3.25)
project(CommonItemsTests)

# Include directories
include_directories(.)
if (PLATFORM STREQUAL "Linux")
    include_directories(/usr/include)
    link_directories(/usr/lib)
endif (PLATFORM STREQUAL "Linux")

# Set the compiler options
set(CMAKE_CXX_STANDARD 23)
set (UNICODE_DEFAULT OFF)

if (PLATFORM STREQUAL "Windows")
	ADD_DEFINITIONS(-DUNICODE -D_UNICODE)
elseif (PLATFORM STREQUAL "Linux")
    set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)
	if (COVERAGE STREQUAL true)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
		set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
	else (COVERAGE STREQUAL true)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall")
	endif (COVERAGE STREQUAL true)
endif (PLATFORM STREQUAL "Windows")

# Create OBJECT_DIR variable
set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/CommonItemsTests.dir)
message("-- Object files will be output to: ${OBJECT_DIR}")

include_directories(../external/googletest/googletest/include/gtest)
include_directories(../external/googletest/googlemock/include/gmock)
add_subdirectory("external/googletest" "googletest")

################################################################################
# Source groups
################################################################################
set(COMMON_SOURCES ${COMMON_SOURCES} "Color.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "Color.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "CommonFunctions.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "CommonFunctions.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "CommonRegexes.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "ConvenientParser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "ConvenientParser.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "ConverterVersion.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "ConverterVersion.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "Date.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "Date.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "GameVersion.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "GameVersion.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "iconvlite.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "iconvlite.h")
if (PLATFORM STREQUAL "Windows")
	set(COMMON_SOURCES ${COMMON_SOURCES} "WinUtils.cpp")
elseif (PLATFORM STREQUAL "Linux")
	set(COMMON_SOURCES ${COMMON_SOURCES} "LinuxUtils.cpp")
endif (PLATFORM STREQUAL "Windows")
set(COMMON_SOURCES ${COMMON_SOURCES} "Log.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "Log.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "Localization/LocalizationBlock.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "Localization/LocalizationBlock.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "Localization/LocalizationDatabase.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "Localization/LocalizationDatabase.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "Localization/LocalizationLambdas.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "ModLoader/Mod.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "ModLoader/ModFilesystem.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "ModLoader/ModFilesystem.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "ModLoader/ModLoader.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "ModLoader/ModLoader.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "ModLoader/ModParser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "ModLoader/ModParser.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "OSCommonLayer.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "OSCompatibilityLayer.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "Parser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "Parser.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "ParserHelpers.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "ParserHelpers.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "StringUtils.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "StringUtils.h")
set(COMMON_SOURCES ${COMMON_SOURCES} "external/zip/src/zip.c")
add_library(commonLib
	${COMMON_SOURCES}
)

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


################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${tests})
target_link_libraries(${PROJECT_NAME} commonLib)

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
