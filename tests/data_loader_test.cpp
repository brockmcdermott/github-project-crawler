#include "data_loader.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

int failures = 0;
const std::filesystem::path fixture_directory{TEST_FIXTURE_DIR};

void expect(bool condition, std::string_view message)
{
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

void expect_load_error(
    const std::filesystem::path& path,
    std::string_view expected_message,
    std::string_view test_name)
{
    try {
        static_cast<void>(DataLoader::load_repositories(path));
        expect(false, test_name);
    } catch (const std::runtime_error& error) {
        const std::string message = error.what();
        expect(message.find(expected_message) != std::string::npos, test_name);
        expect(message.find(path.string()) != std::string::npos,
               "loader error identifies the dataset path");
    }
}

void test_valid_dataset()
{
    const auto repositories = DataLoader::load_repositories(
        fixture_directory / "repositories.json");

    expect(repositories.size() == 3, "valid dataset loads every repository");

    const Repository& repository = repositories.front();
    expect(repository.owner == "alpha", "owner is loaded");
    expect(repository.name == "cpp-tools", "name is loaded");
    expect(repository.description == "C++ developer tools", "description is loaded");
    expect(repository.stars == 5000, "stars are loaded");
    expect(repository.forks == 400, "forks are loaded");
    expect(repository.language == "C++", "language is loaded");
    expect(repository.size == 1200, "size is loaded");
    expect(repository.updated_at == "2026-10-10T12:00:00Z", "update time is loaded");
    expect(repository.url == "https://github.com/alpha/cpp-tools", "URL is loaded");
}

void test_invalid_datasets()
{
    expect_load_error(
        fixture_directory / "does-not-exist.json",
        "Cannot open repository dataset",
        "missing file is rejected");
    expect_load_error(
        fixture_directory / "malformed.json",
        "Invalid repository dataset",
        "malformed JSON is rejected");
    expect_load_error(
        fixture_directory / "non_array.json",
        "must be a JSON array",
        "non-array root is rejected");
    expect_load_error(
        fixture_directory / "missing_field.json",
        "Invalid repository dataset",
        "missing repository field is rejected");
    expect_load_error(
        fixture_directory / "wrong_type.json",
        "Invalid repository dataset",
        "incorrect repository field type is rejected");
}

} // namespace

int main()
{
    test_valid_dataset();
    test_invalid_datasets();

    if (failures == 0) {
        std::cout << "All data loader tests passed.\n";
    }

    return failures == 0 ? 0 : 1;
}
