#include "analysis_options.h"

#include <charconv>
#include <limits>
#include <stdexcept>

namespace {

std::string_view option_value(
    std::span<const std::string_view> arguments,
    std::size_t& index)
{
    const std::string_view option = arguments[index];
    if (index + 1 >= arguments.size() || arguments[index + 1].starts_with("--")) {
        throw std::invalid_argument("missing value for " + std::string(option));
    }

    ++index;
    return arguments[index];
}

std::uint64_t parse_unsigned(std::string_view value, std::string_view option)
{
    std::uint64_t result{};
    const char* const begin = value.data();
    const char* const end = value.data() + value.size();
    const auto conversion = std::from_chars(begin, end, result);

    if (value.empty() || conversion.ec != std::errc{} || conversion.ptr != end) {
        throw std::invalid_argument(
            std::string(option) + " must be a non-negative whole number");
    }

    return result;
}

std::size_t parse_top_n(std::string_view value)
{
    const std::uint64_t parsed = parse_unsigned(value, "--top");
    if (parsed == 0) {
        throw std::invalid_argument("--top must be greater than zero");
    }
    if (parsed > std::numeric_limits<std::size_t>::max()) {
        throw std::invalid_argument("--top is too large for this system");
    }

    return static_cast<std::size_t>(parsed);
}

bool is_supported_sort_key(std::string_view sort_key)
{
    return sort_key == "stars" || sort_key == "forks" || sort_key == "updated";
}

} // namespace

AnalysisCommandLineOptions parse_analysis_command_line(
    std::span<const std::string_view> arguments)
{
    AnalysisCommandLineOptions command_line;

    for (std::size_t index = 0; index < arguments.size(); ++index) {
        const std::string_view argument = arguments[index];

        if (argument == "-h" || argument == "--help") {
            command_line.show_help = true;
            return command_line;
        }
        if (argument == "--input") {
            const std::string_view value = option_value(arguments, index);
            if (value.empty()) {
                throw std::invalid_argument("--input path cannot be empty");
            }
            command_line.analysis.input_path = std::string(value);
        } else if (argument == "--min-stars") {
            command_line.analysis.min_stars = parse_unsigned(
                option_value(arguments, index), "--min-stars");
        } else if (argument == "--language") {
            command_line.analysis.language = option_value(arguments, index);
        } else if (argument == "--sort") {
            const std::string_view value = option_value(arguments, index);
            if (!is_supported_sort_key(value)) {
                throw std::invalid_argument(
                    "--sort must be one of: stars, forks, updated");
            }
            command_line.analysis.sort_key = value;
        } else if (argument == "--top") {
            command_line.analysis.top_n = parse_top_n(option_value(arguments, index));
        } else if (argument == "--output") {
            const std::string_view value = option_value(arguments, index);
            if (value.empty()) {
                throw std::invalid_argument("--output path cannot be empty");
            }
            command_line.analysis.output_path = std::string(value);
        } else {
            throw std::invalid_argument("unknown analysis option: " + std::string(argument));
        }
    }

    return command_line;
}

std::string analysis_command_line_usage(std::string_view executable_name)
{
    return "Usage: " + std::string(executable_name) + " analyze [OPTIONS]\n"
        "\n"
        "Analyze the local Milestone 1 repository dataset.\n"
        "\n"
        "Options:\n"
        "  --input PATH       Dataset path (default: repositories.json)\n"
        "  --min-stars N      Keep repositories with at least N stars\n"
        "  --language NAME    Keep only repositories using NAME\n"
        "  --sort KEY         Sort by stars, forks, or updated\n"
        "  --top N            Select at most N repositories (default: 10)\n"
        "  --output PATH      Export path (default: selected.json)\n"
        "  -h, --help         Show this help message\n";
}
