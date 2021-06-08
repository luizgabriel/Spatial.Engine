#pragma once

#include <spatial/common/Math.h>
#include <spatial/render/Resources.h>

#include <cstdint>
#include <spatial/resources/FilameshFile.h>
#include <string>

namespace spatial::render
{

Material createMaterial(filament::Engine& engine, const std::string& resourceData);

Texture createTexture(filament::Engine& engine, math::int2 dimensions,
					  fl::Texture::InternalFormat format, fl::Texture::Usage usage = fl::Texture::Usage::DEFAULT,
					  fl::Texture::Sampler sampler = fl::Texture::Sampler::SAMPLER_2D);

Texture createTexture(filament::Engine& engine, const std::string& resourceData,
					  fl::Texture::Usage usage = fl::Texture::Usage::DEFAULT,
					  fl::Texture::Sampler sampler = fl::Texture::Sampler::SAMPLER_2D);

VertexBuffer createVertexBuffer(fl::Engine& engine, const FilameshFileHeader& header, const std::vector<char>& vertices);

IndexBuffer createIndexBuffer(fl::Engine& engine, const FilameshFileHeader& header, const std::vector<char>& indices);

} // namespace spatial::render