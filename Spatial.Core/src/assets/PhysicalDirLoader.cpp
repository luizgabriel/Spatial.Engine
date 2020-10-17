#include <spatial/assets/PhysicalDirLoader.h>

namespace spatial::assets
{

PhysicalDirLoader::PhysicalDirLoader(std::filesystem::path basePath) : mRootPath{std::move(basePath)}
{
}

assets::ResourceData PhysicalDirLoader::load(const std::string_view fileName) const
{
	auto absolute = std::filesystem::absolute(mRootPath / fileName);
	auto fs = std::fstream{absolute};
	if (!fs)
		return std::nullopt;

	return std::vector<char>{std::istreambuf_iterator<char>{fs}, std::istreambuf_iterator<char>{}};
}

} // namespace spatial::assets