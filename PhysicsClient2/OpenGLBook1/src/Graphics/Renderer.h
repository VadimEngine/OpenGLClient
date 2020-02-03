#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <algorithm>

#include "Shader.h"

/// <summary>
/// User Shader and OpenGL to render shapes
/// </summary>
class Renderer {

public:
	std::vector<GLfloat> vertList;
	Shader* shader;
	GLuint VAO, VBO;

	Renderer(Shader* shader);
	~Renderer();

	void addVertices(GLfloat x, GLfloat y);//not needed, can just add cirlce
	void clear();
	void Draw();


	void initRenderData();

	void renderText(std::string theString, float x, float y);

	void renderTriange(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

	//x y is the top left corner
	void renderRectangle(float x, float y, float width, float height);

	void renderCircle(float x, float y, float radius, int sides);

	void renderPolygon(glm::vec3* points);


};
