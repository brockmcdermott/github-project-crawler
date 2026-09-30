#pragma once
#include <string>
#include <vector>
#include "repository.h"

class RepositoryParser {
public:
    static std:: vector <Repository> parse(const std::string& json_body);
};