#include "spatial/ui/components/TreeNode.h"
#include <boost/algorithm/string/join.hpp>
#include <imgui.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Texture.h>
#include <spatial/graphics/Resources.h>
#include <spatial/ui/components/CollapsingHeader.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DirectionInput.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/SceneView.h>
#include <spatial/ui/components/Search.h>
#include <spatial/ui/components/VectorInput.h>

namespace spatial::ui
{

static constexpr ImGuiTableFlags gTableFlags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH
											   | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg
											   | ImGuiTableFlags_NoBordersInBody;

void spacing(std::uint32_t times)
{
	for (std::uint32_t i = 0; i < times; i++)
		ImGui::Spacing();
}

void separator(std::uint32_t spacing)
{
	ui::spacing(spacing);
	ImGui::Separator();
	ui::spacing(spacing);
}

int inputTextCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		auto* str = reinterpret_cast<std::string*>(data->UserData);
		str->resize(static_cast<unsigned long>(data->BufTextLen));
		data->Buf = str->data();
	}

	return 1;
};

bool inputText(const std::string_view label, std::string& value, std::string_view placeholder)
{
	return ImGui::InputTextWithHint(label.data(), placeholder.data(), value.data(), value.size() + 1,
									ImGuiInputTextFlags_CallbackResize, &inputTextCallback, &value);
}

bool inputPath(const std::string_view label, std::string& value, std::string_view placeholder)
{
	if (ui::inputText(label, value, placeholder))
		return true;

	{
		auto dnd = ui::DragAndDropTarget{};
		auto result = dnd.getPayload();
		if (result)
		{
			value = *result;
			return true;
		}
	}

	return false;
}

void image(graphics::OptionalTexture texture, math::vec2 size, math::vec4 uv)
{
	if (!texture)
		return;

	ImGui::Image(*texture, ImVec2(size.x, size.y), ImVec2(uv.x, uv.y), ImVec2(uv.z, uv.w));
}

bool imageButton(graphics::OptionalTexture texture, math::vec2 size, math::vec4 uv)
{
	if (!texture)
		return false;

	return ImGui::ImageButton(*texture, ImVec2(size.x, size.y), ImVec2(uv.x, uv.y), ImVec2(uv.z, uv.w));
}

void spanToAvailWidth(float weight)
{
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * weight);
}

void ComponentInputImpl<ecs::DirectionalLight>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::DirectionalLight>(entity);

	ImGui::Checkbox("Cast Shadows", &light.castShadows);
	ImGui::ColorEdit3("Color", &light.color.r);
	ImGui::SliderFloat("Intensity", &light.intensity, .0f, 100000.0f);
	directionInput("Direction", light.direction);
}

void ComponentInputImpl<ecs::PointLight>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::PointLight>(entity);

	ImGui::ColorEdit3("Color", &light.color.r);
	ImGui::SliderFloat("Intensity", &light.intensity, .0f, 1000000.0f);
	ImGui::InputFloat("Falloff", &light.falloff);
}

void ComponentInputImpl<ecs::SpotLight>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::SpotLight>(entity);

	ImGui::Checkbox("Cast Shadows", &light.castShadows);
	ImGui::ColorEdit3("Color", &light.color.r);
	ImGui::SliderFloat("Intensity", &light.intensity, .0f, 1000000.0f);
	ImGui::InputFloat("Falloff", &light.falloff);
	ImGui::DragFloat("Inner Angle", &light.innerAngle, math::pi / 180.0f, 0, math::pi / 2.0f);
	ImGui::DragFloat("Outer Angle", &light.outerAngle, math::pi / 180.0f, light.innerAngle, math::pi / 2.0f);
	directionInput("Direction", light.direction);
}

void ComponentInputImpl<ecs::SunLight>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::SunLight>(entity);

	ImGui::Checkbox("Cast Shadows", &light.castShadows);
	ImGui::ColorEdit3("Color", &light.color.r);
	ImGui::SliderFloat("Intensity", &light.intensity, .0f, 100000.0f);
	ImGui::InputFloat("Halo Falloff", &light.haloFalloff);
	ImGui::InputFloat("Halo size", &light.haloSize);
}

void ComponentInputImpl<ecs::IndirectLight, graphics::OptionalTexture>::draw(ecs::Registry& registry,
																			 ecs::Entity entity,
																			 graphics::OptionalTexture icons)
{
	auto& light = registry.getComponent<ecs::IndirectLight>(entity);

	ImGui::SliderFloat("Intensity", &light.intensity, .0f, 1000000.0f);
	ui::Search::searchResource<ecs::tags::IsCubeMapTexture>("Reflections Texture", icons, registry,
															light.reflectionsTexture);
	ui::Search::searchResource<ecs::tags::IsIrradianceValues>("Irradiance Values", icons, registry,
															  light.irradianceValues);
}

void ComponentInputImpl<ecs::ScriptModule>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& script = registry.getComponent<ecs::ScriptModule>(entity);

	ui::spacing(3);

	if (!script.systems.empty() && ImGui::BeginTable("ScriptSystemsTable", 3, gTableFlags))
	{
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Required Components", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Excluded Components", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();

		for (const auto& [name, system] : script.systems)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%s", name.c_str());

			ImGui::TableNextColumn();
			ImGui::Text("%s", boost::algorithm::join(system.requiredComponents, ", ").c_str());

			ImGui::TableNextColumn();
			ImGui::Text("%s", boost::algorithm::join(system.excludedComponents, ", ").c_str());
		}

		ImGui::EndTable();
	}

	ui::spacing(3);

	if (!script.components.empty() && ImGui::BeginTable("ScriptComponentsTable", 2, gTableFlags))
	{
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Properties", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();

		for (const auto& [name, component] : script.components)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%s", name.c_str());

			ImGui::TableNextColumn();
			auto props = std::vector<std::string>{};
			props.reserve(component.properties.size());
			std::transform(component.properties.begin(), component.properties.end(), std::back_inserter(props),
						   [](const auto& prop) { return prop.first; });

			ImGui::Text("%s", boost::algorithm::join(props, ", ").c_str());
		}

		ImGui::EndTable();
	}
}

void ComponentInputImpl<ecs::FileSystemResource>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& resource = registry.getComponent<ecs::FileSystemResource>(entity);

	if (registry.hasComponent<ecs::tags::IsImageTexture>(entity))
	{
		ui::previewTexture(registry, entity, std::nullopt);
		ImGui::SameLine();
	}

	ui::inputPath("Resource", resource.relativePath, "project/assets/*");

	ui::spacing(2);

	auto loaded = registry.hasAllComponents<ecs::tags::IsResourceLoaded>(entity);
	ImGui::Checkbox("Is Loaded", &loaded);
	ImGui::SameLine();
	if (loaded && ImGui::Button("Reload"))
		registry.removeComponent<ecs::tags::IsResourceLoaded>(entity);

	ui::spacing(2);

	const auto* error = registry.tryGetComponent<const ecs::ResourceError>(entity);

	if (error)
	{
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.81f, 0.27f, 0.33f, 1.0), "Error: %s", error->errorMessage.c_str());
	}

	ui::spacing(2);
}

void ComponentInputImpl<ecs::MeshInstance, graphics::OptionalTexture>::draw(ecs::Registry& registry, ecs::Entity entity,
																			graphics::OptionalTexture icons)
{
	auto& mesh = registry.getComponent<ecs::MeshInstance>(entity);
	const size_t smallStep = 1, largeStep = 5;

	bool shouldRecreateMesh = Search::searchResource<ecs::tags::IsMesh>("Resource", icons, registry, mesh.meshSource);

	spacing(3);

	ImGui::Checkbox("Cast Shadows", &mesh.castShadows);
	ImGui::SameLine();
	ImGui::Checkbox("Receive Shadows", &mesh.receiveShadows);

	ImGui::Checkbox("Culling", &mesh.culling);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.0f);
	ImGui::InputScalar("Priority", ImGuiDataType_U8, &mesh.priority, &smallStep, &largeStep);

	spacing(3);

	Search::searchEntity<ecs::MaterialInstance>("Default Material", icons, registry, mesh.defaultMaterialInstance);

	spacing(3);

	if (ImGui::TreeNodeEx("Materials", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		spacing(3);

		if (ImGui::Button("Add Slot"))
			ecs::MeshInstance::addMaterial(registry, entity, ecs::NullEntity);

		if (ImGui::BeginTable("MaterialsTable", 3, gTableFlags))
		{
			ImGui::TableSetupColumn("Primitive Index");
			ImGui::TableSetupColumn("Material", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Actions");
			ImGui::TableHeadersRow();

			if (registry.hasAllComponents<ecs::Parent>(entity))
			{
				ecs::Entity childToDestroy = ecs::NullEntity;

				ecs::Parent::forEachChild(registry, entity, [&](ecs::Entity child) {
					if (!registry.hasAllComponents<ecs::MeshPrimitive>(child))
						return;

					auto& meshPrimitive = registry.getComponent<ecs::MeshPrimitive>(child);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
					ImGui::PushID((int)child);
#pragma clang diagnostic pop

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					ui::spanToAvailWidth();
					ImGui::InputScalar("##PrimitiveIndex", ImGuiDataType_U64, &meshPrimitive.primitiveIndex, &smallStep,
									   &largeStep, "%lu");

					ImGui::TableNextColumn();

					ui::spanToAvailWidth();
					ui::Search::searchEntity<ecs::MaterialInstance>("##Material", icons, registry,
																	meshPrimitive.materialInstance);

					ImGui::TableNextColumn();

					ui::spanToAvailWidth();

					if (ImGui::Button("Remove"))
						childToDestroy = child;

					ImGui::PopID();
				});

				if (registry.isValid(childToDestroy))
					registry.addComponent<ecs::tags::CanDestroy>(childToDestroy);
			}

			ImGui::EndTable();
		}

		spacing(3);

		ImGui::TreePop();
	}

	spacing(3);

	if (ImGui::TreeNodeEx("Geometry Slice", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		spacing(3);

		shouldRecreateMesh |=
			ImGui::InputScalar("Parts Count", ImGuiDataType_U64, &mesh.partsCount, &smallStep, &largeStep, "%lu");
		shouldRecreateMesh |=
			ImGui::InputScalar("Parts Offset", ImGuiDataType_U64, &mesh.partsOffset, &smallStep, &largeStep, "%lu");

		ImGui::TreePop();

		spacing(3);
	}

	if (shouldRecreateMesh)
		registry.addOrReplaceComponent<ecs::tags::ShouldRecreateRenderable>(entity);
}

void ComponentInputImpl<ecs::MeshPrimitive, graphics::OptionalTexture>::draw(ecs::Registry& registry,
																			 ecs::Entity entity,
																			 graphics::OptionalTexture icons)
{
	auto& meshPrimitive = registry.getComponent<ecs::MeshPrimitive>(entity);
	const size_t smallStep = 1, largeStep = 1;

	ImGui::InputScalar("Primitive Index", ImGuiDataType_U64, &meshPrimitive.primitiveIndex, &smallStep, &largeStep,
					   "%lu");
	ui::Search::searchEntity<ecs::MaterialInstance>("Material Instance", std::move(icons), registry,
													meshPrimitive.materialInstance);
	ImGui::InputScalar("Blend Order", ImGuiDataType_U16, &meshPrimitive.blendOrder, &smallStep, &largeStep, "%u");
}

void ComponentInputImpl<ecs::Transform>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& transform = registry.getComponent<ecs::Transform>(entity);

	vec3Input("Position", transform.position);

	if (!registry.hasAllComponents<ecs::tags::IsLight>(entity))
	{
		auto eulerAnglesRotation = transform.getEulerAnglesRotation();
		vec3Input("Rotation", eulerAnglesRotation);
		transform.rotation = math::deg2rad * eulerAnglesRotation;

		vec3Input("Scale", transform.scale, 1.0f);
	}
}

void ComponentInputImpl<ecs::PerspectiveCamera>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::PerspectiveCamera>(entity);

	ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	double min = 15.0, max = 180.0, fov = camera.fieldOfView * math::rad2deg_v<double>;
	ImGui::DragScalar("Field Of View", ImGuiDataType_Double, &fov, 1.0f, &min, &max, "%.1f");
	camera.fieldOfView = fov * math::deg2rad_v<double>;

	ImGui::InputDouble("Aspect Ratio", &camera.aspectRatio);

	if (camera.near > camera.far)
		camera.near = camera.far - 1.0;
}

void ComponentInputImpl<ecs::OrthographicCamera>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::OrthographicCamera>(entity);

	ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	ImGui::InputDouble("Left", &camera.left);
	ImGui::InputDouble("Right", &camera.right);
	ImGui::InputDouble("Bottom", &camera.bottom);
	ImGui::InputDouble("Top", &camera.top);
}

void ComponentInputImpl<ecs::CustomCamera>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::CustomCamera>(entity);

	ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	ImGui::InputScalarN("m0", ImGuiDataType_Double, &camera.projectionMatrix[0], 4);
	ImGui::InputScalarN("m1", ImGuiDataType_Double, &camera.projectionMatrix[1], 4);
	ImGui::InputScalarN("m2", ImGuiDataType_Double, &camera.projectionMatrix[2], 4);
	ImGui::InputScalarN("m3", ImGuiDataType_Double, &camera.projectionMatrix[3], 4);
}

void ComponentInputImpl<ecs::View, graphics::OptionalTexture>::draw(ecs::Registry& registry, ecs::Entity entity,
																	graphics::OptionalTexture icons)
{
	auto& sceneView = registry.getComponent<ecs::View>(entity);

	vec2Input("Size", sceneView.size);
	Search::searchEntity<ecs::IndirectLight>("Indirect Light", icons, registry, sceneView.indirectLight);
	Search::searchEntity<ecs::tags::IsCamera>("Camera", icons, registry, sceneView.camera);

	ui::spacing();

	if (auto node = ui::TreeNode{"Camera Properties"}; node.isOpen())
	{
		spacing(3);
		if (registry.hasAllComponents<ecs::PerspectiveCamera>(sceneView.camera))
			ui::componentInput<ecs::PerspectiveCamera>(registry, sceneView.camera);
		else if (registry.hasAllComponents<ecs::OrthographicCamera>(sceneView.camera))
			ui::componentInput<ecs::OrthographicCamera>(registry, sceneView.camera);
		else if (registry.hasAllComponents<ecs::CustomCamera>(sceneView.camera))
			ui::componentInput<ecs::CustomCamera>(registry, sceneView.camera);
		spacing(3);
	}

	ui::spacing();

	if (auto node = ui::TreeNode{"Camera Preview"}; node.isOpen())
		SceneView::image(registry, entity, {ImGui::GetContentRegionAvail().x, 200});
}

void ComponentInputImpl<ecs::Parent, graphics::OptionalTexture>::draw(ecs::Registry& registry, ecs::Entity entity,
																	  graphics::OptionalTexture icons)
{
	auto& parent = registry.getComponent<ecs::Parent>(entity);

	ui::Search::searchEntity<ecs::Name>("First Child", icons, registry, parent.first);
	ui::Search::searchEntity<ecs::Name>("Last Child", icons, registry, parent.last);

	size_t stepSlow = 1, stepFast = 5;
	ImGui::InputScalar("Children Count", ImGuiDataType_U64, &parent.childrenCount, &stepSlow, &stepFast, "%d");
}

void ComponentInputImpl<ecs::Child, graphics::OptionalTexture>::draw(ecs::Registry& registry, ecs::Entity entity,
																	 graphics::OptionalTexture icons)
{
	auto& child = registry.getComponent<ecs::Child>(entity);

	ui::Search::searchEntity<ecs::Name>("Parent", icons, registry, child.parent);
	ui::Search::searchEntity<ecs::Name>("Previous Sibling", icons, registry, child.previous);
	ui::Search::searchEntity<ecs::Name>("Next Sibling", icons, registry, child.next);
}

void ComponentInputImpl<ecs::AttachmentTexture>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& attachment = registry.getComponent<ecs::AttachmentTexture>(entity);

	ui::vec2Input("Size", attachment.size);
	ui::Combo::fromEnum("Type", attachment.type);
}

void ComponentInputImpl<ecs::tags::IsImageTexture>::draw(ecs::Registry& registry, ecs::Entity entity)
{

	auto texture = graphics::getTexture(registry, entity);
	if (!texture || isDepthFormat(texture->get()->getFormat()))
		return;

	auto size = math::vec2{ImGui::GetContentRegionAvail().x, 200.0f};

	{
		const auto currentPosition = ImGui::GetCursorScreenPos();
		const auto rectMax = ImVec2(currentPosition.x + size.x, currentPosition.y + size.y);

		ImGui::GetWindowDrawList()->AddRectFilled(currentPosition, rectMax, IM_COL32(34, 34, 34, 255));
	}

	auto textureAspectRatio = BaseAspectRatio<float>{static_cast<float>(texture->get()->getWidth()),
													 static_cast<float>(texture->get()->getHeight())};
	auto textureSize = math::vec2{textureAspectRatio.get() * size.y, size.y};

	ImGui::SetCursorScreenPos(
		ImVec2(ImGui::GetCursorScreenPos().x + (size.x - static_cast<float>(textureSize.x)) * 0.5f,
			   ImGui::GetCursorScreenPos().y));

	ui::image(std::move(texture), textureSize);
}

void ComponentInputImpl<ecs::MaterialInstance, graphics::OptionalTexture>::draw(ecs::Registry& registry,
																				ecs::Entity entity,
																				graphics::OptionalTexture icons)
{
	auto& materialInstance = registry.getComponent<ecs::MaterialInstance>(entity);
	auto& child = registry.getComponent<ecs::Child>(entity);

	auto parent = child.parent;
	bool changed = ui::Search::searchResource<ecs::tags::IsMaterial>("Material", icons, registry, parent);
	if (changed)
		ecs::MaterialInstance::changeMaterialSource(registry, entity, parent);

	if (auto collapse = ui::TreeNode{"Scissor", false}; collapse.isOpen())
	{
		ImGui::Columns(2);
		ImGui::InputScalar("Left", ImGuiDataType_U32, &materialInstance.scissor.left);
		ImGui::NextColumn();
		ImGui::InputScalar("Bottom", ImGuiDataType_U32, &materialInstance.scissor.bottom);
		ImGui::NextColumn();
		ImGui::InputScalar("Width", ImGuiDataType_U32, &materialInstance.scissor.width);
		ImGui::NextColumn();
		ImGui::InputScalar("Height", ImGuiDataType_U32, &materialInstance.scissor.height);
		ImGui::Columns(1);
	}
}

} // namespace spatial::ui