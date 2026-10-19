#include "analysis_options.h"
#include "search_options.h"

#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

int main(int argc, char* argv[])
{
    std::vector<std::string_view> arguments;
    arguments.reserve(static_cast<std::size_t>(argc > 0 ? argc - 1 : 0));

    const std::string_view executable_name = argc > 0 && argv[0] != nullptr
        ? argv[0]
        : "github_project_crawler";

    for (int index = 1; index < argc; ++index) {
        arguments.emplace_back(argv[index]);
    }

    try {
        if (!arguments.empty() && arguments.front() == "analyze") {
            const std::span<const std::string_view> analysis_arguments(arguments);
            const auto command_line = parse_analysis_command_line(
                analysis_arguments.subspan(1));

            if (command_line.show_help) {
                std::cout << analysis_command_line_usage(executable_name);
                return 0;
            }

            const AnalysisOptions& options = command_line.analysis;
            std::cout << "GitHub Project Crawler - Analysis\n"
                      << "Dataset: " << options.input_path.string() << '\n'
                      << "Filter: stars >= " << options.min_stars << '\n'
                      << "Language: "
                      << (options.language.empty() ? "any" : options.language) << '\n'
                      << "Sort: " << options.sort_key << " (descending)\n"
                      << "Top N: " << options.top_n << '\n'
                      << "Output: " << options.output_path.string() << '\n'
                      << "Analysis options are valid.\n";
            return 0;
        }

        const auto command_line = parse_command_line(arguments);

        if (command_line.show_help) {
            std::cout << command_line_usage(executable_name);
            return 0;
        }

        std::cout << "GitHub Project Crawler\n"
                  << "Search query: " << command_line.search.query << '\n'
                  << "Repository count: " << command_line.search.count << '\n'
                  << "Collection pipeline is ready for client, parser, and store integration.\n";
    } catch (const std::invalid_argument& error) {
        std::cerr << "Error: " << error.what() << "\n\n";
        if (!arguments.empty() && arguments.front() == "analyze") {
            std::cerr << analysis_command_line_usage(executable_name);
        } else {
            std::cerr << command_line_usage(executable_name);
        }
        return 2;
    }

    return 0;
}
