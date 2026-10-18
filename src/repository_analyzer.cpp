#include "repository_analyzer.h"

#include <algorithm>
#include <stdexcept>

bool compare_by_stars(const Repository& a, const Repository& b) {
    if (a.stars != b.stars) {
        return a.stars > b.stars;
    }
    if (a.owner != b.owner) {
        return a.owner < b.owner;
    }
    return a.name < b.name;
}

bool compare_by_forks(const Repository& a, const Repository& b) {
    if (a.forks != b.forks) {
        return a.forks > b.forks;
    }
    if (a.owner != b.owner) {
        return a.owner < b.owner;
    }
    return a.name < b.name;
}

bool compare_by_updated(const Repository& a, const Repository& b) {
    if (a.updated_at != b.updated_at) {
        return a.updated_at > b.updated_at;
    }
    if (a.owner != b.owner) {
        return a.owner < b.owner;
    }
    return a.name < b.name;
}

void sort_repositories(std::vector<Repository>& rows, const std::string& sort_key) {
    if (sort_key == "stars") {
        std::sort(rows.begin(), rows.end(), compare_by_stars);
    } else if (sort_key == "forks") {
        std::sort(rows.begin(), rows.end(), compare_by_forks);
    } else if (sort_key == "updated") {
        std::sort(rows.begin(), rows.end(), compare_by_updated);
    } else {
        throw std::invalid_argument("Unsupported sot key: " + sort_key);
    }
}

void select_top_n(std::vector<Repository>& rows, std::size_t top_n) {
    if (rows.size() > top_n) {
        rows.resize(top_n);
    }
}