#pragma once

# define MAX_LOADED_CHUNKS 20000

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
		void	FlatMapHandler();
		Chunk	**loaded_chunks;
		Player	player;
		int		nbLoadedChunks;
	private:
		MapGeneration	*mapgen;
		Chunk			*CreateFlatChunk(int x, int y);
		void			GenerateFlatChunks();
		bool			CheckIfChunkAtPos(int x, int y);
};