#include "search_options.h"

#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

int main(int argc, char* argv[])
{
    std::vector<std::string_view> arguments;
    arguments.reserve(static_cast<std::size_t>(argc > 0 ? argc - 1 : 0));

    for (int index = 1; index < argc; ++index) {
        arguments.emplace_back(argv[index]);
    }

    try {
        const auto command_line = parse_command_line(arguments);

        if (command_line.show_help) {
            std::cout << command_line_usage(argv[0]);
            return 0;
        }

        std::cout << "GitHub Project Crawler\n"
                  << "Search query: " << command_line.search.query << '\n'
                  << "Repository count: " << command_line.search.count << '\n'
                  << "Collection pipeline is ready for client, parser, and store integration.\n";
    } catch (const std::invalid_argument& error) {
        std::cerr << "Error: " << error.what() << "\n\n"
                  << command_line_usage(argv[0]);
        return 2;
    }

    return 0;
}
