#pragma once

#include "Chunk.hpp"
#include <vector>
#include <map>
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
		std::map<int, Chunk*>			enabledChunks;
		std::map<int, Chunk*>			disabledChunks;
		MapGeneration				*mapgen;
		Chunk						*GenerateChunk(int x, int y);
		void						GenerateChunks();
		void						AddChunkAtPos(int x, int y);
		bool						CheckIfChunkAtPos(int x, int y);
		void						DisableChunks();
		void						RemoveFarChunks();
		std::thread					*t;
		std::mutex					mutex;
};