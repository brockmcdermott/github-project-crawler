# Milestone 2 Work Plan

## Goal

Extend the Milestone 1 GitHub Project Crawler so it analyzes the existing local `repositories.json` dataset. Milestone 2 must not collect another dataset from GitHub. The required processing order is:

1. Load `repositories.json`.
2. Filter the loaded `Repository` objects.
3. Sort the filtered objects.
4. Select the top N objects.
5. Print and export the same selected collection.

All three parts must reuse the existing `Repository` type and `nlohmann/json` configuration from Milestone 1.

## Agreed Interfaces

The team should agree on headers and function signatures before implementation begins. The components should exchange `Repository` and `std::vector<Repository>` values, not raw JSON text.

The shared analysis options should contain at least:

- Minimum stars, defaulting to 0.
- Optional language, where an empty value means any language.
- Sort key: `stars`, `forks`, or `updated`.
- Top-N count, defaulting to 10.
- Input path, defaulting to `repositories.json`.
- Output path, defaulting to `selected.json`.

A suggested command is:

```bash
./build/github_project_crawler analyze \
  --input repositories.json \
  --min-stars 1000 \
  --language C++ \
  --sort stars \
  --top 5 \
  --output selected.json
```

The exact syntax may change, but it must visibly support all required analysis choices and must not conflict with the Milestone 1 collection command.

## Part 1: Input, Options, and Filtering


### Responsibilities

- Add the shared `AnalysisOptions` value type.
- Add command-line parsing and validation for analysis mode.
- Support input path, minimum stars, optional language, sort key, top N, and output path.
- Reject invalid numeric values, unsupported sort keys, missing option values, and unknown options with useful messages.
- Implement `DataLoader` to read the selected local JSON file.
- Verify the top-level JSON value is an array.
- Rebuild the existing `Repository` type from every JSON object.
- Report useful errors for a missing file, malformed JSON, incorrect field types, or missing required fields.
- Implement ordinary `matches_filters` and `filter_repositories` functions.
- Filter by minimum stars and, when supplied, exact language.
- Add focused tests for option parsing, dataset loading, malformed input, minimum-star filtering, language filtering, and zero matches.

### Deliverables

- Analysis options and CLI source/header files.
- Data loader source/header files.
- Filtering source/header files, or the filtering portion of the agreed analyzer files.
- Unit-test fixtures and tests for this part.

### Acceptance checks

- A valid dataset loads into `std::vector<Repository>` and visibly reports its record count.
- The loader does not create a second repository representation.
- Every filtered result meets the minimum-star threshold.
- A language filter returns only the requested language.
- Empty language means all languages are allowed.
- Errors identify the input or option that caused the failure.

## Part 2: Sorting and Top-N Analysis


### Responsibilities

- Implement the sorting and top-N portion of `RepositoryAnalyzer`.
- Define named comparison functions for stars, forks, and update time.
- Do not use lambdas as comparison functions for this milestone.
- Sort all three modes in descending order.
- Use owner and then repository name as deterministic ascending tie-breakers.
- Select the comparison function from `AnalysisOptions.sort_key`.
- Apply top-N only after filtering and sorting.
- Return all matches when fewer than N repositories remain.
- Handle top-N equal to zero consistently with the agreed CLI validation policy.
- Keep analysis independent from terminal printing and file export.
- Add focused tests for every sort mode, deterministic ties, top-N truncation, fewer-than-N results, empty input, and unchanged metadata.

### Deliverables

- Repository analyzer source/header files.
- Named comparison functions for all three required sort keys.
- Unit tests for sorting and top-N behavior.

### Acceptance checks

- Stars, forks, and update-time modes are descending.
- Equal primary values are consistently ordered by owner/name.
- Sorting operates on `Repository` objects rather than JSON.
- Top-N is applied after filtering and sorting.
- Exactly N results are returned when at least N matches exist.
- All remaining results are returned when fewer than N match.

## Part 3: Reporting, Export, and Integration


### Responsibilities

- Implement a terminal reporter for the already-selected repositories.
- Print a visible run summary containing dataset path, loaded count, filters, sort mode, top N, selected count, and export path.
- Print readable repository rows containing at least owner/name, stars, forks, language, and update time.
- Implement JSON export to `selected.json` or the requested output path.
- Export all required metadata fields: owner, name, description, stars, forks, language, size, updated time, and URL.
- Ensure the reporter and exporter receive the same selected vector.
- Do not reload, refilter, resort, or repeat top-N processing inside the exporter.
- Connect the full analysis path in `main`: Load -> Filter -> Sort -> Top N -> Report -> Export.
- Keep the Milestone 1 collection path available and separate from analysis mode.
- Update CMake, README build/run examples, and end-to-end test setup.
- Add integration tests that compare displayed selections with exported selections.

### Deliverables

- Reporter source/header files.
- JSON exporter source/header files. CSV may be added only as an optional extension.
- Integrated analysis mode in `main`.
- CMake and README updates.
- End-to-end tests for the complete local analysis workflow.

### Acceptance checks

- The terminal visibly reports how many repositories were loaded and selected.
- Every selected repository is printed in a readable row.
- The exported file is a valid JSON array.
- The exported array contains exactly the same repositories, in the same order, as the reported selection.
- Exported objects contain all required `Repository` metadata fields.
- Analysis mode performs no GitHub API request.

## Integration Sequence

1. Brock lands `AnalysisOptions`, CLI contracts, `DataLoader`, and filtering.
2. Jameson connects sorting and top-N to the filtered vector.
3. Kevin connects reporting and export to the selected vector and integrates analysis mode.
4. Each owner reviews the integrated behavior associated with their component.
5. The team runs all unit tests and one clean-checkout end-to-end analysis before merging Milestone 2.

## Shared Test Dataset

Create a small committed test fixture separate from the real 100-repository dataset. It should include:

- Multiple languages.
- Star and fork ties.
- Different update timestamps.
- Fewer matches than some top-N requests.
- Repository names that prove owner/name tie-breaking is deterministic.

The fixture must use the same fields and types as the Milestone 1 `repositories.json` output.

## Final Team Checklist

- The project builds from a clean checkout through `./build.sh` with C++20 and Clang/Clang++.
- A normal analysis run reads the existing `repositories.json` and reports the loaded count.
- No GitHub request occurs during the Milestone 2 analysis path.
- Minimum-star and optional language filters work correctly.
- Stars, forks, and updated-time sorts are descending with deterministic ties.
- Top-N occurs after filtering and sorting.
- Fewer than N matches are handled without error.
- The terminal and exported JSON represent the exact same selected collection.
- Exported repositories include every required metadata field.
- Unit and end-to-end tests pass.
- Each student contributes through a separate feature branch and pull request.
