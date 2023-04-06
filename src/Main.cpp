#include <iostream>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_glfw.h>
#include <glm/vector_relational.hpp>
#include <stb/stb_image_write.h>
#include <execution>
#include "Renderer.h"
#include "Loader.h"
#include "Window.h"
#include "ShaderClass.h"
#include <string>

#define IMAGE_WIDTH 1600
#define IMAGE_HEIGHT 800
#define IMAGE_NAME "RayTracing.png"

#define MT 1

static uint32_t ConvertToRGBA(const glm::vec4& color)
{
	uint8_t r = (uint8_t)(color.r * 255.0f);
	uint8_t g = (uint8_t)(color.g * 255.0f);
	uint8_t b = (uint8_t)(color.b * 255.0f);
	uint8_t a = (uint8_t)(color.a * 255.0f);

	uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
	return result;
}

static glm::vec4 GammaCorection2(glm::vec4 color)
{
	return glm::vec4(glm::sqrt(color));
}

int main()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.WantCaptureMouse = true;
	ImGuiPlatformIO& p_io = ImGui::GetPlatformIO(); (void)p_io;
	ImGui::RenderPlatformWindowsDefault();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
	io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
	//io.ConfigViewportsNoAutoMerge = true;

	ImGuiWindowClass wclass;
	io.ConfigViewportsNoDefaultParent = true;
	wclass.ViewportFlagsOverrideSet = ImGuiWindowFlags_NoBringToFrontOnFocus;
	wclass.ViewportFlagsOverrideSet = ImGuiViewportFlags_CanHostOtherWindows;

	ImGui::StyleColorsDark();

	std::vector<int> ImageVerticalIter;
	bool accumulate = false;
	int frameIndex = 1;
	// Vertices coordinates
	GLfloat vertices[] =
	{ //     COORDINATES   /   TexCoord  //
		-1.0f, -1.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
		-1.0f,  1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
		 1.0f,  1.0f, 0.0f,	1.0f, 1.0f, // Upper right corner
		 1.0f, -1.0f, 0.0f,	1.0f, 0.0f  // Lower right corner
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 2, 1, // Upper triangle
		0, 3, 2 // Lower triangle
	};


	loader::GLFW();
	Window window = Window(IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_NAME);
	loader::GladGL();

	ImGui_ImplGlfw_InitForOpenGL(window.GetID(), true);
	ImGui_ImplOpenGL3_Init("#version 460");


	Shader shader = Shader("shaders\\basic.vert", "shaders\\basic.frag");
	// Create reference containers for the Vartex Array Object, the Vertex Buffer Object, and the Element Buffer Object
	GLuint VAO, VBO, EBO;

	// Generate the VAO, VBO, and EBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind the EBO specifying it's a GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Introduce the indices into the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GLuint screenTex;
	glGenTextures(1, &screenTex);
	glBindTexture(GL_TEXTURE_2D, screenTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glViewport(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);

	ImageVerticalIter.resize(IMAGE_HEIGHT);
	for (int y = 0; y < IMAGE_HEIGHT; y++)
	{
		ImageVerticalIter[y] = y;
	}

	uint32_t* image = new uint32_t[IMAGE_WIDTH * IMAGE_HEIGHT];
	glm::vec4* accumulationImage = new glm::vec4[IMAGE_WIDTH * IMAGE_HEIGHT];

	srand(NULL);
	glm::vec3 originChange = glm::vec3(0);
	float aspectRatio = IMAGE_WIDTH / IMAGE_HEIGHT;

	camera cam = camera(glm::vec3(0, 0, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 90.0f, aspectRatio);
	Renderer render = Renderer(IMAGE_WIDTH, IMAGE_HEIGHT, &cam);

#if MT

	while (!glfwWindowShouldClose(window.GetID()))
	{
		if (frameIndex == 1)
			memset(accumulationImage, 0, IMAGE_WIDTH * IMAGE_HEIGHT * sizeof(glm::vec4));

		auto start = std::chrono::steady_clock::now();

		std::for_each(std::execution::par, ImageVerticalIter.begin(), ImageVerticalIter.end(),
			[&](unsigned int y)
			{
				for (int x = 0; x < IMAGE_WIDTH; x++)
				{
					int index = (x + y * IMAGE_WIDTH);
					float offset_u = 0.53f * RandFloat() / (float)IMAGE_WIDTH;
					float offset_v = 0.53f * RandFloat() / (float)IMAGE_HEIGHT;


					float u = ((float)(x) / IMAGE_WIDTH); // transforms to [-1.0, 1.0]
					float v = ((float)(y) / IMAGE_HEIGHT);               // transforms to [-1.0, 1.0]


					Ray ray = cam.get_ray(u, v);

					accumulationImage[index] += render.Trace(ray);

					glm::vec4 accumulationColor = accumulationImage[index];
					accumulationColor /= (float)frameIndex;
					accumulationColor.a = 1.0f;
					image[index] = ConvertToRGBA(GammaCorection2(accumulationColor));
				}
			}
		);

		if (accumulate)
			frameIndex++;
		else
			frameIndex = 1;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_WIDTH, IMAGE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		
		shader.Activate();
		glBindTexture(GL_TEXTURE_2D, screenTex);
		shader.uni1f("tex0", 0);
		glBindVertexArray(VAO);

		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		render.ImGuiRender(accumulate, image, cam);

		ImGui::Render();
		ImGui::EndFrame();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		window.SwapBuffers();
		window.PollEvents();
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);


		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> result = end - start;
		std::string diff = std::to_string(result.count());
		glfwSetWindowTitle(window.GetID(), diff.c_str());
	}
	//glfwDestroyWindow(window.GetID());

#else
	for (int y = 0; y < IMAGE_HEIGHT; y++)
	{
		for (int x = 0; x < IMAGE_WIDTH; x++)
		{
			glm::vec4 color = glm::vec4(0);
			float aspectRatio = IMAGE_WIDTH / IMAGE_HEIGHT;
			Camera cam;
			cam.fov = 90.f;
			cam.origin = glm::vec3(0.0f, 0.0f, -glm::tan(cam.fov / 2.0f));
			for (int i = 0; i < sapmles; i++)
			{
				float offset_u = 5.0f * RandFloat() / (float)IMAGE_WIDTH;
				float offset_v = 5.0f * RandFloat() / (float)IMAGE_HEIGHT;


				float u = -((float)(x * 2.f - IMAGE_WIDTH) / IMAGE_WIDTH) * aspectRatio + offset_u; // transforms to [-1.0, 1.0]
				float v = -((float)(y * 2.f - IMAGE_HEIGHT) / IMAGE_HEIGHT) + offset_v;               // transforms to [-1.0, 1.0]


				Ray ray;
				ray.origin = cam.origin;
				ray.direction = -glm::normalize(glm::vec3(u, v, 0.0) - cam.origin);

				color += render.Trace(ray);

			}
			color = color / (float)sapmles;
			int index = (x + y * IMAGE_WIDTH) * 4;
			color.a = 1.0f;
			image[index + 0] = static_cast<int>(color.r * 255.0f);
			image[index + 1] = static_cast<int>(color.g * 255.0f);
			image[index + 2] = static_cast<int>(color.b * 255.0f);
			image[index + 3] = static_cast<int>(color.a * 255.0f);
		}
	}
#endif
	return 0;
}