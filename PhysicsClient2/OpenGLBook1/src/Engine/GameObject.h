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
/// Currently only renders cirlces
/// <Summary>
class GameObject {

public:
	/// <summary>
	/// The radius of the gameobject, currenty only circular gameobjects.
	/// Currently always .04
	///</summary>
	GLfloat radius;

	/// <summary>
	/// The position of the particle. Currenlty only x and y but
	/// later can add the z coordinate
	/// </summary>
	glm::vec2 position;

	/// <summary>
	/// the velocity vector of this particle
	/// </summary>
	glm::vec2 velocity;

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
	
	/// <summary>
	/// Constutor, sets the coordinates and postion,
	/// velocity is random and radius is always .04
	/// </summary>
	GameObject(GLfloat x, GLfloat y, GLboolean isPlayer);


	GameObject(GLfloat x, GLfloat y, GLboolean isPlayer, glm::vec3 color);

	/// <summary
	/// Updates particle postion based on velocity.
	/// If player then the particle's position is updated by the keys
	/// </summary>
	void tick(GLfloat dt, GLboolean keys[1024]);

	/// <summary>
	/// Calls Renderer to render a cirlce at the objects x, y
	/// </summary>
	void render(Renderer* renderer);
};

