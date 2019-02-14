#include "ChunkHandler.hpp"

ChunkHandler::ChunkHandler()
{
	mapgen = new MapGeneration();
	firstChunk = new Chunk(glm::vec2(0, 0), &mapgen);
}

ChunkHandler::~ChunkHandler()
{
	delete(mapgen);
}

void	ChunkHandler::MapHandler()
{

}

Chunk	*ChunkHandler::GetFirstChunk()
{
	return firstChunk;
}