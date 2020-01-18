#pragma once

#include <filesystem>
#include <vector>

namespace spatial::core
{

class Asset
{
private:
    static std::filesystem::path s_basePath;

public:
    static void init(const std::filesystem::path &basePath)
    {
        s_basePath = basePath;
    }

    static std::filesystem::path absolute(const std::filesystem::path &relative)
    {
        return s_basePath / relative;
    }

};

} // namespace spatial::core