#include <spatial/render/SkyboxResources.h>

#include <image/KtxBundle.h>
#include <image/KtxUtility.h>
#include <sstream>

namespace spatial::render
{

Texture createKtxTexture(filament::Engine& engine, const uint8_t* data, uint32_t size)
{
	using namespace std;

	auto ktxBundle = new image::KtxBundle(data, size);
	return Texture{engine, image::ktx::createTexture(&engine, ktxBundle, true)};
}

std::istream& operator>>(std::istream& istream, bands_t& bands)
{
	istream >> std::skipws;

	char c;
	for (auto& band : bands)
	{
		while (!istream.eof() && istream >> c && c != '(')
			;

		istream >> band.r;
		istream >> c;
		istream >> band.g;
		istream >> c;
		istream >> band.b;
	}

	return istream;
}

bands_t parseShFile(const uint8_t* data, uint32_t size)
{
	auto bands = bands_t{};
	auto stream = std::stringstream{};
	std::copy(data, data + size, std::ostream_iterator<char>(stream));
	stream >> bands;
	return bands;
}

Skybox createSkybox(filament::Engine& engine, filament::math::float4 color)
{
	auto resource = filament::Skybox::Builder().color(color).build(engine);

	return createResource(engine, resource);
}

Skybox createSkybox(filament::Engine& engine, filament::Texture& skybox, bool showSun)
{
	auto resource = filament::Skybox::Builder().environment(&skybox).showSun(showSun).build(engine);

	return createResource(engine, resource);
}

IndirectLight createImageBasedLight(filament::Engine& engine, const filament::Texture& cubeMap, const bands_t& bands,
									float intensity)
{
	auto light = filament::IndirectLight::Builder()
					 .reflections(&cubeMap)
					 .irradiance(3, &bands[0])
					 .intensity(intensity)
					 .build(engine);

	return createResource(engine, light);
}

IndirectLight createImageBasedLight(filament::Engine& engine, const filament::Texture& cubeMap,
									const filament::Texture& irradianceCubeMap, float intensity)
{
	auto light = filament::IndirectLight::Builder()
					 .reflections(&cubeMap)
					 .irradiance(&irradianceCubeMap)
					 .intensity(intensity)
					 .build(engine);

	return createResource(engine, light);
}

IndirectLight createImageBasedLight(filament::Engine& engine, const filament::Texture& cubemap, const uint8_t* data,
									uint32_t size, float intensity)
{
	const auto bands = parseShFile(data, size);
	return createImageBasedLight(engine, cubemap, bands, intensity);
}

} // namespace spatial::render