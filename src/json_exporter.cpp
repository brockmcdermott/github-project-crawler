#include "json_exporter.h"

namespace JsonExporter {

nlohmann::json to_json(const std::vector<Repository>& selected){
	auto array = nlohmann::json::array();
	for (const auto& repo : selected) {
		array.push_back({
		    	{"owner", repo.owner},
		    	{"name", repo.name},
		    	{"description", repo.description},
		    	{"stars", repo.stars},
		    	{"forks", repo.forks},
		   	{"language", repo.language},
		   	{"size", repo.size},
		   	{"updated_at", repo.updated_at},
		    	{"url", repo.url},
		});
	}
	return array;
}

void write(const std::vector<Repository>& selected, std::ostream& out){
	out << to_json(selected).dump(2) << '\n';
}

void save(const std::vector<Repository>& selected, const std::filesystem::path& path){
	std::ofstream file(path);
	if(!file) throw std::runtime_error("Cannot open output file: " + path.string());
	
	write(selected,file);
	file.flush();
	
	if(!file) throw std::runtime_error("Failed whilst writing to the output file: " + path.string());
}
}
