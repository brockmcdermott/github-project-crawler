#include "analysis_options.h"
#include "json_exporter.h"
#include "reporter.h"
#include "repository.h"
#include "repository_analyzer.h"
#include "repository_filter.h"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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

    Repository make_repo(const std::string& owner, const std::string& name,
                         std::uint64_t stars, std::uint64_t forks,
                         const std::string& language, const std::string& updated_at)
    {
        Repository r;
        r.owner = owner;
        r.name = name;
        r.stars = stars;
        r.forks = forks;
        r.language = language;
        r.updated_at = updated_at;
        r.description = "desc " + name;
        r.size = 10;
        r.url = "https://github.com/" + owner + "/" + name;
        return r;
    }

    std::vector<Repository> dataset()
    {
        return {
            make_repo("zed", "alpha", 5000, 800, "C++", "2026-10-01T12:00:00Z"),
            make_repo("amy", "beta", 5000, 300, "C++", "2026-10-10T08:30:00Z"),
            make_repo("bob", "gamma", 2500, 300, "Python", "2026-09-15T20:45:00Z"),
            make_repo("cat", "delta", 1200, 50, "", "2026-10-18T03:10:00Z"),
            make_repo("dan", "epsilon", 800, 120, "Rust", "2026-08-20T16:00:00Z"),
        };
    }

    std::vector<Repository> run_pipeline(const AnalysisOptions& options)
    {
        auto selected = filter_repositories(dataset(), options);
        sort_repositories(selected, options.sort_key);
        select_top_n(selected, options.top_n);
        return selected;
    }

    std::vector<std::string> printed_order(const std::vector<Repository>& selected)
    {
        std::ostringstream out;
        Reporter::print_rows(selected, out);
        const std::string text = out.str();

        std::vector<std::pair<std::size_t, std::string>> found;
        for (const auto& r : dataset()) {
            const std::string id = r.owner + "/" + r.name;
            const auto pos = text.find(id);
            if (pos != std::string::npos) {
                found.emplace_back(pos, id);
            }
        }
        std::sort(found.begin(), found.end());

        std::vector<std::string> result;
        for (const auto& item : found) {
            result.push_back(item.second);
        }
        return result;
    }

    std::vector<std::string> exported_order(const std::vector<Repository>& selected)
    {
        std::vector<std::string> result;
        for (const auto& item : JsonExporter::to_json(selected)) {
            result.push_back(item["owner"].get<std::string>() + "/" +
                             item["name"].get<std::string>());
        }
        return result;
    }

    void test_printed_and_exported_match()
    {
        AnalysisOptions options;
        options.sort_key = "stars";
        options.top_n = 3;

        const auto selected = run_pipeline(options);

        const std::vector<std::string> expected{"amy/beta", "zed/alpha", "bob/gamma"};
        expect(printed_order(selected) == expected, "printed rows follow the selected order");
        expect(exported_order(selected) == expected, "exported array follows the selected order");
        expect(printed_order(selected) == exported_order(selected),
               "printed and exported selections are identical");
    }

    void test_filters_apply_to_both_outputs()
    {
        AnalysisOptions options;
        options.min_stars = 1000;
        options.language = "C++";
        options.top_n = 10;

        const auto selected = run_pipeline(options);

        expect(selected.size() == 2, "filters keep only the two C++ repos with 1000+ stars");
        expect(printed_order(selected) == exported_order(selected),
               "filtered printed and exported selections are identical");
    }

    void test_fewer_than_n()
    {
        AnalysisOptions options;
        options.top_n = 50;

        const auto selected = run_pipeline(options);

        expect(selected.size() == 5, "fewer than N matches returns all of them");
        expect(JsonExporter::to_json(selected).size() == selected.size(),
               "export count equals selected count");
    }

    void test_sort_modes_stay_consistent()
    {
        for (const char* key : {"stars", "forks", "updated"}) {
            AnalysisOptions options;
            options.sort_key = key;
            options.top_n = 4;

            const auto selected = run_pipeline(options);
            expect(selected.size() == 4, "top N is applied for every sort key");
            expect(printed_order(selected) == exported_order(selected),
                   "printed and exported orders match for every sort key");
        }
    }

    void test_zero_matches()
    {
        AnalysisOptions options;
        options.min_stars = 99999;

        const auto selected = run_pipeline(options);

        std::ostringstream out;
        Reporter::print_rows(selected, out);
        const auto j = JsonExporter::to_json(selected);

        expect(selected.empty(), "no repository meets the threshold");
        expect(out.str().empty(), "zero matches prints no rows");
        expect(j.is_array() && j.empty(), "zero matches exports []");
    }

    void test_saved_file_matches_selection()
    {
        AnalysisOptions options;
        options.top_n = 3;
        const auto selected = run_pipeline(options);

        const auto path = std::filesystem::temp_directory_path() / "analysis_integration_out.json";
        JsonExporter::save(selected, path);

        std::ifstream in(path);
        const auto from_file = nlohmann::json::parse(in);
        expect(from_file == JsonExporter::to_json(selected),
               "file on disk equals the selected collection");
        std::filesystem::remove(path);
    }
}

int main()
{
    test_printed_and_exported_match();
    test_filters_apply_to_both_outputs();
    test_fewer_than_n();
    test_sort_modes_stay_consistent();
    test_zero_matches();
    test_saved_file_matches_selection();

    if (failures == 0) {
        std::cout << "All analysis integration tests passed.\n";
    }
    return failures == 0 ? 0 : 1;
}
