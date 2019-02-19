#include "Window.hpp"

Window::Window()
{

}

Window::~Window()
{
	glfwTerminate();
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
	g_deltaTime = current_frame - last_frame;
	last_frame = current_frame;
	fps++;
	fpstime += g_deltaTime;
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
	window = glfwCreateWindow(Wscreen, Hscreen, "ft_vox", NULL, NULL);
	if (!window)
		throw VoxException("ERROR: could not open window with GLFW3\n");
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE);

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	
	programID = s.create_program(s.create_shader((char*)"shaders/vertex.glsl",
			GL_VERTEX_SHADER), s.create_shader((char*)"shaders/fragment.glsl",
			GL_FRAGMENT_SHADER));
	MatrixID = glGetUniformLocation(programID, "MVP");
	grassID = glGetUniformLocation(programID, "grass");
	stoneID = glGetUniformLocation(programID, "stone");
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	std::cout << vendor << " ||| " << renderer << std::endl;
}

void	Window::loop(bool is_seed, unsigned int seed)
{
	GLuint				grass;
	GLuint				stone;
	ChunkHandler		chunkHandler(is_seed, seed);

	grass = texture.load_cubemap((char*)"texture/grass_top.bmp", (char*)"texture/grass_side.bmp", (char*)"texture/dirt.bmp");
	stone = texture.load_cubemap((char*)"texture/stone.bmp", (char*)"texture/stone.bmp", (char*)"texture/stone.bmp");

	glEnable(GL_CULL_FACE); 

	float	tmp_time;
	float	duration[3];
	float	total;
	duration[0] = 0;
	duration[1] = 0;
	duration[2] = 0;
	total = 0;

	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		handleTime();
		tmp_time = glfwGetTime();
		chunkHandler.MapHandler();
		g_player.mouseControl(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g_player.mvp[0][0]);

		glUniform1i(grassID, 0);
		glUniform1i(stoneID, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, grass);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, stone);

		duration[0] += glfwGetTime() - tmp_time;
		total += glfwGetTime() - tmp_time;
		tmp_time = glfwGetTime();
		
		chunkHandler.LoadChunks();

		duration[1] += glfwGetTime() - tmp_time;
		total += glfwGetTime() - tmp_time;
		tmp_time = glfwGetTime();

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		duration[2] += glfwGetTime() - tmp_time;
		total += glfwGetTime() - tmp_time;
		if (total > 1.0f)
		{
			std::cout << "part1: " << (duration[0] / total)*100 << ", part2: " << (duration[1] / total)*100 << ", part3: " << (duration[2] / total)*100 << std::endl;
			total -= 1.0f;
			duration[0] = 0;
			duration[1] = 0;
			duration[2] = 0;
		}
	}
	exit(0);
}