#include "reporter.h"

namespace Reporter {

void print_summary(const AnalysisOptions& options, std::size_t loaded_count, std::size_t selected_count, std::ostream& out){
            out	      << "GitHub Project Crawler - Analysis\n"
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
void print_rows(const std::vector<Repository>& selected, std::ostream& out){
	std::size_t i = 1;
	for(const auto& repo : selected){
		const std::string full_name = repo.owner + "/" + repo.name;
		out << std::format("{:>3d}. {:<45s}  | stars: {:>8d} | forks: {:>8d} | {:<10s} | updated: {}\n", 
		i++, 
		full_name, 
		repo.stars, 
		repo.forks, 
		repo.language.empty() ? "(none)" : repo.language, 
		repo.updated_at
		);
	}

}


}
