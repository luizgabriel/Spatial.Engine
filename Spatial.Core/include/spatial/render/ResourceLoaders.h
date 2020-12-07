#pragma once

#include <spatial/render/Resources.h>

#include <cstdint>
#include <filesystem>
#include <unordered_map>

#include <filament/Box.h>
#include <math/vec2.h>
#include <math/vec3.h>
#include <math/vec4.h>
#include <spatial/render/Mesh.h>
#include <string_view>
#include <utility>

namespace spatial
{

Material createMaterial(filament::Engine& engine, const std::vector<char>& resourceData);

Texture createTexture(filament::Engine& engine, std::pair<std::uint32_t, std::uint32_t> dimensions,
					  fl::Texture::InternalFormat format, fl::Texture::Usage usage = fl::Texture::Usage::DEFAULT,
					  fl::Texture::Sampler sampler = fl::Texture::Sampler::SAMPLER_2D);

Texture createTexture(filament::Engine& engine, const std::vector<char>& resourceData,
					  fl::Texture::Usage usage = fl::Texture::Usage::DEFAULT,
					  fl::Texture::Sampler sampler = fl::Texture::Sampler::SAMPLER_2D);

Mesh createMesh(filament::Engine& engine, const std::vector<char>& resourceData);

} // namespace spatial