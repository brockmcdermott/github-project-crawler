#include "analysis_options.h"

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

void expect_invalid(
    const std::vector<std::string_view>& arguments,
    std::string_view message)
{
    try {
        static_cast<void>(parse_analysis_command_line(arguments));
        expect(false, message);
    } catch (const std::invalid_argument&) {
    }
}

void test_defaults()
{
    const std::vector<std::string_view> arguments;
    const auto command_line = parse_analysis_command_line(arguments);

    expect(command_line.analysis.input_path == "repositories.json", "default input path");
    expect(command_line.analysis.min_stars == 0, "default minimum stars");
    expect(command_line.analysis.language.empty(), "default language allows any");
    expect(command_line.analysis.sort_key == "stars", "default sort key");
    expect(command_line.analysis.top_n == 10, "default top N");
    expect(command_line.analysis.output_path == "selected.json", "default output path");
    expect(!command_line.show_help, "default invocation does not show help");
}

void test_all_options()
{
    const std::vector<std::string_view> arguments{
        "--input", "fixtures/repos.json",
        "--min-stars", "1500",
        "--language", "C++",
        "--sort", "updated",
        "--top", "25",
        "--output", "results/selected.json",
    };
    const auto command_line = parse_analysis_command_line(arguments);

    expect(command_line.analysis.input_path == "fixtures/repos.json", "custom input path");
    expect(command_line.analysis.min_stars == 1500, "custom minimum stars");
    expect(command_line.analysis.language == "C++", "custom language");
    expect(command_line.analysis.sort_key == "updated", "custom sort key");
    expect(command_line.analysis.top_n == 25, "custom top N");
    expect(command_line.analysis.output_path == "results/selected.json", "custom output path");
}

void test_help()
{
    const std::vector<std::string_view> arguments{"--help"};
    expect(parse_analysis_command_line(arguments).show_help, "--help requests usage output");
}

void test_invalid_options()
{
    expect_invalid({"--min-stars", "many"}, "non-numeric minimum stars is rejected");
    expect_invalid({"--min-stars", "-1"}, "negative minimum stars is rejected");
    expect_invalid({"--sort", "name"}, "unsupported sort key is rejected");
    expect_invalid({"--top", "0"}, "zero top N is rejected");
    expect_invalid({"--top", "ten"}, "non-numeric top N is rejected");
    expect_invalid({"--input"}, "missing option value is rejected");
    expect_invalid({"--input", "--top", "5"}, "option token is not accepted as a value");
    expect_invalid({"--unknown", "value"}, "unknown option is rejected");
    expect_invalid({"unexpected"}, "positional analysis argument is rejected");
}

} // namespace

int main()
{
    test_defaults();
    test_all_options();
    test_help();
    test_invalid_options();

    if (failures == 0) {
        std::cout << "All analysis option tests passed.\n";
    }

    return failures == 0 ? 0 : 1;
}
