#pragma once
#include <vector>
#include "GameObject.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Renderer.h"


/// <summary>
/// Class to handle the game enviornment, iterates through all
/// game objects and updates/renders them
/// </summary>
class Handler {
public:
	/// <summary>
	/// The player objects that the user controls
	/// </summary>
	GameObject* player;

	/// <summary>
	/// The renderer object that takes in input and renders
	/// using OpenGL
	/// </summary>
	Renderer* renderer;

	/// <summary>
	/// List of all objects in the game enviornment. This handler
	/// iterates all to update/render them
	/// </summary>
	std::vector<GameObject*> objs;

	/// <summary>
	/// If the game is running is server-mode
	/// </summary>
	bool isServer;

	/// <summary>
	/// boolean array to track which keys are pressed, updated by
	/// the window
	/// </summary>
	/// <remarks>
	/// Might be better if this was static
	/// </remarks>
	GLboolean keys[1024];

public:
	/// <summary>
	/// Constructor that initialize the game enviornment count number
	/// of particles
	/// </summary>
	Handler(int count);

	/// <summary>
	/// Destructor that deletes the renderer, player, and objects in objs
	/// </summary>
	~Handler();

	/// <summary>
	/// Updates the game envoirnment is dt steps
	/// </summary>
	void tick(GLfloat dt);

	/// <summary>
	/// Render the game environment
	/// </summary>
	void render();

	/// <summary>
	/// Add a GameObject to the handler
	/// </summary>
	void addObj(GameObject* obj);

	/// <summary>
	/// Handle the collision logic of colliding 2 gameobjects
	/// by swapping their velocities
	/// </summary>
	void collide(GameObject* obj1, GameObject* obj2);
};