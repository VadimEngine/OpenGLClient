#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <algorithm>

#include "SandboxShader.h"


class Renderer {

public:
	std::vector<GLfloat> vertList;
	SandboxShader* shader;
	GLuint VAO, VBO;

	GLuint texture1;

	Renderer(SandboxShader* shader);
	~Renderer();

	void addVertices(GLfloat x, GLfloat y);
	void clear();
	void Draw();//, glm::vec3 color


	void initRenderData();

	void renderText(std::string theString, float x, float y);

	//x y is the top left corner
	void renderRectangle(float x, float y, float width, float height);

	void renderCircle(float x, float y, float radius, int sides);

};
