#include "repository_filter.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

int failures = 0;

void expect(bool condition, std::string_view message)
{
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

Repository make_repository(
    std::string owner,
    std::string name,
    std::uint64_t stars,
    std::string language)
{
    Repository repository;
    repository.owner = std::move(owner);
    repository.name = std::move(name);
    repository.stars = stars;
    repository.language = std::move(language);
    return repository;
}

const std::vector<Repository>& sample_repositories()
{
    static const std::vector<Repository> repositories{
        make_repository("alpha", "high-cpp", 5000, "C++"),
        make_repository("beta", "python", 2500, "Python"),
        make_repository("gamma", "boundary-cpp", 1000, "C++"),
        make_repository("delta", "small-cpp", 500, "C++"),
    };
    return repositories;
}

void test_minimum_stars()
{
    AnalysisOptions options;
    options.min_stars = 1000;

    const auto result = filter_repositories(sample_repositories(), options);

    expect(result.size() == 3, "minimum-stars filter keeps qualifying repositories");
    expect(result.back().name == "boundary-cpp", "minimum-stars boundary is inclusive");
}

void test_language()
{
    AnalysisOptions options;
    options.language = "C++";

    const auto result = filter_repositories(sample_repositories(), options);

    expect(result.size() == 3, "language filter keeps exact language matches");
    for (const Repository& repository : result) {
        expect(repository.language == "C++", "every language result matches C++");
    }
}

void test_combined_filters()
{
    AnalysisOptions options;
    options.min_stars = 1000;
    options.language = "C++";

    const auto result = filter_repositories(sample_repositories(), options);

    expect(result.size() == 2, "star and language filters combine");
    expect(result[0].name == "high-cpp" && result[1].name == "boundary-cpp",
           "filtering preserves input order for the analyzer");
}

void test_empty_language_allows_any()
{
    AnalysisOptions options;

    const auto result = filter_repositories(sample_repositories(), options);
    expect(result.size() == sample_repositories().size(), "empty language allows any language");
}

void test_zero_matches()
{
    AnalysisOptions options;
    options.min_stars = 1000000;

    expect(filter_repositories(sample_repositories(), options).empty(),
           "filtering can return zero matches");
}

void test_matches_filters()
{
    const Repository repository = make_repository("owner", "repo", 1000, "Rust");
    AnalysisOptions options;
    options.min_stars = 1000;
    options.language = "Rust";

    expect(matches_filters(repository, options), "single repository can satisfy both filters");
    options.language = "rust";
    expect(!matches_filters(repository, options), "language matching is exact");
}

} // namespace

int main()
{
    test_minimum_stars();
    test_language();
    test_combined_filters();
    test_empty_language_allows_any();
    test_zero_matches();
    test_matches_filters();

    if (failures == 0) {
        std::cout << "All repository filter tests passed.\n";
    }

    return failures == 0 ? 0 : 1;
}
