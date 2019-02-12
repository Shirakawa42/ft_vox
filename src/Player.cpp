#include "Player.hpp"

#include <iostream>

Player::Player()
{
	pos.x = 0.0f;
	pos.y = 0.0f;
	pos.z = 5.0f;
	speed = 0.5f;
	Projection = glm::perspective(glm::radians(45.0f), (float)1000 / (float)1000, 0.1f, 100.0f);
	View = glm::lookAt(
			glm::vec3(4,6,3),
			glm::vec3(0,1,0),
			glm::vec3(0,1,0)
		);
	Model = glm::mat4(1.0f);
	mvp = Projection * View * Model;
	position = glm::vec3(0,0,5);
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	initialFoV = FOV;
	speed = 3.0f;
	mouseSpeed = 0.05f;
}

Player::~Player()
{

}

void	Player::mouseControl(GLFWwindow *window)
{
	double x;
	double y;
	glfwGetCursorPos(window, &x, &y);
	horizontalAngle += mouseSpeed * deltaTime * float(1000/2 - x);
	verticalAngle += mouseSpeed * deltaTime * float(1000/2 - y);
	glm::vec3 direction(
    	cos(verticalAngle) * sin(horizontalAngle),
   		sin(verticalAngle),
   		cos(verticalAngle) * cos(horizontalAngle)
	);
	glm::vec3 right = glm::vec3(
    	sin(horizontalAngle - 3.14f/2.0f),
    	0,
    	cos(horizontalAngle - 3.14f/2.0f)
	);
	glm::vec3 up = glm::cross(right, direction);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	    position += direction * deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	    position -= direction * deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	    position += right * deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	    position -= right * deltaTime * speed;
	View = glm::lookAt(position, position+direction, up);
	mvp = Projection * View * Model;
	glfwSetCursorPos(window, 1000/2, 1000/2);
}

t_vec3	Player::GetPos()
{
	return pos;
}