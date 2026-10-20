cmake_minimum_required(VERSION 3.20)

project(github_crawler VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#a built in cmake module similar to include
include(FetchContent)

set(JSON_Install OFF CACHE BOOL "" FORCE)
set(JSON_Buildtests OFF CACHE BOOL "" FORCE)

# register a dependancy: a name we're choosing (nlohmann_json),
# where to get it (GIT_REPOSITORY), and the version (v3.12.0).

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
	httplib
	GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    	GIT_TAG v0.41.0
    	GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(httplib)

add_executable(github_project_crawler
    src/main.cpp
    src/analysis_options.cpp
    src/data_loader.cpp
    src/repository_filter.cpp
    src/search_options.cpp
    src/github_client.cpp
    src/repository_parser.cpp
    src/repository_store.cpp
    src/repository_analyzer.cpp
    src/reporter.cpp
    src/json_exporter.cpp

)

target_compile_features(github_project_crawler PRIVATE cxx_std_20)
target_include_directories(github_project_crawler PRIVATE src)
target_link_libraries(github_project_crawler PRIVATE
    httplib::httplib
    nlohmann_json::nlohmann_json
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(github_project_crawler PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

include(CTest)

if(BUILD_TESTING)
    add_executable(search_options_tests
            tests/search_options_test.cpp
            src/search_options.cpp
    )
    target_compile_features(search_options_tests PRIVATE cxx_std_20)
    target_include_directories(search_options_tests PRIVATE src)

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(search_options_tests PRIVATE
                -Wall
                -Wextra
                -Wpedantic
        )
    endif()

    add_test(NAME search_options COMMAND search_options_tests)

    add_executable(repository_analyzer_tests
            tests/repository_analyzer_test.cpp
            src/repository_analyzer.cpp
    )
    target_compile_features(repository_analyzer_tests PRIVATE cxx_std_20)
    target_include_directories(repository_analyzer_tests PRIVATE src)

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(repository_analyzer_tests PRIVATE
                -Wall
                -Wextra
                -Wpedantic
        )
    endif()

    add_test(NAME repository_analyzer COMMAND repository_analyzer_tests)

    add_executable(analysis_options_tests
            tests/analysis_options_test.cpp
            src/analysis_options.cpp
    )
    target_compile_features(analysis_options_tests PRIVATE cxx_std_20)
    target_include_directories(analysis_options_tests PRIVATE src)

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(analysis_options_tests PRIVATE
                -Wall
                -Wextra
                -Wpedantic
        )
    endif()

    add_test(NAME analysis_options COMMAND analysis_options_tests)

    add_executable(data_loader_tests
            tests/data_loader_test.cpp
            src/data_loader.cpp
    )
    target_compile_features(data_loader_tests PRIVATE cxx_std_20)
    target_include_directories(data_loader_tests PRIVATE src)
    target_compile_definitions(data_loader_tests PRIVATE
            TEST_FIXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures"
    )
    target_link_libraries(data_loader_tests PRIVATE
            nlohmann_json::nlohmann_json
    )

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(data_loader_tests PRIVATE
                -Wall
                -Wextra
                -Wpedantic
        )
    endif()

    add_test(NAME data_loader COMMAND data_loader_tests)

    add_executable(repository_filter_tests
            tests/repository_filter_test.cpp
            src/repository_filter.cpp
    )
    target_compile_features(repository_filter_tests PRIVATE cxx_std_20)
    target_include_directories(repository_filter_tests PRIVATE src)

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(repository_filter_tests PRIVATE
                -Wall
                -Wextra
                -Wpedantic
        )
    endif()

    add_test(NAME repository_filter COMMAND repository_filter_tests)
        add_executable(reporter_tests
            tests/reporter_test.cpp
            src/reporter.cpp
    )
    target_compile_features(reporter_tests PRIVATE cxx_std_20)
    target_include_directories(reporter_tests PRIVATE src)

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(reporter_tests PRIVATE
                -Wall
                -Wextra
                -Wpedantic
        )
    endif()

    add_test(NAME reporter COMMAND reporter_tests)

    add_executable(json_exporter_tests
            tests/json_exporter_test.cpp
            src/json_exporter.cpp
    )
    target_compile_features(json_exporter_tests PRIVATE cxx_std_20)
    target_include_directories(json_exporter_tests PRIVATE src)
    target_link_libraries(json_exporter_tests PRIVATE
            nlohmann_json::nlohmann_json
    )

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(json_exporter_tests PRIVATE
                -Wall
                -Wextra
                -Wpedantic
        )
    endif()

    add_test(NAME json_exporter COMMAND json_exporter_tests)
endif()
