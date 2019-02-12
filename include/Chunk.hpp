#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>

# define CHUNK_XY 64
# define CHUNK_Z 256
# define CHUNK_SIZE CHUNK_XY*CHUNK_XY*CHUNK_Z

class Chunk
{
	public:
		Chunk();
		~Chunk();
		void			setFlat();
		int				x;
		int				y;
		GLuint			vboID;
		GLuint			iboID;
		float			*vertices;
		int				nbVertices;
		unsigned int	*indices;
		int				nbIndices;
		float			*uv;
		int				nbUv;

	private:
		int		chunk[CHUNK_XY][CHUNK_XY][CHUNK_Z];
		void	calcVertices();
		void	setVBO();
		void	setIBO();
		void	addCubeVertices(int x, int y, int z, int nb);
		void	addIndices(int nb);
		void	addUv(int nb);
};