#pragma once

#include <spatial/spatial.h>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

extern std::filesystem::path g_basePath;

std::vector<char> read(const std::filesystem::path &filePath);