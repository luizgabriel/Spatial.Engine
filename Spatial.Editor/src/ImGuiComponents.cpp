#include "ImGuiComponents.h"
#include "DefaultMaterial.h"
#include "EditorCamera.h"
#include <array>
#include <imgui_internal.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/ecs/Tags.h>
#include <set>

namespace spatial::editor
{

bool buttonInput(const std::string_view label, float& value, float resetValue, float speed, float min, float max,
				 const std::string_view format)
{
	bool changed = false;
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.data());

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});
	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

	ImGui::PushFont(boldFont);
	if (ImGui::Button(label.data(), buttonSize))
	{
		changed = true;
		value = resetValue;
	}
	ImGui::PopFont();

	ImGui::SameLine();
	changed |= ImGui::DragFloat("##ValueDrag", &value, speed, min, max, format.data());

	ImGui::PopStyleVar();

	ImGui::PopID();

	return changed;
}

bool vec2Input(const std::string_view label, math::float2& v, float resetValue, float speed, float min, float max,
			   const std::string_view format)
{
	bool changed = false;

	ImGui::PushID(label.data());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 70.0f);
	ImGui::Text("%s", label.data());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	changed |= buttonInput("X", v.x, resetValue, speed, min, max, format);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	changed |= buttonInput("Y", v.y, resetValue, speed, min, max, format);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);

	ImGui::Columns(1);

	ImGui::PopID();

	return changed;
}

bool vec3Input(const std::string_view label, math::float3& v, float resetValue, float speed, float min, float max,
			   const std::string_view format)
{
	bool changed = false;

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 70.0f);
	ImGui::Text("%s", label.data());
	ImGui::NextColumn();

	ImGui::PushID(label.data());
	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	changed |= buttonInput("X", v.x, resetValue, speed, min, max, format);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	changed |= buttonInput("Y", v.y, resetValue, speed, min, max, format);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.2f, 0.7f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.3f, 0.8f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.2f, 0.7f, 1.0f});
	changed |= buttonInput("Z", v.z, resetValue, speed, min, max, format);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);

	ImGui::Columns(1);

	ImGui::PopID();

	return changed;
}

bool vec4Input(const std::string_view label, math::float4& v, float resetValue, float speed, float min, float max,
			   const std::string_view format)
{
	bool changed = false;

	ImGui::PushID(label.data());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 70.0f);
	ImGui::Text("%s", label.data());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	changed |= buttonInput("X", v.x, resetValue, speed, min, max, format);
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	changed |= buttonInput("Y", v.y, resetValue, speed, min, max, format);
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.2f, 0.7f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.3f, 0.8f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.2f, 0.7f, 1.0f});
	changed |= buttonInput("Z", v.z, resetValue, speed, min, max, format);
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.7f, 0.7f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.8f, 0.8f, 0.3f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.7f, 0.7f, 0.2f, 1.0f});
	changed |= buttonInput("W", v.w, resetValue, speed, min, max, format);
	ImGui::PopStyleColor(3);

	ImGui::Columns(1);

	ImGui::PopID();

	return changed;
}

void entitiesTreeView(ecs::Registry& stage, ecs::Entity& selectedInstance)
{
	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		selectedInstance = {};

	const auto view = stage.getEntities<const ecs::EntityName>(ecs::ExcludeComponents<ecs::tags::IsMeshMaterial>);
	for (auto entity : view)
	{
		const auto& name = stage.getComponent<const ecs::EntityName>(entity);

		ImGuiTreeNodeFlags flags = (selectedInstance == entity) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
		flags |= ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx("##", flags, "%s", name.name.c_str());
		if (ImGui::IsItemClicked())
			selectedInstance = entity;

		if (opened)
			ImGui::TreePop();
	}
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

bool inputText(const std::string_view label, std::string& value)
{
	return ImGui::InputText(label.data(), value.data(), value.size() + 1, ImGuiInputTextFlags_CallbackResize,
							&inputTextCallback, &value);
}

float quatD(float w, float h)
{
	return std::min(std::abs(w), std::abs(h)) - 4.0f;
}

float quatPX(float x, float w, float h)
{
	return (x * 0.5f * quatD(w, h) + w * 0.5f + 0.5f);
}

float quatPY(float y, float w, float h)
{
	return (-y * 0.5f * quatD(w, h) + h * 0.5f - 0.5f);
}

float quatIX(float x, float w, float h)
{
	return (2.0f * x - w - 1.0f) / quatD(w, h);
}

float quatIY(float y, float w, float h)
{
	return (-2.0f * y + h - 1.0f) / quatD(w, h);
}

static ImVector<math::float3> s_ArrowTri[4];
static ImVector<ImVec2> s_ArrowTriProj[4];
static ImVector<math::float3> s_ArrowNorm[4];
static ImVector<ImU32> s_ArrowColLight[4];

void initArrowData()
{
	constexpr int SUBDIV = 15;
	constexpr float CYL_RADIUS = 0.08f;
	constexpr float CONE_RADIUS = 0.16f;
	constexpr float CONE_LENGTH = 0.25f;
	constexpr float ARROW_BGN = -1.1f;
	constexpr float ARROW_END = 1.15f;
	constexpr int ARROW_CONE = 0;
	constexpr int ARROW_CONE_CAP = 1;
	constexpr int ARROW_CYL = 2;
	constexpr int ARROW_CAP = 3;

	for (int i = 0; i < 4; ++i)
	{
		s_ArrowTri[i].clear();
		s_ArrowNorm[i].clear();
	}

	float x0, x1, y0, y1, z0, z1, a0, a1, nx, nn;
	for (int i = 0; i < SUBDIV; ++i)
	{
		a0 = 2.0f * math::pi * (float(i)) / SUBDIV;
		a1 = 2.0f * math::pi * (float(i + 1)) / SUBDIV;
		x0 = ARROW_BGN;
		x1 = ARROW_END - CONE_LENGTH;
		y0 = cosf(a0);
		z0 = sinf(a0);
		y1 = cosf(a1);
		z1 = sinf(a1);
		s_ArrowTri[ARROW_CYL].push_back(math::float3(x1, CYL_RADIUS * y0, CYL_RADIUS * z0));
		s_ArrowTri[ARROW_CYL].push_back(math::float3(x0, CYL_RADIUS * y0, CYL_RADIUS * z0));
		s_ArrowTri[ARROW_CYL].push_back(math::float3(x0, CYL_RADIUS * y1, CYL_RADIUS * z1));
		s_ArrowTri[ARROW_CYL].push_back(math::float3(x1, CYL_RADIUS * y0, CYL_RADIUS * z0));
		s_ArrowTri[ARROW_CYL].push_back(math::float3(x0, CYL_RADIUS * y1, CYL_RADIUS * z1));
		s_ArrowTri[ARROW_CYL].push_back(math::float3(x1, CYL_RADIUS * y1, CYL_RADIUS * z1));
		s_ArrowNorm[ARROW_CYL].push_back(math::float3(0, y0, z0));
		s_ArrowNorm[ARROW_CYL].push_back(math::float3(0, y0, z0));
		s_ArrowNorm[ARROW_CYL].push_back(math::float3(0, y1, z1));
		s_ArrowNorm[ARROW_CYL].push_back(math::float3(0, y0, z0));
		s_ArrowNorm[ARROW_CYL].push_back(math::float3(0, y1, z1));
		s_ArrowNorm[ARROW_CYL].push_back(math::float3(0, y1, z1));
		s_ArrowTri[ARROW_CAP].push_back(math::float3(x0, 0, 0));
		s_ArrowTri[ARROW_CAP].push_back(math::float3(x0, CYL_RADIUS * y1, CYL_RADIUS * z1));
		s_ArrowTri[ARROW_CAP].push_back(math::float3(x0, CYL_RADIUS * y0, CYL_RADIUS * z0));
		s_ArrowNorm[ARROW_CAP].push_back(math::float3(-1, 0, 0));
		s_ArrowNorm[ARROW_CAP].push_back(math::float3(-1, 0, 0));
		s_ArrowNorm[ARROW_CAP].push_back(math::float3(-1, 0, 0));
		x0 = ARROW_END - CONE_LENGTH;
		x1 = ARROW_END;
		nx = CONE_RADIUS / (x1 - x0);
		nn = 1.0f / sqrtf(nx * nx + 1);
		s_ArrowTri[ARROW_CONE].push_back(math::float3(x1, 0, 0));
		s_ArrowTri[ARROW_CONE].push_back(math::float3(x0, CONE_RADIUS * y0, CONE_RADIUS * z0));
		s_ArrowTri[ARROW_CONE].push_back(math::float3(x0, CONE_RADIUS * y1, CONE_RADIUS * z1));
		s_ArrowTri[ARROW_CONE].push_back(math::float3(x1, 0, 0));
		s_ArrowTri[ARROW_CONE].push_back(math::float3(x0, CONE_RADIUS * y1, CONE_RADIUS * z1));
		s_ArrowTri[ARROW_CONE].push_back(math::float3(x1, 0, 0));
		s_ArrowNorm[ARROW_CONE].push_back(math::float3(nn * nx, nn * y0, nn * z0));
		s_ArrowNorm[ARROW_CONE].push_back(math::float3(nn * nx, nn * y0, nn * z0));
		s_ArrowNorm[ARROW_CONE].push_back(math::float3(nn * nx, nn * y1, nn * z1));
		s_ArrowNorm[ARROW_CONE].push_back(math::float3(nn * nx, nn * y0, nn * z0));
		s_ArrowNorm[ARROW_CONE].push_back(math::float3(nn * nx, nn * y1, nn * z1));
		s_ArrowNorm[ARROW_CONE].push_back(math::float3(nn * nx, nn * y1, nn * z1));
		s_ArrowTri[ARROW_CONE_CAP].push_back(math::float3(x0, 0, 0));
		s_ArrowTri[ARROW_CONE_CAP].push_back(math::float3(x0, CONE_RADIUS * y1, CONE_RADIUS * z1));
		s_ArrowTri[ARROW_CONE_CAP].push_back(math::float3(x0, CONE_RADIUS * y0, CONE_RADIUS * z0));
		s_ArrowNorm[ARROW_CONE_CAP].push_back(math::float3(-1, 0, 0));
		s_ArrowNorm[ARROW_CONE_CAP].push_back(math::float3(-1, 0, 0));
		s_ArrowNorm[ARROW_CONE_CAP].push_back(math::float3(-1, 0, 0));
	}

	for (int i = 0; i < 4; ++i)
	{
		s_ArrowTriProj[i].clear();
		s_ArrowTriProj[i].resize(s_ArrowTri[i].size());
		s_ArrowColLight[i].clear();
		s_ArrowColLight[i].resize(s_ArrowTri[i].size());
	}
}

ImU32 blendColor(ImU32 c1, ImU32 c2, float t)
{
	ImColor color1(c1);
	ImColor color2(c2);
	float invt = 1.0f - t;
	color1 = ImColor((color1.Value.x * invt) + (color2.Value.x * t), (color1.Value.y * invt) + (color2.Value.y * t),
					 (color1.Value.z * invt) + (color2.Value.z * t), (color1.Value.w * invt) + (color2.Value.w * t));
	return color1;
}

inline float ImVec2Cross(const ImVec2& left, const ImVec2& right)
{
	return (left.x * right.y) - (left.y * right.x);
}

inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
{
	return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)
{
	return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

void drawTriangles(ImDrawList* draw_list, const ImVec2& offset, const ImVector<ImVec2>& triProj,
				   const ImVector<ImU32>& colLight, int numVertices)
{
	const ImVec2 uv = ImGui::GetFontTexUvWhitePixel();
	assert(numVertices % 3 == 0);
	draw_list->PrimReserve(numVertices, numVertices);
	for (int ii = 0; ii < numVertices / 3; ii++)
	{
		ImVec2 v1 = offset + triProj[ii * 3];
		ImVec2 v2 = offset + triProj[ii * 3 + 1];
		ImVec2 v3 = offset + triProj[ii * 3 + 2];

		// 2D cross product to do culling
		ImVec2 d1 = v2 - v1;
		ImVec2 d2 = v3 - v1;
		float c = ImVec2Cross(d1, d2);
		if (c > 0.0f)
		{
			v2 = v1;
			v3 = v1;
		}

		draw_list->PrimWriteIdx(ImDrawIdx(draw_list->_VtxCurrentIdx));
		draw_list->PrimWriteIdx(ImDrawIdx(draw_list->_VtxCurrentIdx + 1));
		draw_list->PrimWriteIdx(ImDrawIdx(draw_list->_VtxCurrentIdx + 2));
		draw_list->PrimWriteVtx(v1, uv, colLight[ii * 3]);
		draw_list->PrimWriteVtx(v2, uv, colLight[ii * 3 + 1]);
		draw_list->PrimWriteVtx(v3, uv, colLight[ii * 3 + 2]);
	}
}

bool drawArrowWidget(math::float3& direction, float widgetSize, std::uint32_t color)
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	auto directionQuaternion = math::directionToQuaternion(direction);

	if (s_ArrowTri[0].empty())
		initArrowData();

	bool value_changed = false;

	ImVec2 orient_pos = ImGui::GetCursorScreenPos();

	float sv_orient_size = std::min(ImGui::CalcItemWidth(), widgetSize);
	float w = sv_orient_size;
	float h = sv_orient_size;

	// We want to generate quaternion rotations relative to the quaternion in the down press state.
	// This gives us cleaner control over rotation (it feels better)
	static math::quatf origQuat;
	static math::float3 coordOld;
	bool highlighted = false;
	ImGui::InvisibleButton("widget", ImVec2(sv_orient_size, sv_orient_size));
	if (ImGui::IsItemActive())
	{
		highlighted = true;
		ImVec2 mouse = ImGui::GetMousePos() - orient_pos;
		if (ImGui::IsMouseClicked(0))
		{
			origQuat = directionQuaternion;
			coordOld = math::float3(quatIX(mouse.x, w, h), quatIY(mouse.y, w, h), 1.0f);
		}
		else if (ImGui::IsMouseDragging(0))
		{
			math::float3 coord(quatIX(mouse.x, w, h), quatIY(mouse.y, w, h), 1.0f);
			math::float3 pVec = coord;
			math::float3 oVec = coordOld;
			coord.z = 0.0f;
			float n0 = length(oVec);
			float n1 = length(pVec);
			if (n0 > math::epsilon && n1 > math::epsilon)
			{
				math::float3 v0 = oVec / n0;
				math::float3 v1 = pVec / n1;
				math::float3 axis = cross(v0, v1);
				float sa = length(axis);
				float ca = dot(v0, v1);
				float angle = atan2(sa, ca);
				if (coord.x * coord.x + coord.y * coord.y > 1.0f)
				{
					angle *= 1.0f + 1.5f * (length(coord) - 1.0f);
				}

				math::quatf qrot, qres, qorig;
				qrot = math::quatf::fromAxisAngle(axis, angle);
				float nqorig = sqrt(origQuat.x * origQuat.x + origQuat.y * origQuat.y + origQuat.z * origQuat.z
									+ origQuat.w * origQuat.w);
				if (abs(nqorig) > math::epsilon * math::epsilon)
				{
					qorig = origQuat / nqorig;
					qres = qrot * qorig;
					directionQuaternion = qres;
					direction = math::quaternionToDirection(directionQuaternion);
				}
				else
				{
					directionQuaternion = qrot;
					direction = math::quaternionToDirection(directionQuaternion);
				}
				value_changed = true;
			}
		}
		draw_list->AddRectFilled(orient_pos, orient_pos + ImVec2(sv_orient_size, sv_orient_size),
								 ImColor(style.Colors[ImGuiCol_FrameBgActive]), style.FrameRounding);
	}
	else
	{
		auto imcolor =
			ImColor{ImGui::IsItemHovered() ? style.Colors[ImGuiCol_FrameBgHovered] : style.Colors[ImGuiCol_FrameBg]};
		draw_list->AddRectFilled(orient_pos, orient_pos + ImVec2(sv_orient_size, sv_orient_size), imcolor,
								 style.FrameRounding);
	}

	ImVec2 inner_pos = orient_pos;
	math::quatf quat = normalize(directionQuaternion);
	ImColor alpha(1.0f, 1.0f, 1.0f, highlighted ? 1.0f : 0.75f);
	math::float3 arrowDir = quat * math::float3(1, 0, 0);

	for (int k = 0; k < 4; ++k)
	{
		int j = (arrowDir.z > 0) ? 3 - k : k;
		assert(s_ArrowTriProj[j].size() == (s_ArrowTri[j].size()) && s_ArrowColLight[j].size() == s_ArrowTri[j].size()
			   && s_ArrowNorm[j].size() == s_ArrowTri[j].size());

		auto ntri = s_ArrowTri[j].size();
		for (int i = 0; i < ntri; ++i)
		{
			math::float3 coord = s_ArrowTri[j][i];
			math::float3 norm = s_ArrowNorm[j][i];
			if (coord.x > 0)
			{
				coord.x = 2.5f * coord.x - 2.0f;
			}
			else
			{
				coord.x += 0.2f;
			}
			coord.y *= 1.5f;
			coord.z *= 1.5f;
			coord = quat * coord;
			norm = quat * norm;
			s_ArrowTriProj[j][i] = ImVec2(quatPX(coord.x, w, h), quatPY(coord.y, w, h));
			ImU32 col = (color | 0xff000000) & alpha;
			s_ArrowColLight[j][i] = blendColor(0xff000000, col, abs(std::clamp(norm.z, -1.0f, 1.0f)));
		}
		drawTriangles(draw_list, inner_pos, s_ArrowTriProj[j], s_ArrowColLight[j], ntri);
	}

	return value_changed;
}

bool directionInput(const std::string_view label, math::float3& dir, float size, std::uint32_t color)
{
	ImGui::PushID(label.data());
	ImGui::BeginGroup();

	// DragFloat3 is nicer than SliderFloat3 because it supports double-clicking for keyboard entry.
	bool changed = vec3Input(label.data(), dir, 1.0f, 0.01f, -1.0f, 1.0f);

	auto v = normalize(dir);
	if (drawArrowWidget(v, size, color) && !changed)
	{
		changed = true;
		dir = v;
	}
	ImGui::EndGroup();
	ImGui::PopID();

	return changed;
}

template <>
void componentInput<ecs::DirectionalLight>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::DirectionalLight>(entity);

	ImGui::Checkbox("Cast Shadows", &light.castShadows);
	ImGui::ColorEdit3("Color", &light.color.r);
	ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);

	directionInput("Direction", light.direction);
}

template <>
void componentInput<ecs::PointLight>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::PointLight>(entity);

	ImGui::ColorEdit3("Color", &light.color.r);
	ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);
	ImGui::InputFloat("Falloff", &light.falloff);
}

template <>
void componentInput<ecs::SpotLight>(ecs::Registry& registry, ecs::Entity entity)
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

template <>
void componentInput<ecs::SunLight>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& light = registry.getComponent<ecs::SunLight>(entity);

	ImGui::Checkbox("Cast Shadows", &light.castShadows);
	ImGui::ColorEdit3("Color", &light.color.r);
	ImGui::DragFloat("Intensity", &light.intensity, 1.0f, .0f, 100000.0f);
	ImGui::InputFloat("Halo Falloff", &light.haloFalloff);
	ImGui::InputFloat("Halo size", &light.haloSize);
}

template <>
void componentInput<ecs::Mesh>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& mesh = registry.getComponent<ecs::Mesh>(entity);

	inputText("Resource", mesh.resourceName);

	ImGui::Checkbox("Cast Shadows", &mesh.castShadows);
	ImGui::Checkbox("Receive Shadows", &mesh.receiveShadows);
	ImGui::Text("Parts Count: %lu", mesh.partsCount);
	ImGui::Text("Parts Offset: %lu", mesh.partsOffset);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Spacing();

	if (ImGui::BeginTable("Materials", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable)) {
		ImGui::TableSetupColumn("Geometry Index");
		ImGui::TableSetupColumn("Material");
		ImGui::TableHeadersRow();

		for (std::uint8_t i = 0; i < mesh.partsCount; i++)
		{
			ImGui::TableNextRow();

			auto& material = mesh.materials[i];

			ImGui::TableNextColumn();
			ImGui::Text("Index: %u", i);

			ImGui::TableNextColumn();

			ImGui::PushID(i);
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
			selectEntityInput<ecs::tags::IsMeshMaterial>("##Material", registry, material);
			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}

template <>
void componentInput<ecs::Transform>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& transform = registry.getComponent<ecs::Transform>(entity);

	vec3Input("Position", transform.position);
	vec3Input("Rotation", transform.rotation);
	vec3Input("Scale", transform.scale);
}

template <>
void componentInput<ecs::PerspectiveCamera>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::PerspectiveCamera>(entity);

	ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	double min = 15.0, max = 120.0;
	ImGui::DragScalar("Field Of View", ImGuiDataType_Double, &camera.fieldOfView, 1.0f, &min, &max, "%.1f");
	ImGui::InputDouble("Aspect Ratio", &camera.aspectRatio);
}

template <>
void componentInput<ecs::OrthographicCamera>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::OrthographicCamera>(entity);

	ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	ImGui::InputDouble("Left", &camera.left);
	ImGui::InputDouble("Right", &camera.right);
	ImGui::InputDouble("Bottom", &camera.bottom);
	ImGui::InputDouble("Top", &camera.top);
}

template <>
void componentInput<ecs::CustomCamera>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& camera = registry.getComponent<ecs::CustomCamera>(entity);

	ImGui::InputDouble("Near", &camera.near, 0.1, 1.0, "%.2f");
	ImGui::InputDouble("Far", &camera.far, 0.1, 1.0, "%.2f");

	ImGui::InputScalarN("m0", ImGuiDataType_Double, &camera.projectionMatrix[0], 4);
	ImGui::InputScalarN("m1", ImGuiDataType_Double, &camera.projectionMatrix[1], 4);
	ImGui::InputScalarN("m2", ImGuiDataType_Double, &camera.projectionMatrix[2], 4);
	ImGui::InputScalarN("m3", ImGuiDataType_Double, &camera.projectionMatrix[3], 4);
}

template <>
void componentInput<DefaultMaterial>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& material = registry.getComponent<DefaultMaterial>(entity);

	ImGui::ColorEdit3("Color", &material.baseColor.r);
	ImGui::DragFloat("Metallic", &material.metallic, 0.01f, .0f, 1.0f, "%.2f");
	ImGui::DragFloat("Reflectance", &material.reflectance, 0.01f, .0f, 1.0f, "%.2f");
	ImGui::DragFloat("Roughness", &material.roughness, 0.01f, .0f, 1.0f, "%.2f");
}

} // namespace spatial::editor