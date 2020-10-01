#pragma once
#define GLEW_STATIC
//why is this here?
#include <GL/glew.h>//include GLEW before glfw
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../Graphics/Renderer.h"


/// <Summary>
/// Game objects that will update with the game and be rendered.
/// Currently only renders circles
/// <Summary>
class GameObject {

private:
	/// <summary>
	/// The radius of the gameObject, current only circular gameObjects.
	/// Currently always .04
	///</summary>
	GLfloat radius;

	/// <summary>
	/// If the following particle is a player particle.
	/// Remove this field
	/// </summary>
	GLboolean isPlayer;

	/// <summary>
	/// Last position of the objects, used by player object to
	/// calculate its velocity
	/// </summary>
	glm::vec2 lastPosition;

	///Color
	glm::vec3 color;

	/// <summary>
	/// The speed that the player particle moves 
	/// </summary>
	GLfloat speed;

	int id;

public:
	/// <summary>
	/// The position of the particle. Currently only x and y but
	/// later can add the z coordinate
	/// </summary>
	glm::vec2 position;

	/// <summary>
	/// the velocity vector of this particle
	/// </summary>
	glm::vec2 velocity;



public:
	/// <summary>
	/// Constructor, sets the coordinates and position,
	/// velocity is random and radius is always .04
	/// </summary>
	GameObject(GLfloat x, GLfloat y, GLboolean isPlayer);


	GameObject(GLfloat x, GLfloat y, GLboolean isPlayer, glm::vec3 color);

	~GameObject();

	/// <summary
	/// Updates particle position based on velocity.
	/// If player then the particle's position is updated by the keys
	/// </summary>
	void tick(GLfloat dt, GLboolean keys[1024]);

	/// <summary>
	/// Calls Renderer to render a circle at the objects x, y
	/// </summary>
	void render(Renderer* renderer);

	int getId();

	GLfloat getRadius();

	void setId(int id);
};

