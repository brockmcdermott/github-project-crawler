#include "reporter.h"
#include "repository.h"

#include <algorithm>
#include <cstdint>
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

    bool has(const std::string& text, const std::string& part)
    {
        return text.find(part) != std::string::npos;
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
        return r;
    }

    std::string rows_text(const std::vector<Repository>& selected)
    {
        std::ostringstream out;
        Reporter::print_rows(selected, out);
        return out.str();
    }

    void test_row_has_required_fields()
    {
        const auto text = rows_text({make_repo("zed", "alpha", 5000, 812, "C++", "2026-01-02T00:00:00Z")});

        expect(has(text, "zed/alpha"), "row shows owner/name");
        expect(has(text, "5000"), "row shows stars");
        expect(has(text, "812"), "row shows forks");
        expect(has(text, "C++"), "row shows language");
        expect(has(text, "2026-01-02T00:00:00Z"), "row shows update time");
    }

    void test_rows_keep_input_order()
    {
        const auto text = rows_text({
            make_repo("zed", "alpha", 5, 1, "C", "t"),
            make_repo("amy", "beta", 4, 1, "C", "t"),
            make_repo("bob", "gamma", 3, 1, "C", "t"),
        });

        const auto first = text.find("zed/alpha");
        const auto second = text.find("amy/beta");
        const auto third = text.find("bob/gamma");

        expect(first != std::string::npos && second != std::string::npos && third != std::string::npos,
               "every selected repository is printed");
        expect(first < second && second < third, "rows are printed in the given order, not re-sorted");
    }

    void test_one_line_per_repository()
    {
        const auto text = rows_text({
            make_repo("a", "1", 1, 1, "C", "t"),
            make_repo("a", "2", 1, 1, "C", "t"),
            make_repo("a", "3", 1, 1, "C", "t"),
        });

        expect(std::count(text.begin(), text.end(), '\n') == 3, "each repository gets exactly one line");
    }

    void test_empty_language_prints_none()
    {
        const auto text = rows_text({make_repo("amy", "beta", 1, 1, "", "t")});
        expect(has(text, "(none)"), "empty language is displayed as (none)");
    }

    void test_empty_selection_prints_no_rows()
    {
        expect(rows_text({}).empty(), "empty selection prints no rows");
    }

    void test_summary_shows_all_run_details()
    {
        AnalysisOptions options;
        options.input_path = "data.json";
        options.output_path = "out.json";
        options.min_stars = 1000;
        options.language = "C++";
        options.sort_key = "forks";
        options.top_n = 7;

        std::ostringstream out;
        Reporter::print_summary(options, 100, 3, out);
        const auto text = out.str();

        expect(has(text, "data.json"), "summary shows dataset path");
        expect(has(text, "Repositories loaded: 100"), "summary shows loaded count");
        expect(has(text, "stars >= 1000"), "summary shows minimum stars");
        expect(has(text, "Language: C++"), "summary shows language filter");
        expect(has(text, "Sort: forks"), "summary shows sort mode");
        expect(has(text, "Top N: 7"), "summary shows top N");
        expect(has(text, "Repositories Selected: 3"), "summary shows selected count");
        expect(has(text, "out.json"), "summary shows export path");
    }

    void test_summary_empty_language_means_any()
    {
        AnalysisOptions options;
        std::ostringstream out;
        Reporter::print_summary(options, 1, 1, out);
        expect(has(out.str(), "Language: any"), "empty language filter is shown as any");
    }

    void test_reporter_does_not_modify_selection()
    {
        const std::vector<Repository> selected{make_repo("amy", "beta", 9, 2, "Go", "t")};
        const auto copy = selected;
        rows_text(selected);

        expect(selected.size() == copy.size() && selected[0].owner == copy[0].owner &&
                   selected[0].stars == copy[0].stars,
               "reporter leaves the selection unchanged");
    }
} 

int main()
{
    test_row_has_required_fields();
    test_rows_keep_input_order();
    test_one_line_per_repository();
    test_empty_language_prints_none();
    test_empty_selection_prints_no_rows();
    test_summary_shows_all_run_details();
    test_summary_empty_language_means_any();
    test_reporter_does_not_modify_selection();

    if (failures == 0) {
        std::cout << "All reporter tests passed.\n";
    }
    return failures == 0 ? 0 : 1;
}
