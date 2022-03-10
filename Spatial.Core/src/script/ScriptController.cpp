#include <boost/algorithm/string/predicate.hpp>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Script.h>
#include <spatial/script/ScriptController.h>
#include <spatial/script/ScriptSourceStream.h>
#include <spatial/script/Utils.h>

using namespace boost::algorithm;

namespace spatial::script
{

static auto gLogger = createDefaultLogger();

ScriptController::ScriptController(FileSystem& fileSystem, Isolate&& isolate)
	: mFileSystem{fileSystem}, mIsolate{std::move(isolate)}, mScope{mIsolate.get()}
{
}

void ScriptController::onUpdateFrame(ecs::Registry& registry, float delta)
{
	registry.getEntities<const ecs::Script>(ecs::ExcludeComponents<ecs::tags::IsScriptLoaded>)
		.each([&](ecs::Entity entity, const ecs::Script& script) {
			if (script.resource.isEmpty() || !ends_with(script.resource.filename(), ".js"))
				return;

			auto scriptDefaultName = registry.hasAllComponents<ecs::Name>(entity)
										 ? registry.getComponent<ecs::Name>(entity).name
										 : script.resource.relativePath.stem().string();

			registry.addComponent<ecs::tags::IsScriptLoaded>(entity);

			auto context = v8::Context::New(mIsolate.get());
			try {
				auto module = compileModule(context, script.resource.relativePath.c_str());
				validatorModule(scriptDefaultName, context, module);

				registry.addComponent<v8::Local<v8::Context>>(entity, context);
				registry.addComponent<v8::Local<v8::Module>>(entity, module);

			} catch (const std::invalid_argument& e) {
				gLogger.error(e.what());
				registry.addComponent<ecs::tags::IsScriptInvalid>(entity);
				return;
			}

		});
}
void ScriptController::validatorModule(std::string& scriptDefaultName, v8::Local<v8::Context> context,
									   v8::Local<v8::Module> module)
{
	auto moduleNamespace = evaluateModule(context, module);
	auto defaultExport = cast<v8::Object>(getAttribute(moduleNamespace, "default"), "You must export default an Object");

	auto defaultName = createString(mIsolate.get(), scriptDefaultName);
	auto moduleName = cast<v8::String>(getAttributeOrDefault(defaultExport, "name", defaultName), "The module name must be a String");

	gLogger.info("Hello, from module: {}", getValue(mIsolate.get(), moduleName));

	cast<v8::Function>(getAttribute(defaultExport, "onUpdateEntity"), "The module onUpdateEntity must be a Function");
}

v8::Local<v8::Module> ScriptController::compileModule(v8::Local<v8::Context> context, std::string_view modulePath)
{
	auto resourceStream = mFileSystem.openReadStream(modulePath);
	if (resourceStream->fail())
		throw std::invalid_argument{fmt::format("Script not found: {}", modulePath)};

	auto module = script::compileModule(context, std::move(resourceStream), modulePath);
	auto result =
		module
			->InstantiateModule(context,
								[](v8::Local<v8::Context> context, v8::Local<v8::String> specifier,
								   v8::Local<v8::FixedArray> import_assertions, v8::Local<v8::Module> referrer) {
									context->GetIsolate()->ThrowException(
										createString(context->GetIsolate(), "Module importing is not implemented yet"));
									return v8::MaybeLocal<v8::Module>();
								})
			.ToChecked();

	if (!result || module.IsEmpty())
	{
		auto exception = module->GetException().As<v8::String>();
		throw std::invalid_argument{
			fmt::format("Could not instantiate module: {}, Exception: {}", modulePath, getValue(mIsolate.get(), exception))};
	}

	return module;
}

} // namespace spatial::script