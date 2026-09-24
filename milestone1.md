# Milestone 1 Work Plan

## Goal

Build the online repository collection pipeline described in the Milestone 1 specification. The completed command-line program must query the GitHub REST API, convert the response into C++ repository objects, print the collected repository information, and save 100 public repositories as a valid JSON array in `repositories.json`.

## Shared Design

The three parts should connect through this agreed workflow:

1. `main` creates `SearchOptions` from the command line.
2. `GitHubClient` uses those options and returns the GitHub response body.
3. `RepositoryParser` converts the response body into a `std::vector<Repository>`.
4. `RepositoryStore` prints that vector and writes it to `repositories.json`.

Before implementation begins, the team should agree on the header names and public function signatures between these components. Each person should work on a feature branch created from `milestone1` and open a pull request back into `milestone1` when their part is ready.

## Part 1: Brock McDermott - Foundation, CLI, and Integration

**Owner:** Brock McDermott (Team Leader)

This is the first part and includes a slightly larger share of the work because it also covers final integration.

### Primary responsibilities

- Define the shared `Repository` value type with every required field:
  - owner
  - name
  - description
  - stars
  - forks
  - language
  - size
  - updated time
  - URL
- Define `SearchOptions` with a search query and a default count of 100.
- Decide and document the command-line interface used to supply a visible search criterion such as `stars:>1000`.
- Update `main.cpp` to coordinate the client, parser, printing, and storage components after the teammates' parts are merged.
- Update `CMakeLists.txt` so all source files are compiled and the SSL-enabled `cpp-httplib` and `nlohmann/json` CMake packages are linked.
- Keep `build.sh` working from a clean checkout with Clang/Clang++ and C++20 or later.
- Update the README with dependency installation, build, run, optional `GITHUB_TOKEN`, and search-query examples.
- Merge the three parts, resolve integration issues, and perform the final clean-checkout verification.

### Brock's acceptance checks

- The default run requests 100 repositories.
- A user can visibly provide a criterion such as `stars:>1000`.
- The full project builds through `./build.sh`.
- No GitHub token is hard-coded, printed, or committed.
- The final program connects all three teammates' components and handles a failed stage without continuing with invalid data.

## Part 2: Jameson Irwin - GitHub API Client

**Owner:** Jameson Irwin

### Primary responsibilities

- Implement the `GitHubClient` header and source file.
- Use an SSL-enabled `cpp-httplib` client for `https://api.github.com`.
- Build a `GET /search/repositories` request from `SearchOptions`.
- URL-encode the search query and include the requested repository count.
- Send the required GitHub headers:
  - `Accept: application/vnd.github+json`
  - `X-GitHub-Api-Version: 2026-03-10`
  - a clear `User-Agent`
- Read `GITHUB_TOKEN` from the environment when it is present and add the bearer authorization header.
- Return the successful response body through the interface agreed on with Brock.
- Report clear errors for connection failures, non-200 responses, rate limits, and invalid requests.

### Jameson's acceptance checks

- Public collection works without a token when GitHub's unauthenticated limit permits it.
- Authenticated collection uses the environment token without exposing it.
- Queries containing characters such as `:`, `>`, and spaces are encoded correctly.
- API and network failures produce a useful message and do not look like a successful empty result.

## Part 3: Kevin Silver - JSON Parser, Output, and Dataset Storage

**Owner:** Kevin Silver

### Primary responsibilities

- Implement the `RepositoryParser` header and source file using `nlohmann/json`.
- Parse the GitHub response's `items` array into the shared `Repository` type.
- Map every required field and safely convert null descriptions or languages to empty strings.
- Detect malformed JSON or missing/incorrect fields and report a clear parsing error.
- Implement the `RepositoryStore` header and source file.
- Print every repository during collection with owner/name, description, stars, forks, language, size, update time, and URL.
- Serialize the team's `Repository` objects, rather than the raw GitHub response, to `repositories.json`.
- Write the output as a valid, readable JSON array.

### Kevin's acceptance checks

- Null descriptions and languages do not crash parsing.
- Printed output contains all nine required repository fields.
- `repositories.json` opens and parses as a JSON array without errors.
- The saved object count matches the collected vector count.
- Numeric values remain JSON numbers rather than strings.

## Integration Order

1. Brock lands the shared data types, agreed interfaces, and initial command-line flow.
2. Jameson connects the API client to those interfaces.
3. Kevin connects parsing, printing, and file storage.
4. Brock integrates the pull requests, updates build/documentation files, and runs the complete rubric checks.
5. Jameson and Kevin review the integrated run and help fix issues in their owned components.

## Final Team Checklist

- `./build.sh` succeeds from a clean checkout using Clang/Clang++ and C++20 or later.
- The default collection command visibly reports exactly 100 repositories collected.
- Every collected repository is printed with all required fields.
- The run creates `repositories.json` as a valid JSON array containing the team's normalized repository objects.
- A run using `stars:>1000` displays only repositories whose star counts satisfy that criterion.
- HTTPS support works in the installed `cpp-httplib` package.
- No secrets, build output, or local environment files are committed.
- Each team member contributes through their own branch and pull request into `milestone1`.

