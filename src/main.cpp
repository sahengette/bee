#include "stdafx.hpp"

#include "shader.hpp"
#include "mesh.hpp"
#include "camera.hpp"

void key_callback (GLFWwindow * window, int key, int , int action, int  )
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		}
	if (key == GLFW_KEY_W)
		{
			bee::camera::CameraInMove->go();
			std::cerr << "go";
		}
	if (key == GLFW_KEY_S)
		{
			bee::camera::CameraInMove->back();
		}
		
}

void mopos_callback(GLFWwindow * w, double x, double y)
{
	std::cout << "x=" << x << "  y=" << y << std::endl;
	bee::camera::CameraInMove->pixelMoved(x,y);
	glfwSetCursorPos(w,0,0);
}

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

	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental=true; 
	glewInit();


	bee::VertexShader vtx;
	bee::FragmentShader frg;
	bee::FragmentShader line;
	vtx.loadFromFile("bin/vert");
	vtx.compile();
	frg.loadFromFile("bin/frag");
	frg.compile();
	bee::Program program(&vtx, &frg);

	bee::mesh objmesh;
	objmesh.loadOBJfilespirit("suzannen.obj");

	glBindAttribLocation(program.getID(), 0, "vertex_in");
	glBindAttribLocation(program.getID(), 1, "normal_in");
	program.linkProgram();


	GLuint triangle_buffer[2];
	glGenBuffers(3, &triangle_buffer[0]);

	glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, objmesh.m_vertex_pos.size() * sizeof(glm::vec3) , &objmesh.m_vertex_pos[0],  GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer[2]);
	glBufferData(GL_ARRAY_BUFFER, objmesh.m_vertex_norm.size() * sizeof(glm::vec3) , &objmesh.m_vertex_norm[0], GL_STATIC_DRAW);


	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);

	bee::camera cam(glm::vec3(4,2,4), glm::vec3(0,0,0), glm::vec3(0,1,0));

	
	auto View = cam.getViewMat();
	bee::camera::CameraInMove = &cam;
	
	glm::mat4 Model      = glm::mat4(1.0f);  // Changes for each model !
	Model = glm::translate(Model,glm::vec3(-6,-6,-6));

	GLuint MatrixID = glGetUniformLocation(program.getID(), "mvp_matrix");
	GLuint ModelID = glGetUniformLocation(program.getID(), "norm_matrix");
	glClearColor(0.8,0.8,0.8, 0.0f);
	
	glfwSetCursorPosCallback(window, mopos_callback);
	while (!glfwWindowShouldClose(window))
	{
		View = cam.getViewMat();

	//	Model = glm::rotate(Model, 1.0f, glm::vec3(0,1,0) );	
		
		glm::mat4 mvp  = Projection * View* Model;
		glm::mat4 mv   = View*Model;
		
		mv = glm::transpose(glm::inverse(mv)); //matrice des normales
		
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer[0]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer[2]);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	
		program.useProgram();
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(ModelID, 1, GL_FALSE, &mv[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, objmesh.m_vertex_pos.size());
		program.unuseProgram();

		glDisableVertexAttribArray(0);


		glfwSwapBuffers(window);
		glfwPollEvents();
		

	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	glfwTerminate();
	return 0;
}

