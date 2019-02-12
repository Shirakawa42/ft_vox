#pragma once

# define MAX_LOADED_CHUNKS 100

#include "Chunk.hpp"
#include <vector>
#include <iostream>
#include "Player.hpp"
#include "Maths.hpp"

class ChunkHandler
{
	public:
		ChunkHandler();
		~ChunkHandler();
		void	FlatMapHandler();
		Chunk	**loaded_chunks;
		Player	player;
	private:
		Chunk	*CreateFlatChunk(int x, int y);
		void	GenerateFlatChunks();
		bool	CheckIfChunkAtPos(int x, int y);
};