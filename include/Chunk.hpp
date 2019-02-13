#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include "Maths.hpp"
#include "MapGeneration.hpp"

# define CHUNK_XY 32
# define CHUNK_Z 256
# define CHUNK_SIZE CHUNK_XY*CHUNK_XY*CHUNK_Z

class Chunk
{
	public:
		Chunk();
		~Chunk();
		void			generate();
		int				x;
		int				y;
		GLuint			vboID;
		GLuint			iboID;
		float			*vertices;
		int				nbVertices;
		unsigned int	*indices;
		int				nbIndices;
		int				nbInstances;
		GLfloat			*translations;
		GLuint			translationsID;
		GLuint			cubeID;
		GLuint			*cubes;
		MapGeneration	**mapgen;
	private:
		int		chunk[CHUNK_XY][CHUNK_XY][CHUNK_Z];
		void	calcVertices();
		void	setVBO();
		void	setIBO();
		void	addCubeVertices();
		void	addIndices();
		void	setTranslationsO(int nb);
		void	setCubeO(int nb);
};