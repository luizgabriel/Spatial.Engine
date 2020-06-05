#pragma once

#include <spatial/render/Resources.h>

#include <math/vec3.h>
#include <filesystem>

namespace spatial
{

Texture createKtxTexture(filament::Engine& engine, const std::filesystem::path& filePath);

using bands_t = std::array<filament::math::float3, 9>;

bands_t parseShFile(const std::filesystem::path& file);

Skybox createSkybox(filament::Engine& engine, filament::Texture& skybox, bool showSun = true);

IndirectLight createImageBasedLight(filament::Engine& engine,
									filament::Texture& cubemap,
									const std::filesystem::path& shFile,
									float intensity = 30000.0f);

IndirectLight createImageBasedLight(filament::Engine& engine,
									filament::Texture& cubemap,
									const bands_t& bands,
									float intensity = 30000.0f);

IndirectLight createImageBasedLight(filament::Engine& engine,
									filament::Texture& cubemap,
									filament::Texture& irradianceCubemap,
									float intensity = 30000.0f);

using KtxFolderPaths = std::tuple<std::filesystem::path, std::filesystem::path, std::filesystem::path>;
KtxFolderPaths parseKtxFolder(const std::filesystem::path& folder);

} // namespace spatial
