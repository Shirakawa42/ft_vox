#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

class Texture
{
	public:
		Texture();
		~Texture();	
		GLuint			load_cubemap(char *top_name, char *side_name, char *bot_name);
	private:
		unsigned char	*load_bmp(char *filename, unsigned int *width, unsigned int *height);
};