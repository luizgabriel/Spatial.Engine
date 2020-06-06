#include <spatial/render/SkyboxResources.h>
#include <spatial/core/Asset.h>

#include <image/KtxBundle.h>
#include <image/KtxUtility.h>

#include <fstream>
#include <vector>

namespace fs = std::filesystem;
namespace fl = filament;

namespace spatial
{

Texture createKtxTexture(fl::Engine& engine, const fs::path& filePath)
{
	using namespace std;

	const auto absolutePath = Asset::absolute(filePath);
	auto stream = ifstream{absolutePath, std::ios_base::in | ios::binary};

	const auto contents = vector<uint8_t>{istreambuf_iterator<char>(stream), {}};

	// we are using "new" here because of this legacy api
	// but this pointer is destroyed once the texture has been uploaded
	const auto ktxBundle = new image::KtxBundle(contents.data(), contents.size());
	return Texture{engine, image::ktx::createTexture(&engine, ktxBundle, false)};
}

bands_t parseShFile(const fs::path& file)
{
	auto bands = bands_t{};
	const auto absolutePath = Asset::absolute(file);
	auto stream = std::ifstream{absolutePath, std::ios_base::in};

	if (!stream)
		throw FileNotFoundError(absolutePath);

	stream >> std::skipws;

	char c;
	for (auto& band : bands)
	{
		while (!stream.eof() && stream >> c && c != '(')
			;

		stream >> band.r;
		stream >> c;
		stream >> band.g;
		stream >> c;
		stream >> band.b;
	}

	return bands;
}

Skybox createSkybox(fl::Engine& engine, fl::Texture& skybox, bool showSun)
{
	auto resource = fl::Skybox::Builder().environment(&skybox).showSun(showSun).build(engine);

	return createResource(engine, resource);
}

IndirectLight createImageBasedLight(fl::Engine& engine, fl::Texture& cubemap, const bands_t& bands, float intensity)
{
	auto light = fl::IndirectLight::Builder().reflections(&cubemap).irradiance(3, &bands[0]).intensity(intensity).build(engine);

	return createResource(engine, light);
}

IndirectLight createImageBasedLight(fl::Engine& engine, fl::Texture& cubemap, fl::Texture& irradianceCubemap, float intensity)
{
	auto light =
		fl::IndirectLight::Builder().reflections(&cubemap).irradiance(&irradianceCubemap).intensity(intensity).build(engine);

	return createResource(engine, light);
}

KtxFolderPaths parseKtxFolder(const std::filesystem::path& folder)
{
	const auto name = folder.filename().generic_string();
	auto iblPath = folder / (name + "_ibl.ktx");
	auto skyPath = folder / (name + "_skybox.ktx");
	auto shPath = folder / "sh.txt";

	return std::make_tuple(std::move(iblPath), std::move(skyPath), std::move(shPath));
}

IndirectLight createImageBasedLight(filament::Engine& engine,
									filament::Texture& cubemap,
									const std::filesystem::path& shFile,
									float intensity)
{
	auto bands = parseShFile(shFile);
	return createImageBasedLight(engine, cubemap, bands, intensity);
}

} // namespace spatial