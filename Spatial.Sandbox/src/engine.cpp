#include "engine.h"

namespace fs = std::filesystem;

spatial::core::Application& app()
{
    static spatial::core::Application application;
    return application;
}

std::string read(const fs::path& filePath)
{
    auto path = fs::current_path() / filePath;
    if (!fs::exists(path))
        throw std::exception("File not found.");

    std::ifstream inFile(path);
    std::stringstream strStream;

    strStream << inFile.rdbuf();

    return strStream.str();
}
