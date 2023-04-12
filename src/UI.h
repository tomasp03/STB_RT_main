#pragma once

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_glfw.h>

#include <stb/stb_image_write.h>

#include "Scene/Scene.h"
#include "Camera.h"
#include "Scene/Image.h"
#include "Renderer.h"

#include "Loader.h"
#include "OpenGL/OpenGL.h"
#include "OpenGL/Window.h"

class UI
{
private:
	Window* m_window = nullptr;
	Renderer* m_ActiveRenderer = nullptr;

	float m_sceneFPS;
	float m_sceneDiff;

	void ImGuiRender();
	void UpdateImGui();

public:
	UI();
	void Update(Renderer& renderer, float FPS, float diff);
	bool IsRunning();
};

