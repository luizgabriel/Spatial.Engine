#include "utils.h"

namespace fs = std::filesystem;

spatial::core::Application& app()
{
    static spatial::core::Application application;
    return application;
}

fs::path& basePath()
{
    static fs::path base;
    return base;
}

std::vector<char> read(const fs::path& filePath)
{
    std::ifstream stream{basePath() / filePath, std::ios_base::in | std::ios_base::binary};
    
    if (!stream)
        throw std::exception("Could not read file.");

    std::istreambuf_iterator<char> iterator{stream};
    return {iterator, {}};
}
