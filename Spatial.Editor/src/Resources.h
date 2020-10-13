#pragma once

#include <spatial/assets/AggregatorLoader.h>
#include <spatial/assets/ResourcesLoader.h>

namespace spatial::editor
{

extern assets::AggregatorLoader gResourceLoader;

void initAssets(const std::filesystem::path& executablePath);

assets::ResourceData load(const std::string_view resourceName);

}