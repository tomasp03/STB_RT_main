#pragma once
#include <ImGui/imgui.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "Material.h"
#include "Random.h"
#include "HitInfo.h"
#include "Ray.h"
#include "Camera.h"
#include "Sphere.h"

#include <vector>
#include <string>
#include <math.h>
#include <iostream>

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
	int MaxBounces = 30;
	Skybox skybox;
	camera* m_cam;
	glm::ivec2 dimesnisons;

public:
	Renderer(int width, int height, camera* cam);

	HitInfo CalculateRayCollision(Ray ray);
	HitInfo RaySphere(Ray ray, Sphere sphere);
	glm::vec4 Trace(Ray& ray);
	glm::vec4 BackgroundColor(Ray ray);
	void ImGuiRender(bool& accumulation, uint32_t* image, camera& cam);
	void Render();
	glm::vec4 Skybox(Ray ray);
};

