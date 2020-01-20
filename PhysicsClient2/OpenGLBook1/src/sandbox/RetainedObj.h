#pragma once

//probably include header issues here
#define GLEW_STATIC
#include <GL/glew.h>//include GLEW before glfw
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <iostream>
#include <string>
#include <math.h>
#include<cmath>

#include "SandboxShader.h"
#include "RetainedRenderer.h"


class RetainedObj {

public:
	GLfloat x, y;
	glm::vec3 color;

	glm::vec2 position;
	glm::vec2 velocity;

	glm::vec2 lastPosition;

	GLboolean isPlayer;


	RetainedObj(GLfloat x, GLfloat y, glm::vec3 color, SandboxShader* myShader, GLboolean isPlayer);

	void tick(GLfloat dt, GLboolean keys[1024]);

	void render(RetainedRenderer* renderer);

	void renderCircle(RetainedRenderer* renderer);

};
