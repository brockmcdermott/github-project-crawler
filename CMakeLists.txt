cmake_minimum_required(VERSION 3.20)

project(GitHubProjectCrawler VERSION 0.1.0 LANGUAGES C CXX)

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

add_executable(github_project_crawler
    src/main.cpp
)

#tell CMAKE that this dependancy belongs to this executable
target_link_libraries(github_project_crawler PRIVATE
        nlohmann_json::nlohmann_json
)

target_compile_features(github_project_crawler PRIVATE cxx_std_20)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(github_project_crawler PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

