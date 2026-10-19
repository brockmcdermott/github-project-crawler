#include "repository_filter.h"

bool matches_filters(const Repository& repository, const AnalysisOptions& options)
{
    if (repository.stars < options.min_stars) {
        return false;
    }
    if (!options.language.empty() && repository.language != options.language) {
        return false;
    }
    return true;
}

std::vector<Repository> filter_repositories(
    const std::vector<Repository>& input,
    const AnalysisOptions& options)
{
    std::vector<Repository> result;
    result.reserve(input.size());

    for (const Repository& repository : input) {
        if (matches_filters(repository, options)) {
            result.push_back(repository);
        }
    }

    return result;
}
