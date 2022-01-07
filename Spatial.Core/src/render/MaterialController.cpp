#include <spatial/render/MaterialController.h>
#include <spatial/render/SkyboxResources.h>

namespace spatial::render
{

MaterialController::MaterialController(filament::Engine& engine) : mEngine{engine}, mJobQueue{}, mTextures{}
{
	mJobQueue.connect<LoadResourceEvent<ImageTexture>>(*this);
	mJobQueue.connect<LoadResourceEvent<CubeMapTexture>>(*this);
}

void MaterialController::load(uint32_t resourceId, Texture&& texture)
{
	mTextures.emplace(resourceId, std::move(texture));
}

void MaterialController::onStartFrame()
{
	mJobQueue.update();
}

void MaterialController::onEvent(const LoadResourceEvent<ImageTexture>& event)
{
	auto result =
		makeAbsolutePath(mRootPath, event.texture.relativePath)
			.and_then(validateResourcePath)
			.and_then([](auto&& res) { return validateExtensions(std::forward<decltype(res)>(res), {".png", ".jpg"}); })
			.and_then(openFileReadStream)
			.transform(toVectorData)
			.transform([this](auto&& data) { return render::createTexture(mEngine, data.data(), data.size()); })
			.map_error(logResourceError);

	if (result.has_value()) {
		mTextures.emplace(event.texture.getId(), std::move(result.value()));
	}
}

void MaterialController::onEvent(const LoadResourceEvent<CubeMapTexture>& event)
{
	auto result =
		makeAbsolutePath(mRootPath, event.texture.relativePath)
			.and_then(validateResourcePath)
			.and_then([](auto&& res) { return validateExtensions(std::forward<decltype(res)>(res), {".ktx"}); })
			.and_then(openFileReadStream)
			.transform(toVectorData)
			.transform([this](auto&& data) { return render::createKtxTexture(mEngine, data.data(), data.size()); })
			.map_error(logResourceError);

	if (result.has_value()) {
		mTextures.emplace(event.texture.getId(), std::move(result.value()));
	}
}

void MaterialController::setRootPath(const std::filesystem::path& rootPath)
{
	mRootPath = rootPath;
}

}