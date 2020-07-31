#pragma once

#include <spatial/render/Resources.h>

#include <math/vec3.h>
#include <span>
#include <string_view>

namespace spatial
{

Texture createKtxTexture(filament::Engine& engine, const std::vector<char>& resourceData);

using bands_t = std::array<filament::math::float3, 9>;

bands_t parseShFile(const std::vector<char>& resourceData);

Skybox createSkybox(filament::Engine& engine, filament::Texture& skybox, bool showSun = true);

IndirectLight createImageBasedLight(filament::Engine& engine, filament::Texture& cubemap,
									const std::vector<char>& shResourceData, float intensity = 30000.0f);

IndirectLight createImageBasedLight(filament::Engine& engine, filament::Texture& cubemap, const bands_t& bands,
									float intensity = 30000.0f);

IndirectLight createImageBasedLight(filament::Engine& engine, filament::Texture& cubemap,
									filament::Texture& irradianceCubemap, float intensity = 30000.0f);

} // namespace spatial
