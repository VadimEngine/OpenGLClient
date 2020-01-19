#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <algorithm>

#include "SandboxShader.h"


class RetainedRenderer {

public:
	std::vector<GLfloat> vertList;
	SandboxShader* shader;
	GLuint VAO, VBO;

	RetainedRenderer(SandboxShader* shader);
	~RetainedRenderer();

	void addVertices(GLfloat x, GLfloat y);
	void clear();
	void Draw();//, glm::vec3 color


	void initRenderData();


};
