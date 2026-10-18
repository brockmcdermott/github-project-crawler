#include "repository.h"
#include "repository_analyzer.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>
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
                         const std::string& updated_at)
    {
        Repository r;
        r.owner = owner;
        r.name = name;
        r.stars = stars;
        r.forks = forks;
        r.updated_at = updated_at;
        return r;
    }

    std::vector<std::string> names(const std::vector<Repository>& rows)
    {
        std::vector<std::string> result;
        for (const Repository& r : rows) {
            result.push_back(r.owner + "/" + r.name);
        }
        return result;
    }

    void test_sort_by_stars_with_ties()
    {
        std::vector<Repository> rows{
            make_repo("b", "x", 100, 0, ""),
            make_repo("a", "z", 100, 0, ""),
            make_repo("a", "y", 100, 0, ""),
            make_repo("c", "w", 500, 0, ""),
        };

        sort_repositories(rows, "stars");

        const std::vector<std::string> expected{"c/w", "a/y", "a/z", "b/x"};
        expect(names(rows) == expected, "stars sort is descending with owner/name tie-breaks");
    }

    void test_sort_by_forks_with_ties()
    {
        std::vector<Repository> rows{
            make_repo("b", "x", 0, 20, ""),
            make_repo("a", "y", 0, 20, ""),
            make_repo("c", "z", 0, 90, ""),
        };

        sort_repositories(rows, "forks");

        const std::vector<std::string> expected{"c/z", "a/y", "b/x"};
        expect(names(rows) == expected, "forks sort is descending with owner tie-break");
    }

    void test_sort_by_updated_with_ties()
    {
        std::vector<Repository> rows{
            make_repo("a", "old", 0, 0, "2026-01-05T10:00:00Z"),
            make_repo("b", "same", 0, 0, "2026-09-30T09:12:00Z"),
            make_repo("a", "same", 0, 0, "2026-09-30T09:12:00Z"),
            make_repo("c", "mid", 0, 0, "2026-09-28T14:20:00Z"),
        };

        sort_repositories(rows, "updated");

        const std::vector<std::string> expected{"a/same", "b/same", "c/mid", "a/old"};
        expect(names(rows) == expected, "updated sort is newest-first with owner tie-break");
    }

    void test_sort_rejects_unknown_key()
    {
        std::vector<Repository> rows{make_repo("a", "x", 1, 1, "")};
        try {
            sort_repositories(rows, "star");
            expect(false, "unknown sort key throws");
        } catch (const std::invalid_argument&) {
        }
    }
    void test_top_n_truncates()
    {
        std::vector<Repository> rows{
            make_repo("a", "1", 50, 0, ""),
            make_repo("a", "2", 40, 0, ""),
            make_repo("a", "3", 30, 0, ""),
            make_repo("a", "4", 20, 0, ""),
        };

        select_top_n(rows, 2);

        const std::vector<std::string> expected{"a/1", "a/2"};
        expect(names(rows) == expected, "top-N keeps exactly the first N");
    }

    void test_top_n_fewer_than_n()
    {
        std::vector<Repository> rows{
            make_repo("a", "1", 50, 0, ""),
            make_repo("a", "2", 40, 0, ""),
        };

        select_top_n(rows, 10);

        expect(rows.size() == 2, "top-N keeps all rows when fewer than N remain");
    }

    void test_empty_input()
    {
        std::vector<Repository> rows;

        sort_repositories(rows, "stars");
        select_top_n(rows, 5);

        expect(rows.empty(), "empty input stays empty");
    }

    void test_metadata_unchanged()
    {
        Repository original = make_repo("owner", "repo", 7, 3, "2026-09-30T09:12:00Z");
        original.description = "desc";
        original.language = "C++";
        original.size = 1234;
        original.url = "https://github.com/owner/repo";

        std::vector<Repository> rows{original, make_repo("z", "z", 1, 1, "")};
        sort_repositories(rows, "stars");

        const Repository& r = rows.front();
        expect(r.owner == "owner" && r.name == "repo" && r.description == "desc" &&
               r.stars == 7 && r.forks == 3 && r.language == "C++" && r.size == 1234 &&
               r.updated_at == "2026-09-30T09:12:00Z" &&
               r.url == "https://github.com/owner/repo",
               "sorting does not change repository metadata");
    }
} // namespace

int main()
{
    test_sort_by_stars_with_ties();
    test_sort_by_forks_with_ties();
    test_sort_by_updated_with_ties();
    test_sort_rejects_unknown_key();
    test_top_n_truncates();
    test_top_n_fewer_than_n();
    test_empty_input();
    test_metadata_unchanged();

    if (failures == 0) {
        std::cout << "All repository analyzer tests passed.\n";
    }

    return failures == 0 ? 0 : 1;
}