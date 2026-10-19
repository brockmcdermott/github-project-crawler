#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>

struct AnalysisOptions {
    std::filesystem::path input_path{"repositories.json"};
    std::uint64_t min_stars{};
    std::string language;
    std::string sort_key{"stars"};
    std::size_t top_n{10};
    std::filesystem::path output_path{"selected.json"};
};
