#include "data_loader.h"

#include <fstream>
#include <stdexcept>
#include <utility>

#include <nlohmann/json.hpp>

namespace {

Repository repository_from_json(const nlohmann::json& item)
{
    Repository repository;
    repository.owner = item.at("owner").get<std::string>();
    repository.name = item.at("name").get<std::string>();
    repository.description = item.at("description").get<std::string>();
    repository.stars = item.at("stars").get<std::uint64_t>();
    repository.forks = item.at("forks").get<std::uint64_t>();
    repository.language = item.at("language").get<std::string>();
    repository.size = item.at("size").get<std::uint64_t>();
    repository.updated_at = item.at("updated_at").get<std::string>();
    repository.url = item.at("url").get<std::string>();
    return repository;
}

} // namespace

std::vector<Repository> DataLoader::load_repositories(const std::filesystem::path& path)
{
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error(
            "Cannot open repository dataset: " + path.string());
    }

    try {
        nlohmann::json root;
        input >> root;

        if (!root.is_array()) {
            throw std::runtime_error(
                "Repository dataset must be a JSON array: " + path.string());
        }

        std::vector<Repository> repositories;
        repositories.reserve(root.size());

        for (const nlohmann::json& item : root) {
            repositories.push_back(repository_from_json(item));
        }

        return repositories;
    } catch (const nlohmann::json::exception& error) {
        throw std::runtime_error(
            "Invalid repository dataset '" + path.string() + "': " + error.what());
    }
}
