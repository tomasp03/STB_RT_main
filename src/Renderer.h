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
#include "Sphere.h"
#include "Window.h"
#include "Loader.h"
#include "Image.h"


#include <vector>
#include <chrono>
#include <execution>
#include <string>
#include <math.h>
#include <iostream>
#include "Scene.h"
#include "OpenGL.h"
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
private:
	std::vector<Sphere> m_Spheres;
	std::vector<Plane> m_planes;


	int MaxBounces = 30;
	Skybox skybox;
	camera* m_cam;

	Image m_image;
	glm::vec4* m_accumulationImage;

	Window* m_window;
	Shader* m_shader;

	std::vector<int> ImageVerticalIter;
	bool accumulate = true;
	unsigned int frameIndex = 1;

	GLuint screenTex;
	GLuint VAO, VBO, EBO;

	Random random;

	Scene scene1;

public:
	Renderer(int width, int height);

	HitInfo CalculateRayCollision(Ray ray);
	HitInfo RaySphere(Ray ray, Sphere sphere);
	HitInfo RayPlane(Ray ray, Plane plane);
	glm::vec4 Trace(Ray& ray);
	glm::vec4 BackgroundColor(Ray ray);
	void ImGuiRender();
	bool Render();
	void ShutWindow();
	glm::vec4 Skybox(Ray ray);
};

