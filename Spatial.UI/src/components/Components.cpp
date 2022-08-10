#include <boost/algorithm/string/join.hpp>
#include <imgui.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/Texture.h>
#include <spatial/graphics/Resources.h>
#include <spatial/ui/components/Collapse.h>
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

void image(const filament::Texture* texture, math::vec2 size, math::vec4 uv)
{
	if (texture == nullptr)
		return;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
	ImGui::Image((ImTextureID)texture, ImVec2(size.x, size.y), ImVec2(uv.x, uv.y), ImVec2(uv.z, uv.w));
#pragma clang diagnostic pop
}

bool imageButton(const filament::Texture* texture, math::vec2 size, math::vec4 uv)
{
	if (texture == nullptr)
		return false;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
	return ImGui::ImageButton((ImTextureID)texture, ImVec2(size.x, size.y), ImVec2(uv.x, uv.y), ImVec2(uv.z, uv.w));
#pragma clang diagnostic pop
}

void spanToAvailWidth(float weight)
{
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * weight);
}

bool ComponentInputImpl<ecs::DirectionalLight>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::DirectionalLight>(entity);
	bool changed = false;

	changed |= ImGui::Checkbox("Cast Shadows", &light.castShadows);
	changed |= ImGui::ColorEdit3("Color", &light.color.r);
	changed |= ImGui::SliderFloat("Intensity", &light.intensity, .0f, 100000.0f);
	changed |= directionInput("Direction", light.direction);

	return changed;
}

bool ComponentInputImpl<ecs::PointLight>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::PointLight>(entity);
	bool changed = false;

	changed |= ImGui::ColorEdit3("Color", &light.color.r);
	changed |= ImGui::SliderFloat("Intensity", &light.intensity, .0f, 1000000.0f);
	changed |= ImGui::InputFloat("Falloff", &light.falloff);

	return changed;
}

bool ComponentInputImpl<ecs::SpotLight>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::SpotLight>(entity);
	bool changed = false;

	changed |= ImGui::Checkbox("Cast Shadows", &light.castShadows);
	changed |= ImGui::ColorEdit3("Color", &light.color.r);
	changed |= ImGui::SliderFloat("Intensity", &light.intensity, .0f, 1000000.0f);
	changed |= ImGui::InputFloat("Falloff", &light.falloff);
	changed |= ImGui::DragFloat("Inner Angle", &light.innerAngle, math::pi / 180.0f, 0, math::pi / 2.0f);
	changed |= ImGui::DragFloat("Outer Angle", &light.outerAngle, math::pi / 180.0f, light.innerAngle, math::pi / 2.0f);
	changed |= directionInput("Direction", light.direction);

	return changed;
}

bool ComponentInputImpl<ecs::SunLight>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::SunLight>(entity);
	bool changed = false;

	changed |= ImGui::Checkbox("Cast Shadows", &light.castShadows);
	changed |= ImGui::ColorEdit3("Color", &light.color.r);
	changed |= ImGui::SliderFloat("Intensity", &light.intensity, .0f, 100000.0f);
	changed |= ImGui::InputFloat("Halo Falloff", &light.haloFalloff);
	changed |= ImGui::InputFloat("Halo size", &light.haloSize);

	return changed;
}

bool ComponentInputImpl<ecs::IndirectLight, const filament::Texture*>::draw(ecs::Registry& registry, ecs::Entity entity,
																			const filament::Texture* icons)
{
	auto& light = registry.getComponent<ecs::IndirectLight>(entity);
	bool changed = false;

	changed |= ImGui::SliderFloat("Intensity", &light.intensity, .0f, 1000000.0f);
	changed |= ui::Search::searchResource<ecs::tags::IsCubeMapTexture>("Reflections Texture", icons, registry,
																	   light.reflectionsTexture);
	changed |= ui::Search::searchResource<ecs::tags::IsIrradianceValues>("Irradiance Values", icons, registry,
																		 light.irradianceValues);

	return changed;
}

bool ComponentInputImpl<ecs::ScriptModule>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& script = registry.getComponent<ecs::ScriptModule>(entity);
	bool changed = false;

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

	return changed;
}

bool ComponentInputImpl<ecs::Resource>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& resource = registry.getComponent<ecs::Resource>(entity);
	bool changed = false;

	changed |= ui::inputPath("Resource", resource.relativePath, "project/assets/*");

	auto loaded = registry.hasAllComponents<ecs::tags::IsResourceLoaded>(entity);

	ImGui::Checkbox("Is Loaded", &loaded);

	if (loaded && ImGui::Button("Reload"))
		registry.removeComponent<ecs::tags::IsResourceLoaded>(entity);

	ui::spacing(2);

	const auto* error = registry.tryGetComponent<const ecs::ResourceError>(entity);

	if (error)
	{
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.81f, 0.27f, 0.33f, 1.0), "Error: %s", error->errorMessage.c_str());
	}

	return changed;
}

bool ComponentInputImpl<ecs::MeshInstance, const filament::Texture*>::draw(ecs::Registry& registry, ecs::Entity entity,
																		   const filament::Texture* icons)
{
	auto& mesh = registry.getComponent<ecs::MeshInstance>(entity);
	bool changed = false;
	const size_t smallStep = 1, largeStep = 5;

	bool shouldRecreateMesh = Search::searchResource<ecs::tags::IsMesh>("Resource", icons, registry, mesh.meshSource);

	spacing(3);

	changed |= ImGui::Checkbox("Cast Shadows", &mesh.castShadows);
	ImGui::SameLine();
	changed |= ImGui::Checkbox("Receive Shadows", &mesh.receiveShadows);

	changed |= ImGui::Checkbox("Culling", &mesh.culling);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.0f);
	changed |= ImGui::InputScalar("Priority", ImGuiDataType_U8, &mesh.priority, &smallStep, &largeStep);

	spacing(3);

	changed |=
		Search::searchEntity<ecs::tags::IsMaterialInstance>("Default Material", icons, registry, mesh.defaultMaterial);

	spacing(3);

	if (ImGui::TreeNodeEx("Materials", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		spacing(3);

		if (ImGui::Button("Add Slot"))
		{
			ecs::MeshInstance::addMaterial(registry, entity, ecs::NullEntity);
			shouldRecreateMesh = true;
			changed = true;
		}

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
					if (!registry.hasAllComponents<ecs::MeshMaterial>(child))
						return;

					auto& meshMaterial = registry.getComponent<ecs::MeshMaterial>(child);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
					ImGui::PushID((int)child);
#pragma clang diagnostic pop

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					ui::spanToAvailWidth();
					ImGui::InputScalar("##PrimitiveIndex", ImGuiDataType_U64, &meshMaterial.primitiveIndex, &smallStep,
									   &largeStep, "%lu");

					ImGui::TableNextColumn();

					ui::spanToAvailWidth();
					ui::Search::searchEntity<ecs::tags::IsMaterialInstance>("##Material", icons, registry,
																			meshMaterial.materialInstanceEntity);

					ImGui::TableNextColumn();

					ui::spanToAvailWidth();

					if (ImGui::Button("Remove"))
						childToDestroy = child;

					ImGui::PopID();
				});

				if (registry.isValid(childToDestroy))
				{
					registry.addComponent<ecs::tags::CanDestroy>(childToDestroy);
					shouldRecreateMesh = true;
					changed = true;
				}
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

		shouldRecreateMesh =
			ImGui::InputScalar("Parts Count", ImGuiDataType_U64, &mesh.slice.count, &smallStep, &largeStep, "%lu");
		changed |= shouldRecreateMesh;
		changed |=
			ImGui::InputScalar("Parts Offset", ImGuiDataType_U64, &mesh.slice.offset, &smallStep, &largeStep, "%lu");
		ImGui::TreePop();

		spacing(3);
	}

	if (shouldRecreateMesh)
	{
		registry.addOrReplaceComponent<ecs::tags::ShouldRecreateRenderable>(entity);
		changed = true;
	}

	return changed;
}

bool ComponentInputImpl<ecs::MeshMaterial, const filament::Texture*>::draw(ecs::Registry& registry, ecs::Entity entity,
																		   const filament::Texture* icons)
{
	auto& meshMaterial = registry.getComponent<ecs::MeshMaterial>(entity);
	const size_t smallStep = 1, largeStep = 1;
	bool changed = false;

	changed |= ImGui::InputScalar("Primitive Index", ImGuiDataType_U64, &meshMaterial.primitiveIndex, &smallStep,
								  &largeStep, "%lu");
	changed |= ui::Search::searchEntity<ecs::tags::IsMaterialInstance>("Material", icons, registry,
																	   meshMaterial.materialInstanceEntity);

	return changed;
}

bool ComponentInputImpl<ecs::Transform>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& transform = registry.getComponent<ecs::Transform>(entity);
	bool changed = false;

	changed |= vec3Input("Position", transform.position);

	if (!registry.hasAllComponents<ecs::tags::IsLight>(entity))
	{
		auto eulerAnglesRotation = transform.getEulerAnglesRotation();
		changed |= vec3Input("Rotation", eulerAnglesRotation);
		transform.rotation = math::deg2rad * eulerAnglesRotation;

		changed |= vec3Input("Scale", transform.scale, 1.0f);
	}

	return changed;
}

bool ComponentInputImpl<ecs::PerspectiveCamera>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::PerspectiveCamera>(entity);
	bool changed = false;

	changed |= ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	changed |= ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	double min = 15.0, max = 180.0, fov = camera.fieldOfView * math::rad2deg_v<double>;
	changed |= ImGui::DragScalar("Field Of View", ImGuiDataType_Double, &fov, 1.0f, &min, &max, "%.1f");
	camera.fieldOfView = fov * math::deg2rad_v<double>;

	changed |= ImGui::InputDouble("Aspect Ratio", &camera.aspectRatio);

	if (camera.near > camera.far)
		camera.near = camera.far - 1.0;

	return changed;
}

bool ComponentInputImpl<ecs::OrthographicCamera>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::OrthographicCamera>(entity);
	bool changed = false;

	changed |= ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	changed |= ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	changed |= ImGui::InputDouble("Left", &camera.left);
	changed |= ImGui::InputDouble("Right", &camera.right);
	changed |= ImGui::InputDouble("Bottom", &camera.bottom);
	changed |= ImGui::InputDouble("Top", &camera.top);

	return changed;
}

bool ComponentInputImpl<ecs::CustomCamera>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::CustomCamera>(entity);
	auto changed = false;

	changed |= ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	changed |= ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	changed |= ImGui::InputScalarN("m0", ImGuiDataType_Double, &camera.projectionMatrix[0], 4);
	changed |= ImGui::InputScalarN("m1", ImGuiDataType_Double, &camera.projectionMatrix[1], 4);
	changed |= ImGui::InputScalarN("m2", ImGuiDataType_Double, &camera.projectionMatrix[2], 4);
	changed |= ImGui::InputScalarN("m3", ImGuiDataType_Double, &camera.projectionMatrix[3], 4);

	return changed;
}

bool ComponentInputImpl<ecs::Scene, const filament::Texture*>::draw(ecs::Registry& registry, ecs::Entity entity,
																	const filament::Texture* icons)
{
	auto& sceneView = registry.getComponent<ecs::Scene>(entity);
	auto changed = false;

	changed |= vec2Input("Size", sceneView.size);
	changed |= Search::searchEntity<ecs::IndirectLight>("Indirect Light", icons, registry, sceneView.indirectLight);
	changed |= Search::searchEntity<ecs::tags::IsCamera>("Camera", icons, registry, sceneView.camera);

	if (registry.hasAnyComponent<ecs::tags::IsCamera>(sceneView.camera)
		&& ImGui::TreeNodeEx("Camera Properties", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		spacing(3);
		if (registry.hasAllComponents<ecs::PerspectiveCamera>(sceneView.camera))
			ui::componentInput<ecs::PerspectiveCamera>(registry, sceneView.camera);
		else if (registry.hasAllComponents<ecs::OrthographicCamera>(sceneView.camera))
			ui::componentInput<ecs::OrthographicCamera>(registry, sceneView.camera);
		else if (registry.hasAllComponents<ecs::CustomCamera>(sceneView.camera))
			ui::componentInput<ecs::CustomCamera>(registry, sceneView.camera);
		spacing(3);
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Camera Preview", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		SceneView::image(registry, entity, {ImGui::GetContentRegionAvail().x, 100});
		ImGui::TreePop();
	}

	return changed;
}

bool ComponentInputImpl<ecs::Parent, const filament::Texture*>::draw(ecs::Registry& registry, ecs::Entity entity,
																	 const filament::Texture* icons)
{
	auto& parent = registry.getComponent<ecs::Parent>(entity);
	bool changed = false;

	changed |= ui::Search::searchEntity<ecs::Name>("First Child", icons, registry, parent.first);
	changed |= ui::Search::searchEntity<ecs::Name>("Last Child", icons, registry, parent.last);

	size_t stepSlow = 1, stepFast = 5;
	changed |=
		ImGui::InputScalar("Children Count", ImGuiDataType_U64, &parent.childrenCount, &stepSlow, &stepFast, "%d");

	return changed;
}

bool ComponentInputImpl<ecs::Child, const filament::Texture*>::draw(ecs::Registry& registry, ecs::Entity entity,
																	const filament::Texture* icons)
{
	auto& child = registry.getComponent<ecs::Child>(entity);
	bool changed = false;

	changed |= ui::Search::searchEntity<ecs::Name>("Parent", icons, registry, child.parent);
	changed |= ui::Search::searchEntity<ecs::Name>("Previous Sibling", icons, registry, child.previous);
	changed |= ui::Search::searchEntity<ecs::Name>("Next Sibling", icons, registry, child.next);

	return changed;
}

bool ComponentInputImpl<ecs::AttachmentTexture>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& attachment = registry.getComponent<ecs::AttachmentTexture>(entity);
	bool changed = false;

	changed |= ui::vec2Input("Size", attachment.size);
	changed |= ui::Combo::fromEnum("Type", attachment.type);

	auto aspectRatio = BaseAspectRatio<float>{attachment.size};

	ui::spacing(2);

	const auto* texture = registry.tryGetComponent<graphics::SharedTexture>(entity);
	if (texture != nullptr && attachment.type == ecs::AttachmentTexture::Type::Color)
	{
		auto width = ImGui::GetContentRegionAvail().x * 0.8f;
		ui::image(texture->get(), {width, width * aspectRatio.inv()}, math::vec4{0, 1, 1, 0});
	}
	else
	{
		ImGui::Text("This \"%s\" attachment cannot be previewed", magic_enum::enum_name(attachment.type).data());
	}

	return changed;
}

} // namespace spatial::ui