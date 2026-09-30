#pragma once
#include <string>
#include <vector>
#include "repository.h"

namespace RepositoryStore {

    void print(const std::vector<Repository>& repositories);
    void save(const std::vector<Repository>& repositories, const std:: string& path);

}