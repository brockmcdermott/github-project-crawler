#pragma once

#include "repository.h"

#include <filesystem>
#include <vector>

namespace DataLoader {

std::vector<Repository> load_repositories(const std::filesystem::path& path);

}
