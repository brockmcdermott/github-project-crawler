#pragma once

#include <iostream>
#include <format>
#include <vector>
#include "analysis_options.h"
#include "repository.h"

namespace Reporter {

void print_summary(const AnalysisOptions& options, std::size_t loaded_count, std::size_t selected_count, std::ostream& out = std::cout);

void print_rows(const std::vector<Repository>& selected, std::ostream& out = std::cout);


}
