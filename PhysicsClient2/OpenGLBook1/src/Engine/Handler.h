#pragma once
#include <vector>
#include "GameObject.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Renderer.h"
//#include "../Connection/ConnectionTCP.h"
//#include "../Connection/ConnectionUDP.h"
#include "../Connection/Connection.h"


/// <summary>
/// Class to handle the game enviornment, iterates through all
/// game objects and updates/renders them. Has a connection object
/// to manage its server connection if needed
/// </summary>
/// <remarks>
/// Confirm destructor is deleting correctly
/// </remarks>
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
	/// boolean array to track which keys are pressed, updated by
	/// the window
	/// </summary>
	/// <remarks>
	/// Might be better if this was static. should be in same location
	/// as the leftclick/mouseCoords variables
	/// </remarks>
	GLboolean keys[1024];

	/// <summary>
	/// If left click is pressed.
	/// </summary
	bool leftClick;

	/// <summary>
	/// Tracks the mouse coordinates -1 to 1 for x and y
	/// relative to OpenGL coordinate system 
	/// </summary
	glm::vec2 mouseCoords;

	Connection* connect;

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

	/// <summary>
	/// Promts the user to decide on the mode the application will run on.
	/// options are 1) serverless 2)TCP 3)UDP. If connection cannot be
	/// established or inavlid input is set then the user will be continually
	/// promted until success.
	/// </summary>
	bool connectionProtocol();

	/// <summary>
	/// Closes down the TCP connection
	/// </summary>
	void TCPClose();

	/// <summary>
	/// Closes down the UPD conneciton
	/// </summary>
	void UDPClose();
};