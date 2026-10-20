#include "json_exporter.h"

#include <fstream>
#include <stdexcept>

namespace JsonExporter {

nlohmann::json to_json(const std::vector<Repository>& selected)
{
    auto array = nlohmann::json::array();
    for (const Repository& repository : selected) {
        array.push_back({
            {"owner", repository.owner},
            {"name", repository.name},
            {"description", repository.description},
            {"stars", repository.stars},
            {"forks", repository.forks},
            {"language", repository.language},
            {"size", repository.size},
            {"updated_at", repository.updated_at},
            {"url", repository.url},
        });
    }
    return array;
}

void write(const std::vector<Repository>& selected, std::ostream& out)
{
    out << to_json(selected).dump(2) << '\n';
}

void save(const std::vector<Repository>& selected, const std::filesystem::path& path)
{
    std::ofstream file(path);
    if (!file) {
        throw std::runtime_error("Cannot open output file: " + path.string());
    }

    write(selected, file);
    file.flush();

    if (!file) {
        throw std::runtime_error("Failed while writing output file: " + path.string());
    }
}

}
