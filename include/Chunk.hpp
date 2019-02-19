#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include "Maths.hpp"
#include "MapGeneration.hpp"
#include "Player.hpp"
#include <mutex>

# define CHUNK_XY 64
# define CHUNK_Z 256
# define CHUNK_SIZE CHUNK_XY*CHUNK_XY*CHUNK_Z

class Chunk
{
	public:
		Chunk(glm::vec2 position, int id);
		~Chunk();
		glm::vec2		GetPos();
		int				GetNbInstances();
		GLuint			GetVBOID();
		GLuint			GetIBOID();
		GLuint			GetTID();
		GLuint			GetCID();
		bool			isEnabled();
		void			Enable();
		void			Disable();
		void			generate(MapGeneration **map, std::mutex *mutex);
		bool			isGenerated();
		bool			isUsable();
		void			doOpenGLThings();
		int				id;
	private:
		bool			usable;
		bool			generated;
		bool			enabled;
		GLuint			vboID;
		GLuint			iboID;
		GLuint			translationsID;
		GLuint			cubeID;
		int				nbInstances;
		float			*vertices;
		unsigned int	*indices;
		GLfloat			*translations;
		GLuint			*cubes;
		const glm::vec2	position;
		MapGeneration	*mapgen;
		int				nb;
		unsigned char	***chunk;
		void			calcVertices();
		void			setVBO();
		void			setIBO();
		void			addCubeVertices();
		void			addIndices();
		void			setTranslationsO();
		void			setCubeO(int nb);
		bool			isCubeVisible(int x, int y, int z);
};