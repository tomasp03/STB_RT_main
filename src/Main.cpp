#include "Renderer.h"
#include "Files.h"


#define IMAGE_WIDTH 1000
#define IMAGE_HEIGHT 700
#define IMAGE_NAME "RayTracing.png"

#define MT 1



int main()
{
	Renderer render = Renderer(IMAGE_WIDTH, IMAGE_HEIGHT);

#if MT

	//Test


	//Test end

	while(render.Render());
	
	render.ShutWindow();

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
	//std::cin.get();
	return 0;
}