#include "json_exporter.h"
#include "repository.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
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
                         std::uint64_t stars, const std::string& language)
    {
        Repository r;
        r.owner = owner;
        r.name = name;
        r.stars = stars;
        r.language = language;
        return r;
    }

    std::vector<Repository> sample()
    {
        Repository full = make_repo("amy", "beta", 5000, "C++");
        full.description = "Ties";
        full.forks = 300;
        full.size = 450;
        full.updated_at = "2026-10-10T08:30:00Z";
        full.url = "https://github.com/amy/beta";
        return {full, make_repo("cat", "delta", 1200, "")};
    }

    void test_array_keeps_order_and_all_fields()
    {
        const auto repos = sample();
        const auto j = JsonExporter::to_json(repos);

        expect(j.is_array() && j.size() == repos.size(), "export is an array of the same size");
        for (std::size_t i = 0; i < repos.size(); ++i) {
            for (const char* key : {"owner", "name", "description", "stars", "forks",
                                    "language", "size", "updated_at", "url"}) {
                expect(j[i].contains(key), "every object has all 9 fields");
            }
            expect(j[i]["owner"] == repos[i].owner && j[i]["name"] == repos[i].name,
                   "export keeps the input order");
        }
    }

    void test_empty_values_are_preserved()
    {
        const auto j = JsonExporter::to_json(sample());
        expect(j[1]["language"] == "" && j[1]["description"] == "",
               "empty language and description stay empty strings");
    }

    void test_empty_selection()
    {
        const auto j = JsonExporter::to_json({});
        expect(j.is_array() && j.empty(), "empty selection exports []");
    }

    void test_write_to_stream()
    {
        std::ostringstream out;
        JsonExporter::write(sample(), out);
        expect(nlohmann::json::parse(out.str()) == JsonExporter::to_json(sample()),
               "write produces the same JSON as to_json");
    }

    void test_save_roundtrip()
    {
        const auto path = std::filesystem::temp_directory_path() / "json_exporter_test_out.json";
        JsonExporter::save(sample(), path);

        std::ifstream in(path);
        expect(static_cast<bool>(in), "save creates the file");
        expect(nlohmann::json::parse(in) == JsonExporter::to_json(sample()),
               "saved file matches to_json");
        std::filesystem::remove(path);
    }

    void test_save_bad_path_throws()
    {
        try {
            JsonExporter::save(sample(), "/nonexistent_dir_for_test/out.json");
            expect(false, "save to a bad path throws");
        } catch (const std::runtime_error&) {
        }
    }
}

int main()
{
    test_array_keeps_order_and_all_fields();
    test_empty_values_are_preserved();
    test_empty_selection();
    test_write_to_stream();
    test_save_roundtrip();
    test_save_bad_path_throws();

    if (failures == 0) {
        std::cout << "All json exporter tests passed.\n";
    }
    return failures == 0 ? 0 : 1;
}
