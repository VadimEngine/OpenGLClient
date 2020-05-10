#pragma once
#include "GameObject.h"
#include <map>
#include <iterator>
#include "../Connection/Connection.h"

class Handler;

class GameHandler {

private:
	/// <summary>
	/// The player objects that the user controls
	/// </summary>
	GameObject* player;

	GameObject* selectedObj;

	Handler* theHandler;

	/// <summary>
	/// List of all objects in the game enviornment. This handler
	/// iterates all to update/render them
	/// </summary>
	std::vector<GameObject*> objs;

	//ordered map so the next key can be found
	std::map<int, GameObject*> serverObjs;
	//use unsined int nad set 0 as no pointer?
	int selectedId;

	bool gravity;
	bool paused;
public:
	GameHandler(Handler* theHandler);

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

	void clearServerObjs();

	GameObject* getPlayer();

	GameObject* getSelectedObj();

	int getSelectedId();

	bool isPaused();
	bool gravityEnabled();

	std::vector<GameObject*>* getObjs();

	std::map<int, GameObject*>* getServerObjs();
};