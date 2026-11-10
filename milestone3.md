# Milestone 3 Work Plan

## Goal

Produce one short technical report named `design.md` that proposes an object-oriented refactoring of the existing GitHub Project Crawler. Milestone 3 is a design milestone: the team will describe the proposed classes, relationships, interactions, and tests, but will not implement the refactoring yet.

The final report must contain:

1. Requirements analysis with clear, testable requirement IDs.
2. System design with class responsibilities, interfaces, relationships, and interaction diagrams.
3. Test design with targets, setup, expected results, edge cases, and requirement coverage.
4. An optional Strategy-pattern section for bonus credit.

This file assigns the work. The submitted artifact will be `design.md` after the three parts are combined and reviewed.

## Shared Design Direction

The current project already supports two workflows:

- Collection: query GitHub, parse repository data, print it, and save `repositories.json`.
- Analysis: load the local dataset, filter, sort, select top N, report results, and export JSON.

The proposed design should preserve this behavior while replacing loosely related free functions and namespace functions with classes that have clear responsibilities. The team should evaluate and document a structure similar to:

- `CrawlerApplication`: coordinates collection and analysis workflows.
- `CommandLineParser`: converts command-line input into collection or analysis options.
- `RepositorySource`: common source interface for acquiring repository values.
- `GitHubRepositorySource`: obtains repositories through `GitHubClient`.
- `JsonRepositorySource`: loads repositories from a local JSON dataset.
- `RepositoryJsonCodec`: owns JSON conversion shared by loading and saving.
- `RepositoryAnalyzer`: coordinates filtering, sorting, and top-N selection.
- `SortStrategy`: interface implemented by stars, forks, and updated-time strategies.
- `RepositoryReporter`: prints selected repositories and run summaries.
- `RepositoryExporter`: writes the selected collection to JSON.
- `Repository`: the existing repository value object.

These names are the shared design contract used by the `design.md` outline. The final design document must explain why each class exists and how it improves the current system.

## Current Part Selections

- Part 1: Jameson Irwin.
- Part 2: Brock McDermott.
- Part 3: Unassigned.

Part 1 and Part 2 can proceed at the same time because the requirement IDs, class names, and interface boundaries are fixed in `design.md`.

## Part 1: Requirements and Data Acquisition Design

### Responsibilities

- Write the report overview and requirements-analysis section.
- Inspect the existing collection and analysis behavior before defining requirements.
- Give every major requirement a stable ID such as `R1`, `R2`, and `R3`.
- Make each requirement observable and testable rather than describing implementation details.
- Cover at least:
  - GitHub collection and optional token use.
  - Repository parsing and all required metadata.
  - Local dataset saving and loading.
  - Minimum-star and language filtering.
  - Stars, forks, and updated-time sorting.
  - Top-N selection.
  - Terminal reporting and JSON export of the same selected values.
  - Invalid options, API failures, malformed data, and file errors.
- Design the data-acquisition side of the class model.
- Specify responsibilities, important state, and public interfaces for:
  - `Repository`.
  - Collection and analysis option value types.
  - `CommandLineParser`.
  - `RepositorySource`.
  - `GitHubRepositorySource`.
  - `JsonRepositorySource`.
  - `RepositoryJsonCodec`.
- Explain how GitHub collection and local loading use the same `Repository` representation.
- Add a collection interaction or sequence diagram.
- Provide representative C++ declarations for the acquisition interfaces.
- Map the acquisition classes back to the relevant requirement IDs.

### Deliverables

- Overview text for `design.md`.
- Numbered requirements table.
- Data-acquisition class descriptions and declarations.
- Collection interaction diagram.
- Initial requirements-to-classes mapping.

### Acceptance Checks

- Every requirement has an ID and a testable outcome.
- Requirements describe current required behavior, not imagined extra features.
- The design does not create a second repository data representation.
- Network access, JSON conversion, and application coordination have separate responsibilities.
- Error behavior is included in the requirements.

## Part 2: Analysis Architecture, UML, and Strategy Pattern

### Responsibilities

- Design the analysis side of the object-oriented model.
- Specify responsibilities, important state, and public interfaces for:
  - `RepositoryAnalyzer`.
  - Filter behavior.
  - Top-N selection.
  - `SortStrategy`.
  - `StarsSortStrategy`.
  - `ForksSortStrategy`.
  - `UpdatedSortStrategy`.
- Preserve the required order: Filter -> Sort -> Top N.
- Preserve deterministic owner/name tie-breaking for every sort mode.
- Create the primary UML class diagram for the complete proposed system.
- Label relationships accurately, including inheritance, composition, ownership, and ordinary usage.
- Show how `CrawlerApplication` depends on abstractions instead of knowing each algorithm's details.
- Add an analysis interaction or sequence diagram from local loading through selected results.
- Include representative C++ declarations for the analyzer and strategy interfaces.
- Write the optional design-pattern section for bonus credit:
  - Identify the design problem.
  - Explain why Strategy is appropriate.
  - Identify participating classes.
  - Explain how a strategy is selected.
  - Explain how the pattern makes new sort modes easier to add and test.
- Map analysis classes and interactions to the filtering, sorting, and top-N requirement IDs.

### Deliverables

- Analysis-class descriptions and declarations.
- Complete UML class diagram.
- Analysis interaction diagram.
- Strategy-pattern bonus section.
- Analysis requirements-to-classes mapping.

### Acceptance Checks

- The diagram agrees with the written class declarations.
- Every relationship is explained in prose.
- Strategy solves a real variation point rather than being named only for bonus credit.
- Sorting remains deterministic and selectable at runtime.
- Analysis logic remains independent from printing and exporting.

## Part 3: Application Coordination, Output Design, and Test Plan

### Responsibilities

- Design the application-coordination and output side of the class model.
- Specify responsibilities, important state, and public interfaces for:
  - `CrawlerApplication`.
  - `RepositoryReporter`.
  - `RepositoryExporter`.
- Explain how collection mode and analysis mode enter separate workflows through the same application controller.
- Explain how the reporter and exporter receive the exact same selected collection.
- Add or complete the end-to-end interaction diagram for both modes.
- Write the complete test-design section using planned tests rather than implemented test results.
- Give every planned test an ID such as `T1`, `T2`, and `T3`.
- For every test, provide:
  - Target requirement or component.
  - Input and setup.
  - Expected result.
- Include normal, error, and edge cases for:
  - Successful GitHub collection.
  - API or network failure using a fake source rather than a live dependency.
  - Valid and malformed local datasets.
  - Minimum-star and language filters.
  - All three sort strategies and deterministic ties.
  - Top N, fewer than N, zero matches, and empty input.
  - Reporter/exporter consistency.
  - Output-file failure.
  - End-to-end collection and analysis workflows.
- Create a requirement-to-test traceability table showing that every requirement is covered.
- Combine the three parts into `design.md` and perform the final consistency edit.

### Deliverables

- Application/output class descriptions and declarations.
- Completed workflow interaction descriptions.
- Planned test-case table.
- Requirements-to-tests coverage table.
- Integrated draft of `design.md`.

### Acceptance Checks

- Tests state expected behavior instead of merely saying the program should run.
- Tests do not depend on live GitHub access when a fake or controlled fixture is suitable.
- Every requirement is covered by at least one planned test.
- Reporter and exporter tests verify identical repository identity, order, and count.
- The report describes planned future tests and does not claim unexecuted results.

## Shared Contract

Use the following decisions throughout all three parts:

1. Requirement IDs are `R1` through `R10` as defined in `design.md`; do not renumber them.
2. Repository acquisition uses `IRepositorySource`, implemented by `GitHubRepositorySource` and `JsonRepositorySource`.
3. JSON dataset conversion belongs to `RepositoryJsonCodec`; GitHub response conversion remains in `GitHubResponseParser` because the schemas differ.
4. Output uses `RepositoryReporter` and `IRepositoryExporter`; `JsonRepositoryExporter` implements the exporter interface.
5. Sorting uses `SortStrategy`, implemented by `StarsSortStrategy`, `ForksSortStrategy`, and `UpdatedSortStrategy`.
6. `RepositoryAnalyzer` performs Filter -> Sort -> Top N and receives a strategy through its public interface.
7. `CrawlerApplication` coordinates workflows but does not implement networking, parsing, analysis algorithms, reporting, or serialization.
8. UML uses Mermaid `classDiagram`; interactions use Mermaid `sequenceDiagram`.
9. Solid relationship arrows represent inheritance or composition as labeled in the prose; dashed arrows represent ordinary usage dependencies.

These contracts allow all three students to write in parallel.

## Integration Order

1. Jameson and Brock write Parts 1 and 2 in parallel using the shared `design.md` contract.
2. The Part 3 contributor writes application/output design and the test plan.
3. Brock completes the full UML and Strategy-pattern explanation.
4. The Part 3 contributor assembles the test and traceability tables.
5. Jameson checks every requirement against the final class design and planned tests.
6. The entire team removes outline notes and reviews the report before submission.

## Rubric Coverage

| Grading Category | Work Part | Evidence in `design.md` |
| --- | --- | --- |
| Requirements analysis - 5 points | Part 1 | Numbered, testable requirements table |
| System design - 12 points | Parts 1, 2, and 3 | Class descriptions, declarations, UML, and interactions |
| Test design - 8 points | Part 3 | Planned tests and requirement coverage table |
| Optional Strategy bonus - 5 design points | Part 2 | Problem, participants, selection, and expected improvement |

## Final Checklist

- The final file is named `design.md` and is committed under the repository root.
- It contains Overview, Requirements, System Design, and Test Design sections.
- Requirements use stable IDs and describe observable behavior.
- Every major class has a responsibility, important state, and public interface.
- UML and sequence diagrams match the prose and C++ declarations.
- Relationships identify inheritance, composition, ownership, and usage correctly.
- The design explains how classes work together for both application modes.
- Test cases include targets, setup, and explicit expected results.
- Normal, error, and edge cases are covered.
- Every requirement maps to at least one class and at least one planned test.
- The optional Strategy pattern is justified as a meaningful design choice.
- The report does not claim that the proposed refactoring or planned tests were already implemented.
- All three team members review and can explain the final design decisions.
