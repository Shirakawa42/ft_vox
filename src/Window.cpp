#include "Window.hpp"

Window::Window()
{

}

Window::~Window()
{
	glfwTerminate();
}

float	Window::getDeltaTime()
{
	return deltaTime;
}

void	Window::handleTime()
{
	float			current_frame;
	static float	last_frame = 0.0f;
	static int		fps = 0;
	static float	fpstime = 0.0f;

	current_frame = glfwGetTime();
	if (last_frame == 0.0f)
		last_frame = current_frame;
	deltaTime = current_frame - last_frame;
	last_frame = current_frame;
	fps++;
	fpstime += deltaTime;
	if (fpstime >= 1.0f)
	{
		std::cout << "FPS: " << fps << std::endl;
		fpstime -= 1.0f;
		fps = 0;
	}
}

void	Window::init()
{
	if (!glfwInit())
		throw VoxException("ERROR: could not start GLFW3\n");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(1000, 1000, "ft_vox", NULL, NULL);
	if (!window)
		throw VoxException("ERROR: could not open window with GLFW3\n");
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	
	programID = s.create_program(s.create_shader((char*)"shaders/vertex.glsl",
			GL_VERTEX_SHADER), s.create_shader((char*)"shaders/fragment.glsl",
			GL_FRAGMENT_SHADER));
	MatrixID = glGetUniformLocation(programID, "MVP");
	grassID = glGetUniformLocation(programID, "grass");
	stoneID = glGetUniformLocation(programID, "stone");
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void	Window::loop()
{
	GLuint	grass;
	GLuint	stone;

	grass = texture.load_bmp((char*)"texture/grass.bmp");
	stone = texture.load_bmp((char*)"texture/stone.bmp");

	glfwSwapInterval(0);
	glEnable(GL_CULL_FACE); 
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		handleTime();
		chunkHandler.player.deltaTime = deltaTime;
		chunkHandler.FlatMapHandler();
		chunkHandler.player.mouseControl(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &chunkHandler.player.mvp[0][0]);

		glUniform1i(grassID, 0);
		glUniform1i(stoneID, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, grass);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, stone);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, chunkHandler.loaded_chunks[0]->vboID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, chunkHandler.loaded_chunks[0]->translationsID);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glVertexAttribDivisor(1, 1);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, chunkHandler.loaded_chunks[0]->cubeID);
		glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(GLuint), 0);
		glVertexAttribDivisor(2, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkHandler.loaded_chunks[0]->iboID);
		glDrawElementsInstanced(GL_TRIANGLES, chunkHandler.loaded_chunks[0]->nbIndices, GL_UNSIGNED_INT, NULL, chunkHandler.loaded_chunks[0]->nbInstances);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	exit(0);
}