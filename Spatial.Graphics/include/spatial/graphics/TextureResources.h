#pragma once

#include <spatial/graphics/Resources.h>
#include <vector>

namespace spatial::graphics
{

Texture createTexture(filament::Engine& engine, math::uvec2 dimensions, filament::Texture::InternalFormat format,
					  filament::Texture::Usage usage = filament::Texture::Usage::DEFAULT,
					  filament::Texture::Sampler sampler = filament::Texture::Sampler::SAMPLER_2D);

Texture createTexture(filament::Engine& engine, const uint8_t* data, size_t size,
					  filament::Texture::Usage usage = filament::Texture::Usage::DEFAULT,
					  filament::Texture::Sampler sampler = filament::Texture::Sampler::SAMPLER_2D);

Texture createTexture(filament::Engine& engine, const std::vector<uint32_t>& pixels, size_t width);

Texture createDummyCubemap(filament::Engine& engine);


}