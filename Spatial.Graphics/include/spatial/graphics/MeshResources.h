#pragma once

#include <spatial/graphics/Resources.h>
#include <spatial/resources/FilameshFile.h>

namespace spatial::graphics
{

filament::VertexBuffer::Builder createVertexBufferBuilder(const FilameshFileHeader& header);

filament::VertexBuffer::BufferDescriptor createVertexBufferDescriptor(const FilameshFile& filamesh);

VertexBuffer createVertexBuffer(filament::Engine& engine, const FilameshFile& filamesh);

filament::IndexBuffer::Builder createIndexBufferBuilder(const FilameshFileHeader& header);

filament::IndexBuffer::BufferDescriptor createIndexBufferDescriptor(const FilameshFile& filamesh);

IndexBuffer createIndexBuffer(filament::Engine& engine, const FilameshFile& filamesh);

MeshGeometries createMeshGeometries(const FilameshFile& filamesh);

} // namespace spatial::graphics