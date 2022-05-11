#pragma once

#include <string>

namespace spatial::editor
{

struct LoadSceneEvent
{
	std::string path;
};

struct SaveSceneEvent
{
	std::string path;
};

struct ClearSceneEvent
{
};

struct OpenProjectEvent
{
	std::string path;
};

} // namespace spatial::editor