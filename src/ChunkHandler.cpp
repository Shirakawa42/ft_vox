#include "ChunkHandler.hpp"

ChunkHandler::ChunkHandler()
{
	int i;

	loaded_chunks = (Chunk**)malloc(sizeof(Chunk*) * MAX_LOADED_CHUNKS);
	i = 0;
	while (i < MAX_LOADED_CHUNKS)
		loaded_chunks[i++] = NULL;
}

ChunkHandler::~ChunkHandler()
{
	
}

Chunk	*ChunkHandler::CreateFlatChunk(int x, int y)
{
	Chunk	*chunk = new Chunk();
	
	chunk->x = x;
	chunk->y = y;
	chunk->setFlat();
	return chunk;
}

bool	ChunkHandler::CheckIfChunkAtPos(int x, int y)
{
	int		i;

	i = 0;
	while (i < MAX_LOADED_CHUNKS)
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
		j = VIEW_DISTANCE + player.GetPos().y;
		while (j > -VIEW_DISTANCE - CHUNK_XY + player.GetPos().y)
		{
			if (i < 0)
				x = -(-i - (-i % CHUNK_XY) - 32);
			else
				x = i - (i % CHUNK_XY) - 32;
			if (j < 0)
				y = -(-j - (-j % CHUNK_XY) - 32);
			else
				y = j - (j % CHUNK_XY) - 32;
			if (CheckIfChunkAtPos(x, y) == false)
			{
				k = 0;
				while (k < MAX_LOADED_CHUNKS)
				{
					if (loaded_chunks[k] == NULL)
					{
						loaded_chunks[k] = CreateFlatChunk(x, y);
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
	//GenerateFlatChunks();
	if (loaded_chunks[0] == NULL)
		loaded_chunks[0] = CreateFlatChunk(0, 0);
}