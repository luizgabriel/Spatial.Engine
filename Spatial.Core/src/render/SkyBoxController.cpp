#include <boost/algorithm/string/predicate.hpp>
#include <spatial/ecs/SkyBox.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/Resources.h>
#include <spatial/render/SkyBoxController.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/resources/ResourceLoader.h>

namespace spatial::render
{

SkyBoxController::SkyBoxController(filament::Engine& engine) : mEngine{engine}, mRootPath{}, mTextures{}
{
}

void SkyBoxController::onUpdateFrame(ecs::Registry& registry)
{
	registry.getEntities<const ecs::SkyBoxColor>(ecs::ExcludeComponents<Skybox>)
		.each([&](ecs::Entity entity, const ecs::SkyBoxColor& component) {
			registry.addComponent<Skybox>(entity, createSkybox(mEngine, component.color));
		});

	registry.getEntities<const ecs::SkyBoxColor, Skybox>().each(
		[&](const ecs::SkyBoxColor& component, Skybox& skybox) {
			skybox->setColor(component.color);
		});

	registry.getEntities<const ecs::SkyBoxTexture>().each([&, this](const auto& component) {
		using namespace boost::algorithm;

		if (mTextures.find(component.getResourceId()) != mTextures.end())
			return;

		if (!ends_with(component.texturePath.c_str(), ".ktx"))
			return;

		/*
		 auto result = makeAbsolutePath(mRootPath, component.texturePath)
						  .and_then(validateResourcePath)
						  .and_then(openFileReadStream)
						  .transform(toVectorData)
						  .and_then([&](const auto& data) { return createTexture(data); })
						  .map_error(logResourceError);

		if (result.has_value())
			mTextures.emplace(component.getResourceId(), std::move(result.value()));
		 */
	});

	registry.getEntities<const ecs::SkyBoxTexture>(ecs::ExcludeComponents<Skybox>)
		.each([&](ecs::Entity entity, const ecs::SkyBoxTexture& component) {
			if (mTextures.find(component.getResourceId()) == mTextures.end())
				return;

			auto& texture = mTextures.at(component.getResourceId());
			registry.addComponent<Skybox>(entity, createSkybox(mEngine, texture.ref(), component.showSun));
		});
}

void SkyBoxController::setRootPath(const std::filesystem::path& rootPath)
{
	mRootPath = rootPath;
}

tl::expected<Texture, ResourceError> SkyBoxController::createTexture(const std::vector<uint8_t>&& data)
{
	try
	{
		return createKtxTexture(mEngine, data.data(), data.size());
	}
	catch (const std::exception& e)
	{
		return tl::make_unexpected(ResourceError::ParseError);
	}
}

} // namespace spatial::render