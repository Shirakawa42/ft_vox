#pragma once

# define MAX_LOADED_CHUNKS 200000

#include "Chunk.hpp"
#include <vector>
#include <iostream>
#include "Player.hpp"
#include "Maths.hpp"
#include "MapGeneration.hpp"

class ChunkHandler
{
	public:
		ChunkHandler();
		~ChunkHandler();
		void			MapHandler();
		Chunk			*GetFirstChunk();
	private:
		MapGeneration	*mapgen;
		Chunk			*GenerateChunk(int x, int y);
		void			GenerateChunks();
		bool			CheckIfChunkAtPos(int x, int y);
		void			AddChunkToEnabledList(int x, int y);
		Chunk			*firstChunk;
};