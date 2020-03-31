#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "Shader.h"
#include <unordered_map>


/// <summary>
/// Render class to handle drawing the game. Uses Shader and OpenGL
/// to render shapes
/// </summary>
class Renderer {
public:
	/// <summary>
	/// Shader class that is used to render the data
	/// </summary>
	Shader* polygonShader;

	Shader* imageShader;

	/// <summary>
	///	Vertex array object and vertex buffer object
	/// used to store rendering data that is passed to GPU
	/// </summary>
	GLuint VAO, VBO;

	GLuint polyVAO, polyVBO;

	GLuint texture;

	std::unordered_map<char, glm::vec2> charKeyMap;

	/// <summary>
	/// List of coordinates {x_1, y_1, z_1, ... x_n, y_n, z_n} 
	// added to render
	/// </summary>
	/// <remarks>
	/// label as for triangles.
	/// <remarks>
	std::vector<GLfloat> vertList;

	std::vector<GLfloat> otherVertList;

	/// <summary>
	/// Constuctor that takes in a Shader to use for rendering
	/// </summary>
	Renderer(Shader* polygonShader, Shader* imageShader);

	/// <summary>
	/// Destrucotr that clears the verList and deletes the Shader
	/// </summary>
	~Renderer();

	/// <summary>
	/// Initialize the OpenGL data. Creates and sets the 
	/// VAO (Vertex array object) and VBO (Vertex buffer object).
	/// </summary>
	void initRenderData();

	/// <summary>
	/// Does the actual draw of the vertList and then clears
	// the list one the draw is done
	/// </summary>
	void Draw();

	/// <summary>
	/// Clears the vertList
	/// </summary>
	void clear();

	void addVertexAndColor(glm::vec3 point, glm::vec3 color);
	
	void renderLineColor(glm::vec3 p1, glm::vec3 p2, glm::vec3 color, float width);

	void renderTriangleColor(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color);
	
	void renderRectangleColor(glm::vec3 topLeft, float width, float height, glm::vec3 color);
	
	void renderCircleColor(glm::vec3 position, glm::vec3 color, float radius, float sides);
	

	/// <summary>
	/// Place holder method, not yet implemented. Intended to draw 
	/// the given image at a location
	/// </summary>
	void renderImage(glm::vec3 position, glm::vec3 color);

	void renderSubImage(glm::vec3 position, glm::vec3 color);

	void renderSubImageAdvanced(glm::vec3 position,
								glm::vec3 color,
								float texWidth,
								float texHeight,
								float subWidth,
								float subHeight,
								float subX,
								float subY);

	void renderChar(char theChar, glm::vec3 position, glm::vec3 color, float size);

	/// <summary>
	/// Place holder method, not yet implemented. Intended to draw
	/// to draw the given string at a location. Will work by mapping
	/// character images from a spritesheet of letters by their character
	/// code and draw the sub image at a location. Will first use a simple
	/// font with same character sizes
	/// </summary>
	void renderString(std::string theString, float x, float y);
};