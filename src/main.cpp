#include "stdafx.hpp"
#include "shader.hpp"
int main(void)
{

		static const GLfloat g_vertex_buffer_data[] = {
				-1.0f, -1.0f, 0.0f,
				1.0f, -1.0f, 0.0f,
				0.0f,  1.0f, 0.0f,
		};

		GLFWwindow* window;

		if (!glfwInit())
				return -1;


		window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
		if (!window)
		{
				glfwTerminate();
				return -1;
		}


		glfwMakeContextCurrent(window);

		glewExperimental=true; 
		if (glewInit() != GLEW_OK) { return -1;  }

		bee::VertexShader vtx;
		bee::FragmentShader frg;
		vtx.loadFromFile("bin/vert");
		vtx.compile();
		frg.loadFromFile("bin/frag");
		frg.compile();
		bee::Program program(&vtx, &frg);
		program.linkProgram();

		GLuint vertexbuffer;


		// Generate 0 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexbuffer);

		// The following commands will talk about our 'vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		while (!glfwWindowShouldClose(window))
		{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
				glVertexAttribPointer(
								0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
								3,                  // size
								GL_FLOAT,           // type
								GL_FALSE,           // normalized?
								0,                  // stride
								(void*)0            // array buffer offset
								);

				// Draw the triangle !
				program.useProgram();
				glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
				program.unuseProgram();
				glDisableVertexAttribArray(0);

				glfwSwapBuffers(window);
				glfwPollEvents();

		}

		glfwTerminate();
		return 0;
}

