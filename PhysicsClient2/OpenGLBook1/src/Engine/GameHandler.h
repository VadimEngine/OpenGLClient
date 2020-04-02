#pragma once
#include "GameObject.h"

class GameHandler {

public:
	GameObject* player;

	std::vector<GameObject*> objs;

	bool server;


	GameHandler(bool server);

	~GameHandler();

	void tick(GLfloat dt, bool LeftClick, GLboolean* keys);

	void render(Renderer* renderer);

	void collide(GameObject* obj1, GameObject* obj2);

	void addObj(GameObject* obj);

};