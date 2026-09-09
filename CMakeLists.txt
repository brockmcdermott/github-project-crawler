cmake_minimum_required(VERSION 3.20)

project(GitHubProjectCrawler VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(github_project_crawler
    src/main.cpp
)

target_compile_features(github_project_crawler PRIVATE cxx_std_20)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(github_project_crawler PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
