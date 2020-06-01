#pragma once

#include <spatial/render/EngineResource.h>
#include <spatial/render/Resources.h>
#include <spatial/render/ResourceLoaders.h>

#include <unordered_map>
#include <filesystem>

namespace spatial
{

template <typename ResourceType, typename KeyType, auto ResourceCreator>
class ResourceManager
{
private:
	filament::Engine* m_engine;
	std::unordered_map<KeyType, ResourceType> m_resources;

public:
	explicit ResourceManager(filament::Engine* engine)
		: m_engine{engine}
	{
	}

	template <typename... Args>
	void load(KeyType&& key, Args&&... args)
	{
		m_resources.emplace(key, std::invoke(ResourceCreator, m_engine, std::forward<Args>(args)...));
	}

	void unload(const KeyType& key)
	{
		m_resources.at(name).reset();
	}

	[[nodiscard]] ResourceType& ref(const KeyType& key)
	{
		return m_resources.at(key);
	}

	[[nodiscard]] auto get(const KeyType& key)
	{
		return ref(key).get();
	}

};

using TextureManager = ResourceManager<Texture, std::string, &createTexture>;

using MaterialManager = ResourceManager<Material, std::string, &createMaterial>;

using MaterialInstanceManager = ResourceManager<MaterialInstance, std::uint32_t, &createMaterialInstance>;

using MeshManager = ResourceManager<Mesh, std::string, &createMesh>;

}