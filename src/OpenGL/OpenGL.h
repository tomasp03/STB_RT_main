#include <glad/glad.h>
#include "ShaderClass.h"
#include "Image.h"

namespace OpenGL
{
	void Init();
	void UpdateViewPort(int width, int height);
	void Draw(Image* image);
}