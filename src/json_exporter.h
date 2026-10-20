
#pragma once

#include <filesystem>
#include <ostream>
#include <vector>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

#include "repository.h"

namespace JsonExporter {

nlohmann::json to_json(const std::vector<Repository>& selected);

void write(const std::vector<Repository>& selected, std::ostream& out);

void save(const std::vector<Repository>& selected, const std::filesystem::path& path);


}
