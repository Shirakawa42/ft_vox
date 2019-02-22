#pragma once

#include <cstdlib>
#include <ctime>
#include <cmath>

class MapGeneration
{
	public:
		MapGeneration();
		MapGeneration(unsigned int seed);
		~MapGeneration();
		float	noise(float x, float y, float z);
	private:
		int				*p;
		unsigned int	seed;
};