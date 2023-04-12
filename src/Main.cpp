#include "Renderer.h"
#include "UI.h"
#include "Timer.h"
#include "Files.h"


#define IMAGE_WIDTH 1000
#define IMAGE_HEIGHT 700
#define IMAGE_NAME "RayTracing.png"

#define MT 1

int main()
{
	Renderer render = Renderer(IMAGE_WIDTH, IMAGE_HEIGHT);
	Scene scene = Scene();
    Camera camera = Camera(glm::vec3(0, 0, -3), glm::vec3(0, 1, -1), glm::vec3(0, 1, 0), 90.0f, (float)IMAGE_WIDTH / (float)IMAGE_HEIGHT);
    Image* image;
    UI ui = UI();

    scene.Load("pepa.txt");

    float FPS, diff;

    while (ui.IsRunning())
    {
        Timer timer = Timer(&diff, &FPS);
        image = render.Render(scene, camera);
        ui.Update(render, FPS, diff);
        camera.Recalculate(image->AspectRatio());
    }

	//std::cin.get();
	return 0;
}