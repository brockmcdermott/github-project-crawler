#pragma once

#include "analysis_options.h"
#include "repository.h"

#include <vector>

bool matches_filters(const Repository& repository, const AnalysisOptions& options);
std::vector<Repository> filter_repositories(
    const std::vector<Repository>& input,
    const AnalysisOptions& options);
