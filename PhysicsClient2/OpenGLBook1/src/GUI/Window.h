#pragma once
#include <iostream>
#include "../Engine/Handler.h"

/// <summary>
/// IHandles the window logic for openGL, tells handler to render/update
/// </summary>
/// <remarks>
/// Tracks user inut should be static and only in 1 location. Maybe in handler
/// </remarks>
class Window {
public:
	/// <summary>
	/// If the left mouse button is pressed
	/// </summary>
	/// <remark>
	/// Change logic in outside code to not set it to false
	/// when the action happens, instead summon an action on the
	/// inital click/relase
	/// </remark>
	GLboolean leftClick;

	//GLfloat mouseX;//-1.0 to 1.0
	//GLfloat mouseY;//Vector?

	/// <summary
	/// Holds the mouse position
	/// </summary>
	glm::vec2 mousePosition;

	/// <summary>
	/// The OpenGl window that holds all the rendering.
	/// </summary>
	GLFWwindow* window;

	/// <summary>
	/// The handler object that handles the game enviornment
	/// </summary>
	Handler* handler;

	/// <summary>
	/// Constructor that generates a window with the given dimensions
	/// and with count number of particles
	/// </summary>
	Window(GLuint width, GLuint height, GLuint count);

	/// <summary>
	/// Destructor that deletes the window and the handler.
	/// </summary>
	~Window();

	/// <summary>
	/// If the window should, such as if the player has pressed the close button
	/// </summary>
	GLboolean shouldClose();

	/// <summary>
	/// Updates the game, expected to be called 60 times a second
	/// </summary>
	/// <remarks>
	/// Should take in a dt, maybe in here or the handler dt it can
	/// pull data from server? Or maybe server data pulling should be
	/// done a seperate thread
	/// </remarks>
	void update();

	/// <summary>
	/// Draws the game data with OpenGL, calls handler->render()
	/// <summary>
	void render();

	/// <summary>
	/// Tells handler to close down it connection
	/// </summary
	void closeConnection();
};