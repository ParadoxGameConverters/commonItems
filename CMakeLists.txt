cmake_minimum_required(VERSION 3.25)
project(commonItems)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Allow including this project as a subdirectory without forcing tests
option(BUILD_TESTING "Build tests" OFF)
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(BUILD_TESTING ON)
endif ()

include(FetchContent)

# --- Dependencies ---

# nlohmann_json
find_package(nlohmann_json 3.12.0 QUIET)
if (NOT nlohmann_json_FOUND)
	FetchContent_Declare(
			nlohmann_json
			GIT_REPOSITORY https://github.com/nlohmann/json.git
			GIT_TAG v3.12.0
			)
	FetchContent_MakeAvailable(nlohmann_json)
endif ()

# zip
find_package(zip 0.3.5 QUIET)
if (NOT zip_FOUND)
	FetchContent_Declare(
			zip
			GIT_REPOSITORY https://github.com/kuba--/zip.git
			GIT_TAG v0.3.5
			)
	FetchContent_MakeAvailable(zip)
endif ()

# GTest (Only needed for tests)
if (BUILD_TESTING)
	find_package(GTest 1.17.0 QUIET)
	if (NOT GTest_FOUND)
		FetchContent_Declare(
				GTest
				GIT_REPOSITORY https://github.com/google/googletest.git
				GIT_TAG v1.17.0
				)
		FetchContent_MakeAvailable(GTest)
	endif ()
endif ()

option(COVERAGE "Enable code coverage" OFF)

message("-- Configuring for platform: ${CMAKE_SYSTEM_NAME}")
message("-- Code coverage enabled: ${COVERAGE}")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(UNICODE_DEFAULT OFF)
	ADD_DEFINITIONS(-DUNICODE -D_UNICODE -DWINDOWS)

	# Add warning flags for MSVC here if needed for all configs

elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	# Common flags for all builds on Linux
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-declarations -Wno-unknown-pragmas")

	# Per-configuration flags
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

	# Coverage flags (append only if enabled, typically used with Debug builds)
	if (COVERAGE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
		set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
	endif()
endif()

include_directories(.)
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	include_directories(/usr/include)
	link_directories(/usr/lib)
endif()

################################################################################
# Source groups
################################################################################
set(COMMON_SOURCES ${COMMON_SOURCES} "BulkParser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "Color.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "CommonFunctions.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "ConvenientParser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "ConverterVersion.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "Date.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "GameVersion.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "iconvlite.cpp")
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(COMMON_SOURCES ${COMMON_SOURCES} "WinUtils.cpp")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(COMMON_SOURCES ${COMMON_SOURCES} "LinuxUtils.cpp")
endif()
set(COMMON_SOURCES ${COMMON_SOURCES} "Log.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "OSCommonLayer.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "Parser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "ParserHelpers.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "StringUtils.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "Localization/LocalizationBlock.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "Localization/LocalizationDatabase.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "ModLoader/ModFilesystem.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "ModLoader/ModLoader.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "ModLoader/ModParser.cpp")
add_library(commonLib
            ${COMMON_SOURCES}
            )
add_library(commonItems::commonItems ALIAS commonLib)

target_include_directories(commonLib
                           PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Localization>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ModLoader>
                           $<INSTALL_INTERFACE:include>
                           )

target_link_libraries(commonLib
                      PRIVATE
                      zip
                      PUBLIC
                      nlohmann_json::nlohmann_json
                      )

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_compile_options(commonLib PRIVATE /permissive /W4 /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 /w14311 /w14545 /w14546
	                       /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928 /external:anglebrackets /external:W0 /WX)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
endif()

file(GLOB_RECURSE PUBLIC_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/Localization/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/ModLoader/*.h"
     )

include(GNUInstallDirs)

# Only install dependencies if we built them (not found via find_package), to ensure they are available in the export set
set(INSTALL_TARGETS commonLib)
if (NOT nlohmann_json_FOUND)
	list(APPEND INSTALL_TARGETS nlohmann_json)
endif ()
if (NOT zip_FOUND)
	list(APPEND INSTALL_TARGETS zip)
endif ()

install(TARGETS ${INSTALL_TARGETS}
        EXPORT commonItemsTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
install(FILES ${PUBLIC_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/commonItems
        )
install(EXPORT commonItemsTargets
        FILE commonItemsTargets.cmake
        NAMESPACE commonItems::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/commonItems
        )
include(CMakePackageConfigHelpers)

configure_package_config_file(
		"cmake/commonItemsConfig.cmake.in"
		"${CMAKE_CURRENT_BINARY_DIR}/commonItemsConfig.cmake"
		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/commonItems
		)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/commonItemsConfig.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/commonItems
        )
export(PACKAGE commonItems)

if (BUILD_TESTING)
	set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/commonItemsTests.dir)
	message("-- Object files will be output to: ${OBJECT_DIR}")

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
	    "tests/LogTests.cpp"
	    "tests/ModFilesystemTests.cpp"
	    "tests/ModLoaderTests.cpp"
	    "tests/ModParserTests.cpp"
	    "tests/OSCompatibilityLayerTests.cpp"
	    "tests/ParserHelperTests.cpp"
	    "tests/ParserTests.cpp"
	    "tests/StringUtilsTests.cpp"
	    "tests/Localization/LocalizationBlockTests.cpp"
	    "tests/Localization/LocalizationDatabaseTests.cpp"
	    )
	source_group("tests" FILES ${tests})

	add_executable(commonItemsTests ${tests})

	target_link_libraries(commonItemsTests
	                      PRIVATE
	                      commonLib
	                      GTest::gtest
	                      GTest::gtest_main
	                      GTest::gmock
	                      )
	if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
		target_compile_options(commonItemsTests PRIVATE /permissive /W4 /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 /w14311 /w14545 /w14546
		                       /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928 /external:anglebrackets /external:W0 /WX)
	elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	endif ()

	file(COPY "tests/TestFiles/" DESTINATION "./")
	if (CMAKE_SYSTEM_NAME STREQUAL "Linux" AND COVERAGE)
		add_custom_target(lcov COMMAND mkdir -p lcoverage)
		add_custom_command(TARGET lcov
		                   COMMAND echo "=================== LCOV ===================="
		                   COMMAND echo "-- Passing lcov tool under code coverage"
		                   COMMAND lcov --c --d CMakeFiles/commonLib.dir --o lcoverage/coverage.info --ignore-errors inconsistent,inconsistent,unused,unused
		                   COMMAND lcov --remove lcoverage/coverage.info "/usr/include/c++/*" -o lcoverage/coverage.info --ignore-errors inconsistent,inconsistent,unused,unused
		                   COMMAND echo "-- Generating HTML output files"
		                   COMMAND genhtml lcoverage/coverage.info --output-directory lcoverage --ignore-errors inconsistent,inconsistent,unused,unused
		                   )
		set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES gcoverage)
		add_custom_target(init
		                  COMMAND ${CMAKE_MAKE_PROGRAM} clean
		                  COMMAND rm -f ${TEST_OUTPUT_DIRECTORY}/*.gcno
		                  COMMAND rm -f ${TEST_OUTPUT_DIRECTORY}/*.gcda
		                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		                  )
	endif ()
	enable_testing()
	add_test(NAME runUnitTests COMMAND commonItemsTests)
endif ()