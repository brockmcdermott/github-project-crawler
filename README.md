# GitHub Project Crawler

## Project Topic

GitHub Project Crawler: a C++ command-line application for exploring and collecting information about GitHub projects.

## Team

| Role | Full Name | A-Number | USU Email |
| --- | --- | --- | --- |
| Team Leader | Brock McDermott | A02370118 | a02370118@usu.edu |
| Programmer | Jameson Irwin | A02309411 | a02309411@aggies.usu.edu |
| Programmer | Kevin Silver | A02416055 | a02416055@usu.edu |

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

## Milestone 2 Analysis

Analysis mode works on a local .json file dataset. It does not make any GitHub API calls.
The process order is:
Load -> Filter -> Sort -> Top N -> Report -> Export

Example:

```bash
./build/github_project_crawler analyze \
  --input repositories.json \
  --min-stars 1000 \
  --language C++ \
  --sort stars \
  --top 5 \
  --output selected.json
```

### Options
```text
  --input PATH       Dataset path (default: repositories.json)
  --min-stars N      Keep repositories with at least N stars (default: 0)
  --language NAME    Keep only repositories using NAME
  --sort KEY         Sort by stars, forks, or updated (default: stars)
  --top N            Select at most N repositories (default: 10)
  --output PATH      Export path (default: selected.json)
  -h, --help         Show this help message
```

### How selection works

1. The dataset is loaded into `Repository` objects.
2. Repositories are then filtered out based on criteria like minimum stars, or language.
3. The remaining repositories are sorted in descending order by the chosen key ties broken by the owner then repo name in ascending order.
4. The first N results are kept. All are kept if there are fewer than N matches.

### Output

The terminal gives a tabular summary of results in the following format. 
```text
GitHub Project Crawler - Analysis
Dataset: sample.json
Repositories loaded: 5
Filter: stars >= 0
Language: any
Sort: stars (descending)
Top N: 10
Output: selected.json
Repositories Selected: 5
  1. amy/beta                                       | stars:     5000 | forks:      300 | C++        | updated: 2026-10-10T08:30:00Z
  2. zed/alpha                                      | stars:     5000 | forks:      800 | C++        | updated: 2026-10-01T12:00:00Z
  3. bob/gamma                                      | stars:     2500 | forks:      300 | Python     | updated: 2026-09-15T20:45:00Z
  4. cat/delta                                      | stars:     1200 | forks:       50 | (none)     | updated: 2026-10-18T03:10:00Z
  5. dan/epsilon                                    | stars:      800 | forks:      120 | Rust       | updated: 2026-08-20T16:00:00Z
Exported 5 repositories to selected.json
```
The same selected repos in that order are written to json as follows
```json
[
  {
    "description": "Ties with alpha on stars",
    "forks": 300,
    "language": "C++",
    "name": "beta",
    "owner": "amy",
    "size": 450,
    "stars": 5000,
    "updated_at": "2026-10-10T08:30:00Z",
    "url": "https://github.com/amy/beta"
  },
  {
    "description": "First repo",
    "forks": 800,
    "language": "C++",
    "name": "alpha",
    "owner": "zed",
    "size": 1200,
    "stars": 5000,
    "updated_at": "2026-10-01T12:00:00Z",
    "url": "https://github.com/zed/alpha"
  },
  {
    "description": "A Python project",
    "forks": 300,
    "language": "Python",
    "name": "gamma",
    "owner": "bob",
    "size": 90,
    "stars": 2500,
    "updated_at": "2026-09-15T20:45:00Z",
    "url": "https://github.com/bob/gamma"
  },
  {
    "description": "",
    "forks": 50,
    "language": "",
    "name": "delta",
    "owner": "cat",
    "size": 15,
    "stars": 1200,
    "updated_at": "2026-10-18T03:10:00Z",
    "url": "https://github.com/cat/delta"
  },
  {
    "description": "A Rust project",
    "forks": 120,
    "language": "Rust",
    "name": "epsilon",
    "owner": "dan",
    "size": 700,
    "stars": 800,
    "updated_at": "2026-08-20T16:00:00Z",
    "url": "https://github.com/dan/epsilon"
  }
]
```

if nothing matches, the output file contains "[]"


## Clean Checkout Notes

A clean checkout should build successfully after Clang/Clang++, CMake, and `cpp-httplib` are installed and available to CMake. Network access is required the first time CMake downloads `nlohmann/json`.
