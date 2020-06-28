#pragma once

#include <filesystem>
#include <functional>
#include <unordered_map>
#include <variant>
#include <string>
#include <spatial/common/Signal.h>
#include <spatial/common/ResourceUtils.h>

namespace spatial
{

struct DefaultAssetLoader
{
	std::filesystem::path basePath;
	std::istream operator()(const std::filesystem::path& resourcePath) const;
};

class Asset
{
public:
	using ResourceStream = std::variant<std::ifstream, MemoryStream>;
	using ResourceLoaderFunc = ResourceStream(const std::filesystem::path&);
	using ResourceDelegate = entt::delegate<ResourceLoaderFunc>;
	using NamespaceLoaderMap = std::unordered_map<std::string, ResourceDelegate>;

	template <ResourceLoaderFunc LoaderFunction>
	static void define(std::string name)
	{
		sNamespaceLoaderMap[name].connect<LoaderFunction>();
	}

	template <typename LoaderInstance>
	static void define(std::string name, LoaderInstance& loader)
	{
		sNamespaceLoaderMap[name].connect<decltype(loader)::operator()>(loader);
	}

	template <ResourceLoaderFunc LoaderFunction, typename LoaderInstance>
	static void define(std::string name, LoaderInstance& loader)
	{
		sNamespaceLoaderMap[name].connect<LoaderFunction>(loader);
	}

	static ResourceStream resolve(const std::string& path)
	{
		auto protocolSeparator = path.find("://");
		auto namespacePart = path.substr(0, protocolSeparator - 1);
		auto delegate = sNamespaceLoaderMap.at(namespacePart);

		return delegate(path.substr(protocolSeparator + 1));
	}

private:

	static NamespaceLoaderMap sNamespaceLoaderMap;
};

} // namespace spatial