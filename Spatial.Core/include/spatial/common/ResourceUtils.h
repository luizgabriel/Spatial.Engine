#pragma once

#include <filesystem>
#include <fstream>

namespace spatial
{

std::ifstream createStreamFromPath(const std::filesystem::path& filePath);

std::vector<char> createBufferFromStream(std::ifstream&& stream);

}
