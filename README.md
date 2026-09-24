# GitHub Project Crawler

## Project Topic

GitHub Project Crawler: a C++ command-line application for exploring and collecting information about GitHub projects.

## Team

| Role | Full Name | A-Number | USU Email |
| --- | --- | --- | --- |
| Team Leader | Brock McDermott | A02370118 | a02370118@usu.edu |
| Programmer | Jameson Irwin | A02309411 | a02309411@aggies.usu.edu |
| Programmer | Kevin Silver | A02416055 | a02416065@usu.edu |

## Build Instructions

This project uses C++20, Clang/Clang++, CMake, Git, SSL-enabled `cpp-httplib`, and `nlohmann/json`. CMake downloads the required `nlohmann/json` version during configuration.

On macOS with Homebrew, install the required tools and libraries with:

```bash
brew install cmake cpp-httplib
```

From the root of the repository, run:

```bash
./build.sh
```

The script configures the project with CMake and builds it into the `build/` directory.

Run the automated tests with:

```bash
ctest --test-dir build --output-on-failure
```

## Run Instructions

After building, run with the default 100-repository options from the repository root:

```bash
./build/github_project_crawler
```

Supply one quoted GitHub repository search query to change the search criteria:

```bash
./build/github_project_crawler 'stars:>1000'
./build/github_project_crawler 'language:c++ stars:>1000'
```

The default search query is `stars:>1000`. The repository count is fixed at 100 for Milestone 1.

Until the teammate-owned API client, parser, and store are integrated, the executable validates and displays these options without contacting GitHub.

GitHub public data can be requested without authentication, but a token increases the normal API rate limit. Set it in the environment before running:

```bash
export GITHUB_TOKEN='your-token-here'
./build/github_project_crawler
```

Never place a GitHub token in source code or commit it to the repository.

## Clean Checkout Notes

A clean checkout should build successfully after Clang/Clang++, CMake, and `cpp-httplib` are installed and available to CMake. Network access is required the first time CMake downloads `nlohmann/json`.
