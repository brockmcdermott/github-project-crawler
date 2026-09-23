#ifndef GITHUB_PROJECT_CRAWLER_SEARCH_OPTIONS_H
#define GITHUB_PROJECT_CRAWLER_SEARCH_OPTIONS_H

#include <cstddef>
#include <span>
#include <string>
#include <string_view>

inline constexpr std::string_view default_search_query{"stars:>0"};
inline constexpr std::size_t default_repository_count{100};

struct SearchOptions {
    std::string query{default_search_query};
    std::size_t count{default_repository_count};
};

struct CommandLineOptions {
    SearchOptions search;
    bool show_help{};
};

CommandLineOptions parse_command_line(std::span<const std::string_view> arguments);
std::string command_line_usage(std::string_view executable_name);

#endif
