#include "Resources.h"

namespace spatial::assets
{

assets::ResourcesLoader sResourceLoader{[](const std::string_view) { return std::nullopt; }};

assets::Resource loadResource(const std::string_view resourcePath)
{
	return sResourceLoader(resourcePath);
}

} // namespace spatial::assets