#include "ChunkHandler.hpp"

ChunkHandler::ChunkHandler()
{
	int i;

	nbLoadedChunks = 0;
	loaded_chunks = (Chunk**)malloc(sizeof(Chunk*) * MAX_LOADED_CHUNKS);
	i = 0;
	while (i < MAX_LOADED_CHUNKS)
		loaded_chunks[i++] = NULL;
	mapgen = new MapGeneration();
}

ChunkHandler::~ChunkHandler()
{
	
}

Chunk	*ChunkHandler::CreateFlatChunk(int x, int y)
{
	Chunk	*chunk = new Chunk();
	
	chunk->x = x;
	chunk->y = y;
	chunk->mapgen = &mapgen;
	chunk->generate();
	return chunk;
}

bool	ChunkHandler::CheckIfChunkAtPos(int x, int y)
{
	int		i;

	i = 0;
	while (i < nbLoadedChunks)
	{
		if (loaded_chunks[i] != NULL)
			if (loaded_chunks[i]->x == x && loaded_chunks[i]->y == y)
				return true;
		i++;
	}
	return false;
}

void	ChunkHandler::GenerateFlatChunks()
{
	int		i;
	int		j;
	int		k;
	int		x;
	int		y;

	i = VIEW_DISTANCE + player.GetPos().x;
	while (i > -VIEW_DISTANCE - CHUNK_XY + player.GetPos().x)
	{
		j = VIEW_DISTANCE + player.GetPos().z;
		while (j > -VIEW_DISTANCE - CHUNK_XY + player.GetPos().z)
		{
			if (i < 0)
				x = -(-i - (-i % CHUNK_XY) - CHUNK_XY/2);
			else
				x = i - (i % CHUNK_XY) - CHUNK_XY/2;
			if (j < 0)
				y = -(-j - (-j % CHUNK_XY) - CHUNK_XY/2);
			else
				y = j - (j % CHUNK_XY) - CHUNK_XY/2;
			if (CheckIfChunkAtPos(x, y) == false)
			{
				k = 0;
				while (k < MAX_LOADED_CHUNKS)
				{
					if (loaded_chunks[k] == NULL)
					{
						loaded_chunks[k] = CreateFlatChunk(x, y);
						nbLoadedChunks++;
						std::cout << "Chunk generated ! Nb loaded chunks: " << nbLoadedChunks << std::endl;
						break ;
					}
					k++;
				}
			}
			j -= CHUNK_XY;
		}
		i -= CHUNK_XY;
	}
}

void	ChunkHandler::FlatMapHandler()
{
	//int		i;
//
	//i = 0;
	//while (i < MAX_LOADED_CHUNKS)
	//{
	//	if (loaded_chunks[i] != NULL)
	//	{
	//		if (distance(loaded_chunks[i]->x, loaded_chunks[i]->y, player.GetPos().x, player.GetPos().y) > VIEW_DISTANCE * 2)
	//		{
	//			delete(loaded_chunks[i]);
	//			loaded_chunks[i] = NULL;
	//		}
	//	}
	//	i++;
	//}
	GenerateFlatChunks();
}