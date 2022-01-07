#pragma once

#include <filesystem>

namespace spatial::editor
{

struct LoadSceneEvent
{
	std::filesystem::path path;
};

struct SaveSceneEvent
{
	std::filesystem::path path;
};

struct ClearSceneEvent
{
};

struct OpenProjectEvent
{
	std::filesystem::path path;
};


} // namespace spatial::editor