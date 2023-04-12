#pragma once
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_glfw.h>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "Material.h"
#include "ShaderClass.h"
#include "Random.h"
#include "HitInfo.h"
#include "Ray.h"
#include "Camera.h"
#include "Shapes/AllShapes.h"
#include "Window.h"
#include "Loader.h"
#include "Image.h"


#include <vector>
#include <chrono>
#include <execution>
#include <string>
#include <math.h>
#include <iostream>
#include "Scene/Scene.h"
#include "OpenGL/OpenGL.h"

#define PI 3.1415926538f

struct Skybox
{
	float* image = nullptr;
	int x = 0;
	int y = 0;
	int chanels = 44;
};

class Renderer
{
public:
	struct Settings
	{
		bool Accumulate = false;
	};

	Renderer(int width, int height);
	Image* Render(Scene& scene, Camera& camera);

	Settings& GetSettings() { return m_Settings; }
	Image* GetImgPtr() { return &m_image; }
	Scene* GetScnPtr() { return m_ActiveScene; }
	Camera* GetCamPtr() { return m_ActiveCamera; }

private:
	Scene* m_ActiveScene = nullptr;
	Camera* m_ActiveCamera = nullptr;

	int MaxBounces = 30;
	Skybox skybox;

	glm::vec4* m_accumulationImage;
	Settings m_Settings;
	unsigned int frameIndex = 1;

	std::vector<int> ImageVerticalIter;

	Image m_image;

	Random random;

	double FPS;
	double diff;

	HitInfo CalculateRayCollision(Ray ray);
	glm::vec4 Trace(Ray& ray);
	glm::vec4 BackgroundColor(Ray ray);
	glm::vec4 Skybox(Ray ray);

};

