#include <spatial/common/Exceptions.h>
#include <spatial/core/ConfigurationLoader.h>
#include <spatial/core/Asset.h>
#include <fstream> //Required for toml::parse_file() work

namespace fs = std::filesystem;

namespace spatial
{

Configuration readConfigFile(const fs::path& filePath)
{
	const auto path = Asset::absolute(filePath);
	return Configuration{toml::parse_file(path.generic_u8string())};
}

} // namespace spatial
