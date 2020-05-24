#pragma once

#include <spatial/common/Configuration.h>
#include <filesystem>

namespace spatial
{

Configuration readConfigFile(const std::filesystem::path& filePath);

}
