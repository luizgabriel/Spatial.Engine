#pragma once

#include <spatial/common/Math.h>
#include <spatial/render/Resources.h>

#include <cstdint>
#include <string>
#include <spatial/render/FilameshFile.h>

namespace spatial::render
{

Material createMaterial(filament::Engine& engine, const std::string& resourceData);

Texture createTexture(filament::Engine& engine, math::int2 dimensions,
					  fl::Texture::InternalFormat format, fl::Texture::Usage usage = fl::Texture::Usage::DEFAULT,
					  fl::Texture::Sampler sampler = fl::Texture::Sampler::SAMPLER_2D);

Texture createTexture(filament::Engine& engine, const std::string& resourceData,
					  fl::Texture::Usage usage = fl::Texture::Usage::DEFAULT,
					  fl::Texture::Sampler sampler = fl::Texture::Sampler::SAMPLER_2D);

FilameshFile createFilamesh(filament::Engine& engine, const std::string& resourceData);

} // namespace spatial::render