#pragma once

#include "../../../../../Spatial.Editor/src/ui/Search.h"
#include <filament/Texture.h>
#include <spatial/common/Math.h>
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/Light.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/ecs/Name.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Resource.h>
#include <spatial/ecs/Script.h>
#include <spatial/ecs/Transform.h>
#include <spatial/ecs/View.h>
#include <spatial/graphics/TextureUtils.h>
#include <spatial/ui/components/CollapsingHeader.h>
#include <spatial/ui/components/DirectionInput.h>
#include <spatial/ui/components/SceneView.h>
#include <spatial/ui/components/VectorInput.h>

namespace spatial::ui
{

void spanToAvailWidth(float weight = 1.0F);

bool inputText(std::string_view label, std::string& value, std::string_view placeholder = "");

bool inputPath(std::string_view label, std::string& value, std::string_view placeholder = "");

void spacing(std::uint32_t times = 1);

void separator(std::uint32_t spacing = 0);

void image(graphics::OptionalTexture texture, math::vec2 size = math::vec2{0, 0},
		   math::vec4 uv = math::vec4{0, 0, 1, 1});

bool imageButton(graphics::OptionalTexture texture, math::vec2 size = math::vec2{0, 0},
				 math::vec4 uv = math::vec4{0, 0, 1, 1});

} // namespace spatial::ui