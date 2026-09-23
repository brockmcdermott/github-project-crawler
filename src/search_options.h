#pragma once

#include <string>

struct SearchOptions {
    std::string query{"stars:>1000"};
    int count{100};
    std::string sort{"stars"};
    std::string order{"desc"};
};