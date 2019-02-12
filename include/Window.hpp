#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "VoxException.hpp"
#include "ChunkHandler.hpp"
#include "shaders.hpp"
#include "Texture.hpp"

class Window
{
	public:
		Window();
		~Window();
		void	init();
		void	loop();
		float	getDeltaTime();
	private:
		GLFWwindow		*window;
		ChunkHandler	chunkHandler;
		float			deltaTime;
		void			handleTime();
		GLuint			vaoID;
		GLuint			programID;
		shaders			s;
		Texture			texture;
		GLuint			MatrixID;
		GLuint			grassID;
		GLuint			stoneID;
};