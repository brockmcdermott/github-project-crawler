#pragma once

#include <cstddef>
#include <span>
#include <string>
#include <string_view>

inline constexpr std::string_view default_search_query{"stars:>1000"};
inline constexpr std::size_t default_repository_count{100};

struct SearchOptions {
    std::string query{default_search_query};
    std::size_t count{default_repository_count};
    std::string sort{"stars"};
    std::string order{"desc"};
};

struct CommandLineOptions {
    SearchOptions search;
    bool show_help{};
};

CommandLineOptions parse_command_line(std::span<const std::string_view> arguments);
std::string command_line_usage(std::string_view executable_name);
