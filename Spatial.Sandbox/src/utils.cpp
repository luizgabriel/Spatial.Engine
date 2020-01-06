#include "utils.h"

namespace fs = std::filesystem;
using namespace spatial::core;

fs::path g_basePath{};

Application& app()
{
    static Application application;
    return application;
}

std::vector<char> read(const fs::path& filePath)
{
    std::ifstream stream{g_basePath / filePath, std::ios_base::in | std::ios_base::binary};
    
    if (!stream)
        throw std::exception("Could not read file.");

    std::istreambuf_iterator<char> iterator{stream};
    return {iterator, {}};
}
