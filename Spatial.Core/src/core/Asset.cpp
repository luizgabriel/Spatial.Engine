#include <spatial/core/Asset.h>
#include <fstream>

namespace spatial::core
{

std::filesystem::path Asset::s_basePath{};

std::vector<char> Asset::read(const std::filesystem::path &filePath)
{
    auto absolutePath = absolute(filePath);
    auto stream = std::ifstream{absolutePath, std::ios_base::in | std::ios_base::binary};

    if (!stream)
        throw std::exception("Could not read file.");

    auto iterator = std::istreambuf_iterator<char>{stream};
    return {iterator, {}};
}

} // namespace spatial::core