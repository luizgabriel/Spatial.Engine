#include <spatial/render/ResourceLoaders.h>
#include <spatial/core/Asset.h>
#include <spatial/render/MeshResource.h>
#include <utils/Path.h>

#include <stb_image.h>

#include <fstream>

using namespace spatial::core;
namespace fs = std::filesystem;
namespace fl = filament;
namespace fm = filamesh;

namespace spatial::render
{

Material createMaterial(fl::Engine *engine, const fs::path &filePath)
{
    auto absolutePath = Asset::absolute(filePath);
    auto stream = std::ifstream{absolutePath, std::ios_base::in | std::ios_base::binary};

    if (!stream)
        throw std::runtime_error("could not open file.");

    auto iterator = std::istreambuf_iterator<char>{stream};
    auto data = std::vector<char>{iterator, {}};

    auto material = fl::Material::Builder()
                        .package(&data[0], data.size())
                        .build(*engine);

    return {engine, material};
}

MeshResource createMesh(fl::Engine *engine, fl::MaterialInstance *material, const fs::path &filePath)
{
    auto absolute = Asset::absolute(filePath);
    if (!fs::exists(absolute))
        throw std::runtime_error("could not open file.");

    auto registry = filamesh::MeshReader::MaterialRegistry{};
    registry.registerMaterialInstance(utils::CString("DefaultMaterial"), material);

    auto reader = fm::MeshReader{};
    auto path = utils::Path{absolute.generic_string()};
    auto mesh = reader.loadMeshFromFile(engine, path, registry);

    return {engine, mesh};
}

Texture createTexture(filament::Engine *engine, const std::filesystem::path &filePath)
{
    auto path = Asset::absolute(filePath);

    if (!fs::exists(path))
        throw std::runtime_error("could not open file.");

    int width, height, n;
    unsigned char *data = stbi_load(path.generic_string().c_str(), &width, &height, &n, 4);

    auto buffer = fl::Texture::PixelBufferDescriptor{
        data, size_t(width * height * 4),
        fl::Texture::Format::RGBA,
        fl::Texture::Type::UBYTE,
        (fl::Texture::PixelBufferDescriptor::Callback)&stbi_image_free};

    auto texture = fl::Texture::Builder()
                       .width(uint32_t(width))
                       .height(uint32_t(height))
                       .levels(1)
                       .sampler(fl::Texture::Sampler::SAMPLER_2D)
                       .format(fl::Texture::InternalFormat::RGBA8)
                       .build(*engine);

    texture->setImage(*engine, 0, std::move(buffer));

    return {engine, texture};
}

} // namespace spatial::render