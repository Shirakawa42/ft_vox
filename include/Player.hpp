#pragma once

# define FOV 45.0f
# define VIEW_DISTANCE 200.0f

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Maths.hpp"

class Player
{
	public:
		Player();
		~Player();
		glm::vec3		GetPos();
		glm::mat4		mvp;
		float			deltaTime;
		void			mouseControl(GLFWwindow *window);
	private:
		glm::mat4		Projection;
		glm::mat4		View;
		glm::mat4		Model;
		glm::vec3		position;
		float			horizontalAngle;
		float			verticalAngle;
		float			initialFoV;
		float			speed;
		float			mouseSpeed;
};