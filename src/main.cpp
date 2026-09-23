#include <iostream>
#include "search_options.h"
#include "repository.h"
#include <nlohmann/json.hpp>



int main()
{
    //testcmake edit                 **raw string literal below, easier than the alternative**
    nlohmann::json test_json = nlohmann::json::parse(R"({"name": "hello"})");
    std::cout << "parsed name: " << test_json["name"] << "\n";

    //simple test for search_options.h
    SearchOptions options;
    std::cout << "defualt query: " << options.query << ", count: " << options.count << "\n";

    //simple test for search_options.h
    Repository test;
    test.owner = "linus";
    test.name = "struct test";
    test.stars = 1234567;
    std::cout << test.owner << "/" << test.name << " has " << test.stars << " stars\n";

    return 0;
}
