#include "Chunk.hpp"
#include <iostream>

Chunk::Chunk()
{
	glGenBuffers(1, &vboID);
	glGenBuffers(1, &iboID);
	nbVertices = 0;
	nbIndices = 0;
	nbUv = 0;
}

Chunk::~Chunk()
{
	free(vertices);
	glDeleteBuffers(1, &vboID);
}

void	Chunk::setFlat()
{
	int		x;
	int		y;
	int		z;

	z = 0;
	while (z < CHUNK_Z)
	{
		y = 0;
		while (y < CHUNK_XY)
		{
			x = 0;
			while (x < CHUNK_XY)
			{
				if (z < 1 && x < 1 && y < 1)
					chunk[x][y][z] = 1;
				else
					chunk[x][y][z] = 0;
				x++;
			}
			y++;
		}
		z++;
	}
	calcVertices();
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

void	Chunk::addCubeVertices(int x, int y, int z, int nb)
{
	float	cube[] = {
    	-0.5f + x + this->x, -0.5f + z,  0.5f + y + this->y,
		 0.5f + x + this->x, -0.5f + z,  0.5f + y + this->y,
		 0.5f + x + this->x,  0.5f + z,  0.5f + y + this->y,
		-0.5f + x + this->x,  0.5f + z,  0.5f + y + this->y,
		-0.5f + x + this->x, -0.5f + z, -0.5f + y + this->y,
		 0.5f + x + this->x, -0.5f + z, -0.5f + y + this->y,
		 0.5f + x + this->x,  0.5f + z, -0.5f + y + this->y,
		-0.5f + x + this->x,  0.5f + z, -0.5f + y + this->y
	};
	int		i;

	i = 0;
	while (i < 8 * 3)
	{
		vertices[(nb * 3 * 8) + i] = cube[i];
		vertices[(nb * 3 * 8) + i + 1] = cube[i + 1];
		vertices[(nb * 3 * 8) + i + 2] = cube[i + 2];
		i += 3;
	}
}

void	Chunk::addIndices(int nb)
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
		indices[i] += 8 * nb;
		indices[i + 1] += 8 * nb;
		indices[i + 2] += 8 * nb;
		this->indices[(nb * 3 * 12) + i] = indices[i];
		this->indices[(nb * 3 * 12) + i + 1] = indices[i + 1];
		this->indices[(nb * 3 * 12) + i + 2] = indices[i + 2];
		i += 3;
	}
}

void	Chunk::addUv(int nb)
{

}

void	Chunk::calcVertices()
{
	int		x;
	int		y;
	int		z;
	int		nb;

	x = 0;
	while (x < CHUNK_XY)
	{
		y = 0;
		while (y < CHUNK_XY)
		{
			z = 0;
			while (z < CHUNK_Z)
			{
				if (chunk[x][y][z] == 1)
				{
					nbVertices += 8;
					nbIndices += 12 * 3;
					nbUv += 8;
				}
				z++;
			}
			y++;
		}
		x++;
	}
	vertices = (float*)malloc(sizeof(float) * nbVertices * 3);
	indices = (unsigned int*)malloc(sizeof(unsigned int) * nbIndices);
	uv = (float*)malloc(sizeof(float) * nbUv);
	x = 0;
	nb = 0;
	while (x < CHUNK_XY)
	{
		y = 0;
		while (y < CHUNK_XY)
		{
			z = 0;
			while (z < CHUNK_Z)
			{
				if (chunk[x][y][z] == 1)
				{
					addCubeVertices(x, y, z, nb);
					addIndices(nb);
					addUv(nb);
					nb++;
				}
				z++;
			}
			y++;
		}
		x++;
	}
	setVBO();
	setIBO();
}