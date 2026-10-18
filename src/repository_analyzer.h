#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include "repository.h"

bool compare_by_stars(const Repository& a, const Repository& b);
bool compare_by_forks(const Repository& a, const Repository& b);
bool compare_by_updated(const Repository& a, const Repository& b);

void sort_repositories(std::vector<Repository>& rows, const std::string& sort_key);
void select_top_n(std::vector<Repository>& rows, std::size_t top_n);