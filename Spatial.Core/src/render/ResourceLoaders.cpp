#include <spatial/render/ResourceLoaders.h>
#include <spatial/core/Asset.h>
#include <spatial/render/MeshResource.h>
#include <utils/Path.h>

using namespace spatial::core;
namespace fs = std::filesystem;
namespace fl = filament;
namespace fm = filamesh;

namespace spatial::render
{

Material createMaterial(fl::Engine *engine, const fs::path &name)
{
	return createMaterial(engine, Asset::read(name));
}

MeshResource createMesh(fl::Engine* engine, fl::MaterialInstance* material, const fs::path &name)
{
	auto registry = filamesh::MeshReader::MaterialRegistry{};
	registry.registerMaterialInstance(utils::CString("DefaultMaterial"), material);

	auto reader = fm::MeshReader{};
	auto path = utils::Path{Asset::absolute(name).generic_string()};
	auto mesh = reader.loadMeshFromFile(engine, path, registry);

	return {engine, mesh};
}

}