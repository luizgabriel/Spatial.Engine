#include <spatial/graphics/MaterialInstance.h>

#include <utility>

namespace spatial::graphics
{

MaterialInstance::MaterialInstance(filament::Engine& engine, MaterialInstance::SharedMaterial material,
								   filament::MaterialInstance* materialInstance)
	: mEngine{engine}, mMaterial{std::move(material)}, mInstance{mEngine, materialInstance}
{
}

MaterialInstance::MaterialInstance(MaterialInstance&& other) noexcept
	: mEngine{other.mEngine}, mMaterial{std::move(other.mMaterial)}, mInstance{std::move(other.mInstance)}
{
}

MaterialInstance& MaterialInstance::operator=(MaterialInstance&& other) noexcept
{
	mMaterial = other.mMaterial;
	mInstance = std::move(other.mInstance);
	return *this;
}

std::shared_ptr<MaterialInstance> toShared(MaterialInstance&& materialInstance)
{
	return std::make_shared<MaterialInstance>(std::move(materialInstance));
}

} // namespace spatial::graphics