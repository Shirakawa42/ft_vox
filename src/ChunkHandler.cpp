#include "ChunkHandler.hpp"

ChunkHandler::ChunkHandler()
{
	nbEnabledChunks = 0;
	nbDisabledChunks = 0;
	enabledChunks = (t_ChunkList*)malloc(sizeof(t_ChunkList));
	enabledChunks->next = NULL;
	enabledChunks->chunk = NULL;
	enabledChunks->prev = NULL;
	disabledChunks = NULL;
	mapgen = new MapGeneration();
}

ChunkHandler::~ChunkHandler()
{
	delete(mapgen);
}

Chunk	*ChunkHandler::GenerateChunk(int x, int y)
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
	t_ChunkList	*tmp;

	tmp = enabledChunks;
	while (tmp)
	{
		if (tmp->chunk != NULL)
			if (tmp->chunk->x == x && tmp->chunk->y == y)
				return true;
		tmp = tmp->next;
	}
	tmp = disabledChunks;
	while (tmp)
	{
		if (tmp->chunk != NULL)
			if (tmp->chunk->x == x && tmp->chunk->y == y)
				return true;
		tmp = tmp->next;
	}
	return false;
}

void	ChunkHandler::AddChunkToEnabledList(int x, int y)
{
	t_ChunkList	*tmp;

	tmp = enabledChunks;
	if (tmp->chunk != NULL)
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = (t_ChunkList*)malloc(sizeof(t_ChunkList));
		tmp->next->prev = tmp;
		tmp = tmp->next;
		tmp->next = NULL;
	}
	tmp->chunk = GenerateChunk(x, y);
	lastGeneratedChunk = tmp->chunk;
	nbEnabledChunks++;
}

void	ChunkHandler::GenerateChunks()
{
	int		i;
	int		j;
	int		x;
	int		y;

	i = VIEW_DISTANCE + player.GetPos().x;
	while (i > -VIEW_DISTANCE - CHUNK_XY + player.GetPos().x)
	{
		j = VIEW_DISTANCE + player.GetPos().z;
		while (j > -VIEW_DISTANCE - CHUNK_XY + player.GetPos().z)
		{
			if (i < 0)
				x = (-(-i - (-i % CHUNK_XY) - CHUNK_XY/2)) - CHUNK_XY/2;
			else
				x = (i - (i % CHUNK_XY) - CHUNK_XY/2) - CHUNK_XY/2;
			if (j < 0)
				y = (-(-j - (-j % CHUNK_XY) - CHUNK_XY/2)) - CHUNK_XY/2;
			else
				y = (j - (j % CHUNK_XY) - CHUNK_XY/2) - CHUNK_XY/2;
			if (CheckIfChunkAtPos(x, y) == false)
			{
				AddChunkToEnabledList(x, y);
				std::cout << "Chunk generated ! Nb enabled chunks: " << nbEnabledChunks << " with " << lastGeneratedChunk->nbInstances << " cubes." << std::endl;
			}
			j -= CHUNK_XY;
		}
		i -= CHUNK_XY;
	}
}

void	ChunkHandler::MapHandler()
{
	t_ChunkList	*target;
	t_ChunkList	*tmp2;
	t_ChunkList	*r;

	target = enabledChunks;
	while (target && target->chunk)
	{
		r = target->next;
		if (glm::distance(glm::vec2(target->chunk->x, target->chunk->y), glm::vec2(player.GetPos().x, player.GetPos().z)) > VIEW_DISTANCE * 2.0f)
		{
			target->chunk->enabled = false;
			if (target->prev)
			{
				target->prev->next = target->next;
				if (target->next)
					target->next->prev = target->prev;
			}
			else
			{
				enabledChunks = enabledChunks->next;
				enabledChunks->prev = NULL;
			}
			tmp2 = disabledChunks;
			if (tmp2 != NULL)
			{
				while (tmp2->next)
					tmp2 = tmp2->next;
				tmp2->next = target;
				tmp2->next->prev = tmp2;
				tmp2 = tmp2->next;
				tmp2->next = NULL;
			}
			else
			{
				disabledChunks = target;
				disabledChunks->prev = NULL;
				disabledChunks->next = NULL;
			}
			nbEnabledChunks--;
			nbDisabledChunks++;
		}
		target = r;
	}
	target = disabledChunks;
	while (target && target->chunk)
	{
		r = target->next;
		if (glm::distance(glm::vec2(target->chunk->x, target->chunk->y), glm::vec2(player.GetPos().x, player.GetPos().z)) < VIEW_DISTANCE)
		{
			target->chunk->enabled = true;
			if (target->prev)
			{
				target->prev->next = target->next;
				if (target->next)
					target->next->prev = target->prev;
			}
			else
			{
				disabledChunks = target->next;
				if (disabledChunks)
					disabledChunks->prev = NULL;
			}
			tmp2 = enabledChunks;
			while (tmp2->next)
				tmp2 = tmp2->next;
			tmp2->next = target;
			tmp2->next->prev = tmp2;
			tmp2->next->next = NULL;
			nbEnabledChunks++;
			nbDisabledChunks--;
		}
		target = r;
	}
	GenerateChunks();
}

bool	ChunkHandler::isChunkVisible(Chunk *chunk)
{
	return true;
}

void	ChunkHandler::disableNonVisible()
{
	t_ChunkList	*tmp;

	tmp = enabledChunks;
	while (tmp && tmp->chunk->enabled == true)
	{
		tmp->chunk->enabled = isChunkVisible(tmp->chunk);
		tmp = tmp->next;
	}
}