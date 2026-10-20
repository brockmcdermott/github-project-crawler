#include <nlohmann/json.hpp>

#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#ifndef CRAWLER_EXE
#error "CRAWLER_EXE must be defined by CMake"
#endif
#ifndef TEST_FIXTURE_DIR
#error "TEST_FIXTURE_DIR must be defined by CMake"
#endif

namespace {

    namespace fs = std::filesystem;
    using json = nlohmann::json;

    int failures = 0;

    void expect(bool condition, std::string_view message)
    {
        if (!condition) {
            std::cerr << "FAIL: " << message << '\n';
            ++failures;
        }
    }

    struct RunResult {
        int exit_code{};
        std::string terminal;
        json exported;
        bool output_exists{};
    };

    std::string read_all(const fs::path& path)
    {
        std::ifstream in(path);
        std::stringstream buffer;
        buffer << in.rdbuf();
        return buffer.str();
    }

    RunResult run_analyze(const std::string& args, const fs::path& output)
    {
        const fs::path log = fs::temp_directory_path() / "analysis_e2e_terminal.txt";
        fs::remove(output);

        const std::string command = std::string("\"") + CRAWLER_EXE + "\" analyze " + args +
                                    " --output \"" + output.string() + "\" > \"" + log.string() +
                                    "\" 2>&1";

        RunResult result;
        result.exit_code = std::system(command.c_str());
        result.terminal = read_all(log);
        result.output_exists = fs::exists(output);
        if (result.output_exists) {
            try {
                result.exported = json::parse(read_all(output));
            } catch (const json::exception&) {
                result.exported = nullptr;
            }
        }
        fs::remove(log);
        return result;
    }

    std::string fixture_path()
    {
        return std::string(TEST_FIXTURE_DIR) + "/repositories.json";
    }

    std::vector<std::string> printed_ids(const std::string& terminal)
    {
        std::vector<std::string> ids;
        std::istringstream lines(terminal);
        std::string line;
        while (std::getline(lines, line)) {
            const auto first = line.find_first_not_of(' ');
            if (first == std::string::npos || !std::isdigit(static_cast<unsigned char>(line[first]))) {
                continue;
            }
            const auto dot = line.find(". ", first);
            if (dot == std::string::npos) {
                continue;
            }
            std::istringstream rest(line.substr(dot + 2));
            std::string id;
            rest >> id;
            ids.push_back(id);
        }
        return ids;
    }

    std::vector<std::string> exported_ids(const json& array)
    {
        std::vector<std::string> ids;
        for (const auto& item : array) {
            ids.push_back(item.at("owner").get<std::string>() + "/" + item.at("name").get<std::string>());
        }
        return ids;
    }

    std::size_t selected_count_from_summary(const std::string& terminal)
    {
        const std::string label = "Repositories Selected: ";
        const auto pos = terminal.find(label);
        if (pos == std::string::npos) {
            return static_cast<std::size_t>(-1);
        }
        return static_cast<std::size_t>(std::stoull(terminal.substr(pos + label.size())));
    }

    void test_terminal_matches_export()
    {
        const auto out = fs::temp_directory_path() / "analysis_e2e_selected.json";
        const auto r = run_analyze("--input \"" + fixture_path() + "\" --sort stars --top 5", out);

        expect(r.exit_code == 0, "analyze exits with 0 on the fixture");
        expect(r.output_exists, "analyze writes the output file");
        expect(r.exported.is_array(), "output file is a valid JSON array");
        expect(!r.exported.empty() && r.exported.size() <= 5, "at most top N repositories are exported");

        for (const auto& item : r.exported) {
            for (const char* key : {"owner", "name", "description", "stars", "forks",
                                    "language", "size", "updated_at", "url"}) {
                expect(item.contains(key), "every exported object has all 9 fields");
            }
        }

        expect(printed_ids(r.terminal) == exported_ids(r.exported),
               "printed rows and exported array are the same repositories in the same order");
        expect(selected_count_from_summary(r.terminal) == r.exported.size(),
               "summary selected count equals exported array length");
        expect(r.terminal.find("Repositories loaded: ") != std::string::npos,
               "summary reports the loaded count");
        fs::remove(out);
    }

    void test_sorted_descending_for_each_key()
    {
        const auto out = fs::temp_directory_path() / "analysis_e2e_sorted.json";
        for (const std::string key : {"stars", "forks"}) {
            const auto r = run_analyze("--input \"" + fixture_path() + "\" --sort " + key + " --top 100", out);
            expect(r.exit_code == 0 && r.exported.is_array(), "sort run succeeds");
            for (std::size_t i = 1; i < r.exported.size(); ++i) {
                expect(r.exported[i - 1][key].get<std::uint64_t>() >= r.exported[i][key].get<std::uint64_t>(),
                       "exported array is descending for the sort key");
            }
        }
        const auto r = run_analyze("--input \"" + fixture_path() + "\" --sort updated --top 100", out);
        for (std::size_t i = 1; i < r.exported.size(); ++i) {
            expect(r.exported[i - 1]["updated_at"].get<std::string>() >= r.exported[i]["updated_at"].get<std::string>(),
                   "updated sort is newest first");
        }
        fs::remove(out);
    }

    void test_filters_hold_in_output()
    {
        const auto out = fs::temp_directory_path() / "analysis_e2e_filtered.json";
        const auto r = run_analyze("--input \"" + fixture_path() + "\" --min-stars 1000 --top 100", out);
        expect(r.exit_code == 0, "min-stars run succeeds");
        for (const auto& item : r.exported) {
            expect(item["stars"].get<std::uint64_t>() >= 1000, "every exported repo meets min stars");
        }

        std::string language;
        for (const auto& item : json::parse(read_all(fixture_path()))) {
            if (!item["language"].get<std::string>().empty()) {
                language = item["language"].get<std::string>();
                break;
            }
        }
        if (!language.empty()) {
            const auto lr = run_analyze("--input \"" + fixture_path() + "\" --language \"" + language + "\" --top 100", out);
            expect(lr.exit_code == 0 && !lr.exported.empty(), "language run selects something");
            for (const auto& item : lr.exported) {
                expect(item["language"] == language, "every exported repo has the requested language");
            }
        }
        fs::remove(out);
    }

    void test_zero_matches_exports_empty_array()
    {
        const auto out = fs::temp_directory_path() / "analysis_e2e_empty.json";
        const auto r = run_analyze("--input \"" + fixture_path() + "\" --min-stars 999999999", out);
        expect(r.exit_code == 0, "zero matches is not an error");
        expect(r.exported.is_array() && r.exported.empty(), "zero matches exports []");
        expect(selected_count_from_summary(r.terminal) == 0, "summary reports 0 selected");
        fs::remove(out);
    }

    void test_bad_inputs_fail_cleanly()
    {
        const auto out = fs::temp_directory_path() / "analysis_e2e_bad.json";

        const auto malformed = run_analyze("--input \"" + std::string(TEST_FIXTURE_DIR) + "/malformed.json\"", out);
        expect(malformed.exit_code != 0, "malformed input fails");
        expect(!malformed.output_exists, "malformed input writes no output file");

        const auto missing = run_analyze("--input \"" + std::string(TEST_FIXTURE_DIR) + "/does_not_exist.json\"", out);
        expect(missing.exit_code != 0, "missing input fails");

        const auto bad_out = run_analyze("--input \"" + fixture_path() + "\"", "/nonexistent_dir_for_test/out.json");
        expect(bad_out.exit_code != 0, "unwritable output path fails");
    }

    void test_exact_orders_on_fixture()
    {
        const auto out = fs::temp_directory_path() / "analysis_e2e_exact.json";
        const std::string in = "--input \"" + fixture_path() + "\" ";

        auto ids = [&](const std::string& args) {
            const auto r = run_analyze(in + args, out);
            expect(r.exit_code == 0, "exact-order run succeeds");
            return exported_ids(r.exported);
        };

        using V = std::vector<std::string>;
        expect(ids("--sort stars") == V{"alpha/cpp-tools", "beta/python-kit", "gamma/small-cpp"},
               "stars order on fixture");
        expect(ids("--sort forks") == V{"alpha/cpp-tools", "beta/python-kit", "gamma/small-cpp"},
               "forks order on fixture");
        expect(ids("--sort updated") == V{"beta/python-kit", "alpha/cpp-tools", "gamma/small-cpp"},
               "updated order on fixture");
        expect(ids("--min-stars 1000 --language C++") == V{"alpha/cpp-tools"},
               "combined filters on fixture");
        expect(ids("--sort stars --top 2") == V{"alpha/cpp-tools", "beta/python-kit"},
               "top N truncates on fixture");
        fs::remove(out);
    }
}

int main()
{
    test_terminal_matches_export();
    test_sorted_descending_for_each_key();
    test_filters_hold_in_output();
    test_zero_matches_exports_empty_array();
    test_bad_inputs_fail_cleanly();
    test_exact_orders_on_fixture();

    if (failures == 0) {
        std::cout << "All analysis end-to-end tests passed.\n";
    }
    return failures == 0 ? 0 : 1;
}
