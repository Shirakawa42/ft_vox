#include "Texture.hpp"

Texture::Texture()
{

}

Texture::~Texture()
{

}

GLuint	Texture::load_bmp(char *filename)
{
	int				fd;
	GLuint			texture_id;
	unsigned char	header[54];
	unsigned int	datapos;
	unsigned int	width;
	unsigned int	height;
	unsigned int	image_size;
	unsigned char	*data;

	if ((fd = open(filename, O_RDONLY)) == -1)
	{
		std::cout << "not a correct BMP file !" << std::endl;
		exit(0);
	}
	if (read(fd, header, 54) != 54)
	{
		std::cout << "not a correct BMP file !" << std::endl;
		exit(0);
	}
	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << "not a correct BMP file !" << std::endl;
		exit(0);
	}
	datapos = *(int*)&(header[0x0A]);
	image_size = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	if (image_size == 0)
		image_size = width * height * 3;
	if (datapos == 0)
		datapos = 54;
	data = (unsigned char*)malloc(sizeof(unsigned char) * image_size);
	read(fd, data, image_size);
	close(fd);
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	for (GLint i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_BGR,
				GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return (texture_id);
}