#include <imgui.h>
#include <spatial/ecs/Material.h>
#include <spatial/ecs/Relation.h>
#include <spatial/render/TextureView.h>
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

bool inputPath(const std::string_view label, std::filesystem::path& path, std::string_view placeholder)
{
	auto value = path.string();
	if (ui::inputText(label, value, placeholder))
	{
		path = std::filesystem::path{value};
		return true;
	}
	{
		auto dnd = ui::DragAndDropTarget{};
		auto result = dnd.getPayload<std::filesystem::path>();
		if (result)
		{
			path = std::filesystem::path{result.value()};
			return true;
		}
	}

	return false;
}

void image(const filament::Texture& texture, math::float2 size, math::float4 uv)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
	ImGui::Image((ImTextureID)&texture, ImVec2(size.x, size.y), ImVec2(uv.x, uv.y), ImVec2(uv.z, uv.w));
#pragma clang diagnostic pop
}

bool imageButton(const filament::Texture& texture, math::float2 size, math::float4 uv)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
	return ImGui::ImageButton((ImTextureID)&texture, ImVec2(size.x, size.y), ImVec2(uv.x, uv.y), ImVec2(uv.z, uv.w));
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
	changed |= ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);
	changed |= ImGui::InputFloat("Falloff", &light.falloff);

	return changed;
}

bool ComponentInputImpl<ecs::SpotLight>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::SpotLight>(entity);
	bool changed = false;

	changed |= ImGui::Checkbox("Cast Shadows", &light.castShadows);
	changed |= ImGui::ColorEdit3("Color", &light.color.r);
	changed |= ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);
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
	changed |= ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);
	changed |= ImGui::InputFloat("Halo Falloff", &light.haloFalloff);
	changed |= ImGui::InputFloat("Halo size", &light.haloSize);

	return changed;
}

bool ComponentInputImpl<ecs::IndirectLight>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::IndirectLight>(entity);
	bool changed = false;

	changed |= ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);
	changed |= ui::inputPath("Reflections Texture", light.reflectionsTexturePath.relativePath);
	changed |= ui::inputPath("Irradiance Values", light.irradianceValuesPath.relativePath);

	return changed;
}

bool ComponentInputImpl<ecs::Script>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& script = registry.getComponent<ecs::Script>(entity);
	bool changed = false;

	changed |= ui::inputPath("Resource", script.resource.relativePath, "*.js");
	if (ImGui::Button("Reload Script"))
	{
		registry.removeComponentIfExists<ecs::ScriptInfo>(entity);
		registry.removeComponentIfExists<ecs::ScriptError>(entity);
		changed = true;
	}

	ImGui::SameLine();

	const auto* info = registry.tryGetComponent<const ecs::ScriptInfo>(entity);
	const auto* error = registry.tryGetComponent<const ecs::ScriptError>(entity);

	auto loaded = info != nullptr;
	ImGui::Checkbox("Is Loaded", &loaded);

	if (error)
	{
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.81f, 0.27f, 0.33f, 1.0), "Error: %s", error->message.c_str());
	}

	if (info)
	{
		ui::spacing(3);
		if (ImGui::BeginTable("ScriptPropertiesTable", 2, gTableFlags))
		{

			ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Type");
			ImGui::TableHeadersRow();

			for (const auto& prop : info->properties)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("%s", prop.name.c_str());

				ImGui::TableNextColumn();
				ImGui::Text("%s", prop.getTypeName());
			}

			ImGui::EndTable();
		}
		ui::spacing(3);
	}

	return changed;
}

bool ComponentInputImpl<ecs::Mesh>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& mesh = registry.getComponent<ecs::Mesh>(entity);
	bool changed = false;

	changed |= ui::inputPath("Resource", mesh.resource.relativePath, "*.filamesh");

	auto loaded = registry.hasAllComponents<ecs::tags::IsMeshLoaded>(entity);

	if (loaded && ImGui::Button("Reload Mesh"))
		registry.removeComponent<ecs::tags::IsMeshLoaded>(entity);

	ImGui::SameLine();

	ImGui::Checkbox("Is Loaded", &loaded);

	return changed;
}

bool ComponentInputImpl<ecs::MeshInstance>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& mesh = registry.getComponent<ecs::MeshInstance>(entity);
	bool changed = false;
	bool shouldRecreateMesh = false;

	changed |= Search::searchEntity<ecs::tags::IsMesh>("Resource", registry, mesh.meshSource);
	{
		auto dnd = ui::DragAndDropTarget{};
		auto result = dnd.getPayload<std::filesystem::path>();
		if (result.has_value())
		{
			mesh.meshSource = ecs::Mesh::findOrCreate(registry, result.value());
			shouldRecreateMesh = true;
			changed = true;
		}
	}

	spacing(3);

	changed |= ImGui::Checkbox("Cast Shadows", &mesh.castShadows);
	changed |= ImGui::Checkbox("Receive Shadows", &mesh.receiveShadows);

	spacing(3);

	const size_t smallStep = 1, largeStep = 5;

	spacing(3);

	if (ImGui::TreeNodeEx("Geometry Configuration", ImGuiTreeNodeFlags_SpanFullWidth))
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

	if (ImGui::TreeNodeEx("Materials", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		spacing(3);

		if (ImGui::Button("Add Slot"))
		{
			ecs::MeshInstance::addMaterial(registry, entity, ecs::NullEntity);
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
					ui::Search::searchEntity<ecs::tags::IsMaterialInstance>("##Material", registry,
																			meshMaterial.materialInstanceEntity);

					ImGui::TableNextColumn();

					ui::spanToAvailWidth();

					if (mesh.slice.count == 1) {
						ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
					}

					if (ImGui::Button("Remove") && mesh.slice.count > 1)
						childToDestroy = child;

					if (mesh.slice.count == 1) {
						ImGui::PopStyleVar();
					}

					ImGui::PopID();
				});

				if (registry.isValid(childToDestroy))
				{
					registry.addComponent<ecs::tags::CanDestroy>(childToDestroy);
					mesh.slice.count = std::max(0ul, mesh.slice.count - 1);
					shouldRecreateMesh = true;
					changed = true;
				}
			}

			ImGui::EndTable();
		}

		spacing(3);

		ImGui::TreePop();
	}

	if (shouldRecreateMesh)
	{
		registry.addComponent<ecs::tags::ShouldRecreateRenderable>(entity);
		changed = true;
	}

	return changed;
}

bool ComponentInputImpl<ecs::MeshMaterial>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& meshMaterial = registry.getComponent<ecs::MeshMaterial>(entity);
	const size_t smallStep = 1, largeStep = 1;
	bool changed = false;

	changed |= ImGui::InputScalar("Primitive Index", ImGuiDataType_U64, &meshMaterial.primitiveIndex, &smallStep,
								  &largeStep, "%lu");
	changed |= ui::Search::searchEntity<ecs::tags::IsMaterialInstance>("Material", registry,
																	   meshMaterial.materialInstanceEntity);

	return changed;
}

bool ComponentInputImpl<ecs::PrecompiledMaterial>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& material = registry.getComponent<ecs::PrecompiledMaterial>(entity);
	bool changed = false;

	changed |= ui::inputPath("Resource", material.resource.relativePath, "*.filamat");
	auto loaded = registry.hasAllComponents<ecs::tags::IsMaterialLoaded>(entity);

	if (loaded && ImGui::Button("Reload Material"))
		registry.removeComponent<ecs::tags::IsMaterialLoaded>(entity);

	changed |= ImGui::Checkbox("Is Loaded", &loaded);

	return changed;
}

bool ComponentInputImpl<ecs::Transform>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& transform = registry.getComponent<ecs::Transform>(entity);
	bool changed = false;

	changed |= vec3Input("Position", transform.position);

	auto eulerAnglesRotation = transform.getEulerAnglesRotation();
	changed |= vec3Input("Rotation", eulerAnglesRotation);
	transform.rotation = math::deg2rad * eulerAnglesRotation;

	changed |= vec3Input("Scale", transform.scale);

	return changed;
}

bool ComponentInputImpl<ecs::PerspectiveCamera>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::PerspectiveCamera>(entity);
	bool changed = false;

	changed |= ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	changed |= ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	double min = 15.0, max = 120.0;
	changed |= ImGui::DragScalar("Field Of View", ImGuiDataType_Double, &camera.fieldOfView, 1.0f, &min, &max, "%.1f");
	changed |= ImGui::InputDouble("Aspect Ratio", &camera.aspectRatio);

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

bool ComponentInputImpl<ecs::SceneView>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& sceneView = registry.getComponent<ecs::SceneView>(entity);
	auto changed = false;

	changed |= ImGui::DragInt2("Size", &sceneView.size.x);
	changed |= Search::searchEntity<ecs::IndirectLight>("Indirect Light", registry, sceneView.indirectLight);
	changed |= Search::searchEntity<ecs::tags::IsCamera>("Camera", registry, sceneView.camera);

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

	if (registry.hasAllComponents<render::TextureView>(entity)
		&& ImGui::TreeNodeEx("Camera Preview", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		SceneView::image(registry, entity, {ImGui::GetContentRegionAvail().x, 100});
		ImGui::TreePop();
	}

	return changed;
}

bool ComponentInputImpl<ecs::Parent>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& parent = registry.getComponent<ecs::Parent>(entity);
	bool changed = false;

	changed |= ui::Search::searchEntity<ecs::Name>("First Child", registry, parent.first);
	changed |= ui::Search::searchEntity<ecs::Name>("Last Child", registry, parent.last);

	size_t stepSlow = 1, stepFast = 5;
	changed |=
		ImGui::InputScalar("Children Count", ImGuiDataType_U64, &parent.childrenCount, &stepSlow, &stepFast, "%d");

	return changed;
}

bool ComponentInputImpl<ecs::Child>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& child = registry.getComponent<ecs::Child>(entity);
	bool changed = false;

	changed |= ui::Search::searchEntity<ecs::Name>("Parent", registry, child.parent);
	changed |= ui::Search::searchEntity<ecs::Name>("Previous Sibling", registry, child.previous);
	changed |= ui::Search::searchEntity<ecs::Name>("Next Sibling", registry, child.next);

	return changed;
}

} // namespace spatial::ui