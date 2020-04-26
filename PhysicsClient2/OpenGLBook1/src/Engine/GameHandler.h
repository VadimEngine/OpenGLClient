#pragma once
#include "GameObject.h"
#include <map>
#include <iterator>
#include "../Connection/Connection.h"

class GameHandler {

public:


	Connection* theConnection;
	/// <summary>
	/// The player objects that the user controls
	/// </summary>
	GameObject* player;

	GameObject* selectedObj;

	/// <summary>
	/// List of all objects in the game enviornment. This handler
	/// iterates all to update/render them
	/// </summary>
	std::vector<GameObject*> objs;

	//ordered map so the next key can be found
	std::map<int, GameObject*> serverObjs;
	//use unsined int nad set 0 as no pointer?
	int selectedId;

	//not needed? can be derived from connection object
	bool server;

	bool gravity;
	bool paused;




	GameHandler(bool server, Connection* theConnection);

	~GameHandler();

	void tick(GLfloat dt, bool LeftClick, GLboolean* keys);

	void render(Renderer* renderer);

	void collide(GameObject* obj1, GameObject* obj2);

	void addObj(GameObject* obj);

	void mouseClick(glm::vec2 mouseCoord, int button);

	//going for an inefficient approach for now, better approuch would be to save the index?
	void selectNext();

	void removeParticle(GameObject* obj);



	void removeParticleServer(int theID);
	void selectNextServer();
	void addParticleServer(GameObject* obj, int id);


	void setPause(bool pause);

	void setGravity(bool gravity);
};