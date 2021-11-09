#pragma once

#include "Window.h"
#include <filament/Texture.h>
#include <filesystem>
#include <opencl-c-base.h>
#include <spatial/common/Math.h>
#include <spatial/common/StringHelpers.h>
#include <unordered_map>

namespace spatial::ui
{

class AssetsExplorer
{
  public:
	explicit AssetsExplorer(const std::filesystem::path& rootPath, const filament::Texture& texture);
	~AssetsExplorer();

	bool header(std::filesystem::path& selectedPath);

	bool onSelectPath(std::filesystem::path& selectedPath);

	[[nodiscard]] bool fileButton(const math::float4& uv) const;


  private:
	ui::Window mWindow;
	const std::filesystem::path& mRootPath;
	const filament::Texture& mIconTexture;
};

}