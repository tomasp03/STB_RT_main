#include "OpenGL.h"

namespace OpenGL
{
    static GLuint screenTex;
    static GLuint VAO, VBO, EBO;
    static Shader* m_shader;


	void Init()
	{
        m_shader = new Shader("shaders\\basic.vert", "shaders\\basic.frag");

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

        glGenTextures(1, &screenTex);
        glBindTexture(GL_TEXTURE_2D, screenTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
    void UpdateViewPort(int width, int height)
    {
        glViewport(0, 0, width, height);
    }
    void Draw(Image* image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->size.x, image->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
        glGenerateMipmap(GL_TEXTURE_2D);

        m_shader->Activate();
        glBindTexture(GL_TEXTURE_2D, screenTex);
        m_shader->uni1f("tex0", 0);
        glBindVertexArray(VAO);

        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}