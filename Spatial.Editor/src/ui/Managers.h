#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/graphics/TextureUtils.h>

namespace spatial::ui
{

class SceneTree
{
  public:
	static void displayTree(ecs::Registry& registry, bool showDebugEntities = false, std::string_view search = "");

  private:
	static void displayNode(ecs::Registry& registry, ecs::Entity entity);
};

struct ResourceManager
{
	enum class ResourceType
	{
		All,
		Material,
		Mesh,
		Script,
		Texture,
	};

	static void header(std::string& search, ResourceType& filter, graphics::OptionalTexture icons);
	static void list(ecs::Registry& registry, std::string_view search, ResourceManager::ResourceType type,
					 bool showEditorEntities);
};

struct MaterialsManager
{
	static void list(ecs::Registry& registry, std::string_view search, bool showEditorEntities);
};

struct ViewsManager
{
	static void list(ecs::Registry& registry, std::string_view search, bool showEditorEntities);
};

} // namespace spatial::ui