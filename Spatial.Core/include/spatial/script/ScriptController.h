#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/resources/FileSystem.h>
#include <spatial/script/Isolate.h>

namespace spatial::script
{

class ScriptController
{
  public:
	explicit ScriptController(FileSystem& mFileSystem, Isolate&& isolate);

	void onUpdateFrame(ecs::Registry& registry, float delta);

  private:
	FileSystem& mFileSystem;
	Isolate mIsolate;
	v8::HandleScope mScope;
};

} // namespace spatial::script