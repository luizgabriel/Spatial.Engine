#include <spatial/graphics/MaterialInstance.h>
#include <spatial/graphics/MathConversionUtils.h>

namespace spatial::graphics
{

MaterialInstance::MaterialInstance(filament::Engine& engine, SharedMaterial material,
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

void MaterialInstance::setParameter(std::string_view parameter, bool value)
{
	mInstance->setParameter(parameter.data(), value);
}

void MaterialInstance::setParameter(std::string_view parameter, float value)
{
	mInstance->setParameter(parameter.data(), value);
}

void MaterialInstance::setParameter(std::string_view parameter, math::vec2 value)
{
	mInstance->setParameter(parameter.data(), toFilament(value));
}

void MaterialInstance::setParameter(std::string_view parameter, math::vec3 value)
{
	mInstance->setParameter(parameter.data(), toFilament(value));
}

void MaterialInstance::setParameter(std::string_view parameter, math::vec4 value)
{
	mInstance->setParameter(parameter.data(), toFilament(value));
}

void MaterialInstance::setParameter(std::string_view parameter, const SharedTexture& texture, filament::TextureSampler sampler)
{
	get()->setParameter(parameter.data(), texture.get(), std::move(sampler));
	mTextureParameters.emplace(std::string{parameter}, texture);
}

void MaterialInstance::setScissor(uint32_t left, uint32_t bottom, uint32_t width, uint32_t height)
{
	get()->setScissor(left, bottom, width, height);
}

SharedMaterialInstance toShared(MaterialInstance&& materialInstance)
{
	return std::make_shared<MaterialInstance>(std::move(materialInstance));
}

MaterialInstance createMaterialInstance(filament::Engine& engine, const SharedMaterial& material,
										const std::string_view name) noexcept
{
	return MaterialInstance{engine, material, material->createInstance(name.data())};
}

} // namespace spatial::graphics