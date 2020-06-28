#include <spatial/core/Asset.h>
#include <spatial/common/ResourceUtils.h>
#include <fstream>

namespace spatial
{

std::istream DefaultAssetLoader::operator()(const std::filesystem::path& resourcePath) const
{
	return createStreamFromPath(basePath / resourcePath);
}

} // namespace spatial