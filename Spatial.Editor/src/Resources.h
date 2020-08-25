#pragma once

#include <spatial/assets/ResourcesLoader.h>

namespace spatial::assets
{

extern assets::ResourcesLoader sResourceLoader;

assets::Resource loadResource(const std::string_view resourcePath);

}