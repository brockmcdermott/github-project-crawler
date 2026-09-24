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

find_package(httplib CONFIG REQUIRED)

add_executable(github_project_crawler
    src/main.cpp
    src/search_options.cpp
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
endif()
