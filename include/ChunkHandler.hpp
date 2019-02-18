#pragma once

# define MAX_LOADED_CHUNKS 200000

#include "Chunk.hpp"
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include "Player.hpp"
#include "Maths.hpp"
#include "MapGeneration.hpp"

class ChunkHandler
{
	public:
		ChunkHandler();
		~ChunkHandler();
		void				MapHandler();
		void				LoadChunks();
	private:
		std::vector<Chunk*>			enabledChunks;
		std::vector<Chunk*>			disabledChunks;
		MapGeneration				*mapgen;
		Chunk						*GenerateChunk(int x, int y);
		void						GenerateChunks();
		void						AddChunkAtPos(int x, int y);
		bool						CheckIfChunkAtPos(int x, int y);
		void						DisableChunks();
		std::thread					*t;
		std::mutex					mutex;
};