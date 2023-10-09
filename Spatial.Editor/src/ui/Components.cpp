#include "Components.h"
#include "../ecs/Components.h"
#include "MaterialInputs.h"
#include "Search.h"
#include <boost/algorithm/string/join.hpp>
#include <spatial/ui/components/PreviewTexture.h>
#include <spatial/ui/components/TreeNode.h>

namespace spatial::ui
{

static constexpr ImGuiTableFlags gTableFlags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH
											   | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg
											   | ImGuiTableFlags_NoBordersInBody;

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

void ComponentInputImpl<ecs::MeshPart>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& resource = registry.getComponent<ecs::MeshPart>(entity);

	ImGui::InputScalar("Min Index", ImGuiDataType_U32, &resource.minIndex);
	ImGui::InputScalar("Index Count", ImGuiDataType_U32, &resource.indexCount);
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

					ImGui::PushID((int)child);

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

	auto& child = registry.getComponent<ecs::Child>(entity);

	auto meshInstance = child.parent;
	if (ui::Search::searchEntity<ecs::MeshInstance>("Mesh Instance", icons, registry, meshInstance))
		ecs::MeshPrimitive::changeMeshInstance(registry, entity, meshInstance);

	ui::Search::searchEntity<ecs::MaterialInstance>("Material Instance", std::move(icons), registry,
													meshPrimitive.materialInstance);
	ImGui::InputScalar("Primitive Index", ImGuiDataType_U64, &meshPrimitive.primitiveIndex, &smallStep, &largeStep,
					   "%lu");
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
	ImGui::Checkbox("Post Processing Enabled", &sceneView.isPostProcessingEnabled);
	ImGui::Checkbox("Shadowing Enabled", &sceneView.isShadowingEnabled);

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

	ui::Search::searchResource<ecs::tags::IsMaterial>("Material", icons, registry, materialInstance.materialSource);

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

void ComponentInputImpl<editor::ColorMaterial>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& material = registry.getComponent<editor::ColorMaterial>(entity);

	ImGui::ColorEdit3("Color", &material.baseColor.r);
	ImGui::DragFloat("Metallic", &material.metallic, 0.01f, .0f, 1.0f, "%.2f");
	ImGui::DragFloat("Reflectance", &material.reflectance, 0.01f, .0f, 1.0f, "%.2f");
	ImGui::DragFloat("Roughness", &material.roughness, 0.01f, .0f, 1.0f, "%.2f");
}

void ComponentInputImpl<editor::EditorCamera>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& data = registry.getComponent<editor::EditorCamera>(entity);

	ImGui::DragFloat("Velocity", &data.velocity);
	ImGui::DragFloat("Sensitivity", &data.sensitivity);
}

void ComponentInputImpl<editor::GridMaterial>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& data = registry.getComponent<editor::GridMaterial>(entity);

	ImGui::ColorEdit3("Color", &data.color.r);
	ImGui::DragFloat("Thickness", &data.thickness, 0.001f, .0f, 1.0f);
	ImGui::DragFloat2("Scale", &data.scale.x, 0.01f, .0f, 100.0f);
}

void ComponentInputImpl<editor::SkyBoxMaterial, graphics::OptionalTexture>::draw(ecs::Registry& registry,
																				 ecs::Entity entity,
																				 graphics::OptionalTexture icons)
{
	auto& data = registry.getComponent<editor::SkyBoxMaterial>(entity);

	ui::colorPicker("Background Color", data.color, icons);
	ui::cubemapInput("Cubemap", registry, data.skybox, icons);
	ImGui::Checkbox("Show Sun", &data.showSun);
}

void ComponentInputImpl<editor::StandardOpaqueMaterial, graphics::OptionalTexture>::draw(
	ecs::Registry& registry, ecs::Entity entity, graphics::OptionalTexture icons)

{
	auto& data = registry.getComponent<editor::StandardOpaqueMaterial>(entity);

	ui::albedoInput("Albedo", data.baseColor, registry, data.albedo, icons);

	ui::separator(2);

	ImGui::DragFloat2("Tiling", &data.tiling.x, .01f, .01f);
	ImGui::DragFloat2("Offset", &data.offset.x, .01f);

	ui::separator(1);

	ui::mapInput("Metallic", data.metallic, registry, data.metallicMap, icons);

	ui::separator(1);

	ui::mapInput("Roughness", data.roughness, registry, data.roughnessMap, icons);

	ui::separator(1);

	ui::mapInput("Reflectance", data.reflectance, registry, data.reflectanceMap, icons);

	ui::separator(1);

	ui::mapInput("Ambient Occlusion", registry, data.ambientOcclusionMap, icons);

	ui::separator(1);

	ui::mapInput("Normal", registry, data.normalMap, icons, Icons::normalMap.uv());

	ui::separator(1);

	ui::mapInput("Height Map", data.height, registry, data.heightMap, icons);

	ui::separator(1);

	ui::colorPicker("Emissive", data.emissive, icons);
}

} // namespace spatial::ui