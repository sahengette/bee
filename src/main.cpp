#include "stdafx.hpp"
#include "shader.hpp"
int main(void)
{


	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};
	static const GLuint g_index_buffer_data[] = {
	1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36
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
	glewInit();


	bee::VertexShader vtx;
	bee::FragmentShader frg;
	vtx.loadFromFile("bin/vert");
	vtx.compile();
	frg.loadFromFile("bin/frag");
	frg.compile();
	bee::Program program(&vtx, &frg);

	glBindAttribLocation(program.getID(), 0, "vertex_in");
	program.linkProgram();

	GLuint triangle_buffer[1];
	glGenBuffers(2, &triangle_buffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data) , g_vertex_buffer_data,  GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_index_buffer_data), g_index_buffer_data, GL_STATIC_DRAW);




	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 View       = glm::lookAt(
			glm::vec3(4,3,3), 
			glm::vec3(0,0,0),
			glm::vec3(0,1,0)
			);
	glm::mat4 Model      = glm::mat4(1.0f);  // Changes for each model !
	glm::mat4 mvp        = Projection * View * Model;

	GLuint MatrixID = glGetUniformLocation(program.getID(), "mvp_matrix");
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer[0]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer[1]);

		program.useProgram();

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		
		glDrawArrays(GL_TRIANGLES,0 ,36);
		program.unuseProgram();
		glDisableVertexAttribArray(0);


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

