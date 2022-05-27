#include <spatial/ui/system/ImGuiHelpers.h>
#include <vector>

namespace fl = filament;

namespace spatial::ui
{

void imguiRefreshViewport(float width, float height, float scaleX, float scaleY)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(width, height);
	io.DisplayFramebufferScale.x = scaleX;
	io.DisplayFramebufferScale.y = scaleY;
}

void imguiRefreshDeltaTime(float delta)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = delta;
}

math::vec2 imguiGetFrameSize()
{
	ImGuiIO& io = ImGui::GetIO();
	auto fw = io.DisplaySize.x * io.DisplayFramebufferScale.x;
	auto fh = io.DisplaySize.y * io.DisplayFramebufferScale.y;

	return {fw, fh};
}

bool imguiIsMinimized()
{
	auto fs = imguiGetFrameSize();
	return fs.x == 0 && fs.y == 0;
}

graphics::VertexBuffer imguiCreateVertexBuffer(fl::Engine& engine, uint32_t capacity)
{
	return graphics::VertexBuffer{
		engine,
		fl::VertexBuffer::Builder()
			.vertexCount(capacity)
			.bufferCount(1)
			.attribute(fl::VertexAttribute::POSITION, 0, fl::VertexBuffer::AttributeType::FLOAT2, 0, sizeof(ImDrawVert))
			.attribute(fl::VertexAttribute::UV0, 0, fl::VertexBuffer::AttributeType::FLOAT2, sizeof(fl::math::float2),
					   sizeof(ImDrawVert))
			.attribute(fl::VertexAttribute::COLOR, 0, fl::VertexBuffer::AttributeType::UBYTE4,
					   2 * sizeof(fl::math::float2), sizeof(ImDrawVert))
			.normalized(fl::VertexAttribute::COLOR)
			.build(engine)};
}

graphics::IndexBuffer imguiCreateIndexBuffer(fl::Engine& engine, uint32_t capacity)
{
	return graphics::IndexBuffer{
		engine,
		fl::IndexBuffer::Builder().indexCount(capacity).bufferType(fl::IndexBuffer::IndexType::USHORT).build(engine)};
}

void imguiAddFont(const uint8_t* data, size_t size, float fontSize)
{
	auto& io = ImGui::GetIO();
	auto config = ImFontConfig{};
	config.FontDataOwnedByAtlas = false;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
	io.Fonts->AddFontFromMemoryTTF((void*)data, static_cast<int>(size), fontSize, &config);
#pragma clang diagnostic pop
}

graphics::Texture imguiCreateTextureAtlas(fl::Engine& engine)
{
	auto& io = ImGui::GetIO();

	unsigned char* imageData;
	int width, height, bpp;
	io.Fonts->GetTexDataAsRGBA32(&imageData, &width, &height, &bpp);

	auto textureSize = width * height * bpp;
	auto pb = fl::Texture::PixelBufferDescriptor{imageData, static_cast<size_t>(textureSize), fl::Texture::Format::RGBA,
												 fl::Texture::Type::UBYTE};
	const auto texture = fl::Texture::Builder()
							 .width(static_cast<uint32_t>(width))
							 .height(static_cast<uint32_t>(height))
							 .levels(static_cast<uint8_t>(1))
							 .format(fl::Texture::InternalFormat::RGBA8)
							 .sampler(fl::Texture::Sampler::SAMPLER_2D)
							 .build(engine);

	texture->setImage(engine, 0, std::move(pb));

	return graphics::createResource(engine, texture);
}

void imguiSetupInput()
{
	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	io.KeyMap[ImGuiKey_Tab] = static_cast<int>(Key::Tab);
	io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(Key::Left);
	io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(Key::Right);
	io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(Key::Up);
	io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(Key::Down);
	io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(Key::PageUp);
	io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(Key::PageDown);
	io.KeyMap[ImGuiKey_Home] = static_cast<int>(Key::Home);
	io.KeyMap[ImGuiKey_End] = static_cast<int>(Key::End);
	io.KeyMap[ImGuiKey_Insert] = static_cast<int>(Key::Insert);
	io.KeyMap[ImGuiKey_Delete] = static_cast<int>(Key::Delete);
	io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(Key::Backspace);
	io.KeyMap[ImGuiKey_Space] = static_cast<int>(Key::Space);
	io.KeyMap[ImGuiKey_Enter] = static_cast<int>(Key::Enter);
	io.KeyMap[ImGuiKey_Escape] = static_cast<int>(Key::Escape);
	io.KeyMap[ImGuiKey_A] = static_cast<int>(Key::A);
	io.KeyMap[ImGuiKey_C] = static_cast<int>(Key::C);
	io.KeyMap[ImGuiKey_V] = static_cast<int>(Key::V);
	io.KeyMap[ImGuiKey_X] = static_cast<int>(Key::X);
	io.KeyMap[ImGuiKey_Y] = static_cast<int>(Key::Y);
	io.KeyMap[ImGuiKey_Z] = static_cast<int>(Key::Z);
}

void imguiSetMousePosition(filament::math::float2 position)
{
	auto& io = ImGui::GetIO();
	io.MousePos = ImVec2(position.x, position.y);
}

void imguiSetKey(Key key, KeyAction action)
{
	if (key == Key::UnknownKey)
		return;

	auto pressed = action == KeyAction::Pressed;
	auto& io = ImGui::GetIO();
	io.MouseDown[0] = key == Key::MouseLeft && pressed;
	io.MouseDown[1] = key == Key::MouseRight && pressed;
	io.MouseDown[2] = key == Key::MouseMiddle && pressed;

	io.KeysDown[static_cast<int>(key)] = pressed;
	io.KeyCtrl = (key == Key::LControl || key == Key::RControl) && pressed;
	io.KeyShift = (key == Key::LShift || key == Key::RShift) && pressed;
	io.KeyAlt = (key == Key::LAlt || key == Key::RAlt) && pressed;
	io.KeySuper = (key == Key::System) && pressed;
}

void imguiSetText(std::string_view text)
{
	auto& io = ImGui::GetIO();
	io.AddInputCharactersUTF8(text.data());
}

void imguiSetScrollOffset(double xOffset, double yOffset)
{
	auto& io = ImGui::GetIO();
	io.MouseWheel = static_cast<float>(yOffset);
	io.MouseWheelH = static_cast<float>(xOffset);
}

void imguiSetupTheme()
{
	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_DpiEnableScaleFonts | ImGuiConfigFlags_IsSRGB;

	auto& style = ImGui::GetStyle();
	ImGui::StyleColorsDark(&style);
	auto& colors = style.Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
	colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
	colors[ImGuiCol_Separator] = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.6f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

	style.PopupRounding = 3;

	style.WindowPadding = ImVec2(4, 4);
	style.FramePadding = ImVec2(6, 4);
	style.ItemSpacing = ImVec2(6, 2);

	style.ScrollbarSize = 18;

	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 1;

	style.WindowRounding = 3;
	style.ChildRounding = 3;
	style.FrameRounding = 3;
	style.ScrollbarRounding = 2;
	style.GrabRounding = 3;
	style.TabBorderSize = 1;
	style.TabRounding = 3;

	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);
}

} // namespace spatial::ui