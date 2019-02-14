#include "Chunk.hpp"
#include <iostream>

Chunk::Chunk()
{
	glGenBuffers(1, &vboID);
	glGenBuffers(1, &iboID);
	glGenBuffers(1, &translationsID);
	glGenBuffers(1, &cubeID);
	nbVertices = 8;
	nbIndices = 12*3;
	nbInstances = 0;
	vertices = (float*)malloc(sizeof(float) * nbVertices * 3);
	indices = (unsigned int*)malloc(sizeof(unsigned int) * nbIndices);
	translations = (GLfloat*)malloc(sizeof(GLfloat) * CHUNK_SIZE * 3);
	cubes = (GLuint*)malloc(sizeof(GLuint) * CHUNK_SIZE);
	enabled = true;
}

Chunk::~Chunk()
{
	free(vertices);
	free(indices);
	free(translations);
	free(cubes);
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &iboID);
	glDeleteBuffers(1, &translationsID);
	glDeleteBuffers(1, &cubeID);
}

void	Chunk::generate()
{
	int		x;
	int		y;
	int		z;
	int		power;
	int		nb;
	float	p;

	nb = 0;
	x = 0;
	while (x < CHUNK_XY)
	{
		y = 0;
		while (y < CHUNK_XY)
		{
			p = (*mapgen)->noise((float)(x + this->x) / 30.0f + 0.5f, (float)(y + this->y) / 30.0f + 0.5f, 0.0f);
			power = (int)(15.0f * p) + 160;
			z = 0;
			while (z < power)
			{
				if (z != power - 1)
					chunk[x][y][z] = 2;
				else
					chunk[x][y][z] = 1;
				z++;
			}
			while (z < CHUNK_Z)
			{
				chunk[x][y][z] = 0;
				z++;
			}
			y++;
		}
		x++;
	}
	x = 0;
	while (x < CHUNK_XY)
	{
		y = 0;
		while (y < CHUNK_XY)
		{
			z = 0;
			while(z < CHUNK_Z)
			{
				if (chunk[x][y][z] > 0)
				{
					if (chunk[x + 1][y][z] > 0 && chunk[x - 1][y][z] > 0 && chunk[x][y + 1][z] > 0 && chunk[x][y - 1][z] > 0 && chunk[x][y][z + 1] > 0 && chunk[x][y][z - 1] > 0 && x % CHUNK_XY != 0 && y % CHUNK_XY != 0 && z % CHUNK_Z != 0)
						chunkActive[x][y][z] = false;
					else
					{
						chunkActive[x][y][z] = true;
						translations[nb] = x + this->x;
						translations[nb + 2] = y + this->y;
						translations[nb + 1] = z;
						cubes[nbInstances] = chunk[x][y][z];
						nbInstances++;
						nb += 3;
					}
				}
				z++;
			}
			y++;
		}
		x++;
	}
	setTranslationsO(nb);
	setCubeO(nbInstances);
	calcVertices();
}

void	Chunk::setTranslationsO(int nb)
{
	glBindBuffer(GL_ARRAY_BUFFER, translationsID);
	glBufferData(GL_ARRAY_BUFFER, nb * sizeof(GLfloat), translations, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void	Chunk::setCubeO(int nb)
{
	glBindBuffer(GL_ARRAY_BUFFER, cubeID);
	glBufferData(GL_ARRAY_BUFFER, nb * sizeof(GLuint), cubes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void	Chunk::setVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, nbVertices * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void	Chunk::setIBO()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void	Chunk::addCubeVertices()
{
	float	cube[] = {
    	-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	int		i;

	i = 0;
	while (i < 8 * 3)
	{
		vertices[i] = cube[i];
		vertices[i + 1] = cube[i + 1];
		vertices[i + 2] = cube[i + 2];
		i += 3;
	}
}

void	Chunk::addIndices()
{
	unsigned int	indices[] = {
		0, 1, 2,
		2, 3, 0,
		1, 5, 6,
		6, 2, 1,
		7, 6, 5,
		5, 4, 7,
		4, 0, 3,
		3, 7, 4,
		4, 5, 1,
		1, 0, 4,
		3, 2, 6,
		6, 7, 3
	};
	int		i;

	i = 0;
	while (i < 12 * 3)
	{
		this->indices[i] = indices[i];
		this->indices[i + 1] = indices[i + 1];
		this->indices[i + 2] = indices[i + 2];
		i += 3;
	}
}

void	Chunk::calcVertices()
{
	addCubeVertices();
	addIndices();
	setVBO();
	setIBO();
}