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
		GLuint		grass;
		GLuint		load_bmp(char *filename);
	private:
		unsigned char	header[54];
		unsigned int	datapos;
		unsigned int	width;
		unsigned int	height;
		unsigned int	image_size;
		unsigned char	*data;
};