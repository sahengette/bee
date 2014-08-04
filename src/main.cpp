#include "stdafx.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include <iterator>

int main(void)
{



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

	bee::mesh objmesh;
	objmesh.loadOBJfile("logo.obj");
	
	glBindAttribLocation(program.getID(), 0, "vertex_in");
	program.linkProgram();

	GLuint triangle_buffer[1];
	glGenBuffers(2, &triangle_buffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, objmesh.m_vertex_pos.size() * sizeof(GLfloat), &objmesh.m_vertex_pos[0],  GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, objmesh.m_indice.size() * sizeof(GLushort), &objmesh.m_indice[0], GL_STATIC_DRAW);



	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 View       = glm::lookAt(
			glm::vec3(5,5,5), 
			glm::vec3(0,0,0),
			glm::vec3(0,1,0)
			);
	glm::mat4 Model      = glm::mat4(1.0f);  // Changes for each model !

	GLuint MatrixID = glGetUniformLocation(program.getID(), "mvp_matrix");
	glClearColor(1,1,1, 0.0f);



	while (!glfwWindowShouldClose(window))
	{
	
	Model = glm::rotate(Model, 1.0f, glm::vec3(0,1,0) );	
	glm::mat4 mvp  = Projection * View * Model;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer[0]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer[1]);

		program.useProgram();

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		
		glDrawElements(GL_TRIANGLES, objmesh.m_indice.size(), GL_UNSIGNED_SHORT, (void*)0 );
		program.unuseProgram();
		glDisableVertexAttribArray(0);


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

