#pragma once
//make sure the compiler only reads this file once.
#include <string>

//bundle data a github repo will have all togther into a variable
struct Repository {
    std::string owner;
    std::string name;
    std::string description;
    //i could use an unsigned (uint64_t) type here but im gonna go with regular int,
    //also gonna add {} to keep garbage memory out of the equation
    int stars{};
    int forks{};
    int size{};
    std::string update_at;
    std::string url;
};