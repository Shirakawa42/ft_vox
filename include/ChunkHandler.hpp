#pragma once

# define MAX_LOADED_CHUNKS 200000

#include "Chunk.hpp"
#include <vector>
#include <list>
#include <iostream>
#include <thread>
#include <mutex>
#include "Player.hpp"
#include "Maths.hpp"
#include "MapGeneration.hpp"

class ChunkHandler
{
	public:
		ChunkHandler(bool is_seed, unsigned int seed);
		~ChunkHandler();
		void				MapHandler();
		void				LoadChunks();
	private:
		std::list<Chunk*>			enabledChunks;
		std::list<Chunk*>			disabledChunks;
		MapGeneration				*mapgen;
		Chunk						*GenerateChunk(int x, int y);
		void						GenerateChunks();
		void						AddChunkAtPos(int x, int y);
		bool						CheckIfChunkAtPos(int x, int y);
		void						DisableChunks();
		std::thread					*t;
		std::mutex					mutex;
};