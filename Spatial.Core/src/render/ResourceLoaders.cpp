#include <spatial/render/ResourceLoaders.h>
#include <spatial/core/Asset.h>
#include <spatial/render/Mesh.h>
#include <utils/Path.h>

#include <stb_image.h>

#include <image/KtxBundle.h>
#include <image/KtxUtility.h>

#include <filament/IndirectLight.h>
#include <filament/Skybox.h>

#include <fstream>
#include <array>
#include <string>

using namespace spatial::core;
using namespace filament::math;
using namespace std::string_literals;
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

Mesh createMesh(fl::Engine *engine, fl::MaterialInstance *material, const fs::path &filePath)
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

Texture createTexture(filament::Engine *engine, const fs::path &filePath)
{
    auto path = Asset::absolute(filePath);

    if (!fs::exists(path))
        throw std::runtime_error("could not open file:"s + path.generic_string());

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

Texture createKtxTexture(filament::Engine *engine, const fs::path &filePath)
{
    using namespace std;

    auto absolutePath = Asset::absolute(filePath);
    auto stream = ifstream{absolutePath, std::ios_base::in | ios::binary};

    if (!stream)
        throw std::runtime_error("Could not open file: "s + filePath.generic_string());

    auto contents = vector<uint8_t>{istreambuf_iterator<char>(stream), {}};

    // we are using "new" here because of this legacy api
    // but this pointer is released with the texture holding it
    auto ktxBundle = new image::KtxBundle(contents.data(), contents.size());
    return {engine, image::ktx::createTexture(engine, ktxBundle, false)};
}

using bands_t = std::array<float3, 9>;
bands_t parseShFile(const fs::path &file)
{
    auto bands = bands_t{};
    auto absolutePath = Asset::absolute(file);
    auto stream = std::ifstream{absolutePath, std::ios_base::in};

    if (!stream)
        throw std::runtime_error("Could not open file: "s + absolutePath.generic_string());

    stream >> std::skipws;

    char c;
    for (float3 &band : bands)
    {
        while (stream >> c && c != '(');
        stream >> band.r;
        stream >> c;
        stream >> band.g;
        stream >> c;
        stream >> band.b;
    }

    return bands;
}

ImageBasedLight createIBLFromKtx(filament::Engine *engine, const std::filesystem::path &folder, const std::string &name)
{
    auto iblPath = folder / (name + "_ibl.ktx");
    auto texture = createKtxTexture(engine, iblPath);

    auto skyPath = folder / (name + "_skybox.ktx");
    auto skyboxTexture = createKtxTexture(engine, skyPath);

    auto shPath = folder / "sh.txt";
    auto bands = parseShFile(shPath);

    auto light = IndirectLight{engine, fl::IndirectLight::Builder()
                                           .reflections(texture.get())
                                           .irradiance(3, &bands[0])
                                           .intensity(30000.0f)
                                           .build(*engine)};

    auto skybox = Skybox{engine, fl::Skybox::Builder()
                                     .environment(skyboxTexture.get())
                                     .showSun(true)
                                     .build(*engine)};

    return {
        std::move(light),
        std::move(texture), 
        std::move(skybox),
        std::move(skyboxTexture)
    };
}

} // namespace spatial::render