#pragma once

# define MAX_LOADED_CHUNKS 200000

#include "Chunk.hpp"
#include <vector>
#include <iostream>
#include "Player.hpp"
#include "Maths.hpp"
#include "MapGeneration.hpp"

typedef struct	s_ChunkList
{
	Chunk				*chunk;
	struct s_ChunkList	*next;
	struct s_ChunkList	*prev;
}				t_ChunkList;

class ChunkHandler
{
	public:
		ChunkHandler();
		~ChunkHandler();
		void			MapHandler();
		void			disableNonVisible();
		Player			player;
		int				nbEnabledChunks;
		int				nbDisabledChunks;
		t_ChunkList		*enabledChunks;
		t_ChunkList		*disabledChunks;
	private:
		MapGeneration	*mapgen;
		Chunk			*GenerateChunk(int x, int y);
		void			GenerateChunks();
		bool			CheckIfChunkAtPos(int x, int y);
		bool			isChunkVisible(Chunk *chunk);
		void			AddChunkToEnabledList(int x, int y);
		Chunk			*lastGeneratedChunk;
};