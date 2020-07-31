#include <spatial/render/SkyboxResources.h>

#include <image/KtxBundle.h>
#include <image/KtxUtility.h>
#include <sstream>

namespace fs = std::filesystem;
namespace fl = filament;

namespace spatial
{

Texture createKtxTexture(fl::Engine& engine, const std::vector<char>& resourceData)
{
	using namespace std;

	// we are using "new" here because of this legacy api
	// but this pointer is destroyed once the texture has been uploaded
	const auto ktxBundle =
		new image::KtxBundle(reinterpret_cast<const uint8_t*>(resourceData.data()), resourceData.size());
	return Texture{engine, image::ktx::createTexture(&engine, ktxBundle, false)};
}

bands_t parseShFile(const std::vector<char>& resourceData)
{
	auto bands = bands_t{};
	auto stream = std::stringstream{};
	std::copy(resourceData.begin(), resourceData.end(), std::ostream_iterator<char>(stream));

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
	auto light =
		fl::IndirectLight::Builder().reflections(&cubemap).irradiance(3, &bands[0]).intensity(intensity).build(engine);

	return createResource(engine, light);
}

IndirectLight createImageBasedLight(fl::Engine& engine, fl::Texture& cubemap, fl::Texture& irradianceCubemap,
									float intensity)
{
	auto light = fl::IndirectLight::Builder()
					 .reflections(&cubemap)
					 .irradiance(&irradianceCubemap)
					 .intensity(intensity)
					 .build(engine);

	return createResource(engine, light);
}

IndirectLight createImageBasedLight(filament::Engine& engine, filament::Texture& cubemap,
									const std::vector<char>& shResourceData, float intensity)
{
	auto bands = parseShFile(shResourceData);
	return createImageBasedLight(engine, cubemap, bands, intensity);
}

} // namespace spatial