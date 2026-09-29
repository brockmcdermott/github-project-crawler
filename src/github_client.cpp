#include "github_client.h"

#include <cstdlib>
#include <iostream>

#include <httplib.h>

std::optional<std::string> GitHubClient::search_repositories(const SearchOptions& options) const {
    httplib::Client client("https://api.github.com");
    client.set_follow_location(true);
    client.set_connection_timeout(10);
    client.set_read_timeout(20);

    std::string path = "/search/repositories?q=" + httplib::encode_uri_component(options.query) + "&per_page=" + std::to_string(options.count);
    if (!options.sort.empty()) {
        path += "&sort=" + options.sort;
    }
    if (!options.order.empty()) {
        path += "&order=" + options.order;
    }

    httplib::Headers headers{
    {"Accept", "application/vnd.github+json"},
    {"X-GitHub-Api-Version", "2026-03-10"},
    {"User-Agent", "cs3460-github-project-crawler"},
    };

    if (const char* token = std::getenv("GITHUB_TOKEN")) {
        headers.emplace("Authorization", std::string("Bearer ") + token);
    }

    httplib::Result result = client.Get(path, headers);

    if (!result) {
        std::cerr << "Error: request to GitHub failed: " << httplib::to_string(result.error()) << '\n';
        return std::nullopt;
    }

    if (result->status != 200) {
        std::cerr << "Error: Github API returned HTTP " << result->status << '\n' << result->body << '\n';
        return std::nullopt;
    }

    return result->body;
}