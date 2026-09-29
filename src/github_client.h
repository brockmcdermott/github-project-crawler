#pragma once

#include <optional>
#include <string>

#include "search_options.h"

class GitHubClient {
public:
    std::optional<std::string> search_repositories(const SearchOptions& options) const;

};