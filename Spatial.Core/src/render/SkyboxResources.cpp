#include <spatial/render/SkyboxResources.h>

#include <image/KtxBundle.h>
#include <image/KtxUtility.h>
#include <sstream>

namespace fl = filament;

namespace spatial::render
{

Texture createKtxTexture(fl::Engine& engine, const uint8_t* data, uint32_t size)
{
	using namespace std;

	auto ktxBundle = new image::KtxBundle(data, size);
	return Texture{engine, image::ktx::createTexture(&engine, ktxBundle, true)};
}

bands_t parseShFile(const uint8_t* data, uint32_t size)
{
	auto bands = bands_t{};
	auto stream = std::stringstream{};
	std::copy(data, data + size, std::ostream_iterator<char>(stream));

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

Skybox createSkybox(fl::Engine& engine, fl::math::float4 color)
{
	auto resource = fl::Skybox::Builder().color(color).build(engine);

	return createResource(engine, resource);
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
									const uint8_t* data, uint32_t size, float intensity)
{
	const auto bands = parseShFile(data, size);
	return createImageBasedLight(engine, cubemap, bands, intensity);
}

} // namespace spatial::render