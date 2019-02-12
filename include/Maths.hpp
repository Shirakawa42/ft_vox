#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct	s_vec3
{
	float x;
	float y;
	float z;
}				t_vec3;

typedef struct	s_vec2
{
	float x;
	float y;
}				t_vec2;

float	distance(int x1, int y1, int x2, int y2);