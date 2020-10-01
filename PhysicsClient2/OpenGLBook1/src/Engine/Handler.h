#pragma once
#include <vector>
#include "GameObject.h"
#include "GameHandler.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Renderer.h"
#include "../Connection/Connection.h"

#include "../GUI/Pages/Page.h"
#include "../GUI/Pages/GamePage.h"
#include "../GUI/Pages/MenuPage.h"
#include "../GUI/Pages/IntroPage.h"
#include "../GUI/Pages/InfoPage.h"
#include "../GUI/Pages/LoadPage.h"
#include "../GUI/Pages/ConnectPage.h"
#include "../GUI/GameHUD.h"


/// <summary>
/// Class to handle the game environment, iterates through all
/// game objects and updates/renders them. Has a connection object
/// to manage its server connection if needed
/// </summary>
/// <remarks>
/// Confirm destructor is deleting correctly
/// </remarks>
class Handler {
private:
	Page* currentPage;

	/// <summary>
	/// The renderer object that takes in input and renders
	/// using OpenGL
	/// </summary>
	Renderer* renderer;

	/// <summary>
	/// Separate handler to handle game logic
	/// </summay>
	GameHandler* gameHandler;

	/// <summary>
	/// boolean array to track which keys are pressed, updated by
	/// the window
	/// </summary>
	/// <remarks>
	/// Might be better if this was static. should be in same location
	/// as the leftClick/mouseCoords variables
	/// </remarks>
	GLboolean keys[1024];

	/// <summary>
	/// If left click is pressed.
	/// </summary>
	bool leftClick;
	bool rightClick;

	/// <summary>
	/// Tracks the mouse coordinates -1 to 1 for x and y
	/// relative to OpenGL coordinate system 
	/// </summary>
	/// <remarks>
	/// rename to mousePosition
	/// </remarks>
	glm::vec2 mouseCoords;

	/// <summary>
	/// Connection object that handles connection
	/// to a server, used to connect, send data and get data
	/// from a server
	/// </summary>
	Connection* connect;

public:
	/// <summary>
	/// Constructor that initialize the game environment count number
	/// of particles
	/// </summary>
	Handler();

	/// <summary>
	/// Destructor that deletes the renderer, player, and objects in objs
	/// </summary>
	~Handler();

	/// <summary>
	/// Updates the game environment is dt steps
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
	/// Handle the collision logic of colliding 2 gameObjects
	/// by swapping their velocities
	/// </summary>
	void collide(GameObject* obj1, GameObject* obj2);

	void initGame();

	void setCurrentPage(Page* nextPage);

	void setMousePosition(glm::vec2 position);

	void setMouseX(GLfloat xPosition);

	void setMouseY(GLfloat yPosition);

	void setRightClick(GLboolean rightClick);

	void setLeftClick(GLboolean leftClick);

	Connection* getConnection();

	bool keyPressed(int keyCode);

	void setKeyPress(int keyCode, GLboolean value);

	GameHandler* getGameHander();
};