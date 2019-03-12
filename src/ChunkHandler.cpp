#include "ChunkHandler.hpp"

ChunkHandler::ChunkHandler(bool is_seed, unsigned int seed)
{
	if (is_seed)
		mapgen = new MapGeneration(seed);
	else
		mapgen = new MapGeneration();
}

ChunkHandler::~ChunkHandler()
{
	delete(mapgen);
}

void	ChunkHandler::MapHandler()
{
	int		x;
	int		y;
	int		i;
	int		j;

	i = -VIEW_DISTANCE + g_player.GetPos().x - CHUNK_XY*4;
	while (i < VIEW_DISTANCE + g_player.GetPos().x)
	{
		j = -VIEW_DISTANCE + g_player.GetPos().z - CHUNK_XY*4;
		while (j < VIEW_DISTANCE + g_player.GetPos().z)
		{
			x = i - i % CHUNK_XY;
			y = j - j % CHUNK_XY;
			if (glm::distance(glm::vec2(x, y) + glm::vec2(CHUNK_XY/2, CHUNK_XY/2), glm::vec2(g_player.GetPos().x, g_player.GetPos().z)) <= VIEW_DISTANCE)
			{
				if (!CheckIfChunkAtPos(x, y))
					AddChunkAtPos(x, y);
			}
			j += CHUNK_XY;
		}
		i += CHUNK_XY;
	}
	DisableChunks();
	RemoveFarChunks();
}

static int	GetPosIndex(int x, int y)
{
	int xx = (x >= 0) ? (x * 2) : (-x * 2 - 1);
	int yy = (y >= 0) ? (y * 2) : (-y * 2 - 1);

	return ((xx + yy) * (xx + yy + 1))/2 + yy;
}

static int	GetPosIndex(glm::vec2 pos)
{
	return GetPosIndex(pos.x, pos.y);
}

void	ChunkHandler::CheckNeigbors(Chunk *chunk)
{
	Chunk	*neighbor;

	if ((neighbor = GetChunkAtPos((chunk->GetPos() + glm::vec2(CHUNK_XY, 0)).x,
			(chunk->GetPos() + glm::vec2(CHUNK_XY, 0)).y)) != NULL)
		neighbor->left = chunk;
	if ((neighbor = GetChunkAtPos((chunk->GetPos() + glm::vec2(-CHUNK_XY, 0)).x,
			(chunk->GetPos() + glm::vec2(-CHUNK_XY, 0)).y)) != NULL)
		neighbor->right = chunk;
	if ((neighbor = GetChunkAtPos((chunk->GetPos() + glm::vec2(0, CHUNK_XY)).x,
			(chunk->GetPos() + glm::vec2(0, CHUNK_XY)).y)) != NULL)
		neighbor->front = chunk;
	if ((neighbor = GetChunkAtPos((chunk->GetPos() + glm::vec2(0, -CHUNK_XY)).x,
			(chunk->GetPos() + glm::vec2(0, -CHUNK_XY)).y)) != NULL)
		neighbor->back = chunk;
}

void	ChunkHandler::DisableChunks()
{
	Chunk	*chunk;
	std::map<int, Chunk*>::iterator	it = enabledChunks.begin();
	std::map<int, Chunk*>::iterator tmp;

	while (it != enabledChunks.end() && (chunk = (*it).second))
	{
		if (!chunk->isUsable() && chunk->isGenerated())
		{
			chunk->doOpenGLThings();
			CheckNeigbors(chunk);
		}
		if (chunk->isUsable() && glm::distance(chunk->GetPos() + glm::vec2(CHUNK_XY/2, CHUNK_XY/2), glm::vec2(g_player.GetPos().x, g_player.GetPos().z)) > VIEW_DISTANCE)
		{
			chunk->Disable();
			disabledChunks.insert(std::pair<int, Chunk*>(GetPosIndex(chunk->GetPos()), chunk));
			enabledChunks.erase(it);
			it = enabledChunks.begin();
		}
		else
			it++;
	}
}

void	ChunkHandler::LoadChunks()
{
	Chunk	*current;
	std::map<int, Chunk*>::iterator	it = enabledChunks.begin();

	while (it != enabledChunks.end() && (current = (*it).second))
	{
		if (current->isUsable() && current->HasFourNeigbors() && g_player.frustum->pointIn(glm::vec3(current->GetPos().x + CHUNK_XY/2, CHUNK_Z/2, current->GetPos().y + CHUNK_XY/2)))
		{
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, current->GetVBOID());
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, current->GetTID());
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
			glVertexAttribDivisor(1, 1);
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, current->GetCID());
			glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(GLuint), 0);
			glVertexAttribDivisor(2, 1);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, current->GetIBOID());
			glDrawElementsInstanced(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, NULL, current->GetNbInstances());
		}
		else if (current->isUsable() && !current->HasFourNeigbors())
			CheckNeigbors(current);
		it++;
	}
}

void		generateChunk(Chunk **chunk, std::mutex *mutex, MapGeneration **map)
{
	(*chunk)->generate(map, mutex);
}

void	ChunkHandler::AddChunkAtPos(int x, int y)
{
	static int id = 0;
	Chunk	*chunk;

	chunk = new Chunk(glm::vec2(x, y), id);
	id++;
	enabledChunks.insert(std::pair<int, Chunk*>(GetPosIndex(x, y), chunk));
	t = new std::thread(generateChunk, &(*(enabledChunks.find(GetPosIndex(x, y)))).second, &mutex, &mapgen);
	t->detach();
	std::cout << "Chunk called ! Nb Loaded Chunks: " << enabledChunks.size() << std::endl;
}

void	ChunkHandler::RemoveFarChunks()
{
	std::map<int, Chunk*>::iterator	it = disabledChunks.begin();
	Chunk						*chunk;

	while (it != disabledChunks.end() && (chunk = (*it).second))
	{
		if (glm::distance(chunk->GetPos(), glm::vec2(g_player.GetPos().x, g_player.GetPos().z)) > 4000.0f)
		{
			disabledChunks.erase(it);
			delete(chunk);
			it = disabledChunks.begin();
		}
		else
			it++;
	}
}

bool	ChunkHandler::CheckIfChunkAtPos(int x, int y)
{
	Chunk	*chunk;
	std::map<int, Chunk*>::iterator it;

	it = enabledChunks.find(GetPosIndex(x, y));
	if (it != enabledChunks.end())
		return true;
	it = disabledChunks.find(GetPosIndex(x, y));
	if (it != disabledChunks.end())
	{
		chunk = (*it).second;
		chunk->Enable();
		enabledChunks.insert(std::pair<int, Chunk*>(GetPosIndex(chunk->GetPos()), chunk));
		disabledChunks.erase(it);
		return true;
	}
	return false;
}

Chunk	*ChunkHandler::GetChunkAtPos(int x, int y)
{
	std::map<int, Chunk*>::iterator it;

	it = enabledChunks.find(GetPosIndex(x, y));
	if (it != enabledChunks.end())
		return (*it).second;
	it = disabledChunks.find(GetPosIndex(x, y));
	if (it != disabledChunks.end())
		return (*it).second;
	return NULL;
}
