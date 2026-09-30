#include "repository_store.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void RepositoryStore::print(const std:: vector<Repository>& repositories) {
    std::size_t index = 1;
    for (const auto& repo : repositories) {
        std::cout << index++ << ". " << repo.owner << "/" << repo.name << '\n'
                 << "  description: " << (repo.description.empty() ? "(none)" : repo.description) << '\n'
                 << "  stars: " << repo.stars << "  forks:  " << repo.forks << '\n'
                 << "  language: " << (repo.language.empty() ? "(none)" : repo.language) << '\n'
                 << "  size: " << repo.size << " KB" << '\n'
                 << "  updated: " << repo.updated_at << '\n'
                 << "  url: " << repo.url << '\n';
    }
}

void RepositoryStore::save(const std::vector<Repository>& repositories, const std:: string& path) {
    json out = json::array();
    for (const auto& repo : repositories) {
        out.push_back({
            {"owner", repo.owner},
            {"name", repo.name},
            {"desciption", repo.description},
            {"stars", repo.stars},
            {"forks", repo.forks},
            {"language", repo.language},
            {"size", repo.size},
            {"updated_at", repo.updated_at},
            {"url", repo.url},
        });
    }

    std::ofstream out_file(path);
    if (!out_file) {
        std::cerr << "Unable to open file " << path << '\n';
        return;
    }
    out_file << out.dump(2) << '\n';
}