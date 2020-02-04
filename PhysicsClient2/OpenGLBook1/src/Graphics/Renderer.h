#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "Shader.h"


/// <summary>
/// Render class to handle drawing the game. Uses Shader and OpenGL
/// to render shapes
/// </summary>
class Renderer {
public:
	/// <summary>
	/// Shader class that is used to render the data
	/// </summary>
	Shader* shader;

	/// <summary>
	///	Vertex array object and vertex buffer object
	/// used to store rendering data that is passed to GPU
	/// </summary>
	GLuint VAO, VBO;

	/// <summary>
	/// List of coordinates {x_1, y_1, z_1, ... x_n, y_n, z_n} 
	// added to render
	/// </summary>
	std::vector<GLfloat> vertList;

	/// <summary>
	/// Constuctor that takes in a Shader to use for rendering
	/// </summary>
	Renderer(Shader* shader);

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

	/// <summary>
	/// Adds coordinates to the vertList at z=0 for rendering
	/// </summary>
	void addVertices(GLfloat x, GLfloat y);

	/// <summary>
	/// Draws a line from p1 to p2
	/// </summary>
	void drawLine(glm::vec3 p1, glm::vec3 p2);

	/// <summary>
	/// Draws a triangle with the given corners. Should be counter-clockwise
	/// </summary>
	void renderTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

	/// <summary>
	/// Adds to the verList the corrdinates for a rectangle with the 
	/// given dimensions and location. Coorinates are added counter clock wise
	/// to the vertList. x, y is the top left corner
	/// </summary>
	void renderRectangle(float x, float y, float width, float height);

	/// <summary>
	/// Draws a cirlce with its center at (x,y 0) with given radius size.
	/// the sides determines how "Round" the cirlce is
	/// </summary>
	void renderCircle(float x, float y, float radius, int sides);

	/// <summary>
	/// Place holder method, no yet implemented. Intended to draww
	// a polygon with the given points
	/// </summary>
	void renderPolygon(glm::vec3* points);

	/// <summary>
	/// Place holder method, not yet implemented. Intended to draw 
	/// the given image at a location
	/// </summary>
	void renderImage();

	/// <summary>
	/// Place holder method, not yet implemented. Intended to draw
	/// to draw the given string at a location. Will work by mapping
	/// character images from a spritesheet of letters by their character
	/// code and draw the sub image at a location. Will first use a simple
	/// font with same character sizes
	/// </summary>
	void renderText(std::string theString, float x, float y);
};