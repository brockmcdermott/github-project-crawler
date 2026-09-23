#include "search_options.h"

#include <stdexcept>

CommandLineOptions parse_command_line(std::span<const std::string_view> arguments)
{
    if (arguments.empty()) {
        return {};
    }

    if (arguments.size() > 1) {
        throw std::invalid_argument(
            "expected at most one search query; wrap queries containing spaces in quotes");
    }

    const std::string_view argument = arguments.front();
    if (argument == "-h" || argument == "--help") {
        CommandLineOptions options;
        options.show_help = true;
        return options;
    }

    if (argument.empty()) {
        throw std::invalid_argument("search query cannot be empty");
    }

    CommandLineOptions options;
    options.search.query = argument;
    return options;
}

std::string command_line_usage(std::string_view executable_name)
{
    return "Usage: " + std::string(executable_name) + " [SEARCH_QUERY]\n"
        "\n"
        "Collect up to 100 public repositories matching a GitHub search query.\n"
        "The default query is stars:>0. Quote queries that contain spaces.\n"
        "\n"
        "Examples:\n"
        "  " + std::string(executable_name) + "\n"
        "  " + std::string(executable_name) + " 'stars:>1000'\n";
}
