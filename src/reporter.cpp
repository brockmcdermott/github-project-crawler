#include "reporter.h"

#include <iomanip>
#include <string>

namespace Reporter {

void print_summary(
    const AnalysisOptions& options,
    std::size_t loaded_count,
    std::size_t selected_count,
    std::ostream& out)
{
    out << "GitHub Project Crawler - Analysis\n"
        << "Dataset: " << options.input_path.string() << '\n'
        << "Repositories loaded: " << loaded_count << '\n'
        << "Filter: stars >= " << options.min_stars << '\n'
        << "Language: "
        << (options.language.empty() ? "any" : options.language) << '\n'
        << "Sort: " << options.sort_key << " (descending)\n"
        << "Top N: " << options.top_n << '\n'
        << "Output: " << options.output_path.string() << '\n'
        << "Repositories Selected: " << selected_count << '\n';
}

void print_rows(const std::vector<Repository>& selected, std::ostream& out)
{
    std::size_t index = 1;
    for (const Repository& repository : selected) {
        const std::string full_name = repository.owner + "/" + repository.name;
        const std::string language = repository.language.empty()
            ? "(none)"
            : repository.language;

        out << std::right << std::setw(3) << index++ << ". "
            << std::left << std::setw(45) << full_name
            << "  | stars: " << std::right << std::setw(8) << repository.stars
            << " | forks: " << std::setw(8) << repository.forks
            << " | " << std::left << std::setw(10) << language
            << " | updated: " << repository.updated_at << '\n';
    }
}

}
