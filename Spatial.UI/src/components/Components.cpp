#include <array>
#include <imgui.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/SceneView.h>
#include <spatial/render/TextureView.h>
#include <spatial/ui/components/Collapse.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DirectionInput.h>
#include <spatial/ui/components/SceneView.h>
#include <spatial/ui/components/Search.h>
#include <spatial/ui/components/VectorInput.h>
#include "spatial/ui/components/DragAndDrop.h"

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
		if (result) {
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
	ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);
	ImGui::InputFloat("Falloff", &light.falloff);
}

void ComponentInputImpl<ecs::SpotLight>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::SpotLight>(entity);

	ImGui::Checkbox("Cast Shadows", &light.castShadows);
	ImGui::ColorEdit3("Color", &light.color.r);
	ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);
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
	ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);
	ImGui::InputFloat("Halo Falloff", &light.haloFalloff);
	ImGui::InputFloat("Halo size", &light.haloSize);
}

void ComponentInputImpl<ecs::IndirectLight>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::IndirectLight>(entity);

	ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);
	ui::inputPath("Reflections Texture", light.reflectionsTexturePath.relativePath);
	ui::inputPath("Irradiance Values", light.irradianceValuesPath.relativePath);
}


void ComponentInputImpl<ecs::Mesh>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& mesh = registry.getComponent<ecs::Mesh>(entity);

	ui::inputPath("Resource", mesh.resource.relativePath, "*.filamesh");

	if (ImGui::Button("Reload Mesh")) {
		registry.removeComponent<ecs::tags::IsMeshLoaded>(entity);
	}

	ImGui::SameLine();

	auto loaded = registry.hasAllComponents<ecs::tags::IsMeshLoaded>(entity);
	ImGui::Checkbox("Is Loaded", &loaded);
}

void ComponentInputImpl<ecs::DynamicMesh>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& mesh = registry.getComponent<ecs::DynamicMesh>(entity);

	if (ImGui::BeginTable("Dynamic Mesh Table", 2, gTableFlags)) {

		ImGui::TableSetupColumn("Info");
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Vertex Buffer");
		ImGui::TableNextColumn();
		ImGui::Text("%p", reinterpret_cast<void*>(mesh.vertexBuffer.get()));

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Index Buffer");
		ImGui::TableNextColumn();
		ImGui::Text("%p", reinterpret_cast<void*>(mesh.indexBuffer.get()));

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Geometries Count");
		ImGui::TableNextColumn();
		ImGui::Text("%lu", mesh.geometries.size());

		ImGui::EndTable();
	}
}

void ComponentInputImpl<ecs::MeshInstance>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& mesh = registry.getComponent<ecs::MeshInstance>(entity);

	bool changed = false;

	changed |= Search::searchEntity<ecs::tags::IsMesh>("Resource", registry, mesh.meshSource);
	{
		auto dnd = ui::DragAndDropTarget{};
		if (dnd.isStarted()) {
			auto result = dnd.getPayload<std::filesystem::path>();
			if (result.has_value()) {
				mesh.meshSource = ecs::Mesh::findOrCreate(registry, result.value());
				changed = true;
			}
		}
	}

	spacing(3);

	ImGui::Checkbox("Cast Shadows", &mesh.castShadows);
	ImGui::Checkbox("Receive Shadows", &mesh.receiveShadows);

	spacing(3);

	const size_t smallStep = 1, largeStep = 5;

	spacing(3);

	if (ImGui::TreeNodeEx("Geometry Configuration", ImGuiTreeNodeFlags_SpanFullWidth))
	{
		spacing(3);

		changed |=
			ImGui::InputScalar("Parts Count", ImGuiDataType_U64, &mesh.slice.count, &smallStep, &largeStep, "%lu");
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
					changed |= ImGui::InputScalar("##PrimitiveIndex", ImGuiDataType_U64, &meshMaterial.primitiveIndex,
												  &smallStep, &largeStep, "%lu");

					ImGui::TableNextColumn();

					ui::spanToAvailWidth();
					ui::Search::searchEntity<ecs::tags::IsMaterial>("##Material", registry,
																	meshMaterial.materialEntity);

					ImGui::TableNextColumn();

					ui::spanToAvailWidth();
					if (ImGui::Button("Remove"))
						childToDestroy = child;

					ImGui::PopID();
				});

				if (registry.isValid(childToDestroy))
				{
					ecs::Child::remove(registry, childToDestroy);
					registry.destroy(childToDestroy);
					mesh.slice.count = std::max(0ul, mesh.slice.count - 1);
					changed = true;
				}
			}

			ImGui::EndTable();
		}

		spacing(3);

		ImGui::TreePop();
	}

	if (changed)
		registry.addComponent<ecs::tags::IsMeshDirty>(entity);
}

void ComponentInputImpl<ecs::MeshMaterial>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& meshMaterial = registry.getComponent<ecs::MeshMaterial>(entity);
	const size_t smallStep = 1, largeStep = 1;

	ImGui::InputScalar("Primitive Index", ImGuiDataType_U64, &meshMaterial.primitiveIndex, &smallStep, &largeStep,
					   "%lu");
	ui::Search::searchEntity<ecs::tags::IsMaterial>("Material", registry, meshMaterial.materialEntity);
}

void ComponentInputImpl<ecs::Transform>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& transform = registry.getComponent<ecs::Transform>(entity);

	vec3Input("Position", transform.position);

	auto eulerAnglesRotation = transform.getEulerAnglesRotation();
	vec3Input("Rotation", eulerAnglesRotation);
	transform.rotation = math::deg2rad * eulerAnglesRotation;

	vec3Input("Scale", transform.scale);
}

void ComponentInputImpl<ecs::PerspectiveCamera>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::PerspectiveCamera>(entity);

	ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	double min = 15.0, max = 120.0;
	ImGui::DragScalar("Field Of View", ImGuiDataType_Double, &camera.fieldOfView, 1.0f, &min, &max, "%.1f");
	ImGui::InputDouble("Aspect Ratio", &camera.aspectRatio);
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

void ComponentInputImpl<ecs::SceneView>::draw(ecs::Registry& registry, ecs::Entity entity)
{
	auto& sceneView = registry.getComponent<ecs::SceneView>(entity);

	ImGui::DragInt2("Size", &sceneView.size.x);

	Search::searchEntity<ecs::IndirectLight>("Indirect Light", registry, sceneView.indirectLight);

	Search::searchEntity<ecs::tags::IsCamera>("Camera", registry, sceneView.camera);
	if (registry.hasAnyComponent<ecs::tags::IsCamera>(sceneView.camera)
		&& ImGui::TreeNodeEx("Camera Properties", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		spacing(3);
		if (registry.hasAllComponents<ecs::PerspectiveCamera>(sceneView.camera))
			ui::componentInput<ecs::PerspectiveCamera>(registry, sceneView.camera);
		if (registry.hasAllComponents<ecs::OrthographicCamera>(sceneView.camera))
			ui::componentInput<ecs::OrthographicCamera>(registry, sceneView.camera);
		if (registry.hasAllComponents<ecs::CustomCamera>(sceneView.camera))
			ui::componentInput<ecs::CustomCamera>(registry, sceneView.camera);
		spacing(3);
		ImGui::TreePop();
	}

	if (registry.hasAllComponents<render::TextureView>(entity)
		&& ImGui::TreeNodeEx("Camera Preview", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		SceneView::image(registry, entity, {ImGui::GetWindowContentRegionWidth(), 100});
		ImGui::TreePop();
	}
}

} // namespace spatial::ui