#include "repository.h"
#include "search_options.h"

#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace {

int failures = 0;

void expect(bool condition, std::string_view message)
{
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

void test_defaults()
{
    const std::vector<std::string_view> arguments;
    const auto options = parse_command_line(arguments);

    expect(options.search.query == "stars:>0", "default query is stars:>0");
    expect(options.search.count == 100, "default repository count is 100");
    expect(!options.show_help, "default invocation does not show help");
}

void test_custom_query()
{
    const std::vector<std::string_view> arguments{"language:c++ stars:>1000"};
    const auto options = parse_command_line(arguments);

    expect(options.search.query == "language:c++ stars:>1000", "custom query is preserved");
    expect(options.search.count == 100, "custom query keeps the 100-repository count");
}

void test_help()
{
    const std::vector<std::string_view> arguments{"--help"};
    const auto options = parse_command_line(arguments);

    expect(options.show_help, "--help requests usage output");
}

void test_invalid_arguments()
{
    try {
        const std::vector<std::string_view> arguments{"stars:>1000", "language:c++"};
        static_cast<void>(parse_command_line(arguments));
        expect(false, "multiple arguments are rejected");
    } catch (const std::invalid_argument&) {
    }

    try {
        const std::vector<std::string_view> arguments{""};
        static_cast<void>(parse_command_line(arguments));
        expect(false, "empty query is rejected");
    } catch (const std::invalid_argument&) {
    }
}

void test_repository_defaults()
{
    const Repository repository;

    expect(repository.stars == 0, "repository stars default to zero");
    expect(repository.forks == 0, "repository forks default to zero");
    expect(repository.size == 0, "repository size defaults to zero");
}

} // namespace

int main()
{
    test_defaults();
    test_custom_query();
    test_help();
    test_invalid_arguments();
    test_repository_defaults();

    if (failures == 0) {
        std::cout << "All search option tests passed.\n";
    }

    return failures == 0 ? 0 : 1;
}
