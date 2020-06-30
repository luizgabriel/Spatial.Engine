#include <fstream>
#include <spatial/common/ResourceUtils.h>
#include <spatial/core/Asset.h>

namespace spatial
{

Asset::ResourceDelegate Asset::sAssetResolverDelegate{};

Asset::Resource AssetLoader::operator()(Asset::Id resourceId) const
{
	auto stream = createStreamFromPath(basePath / resourcePath);
}

} // namespace spatial