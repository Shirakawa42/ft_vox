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
	int		k;

	i = -VIEW_DISTANCE + g_player.GetPos().x;
	while (i < VIEW_DISTANCE + g_player.GetPos().x)
	{
		j = -VIEW_DISTANCE + g_player.GetPos().z;
		while (j < VIEW_DISTANCE + g_player.GetPos().z)
		{
			x = i - i % CHUNK_XY;
			y = j - j % CHUNK_XY;
			if (!CheckIfChunkAtPos(x, y))
				AddChunkAtPos(x, y);
			j += CHUNK_XY;
		}
		i += CHUNK_XY;
	}
	std::list<Chunk*>::iterator	it;
	for (it = enabledChunks.begin(); it != enabledChunks.end(); ++it)
	{
		if (!(*it)->isUsable() && (*it)->isGenerated())
			(*it)->doOpenGLThings();
	}
	DisableChunks();
}

void	ChunkHandler::DisableChunks()
{
	Chunk	*chunk;
	std::list<Chunk*>::iterator	it = enabledChunks.begin();

	while (it != enabledChunks.end() && (chunk = *it))
	{
		if (chunk->isUsable() && glm::distance(chunk->GetPos() + glm::vec2(CHUNK_XY/2, CHUNK_XY/2), glm::vec2(g_player.GetPos().x, g_player.GetPos().z)) > VIEW_DISTANCE * 2.0f)
		{
			chunk->Disable();
			disabledChunks.push_back(chunk);
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
	std::list<Chunk*>::iterator	it = enabledChunks.begin();

	while (it != enabledChunks.end() && (current = *it))
	{
		if (current->isUsable() && g_player.frustum->pointIn(glm::vec3(current->GetPos().x + CHUNK_XY/2, CHUNK_Z/2, current->GetPos().y + CHUNK_XY/2)))
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
		it++;
	}
}

static void		generateChunk(Chunk **chunk, std::mutex *mutex, MapGeneration **map)
{
	(*chunk)->generate(map, mutex);
}

void	ChunkHandler::AddChunkAtPos(int x, int y)
{
	static int id = 0;
	Chunk	*chunk;

	chunk = new Chunk(glm::vec2(x, y), id);
	id++;
	enabledChunks.push_back(chunk);
	t = new std::thread(generateChunk, &(enabledChunks.back()), &mutex, &mapgen);
	t->detach();
	std::cout << "Chunk called ! Nb Loaded Chunks: " << enabledChunks.size() << std::endl;
}

bool	ChunkHandler::CheckIfChunkAtPos(int x, int y)
{
	Chunk	*chunk;
	std::list<Chunk*>::iterator	it = enabledChunks.begin();

	while (it != enabledChunks.end() && (chunk = *it))
	{
		if (chunk->GetPos() == glm::vec2(x, y))
			return true;
		it++;
	}
	it = disabledChunks.begin();
	while (it != disabledChunks.end() && (chunk = *it))
	{
		if (chunk->GetPos() == glm::vec2(x, y))
		{
			chunk->Enable();
			enabledChunks.push_back(chunk);
			disabledChunks.erase(it);
			return true;
		}
		else if (glm::distance((*it)->GetPos(), glm::vec2(g_player.GetPos().x, g_player.GetPos().z)) > 5000.0f)
		{
			disabledChunks.erase(it);
			delete(chunk);
			it = disabledChunks.begin();
			std::cout << "removing chunk" << std::endl;
		}
		else
			it++;
	}
	return false;
}