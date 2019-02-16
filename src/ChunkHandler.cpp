#include "ChunkHandler.hpp"

ChunkHandler::ChunkHandler()
{
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
	DisableChunks();
}

void	ChunkHandler::DisableChunks()
{
	Chunk	*chunk;
	int		i;

	i = 0;
	while (i < enabledChunks.size() && (chunk = enabledChunks[i]))
	{
		if (glm::distance(chunk->GetPos() + glm::vec2(CHUNK_XY/2, CHUNK_XY/2), glm::vec2(g_player.GetPos().x, g_player.GetPos().z)) > VIEW_DISTANCE * 2.0f)
		{
			chunk->Disable();
			disabledChunks.push_back(chunk);
			enabledChunks.erase(enabledChunks.begin() + i);
		}
		else
			i++;
	}
}

void	ChunkHandler::LoadChunks()
{
	int		i;
	Chunk	*current;

	i = 0;
	while (i < enabledChunks.size() && (current = enabledChunks[i]))
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
		i++;
	}
}

void	ChunkHandler::AddChunkAtPos(int x, int y)
{
	Chunk	*chunk;

	chunk = new Chunk(glm::vec2(x, y), &mapgen);
	enabledChunks.push_back(chunk);
	std::cout << "Chunk generated ! Nb Loaded Chunks: " << enabledChunks.size() << std::endl;
}

bool	ChunkHandler::CheckIfChunkAtPos(int x, int y)
{
	int		i;
	Chunk	*chunk;

	i = 0;
	while (i < enabledChunks.size() && (chunk = enabledChunks[i]))
	{
		if (chunk->GetPos() == glm::vec2(x, y))
			return true;
		i++;
	}
	i = 0;
	while (i < disabledChunks.size() && (chunk = disabledChunks[i]))
	{
		if (chunk->GetPos() == glm::vec2(x, y))
		{
			chunk->Enable();
			enabledChunks.push_back(chunk);
			disabledChunks.erase(disabledChunks.begin() + i);
			std::cout << "Nb Loaded Chunks: " << enabledChunks.size() << std::endl;
			return true;
		}
		else
			i++;
	}
	return false;
}