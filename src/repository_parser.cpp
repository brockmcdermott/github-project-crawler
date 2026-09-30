#include "repository_parser.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<Repository> RepositoryParser::parse(const std::string& json_body) {
    json root = json::parse(json_body);

    const auto& items = root.at ("items");

    std::vector<Repository> repositories;
    repositories.reserve(items.size());

    for (const auto& item : items) {
        Repository repo;
        repo.owner = item.at("owner").at("login").get<std::string>();
        repo.name = item.at("name").get<std::string>();
        repo.description = item.at("description").is_null()
         ? ""
            : item.at("description").get<std::string>();
        repo.stars = item.at("stargazers_count").get<std::uint64_t>();
        repo.forks = item.at("forks_count").get<std::uint64_t>();
        repo.language = item.at("language").is_null()
            ? ""
            : item.at("language").get<std::string>();
        repo.size = item.at("size").get<std::uint64_t>();
        repo.updated_at = item.at("updated_at").get<std::string>();
        repo.url = item.at("html_url").get<std::string>();

        repositories.push_back(repo);
    }

    return repositories;
}