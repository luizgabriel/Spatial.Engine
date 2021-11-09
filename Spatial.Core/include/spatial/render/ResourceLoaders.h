#pragma once

#include <spatial/common/Math.h>
#include <spatial/render/Resources.h>

#include <cstdint>
#include <spatial/resources/FilameshFile.h>
#include <string>

namespace spatial::render
{

Material createMaterial(filament::Engine& engine, const uint8_t* data, size_t size);

Texture createTexture(filament::Engine& engine, math::int2 dimensions, filament::Texture::InternalFormat format,
					  filament::Texture::Usage usage = filament::Texture::Usage::DEFAULT,
					  filament::Texture::Sampler sampler = filament::Texture::Sampler::SAMPLER_2D);

Texture createTexture(filament::Engine& engine, const uint8_t* data, size_t size,
					  filament::Texture::Usage usage = filament::Texture::Usage::DEFAULT,
					  filament::Texture::Sampler sampler = filament::Texture::Sampler::SAMPLER_2D);

VertexBuffer createVertexBuffer(filament::Engine& engine, const FilameshFile& filamesh);

IndexBuffer createIndexBuffer(filament::Engine& engine, const FilameshFile& filamesh);

} // namespace spatial::render