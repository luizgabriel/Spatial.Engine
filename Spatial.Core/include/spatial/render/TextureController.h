#pragma once

#include <spatial/resources/FileSystem.h>
#include <filament/Engine.h>
#include <spatial/ecs/Registry.h>

namespace spatial::render
{

class TextureController
{
  public:
	TextureController(filament::Engine& engine, FileSystem& fileSystem);

	void onUpdateFrame(ecs::Registry& registry);

  private:
	filament::Engine& mEngine;
	FileSystem& mFileSystem;
};

}