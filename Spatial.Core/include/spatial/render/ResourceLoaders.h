#pragma once

#include <spatial/common/Math.h>
#include <spatial/render/Resources.h>

#include <cstdint>
#include <spatial/resources/FilameshFile.h>
#include <string>

namespace spatial::render
{

Material createMaterial(filament::Engine& engine, const uint8_t* data, uint32_t size);

Texture createTexture(filament::Engine& engine, math::int2 dimensions,
					  fl::Texture::InternalFormat format, fl::Texture::Usage usage = fl::Texture::Usage::DEFAULT,
					  fl::Texture::Sampler sampler = fl::Texture::Sampler::SAMPLER_2D);

Texture createTexture(filament::Engine& engine, const uint8_t* data, uint32_t size,
					  fl::Texture::Usage usage = fl::Texture::Usage::DEFAULT,
					  fl::Texture::Sampler sampler = fl::Texture::Sampler::SAMPLER_2D);

VertexBuffer createVertexBuffer(fl::Engine& engine, const FilameshFileHeader& header, const uint8_t* data);

IndexBuffer createIndexBuffer(fl::Engine& engine, const FilameshFileHeader& header, const uint8_t* data);

} // namespace spatial::render