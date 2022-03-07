#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/script/Isolate.h>
#include <spatial/resources/FilesSystem.h>

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

	v8::Local<v8::Module> compileModule(v8::Local<v8::Context> context, std::string_view modulePath);
};

} // namespace spatial::script